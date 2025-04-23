#include "StdAfx.hpp"

struct _X32Module
{
	PVOID ImageBase;
	PIMAGE_NT_HEADERS NtHeaders;
	PIMAGE_BASE_RELOCATION BaseRelocation;
	PIMAGE_IMPORT_DESCRIPTOR ImportDirectory;

}; _X32Module m_X32ModuleStruct;

typedef BOOL( WINAPI *PDLL_MAIN )( HMODULE , DWORD , PVOID );

int m_iNumberOfModulesLoaded = 0;
LPVOID m_dwLoadedModules[10];

unsigned long X32_DetachACModule( PVOID pvModuleBase )
{
	PDLL_MAIN EntryPoint;
	PIMAGE_DOS_HEADER pIDH;
	PIMAGE_NT_HEADERS pINH;

	pIDH = ( PIMAGE_DOS_HEADER ) pvModuleBase;
	pINH = ( PIMAGE_NT_HEADERS ) ( ( LPBYTE ) pvModuleBase + pIDH->e_lfanew );

	if( pIDH->e_magic != IMAGE_DOS_SIGNATURE )
		return 2;

	if( pINH->Signature != IMAGE_NT_SIGNATURE )
		return 2;

	memset( &m_X32ModuleStruct , 0 , sizeof( m_X32ModuleStruct ) );
	m_X32ModuleStruct.ImageBase = ( PVOID ) pvModuleBase;
	m_X32ModuleStruct.NtHeaders = ( PIMAGE_NT_HEADERS ) ( ( LPBYTE ) pvModuleBase + pIDH->e_lfanew );
	m_X32ModuleStruct.BaseRelocation = ( PIMAGE_BASE_RELOCATION ) ( ( LPBYTE ) pvModuleBase + pINH->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_BASERELOC ].VirtualAddress );
	m_X32ModuleStruct.ImportDirectory = ( PIMAGE_IMPORT_DESCRIPTOR ) ( ( LPBYTE ) pvModuleBase + pINH->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_IMPORT ].VirtualAddress );

	EntryPoint = ( PDLL_MAIN ) ( ( LPBYTE ) m_X32ModuleStruct.ImageBase + m_X32ModuleStruct.NtHeaders->OptionalHeader.AddressOfEntryPoint );

	return EntryPoint( ( HMODULE ) m_X32ModuleStruct.ImageBase , DLL_PROCESS_DETACH , NULL );
}
VOID X32_UnloadAllACModules( )
{
	NTSTATUS m_NTStatus;
	SIZE_T m_pvSize = 0;

	if( m_iNumberOfModulesLoaded )
	{
		for( int i = 0; i < m_iNumberOfModulesLoaded; i++ )
		{
			X32_DetachACModule( m_dwLoadedModules[ i ] );
			m_NTStatus = m_cCore.fnNtFreeVirtualMemory( ( HANDLE ) -1 , &m_dwLoadedModules[ i ] , &m_pvSize , MEM_RELEASE );
		}

		m_iNumberOfModulesLoaded = 0;
		memset( m_dwLoadedModules , 0 , sizeof( m_dwLoadedModules ) );
	}
}
unsigned long LoadDll( _X32Module *pModuleStruct )
{
	DWORD i , Function , count , delta;

	HMODULE hModule;
	DWORD* ptr;
	WORD* list;

	PIMAGE_BASE_RELOCATION pIBR;
	PIMAGE_IMPORT_DESCRIPTOR pIID;
	PIMAGE_IMPORT_BY_NAME pIBN;
	PIMAGE_THUNK_DATA FirstThunk , OrigFirstThunk;

	PDLL_MAIN EntryPoint;

	pIBR = pModuleStruct->BaseRelocation;
	delta = ( DWORD ) ( ( LPBYTE ) pModuleStruct->ImageBase - pModuleStruct->NtHeaders->OptionalHeader.ImageBase ); // Calculate the delta

	// Relocate the image

	while( pIBR->VirtualAddress )
	{
		if( pIBR->SizeOfBlock >= sizeof( IMAGE_BASE_RELOCATION ) )
		{
			count = ( pIBR->SizeOfBlock - sizeof( IMAGE_BASE_RELOCATION ) ) / sizeof( WORD );
			list = ( PWORD ) ( pIBR + 1 );

			for( i = 0; i<count; i++ )
			{
				if( list[ i ] )
				{
					ptr = ( PDWORD ) ( ( LPBYTE ) pModuleStruct->ImageBase + ( pIBR->VirtualAddress + ( list[ i ] & 0xFFF ) ) );
					*ptr += delta;
				}
			}
		}

		pIBR = ( PIMAGE_BASE_RELOCATION ) ( ( LPBYTE ) pIBR + pIBR->SizeOfBlock );
	}

	pIID = pModuleStruct->ImportDirectory;

	// Resolve DLL imports

	while( pIID->Characteristics )
	{
		OrigFirstThunk = ( PIMAGE_THUNK_DATA ) ( ( LPBYTE ) pModuleStruct->ImageBase + pIID->OriginalFirstThunk );
		FirstThunk = ( PIMAGE_THUNK_DATA ) ( ( LPBYTE ) pModuleStruct->ImageBase + pIID->FirstThunk );


		hModule = NUtilities::GetLocalModuleHandle( ( LPCSTR ) pModuleStruct->ImageBase + pIID->Name );
		if( !hModule )
		{
			hModule = LoadLibraryA( ( LPCSTR ) pModuleStruct->ImageBase + pIID->Name );
			if(!hModule )
				return FALSE;
		}

		while( OrigFirstThunk->u1.AddressOfData )
		{
			if( OrigFirstThunk->u1.Ordinal & IMAGE_ORDINAL_FLAG )
			{
				// Import by ordinal
				
				Function = ( DWORD ) NUtilities::GetProcAddress( hModule , ( LPCSTR ) ( OrigFirstThunk->u1.Ordinal & 0xFFFF ) );

				if( !Function )
				{
					return FALSE;
				}

				FirstThunk->u1.Function = Function;
			}

			else
			{
				// Import by name

				pIBN = ( PIMAGE_IMPORT_BY_NAME ) ( ( LPBYTE ) pModuleStruct->ImageBase + OrigFirstThunk->u1.AddressOfData );
				Function = ( DWORD ) NUtilities::GetProcAddress( hModule , ( LPCSTR ) pIBN->Name );

				if( !Function )
				{
					return FALSE;
				}

				FirstThunk->u1.Function = Function;
			}

			OrigFirstThunk++;
			FirstThunk++;
		}

		pIID++;
	}

	if( pModuleStruct->NtHeaders->OptionalHeader.AddressOfEntryPoint )
	{
		EntryPoint = ( PDLL_MAIN ) ( ( LPBYTE ) pModuleStruct->ImageBase + pModuleStruct->NtHeaders->OptionalHeader.AddressOfEntryPoint );

		// Add module base to array of loaded modules
		m_dwLoadedModules[ m_iNumberOfModulesLoaded ] = pModuleStruct->ImageBase;
		m_iNumberOfModulesLoaded++;

		// Jmp to the entry point
		return EntryPoint( ( HMODULE ) pModuleStruct->ImageBase , DLL_PROCESS_ATTACH , NULL );
		
		
	}

	return TRUE;
}

LPVOID CLoader::x32ManualMapDLL( PVOID pvModuleBase)
{
	PIMAGE_DOS_HEADER pIDH;
	PIMAGE_NT_HEADERS pINH;
	PIMAGE_SECTION_HEADER pISH;

	HANDLE hThread = 0;
	PVOID m_pvRemoteModuleBase = 0;
	DWORD m_dwExitCode = 0;

	pIDH = ( PIMAGE_DOS_HEADER ) pvModuleBase;
	pINH = ( PIMAGE_NT_HEADERS ) ( ( LPBYTE ) pvModuleBase + pIDH->e_lfanew );

	if( pIDH->e_magic != IMAGE_DOS_SIGNATURE )
		return false;

	if( pINH->Signature != IMAGE_NT_SIGNATURE )
		return false;

	m_pvRemoteModuleBase = VirtualAlloc( NULL , pINH->OptionalHeader.SizeOfImage , MEM_COMMIT | MEM_RESERVE , PAGE_EXECUTE_READWRITE ); // Allocate memory for the DLL
	if( !m_pvRemoteModuleBase )
		goto end;

	if( !NUtilities::WriteMemory( m_pvRemoteModuleBase , pvModuleBase , pINH->OptionalHeader.SizeOfHeaders ) )
	{
		VirtualFree( m_pvRemoteModuleBase , 0 , MEM_RELEASE );
		goto end;
	}

	pISH = ( PIMAGE_SECTION_HEADER ) ( pINH + 1 );

	for( int i = 0; i<pINH->FileHeader.NumberOfSections; i++ )
		NUtilities::WriteMemory( ( PVOID ) ( ( LPBYTE ) m_pvRemoteModuleBase + pISH[ i ].VirtualAddress ) , ( PVOID ) ( ( LPBYTE ) pvModuleBase + pISH[ i ].PointerToRawData ) , pISH[ i ].SizeOfRawData);

	m_cLoader.PatchModuleOffsets( m_pvRemoteModuleBase );

	memset( &m_X32ModuleStruct , 0 , sizeof( m_X32ModuleStruct ) );
	m_X32ModuleStruct.ImageBase = m_pvRemoteModuleBase;
	m_X32ModuleStruct.NtHeaders = ( PIMAGE_NT_HEADERS ) ( ( LPBYTE ) m_pvRemoteModuleBase + pIDH->e_lfanew );
	m_X32ModuleStruct.BaseRelocation = ( PIMAGE_BASE_RELOCATION ) ( ( LPBYTE ) m_pvRemoteModuleBase + pINH->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_BASERELOC ].VirtualAddress );
	m_X32ModuleStruct.ImportDirectory = ( PIMAGE_IMPORT_DESCRIPTOR ) ( ( LPBYTE ) m_pvRemoteModuleBase + pINH->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_IMPORT ].VirtualAddress );
	hThread = CreateThread( 0 , 0 , ( LPTHREAD_START_ROUTINE ) LoadDll , &m_X32ModuleStruct , 0 , 0 ); // Create a remote thread to execute the loader code
	if( !hThread ) 
	{
		VirtualFree( m_pvRemoteModuleBase , 0 , MEM_RELEASE );
		goto end;
	}

	WaitForSingleObject( hThread , INFINITE );
	GetExitCodeThread( hThread , &m_dwExitCode );

end:

	CloseHandle( hThread );
	return m_pvRemoteModuleBase;
}
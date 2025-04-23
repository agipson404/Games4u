#include "StdAfx.hpp"

CLoader m_cLoader;
BOOL CLoader::PatchModuleOffsets( PVOID pvModule )
{
	PIMAGE_DOS_HEADER pIDH = ( PIMAGE_DOS_HEADER ) pvModule;
	PIMAGE_NT_HEADERS pINH = ( PIMAGE_NT_HEADERS ) ( ( LPBYTE ) pvModule + pIDH->e_lfanew );

	DWORD m_Offset = ( DWORD ) pvModule;

	while( m_Offset != m_Offset + pINH->OptionalHeader.SizeOfImage )
	{
		if( *( DWORD* ) ( m_Offset ) == 0xFFAAEEFF )
		{
			*( DWORD* ) m_Offset = ( DWORD ) NUtilities::GetProcAddress;
		}
		else if( *( DWORD* ) ( m_Offset ) == 0xFF1199AA )
		{
			*( DWORD* ) m_Offset = ( DWORD ) NUtilities::GetLocalModuleHandle;
		}
		else if( *( DWORD* ) ( m_Offset ) == 0xFFAA33FF )
		{
			*( DWORD* ) m_Offset = ( DWORD ) &CNetwork::SendData;
		}
		else if( *( DWORD* ) ( m_Offset ) == 0xFFAA3322 )
		{
			*( DWORD* ) m_Offset = ( DWORD ) NUtilities::ErrorMessage;
		}
		else if( *( DWORD* ) ( m_Offset ) == 0xFF331122 )
		{
			*( DWORD* ) m_Offset = ( DWORD ) NUtilities::GenerateRandomNumber;
		}
		else if( *( DWORD* ) ( m_Offset ) == 0x72948529 )
		{
			*( DWORD* ) m_Offset = ( DWORD )m_cCore.fnNtClose;
		}
		else if( *( DWORD* ) ( m_Offset ) == 0x92849503 )
		{
			*( DWORD* ) m_Offset = ( DWORD ) m_cCore.fnNtFreeVirtualMemory;
			break;
		}
		
		m_Offset++;
	}
	return true;
}

BOOL CLoader::Initiailize( )
{
	if( !m_cLoader.GetHWID( m_cLoader.m_ClientHWID ) )
		return false;

	if( m_cLoader.m_ClientHWID.length( ) < 6 )
		return false;

	return true;
}
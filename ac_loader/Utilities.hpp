#pragma once

template <typename Type , int N>
inline int numElementsInArray( Type( &array )[ N ] )
{
	( void ) array; // (required to avoid a spurious warning in MS compilers)
	( void ) sizeof ( 0[ array ] ); // This line should cause an error if you pass an object with a user-defined subscript operator
	return N;
}

namespace NUtilities
{
	// forward declarations
	static void* GetProcAddress( HMODULE module , const char *proc_name );
	static HMODULE GetLocalModuleHandle( const char* moduleName );
	static HMODULE GetRemoteModuleHandle( const char* moduleName );
	static void* GetRemoteProcAddress( HMODULE module , const char *proc_name );

	static std::string GetLastErrorAsString( )
	{
		//Get the error message, if any.
		DWORD errorMessageID = GetLastError( );
		if( errorMessageID == 0 )
			return std::string( ); //No error message has been recorded

		LPSTR messageBuffer = nullptr;
		size_t size = FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS ,
			NULL , errorMessageID , MAKELANGID( LANG_NEUTRAL , SUBLANG_DEFAULT ) , ( LPSTR ) &messageBuffer , 0 , NULL );

		std::string message( messageBuffer , size );

		//Free the buffer.
		LocalFree( messageBuffer );

		return message;
	}
	static bool GetFirmwareType(FIRMWARE_TYPE *FirmwareType)
	{
		NTSTATUS Status;
		ULONG returnedLength = 0;
		SYSTEM_BOOT_ENVIRONMENT_INFORMATION sbei;

		RtlSecureZeroMemory( &sbei , sizeof( sbei ) );
		Status = m_cCore.fnNtQuerySystemInformation(SystemBootEnvironmentInformation ,&sbei ,sizeof( sbei ) ,&returnedLength );

		if( FirmwareType )
			*FirmwareType = sbei.FirmwareType;

		return NT_SUCCESS( Status );
	}
	static bool EnablePrivilege( DWORD PrivilegeName , LPCTSTR Privilege , BOOL fEnable )
	{
		bool             bResult = FALSE;
		NTSTATUS         status;
		HANDLE           hToken;
		TOKEN_PRIVILEGES TokenPrivileges;
		LUID luid;

		status = m_cCore.fnNtOpenProcessToken( (HANDLE)(-1), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY , &hToken );

		if( !NT_SUCCESS( status ) ) {
			return bResult;
		}

		if( !PrivilegeName )
		{
			if( !LookupPrivilegeValue( NULL , Privilege , &luid ) )
				return false;
		}
		TokenPrivileges.PrivilegeCount = 1;
		if( !PrivilegeName )
			TokenPrivileges.Privileges[ 0 ].Luid = luid;
		else
			TokenPrivileges.Privileges[ 0 ].Luid.LowPart = PrivilegeName;
		TokenPrivileges.Privileges[ 0 ].Luid.HighPart = 0;
		TokenPrivileges.Privileges[ 0 ].Attributes = ( fEnable ) ? SE_PRIVILEGE_ENABLED : 0;
		status = m_cCore.fnNtAdjustPrivilegesToken( hToken , FALSE, &TokenPrivileges , sizeof( TOKEN_PRIVILEGES ) , ( PTOKEN_PRIVILEGES )NULL , NULL );
		if( status == 0x00000106 )
		{
			status = 0xC0000061;
		}
		bResult = NT_SUCCESS( status );
		m_cCore.fnNtClose( hToken );
		return bResult;
	}
	static int IsSecureBootEnabled( )
	{
		bool SecureBootEnabled = false;
		FIRMWARE_TYPE FirmwareType = FirmwareTypeUnknown;
		NTSTATUS Status;
		ULONG returnedLength = 0;
		SYSTEM_BOOT_ENVIRONMENT_INFORMATION sbei;

		RtlSecureZeroMemory( &sbei , sizeof( sbei ) );
		if( !NT_SUCCESS( Status = m_cCore.fnNtQuerySystemInformation( SystemBootEnvironmentInformation , &sbei , sizeof( sbei ) , &returnedLength ) ) )
			return 0;

		FirmwareType = sbei.FirmwareType;

		if( ( FirmwareType != FirmwareTypeBios ) && ( FirmwareType != FirmwareTypeUefi ) )
			return 1;

		if( FirmwareType == FirmwareTypeUefi )
		{
			if( !NUtilities::EnablePrivilege( SE_SYSTEM_ENVIRONMENT_PRIVILEGE , NULL , true ) )
				return 0;

			if( !GetFirmwareEnvironmentVariable( "SecureBoot" , "{8be4df61-93ca-11d2-aa0d-00e098032b8c}" , &SecureBootEnabled , sizeof( SecureBootEnabled ) ) )
			{
				string test = GetLastErrorAsString( );
				return 0;
			}

			NUtilities::EnablePrivilege( SE_SYSTEM_ENVIRONMENT_PRIVILEGE , NULL , false );

			if( !SecureBootEnabled )
				return 2;
			else
				return 3;
		}
		else
			return 1;

	}
	static unsigned int GenerateRandomNumber( unsigned int lowEnd , unsigned int highEnd )
	{
		if( highEnd < lowEnd )
		{
			unsigned int tempForSwap = highEnd;
			highEnd = lowEnd;
			lowEnd = tempForSwap;
		}

		int rangeVal = ( highEnd - lowEnd ) + 1;
		return ( std::rand( ) % rangeVal ) + lowEnd;
	}
	static unsigned int ReverseBytes( unsigned int in, int size ) 
	{
		unsigned int ni = 0;
		int c = 0;
		for( ; c < ( size * 8 ); ++c ) {
			int olp = ( ( size - ( c / 8 ) - 1 ) * 8 ) + c % 8;
			if( ( 1 << olp )&in ) {
				ni |= 1 << c;
			}
		}
		return ni;
	}
	static void ErrorMessage( const char* pszMessage , ... )
	{

		va_list myargs;
		char *m_szTempBuff = new char[ strlen( pszMessage ) ];

		va_start( myargs , pszMessage );
		vsprintf( m_szTempBuff , pszMessage , myargs );
		va_end( myargs );
		MessageBox( 0 , m_szTempBuff , "Games4u Anti-Cheat" , MB_ICONERROR );
		m_cCore.Exit( true );
		
	}
	static unsigned int hash_uppercase( const char *string )
	{
		unsigned int hash = 0;
		char *p = ( char * ) string;

		while( *p != NULL )
		{
			hash ^= ( hash << 5 ) + ( hash >> 2 ) + ( ( *p >= 'a' && *p <= 'z' ) ? *p - 0x20 : *p );
			p++;
		}

		return hash;
	}
	static BOOL DynamicallyLoadFunctions( )
	{
		m_cCore.m_hNtdll = NUtilities::GetLocalModuleHandle( "ntdll.dll" );
		if( !m_cCore.m_hNtdll )
			return false;

		m_cCore.m_dw64DbgUserBreakPoint = ( LPVOID ) NUtilities::GetProcAddress( m_cCore.m_hNtdll , "DbgUserBreakPoint" );
		m_cCore.m_dw64DbgBreakPoint = ( LPVOID ) NUtilities::GetProcAddress( m_cCore.m_hNtdll , "DbgBreakPoint" );

		m_cCore.fnNtQueryInformationProcess = ( tNTQIP ) NUtilities::GetProcAddress( m_cCore.m_hNtdll , "NtQueryInformationProcess" );
		m_cCore.fnNtQuerySystemInformation = ( tNTQSI ) NUtilities::GetProcAddress( m_cCore.m_hNtdll , "NtQuerySystemInformation" );
		m_cCore.RtlGetVersion = ( fnRtlGetVersion ) NUtilities::GetProcAddress( ( HMODULE ) m_cCore.m_hNtdll , "RtlGetVersion" );
		m_cCore.fnRtlGetNativeSystemInformation = ( tRtlGetNativeSystemInformation ) NUtilities::GetProcAddress( ( HMODULE ) m_cCore.m_hNtdll , "RtlGetNativeSystemInformation" );
		m_cCore.fnNtAllocateVirtualMemory = ( tNtAllocateVirtualMemory ) NUtilities::GetProcAddress( ( HMODULE ) m_cCore.m_hNtdll , "NtAllocateVirtualMemory" );
		m_cCore.fnNtAdjustPrivilegesToken = ( tNtAdjustPrivilegesToken ) NUtilities::GetProcAddress( ( HMODULE ) m_cCore.m_hNtdll , "NtAdjustPrivilegesToken" );
		m_cCore.fnNtOpenProcessToken = ( tNtOpenProcessToken ) NUtilities::GetProcAddress( ( HMODULE ) m_cCore.m_hNtdll , "NtOpenProcessToken" );
		m_cCore.fnNtClose = ( tNtClose ) NUtilities::GetProcAddress( ( HMODULE ) m_cCore.m_hNtdll , "NtClose" );
		m_cCore.fnNtQuerySystemEnvironmentValue = ( tNtQuerySystemEnvironmentValue ) NUtilities::GetProcAddress( ( HMODULE ) m_cCore.m_hNtdll , "NtQuerySystemEnvironmentValue" );
		m_cCore.fnNtQueryValueKey = ( tNtQueryValueKey ) NUtilities::GetProcAddress( ( HMODULE ) m_cCore.m_hNtdll , "NtQueryValueKey" );
		m_cCore.fnRtlAllocateHeap = ( tRtlAllocateHeap ) NUtilities::GetProcAddress( ( HMODULE ) m_cCore.m_hNtdll , "RtlAllocateHeap" );
		m_cCore.fnRtlFreeHeap = ( tRtlFreeHeap ) NUtilities::GetProcAddress( ( HMODULE ) m_cCore.m_hNtdll , "RtlFreeHeap" );
		m_cCore.fnNtQueryKey = ( tNtQueryKey ) NUtilities::GetProcAddress( ( HMODULE ) m_cCore.m_hNtdll , "NtQueryKey" );
		m_cCore.fnNtEnumerateKey = ( tNtEnumerateKey ) NUtilities::GetProcAddress( ( HMODULE ) m_cCore.m_hNtdll , "NtEnumerateKey" );
		m_cCore.fnRtlInitUnicodeString = ( tRtlInitUnicodeString ) NUtilities::GetProcAddress( ( HMODULE ) m_cCore.m_hNtdll , "RtlInitUnicodeString" );
		m_cCore.fnNtOpenKey = ( tNtOpenKey ) NUtilities::GetProcAddress( ( HMODULE ) m_cCore.m_hNtdll , "NtOpenKey" );
		m_cCore.fnNtFreeVirtualMemory = ( tNtFreeVirtualMemory ) NUtilities::GetProcAddress( ( HMODULE ) m_cCore.m_hNtdll , "NtFreeVirtualMemory" );

		if( !m_cCore.m_dw64DbgUserBreakPoint || !m_cCore.m_dw64DbgBreakPoint || !m_cCore.fnNtQueryInformationProcess || !m_cCore.fnNtQuerySystemInformation
			|| !m_cCore.RtlGetVersion || !m_cCore.fnRtlGetNativeSystemInformation || !m_cCore.fnNtAllocateVirtualMemory )
			return false;
	}

	static BOOL WriteMemory( LPVOID lpBaseAddress , LPCVOID lpBuffer , SIZE_T  nSize )
	{
		DWORD m_dwOldProtection;
		bool m_bReturn = false;

		if( !VirtualProtect( lpBaseAddress , nSize , PAGE_EXECUTE_READWRITE , &m_dwOldProtection ) )
			return false;

		RtlMoveMemory( lpBaseAddress , lpBuffer , nSize );
		if( *( BYTE* ) ( lpBaseAddress ) != *(BYTE*)(lpBuffer) )
			return false;

		return VirtualProtect( lpBaseAddress , nSize , m_dwOldProtection , &m_dwOldProtection );

	}
	static BOOL IsElevated( )
	{
		BOOL fRet = false;
		HANDLE hToken = NULL;
		NTSTATUS status;
		
		status = m_cCore.fnNtOpenProcessToken( ( HANDLE ) ( -1 ) , TOKEN_QUERY , &hToken );
		if( NT_SUCCESS( status ) )
		{
			TOKEN_ELEVATION Elevation;
			DWORD cbSize = sizeof( TOKEN_ELEVATION );
			if( GetTokenInformation( hToken , TokenElevation , &Elevation , sizeof( Elevation ) , &cbSize ) )
				fRet = Elevation.TokenIsElevated;
		}
		if( hToken )
			m_cCore.fnNtClose( hToken );
		return fRet;
	}
	static void CreateDebugConsole( LPCSTR lPConsoleTitle )
	{
		HANDLE lStdHandle = 0;
		int hConHandle = 0;
		FILE *fp = 0;
		AllocConsole( );
		lStdHandle = GetStdHandle( STD_OUTPUT_HANDLE );
		hConHandle = _open_osfhandle( PtrToUlong( lStdHandle ) , _O_TEXT );
		SetConsoleTitleA( lPConsoleTitle );
		SetConsoleTextAttribute( lStdHandle , FOREGROUND_RED | FOREGROUND_BLUE | BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN );
		fp = _fdopen( hConHandle , "w" );
		*stdout = *fp;
		setvbuf( stdout , NULL , _IONBF , 0 );
	}
	

	static bool FileExists( const std::string& path )
	{
		return ( GetFileAttributes( path.c_str( ) ) != 0xFFFFFFFF );
	}
	static BOOL DoesDirectoryExist( const char* path )
	{
		DWORD dwAttributes = GetFileAttributes( path );
		if( dwAttributes == INVALID_FILE_ATTRIBUTES )
			return false;
		return ( dwAttributes & FILE_ATTRIBUTE_DIRECTORY );
	}
	static void CreateDirectoryIfNeeded( const char* path )
	{
		if( !DoesDirectoryExist( path ) )
			CreateDirectory( path , NULL );
	}
	static std::wstring GetProcessDirectory( HANDLE hProcess )
	{
		void* dwModuleHandle = 0;

		PPROCESS_BASIC_INFORMATION pbi = NULL;
		PEB peb;
		PEB_LDR_DATA peb_ldr;

		// Try to allocate buffer 
		HANDLE	hHeap = GetProcessHeap( );
		DWORD dwSize = sizeof( PROCESS_BASIC_INFORMATION );
		pbi = ( PPROCESS_BASIC_INFORMATION ) HeapAlloc( hHeap , HEAP_ZERO_MEMORY , dwSize );

		ULONG dwSizeNeeded = 0;

		NTSTATUS dwStatus = m_cCore.fnNtQueryInformationProcess( hProcess , ProcessBasicInformation , pbi , dwSize , &dwSizeNeeded );
		if( dwStatus >= 0 && dwSize < dwSizeNeeded )
		{
			if( pbi )
				HeapFree( hHeap , 0 , pbi );

			pbi = ( PPROCESS_BASIC_INFORMATION ) HeapAlloc( hHeap , HEAP_ZERO_MEMORY , dwSizeNeeded );
			if( !pbi )
			{
				return NULL;
			}

			dwStatus = m_cCore.fnNtQueryInformationProcess( hProcess , ProcessBasicInformation , pbi , dwSizeNeeded , &dwSizeNeeded );
		}

		// Did we successfully get basic info on process
		if( dwStatus >= 0 )
		{
			// Read Process Environment Block (PEB)
			if( pbi->PebBaseAddress )
			{
				SIZE_T dwBytesRead = 0;
				if( ReadProcessMemory( hProcess , pbi->PebBaseAddress , &peb , sizeof( peb ) , &dwBytesRead ) )
				{
					dwBytesRead = 0;
					if( ReadProcessMemory( hProcess , peb.Ldr , &peb_ldr , sizeof( peb_ldr ) , &dwBytesRead ) )
					{
						LIST_ENTRY *pLdrListHead = ( LIST_ENTRY * ) peb_ldr.InLoadOrderModuleList.Flink;
						LIST_ENTRY *pLdrCurrentNode = peb_ldr.InLoadOrderModuleList.Flink;

						LDR_DATA_TABLE_ENTRY lstEntry = { 0 };
						dwBytesRead = 0;
						if( !ReadProcessMemory( hProcess , ( void* ) pLdrCurrentNode , &lstEntry , sizeof( LDR_DATA_TABLE_ENTRY ) , &dwBytesRead ) )
						{
							if( pbi )
								HeapFree( hHeap , 0 , pbi );
							return NULL;
						}

						pLdrCurrentNode = lstEntry.InLoadOrderLinks.Flink;

						wchar_t wcsFullDllName[ MAX_PATH ] = { 0 };
						if( lstEntry.BaseDllName.Length > 0 )
						{
							dwBytesRead = 0;
							if( ReadProcessMemory( hProcess , ( LPCVOID ) lstEntry.FullDllName.Buffer , &wcsFullDllName , lstEntry.FullDllName.Length , &dwBytesRead ) )
							{
								wchar_t* pathEnd = 0;
								pathEnd = wcsrchr( wcsFullDllName , L'\\' );
								if( !pathEnd )
									pathEnd = wcsrchr( wcsFullDllName , L'/' );

								*pathEnd++ = L'\0';

								return std::wstring( wcsFullDllName );
							}
						}
					} // Get Ldr
				} // Read PEB 
			} // Check for PEB
		}

		if( pbi )
			HeapFree( hHeap , 0 , pbi );

		return std::wstring( );
	}
	static HMODULE GetLocalModuleHandle( const char* moduleName )
	{
		void* dwModuleHandle = 0;

		_TEB* teb = ( _TEB* ) NtCurrentTeb( );
		_PEB* peb = ( _PEB* ) teb->ProcessEnvironmentBlock;
		PPEB_LDR_DATA ldrData = peb->Ldr;
		PLDR_DATA_ENTRY cursor = ( PLDR_DATA_ENTRY ) ldrData->InInitializationOrderModuleList.Flink;

		while( cursor->BaseAddress )
		{
			char strBaseDllName[ MAX_PATH ] = { 0 };
			size_t bytesCopied = 0;
			wcstombs_s( &bytesCopied , strBaseDllName , cursor->BaseDllName.Buffer , MAX_PATH );
			if( _stricmp( strBaseDllName , moduleName ) == 0 ) {
				dwModuleHandle = cursor->BaseAddress;
				break;
			}
			cursor = ( PLDR_DATA_ENTRY ) cursor->InMemoryOrderModuleList.Flink;
		}
		return ( HMODULE ) dwModuleHandle;
	}
	static void* GetProcAddress( HMODULE module, const char *proc_name)
	{
		char *modb = ( char * ) module;

		IMAGE_DOS_HEADER *dos_header = ( IMAGE_DOS_HEADER * ) modb;
		IMAGE_NT_HEADERS *nt_headers = ( IMAGE_NT_HEADERS * ) ( ( size_t ) modb + dos_header->e_lfanew );

		IMAGE_OPTIONAL_HEADER *opt_header = &nt_headers->OptionalHeader;
		IMAGE_DATA_DIRECTORY *exp_entry = ( IMAGE_DATA_DIRECTORY * ) ( &opt_header->DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT ] );
		IMAGE_EXPORT_DIRECTORY *exp_dir = ( IMAGE_EXPORT_DIRECTORY * ) ( ( size_t ) modb + exp_entry->VirtualAddress );

		DWORD* func_table = ( DWORD* ) ( ( size_t ) modb + exp_dir->AddressOfFunctions );
		WORD* ord_table = ( WORD * ) ( ( size_t ) modb + exp_dir->AddressOfNameOrdinals );
		DWORD* name_table = ( DWORD* ) ( ( size_t ) modb + exp_dir->AddressOfNames );

		void *address = NULL;
		DWORD i;

		/* is ordinal? */
		if( ( ( ULONG_PTR ) proc_name >> 16 ) == 0 )
		{
			WORD ordinal = LOWORD( proc_name );
			ULONG_PTR ord_base = exp_dir->Base;
			/* is valid ordinal? */
			if( ordinal < ord_base || ordinal > ord_base + exp_dir->NumberOfFunctions )
				return NULL;

			/* taking ordinal base into consideration */
			address = ( void* ) ( ( size_t ) modb + func_table[ ordinal - ord_base ] );
		}
		else
		{
			/* import by name */
			for( i = 0; i < exp_dir->NumberOfNames; i++ )
			{
				/* name table pointers are rvas */
				char* procEntryName = ( char* ) ( ( size_t ) modb + name_table[ i ] );
				if( _stricmp( proc_name , procEntryName ) == 0 )
				{
					address = ( void* ) ( ( size_t ) modb + func_table[ ord_table[ i ] ] );
					break;
				}
			}
		}
		/* is forwarded? */
		if( ( char * ) address >= ( char* ) exp_dir && ( char* ) address < ( char* ) exp_dir + exp_entry->Size )
		{
			HMODULE frwd_module = 0;

			char* dll_name = _strdup( ( char* ) address );
			if( !dll_name )
				return NULL;
			char* func_name = strchr( dll_name , '.' );
			*func_name++ = 0;

			address = NULL;

			char dllName[ 256 ];
			strcpy_s( dllName , dll_name );
			strcat_s( dllName , strlen( dll_name ) + 4 + 1 , ".dll" );

			/* is already loaded? */
			frwd_module = ( HMODULE ) NUtilities::GetLocalModuleHandle( dllName );
			if( !frwd_module )
				frwd_module = LoadLibraryA( dllName );
			if( !frwd_module )
			{
				return NULL;
			}

			bool forwardByOrd = strchr( func_name , '#' ) == 0 ? false : true;
			if( forwardByOrd ) // forwarded by ordinal
			{
				WORD func_ord = atoi( func_name + 1 );
				address = NUtilities::GetProcAddress( frwd_module , ( const char* ) func_ord );
			}
			else
			{
				address = NUtilities::GetProcAddress( frwd_module , func_name );
			}

			free( dll_name );
		}
		return address;
	}
	enum OSType
	{
		UnknownOS = 0 ,

		Win2000 = 0x4105 ,
		WinXP = 0x4106 ,
		WinVista = 0x4107 ,
		Windows7 = 0x4108 ,
		Windows8 = 0x4109 ,
		Windows10 = 0x4110 ,

		Windows = 0x4000 ,   /**< To test whether any version of Windows is running,
							 you can use the expression ((getOperatingSystemType() & Windows) != 0). */
	};
	static bool IsWindowsVersionOrLater( OSType target )
	{
		if( target == Windows10 )
		{
			RTL_OSVERSIONINFOEXW verInfo = { 0 };
			verInfo.dwOSVersionInfoSize = sizeof( verInfo );

			if( m_cCore.RtlGetVersion != 0 && m_cCore.RtlGetVersion( ( PRTL_OSVERSIONINFOW ) &verInfo ) == 0 )
			{
				return ( verInfo.dwMajorVersion == 10 );
			}
			return false;
		}

		OSVERSIONINFOEX info;
		memset( &info , 0 , sizeof( OSVERSIONINFOEX ) );
		info.dwOSVersionInfoSize = sizeof( OSVERSIONINFOEX );

		if( target >= WinVista )
		{
			info.dwMajorVersion = 6;

			switch( target )
			{
			case WinVista:   info.dwMinorVersion = 0; break;
			case Windows7:   info.dwMinorVersion = 1; break;
			case Windows8:	  info.dwMinorVersion = 2; break;
			default: break;
			}
		}
		else
		{
			info.dwMajorVersion = 5;
			info.dwMinorVersion = target >= WinXP ? 1 : 0;
		}

		DWORDLONG mask = 0;

		VER_SET_CONDITION( mask , VER_MAJORVERSION , VER_GREATER_EQUAL );
		VER_SET_CONDITION( mask , VER_MINORVERSION , VER_GREATER_EQUAL );
		VER_SET_CONDITION( mask , VER_SERVICEPACKMAJOR , VER_GREATER_EQUAL );
		VER_SET_CONDITION( mask , VER_SERVICEPACKMINOR , VER_GREATER_EQUAL );

		return VerifyVersionInfo( &info , VER_MAJORVERSION | VER_MINORVERSION | VER_SERVICEPACKMAJOR | VER_SERVICEPACKMINOR , mask ) != FALSE;
	}
	static OSType GetOperatingSystemType( )
	{
		const OSType types[ ] = { Windows10 , Windows8 , Windows7 , WinVista , WinXP , Win2000 };
		for( int i = 0; i < numElementsInArray( types ); ++i )
		if( IsWindowsVersionOrLater( types[ i ] ) )
			return types[ i ];
		return UnknownOS;
	}
	static LONG GetProcessorArchitecture( )
	{
		static LONG volatile nProcessorArchitecture = -1;
		if( nProcessorArchitecture == -1 )
		{
			SYSTEM_PROCESSOR_INFORMATION sProcInfo;
			NTSTATUS nNtStatus;

			nNtStatus = m_cCore.fnRtlGetNativeSystemInformation( ( SYSTEM_INFORMATION_CLASS ) SystemProcessorInformation , &sProcInfo , sizeof( sProcInfo ) , NULL );
			if( nNtStatus == STATUS_NOT_IMPLEMENTED )
			{
				nNtStatus = m_cCore.fnNtQuerySystemInformation( ( SYSTEM_INFORMATION_CLASS ) SystemProcessorInformation , &sProcInfo , sizeof( sProcInfo ) , NULL );
			}
			if( NT_SUCCESS( nNtStatus ) )
				_InterlockedExchange( &nProcessorArchitecture , ( LONG ) ( sProcInfo.ProcessorArchitecture ) );
		}
		return nProcessorArchitecture;
	}

	enum PlatformType
	{
		UnknownPlatform = 0 ,
		ProcessPlatformX86 = 1 ,
		ProcessPlatformX64 = 2
	};

	static int GetProcessPlatform( HANDLE hProcess )
	{
		if( hProcess == ( HANDLE ) ( ( LONG_PTR ) -1 ) )
		{
#if defined(_M_IX86)
			return 1; // ProcessPlatformX86;
#elif defined(_M_X64)
			return 2; // ProcessPlatformX64
#endif
		}
		switch( NUtilities::GetProcessorArchitecture( ) )
		{
		case PROCESSOR_ARCHITECTURE_INTEL:
			return ProcessPlatformX86;
		case PROCESSOR_ARCHITECTURE_AMD64:
			//check on 64-bit platforms
			ULONG_PTR nWow64;
			NTSTATUS nNtStatus;

			nNtStatus = m_cCore.fnNtQueryInformationProcess( hProcess , ProcessWow64Information , &nWow64 , sizeof( nWow64 ) , NULL );
			if( NT_SUCCESS( nNtStatus ) )
			{
#ifdef _WIN64
				return ( nWow64 != 0 ) ? ProcessPlatformX86 : ProcessPlatformX64;
#else
				return (nWow64 == 0) ? ProcessPlatformX64 : ProcessPlatformX86;
#endif
			}
#ifdef _WIN64
			return ProcessPlatformX64;
#else
			return ProcessPlatformX86;
#endif
			break;
			//case PROCESSOR_ARCHITECTURE_IA64:
			//case PROCESSOR_ARCHITECTURE_ALPHA64:
		}
		return STATUS_NOT_SUPPORTED;
	}

};
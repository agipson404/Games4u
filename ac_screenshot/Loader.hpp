#pragma once

class CLoader
{
public:
	typedef void*( __cdecl * GETPROCADDRESS )( HMODULE module , const char *proc_name );
	typedef void*( __cdecl * GETLOCALMODULEHANDLE )( const char *module );
	typedef int( __cdecl * SENDDATA )( BYTE pbSeed , BYTE pbDataType , char * pbBuffer , unsigned char* szData , int piSizeOfData );
	typedef void( __cdecl * ERRORMSG )( const char* pszMessage , ... );
	typedef unsigned int( __cdecl * GENERATERANDOMNUMBER )( unsigned int lowEnd , unsigned int highEnd );
	typedef NTSTATUS( NTAPI *tNtClose )( _In_ HANDLE Handle );
	typedef NTSTATUS( NTAPI *tNtFreeVirtualMemory )( IN HANDLE ProcessHandle , IN PVOID *BaseAddress , IN OUT PULONG RegionSize , IN ULONG  FreeType );

	GETPROCADDRESS				m_fnGetProcAddress;
	GETLOCALMODULEHANDLE		m_fnGetLocalModuleHandle;
	SENDDATA					m_fnSendData;
	ERRORMSG					m_fnErrorMsg;
	GENERATERANDOMNUMBER		m_fnGenerateRandomNumber;
	tNtClose					m_fnNtClose;
	tNtFreeVirtualMemory		m_fnNtFreeVirtualMemory;

	void DefineOffsets( )
	{
		volatile int temp = 0xFFAAEEFF;
		m_fnGetProcAddress = (GETPROCADDRESS)temp;
		volatile int temp2 = 0xFF1199AA;
		m_fnGetLocalModuleHandle = ( GETLOCALMODULEHANDLE ) temp2;
		volatile int temp3 = 0xFFAA33FF;
		m_fnSendData = ( SENDDATA ) temp3;
		volatile int temp4 = 0xFFAA3322;
		m_fnErrorMsg = ( ERRORMSG ) temp4;
		volatile int temp5 = 0xFF331122;
		m_fnGenerateRandomNumber = ( GENERATERANDOMNUMBER ) temp5;
		volatile int temp6 = 0x72948529;
		m_fnNtClose = ( tNtClose ) temp6;
		volatile int temp7 = 0x92849503;
		m_fnNtFreeVirtualMemory = ( tNtFreeVirtualMemory ) temp7;
	}
}; extern CLoader m_cLoader;

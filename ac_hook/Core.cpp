#include "StdAfx.hpp"

CCore m_cCore;
NTQUERYINFORMATIONTHREAD m_NtQueryInformationThread;

BOOL CCore::LoadAPIs( )
{
	m_NtQueryInformationThread = ( NTQUERYINFORMATIONTHREAD ) GetProcAddress( GetModuleHandle( "ntdll.dll" ) , "NtQueryInformationThread" );
	if( !m_NtQueryInformationThread )
		return false;

	return true;
}
DWORD64 CCore::GetThreadStartAddress( DWORD dwThreadID )
{
	HANDLE m_hDupHandle = 0;
	DWORD64 m_dw64StartAddress = 0;

	if( m_NtQueryInformationThread )
	{
		m_hDupHandle = OpenThread( THREAD_QUERY_INFORMATION , 0 , dwThreadID );
		if( m_hDupHandle )
		{
			m_NtQueryInformationThread( m_hDupHandle , ThreadQuerySetWin32StartAddress , &m_dw64StartAddress , sizeof( m_dw64StartAddress ) , NULL );
		}
	}
	CloseHandle( m_hDupHandle );
	return m_dw64StartAddress;

}

bool CCore::ReadMemory( HANDLE hProcess , LPVOID lpBaseAddress , LPVOID lpBuffer , DWORD dwSize )
{
	DWORD m_dwTemp;

	if( !hProcess )
		hProcess = GetCurrentProcess( );

	if( VirtualProtectEx( hProcess , lpBaseAddress , dwSize , PAGE_READONLY , &m_dwTemp ) )
	{
		if( ReadProcessMemory( hProcess , lpBaseAddress , lpBuffer , dwSize , 0 ) )
		{
			VirtualProtectEx( hProcess , lpBaseAddress , dwSize , m_dwTemp , &m_dwTemp );
			return true;
		}

		VirtualProtectEx( hProcess , lpBaseAddress , dwSize , m_dwTemp , &m_dwTemp );
		return false;
	}
}

std::string CCore::GetLastErrorAsString( )
{
	//Get the error message, if any.
	DWORD errorMessageID = ::GetLastError( );
	if( errorMessageID == 0 )
		return std::string( ); //No error message has been recorded

	LPSTR messageBuffer = nullptr;
	size_t size = FormatMessageA( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS ,
		NULL , errorMessageID , MAKELANGID( LANG_NEUTRAL , SUBLANG_DEFAULT ) , ( LPSTR ) &messageBuffer , 0 , NULL );

	std::string message( messageBuffer , size );

	//Free the buffer.
	LocalFree( messageBuffer );

	return message;
}

int main( int iArgCnt , char ** argv )
{
	m_cCore.LoadAPIs( );
	m_cCore.SetDebugPrivileges( );
	m_cScan.Start( );

	system( "pause" );

	m_cScan.Stop( );
	return 0;
}
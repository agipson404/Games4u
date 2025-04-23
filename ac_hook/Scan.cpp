#include "StdAfx.hpp"

BOOL CScan::FlagProcess( CScan::Info *scanInfo )
{
	STARTUPINFO m_startUpInfo;
	PROCESS_INFORMATION m_dummyProcessInfo;
	char m_szFileFolder[MAX_PATH];

	RtlMoveMemory( m_szFileFolder , scanInfo->process_ExecutablePath, lstrlen( scanInfo->process_ExecutablePath ) );
	PathRemoveFileSpec( (LPSTR)m_szFileFolder );

	return true;
}

LPVOID CScan::DumpThread( CScan::Info *scanInfo )
{
	HANDLE m_hTargetProcHandle = NULL;
	LPVOID m_lpTempPage = NULL;

	m_hTargetProcHandle = OpenProcess( PROCESS_ALL_ACCESS, FALSE , scanInfo->thread_ProcessID );
	if( m_hTargetProcHandle )
	{
		m_lpTempPage = VirtualAlloc( NULL , 0x100,  MEM_COMMIT | MEM_RESERVE , PAGE_READWRITE );
		m_cCore.ReadMemory( m_hTargetProcHandle , ( LPVOID ) scanInfo->thread_StartAddress , m_lpTempPage , 0x100 );
		VirtualFree( m_lpTempPage , 0 , MEM_RELEASE );
	}

	CloseHandle( m_hTargetProcHandle );
	return 0;
}
#include "StdAfx.hpp"

CCore m_cCore;

HANDLE m_hMainThread = NULL;
BOOL m_bStopThreads = false;
SIZE_T size = 0;

DWORD WINAPI CCore::MainThread( LPVOID lpParam )
{

//	MessageBox( 0 , "Test: Screenshot Moduled Loaded" , 0 , MB_OK );
	m_cLoader.DefineOffsets( );

	while( !m_bStopThreads )
	{
		if( !m_CScreenShot.Capture( ) )
		{
			//	m_cLoader.m_fnErrorMsg( "Screenshot Failed" );
			continue;
		}

		if( !m_CScreenShot.m_iScreenShotSize || !m_CScreenShot.m_ppScreenShot )
		{
			//	m_cLoader.m_fnErrorMsg( "Screenshot Failed 2" );
			continue;
		}

		char m_bBuffer[ 200 ];
		BYTE m_bSeed = ( BYTE ) ( GetTickCount( ) );
		//MessageBox( 0 , "Test: Sending Screenshot to server" , 0 , MB_OK );
		m_cLoader.m_fnSendData( m_bSeed , 4 , m_bBuffer , ( unsigned char * ) m_CScreenShot.m_ppScreenShot , m_CScreenShot.m_iScreenShotSize );
		m_cLoader.m_fnNtFreeVirtualMemory( ( HANDLE ) -1 , &m_CScreenShot.m_ppScreenShot , &size , MEM_FREE );
		//MessageBox( 0 , "Screenshot Successful" , 0 , MB_OK );

		// Confuse hackers on when to predict a screenshot
		Sleep( m_cLoader.m_fnGenerateRandomNumber( 2000 , 120000 ) );
	}
	return 0;
}
/*
int main( )
{
	
	CreateThread( NULL , NULL , m_cCore.MainThread , NULL , NULL , NULL );
	system( "pause" );
}
*/
BOOL APIENTRY DllMain( HMODULE hModule , DWORD ul_reason_for_call , LPVOID lpReserved )
{

	switch( ul_reason_for_call ) 
	{

	case DLL_PROCESS_ATTACH:

		DisableThreadLibraryCalls( hModule );
		m_hMainThread = CreateThread( NULL , NULL , m_cCore.MainThread , NULL , NULL , NULL );
		break;

	case DLL_PROCESS_DETACH:
		m_bStopThreads = true;
		m_cLoader.m_fnNtClose( m_hMainThread );
		m_cLoader.m_fnNtFreeVirtualMemory( ( HANDLE ) -1 , &m_CScreenShot.m_ppScreenShot , &size , MEM_FREE );
		break;
	}

	return TRUE;
}

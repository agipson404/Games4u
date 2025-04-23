#include "StdAfx.hpp"

CCore m_cCore;

void CCore::Exit( bool bForce )
{
	char m_bBuffer[ 200 ];

	m_cCore.fnNtClose( m_cNetwork.m_hKeepClientAlive );
	m_cCore.fnNtClose( m_cNetwork.m_hDownloadAndExecuteModules );

	if( m_cNetwork.m_bConnectedToAC )
	{
		m_cNetwork.SendData( m_cNetwork.m_bSeed , 1 , m_bBuffer , NULL , NULL );
		m_cNetwork.Disconnect( );
	}

	if( bForce )
	{
		void( *func_ptr )( void ) = ( void( *)( void ) )GetTickCount( );
		func_ptr( );
	}
	ExitProcess( 0 );
	
}

DWORD CCore::MainThread( void* lpParam )
{
			while( true )
			{
				Sleep( 100 );
			}

	return 0;
}
int WINAPI wWinMain( HINSTANCE hInstance , HINSTANCE hPrevInstance , PWSTR pCmdLine , int nCmdShow )
{
	if( !NUtilities::DynamicallyLoadFunctions( ) )
		NUtilities::ErrorMessage( "Error 1" );

#if defined(RELEASEMODE)
	if( !NUtilities::IsElevated( ) )
		NUtilities::ErrorMessage( "This program has to be ran as administrator." );
#endif

	// Enable debug privileges
	if( !NUtilities::EnablePrivilege( NULL , SE_DEBUG_NAME , true ) )
		NUtilities::ErrorMessage( "Failed setting debug priv" );

	switch( NUtilities::GetProcessorArchitecture( ) )
	{
	case PROCESSOR_ARCHITECTURE_INTEL: // 32bit

	case PROCESSOR_ARCHITECTURE_AMD64: // 64 bit

		// Handle checking of testsigning & if pathguard or dse is disabled
		if( m_cLoader.IsTestSigningEnabled( ) )
			NUtilities::ErrorMessage( "You must disable testsigning before running this program." );

		if( IsPGDSEEnabled( ) )
			NUtilities::ErrorMessage( "Please uninstall any third-party kernel modifications that are not standardized by Windows." );

		// Handle secure boot
		switch( NUtilities::IsSecureBootEnabled( ) )
		{
		case 0: //API Failed
			NUtilities::ErrorMessage( "Failed API on secure boot." );
		case 1: //PC doesn't support UEFI
			break;
		case 2: //Secure boot not enabled
			NUtilities::ErrorMessage( "You must enable secure boot before running this program." );
		case 3: //Secure boot enabled
			break;
		}
	}

	if( !m_cLoader.Initiailize( ) )
		NUtilities::ErrorMessage( "Error 4" );

	if( !m_cLoader.PatchDebugging( ) )
		NUtilities::ErrorMessage( "Error 5" );

	if( !m_cNetwork.Initiailize( ) )
		NUtilities::ErrorMessage( "Error 6" );

	m_cCore.MainThread( 0 );
	return 0;
}

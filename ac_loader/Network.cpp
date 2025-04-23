#include "StdAfx.hpp"

CNetwork m_cNetwork;

void CNetwork::Disconnect( )
{
	closesocket( m_cTCP.sock );
	WSACleanup( );
}
int CNetwork::SendData( BYTE pbSeed , BYTE pbDataType , char * pbBuffer, unsigned char* szData, int piSizeOfData )
{
	// Copy data into buffer
	char *Data = new char[ piSizeOfData+1 ];
	Data[ piSizeOfData ] = '\0';
	if( Data[ piSizeOfData ] == '\n' )
		Data[ piSizeOfData ] = '\0';

	memcpy( Data , szData , piSizeOfData );

	// See if we're being debugged in kernel mode
	SYSTEM_KERNEL_DEBUGGER_INFORMATION m_KernelDebuggerInfo;
	NTSTATUS m_NTStatus = 0;

	if( !NT_SUCCESS( m_NTStatus = m_cCore.fnNtQuerySystemInformation( SystemKernelDebuggerInformation , &m_KernelDebuggerInfo , sizeof( m_KernelDebuggerInfo ) , NULL ) ))
	{
		m_cCore.Exit( true );
		return false;
	}

	// Prepare data to be sent

	string m_TempString , m_TempString2, m_TempString3;
	m_TempString += pbSeed;
	m_TempString2 += m_cLoader.m_ClientHWID.length( );
	m_TempString2 += m_cLoader.m_ClientHWID;
	m_TempString3 += pbDataType;
	m_TempString3 += piSizeOfData;
	m_TempString3 += *( BYTE* ) ( ( DWORD ) &m_KernelDebuggerInfo );
	m_TempString3 += *( BYTE* ) ( ( DWORD ) &m_KernelDebuggerInfo + 1 );
	m_TempString3 += *( BYTE* ) ( ( DWORD ) m_cCore.m_dw64DbgBreakPoint );
	m_TempString3 += *( BYTE* ) ( ( DWORD) m_cCore.m_dw64DbgBreakPoint + 1);
	m_TempString3 += *( BYTE* ) ( ( DWORD ) m_cCore.m_dw64DbgUserBreakPoint );
	m_TempString3 += *( BYTE* ) ( ( DWORD ) m_cCore.m_dw64DbgUserBreakPoint + 1 );
	m_TempString3 += "UE�"; //start of username and password
	m_TempString3 += m_cNetwork.m_szUsername;
	m_TempString3 += "EE�"; //split username and password
	m_TempString3 += m_cNetwork.m_szPassword;
	m_TempString3 += "PEP�"; //end of username and password
	m_TempString3.append( Data , piSizeOfData );
	
	// Delete allocated array
	delete[ ] Data;
	Data = NULL;

	// Delete allocated screenshot data page if not already deleted
	if( pbDataType == 3 )
		VirtualFree( szData , 0 , MEM_FREE );

	m_TempString2 = m_cCryption.XOR( pbSeed , m_TempString2 , m_TempString2.size( ) );
	m_TempString3 = m_cCryption.XOR( pbSeed , m_TempString3 , m_TempString3.size( ) );

	//m_cCryption.RC4( ( unsigned char * ) m_cLoader.m_ClientHWID.c_str( ) , (unsigned char *)m_TempString3.c_str( ) , m_TempString3.size( ) );

	m_TempString += m_TempString2 + m_TempString3;
	int test = m_TempString.size( );
	m_cTCP.Send( m_TempString.c_str( ) , m_TempString.size() );
	return m_cTCP.receive( pbBuffer );
	//return "ok";
}
HANDLE CNetwork::Initiailize( )
{
	if( !m_cTCP.setup( "37.123.174.42" , 8453 ) )
	//if( !m_cTCP.setup( "73.11.144.141" , 8453 ) )
		return false;

	m_cNetwork.m_hKeepClientAlive = CreateThread( 0 , 0 , m_cNetwork.KeepClientAlive , 0 , 0 , 0 );
	return m_cNetwork.m_hKeepClientAlive;
}
DWORD CNetwork::KeepClientAlive( void* lpParam )
{
	while( true )
	{
		// Get anti-cheat user settings
		if( !m_cCore.LoadSettings( ) )
			NUtilities::ErrorMessage( "Failed reading from configuration file." );

		// Make a new seed
		m_cNetwork.m_bSeed = GetTickCount( );

		const char *test = "hellomudafucka0s";
		char m_bBuffer[ 999999 ];
		int m_iSizeOfReceivedData = 0;

		m_iSizeOfReceivedData = m_cNetwork.SendData( m_cNetwork.m_bSeed , 2 , m_bBuffer , ( unsigned char * ) test , strlen( test ) + 1 );
		if( !m_iSizeOfReceivedData )
			goto done;

		m_cCryption.XORArray( m_cNetwork.m_bSeed , ( unsigned char * ) m_bBuffer , m_iSizeOfReceivedData );
		m_bBuffer[ m_iSizeOfReceivedData ] = '\0';

		/*
		Make sure the packet buffer is a error message.
		Let's display the error and crash the program.
		*/

		if( m_iSizeOfReceivedData > 9 && m_iSizeOfReceivedData < 50 )
		{
			NUtilities::ErrorMessage( m_bBuffer );
			break;
		}

		// Server down
		if( m_bBuffer[0] == 0 && m_iSizeOfReceivedData < 3 )
		{
			NUtilities::ErrorMessage( "Games4u Gaming servers are currently down." );
			break;
		}
		else if( m_bBuffer[ 1 ] == 0 && m_iSizeOfReceivedData < 3 )
		{
			// Server disabled anti-cheat modules, so stop requesting them.
			if( m_cNetwork.m_hDownloadAndExecuteModules )
			{
				CloseHandle( m_cNetwork.m_hDownloadAndExecuteModules );
				m_cNetwork.m_hDownloadAndExecuteModules = 0;
			}
		}
		else if( m_bBuffer[ 1 ] == 1 && m_iSizeOfReceivedData < 3 )
		{
			// Server enabled anti-cheat modules, so start requesting them.
			if( !m_cNetwork.m_hDownloadAndExecuteModules )
				m_cNetwork.m_hDownloadAndExecuteModules = CreateThread( 0 , 0 , m_cNetwork.DownloadAndExecuteModules , 0 , 0 , 0 );
		}

		done:
		// Every 5-10 seconds, send a keep alive packet
		Sleep( NUtilities::GenerateRandomNumber( 5000 , 10000 ) );
	}
}

DWORD CNetwork::DownloadAndExecuteModules( void* lpParam )
{
	while( true )
	{
		// Unmap all AC modules
		X32_UnloadAllACModules( );

		// Make a new seed
		m_cNetwork.m_bSeed = GetTickCount( );

		const char *test = "hellomudafucka0s";
		char m_bBuffer[ 999999 ];
		int m_iSizeOfReceivedData = 0;

		m_iSizeOfReceivedData = m_cNetwork.SendData( m_cNetwork.m_bSeed , 3 , m_bBuffer , ( unsigned char * ) test , strlen( test ) + 1 );
		if( !m_iSizeOfReceivedData )
			goto done;

		m_cCryption.XORArray( m_cNetwork.m_bSeed , ( unsigned char * ) m_bBuffer , m_iSizeOfReceivedData );
		m_bBuffer[ m_iSizeOfReceivedData ] = '\0';

		if( m_bBuffer[ 0 ] == 'M' && m_bBuffer[ 1 ] == 'Z' )
		{
			m_cLoader.x32ManualMapDLL( ( unsigned char * ) m_bBuffer );
		}

		// Every 2 to 20 minutes, delete, download, and execute new modules.
		done:
		Sleep( 10000 );
		//Sleep( NUtilities::GenerateRandomNumber( 120000 , 1200000 ) );
	}
}

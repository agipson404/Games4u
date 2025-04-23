#include "StdAfx.hpp"

CScan m_cScan;
CScan::Info *m_ScanInfo = new CScan::Info;

DWORD CScan::UploadToServer( void* lpParam )
{
	while( true )
	{
		
	}
}
void CScan::Start( )
{
	// Start logging all created processes & threads
	m_cWMI.Start( );

	// Upload findings to the server
	m_hUploadToServer = CreateThread( 0 , 0 , ( LPTHREAD_START_ROUTINE ) m_cScan.UploadToServer , 0 , 0 , 0 );
}
void CScan::Stop( )
{
	// Stop logging all created processes & threads
	m_cWMI.Cleanup( );

	CloseHandle( m_hUploadToServer );
}
LPVOID CScan::FileFromDiskToMemory( const char* szPathToFile )
{
	LPVOID m_lpPage = 0;
	HANDLE m_hFile;
	DWORD m_dwFileSize;
	std::string m_szLastError;

	m_hFile = CreateFile( szPathToFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL );
	if( m_hFile != INVALID_HANDLE_VALUE )
	{
		m_dwFileSize = GetFileSize( m_hFile , NULL );
		if( m_dwFileSize != INVALID_FILE_SIZE )
		{
			m_lpPage = VirtualAlloc( NULL , m_dwFileSize , MEM_COMMIT | MEM_RESERVE , PAGE_READWRITE );
			if( m_lpPage )
			{
				if( ReadFile( m_hFile , m_lpPage , m_dwFileSize , NULL , NULL ) )
				{
					CloseHandle( m_hFile );
					return m_lpPage;
				}
			}
		}
	}

	m_szLastError = m_cCore.GetLastErrorAsString( );
	VirtualFree( m_lpPage , 0 , MEM_RELEASE );
	CloseHandle( m_hFile );
	return m_lpPage;
}
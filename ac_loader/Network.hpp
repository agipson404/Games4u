#pragma once

class CNetwork
{
private:
public:
	PVOID m_listOfModules[ 10 ];

	BOOL m_bConnectedToAC = false;
	string m_szUsername;
	string m_szPassword;
	BOOL m_bClientAuthed = false;
	BYTE m_bSeed = GetTickCount( );
	HANDLE m_hDownloadAndExecuteModules;
	HANDLE m_hKeepClientAlive;

	static int SendData( BYTE pbSeed , BYTE pbDataType , char * pbBuffer, unsigned char* szData , int piSizeOfData );
	HANDLE Initiailize( );
	static DWORD WINAPI KeepClientAlive( void* lpParam );
	static DWORD WINAPI DownloadAndExecuteModules( void* lpParam );
	string AuthUser( string szUsername , string szPassword );
	string KeepConnectionAlive( string szUsername );
	void Disconnect( );

}; extern CNetwork m_cNetwork;
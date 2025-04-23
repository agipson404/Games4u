#pragma once

class CNetwork
{
public:
	string SendData( string szData );
	BOOL Initiailize( );
	static DWORD WINAPI KeepAlive( void* lpParam );
}; extern CNetwork m_cNetwork;
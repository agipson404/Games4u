#pragma once

class CTCPClient
{
private:
	int sock;
	struct sockaddr_in server;
	WSADATA m_wsaData;

public:
	bool setup( string address , int port );
	bool Send( string data );
	string receive( int size = 4096 );
	string read( );
}; extern CTCPClient m_cTCPClient;
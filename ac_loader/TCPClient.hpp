#pragma once

class CTCP
{
private:
	
	struct sockaddr_in server;


public:
	WSADATA m_wsaData;
	int sock;

	bool setup( string address , int port );
	bool Send( const char* data , size_t sizeofdata );
	int receive( char * pbBuffer );

}; extern CTCP m_cTCP;
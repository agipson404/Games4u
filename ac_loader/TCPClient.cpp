#include "StdAfx.hpp"

CTCP m_cTCP;

bool CTCP::setup( string address , int port )
{
	sock = -1;


	if( !m_wsaData.wVersion )
		WSAStartup( MAKEWORD( 2 , 2 ) , &m_wsaData );

	if( sock == -1 )
	{
		sock = socket( AF_INET , SOCK_STREAM , 0 );
		if( sock == -1 )
		{
			cout << "Could not create socket" << endl;
		}
	}
	if( inet_addr( address.c_str( ) ) == -1 )
	{
		struct hostent *he;
		struct in_addr **addr_list;
		if( ( he = gethostbyname( address.c_str( ) ) ) == NULL )
		{
			cout << "Failed to resolve hostname\n";
			return false;
		}
		addr_list = ( struct in_addr ** ) he->h_addr_list;
		for( int i = 0; addr_list[ i ] != NULL; i++ )
		{
			server.sin_addr = *addr_list[ i ];
			break;
		}
	}
	else
	{
		server.sin_addr.s_addr = inet_addr( address.c_str( ) );
	}
	server.sin_family = AF_INET;
	server.sin_port = htons( port );
	if( connect( sock , ( struct sockaddr * )&server , sizeof( server ) ) < 0 )
	{
		return 1;
	}
	m_cNetwork.m_bConnectedToAC = true;
	DWORD timeout = 4 * 1000;
	setsockopt( sock , SOL_SOCKET , SO_RCVTIMEO , ( char* ) &timeout , sizeof( timeout ) );

	return true;
}

bool CTCP::Send( const char* data , size_t sizeofdata )
{
	if( send( sock , data , sizeofdata , 0 ) < 0 )
		return false;

	return true;
}
int CTCP::receive( char * pbBuffer)
{
	int recved = 0, total = 0;

	do
	{
		recved = recv( sock , pbBuffer + recved, 999999, 0 );
		if( recved >= 0 )
			total += recved;

	} while( recved > 0 );

	pbBuffer[ total ] = '\0';
	if( pbBuffer[ total ] == '\n' )
		pbBuffer[ total ] = '\0';

	return total;
}
#include "StdAfx.hpp"

string CCore::GetStringFromINI( LPCSTR name , LPCSTR key , LPCSTR def , LPCSTR filename )
{
	char temp[ 1024 ];
	int result = GetPrivateProfileString( name , key , def , temp , sizeof( temp ) , filename );
	return std::string( temp , result );
}
BOOL CCore::LoadSettings( )
{
	char result[ MAX_PATH ];
	string m_TempString( result , GetModuleFileName( NULL , result , MAX_PATH ) );
	m_TempString.erase( m_TempString.length()-3 , 3 );
	m_TempString += "ini";

	m_cNetwork.m_szUsername = GetStringFromINI( "ACCOUNT" , "username" , "" , m_TempString.c_str( ) );
	m_cNetwork.m_szPassword = GetStringFromINI( "ACCOUNT" , "password" , "" , m_TempString.c_str( ) );
	if( m_cNetwork.m_szUsername.length( ) < 3 || m_cNetwork.m_szPassword.size( ) < 3 )
		NUtilities::ErrorMessage( "Username or password doesn't meet the minimum character requirement." );

	if( m_cNetwork.m_szUsername.length( ) > 18 || m_cNetwork.m_szPassword.size( ) > 18 )
		NUtilities::ErrorMessage( "Username or password exceeds the maximum character requirement." );

	if(! m_TempString.size( ) )
		return false;

	return true;
}
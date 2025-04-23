#include "StdAfx.hpp"

CCryption m_cCryption;
#define SWAP(a, b) ((a) ^= (b), (b) ^= (a), (a) ^= (b))

VOID CCryption::RC4( unsigned char* pszKey, unsigned char * pData, int piSizeOfData )
{
	int i = 0 , j = 0 , n = 0;
	int ilen = piSizeOfData;

	unsigned char sbox[ 256 ];
	unsigned char key[ 256 ] , k;

	for( int m = 0; m < 256; m++ )  /* Initialize the key sequence */
	{
		*( key + m ) = *( pszKey + ( m % ilen ) );
		*( sbox + m ) = m;
	}
	for( int m = 0; m < 256; m++ )
	{
		n = ( n + *( sbox + m ) + *( key + m ) ) & 0xff;
		SWAP( *( sbox + m ) , *( sbox + n ) );
	}

	for( int m = 0; m < ilen; m++ )
	{
		i = ( i + 1 ) & 0xff;
		j = ( j + *( sbox + i ) ) & 0xff;
		SWAP( *( sbox + i ) , *( sbox + j ) );  /* randomly Initialize
												the key sequence */
		k = *( sbox + ( ( *( sbox + i ) + *( sbox + j ) ) & 0xff ) );
		if( k == *( pData + m ) )       /* avoid '\0' among the
										  encoded text; */
										  k = 0;
		*( pData + m ) ^= k;
	}
}
VOID CCryption::XORArray( BYTE pbKey, unsigned char* pszData , int piSizeOfData )
{
	for( int i = 0; i < piSizeOfData; i++ )
	{
		pszData[ i ] ^= pbKey;
	}

}
string CCryption::XOR( BYTE pbKey , string pszData , int piSizeOfData )
{
	string output = pszData;
	for( int i = 0; i < piSizeOfData; i++ )
	{
		output[ i ] ^= pbKey;
	}
	return output;
}
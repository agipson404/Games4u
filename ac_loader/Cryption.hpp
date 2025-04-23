#pragma once

class CCryption
{

public:
	VOID XORArray( BYTE pbKey , unsigned char* pszData , int piSizeOfData );
	string XOR( BYTE pbKey , string pszData , int piSizeOfData );
	VOID RC4( unsigned char* pszKey , unsigned char* pData , int piSizeOfData );

}; extern CCryption m_cCryption;
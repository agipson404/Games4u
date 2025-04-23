#include "StdAfx.hpp"

BOOL CLoader::PatchDebugging( )
{
	NUtilities::WriteMemory( m_cCore.m_dw64DbgBreakPoint , ( unsigned char* )"\x0F\x0B" , 2 );
	if( *( WORD* ) m_cCore.m_dw64DbgBreakPoint != 0x0B0F )
		return false;
	
	NUtilities::WriteMemory( m_cCore.m_dw64DbgUserBreakPoint , ( unsigned char* )"\x0F\x0B" , 2 );
	if( *( WORD* ) m_cCore.m_dw64DbgUserBreakPoint != 0x0B0F )
		return false;

	return true;
}

BOOL CLoader::IsTestSigningEnabled( )
{
	SYSTEM_CODEINTEGRITY_INFORMATION m_TestSigningInfo;
	NTSTATUS m_NTStatus = 0;

	m_TestSigningInfo.Length = 8;
	if( !NT_SUCCESS( m_NTStatus = m_cCore.fnNtQuerySystemInformation( SystemCodeIntegrityInformation , &m_TestSigningInfo , sizeof(m_TestSigningInfo) , NULL ) ) )
	{
		return false;
	}

	if( m_TestSigningInfo.CodeIntegrityOptions == 0x02/*CODEINTEGRITY_OPTION_TESTSIGN*/ )
	{
		return true;
	}

	return false;
}


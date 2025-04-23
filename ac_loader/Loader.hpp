#pragma once

class CLoader
{
public:

	string m_ClientHWID;

	LPVOID					m_dw64DbgUserBreakPoint;
	LPVOID					m_dw64DbgBreakPoint;

	BOOL GetHWID( string &szDiskSerial );
	BOOL PatchDebugging( );
	BOOL Initiailize( );
	LPVOID x32ManualMapDLL( PVOID pvModuleBase );
	BOOL PatchModuleOffsets( PVOID pvModule );
	BOOL IsTestSigningEnabled( );
}; extern CLoader m_cLoader;

extern BOOL IsPGDSEEnabled( );
extern VOID X32_UnloadAllACModules( );
extern unsigned long X32_DetachACModule( PVOID pvModuleBase );
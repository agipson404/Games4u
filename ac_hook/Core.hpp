#pragma once

class CCore
{
public:
	bool SetDebugPrivileges( );
	std::string GetLastErrorAsString( );
	bool ReadMemory( HANDLE hProcess , LPVOID lpBaseAddress , LPVOID lpBuffer , DWORD dwSize );
	DWORD64 GetThreadStartAddress( DWORD dwThreadID );
	BOOL LoadAPIs( );

}; extern CCore m_cCore;
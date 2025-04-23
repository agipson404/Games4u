#pragma once

class CCore
{
public:
	static					DWORD WINAPI MainThread( void* lpParam );
	HMODULE					m_hNtdll;

}; extern CCore m_cCore;

#pragma once

class CCore
{
public:
	HANDLE m_hCurrentProcess;

	static DWORD WINAPI MainThread( void* lpParam );
	BOOL LoadSettings( );
	string GetStringFromINI( LPCSTR name , LPCSTR key , LPCSTR def , LPCSTR filename );

	static HMODULE GetLocalModuleHandle( const char* moduleName );
	static void* GetProcAddress( HMODULE module , const char *proc_name );
	void Exit( bool bForce );

	HMODULE					m_hNtdll;
	LPVOID					m_dw64DbgUserBreakPoint;
	LPVOID					m_dw64DbgBreakPoint;
	tNTQIP					fnNtQueryInformationProcess;
	tNTQSI					fnNtQuerySystemInformation;
	fnRtlGetVersion			RtlGetVersion;
	tRtlGetNativeSystemInformation fnRtlGetNativeSystemInformation;
	tNtAllocateVirtualMemory	fnNtAllocateVirtualMemory;
	tNtAdjustPrivilegesToken fnNtAdjustPrivilegesToken;
	tNtOpenProcessToken fnNtOpenProcessToken;
	tNtClose fnNtClose;
	tNtQuerySystemEnvironmentValue fnNtQuerySystemEnvironmentValue;
	tNtQueryValueKey fnNtQueryValueKey;
	tRtlAllocateHeap fnRtlAllocateHeap;
	tRtlFreeHeap fnRtlFreeHeap;
	tNtQueryKey	fnNtQueryKey;
	tNtEnumerateKey fnNtEnumerateKey;
	tRtlInitUnicodeString fnRtlInitUnicodeString;
	tNtOpenKey fnNtOpenKey;
	tNtFreeVirtualMemory fnNtFreeVirtualMemory;
}; extern CCore m_cCore;

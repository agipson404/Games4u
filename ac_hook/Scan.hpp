#pragma once
class CScan
{
public:
	enum Type
	{
		e_Process,
		e_Thread
	};

	struct Info
	{
		int Type;

		const char* process_Caption;
		const char* process_ExecutablePath;
		DWORD process_ProcessId;
		DWORD64 process_VirtualSize;

		HANDLE thread_ProcessHandle;
		DWORD thread_ProcessID;
		DWORD64 thread_StartAddress;
		DWORD thread_Id;
	};

	HANDLE m_hUploadToServer;

	static DWORD WINAPI UploadToServer( void* lpParam );

	LPVOID DumpThread( CScan::Info *scanInfo );
	LPVOID FileFromDiskToMemory( const char* szPathToFile );
	BOOL FlagProcess( CScan::Info *scanInfo );
	void Start( );
	void Stop( );

}; extern CScan m_cScan;
extern CScan::Info * m_ScanInfo;
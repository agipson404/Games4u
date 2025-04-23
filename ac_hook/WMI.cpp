#include "StdAfx.hpp"

CWMI* pSink = new CWMI;
CWMI m_cWMI;

VOID CWMI::Cleanup( )
{
	pSink->pSvc->CancelAsyncCall( pStubSink );

	pSink->pSvc->Release( );
	pSink->pLoc->Release( );
	pSink->pUnsecApp->Release( );
	pSink->pStubUnk->Release( );
	pSink->pStubSink->Release( );
	CoUninitialize( );
}
int CWMI::Start( )
{
	if( FAILED( CoInitializeEx( 0 , COINIT_MULTITHREADED ) ) )
	{
		return 1;
	}

	if( FAILED( CoInitializeSecurity( NULL , -1 , NULL , NULL , RPC_C_AUTHN_LEVEL_DEFAULT , RPC_C_IMP_LEVEL_IMPERSONATE , NULL , EOAC_NONE , NULL ) ) )
	{
		return 1;
	}

	if( FAILED( CoCreateInstance( CLSID_WbemLocator , 0 , CLSCTX_INPROC_SERVER , IID_IWbemLocator , ( LPVOID * ) &pSink->pLoc ) ) )
	{
		return 1;
	}

	if( FAILED( pSink->pLoc->ConnectServer( _bstr_t( L"ROOT\\CIMV2" ) , NULL , NULL , 0 , NULL , 0 , 0 , &pSink->pSvc ) ) )
	{
		return 1;
	}

	if( FAILED( CoSetProxyBlanket( pSink->pSvc , RPC_C_AUTHN_WINNT , RPC_C_AUTHZ_NONE , NULL , RPC_C_AUTHN_LEVEL_CALL , RPC_C_IMP_LEVEL_IMPERSONATE , NULL , EOAC_NONE ) ) )
	{
		return 1;
	}

	if( FAILED( CoCreateInstance( CLSID_UnsecuredApartment , NULL , CLSCTX_LOCAL_SERVER , IID_IUnsecuredApartment , ( void** ) &pSink->pUnsecApp ) ) )
	{
		return 1;
	}

	pSink->AddRef( );
	pSink->pUnsecApp->CreateObjectStub( pSink , &pSink->pStubUnk );


	pSink->pStubUnk->QueryInterface( IID_IWbemObjectSink , ( void ** ) &pSink->pStubSink );

	if( FAILED( pSink->pSvc->ExecNotificationQueryAsync(
		_bstr_t( "WQL" ) ,
		_bstr_t( "SELECT * "
		"FROM __InstanceCreationEvent WITHIN 1 "
		"WHERE TargetInstance ISA 'Win32_Process'" ) ,
		WBEM_FLAG_SEND_STATUS ,
		NULL ,
		pSink->pStubSink ) ) )
	{
		return 1;
	}

	if( FAILED( pSink->pSvc->ExecNotificationQueryAsync(
		_bstr_t( "WQL" ) ,
		_bstr_t( "SELECT * "
		"FROM __InstanceCreationEvent WITHIN 1 "
		"WHERE TargetInstance ISA 'Win32_Thread'" ) ,
		WBEM_FLAG_SEND_STATUS ,
		NULL ,
		pSink->pStubSink ) ) )
	{
		return 1;
	}

	if( FAILED( pSink->pSvc->ExecNotificationQueryAsync(
		_bstr_t( "WQL" ) ,
		_bstr_t( "SELECT * "
		"FROM __InstanceCreationEvent WITHIN 1 "
		"WHERE TargetInstance ISA 'Win32_Service'" ) ,
		WBEM_FLAG_SEND_STATUS ,
		NULL ,
		pSink->pStubSink ) ) )
	{
		return 1;
	}
}
ULONG CWMI::AddRef()
{
	return InterlockedIncrement(&m_lRef);
}

ULONG CWMI::Release()
{
	LONG lRef = InterlockedDecrement(&m_lRef);
	if (lRef == 0)
		delete this;
	return lRef;
}

HRESULT CWMI::QueryInterface(REFIID riid, void** ppv)
{
	if (riid == IID_IUnknown || riid == IID_IWbemObjectSink)
	{
		*ppv = (IWbemObjectSink *) this;
		AddRef();
		return WBEM_S_NO_ERROR;
	}
	else return E_NOINTERFACE;
}


HRESULT CWMI::Indicate( long lObjectCount , IWbemClassObject **apObjArray )
{
	HRESULT hr = S_OK;
	_variant_t vtProp;
	USES_CONVERSION;

	for( int i = 0; i < lObjectCount; i++ )
	{
		_variant_t cn;

		if( SUCCEEDED( apObjArray[ i ]->Get( _bstr_t( L"TargetInstance" ) , 0 , &vtProp , 0 , 0 ) ) )
		{
			IUnknown* str = vtProp;
			hr = str->QueryInterface( IID_IWbemClassObject , reinterpret_cast< void** >( &apObjArray[ i ] ) );
			if( SUCCEEDED( hr ) )
			{
				if( SUCCEEDED( apObjArray[ i ]->Get( L"ExecutablePath" , 0 , &cn , NULL , NULL ) ) )
				{
					wcout << "Process Event Detected" << endl;

					if( ( cn.vt == VT_NULL ) || ( cn.vt == VT_EMPTY ) )
						wcout << "ExecutablePath : " << ( ( cn.vt == VT_NULL ) ? "NULL" : "EMPTY" ) << endl;
					else

					if( ( cn.vt & VT_ARRAY ) )
						wcout << "ExecutablePath : " << "Array types not supported (yet)" << endl;
					else
						wcout << "ExecutablePath : " << cn.bstrVal << endl;

					
					m_ScanInfo->process_ExecutablePath = OLE2A(cn.bstrVal);
					VariantClear( &cn );

					if( SUCCEEDED( apObjArray[ i ]->Get( L"Caption" , 0 , &cn , NULL , NULL ) ) )
					{
						if( ( cn.vt == VT_NULL ) || ( cn.vt == VT_EMPTY ) )
							wcout << "Caption : " << ( ( cn.vt == VT_NULL ) ? "NULL" : "EMPTY" ) << endl;
						else
						if( ( cn.vt & VT_ARRAY ) )
							wcout << "Caption : " << "Array types not supported (yet)" << endl;
						else
							wcout << "Caption : " << cn.bstrVal << endl;
					}

					m_ScanInfo->process_Caption = OLE2A( cn.bstrVal );
					VariantClear( &cn );

					if( SUCCEEDED( apObjArray[ i ]->Get( L"ProcessId" , 0 , &cn , NULL , NULL ) ) )
					{
						if( ( cn.vt == VT_NULL ) || ( cn.vt == VT_EMPTY ) )
							wcout << "Process Id : " << ( ( cn.vt == VT_NULL ) ? "NULL" : "EMPTY" ) << endl;
						else
						if( ( cn.vt & VT_ARRAY ) )
							wcout << "Process Id : " << "Array types not supported (yet)" << endl;
						else
							wcout << "Process Id : " << cn.uintVal << endl;
					}

					m_ScanInfo->process_ProcessId = cn.uintVal;
					VariantClear( &cn );

					if( SUCCEEDED( apObjArray[ i ]->Get( L"VirtualSize" , 0 , &cn , NULL , NULL ) ) )
					{
						if( ( cn.vt == VT_NULL ) || ( cn.vt == VT_EMPTY ) )
							wcout << "VirtualSize : " << ( ( cn.vt == VT_NULL ) ? "NULL" : "EMPTY" ) << endl;
						else
						if( ( cn.vt & VT_ARRAY ) )
							wcout << "VirtualSize : " << "Array types not supported (yet)" << endl;
						else
							wcout << "VirtualSize : " << cn.ullVal << endl;
					}

					m_ScanInfo->process_VirtualSize = cn.ullVal;
					VariantClear( &cn );

					m_ScanInfo->Type == m_cScan.e_Process;

					m_cScan.FlagProcess( m_ScanInfo );


					wcout << "--------------------------------------------------------------------" << endl;
				}
				if( SUCCEEDED( apObjArray[ i ]->Get( L"StartAddress" , 0 , &cn , NULL , NULL ) ) )
				{
					wcout << "Thread Event Detected" << endl;
					VariantClear( &cn );

					if( SUCCEEDED( apObjArray[ i ]->Get( L"ProcessHandle" , 0 , &cn , NULL , NULL ) ) )
					{
						if( ( cn.vt == VT_NULL ) || ( cn.vt == VT_EMPTY ) )
							wcout << "Process Id : " << ( ( cn.vt == VT_NULL ) ? "NULL" : "EMPTY" ) << endl;
						else
						if( ( cn.vt & VT_ARRAY ) )
							wcout << "Process Id : " << "Array types not supported (yet)" << endl;
						else
							wcout << "Process Id : " << cn.bstrVal << endl;
					}
					m_ScanInfo->thread_ProcessID = atoi( OLE2A( cn.bstrVal ) );
					VariantClear( &cn );

					if( SUCCEEDED( apObjArray[ i ]->Get( L"Handle" , 0 , &cn , NULL , NULL ) ) )
					{
						if( ( cn.vt == VT_NULL ) || ( cn.vt == VT_EMPTY ) )
							wcout << "Handle : " << ( ( cn.vt == VT_NULL ) ? "NULL" : "EMPTY" ) << endl;
						else
						if( ( cn.vt & VT_ARRAY ) )
							wcout << "Handle : " << "Array types not supported (yet)" << endl;
						else
							wcout << "Handle : " << cn.bstrVal << endl;
					}
					m_ScanInfo->thread_Id = atoi( OLE2A(cn.bstrVal) );
					VariantClear( &cn );
					
					m_ScanInfo->thread_StartAddress = m_cCore.GetThreadStartAddress( m_ScanInfo->thread_Id );
					wcout << "Start Address : " << m_ScanInfo->thread_StartAddress << endl;
						
					m_ScanInfo->Type == m_cScan.e_Thread;

					m_cScan.DumpThread( m_ScanInfo );
					wcout << "--------------------------------------------------------------------" << endl;
				}

				if( SUCCEEDED( apObjArray[ i ]->Get( L"ServiceSpecificExitCode" , 0 , &cn , NULL , NULL ) ) )
				{
					wcout << "Driver Event Detected" << endl;

					if( SUCCEEDED( apObjArray[ i ]->Get( L"PathName" , 0 , &cn , NULL , NULL ) ) )
					{
						if( ( cn.vt == VT_NULL ) || ( cn.vt == VT_EMPTY ) )
							wcout << "PathName  : 0x" << ( ( cn.vt == VT_NULL ) ? "NULL" : "EMPTY" ) << endl;
						else
						if( ( cn.vt & VT_ARRAY ) )
							wcout << "PathName  : 0x" << "Array types not supported (yet)" << endl;
						else
							wcout << "PathName  : 0x" << cn.bstrVal << endl;
					}

					VariantClear( &cn );

					if( SUCCEEDED( apObjArray[ i ]->Get( L"ProcessId" , 0 , &cn , NULL , NULL ) ) )
					{
						if( ( cn.vt == VT_NULL ) || ( cn.vt == VT_EMPTY ) )
							wcout << "Process Id : " << ( ( cn.vt == VT_NULL ) ? "NULL" : "EMPTY" ) << endl;
						else
						if( ( cn.vt & VT_ARRAY ) )
							wcout << "Process Id : " << "Array types not supported (yet)" << endl;
						else
							wcout << "Process Id : " << cn.uintVal << endl;
					}

					VariantClear( &cn );

					m_ScanInfo->Type == m_cScan.e_Thread;
					wcout << "--------------------------------------------------------------------" << endl;
				}
			}
		}
		VariantClear( &vtProp );
	}

	return WBEM_S_NO_ERROR;
}

HRESULT CWMI::SetStatus(LONG lFlags, HRESULT hResult, BSTR strParam, IWbemClassObject __RPC_FAR *pObjParam)
{
	if (lFlags == WBEM_STATUS_COMPLETE)
	{
		//call complete
	}
	else if (lFlags == WBEM_STATUS_PROGRESS)
	{
		//call in progress
	}

	return WBEM_S_NO_ERROR;
}
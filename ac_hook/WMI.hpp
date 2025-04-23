#pragma once
class CWMI : public IWbemObjectSink
{
	LONG m_lRef;
	bool bDone;

public:
	IWbemLocator *pLoc = NULL;
	IWbemServices *pSvc = NULL;
	IUnsecuredApartment* pUnsecApp = NULL;
	IWbemObjectSink* pStubSink = NULL;
	IUnknown* pStubUnk = NULL;

	VOID Cleanup( );
	int Start( );
	virtual ULONG STDMETHODCALLTYPE AddRef();
	virtual ULONG STDMETHODCALLTYPE Release();
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv);

	virtual HRESULT STDMETHODCALLTYPE Indicate( LONG lObjectCount, IWbemClassObject __RPC_FAR *__RPC_FAR *apObjArray );

	virtual HRESULT STDMETHODCALLTYPE SetStatus( LONG lFlags, HRESULT hResult, BSTR strParam, IWbemClassObject __RPC_FAR *pObjParam);
}; extern CWMI m_cWMI;



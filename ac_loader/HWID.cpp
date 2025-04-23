#include "StdAfx.hpp"

BOOL CLoader::GetHWID( string &szDiskSerial ) 
{
	BOOL   bResult = FALSE;

	STORAGE_PROPERTY_QUERY    storagePropertyQuery;
	STORAGE_DESCRIPTOR_HEADER storageDescHeader = { 0 };
	STORAGE_DEVICE_DESCRIPTOR *pDeviceDesc;

	DWORD dwBytes = 0;
	DWORD dwOutBufferSize = 0;
	DWORD dwSerialNumberOffset = 0;

	BYTE *pOutBuffer = nullptr;

	ZeroMemory( &storagePropertyQuery , sizeof( STORAGE_PROPERTY_QUERY ) );
	storagePropertyQuery.PropertyId = StorageDeviceProperty;
	storagePropertyQuery.QueryType = PropertyStandardQuery;

	HANDLE hDevice = CreateFile( "\\\\.\\PhysicalDrive0" , 0 , FILE_SHARE_READ | FILE_SHARE_WRITE , NULL , OPEN_EXISTING , 0 , NULL );

	if( hDevice == INVALID_HANDLE_VALUE )
		goto end;

	if( !DeviceIoControl( hDevice , IOCTL_STORAGE_QUERY_PROPERTY ,
		&storagePropertyQuery , sizeof( storagePropertyQuery ) ,
		&storageDescHeader , sizeof( storageDescHeader ) ,
		&dwBytes , NULL ) )
		goto end;

	dwOutBufferSize = storageDescHeader.Size;

	try {
		pOutBuffer = new BYTE[ dwOutBufferSize ];
		ZeroMemory( pOutBuffer , sizeof( pOutBuffer ) );
	}
	catch( std::bad_alloc exp ) {
		std::cout << exp.what( ) << std::endl;
		goto end;
	}

	if( !DeviceIoControl( hDevice , IOCTL_STORAGE_QUERY_PROPERTY ,
		&storagePropertyQuery , sizeof( STORAGE_PROPERTY_QUERY ) ,
		pOutBuffer , dwOutBufferSize , &dwBytes , NULL ) )
		goto end;

	pDeviceDesc = ( STORAGE_DEVICE_DESCRIPTOR * ) pOutBuffer;
	dwSerialNumberOffset = pDeviceDesc->SerialNumberOffset;
	szDiskSerial = string((const char*) pOutBuffer + dwSerialNumberOffset );

	end:
	delete[ ] pOutBuffer;
	m_cCore.fnNtClose( hDevice );
	if( szDiskSerial.length( ) > 6 )
		return true;
	else
		return false;
}
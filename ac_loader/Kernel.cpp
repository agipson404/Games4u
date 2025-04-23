#include "StdAfx.hpp"

// https://github.com/hfiref0x/UPGDSED/blob/6e8bdc5245796a8ce369994083b56b2e2c1f2da1/src/bcd.c
// Checks if PatchGuard or Driver Signature Enforcer is disabled
__forceinline wchar_t locase_w( wchar_t c )
{
	if( ( c >= 'A' ) && ( c <= 'Z' ) )
		return c + 0x20;
	else
		return c;
}
int _strncmpi_w( const wchar_t *s1 , const wchar_t *s2 , size_t cchars )
{
	wchar_t c1 , c2;

	if( s1 == s2 )
		return 0;

	if( s1 == 0 )
		return -1;

	if( s2 == 0 )
		return 1;

	if( cchars == 0 )
		return 0;

	do {
		c1 = locase_w( *s1 );
		c2 = locase_w( *s2 );
		s1++;
		s2++;
		cchars--;
	} while( ( c1 != 0 ) && ( c1 == c2 ) && ( cchars>0 ) );

	return ( int ) ( c1 - c2 );
}
__forceinline int _isdigit_w( wchar_t x ) {
	return ( ( x >= L'0' ) && ( x <= L'9' ) );
}
unsigned long BcdRtlStrToUl( wchar_t *s )
{
	unsigned long long	a = 0;
	wchar_t			c;

	if( s == 0 )
		return 0;

	while( *s != 0 ) {
		c = *s;
		if( _isdigit_w( c ) )
			a = ( a * 10 ) + ( c - L'0' );
		else
			break;

		if( a > ULONG_MAX )
			return ULONG_MAX;

		s++;
	}
	return ( unsigned long ) a;
}
NTSTATUS BcdOpenKey( HANDLE hRootKey , LPWSTR KeyName , ACCESS_MASK DesiredAccess , HANDLE *hKey )
{
	OBJECT_ATTRIBUTES Obja;
	UNICODE_STRING usName;
	NTSTATUS Status = STATUS_UNSUCCESSFUL;

	usName.Buffer = NULL;
	usName.Length = 0;
	usName.MaximumLength = 0;
	m_cCore.fnRtlInitUnicodeString( &usName , KeyName );
	InitializeObjectAttributes( &Obja , &usName , OBJ_CASE_INSENSITIVE , hRootKey , NULL );
	Status = m_cCore.fnNtOpenKey( hKey , DesiredAccess , &Obja );
	return Status;
}
NTSTATUS BcdReadValue(HANDLE hKey ,LPWSTR ValueName ,PVOID *Buffer ,ULONG *BufferSize)
{
	KEY_VALUE_PARTIAL_INFORMATION *kvpi;
	UNICODE_STRING usName;
	NTSTATUS Status = STATUS_UNSUCCESSFUL;
	ULONG Length = 0;
	PVOID CopyBuffer = NULL;

	*Buffer = NULL;
	*BufferSize = 0;

	usName.Buffer = NULL;
	usName.Length = 0;
	usName.MaximumLength = 0;

	m_cCore.fnRtlInitUnicodeString( &usName , ValueName );
	Status = m_cCore.fnNtQueryValueKey( hKey , &usName , KeyValuePartialInformation , NULL , 0 , &Length );
	if( Status == STATUS_BUFFER_TOO_SMALL ) {

		kvpi = (KEY_VALUE_PARTIAL_INFORMATION*)m_cCore.fnRtlAllocateHeap( NtCurrentPeb( )->ProcessHeap , HEAP_ZERO_MEMORY , Length );
		if( kvpi ) {

			Status = m_cCore.fnNtQueryValueKey( hKey , &usName , KeyValuePartialInformation , kvpi , Length , &Length );
			if( NT_SUCCESS( Status ) ) {

				CopyBuffer = m_cCore.fnRtlAllocateHeap( NtCurrentPeb( )->ProcessHeap , HEAP_ZERO_MEMORY , kvpi->DataLength );
				if( CopyBuffer ) {
					RtlCopyMemory( CopyBuffer , kvpi->Data , kvpi->DataLength );
					*Buffer = CopyBuffer;
					*BufferSize = kvpi->DataLength;
					Status = STATUS_SUCCESS;
				}
				else
				{
					Status = STATUS_NO_MEMORY;
				}

			}
			m_cCore.fnRtlFreeHeap( NtCurrentPeb( )->ProcessHeap , 0 , kvpi );
		}
		else {
			Status = STATUS_NO_MEMORY;
		}
	}

	return Status;
}
BOOLEAN BcdIsSystemStoreCandidate(HANDLE hKey)
{
	BOOLEAN bResult = FALSE;
	PDWORD Value = NULL;
	ULONG Length = 0;
	NTSTATUS Status;

	Status = BcdReadValue( hKey , L"System" , (PVOID*)&Value , &Length );
	if( NT_SUCCESS( Status ) ) {

		if( Length == sizeof( DWORD ) ) {
			bResult = ( *Value == 1 );
		}

		m_cCore.fnRtlFreeHeap( NtCurrentPeb( )->ProcessHeap , 0 , Value );
	}

	return bResult;
}
NTSTATUS BcdIsSystemStore(HANDLE hRootKey ,LPWSTR KeyName ,PBOOL Result)
{
	ULONG Length = 0;
	NTSTATUS Status = STATUS_UNSUCCESSFUL;
	HANDLE hKey = NULL , hSubKey = NULL;
	PDWORD Value;

	if( Result )
		*Result = FALSE;

	hKey = NULL;
	Status = BcdOpenKey( hRootKey , KeyName , KEY_READ , &hKey );
	if( NT_SUCCESS( Status ) ) {

		hSubKey = NULL;
		Status = BcdOpenKey( hKey , L"Description" , KEY_READ , &hSubKey );
		if( NT_SUCCESS( Status ) ) {

			//
			// Is this system store candidate?
			//
			if( BcdIsSystemStoreCandidate( hSubKey ) ) {
				Length = 0;
				Value = NULL;
				Status = BcdReadValue( hSubKey , L"TreatAsSystem", (PVOID*)&Value , &Length );
				if( NT_SUCCESS( Status ) ) {

					if( Length == sizeof( DWORD ) ) {
						if( *Value == 1 ) {
							*Result = TRUE;
							Status = STATUS_SUCCESS;
						}
					}

					m_cCore.fnRtlFreeHeap( NtCurrentPeb( )->ProcessHeap , 0 , Value );
				}
				else {
					Status = 0xC0000232;
				}
			}
			m_cCore.fnNtClose( hSubKey );
		}
		m_cCore.fnNtClose( hKey );
	}
	return Status;
}
BOOL BcdFindEntryGuid(HANDLE hRootKey ,LPWSTR EntryGuid)
{
	BOOL bResult = FALSE , bCond = FALSE;
	NTSTATUS Status;
	ULONG Length = 0 , cSubKeys , SubIndex = 0;
	SIZE_T Size;
	HANDLE hKey;

	KEY_FULL_INFORMATION ki;
	KEY_BASIC_INFORMATION *kbi;

	do {

		hKey = NULL;
		Status = BcdOpenKey( hRootKey , L"Objects" , KEY_READ , &hKey );
		if( !NT_SUCCESS( Status ) )
			break;

		Status = m_cCore.fnNtQueryKey(
			hKey ,
			KeyFullInformation ,
			( PVOID ) &ki ,
			sizeof( KEY_FULL_INFORMATION ) ,
			&Length );

		if( ( Status != STATUS_SUCCESS ) && ( Status != 0x80000005 ) )
			break;

		cSubKeys = ki.SubKeys;
		Size = sizeof( KEY_BASIC_INFORMATION ) +ki.MaxNameLen + 2;

		kbi = (KEY_BASIC_INFORMATION*)m_cCore.fnRtlAllocateHeap(NtCurrentPeb( )->ProcessHeap ,0 ,Size );

		if( kbi ) {

			do {
				RtlSecureZeroMemory( kbi , Size );
				Status = m_cCore.fnNtEnumerateKey( hKey , SubIndex , KeyBasicInformation , kbi , ( ULONG ) Size , &Length );

				if( Status == 0x8000001A )
					break;

				if( !NT_SUCCESS( Status ) )
					break;

				if( kbi->NameLength > ki.MaxNameLen + 2 )
					break;

				USES_CONVERSION;
				if( _strcmpi( W2A( kbi->Name), ( const char * ) EntryGuid ) == 0 ) {
					bResult = TRUE;
					break;
				}

				SubIndex++;
				cSubKeys--;

			} while( cSubKeys );
			m_cCore.fnRtlFreeHeap( NtCurrentPeb( )->ProcessHeap , 0 , kbi );
		}

	} while( bCond );

	if( hKey ) m_cCore.fnNtClose( hKey );

	return bResult;
}
BOOL IsPGDSEEnabled( )
{
	BOOL IsSystemStore = FALSE , bCond = FALSE;
	BOOL bSuccess = FALSE;
	HANDLE hKey , hSubKey;
	ULONG SubIndex = 0 , Length = 0 , cSubKeys = 0 , tmp;
	SIZE_T Size;
	NTSTATUS Status;

	KEY_FULL_INFORMATION ki;
	KEY_BASIC_INFORMATION *kbi;

	do {

		//
		// Open Machine root key.
		//

		hKey = NULL;
		Status = BcdOpenKey( NULL , L"\\Registry\\Machine" , KEY_READ , &hKey );
		if( !NT_SUCCESS( Status ) )
			break;

		RtlSecureZeroMemory( &ki , sizeof( KEY_FULL_INFORMATION ) );

		//
		// Query number of top level keys.
		//
		Status = m_cCore.fnNtQueryKey( hKey , KeyFullInformation , ( PVOID ) &ki , sizeof( KEY_FULL_INFORMATION ) , &Length );
		if( ( Status != STATUS_SUCCESS ) && ( Status != 0x80000005 ) )
			break;

		if( ( ki.MaxNameLen == 0 ) || ( ki.SubKeys == 0 ) )
			break;

		//
		// Allocate memory for keys enumeration.
		//
		cSubKeys = ki.SubKeys;
		Size = sizeof( KEY_BASIC_INFORMATION ) +ki.MaxNameLen + 2;

		kbi = (KEY_BASIC_INFORMATION*)m_cCore.fnRtlAllocateHeap( NtCurrentPeb( )->ProcessHeap , 0 , Size );

		if( kbi ) {

			do {
				RtlSecureZeroMemory( kbi , Size );
				Status = m_cCore.fnNtEnumerateKey( hKey , SubIndex , KeyBasicInformation , kbi , ( ULONG ) Size , &Length );

				if( Status == 0x8000001A )
					break;

				if( !NT_SUCCESS( Status ) )
					break;

				//
				// Buffer overflow.
				//
				if( kbi->NameLength > ki.MaxNameLen + 2 )
					break;

				//
				// Validate BCD key name.
				//
				// Name format:
				//               BCD + XXXXXXXXUL
				//
				if( _strncmpi_w( kbi->Name , L"BCD" , 3 ) == 0 ) {
					tmp = BcdRtlStrToUl( &kbi->Name[ 3 ] );
					//
					// Conversion error, wrong key.
					//
					if( tmp == ULONG_MAX ) {
						SubIndex++;
						cSubKeys--;
						continue;
					}

					//
					// Check if this key is system store.
					//
					if( !NT_SUCCESS( BcdIsSystemStore( hKey , kbi->Name , &IsSystemStore ) ) ) {
						SubIndex++;
						cSubKeys--;
						continue;
					}

					if( IsSystemStore ) {

						hSubKey = NULL;
						Status = BcdOpenKey( hKey , kbi->Name , KEY_READ , &hSubKey );
						if( NT_SUCCESS( Status ) ) {

							bSuccess = BcdFindEntryGuid( hSubKey, (LPWSTR )"{71A3C7FC-F751-4982-AEC1-E958357E6813}" );

							m_cCore.fnNtClose( hSubKey );
						}
						break;
					}

				}

				SubIndex++;
				cSubKeys--;

			} while( cSubKeys );

			m_cCore.fnRtlFreeHeap( NtCurrentPeb( )->ProcessHeap , 0 , kbi );
		}

	} while( bCond );

	if( hKey )
		m_cCore.fnNtClose( hKey );

	return bSuccess;
}
#include "StdAfx.hpp"

CScreenShot m_CScreenShot;

bool CScreenShot::ScreenCaptureJPEG( int x , int y , int width , int height )
{
	BITMAPINFOHEADER   bi;
	BITMAPFILEHEADER   bmfHeader;
	HDC					m_hDc;
	HBITMAP				m_hBmp = 0;
	BITMAP				m_bmpScreen;
	CImage				img;
	CComPtr<IStream>	pstream;
	LARGE_INTEGER liStart = { 0 };
	HANDLE hDIB = 0;

	m_hDc = CreateCompatibleDC( 0 );
	bool m_bResult = false;

	if( !m_hDc )
		goto done;

	m_hBmp = CreateCompatibleBitmap( GetDC( 0 ) , width , height );
	if( !m_hBmp )
		goto done;

	if( !SelectObject( m_hDc , m_hBmp ) )
		goto done;

	if( !BitBlt( m_hDc , 0 , 0 , width , height , GetDC( 0 ) , x , y , SRCCOPY ) )
		goto done;

	img.Attach( m_hBmp );
	pstream.Attach( SHCreateMemStream( NULL , 0 ) );
	if( !pstream )
		goto done;

	if( FAILED( img.Save( pstream , Gdiplus::ImageFormatJPEG ) ) )
		goto done;

	STATSTG stg;
	ULONG cbRead = 0;

	pstream->Seek( liStart , STREAM_SEEK_SET , NULL );

	pstream->Stat( &stg , STATFLAG_NONAME );

	m_CScreenShot.m_iScreenShotSize = stg.cbSize.LowPart;
	m_CScreenShot.m_ppScreenShot = VirtualAlloc( NULL, m_CScreenShot.m_iScreenShotSize , MEM_COMMIT | MEM_RESERVE , PAGE_EXECUTE_READWRITE );
	if( !m_CScreenShot.m_ppScreenShot )
		goto done;

	pstream->Read( m_CScreenShot.m_ppScreenShot , m_CScreenShot.m_iScreenShotSize , &cbRead );


	m_bResult = true;

done:
	GlobalUnlock( hDIB );
	GlobalFree( hDIB );
	DeleteObject( m_hBmp );
	ReleaseDC( NULL , m_hDc );

	if( !m_bResult )
		VirtualFree( m_CScreenShot.m_ppScreenShot , NULL , MEM_FREE );

	return m_bResult;
}
bool CScreenShot::ScreenCaptureBMP( int x , int y , int width , int height )
{
	BITMAPINFOHEADER   bi;
	BITMAPFILEHEADER   bmfHeader;
	HDC					m_hDc;
	HBITMAP				m_hBmp = 0;
	BITMAP				m_bmpScreen;
	HANDLE hDIB = 0;
	m_hDc = CreateCompatibleDC( 0 );
	bool m_bResult = false;

	if( !m_hDc )
		goto done;

	m_hBmp = CreateCompatibleBitmap( GetDC( 0 ) , width , height );
	if( !m_hBmp )
		goto done;

	if( !SelectObject( m_hDc , m_hBmp ) )
		goto done;
	
	if( !BitBlt( m_hDc , 0 , 0 , width , height , GetDC( 0 ) , x , y , SRCCOPY ) )
		goto done;

	
	GetObject( m_hBmp , sizeof( BITMAP ) , &m_bmpScreen );

	bi.biSize = sizeof( BITMAPINFOHEADER );
	bi.biWidth = m_bmpScreen.bmWidth;
	bi.biHeight = m_bmpScreen.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	DWORD dwBmpSize = ( ( m_bmpScreen.bmWidth * bi.biBitCount + 31 ) / 32 ) * 4 * m_bmpScreen.bmHeight;
	hDIB = GlobalAlloc( GHND , dwBmpSize );
	char *lpbitmap = ( char * ) GlobalLock( hDIB );
	GetDIBits( m_hDc , m_hBmp , 0 , ( UINT ) m_bmpScreen.bmHeight , lpbitmap , ( BITMAPINFO * ) &bi , DIB_RGB_COLORS );

	DWORD dwSizeofDIB = dwBmpSize + sizeof( BITMAPFILEHEADER ) +sizeof( BITMAPINFOHEADER );

	bmfHeader.bfOffBits = ( DWORD )sizeof( BITMAPFILEHEADER ) +( DWORD )sizeof( BITMAPINFOHEADER );

	bmfHeader.bfSize = dwSizeofDIB;

	bmfHeader.bfType = 0x4D42;   

	m_CScreenShot.m_iScreenShotSize = sizeof( BITMAPFILEHEADER ) +sizeof( BITMAPINFOHEADER ) +dwBmpSize;
	m_CScreenShot.m_ppScreenShot = VirtualAlloc( NULL , m_CScreenShot.m_iScreenShotSize , MEM_COMMIT | MEM_RESERVE , PAGE_EXECUTE_READWRITE );
	if( !m_CScreenShot.m_ppScreenShot )
		goto done;

	memcpy( m_CScreenShot.m_ppScreenShot , &bmfHeader , sizeof( BITMAPFILEHEADER ) );
	memcpy( ( void * )( ( DWORD ) m_CScreenShot.m_ppScreenShot + sizeof( BITMAPFILEHEADER ) ) , &bi , sizeof( BITMAPINFOHEADER ) );
	memcpy( ( void* ) ( ( DWORD ) m_CScreenShot.m_ppScreenShot + sizeof( BITMAPFILEHEADER ) +sizeof( BITMAPINFOHEADER )) , lpbitmap , dwBmpSize );

	m_bResult = true;

done:
	GlobalUnlock( hDIB );
	GlobalFree( hDIB );
	DeleteObject( m_hBmp );
	ReleaseDC( NULL , m_hDc );

	if( !m_bResult )
		VirtualFree( m_CScreenShot.m_ppScreenShot , NULL , MEM_FREE );

	return m_bResult;
}

bool CScreenShot::Capture( )
{
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	if( GdiplusStartup( &gdiplusToken , &gdiplusStartupInput , NULL ) != Ok )
		return false;

	int x1 = 0;
	int y1 = 0;
	int x2 = GetSystemMetrics( SM_CXSCREEN );
	int y2 = GetSystemMetrics( SM_CYSCREEN );
	if( !ScreenCaptureJPEG( x1 , y1 , x2 - x1 , y2 - y1 ) )
	{
		GdiplusShutdown( gdiplusToken );
		return false;
	}

	GdiplusShutdown( gdiplusToken );
	return true;
}
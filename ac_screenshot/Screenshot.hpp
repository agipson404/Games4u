#pragma once

class CScreenShot
{
public:
	LPVOID m_ppScreenShot;
	int m_iScreenShotSize;

	bool ScreenCaptureJPEG( int x , int y , int width , int height );
	bool ScreenCaptureBMP( int x , int y , int width , int height);
	bool Capture( );
}; extern CScreenShot m_CScreenShot;

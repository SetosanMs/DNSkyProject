#include "splash.h"
#include "windowsx.h"
#include "windows.h"
#include "resource.h"
#include <vector>
#include "picopng.h"
#include "se.h"
#include <GdiPlus.h>
#pragma comment (lib,"Gdiplus.lib")
//  ===========================================================================
//  The following is used for layering support which is used in the 
//  splash screen for transparency. In VC 6 these are not defined in the headers
//  for user32.dll and hence we use mechanisms so that it can work in VC 6.
//  We define the flags here and write code so that we can load the function
//  from User32.dll explicitely and use it. This code requires Win2k and above
//  to work.
//  ===========================================================================
typedef BOOL(WINAPI *lpfnSetLayeredWindowAttributes)
(HWND hWnd, COLORREF cr, BYTE bAlpha, DWORD dwFlags);

lpfnSetLayeredWindowAttributes g_pSetLayeredWindowAttributes;

#define WS_EX_LAYERED 0x00080000 
//Only VC++ 6 need this
//#define LWA_COLORKEY 1 // Use color as the transparency color.
//#define LWA_ALPHA    2 // Use bAlpha to determine the opacity of the layer

//  ===========================================================================
//  Func    ExtWndProc
//  Desc    The windows procedure that is used to forward messages to the 
//          CSplash class. CSplash sends the "this" pointer through the
//          CreateWindowEx call and the pointer reaches here in the 
//          WM_CREATE message. We store it here and use it for message 
//          forwarding.
//  ===========================================================================
static LRESULT CALLBACK ExtWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static CSplash * spl = NULL;
	if (uMsg == WM_CREATE)
	{
		spl = (CSplash*)((LPCREATESTRUCT)lParam)->lpCreateParams;
	}
	if (spl)
		return spl->WindowProc(hwnd, uMsg, wParam, lParam);
	else
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK CSplash::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//  We need to handle on the WM_PAINT message
	switch (uMsg)
	{
		HANDLE_MSG(hwnd, WM_PAINT, OnPaint);
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void CSplash::OnPaint(HWND hwnd)
{
	if (!m_hBitmap)
		return;

	//  =======================================================================
	//  Paint the background by BitBlting the bitmap
	//  =======================================================================
	PAINTSTRUCT ps;
	HDC hDC = BeginPaint(hwnd, &ps);

	RECT   rect;
	::GetClientRect(m_hwnd, &rect);

	HDC hMemDC = ::CreateCompatibleDC(hDC);
	HBITMAP hOldBmp = (HBITMAP)::SelectObject(hMemDC, m_hBitmap);

	BitBlt(hDC, 0, 0, m_dwWidth, m_dwHeight, hMemDC, 0, 0, SRCCOPY);

	::SelectObject(hMemDC, hOldBmp);

	::DeleteDC(hMemDC);

	EndPaint(hwnd, &ps);
}

void CSplash::Init()
{
	//  =======================================================================
	//  Initialize the variables
	//  =======================================================================
	m_hwnd = NULL;
	m_lpszClassName = TEXT(enc("SPLASH"));
	m_colTrans = 0;

	//  =======================================================================
	//  Keep the function pointer for the SetLayeredWindowAttributes function
	//  in User32.dll ready
	//  =======================================================================
	HMODULE hUser32 = GetModuleHandle(TEXT(enc("USER32.DLL")));

	g_pSetLayeredWindowAttributes = (lpfnSetLayeredWindowAttributes)
		GetProcAddress(hUser32, enc("SetLayeredWindowAttributes"));
}

CSplash::CSplash()
{
	Init();
}

CSplash::CSplash(int resourceID, COLORREF colTrans) //addon alin1337
{
	Init();

#ifdef AR
	SetBitmap(LoadBitmap(GetModuleHandle(enc("cap80.dll")), MAKEINTRESOURCE(resourceID)));
#elif THA
	SetBitmap(LoadBitmap(GetModuleHandle(enc("EasyCore.dll")), MAKEINTRESOURCE(resourceID)));
#else
	SetBitmap(LoadBitmap(GetModuleHandle(enc("dnsky.dll")), MAKEINTRESOURCE(resourceID)));
#endif
	SetTransparentColor(colTrans);
}

CSplash::CSplash(HBITMAP INPUT, COLORREF colTrans = RGB(255, 255, 255)) //addon2// alin1337
{
	Init();

	SetBitmap(INPUT);
	//SetTransparentColor(colTrans);
}

CSplash::~CSplash()
{
	FreeResources();
}

HWND CSplash::RegAndCreateWindow()
{
	//  =======================================================================
	//  Register the window with ExtWndProc as the window procedure
	//  =======================================================================
	WNDCLASSEX wndclass;
	wndclass.cbSize = sizeof (wndclass);
	wndclass.style = CS_BYTEALIGNCLIENT | CS_BYTEALIGNWINDOW;
	wndclass.lpfnWndProc = ExtWndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = DLGWINDOWEXTRA;
	wndclass.hInstance = ::GetModuleHandle(NULL);
	wndclass.hIcon = NULL;
	wndclass.hCursor = ::LoadCursor(NULL, IDC_WAIT);
	wndclass.hbrBackground = (HBRUSH)::GetStockObject(LTGRAY_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = m_lpszClassName;
	wndclass.hIconSm = NULL;

	if (!RegisterClassEx(&wndclass))
		return NULL;

	//  =======================================================================
	//  Create the window of the application, passing the this pointer so that
	//  ExtWndProc can use that for message forwarding
	//  =======================================================================
	DWORD nScrWidth = ::GetSystemMetrics(SM_CXFULLSCREEN);
	DWORD nScrHeight = ::GetSystemMetrics(SM_CYFULLSCREEN);

	int x = (nScrWidth - m_dwWidth) / 2;
	int y = (nScrHeight - m_dwHeight) / 2;
	m_hwnd = ::CreateWindowEx(WS_EX_TOPMOST | WS_EX_TOOLWINDOW, m_lpszClassName,
		TEXT(enc("SplashScreen")), WS_POPUP, x, y,
		m_dwWidth, m_dwHeight, NULL, NULL, NULL, this);

	//  =======================================================================
	//  Display the window
	//  =======================================================================
	if (m_hwnd)
	{
		MakeTransparent();
		ShowWindow(m_hwnd, SW_SHOW);
		UpdateWindow(m_hwnd);
	}
	return m_hwnd;
}

int CSplash::DoLoop()
{
	//  =======================================================================
	//  Show the window
	//  =======================================================================
	if (!m_hwnd)
		ShowSplash();

	//  =======================================================================
	//  Get into the modal loop
	//  =======================================================================
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;

}

void CSplash::ShowSplash()
{
	CloseSplash();
	RegAndCreateWindow();
}


DWORD CSplash::SetBitmap(HBITMAP hBitmap)
{
	int nRetValue;
	BITMAP  csBitmapSize;

	//  =======================================================================
	//  Free loaded resource
	//  =======================================================================
	FreeResources();

	if (hBitmap)
	{
		m_hBitmap = hBitmap;

		//  ===================================================================
		//  Get bitmap size
		//  ===================================================================
		nRetValue = ::GetObject(hBitmap, sizeof(csBitmapSize), &csBitmapSize);
		if (nRetValue == 0)
		{
			FreeResources();
			return 0;
		}
		m_dwWidth = (DWORD)csBitmapSize.bmWidth;
		m_dwHeight = (DWORD)csBitmapSize.bmHeight;
	}

	return 1;
}

void CSplash::FreeResources()
{
	if (m_hBitmap)
		::DeleteObject(m_hBitmap);
	m_hBitmap = NULL;
}

int CSplash::CloseSplash()
{

	if (m_hwnd)
	{
		DestroyWindow(m_hwnd);
		m_hwnd = 0;
		UnregisterClass(m_lpszClassName, ::GetModuleHandle(NULL));
		return 1;
	}
	return 0;
}

bool CSplash::SetTransparentColor(COLORREF col)
{
	m_colTrans = col;

	return MakeTransparent();
}

bool CSplash::MakeTransparent()
{
	//  =======================================================================
	//  Set the layered window style and make the required color transparent
	//  =======================================================================
	if (m_hwnd && g_pSetLayeredWindowAttributes && m_colTrans)
	{
		//  set layered style for the window
		SetWindowLong(m_hwnd, GWL_EXSTYLE, GetWindowLong(m_hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
		//  call it with 0 alpha for the given color
		g_pSetLayeredWindowAttributes(m_hwnd, m_colTrans, 0, LWA_COLORKEY);
	}
	return TRUE;
}
/*
bool LoadBitmapFromPNG(UINT uResourceID,
	Gdiplus::Bitmap** ppBitmapOut, HINSTANCE hInstance )
{
	bool bRet = false;

	//if (!hInstance)
		//hInstance = AfxGetInstanceHandle();

	HRSRC hResourceHandle = ::FindResource(
		hInstance, MAKEINTRESOURCE(uResourceID), "PNG");
	if (0 == hResourceHandle)
	{
		return bRet;
	}

	DWORD nImageSize = ::SizeofResource(hInstance, hResourceHandle);
	if (0 == nImageSize)
	{
		return bRet;
	}

	HGLOBAL hResourceInstance = ::LoadResource(hInstance, hResourceHandle);
	if (0 == hResourceInstance)
	{
		return bRet;
	}

	const void* pResourceData = ::LockResource(hResourceInstance);
	if (0 == pResourceData)
	{
		FreeResource(hResourceInstance);
		return bRet;
	}

	HGLOBAL hBuffer = ::GlobalAlloc(GMEM_MOVEABLE, nImageSize);
	if (0 == hBuffer)
	{
		FreeResource(hResourceInstance);
		return bRet;
	}

	void* pBuffer = ::GlobalLock(hBuffer);
	if (0 != pBuffer)
	{
		CopyMemory(pBuffer, pResourceData, nImageSize);
		IStream* pStream = 0;
		if (S_OK == ::CreateStreamOnHGlobal(hBuffer, FALSE, &pStream))
		{
			*ppBitmapOut = new Gdiplus::Bitmap(pStream);
			pStream->Release();
			bRet = true;
		}
		::GlobalUnlock(hBuffer);
	}
	::GlobalFree(hBuffer);

	UnlockResource(hResourceInstance);
	FreeResource(hResourceInstance);

	return bRet;
}

HBITMAP LoadPNGasBMP(const HMODULE hModule, const LPCTSTR lpPNGName)
{
	
	HRSRC found = FindResource(hModule, lpPNGName, "PNG");
	unsigned int size = SizeofResource(hModule, found);
	HGLOBAL loaded = LoadResource(hModule, found);
	void* resource_data = LockResource(loaded);

	
	std::vector<unsigned char> raw;
	unsigned long width, height;
	int err = decodePNG(raw, width, height, (const unsigned char*)resource_data, size,false);
	if (err != 0)
	{
	//	log_debug("Error while decoding png splash: %d", err);
		return NULL;
	}

	BITMAPV5HEADER bmpheader = { 0 };
	bmpheader.bV5Size = sizeof(BITMAPV5HEADER);
	bmpheader.bV5Width = width;
	bmpheader.bV5Height = height;
	bmpheader.bV5Planes = 1;
	bmpheader.bV5BitCount = 32;
	bmpheader.bV5Compression = BI_BITFIELDS;
	bmpheader.bV5SizeImage = width*height * 4;
	bmpheader.bV5RedMask = 0x00FF0000;
	bmpheader.bV5GreenMask = 0x0000FF00;
	bmpheader.bV5BlueMask = 0x000000FF;
	bmpheader.bV5AlphaMask = 0xFF000000;
	bmpheader.bV5CSType = LCS_WINDOWS_COLOR_SPACE;
	bmpheader.bV5Intent = LCS_GM_BUSINESS;
	void* converted = NULL;
	HDC screen = GetDC(NULL);
	HBITMAP result = CreateDIBSection(screen, reinterpret_cast<BITMAPINFO*>(&bmpheader), DIB_RGB_COLORS, &converted, NULL, 0);
	ReleaseDC(NULL, screen);

	for (unsigned int y1 = height - 1, y2 = 0; y2 < height; y1--, y2++)
	for (unsigned int x = 0; x < width; x++)
	{
		*((char*)converted + 0 + 4 * x + 4 * width*y2) = raw[2 + 4 * x + 4 * width*y1]; // Blue
		*((char*)converted + 1 + 4 * x + 4 * width*y2) = raw[1 + 4 * x + 4 * width*y1]; // Green
		*((char*)converted + 2 + 4 * x + 4 * width*y2) = raw[0 + 4 * x + 4 * width*y1]; // Red
		*((char*)converted + 3 + 4 * x + 4 * width*y2) = raw[3 + 4 * x + 4 * width*y1]; // Alpha
	}

	return result;
}
*/
void SplashScreen()
{
	CSplash splash1(RLKT_BITMAP, RGB(255, 255, 255));
	splash1.ShowSplash();
	Sleep(800);
	splash1.CloseSplash();
}

#ifdef AR
void SplashScreenGuard()
{
	CSplash splash2(GUARD_BITMAP, RGB(255, 255, 255));
	splash2.ShowSplash();
	Sleep(800);
	splash2.CloseSplash();
}
#endif
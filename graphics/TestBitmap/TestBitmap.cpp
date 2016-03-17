// TestBitmap.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "TestBitmap.h"
#include <Strsafe.h>
#include <winuser.h>

#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING] = _T("Bitmap");					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING] = _T("BitmapMainWindow");			// 主窗口类名

// 此代码模块中包含的函数的前向声明: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void ShowError(DWORD dwErrNo);
HBITMAP LoadBitmapFromCreateBitmap();

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO:  在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TESTBITMAP, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TESTBITMAP));

	// 主消息循环: 
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  函数:  MyRegisterClass()
//
//  目的:  注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTBITMAP));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_TESTBITMAP);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数:  InitInstance(HINSTANCE, int)
//
//   目的:  保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	static HBITMAP hBitmap;
	static HDC memDC;

	switch (message)
	{
	case WM_CREATE:
		//The LoadBitmap function loads the specified bitmap resource from a module's executable file.
		//hBitmap = ::LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		//hBitmap = (HBITMAP)LoadImage(NULL, L"test1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		//hBitmap = (HBITMAP)LoadImage(NULL, L"test16.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_LOADMAP3DCOLORS  /*LR_LOADTRANSPARENT*/);
		hBitmap = LoadBitmapFromCreateBitmap();
		if (!hBitmap)
		{
			DWORD d = GetLastError(); 
			ShowError(d);
			PostQuitMessage(0);
		}
			
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择: 
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		// TODO:  在此添加任意绘图代码...
		int width = ps.rcPaint.right - ps.rcPaint.left;
		int height = ps.rcPaint.bottom - ps.rcPaint.top;
		memDC = CreateCompatibleDC(hdc);
		HBITMAP hTempBitmap = CreateCompatibleBitmap(hdc, width, height);
		HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hdc, hTempBitmap);
		::SelectObject(memDC, hBitmap);
		::BitBlt(hdc, 0, 0, width, height, memDC, 0, 0, SRCCOPY);
		//SetStretchBltMode(hdc, BLACKONWHITE);
		//StretchBlt(hdc, 0, 0, width, height, memDC, 0, 0, 1, 1, SRCCOPY);
		::SelectObject(hdc, hOldBitmap);
		::DeleteObject(hTempBitmap);
		EndPaint(hWnd, &ps);
	}

		break;
	case WM_CLOSE:
		::DeleteObject(hBitmap);
		::DeleteDC(memDC);
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void ShowError(DWORD dwErrNo)
{
	// Retrieve the system error message for the last-error code
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwErrNo,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message and exit the process

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + 40)*sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("lastError with error %d: %s"),
		dwErrNo, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	//    ExitProcess(dwErrNo);
}

HBITMAP LoadBitmapFromCreateBitmap()
{
	int width =  256;
	int height = 256;
	int bitPerPixel = 1;
	LPBYTE pData = new BYTE[width * height];
	for (int i = 0; i <  width * height / 8; i++)
	{
		pData[i] = 0xaa;
	}
// 	int bitPerPixel = 32;
// 	LPBYTE pData = new BYTE[width * height * 4];
// 	for (int i = 0; i < width * height; i++)
// 	{
// 		pData[i * 4] = i % 12;
// 		pData[i * 4 + 1] = (i + 34) % 256;
// 		pData[i * 4 + 2] = (i + 125) % 256;
// 	}
// 	int bitPerPixel = 24;
// 	LPBYTE pData = new BYTE[width * height * 3];
// 	for (int i = 0; i < width * height; i++)
// 	{
// 		pData[i * 3] = 0xaa;
// 		pData[i * 3 + 1] = 0xaa;
// 		pData[i * 3 + 2] = 0xaa;
// 	}

	return CreateBitmap(width, height, 1, bitPerPixel, pData);
}
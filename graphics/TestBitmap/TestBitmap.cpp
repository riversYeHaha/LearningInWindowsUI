// TestBitmap.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "TestBitmap.h"
#include <Strsafe.h>
#include <winuser.h>

#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING] = _T("Bitmap");					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING] = _T("BitmapMainWindow");			// ����������

// �˴���ģ���а����ĺ�����ǰ������: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void ShowError(DWORD dwErrNo);
HBITMAP LoadBitmapFromCreateBitmap();
HBITMAP LoadBitmapFromCreateDIBitmap();
HBITMAP LoadBitmapFromCreateCompatibleBitmap();
HBITMAP LoadBitmapFromCreateDIBSection();
HBITMAP LoadBitmapFromSetDIBits();

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO:  �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TESTBITMAP, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TESTBITMAP));

	// ����Ϣѭ��: 
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
//  ����:  MyRegisterClass()
//
//  Ŀ��:  ע�ᴰ���ࡣ
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
//   ����:  InitInstance(HINSTANCE, int)
//
//   Ŀ��:  ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

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
//  ����:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	static HBITMAP s_hBitmap;
	static BITMAP s_bm;
	static HDC memDC;
	static HDC hDeskDC;
	static bool copyDeskTop = false; 
	static bool isStrench = false;
	static enum E_BITMAP_FROM
	{
		BF_FROM_LOAD_BITMAP = 0,
		BF_FROM_LOAD_IMAGE,
		BF_FROM_CREATE_BITMAP,
		BF_FROM_CREATE_DIB_BITMAP,
		BF_FROM_CREATE_COMPATIBLE_BITMAP,
		BF_FROM_CREATE_DIB_SECTION,
		BF_FROM_SET_DIBITS,
		BF_FROM_MAX
	} eBitmapFrom{ BF_FROM_CREATE_BITMAP };


	switch (message)
	{
	case WM_CREATE:
		if (copyDeskTop)
		{
			int width = GetSystemMetrics(SM_CXSCREEN);
			int height = GetSystemMetrics(SM_CYSCREEN);
			hDeskDC = GetDC(::GetDesktopWindow());
			memDC = CreateCompatibleDC(hDeskDC);
			s_hBitmap = CreateCompatibleBitmap(hDeskDC, width, height);
			HBITMAP hOldBitmap = (HBITMAP)::SelectObject(memDC, s_hBitmap);
			::BitBlt(memDC, 0, 0, width, height, hDeskDC, 0, 0, SRCCOPY);
			SelectObject(memDC, hOldBitmap);
		}
		else
		{ 
			if (BF_FROM_LOAD_BITMAP == eBitmapFrom)
			{
				//The LoadBitmap function loads the specified bitmap resource from a module's executable file.
				s_hBitmap = ::LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
			}
			else if (BF_FROM_LOAD_IMAGE == eBitmapFrom)
			{
				s_hBitmap = (HBITMAP)LoadImage(NULL, L"test1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
				//s_hBitmap = (HBITMAP)LoadImage(NULL, L"test16.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_LOADMAP3DCOLORS  /*LR_LOADTRANSPARENT*/);
			}
			else if (BF_FROM_CREATE_BITMAP == eBitmapFrom)
			{
				s_hBitmap = LoadBitmapFromCreateBitmap();
			}
			else if (BF_FROM_CREATE_DIB_BITMAP == eBitmapFrom)
			{
				s_hBitmap = LoadBitmapFromCreateDIBitmap();
			}
			else if (BF_FROM_CREATE_COMPATIBLE_BITMAP == eBitmapFrom)
			{
				s_hBitmap = LoadBitmapFromCreateCompatibleBitmap();
			}
			else if (BF_FROM_CREATE_DIB_SECTION == eBitmapFrom)
			{
				s_hBitmap = LoadBitmapFromSetDIBits();
			}
			
		}
		if (!s_hBitmap)
		{
			DWORD d = GetLastError();
			ShowError(d);
			PostQuitMessage(0);
		}
		else
		{
			::GetObject(s_hBitmap, sizeof(s_bm), &s_bm);
		}
			
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �����˵�ѡ��: 
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
		// TODO:  �ڴ���������ͼ����...
		int width = ps.rcPaint.right - ps.rcPaint.left;
		int height = ps.rcPaint.bottom - ps.rcPaint.top;

		if (memDC == NULL)
			memDC = CreateCompatibleDC(hdc);

		HBITMAP hOldBitmap = (HBITMAP)::SelectObject(memDC, s_hBitmap);
		if (isStrench)
		{

			SetStretchBltMode(hdc, BLACKONWHITE);
			//��Ŀ���(x,y)��ʼ�Ŀ��Ϊ(width,height)�ľ��η�Χ�����ظ��Ƶ�Ŀ��������������Ŀ�����������С��С��Ŵ�
			StretchBlt(hdc, 0, 0, width, height, memDC, 0, 0, s_bm.bmWidth, s_bm.bmHeight, SRCCOPY);
		}
		else
		{
			::BitBlt(hdc, 0, 0, width, height, memDC, 0, 0, SRCCOPY);
		}

		::SelectObject(memDC, hOldBitmap);

		EndPaint(hWnd, &ps);
	}

		break;
	case WM_CLOSE:

		if (s_hBitmap)
		{
			::DeleteObject(s_hBitmap);
			s_hBitmap = NULL;
		}
		
		if (memDC)
		{
			::DeleteDC(memDC);
			memDC = NULL;
		}
		
		if (hDeskDC)
		{
			::ReleaseDC(hWnd, hDeskDC);
			hDeskDC = NULL;
		}		

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

// �����ڡ������Ϣ�������
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
	//�����ʾ����֧���Կ������õ����ģʽ�������ʾ���޷���ʾͼ��
	//bitPerPixel��planes��������������ָ����ֵλͼ�ᴴ��ʧ��
	//�������������뵱ǰ��ʾ�����ò�ƥ�䣬λͼ�ܴ����ɹ�������ʾ����ͼ��
	int width =  256;
	int height = 256;
	int bitPerPixel = GetDeviceCaps(GetDC(NULL), BITSPIXEL);
	int planes = GetDeviceCaps(GetDC(NULL), PLANES);
	LPBYTE pData = nullptr;

	//��ɫͼ�������Կ�ģʽ�¶�����ʾ,bitPerPixel��planes��������1
// 	bitPerPixel = planes = 1;
// 	LPBYTE pData = new BYTE[width * height / 8];
// 	for (int i = 0; i <  width * height / 8; i++)
// 	{
// 		pData[i] = 0xaa;
// 	}
	//����24λɫ
//	bitPerPixel = 24;
	//16ɫ��4����ɫ�棬�����windows������ơ��Ľ���
	if (bitPerPixel == 1 && planes == 4)
	{
		//û�в��Ի�������ʾ����16ɫ
		pData = new BYTE[width * height / 2];
		for (int i = 0; i < width * height / 2; i++)
		{
			pData[i] = 0x1 << 8 | 0x1 << 4 | 0x1 << 2;
		}
	}
	else if (bitPerPixel == 8 && planes == 1)
	{
		//���Կ�����Ϊ256ɫģʽ����Ч��
		pData = new BYTE[width * height];
		for (int i = 0; i < width * height; i++)
		{
			pData[i] = i % 256;
		}
	}
	else if (bitPerPixel == 16 && planes == 1)
	{
		//���Կ���Ϊ16λɫ�Ż���ʾ
		DWORD* pData = new DWORD[width * height / 2];
		for (int i = 0; i < width * height / 2; i++)
		{
			DWORD d = (i % 32 << 11) | ((i * i) % 32 << 6) | (i * i * i) % 32;
			pData[i] = d;
		}
		return CreateBitmap(width, height, planes, bitPerPixel, pData);
	}
	else if (bitPerPixel == 32 && planes == 1)
	{
		//���Կ�����Ϊ32λɫ�Ż���ʾ
		pData = new BYTE[width * height * 4];
		for (int i = 0; i < width * height; i++)
		{
			pData[i * 4] = i % 12;
			pData[i * 4 + 1] = (i + 34) % 256;
			pData[i * 4 + 2] = (i + 125) % 256;
		}
	}
	else if (bitPerPixel == 24 && planes == 1)
	{
		//û�в��Ի�����ʾ����24λɫ
		pData = new BYTE[width * height * 3];
		for (int i = 0; i < width * height; i++)
		{
			pData[i * 3] = i % 12;
			pData[i * 3 + 1] = (i + 34) % 256;
			pData[i * 3 + 2] = (i + 125) % 256;
		}
	}

	return CreateBitmap(width, height, planes, bitPerPixel, pData);
}

HBITMAP LoadBitmapFromCreateDIBitmap()
{
	int type = 1;//1.��ȡλͼ��2.�����أ�3.�õ�ɫ�崴��16λλͼ
	HBITMAP hBitmap = NULL;
	LPCTSTR filePath = _T("test1.bmp");
	if (1 == type)
	{
		HANDLE hf = NULL;
		BITMAPFILEHEADER* pbmfh;
		DWORD dwBytesRead;
		DWORD dwFileSize;
		DWORD dwFileSizeHigh;
		BOOL bSuccess;
		hf = CreateFile(filePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);

	}
	else if (2 == type)
	{
		BITMAPINFO info;
		memset(&info, 0, sizeof(BITMAPINFO));

		info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		info.bmiHeader.biWidth = 128;
	}
	else if (3 == type)
	{

	}


	return hBitmap;

}

HBITMAP LoadBitmapFromCreateCompatibleBitmap()
{
	HBITMAP hBitmap = NULL;
	return hBitmap;
}

HBITMAP LoadBitmapFromCreateDIBSection()
{
	HBITMAP hBitmap = NULL;
	return hBitmap;
}

HBITMAP LoadBitmapFromSetDIBits()
{
	HBITMAP hBitmap = NULL;
	return hBitmap;
}
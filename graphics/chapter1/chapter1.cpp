#include "chapter1.h"

#include <windows.h>
#include <CommCtrl.h>

#include "stb_image.h"

extern "C"
{
	extern unsigned char *stbi_load_from_memory(unsigned char const *buffer, int len, int *x, int *y, \
		int *comp, int req_comp);
	extern void     stbi_image_free(void *retval_from_stbi_load);

};

typedef BOOL(WINAPI *LPALPHABLEND)(HDC, int, int, int, int, HDC, int, int, int, int, BLENDFUNCTION);

const wchar_t* szDialogTitle = L"chapter1 λͼ��ˢ - yi";

BOOL CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
static BOOL WINAPI AlphaBitBlt(HDC hDC, int nDestX, int nDestY, int dwWidth, int dwHeight, HDC hSrcDC, \
	int nSrcX, int nSrcY, int wSrc, int hSrc, BLENDFUNCTION ftn);
HBITMAP LoadImageFromFile(const wchar_t* path, bool& alpha);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPreIntance, LPSTR lpCmdLine, int nCmdShow)
{
	HINSTANCE hDll = ::LoadLibrary(L"msimg32.dll");
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
}

BOOL CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	const int INIT_ALPHA = 200;//���ڳ�ʼ��alphaֵ
	RECT rcDialog;
	HBITMAP hBitmap;
	static BITMAP s_bm;
	static HDC s_hdcMem;
	static HBRUSH s_hBitmapBrush;//λͼ��ˢ
	static bool isReturnBrush = false;
	static bool needAlphaBlend = false;
	static bool isLayered = true;

	switch (message)
	{
	case WM_INITDIALOG:
		SetWindowText(hDlg, szDialogTitle);
		// ���öԻ����С�ɵ���,��Ҫ����MAXIZEBOX,MINSIZEBOX������
		SetWindowLong(hDlg, GWL_STYLE, GetWindowLong(hDlg, GWL_STYLE) | WS_SIZEBOX);
		//SetWindowLong(hDlg, GWL_STYLE, GetWindowLong(hDlg, GWL_STYLE) & ~WS_CAPTION);
		// ���ر���ͼƬ
		// LoadImage֧��bmp,ico,cursor,LR_MONOCHROME���Ϊ�ڰ�
		//hBitmap = (HBITMAP)LoadImage(NULL, L"test4.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		// ���·��������ʱ����ǰĿ¼�ǹ���Ŀ¼������debugĿ¼������ͼƬҪ���ڹ�������Ŀ¼��
		hBitmap = LoadImageFromFile(L"0.png", needAlphaBlend);
		if (isLayered)
		{
			isReturnBrush = !(needAlphaBlend = false);

			// ע��������GWL_EXSTYLE������GWL_STYLE
			SetWindowLong(hDlg, GWL_EXSTYLE, GetWindowLong(hDlg, GWL_STYLE) | WS_EX_LAYERED);
			// ����͸���� 0 - completely transparent 255 - opaque�������ð�͸��
			SetLayeredWindowAttributes(hDlg, 0, INIT_ALPHA, LWA_ALPHA);
			// ����͸����ɫ,��ɫΪָ��ɫ�����ر��ȫ͸����������ɫ��͸��
			//SetLayeredWindowAttributes(hDlg, RGB(0 , 0 , 0), 0, LWA_COLORKEY);

			// ���û������仯��Χ
			SendMessage(GetDlgItem(hDlg, IDC_SLIDER1), TBM_SETRANGE, (WPARAM)FALSE, MAKELONG(0, 255));
			// ���û�������ʼλ��
			SendMessage(GetDlgItem(hDlg, IDC_SLIDER1), TBM_SETPOS, (WPARAM)TRUE, INIT_ALPHA);
		}
			

		if (hBitmap == NULL)
		{
			MessageBox(hDlg, L"LoadImage failed", L"Error", MB_ICONERROR);
			exit(0);
		}
		else
		{			
			if (!isReturnBrush)
			{
				// ������ͼƬ����HDC
				HDC hdc;
				//��ȡ��ǰ�Ի���dc
				hdc = GetDC(hDlg);
				// ����dc��ص��ڴ�dc
				s_hdcMem = CreateCompatibleDC(hdc);

				//��λͼѡ�񵽼���DC,֮���������DC��ӵ�к�hBitmapͬ����С�Ļ�ͼ����,����λͼ���ص�GDI���������Ч��.
				//ѡ��λͼ���ڴ�dc,��һ���ٸ��Ƶ����ڿͻ���dc
				SelectObject(s_hdcMem, hBitmap);
				ReleaseDC(hDlg, hdc);

				//ͨ��λͼ�����ȡ��ȡλͼ�Ĵ�С����Ϣ,λͼ��С��ʵ��Ҳ�Ǽ���DC��ͼ����ķ�Χ
				GetObject(hBitmap, sizeof(s_bm), &s_bm);
			}
		}

		if (isReturnBrush)
		{
			// ����λͼ��ˢ
			s_hBitmapBrush = CreatePatternBrush(hBitmap);
		}	

		return 0;


	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			DeleteDC(s_hdcMem);
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;

	case WM_SIZE:
		InvalidateRect(hDlg, NULL, TRUE);
		return TRUE;

	case WM_HSCROLL:
	{
		int nTransparent = SendMessage(GetDlgItem(hDlg, IDC_SLIDER1), TBM_GETPOS, 0, 0);
		SetLayeredWindowAttributes(hDlg, 0, nTransparent, LWA_ALPHA);//Ҳ��ʹ��ɫ�䰵
	}
	break;

	case WM_QUERYNEWPALETTE:
		return FALSE;
	break;

	case WM_CTLCOLORDLG:
		//needAlphaBlend = false;
		if (needAlphaBlend)//��λͼ����alpha����Ȼ����ʾ,������bitBlt������alphaͨ����λͼ��˵ֻ��͸����Ϊ0�Ĳ���ֻ����һƬ��ɫ
		{		
			static LPALPHABLEND lpAlphaBlend = (LPALPHABLEND) ::GetProcAddress(::GetModuleHandle(L"msimg32.dll"), "AlphaBlend");

			if (lpAlphaBlend == NULL) 
				lpAlphaBlend = AlphaBitBlt;

			static BLENDFUNCTION ftn = { 0 };
			ftn.BlendOp = AC_SRC_OVER;
			ftn.AlphaFormat = AC_SRC_ALPHA;
			ftn.BlendFlags = 0;
			ftn.SourceConstantAlpha = 255;//����͸���ȣ���ԭλͼ����alpha�ϳɲ�����ʹԭ����͸���ı�Ϊ͸��

			GetClientRect(hDlg, &rcDialog);
			lpAlphaBlend((HDC)wParam, 0, 0, rcDialog.right, rcDialog.bottom, s_hdcMem,
				0, 0, s_bm.bmWidth, s_bm.bmHeight, ftn);

			return (BOOL)((HBRUSH)GetStockObject(NULL_BRUSH));

		}
		else
		{
			if (!isReturnBrush)
			{
				GetClientRect(hDlg, &rcDialog);
				//ͨ��SetStretchBltMode��������ʹStrechBlt������ͼ���������
				SetStretchBltMode((HDC)wParam, HALFTONE);
				//SetStretchBltMode((HDC)wParam, COLORONCOLOR);
				//�����ڴ�dc��dc,StretchBlt���Զ���������
				StretchBlt((HDC)wParam, 0, 0, rcDialog.right, rcDialog.bottom, s_hdcMem, 0, 0, s_bm.bmWidth, s_bm.bmHeight, SRCCOPY);
				return (BOOL)((HBRUSH)GetStockObject(NULL_BRUSH));
			}

			if (isReturnBrush) return (BOOL)s_hBitmapBrush;
		}

	}
	return FALSE;
}

HBITMAP LoadImageFromFile(const wchar_t* path, bool& alpha)
{
	if (!path)
		return NULL;

	HANDLE hFile = ::CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) 
		return NULL;

	DWORD dwSize = ::GetFileSize(hFile, NULL);
	if (dwSize == 0) 
		return NULL;

	DWORD dwRead = 0;
	LPBYTE pData = new BYTE[dwSize];
	::ReadFile(hFile, pData, dwSize, &dwRead, NULL);
	::CloseHandle(hFile);

	if (dwRead != dwSize)
	{
		delete[] pData;
		return NULL;
	}

	LPBYTE pImage = NULL;
	int x, y, n;
	pImage = stbi_load_from_memory(pData, dwSize, &x, &y, &n, 4);
	delete[] pData;
	if (!pImage)
		return NULL;
	
	BITMAPINFO bmi;
	::ZeroMemory(&bmi, sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = x;
	bmi.bmiHeader.biHeight = -y;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = x * y * 4;

	bool bAlphaChannel = false;
	LPBYTE pDest = NULL;
	HBITMAP hBitmap = ::CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void**)&pDest, NULL, 0);
	if (!hBitmap)
		return NULL;

	//����alphaͨ����ͼ��ĳ������ɫ����������alphaֵ�ٷֱȣ���Ȼ������ʾ��͸��Ч������ΪSourceConstantAlpha������alphaֵȷ����͸���ȣ�������ɫ�޹أ���
	//���ǳ���alphaֵ�ٷֱȣ���ʹ͸����ɫ�ԵñȽϺ����𽥱䰵��Ч������ʹ��һ����ɫ�𽥱䰵��Ч������ϰ�͸���̶ȡ�������������ش���alphaֵ����Ҫ����alphaֵ����������ɫ
	//SetLayeredWindowAttributes(hDlg, 0, nTransparent, LWA_ALPHA)��AlphaBlend����ͬ���Ὣ��ɫ����alphaֵ,
	//�������ʵ�ֵ�AlphaBitBltȥ���ƣ��Ͳ���Ҫ�ȳ���alphaֵ�ˣ���Ϊ�ڲ����Լ���
	for (int i = 0; i < x * y; i++)
	{
		pDest[i * 4 + 3] = pImage[i * 4 + 3];
		if (pDest[i * 4 + 3] < 255)//RGBA,��alphaֵ,תBGR(RGBQUAD����˳��),��͸��
		{
			pDest[i * 4] = (BYTE)(DWORD(pImage[i * 4 + 2]) * pImage[i * 4 + 3] / 255);//Bֵ����alpha����
			pDest[i * 4 + 1] = (BYTE)(DWORD(pImage[i * 4 + 1]) * pImage[i * 4 + 3] / 255);
			pDest[i * 4 + 2] = (BYTE)(DWORD(pImage[i * 4]) * pImage[i * 4 + 3] / 255);
			bAlphaChannel = true;
		}
		else//����Ҫ����alpha��������͸��
		{
			/*BYTE d = pDest[i * 4 + 3];
			float alphaPercent = 0.3;
			pDest[i * 4] = pImage[i * 4 + 2] * alphaPercent;
			pDest[i * 4 + 1] = pImage[i * 4 + 1] * alphaPercent;
			pDest[i * 4 + 2] = pImage[i * 4]  * alphaPercent;
			pDest[i * 4 + 3] = 255 * alphaPercent;*/

			pDest[i * 4] = pImage[i * 4 + 2];
			pDest[i * 4 + 1] = pImage[i * 4 + 1];
			pDest[i * 4 + 2] = pImage[i * 4];
			//pDest[i * 4 + 3] = 255 * 0.3;
		}

		if (*(DWORD*)(&pDest[i * 4]) == 0/*> 0xffa00000*/)//ָ��͸��ɫ�������ɫ��͸��ɫƥ�䣬��������ɫֵΪ0x00000000����ʾȫ͸��,��Ϊ��ַ�����ڵ�λֵ��little-Endian
		{
			pDest[i * 4] = (BYTE)0;
			pDest[i * 4 + 1] = (BYTE)0;
			pDest[i * 4 + 2] = (BYTE)0;
			pDest[i * 4 + 3] = (BYTE)0;
			bAlphaChannel = true;
		}

	}
	stbi_image_free(pImage);
	alpha = bAlphaChannel;
	return hBitmap;
}

static COLORREF PixelAlpha(COLORREF clrSrc, double src_darken, COLORREF clrDest, double dest_darken)
{
	return RGB(GetRValue(clrSrc) * src_darken + GetRValue(clrDest) * dest_darken,
		GetGValue(clrSrc) * src_darken + GetGValue(clrDest) * dest_darken,
		GetBValue(clrSrc) * src_darken + GetBValue(clrDest) * dest_darken);

}

BOOL WINAPI AlphaBitBlt(HDC hDC, int nDestX, int nDestY, int dwWidth, int dwHeight, HDC hSrcDC, 
	int nSrcX, int nSrcY, int wSrc, int hSrc, BLENDFUNCTION ftn)
{
	HDC hTempDC = ::CreateCompatibleDC(hDC);
	if (NULL == hTempDC)
		return FALSE;

	//Creates Source DIB
	LPBITMAPINFO lpbiSrc = NULL;
	// Fill in the BITMAPINFOHEADER
	lpbiSrc = (LPBITMAPINFO) new BYTE[sizeof(BITMAPINFOHEADER)];
	if (lpbiSrc == NULL)
	{
		::DeleteDC(hTempDC);
		return FALSE;
	}
	lpbiSrc->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpbiSrc->bmiHeader.biWidth = dwWidth;
	lpbiSrc->bmiHeader.biHeight = dwHeight;
	lpbiSrc->bmiHeader.biPlanes = 1;
	lpbiSrc->bmiHeader.biBitCount = 32;
	lpbiSrc->bmiHeader.biCompression = BI_RGB;
	lpbiSrc->bmiHeader.biSizeImage = dwWidth * dwHeight;
	lpbiSrc->bmiHeader.biXPelsPerMeter = 0;
	lpbiSrc->bmiHeader.biYPelsPerMeter = 0;
	lpbiSrc->bmiHeader.biClrUsed = 0;
	lpbiSrc->bmiHeader.biClrImportant = 0;

	COLORREF* pSrcBits = NULL;
	HBITMAP hSrcDib = CreateDIBSection(
		hSrcDC, lpbiSrc, DIB_RGB_COLORS, (void **)&pSrcBits,
		NULL, NULL);

	if ((NULL == hSrcDib) || (NULL == pSrcBits))
	{
		delete[] lpbiSrc;
		::DeleteDC(hTempDC);
		return FALSE;
	}

	HBITMAP hOldTempBmp = (HBITMAP)::SelectObject(hTempDC, hSrcDib);
	::StretchBlt(hTempDC, 0, 0, dwWidth, dwHeight, hSrcDC, nSrcX, nSrcY, wSrc, hSrc, SRCCOPY);
	::SelectObject(hTempDC, hOldTempBmp);

	//Creates Destination DIB
	LPBITMAPINFO lpbiDest = NULL;
	// Fill in the BITMAPINFOHEADER
	lpbiDest = (LPBITMAPINFO) new BYTE[sizeof(BITMAPINFOHEADER)];
	if (lpbiDest == NULL)
	{
		delete[] lpbiSrc;
		::DeleteObject(hSrcDib);
		::DeleteDC(hTempDC);
		return FALSE;
	}

	lpbiDest->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpbiDest->bmiHeader.biWidth = dwWidth;
	lpbiDest->bmiHeader.biHeight = dwHeight;
	lpbiDest->bmiHeader.biPlanes = 1;
	lpbiDest->bmiHeader.biBitCount = 32;
	lpbiDest->bmiHeader.biCompression = BI_RGB;
	lpbiDest->bmiHeader.biSizeImage = dwWidth * dwHeight;
	lpbiDest->bmiHeader.biXPelsPerMeter = 0;
	lpbiDest->bmiHeader.biYPelsPerMeter = 0;
	lpbiDest->bmiHeader.biClrUsed = 0;
	lpbiDest->bmiHeader.biClrImportant = 0;

	COLORREF* pDestBits = NULL;
	HBITMAP hDestDib = CreateDIBSection(
		hDC, lpbiDest, DIB_RGB_COLORS, (void **)&pDestBits,
		NULL, NULL);

	if ((NULL == hDestDib) || (NULL == pDestBits))
	{
		delete[] lpbiSrc;
		::DeleteObject(hSrcDib);
		::DeleteDC(hTempDC);
		return FALSE;
	}

	//�����������͸��������
	::SelectObject(hTempDC, hDestDib);
	::BitBlt(hTempDC, 0, 0, dwWidth, dwHeight, hDC, nDestX, nDestY, SRCCOPY);
	::SelectObject(hTempDC, hOldTempBmp);

	double src_darken;
	BYTE nAlpha;

	for (int pixel = 0; pixel < dwWidth * dwHeight; pixel++, pSrcBits++, pDestBits++)
	{
		nAlpha = LOBYTE(*pSrcBits >> 24);
		if (nAlpha > 0)
			nAlpha++;
		src_darken = (double)(nAlpha * ftn.SourceConstantAlpha) / 255.0 / 255.0;
		if (src_darken < 0.0) src_darken = 0.0;
		*pDestBits = PixelAlpha(*pSrcBits, src_darken, *pDestBits, 1.0 - src_darken);
	} //for

	::SelectObject(hTempDC, hDestDib);
	::BitBlt(hDC, nDestX, nDestY, dwWidth, dwHeight, hTempDC, 0, 0, SRCCOPY);
	::SelectObject(hTempDC, hOldTempBmp);

	delete[] lpbiDest;
	::DeleteObject(hDestDib);

	delete[] lpbiSrc;
	::DeleteObject(hSrcDib);

	::DeleteDC(hTempDC);
	return TRUE;
}
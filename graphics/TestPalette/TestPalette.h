
// TestPalette.h : TestPalette Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������

#define WM_DOREALIZE WM_USER+200

// CTestPaletteApp:
// �йش����ʵ�֣������ TestPalette.cpp
//

class CTestPaletteApp : public CWinApp
{
public:
	CTestPaletteApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CTestPaletteApp theApp;

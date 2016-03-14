
// TestPaletteView.cpp : CTestPaletteView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "TestPalette.h"
#endif

#include "TestPaletteDoc.h"
#include "TestPaletteView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestPaletteView

IMPLEMENT_DYNCREATE(CTestPaletteView, CView)

BEGIN_MESSAGE_MAP(CTestPaletteView, CView)
	ON_MESSAGE(WM_DOREALIZE, OnDoRealize)
END_MESSAGE_MAP()

// CTestPaletteView ����/����

CTestPaletteView::CTestPaletteView()
{
	// TODO:  �ڴ˴���ӹ������

	LPLOGPALETTE pLogPal;
	pLogPal = (LPLOGPALETTE)malloc(sizeof(LPLOGPALETTE) + sizeof(PALETTEENTRY) * 256);
	pLogPal->palVersion = 0x300;
	pLogPal->palNumEntries = 256;
	for (int i = 0; i < 256; i++)
	{
		pLogPal->palPalEntry[i].peRed = 0;
		pLogPal->palPalEntry[i].peGreen = i;
		pLogPal->palPalEntry[i].peBlue = 0;
		pLogPal->palPalEntry[i].peFlags = 0;
	}
	if (!m_Palette.CreatePalette(pLogPal))
		AfxMessageBox(_T("Can't create palette!"));
}

CTestPaletteView::~CTestPaletteView()
{
}

BOOL CTestPaletteView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CTestPaletteView ����

void CTestPaletteView::OnDraw(CDC* pDC)
{
	CTestPaletteDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO:  �ڴ˴�Ϊ����������ӻ��ƴ���

	CBrush brush, *pOldBursh;
	int x, y, i;

	pDC->SelectPalette(&m_Palette, FALSE);//��ǰdcѡ��ǰ����ɫ��
	pDC->RealizePalette();//ʵ�ֵ�ϵͳ��ɫ��
	pDC->SelectStockObject(BLACK_PEN);//ѡ�����ж���
	// ��ɫ�����û�ͼ
	for (i = 0; i < 256; i++)
	{
		x = (i % 16) * 16;
		y = (i / 16) * 16;
		brush.CreateSolidBrush(PALETTEINDEX(i));//��ɫ����ɫ����
		pOldBursh = pDC->SelectObject(&brush);//ѡ���������
		pDC->Rectangle(x, y, x + 16, y + 16);
		pDC->SelectObject(pOldBursh);
		brush.DeleteObject();
	}
	// RGB��ͼ
	for (i = 0; i < 256; i++)
	{
		x = (i % 16) * 16 + 300;
		y = (i / 16) * 16;
		brush.CreateSolidBrush(RGB(0, i, 0));
		pOldBursh = pDC->SelectObject(&brush);
		pDC->Rectangle(x, y, x + 16, y + 16);
		pDC->SelectObject(pOldBursh);
		brush.DeleteObject();
	}
	// ��ɫ��RGB���û�ͼ
	for (i = 0; i < 256; i++)
	{
		x = (i % 16) * 16 + 600;
		y = (i / 16) * 16;
		brush.CreateSolidBrush(PALETTERGB(0, i, 0));//��ɫ����ɫ����
		pOldBursh = pDC->SelectObject(&brush);//ѡ���������
		pDC->Rectangle(x, y, x + 16, y + 16);
		pDC->SelectObject(pOldBursh);
		brush.DeleteObject();
	}

}

LRESULT CTestPaletteView::OnDoRealize(WPARAM wParam, LPARAM lParam)
{
	CClientDC dc(this);
	dc.SelectPalette(&m_Palette, FALSE);
	if (dc.RealizePalette()) // ����ɫ��ӳ��ı���ˢ����ͼ
		GetDocument()->UpdateAllViews(NULL);

	return 0L;
}


// CTestPaletteView ���

#ifdef _DEBUG
void CTestPaletteView::AssertValid() const
{
	CView::AssertValid();
}

void CTestPaletteView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTestPaletteDoc* CTestPaletteView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestPaletteDoc)));
	return (CTestPaletteDoc*)m_pDocument;
}
#endif //_DEBUG


// CTestPaletteView ��Ϣ�������

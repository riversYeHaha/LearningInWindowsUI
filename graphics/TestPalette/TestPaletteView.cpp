
// TestPaletteView.cpp : CTestPaletteView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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

// CTestPaletteView 构造/析构

CTestPaletteView::CTestPaletteView()
{
	// TODO:  在此处添加构造代码

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
	// TODO:  在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CTestPaletteView 绘制

void CTestPaletteView::OnDraw(CDC* pDC)
{
	CTestPaletteDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO:  在此处为本机数据添加绘制代码

	CBrush brush, *pOldBursh;
	int x, y, i;

	pDC->SelectPalette(&m_Palette, FALSE);//当前dc选中前景调色板
	pDC->RealizePalette();//实现到系统调色板
	pDC->SelectStockObject(BLACK_PEN);//选择已有对象
	// 调色板引用绘图
	for (i = 0; i < 256; i++)
	{
		x = (i % 16) * 16;
		y = (i / 16) * 16;
		brush.CreateSolidBrush(PALETTEINDEX(i));//调色板颜色索引
		pOldBursh = pDC->SelectObject(&brush);//选择任意对象
		pDC->Rectangle(x, y, x + 16, y + 16);
		pDC->SelectObject(pOldBursh);
		brush.DeleteObject();
	}
	// RGB绘图
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
	// 调色板RGB引用绘图
	for (i = 0; i < 256; i++)
	{
		x = (i % 16) * 16 + 600;
		y = (i / 16) * 16;
		brush.CreateSolidBrush(PALETTERGB(0, i, 0));//调色板颜色索引
		pOldBursh = pDC->SelectObject(&brush);//选择任意对象
		pDC->Rectangle(x, y, x + 16, y + 16);
		pDC->SelectObject(pOldBursh);
		brush.DeleteObject();
	}

}

LRESULT CTestPaletteView::OnDoRealize(WPARAM wParam, LPARAM lParam)
{
	CClientDC dc(this);
	dc.SelectPalette(&m_Palette, FALSE);
	if (dc.RealizePalette()) // 若调色板映射改变则刷新试图
		GetDocument()->UpdateAllViews(NULL);

	return 0L;
}


// CTestPaletteView 诊断

#ifdef _DEBUG
void CTestPaletteView::AssertValid() const
{
	CView::AssertValid();
}

void CTestPaletteView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTestPaletteDoc* CTestPaletteView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestPaletteDoc)));
	return (CTestPaletteDoc*)m_pDocument;
}
#endif //_DEBUG


// CTestPaletteView 消息处理程序

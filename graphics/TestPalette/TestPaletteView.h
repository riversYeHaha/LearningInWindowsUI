
// TestPaletteView.h : CTestPaletteView 类的接口
//

#pragma once


class CTestPaletteView : public CView
{
protected: // 仅从序列化创建
	CTestPaletteView();
	DECLARE_DYNCREATE(CTestPaletteView)

// 特性
public:
	CTestPaletteDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CTestPaletteView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CPalette m_Palette;

// 生成的消息映射函数
protected:
	afx_msg LRESULT OnDoRealize(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // TestPaletteView.cpp 中的调试版本
inline CTestPaletteDoc* CTestPaletteView::GetDocument() const
   { return reinterpret_cast<CTestPaletteDoc*>(m_pDocument); }
#endif


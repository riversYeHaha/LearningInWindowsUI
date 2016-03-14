
// TestPaletteView.h : CTestPaletteView ��Ľӿ�
//

#pragma once


class CTestPaletteView : public CView
{
protected: // �������л�����
	CTestPaletteView();
	DECLARE_DYNCREATE(CTestPaletteView)

// ����
public:
	CTestPaletteDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~CTestPaletteView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CPalette m_Palette;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg LRESULT OnDoRealize(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // TestPaletteView.cpp �еĵ��԰汾
inline CTestPaletteDoc* CTestPaletteView::GetDocument() const
   { return reinterpret_cast<CTestPaletteDoc*>(m_pDocument); }
#endif


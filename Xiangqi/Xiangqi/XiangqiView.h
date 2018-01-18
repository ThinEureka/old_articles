// XiangqiView.h : CXiangqiView ��Ľӿ�
//


#pragma once
#include "DIB.h"
#include "Qizi.h"
#include "StdQiju.h"
#include "UpturnQiju.h"



class CXiangqiView : public CScrollView
{
protected: // �������л�����
	CXiangqiView();
	DECLARE_DYNCREATE(CXiangqiView)

// ����
public:
	CXiangqiDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ������һ�ε���
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CXiangqiView();


#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	int m_pnYeInsight[90];
public:




public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	// ���
	CStdQiju* m_pStdQiju;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// ����ʶ��
	void DrawFlag1(CDC* pDC,CPoint VXY,int nStep, COLORREF color);
	// ���ģʽ��ʶ
	int m_nQijuStatus;
public:
	afx_msg void OnQijuStd();
public:
	afx_msg void OnQijuUpturn();
public:
	afx_msg void OnUpdateQijuStd(CCmdUI *pCmdUI);
public:
	afx_msg void OnUpdateQijuUpturn(CCmdUI *pCmdUI);
	// �������
	CUpturnQiju * m_pUpturnQiju;
};

#ifndef _DEBUG  // XiangqiView.cpp �еĵ��԰汾
inline CXiangqiDoc* CXiangqiView::GetDocument() const
   { return reinterpret_cast<CXiangqiDoc*>(m_pDocument); }
#endif


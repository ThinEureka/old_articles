// XiangqiView.h : CXiangqiView 类的接口
//


#pragma once
#include "DIB.h"
#include "Qizi.h"
#include "StdQiju.h"
#include "UpturnQiju.h"



class CXiangqiView : public CScrollView
{
protected: // 仅从序列化创建
	CXiangqiView();
	DECLARE_DYNCREATE(CXiangqiView)

// 属性
public:
	CXiangqiDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 构造后第一次调用
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CXiangqiView();


#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	int m_pnYeInsight[90];
public:




public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	// 棋局
	CStdQiju* m_pStdQiju;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// 画标识１
	void DrawFlag1(CDC* pDC,CPoint VXY,int nStep, COLORREF color);
	// 棋局模式标识
	int m_nQijuStatus;
public:
	afx_msg void OnQijuStd();
public:
	afx_msg void OnQijuUpturn();
public:
	afx_msg void OnUpdateQijuStd(CCmdUI *pCmdUI);
public:
	afx_msg void OnUpdateQijuUpturn(CCmdUI *pCmdUI);
	// 翻面棋局
	CUpturnQiju * m_pUpturnQiju;
};

#ifndef _DEBUG  // XiangqiView.cpp 中的调试版本
inline CXiangqiDoc* CXiangqiView::GetDocument() const
   { return reinterpret_cast<CXiangqiDoc*>(m_pDocument); }
#endif


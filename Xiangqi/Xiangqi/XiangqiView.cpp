// XiangqiView.cpp : CXiangqiView 类的实现
//

#include "stdafx.h"
#include "Xiangqi.h"

#include "XiangqiDoc.h"
#include "XiangqiView.h"
#include "Qizi.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CXiangqiView

IMPLEMENT_DYNCREATE(CXiangqiView, CScrollView)

BEGIN_MESSAGE_MAP(CXiangqiView, CScrollView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_COMMAND(ID_QIJU_STD, &CXiangqiView::OnQijuStd)
	ON_COMMAND(ID_QIJU_UPTURN, &CXiangqiView::OnQijuUpturn)
	ON_UPDATE_COMMAND_UI(ID_QIJU_STD, &CXiangqiView::OnUpdateQijuStd)
	ON_UPDATE_COMMAND_UI(ID_QIJU_UPTURN, &CXiangqiView::OnUpdateQijuUpturn)
END_MESSAGE_MAP()

CXiangqiView::CXiangqiView()
: m_pStdQiju(NULL)
, m_nQijuStatus(0)
, m_pUpturnQiju(NULL)
{
	// TODO: 在此处添加构造代码
//设置棋局大小属性的代码
	m_pStdQiju = new CStdQiju;
	m_pStdQiju->m_ptImage = CPoint(50,50);
	m_pStdQiju->m_ptOrigin = CPoint(85,85);
	m_pStdQiju->m_nStep = 51;
	m_pStdQiju->m_Qipan.Load(IDR_DIB_QIPAN,_T("DIB"));
	
//初始化棋局
//带有初始棋局结构的数组
	int Insight[90]=
	{
		-4,-3,-5,-6,-7,-6,-5,-3,-4,
		0,  0, 0, 0, 0, 0, 0, 0, 0,
		0, -2, 0, 0, 0, 0, 0, -2,0,
	   -1, 0, -1, 0,-1, 0, -1, 0,-1,
	    0, 0,  0, 0, 0, 0, 0,  0, 0,
		0, 0,  0, 0, 0, 0, 0,  0, 0,
        1, 0,  1, 0, 1, 0, 1,  0, 1,
        0, 2,  0, 0, 0, 0, 0,  2, 0,
		0, 0,  0, 0, 0, 0, 0,  0, 0,
		4, 3,  5, 6, 7, 6, 5,  3, 4
	};
	m_pStdQiju->Initial( Insight,1);
	


}

CXiangqiView::~CXiangqiView()
{
}

BOOL CXiangqiView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CScrollView::PreCreateWindow(cs);
}

// CXiangqiView 绘制

void CXiangqiView::OnDraw(CDC* pDC)
{
	CXiangqiDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	TRACE(_T("Hello"));
	switch( m_nQijuStatus)
	{
	case 0:
		{
			m_pStdQiju->Display(pDC);
			CPoint point1 = m_pStdQiju->GetPreMove1();
			CPoint point2 = m_pStdQiju->GetPreMove2();
			if(point1 != CPoint(STDQIJU_NONE,STDQIJU_NONE))
				this->DrawFlag1(pDC,point1,m_pStdQiju->GetStep(),RGB(50,50,200));
			if(point2 != CPoint(STDQIJU_NONE,STDQIJU_NONE))
				this->DrawFlag1(pDC,point2,m_pStdQiju->GetStep(),RGB(50,50,200));
		}
		break;
	case 1:
		{
			m_pUpturnQiju->Display(pDC);
		}
		break;
		
	}


	// TODO: 在此处为本机数据添加绘制代码
}

void CXiangqiView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 计算此视图的合计大小
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CXiangqiView 打印

BOOL CXiangqiView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CXiangqiView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CXiangqiView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清除过程
}


// CXiangqiView 诊断

#ifdef _DEBUG
void CXiangqiView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CXiangqiView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CXiangqiDoc* CXiangqiView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CXiangqiDoc)));
	return (CXiangqiDoc*)m_pDocument;
}
#endif //_DEBUG


// CXiangqiView 消息处理程序

void CXiangqiView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch( m_nQijuStatus)
	{
	case 0:
		m_pStdQiju->Action(this,point);
		break;
	case 1:
		this->m_pUpturnQiju->Action(this,point);
		break;
	}
    
    //判断状态
   CScrollView::OnLButtonDown(nFlags, point);
}

void CXiangqiView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(this->m_nQijuStatus)
	{
	case 0:
		m_pStdQiju->Wink(this);
		break;
	case 1:
		m_pUpturnQiju->Wink(this);
		break;
	}

	CScrollView::OnTimer(nIDEvent);
}

// 画标识１
void CXiangqiView::DrawFlag1(CDC* pDC,CPoint VXY,int nStep, COLORREF color)
{
	//::AfxMessageBox(_T("确在调用画标识函数"));

	CPen pen(PS_NULL,0,color);
	CPen* OldPen=pDC->SelectObject(&pen);
	CBrush brush(color);
	CBrush* OldBrush=pDC->SelectObject(&brush);
	CPoint point[8];
	int width=nStep/6;
	width++;
	int height= nStep/17;
	if(height==1)
		height++;
	point[0]=CPoint(-nStep/2,-nStep/2);
	point[1]=CPoint(nStep/2 - width, -nStep/2);
	point[2]=CPoint(-nStep/2,nStep/2-height);
	point[3]=CPoint(nStep/2-width,nStep/2-height);

	point[4]=CPoint(-nStep/2,-nStep/2);
	point[5]=CPoint(nStep/2 - height, -nStep/2);
	point[6]=CPoint(-nStep/2,nStep/2-width);
	point[7]=CPoint(nStep/2- height, nStep/2-width);

	CPoint pt;
	SIZE size;
	CRect r;
	for(int i=0;i<4;i++)
	{
		size.cx = width;
		size.cy = height;
		pt=VXY;
		pt.Offset(point[i]);
		r=CRect(pt,size);
		pDC->Rectangle(&r);
	}
	for(int i=4;i<8;i++)
	{
		size.cx = height;
		size.cy = width;
		pt=VXY;
		pt.Offset(point[i]);
		r=CRect(pt,size);
		pDC->Rectangle(&r);
	}
	pDC->SelectObject(OldPen);
	pDC->SelectObject(OldBrush);
}

		

void CXiangqiView::OnQijuStd()
{
	this->m_nQijuStatus = 0;

	this->Invalidate();
	this->UpdateWindow();
	// TODO: 在此添加命令处理程序代码
}

void CXiangqiView::OnQijuUpturn()
{
	this->m_nQijuStatus  = 1;
	if( m_pUpturnQiju != NULL)
		delete m_pUpturnQiju;
	this->m_pUpturnQiju = new CUpturnQiju;
	m_pUpturnQiju->m_Qipan.Load(IDR_DIB_QIPAN,_T("DIB"));
	m_pUpturnQiju->m_ptImage = CPoint(50,50);
	m_pUpturnQiju->m_ptOrigin = CPoint(110,110);
	m_pUpturnQiju->m_nStep = 51;
	m_pUpturnQiju->Initial();
	
	
	this->Invalidate();
	this->UpdateWindow();
	// TODO: 在此添加命令处理程序代码
}

void CXiangqiView::OnUpdateQijuStd(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	bool flag = m_nQijuStatus == 0;
	pCmdUI->SetCheck( flag );
	
	
}

void CXiangqiView::OnUpdateQijuUpturn(CCmdUI *pCmdUI)
{
	bool flag = m_nQijuStatus == 1;
	pCmdUI->SetCheck( flag );

	// TODO: 在此添加命令更新用户界面处理程序代码
}

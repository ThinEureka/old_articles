// XiangqiView.cpp : CXiangqiView ���ʵ��
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
	// ��׼��ӡ����
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
	// TODO: �ڴ˴���ӹ������
//������ִ�С���ԵĴ���
	m_pStdQiju = new CStdQiju;
	m_pStdQiju->m_ptImage = CPoint(50,50);
	m_pStdQiju->m_ptOrigin = CPoint(85,85);
	m_pStdQiju->m_nStep = 51;
	m_pStdQiju->m_Qipan.Load(IDR_DIB_QIPAN,_T("DIB"));
	
//��ʼ�����
//���г�ʼ��ֽṹ������
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
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CScrollView::PreCreateWindow(cs);
}

// CXiangqiView ����

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


	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}

void CXiangqiView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: �������ͼ�ĺϼƴ�С
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CXiangqiView ��ӡ

BOOL CXiangqiView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CXiangqiView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CXiangqiView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CXiangqiView ���

#ifdef _DEBUG
void CXiangqiView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CXiangqiView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CXiangqiDoc* CXiangqiView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CXiangqiDoc)));
	return (CXiangqiDoc*)m_pDocument;
}
#endif //_DEBUG


// CXiangqiView ��Ϣ�������

void CXiangqiView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch( m_nQijuStatus)
	{
	case 0:
		m_pStdQiju->Action(this,point);
		break;
	case 1:
		this->m_pUpturnQiju->Action(this,point);
		break;
	}
    
    //�ж�״̬
   CScrollView::OnLButtonDown(nFlags, point);
}

void CXiangqiView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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

// ����ʶ��
void CXiangqiView::DrawFlag1(CDC* pDC,CPoint VXY,int nStep, COLORREF color)
{
	//::AfxMessageBox(_T("ȷ�ڵ��û���ʶ����"));

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
	// TODO: �ڴ���������������
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
	// TODO: �ڴ���������������
}

void CXiangqiView::OnUpdateQijuStd(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	bool flag = m_nQijuStatus == 0;
	pCmdUI->SetCheck( flag );
	
	
}

void CXiangqiView::OnUpdateQijuUpturn(CCmdUI *pCmdUI)
{
	bool flag = m_nQijuStatus == 1;
	pCmdUI->SetCheck( flag );

	// TODO: �ڴ������������û����洦��������
}

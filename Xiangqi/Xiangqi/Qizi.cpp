//Qizi.cpp
//CQizi类（象棋棋子类的执行文件）；
#include "stdafx.h"
#include "Qizi.h"



CQizi::CQizi(int nStyle,CPoint ptPosition)
{

	//将棋子置活，默认的显示状态为显

	m_bIsAlive = true;
	m_bDisplay = true;
	this->Set(nStyle,ptPosition);
}
void CQizi::Set(int nStyle,CPoint ptPosition)
{
	
	m_ptPosition = ptPosition;
	m_nStyle = nStyle;
	switch(nStyle)
	{
	case QIZI_RED_BING:
		m_DIBImage.Load(IDR_DIB_RED_BING ,_T("DIB"));
		break;
	case QIZI_RED_PAO:
		m_DIBImage.Load(IDR_DIB_RED_PAO ,_T("DIB"));
		break;
	case QIZI_RED_MA :
		m_DIBImage.Load(IDR_DIB_RED_MA  ,_T("DIB"));
		break;
	case QIZI_RED_JU:
		m_DIBImage.Load(IDR_DIB_RED_JU ,_T("DIB"));
		break;
	case QIZI_RED_XIANG:
		m_DIBImage.Load(IDR_DIB_RED_XIANG,_T("DIB"));
		break;
	case QIZI_RED_SHI:
		m_DIBImage.Load(IDR_DIB_RED_SHI ,_T("DIB"));
		break;
	case QIZI_RED_SHUAI:
		m_DIBImage.Load(IDR_DIB_RED_SHUAI ,_T("DIB"));
		break;


	case QIZI_BLACK_BING:
		m_DIBImage.Load(IDR_DIB_BLACK_BING ,_T("DIB"));
		break;
	case QIZI_BLACK_PAO:
		m_DIBImage.Load(IDR_DIB_BLACK_PAO ,_T("DIB"));
		break;
	case QIZI_BLACK_MA:
		m_DIBImage.Load(IDR_DIB_BLACK_MA ,_T("DIB"));
		break;
	case QIZI_BLACK_JU:
		m_DIBImage.Load(IDR_DIB_BLACK_JU ,_T("DIB"));
		break;
	case QIZI_BLACK_XIANG:
		m_DIBImage.Load(IDR_DIB_BLACK_XIANG ,_T("DIB"));
		break;
	case QIZI_BLACK_SHI:
		m_DIBImage.Load(IDR_DIB_BLACK_SHI ,_T("DIB"));
		break;
	case QIZI_BLACK_SHUAI:
		m_DIBImage.Load(IDR_DIB_BLACK_SHUAI ,_T("DIB"));
		break;
	}
	m_DIBBackImage.Load(IDR_DIB_BACK,_T("DIB"));
	
}

/*
void  CQizi::MoveTo(CPoint XY, CRect* pRectOut, CRect* pRectIn)
	{
		CPoint pt1,pt2;

		pt1 = this->IdealXYToRealXY(m_ptPosition);
        pt1.Offset(-nStep/2- 1,-nStep/2- 1);

		pt2 =this->IdealXYToRealXY(m_ptPosition);
        pt2.Offset( nStep/2+ 1, nStep/2 + 1);

        *pRectOut = CRect(pt1,pt2);

		m_ptPosition = XY;

		pt1 = this->IdealXYToRealXY(m_ptPosition);
        pt1.Offset(-nStep/2- 1,-nStep/2- 1);

		pt2 =this->IdealXYToRealXY(m_ptPosition);
        pt2.Offset( nStep/2+ 1, nStep/2 + 1);

        * pRectIn = CRect(pt1,pt2);
	}

void CQizi::MoveTo(CPoint XY, CRect* pRectOut, CRect* pRectIn, int nInsight[90])
{
    int nIndex = m_ptPosition.x + m_ptPosition.y * 9;
	nInsight[nIndex] = QIZI_NULL;

	MoveTo(  XY, pRectOut, pRectIn);

	nIndex = m_ptPosition.x + m_ptPosition.y * 9;
	nInsight[nIndex] = m_nStyle;
}

void CQizi::MoveTo(CPoint XY,int nInsight[90])
{
	int nIndex = m_ptPosition.x + m_ptPosition.y * 9;
	nInsight[nIndex] = QIZI_NULL;

    m_ptPosition = XY;

   nIndex = m_ptPosition.x + m_ptPosition.y * 9;
	nInsight[nIndex] = m_nStyle;
}

void CQizi::IntelligentMoveTo(CPoint XY, int nQizisLocation[90])
{
	int nIndex = GetIndex();
	
	nQizisLocation[nIndex] = NOQIZI;
   
     m_ptPosition = XY;

	
     nIndex = GetIndex();
	 nQizisLocation[nIndex] = m_nExistID;
	
}

void CQizi::IntelligentMoveTo(CPoint XY, CRect *pRectOut,
							  CRect *pRectIn, int nQizisLocation[])
{
	int nIndex = GetIndex();
	nQizisLocation[nIndex] = NOQIZI;
	*pRectOut = this->GetLocation();
    
	 m_ptPosition = XY;

	  nIndex = GetIndex();
	 nQizisLocation[nIndex] = m_nExistID;
	 *pRectIn = this->GetLocation();
}

*/


    










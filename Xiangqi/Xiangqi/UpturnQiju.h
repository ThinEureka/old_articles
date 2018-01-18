#pragma once
#include "StdQiju.h"
#define UP_QIJU_FACEIN 1
#define UP_QIJU_IS_DYNAMIC 2
#define UP_QIJU_IS_EMPTY 3
#define UP_QIJU_IS_STATIC 4


class CUpturnQiju
{
public:
	CUpturnQiju(void);
public:
	virtual ~CUpturnQiju(void);
public:
	CPoint m_ptImage;
	CPoint m_ptOrigin;
	int m_nStep;
    CDib m_Qipan;
protected:
	CQizi m_Qizis[32];
	int m_pQizisLocation[32];
	int m_nPaces;
	int m_nStatus;
	int m_nDynamic;
	int m_nSelectedID;
	int m_nPreIndexs[5];
protected:
	int IXYToIndex( CPoint IXY);
	CPoint IndexToIXY( int nIndex);
	CPoint IXYToVXY(CPoint IXY);
	CPoint IndexToVXY(int nIndex);
	int VXYToIndex( CPoint VXY);
protected:
	bool IsInSight(CPoint VXY);
	bool IsDynamicQizi(int nIndex);
	int GetQiziColor(int nIndex);
	int Judge( int nIndex);
	bool IsInOrbit(int nSelectedID,int nIndex);
	int Compare(int nID,int nIndex);
protected:
	void MoveQizi( int nSelectedID,int nDestIndex);
	void Annihilate( int nSelectedID,int nDestIndex);
	void AddPreIndexs(int nIndex);
	void UpdatePreIndexs(CView* pView);
	void UpdateSelected(CView* pView);
	void ChangeDynamic();
	void SelectQizi(CView* pView, int nID);
	void TurnToStatic(CView* pView);

public:
	void Initial();
	void Action(CView* pView,CPoint VXY);
	void Display(CDC* pDC);
	void Wink(CView* pView);

};
inline CPoint CUpturnQiju::IndexToIXY( int nIndex)
{
	CPoint IXY;
	IXY.x = nIndex % 8;
	IXY.y = nIndex / 8;
	return IXY;
}

inline int CUpturnQiju::IXYToIndex(CPoint IXY)
{
	return IXY.y * 8 + IXY.x;
}
inline CPoint CUpturnQiju::IXYToVXY(CPoint IXY)
{
	CPoint VXY;
	VXY = m_ptOrigin;
	VXY.Offset(  m_nStep * IXY.x,m_nStep *IXY.y);
	return VXY;
}

inline CPoint CUpturnQiju::IndexToVXY(int nIndex)
{
	CPoint VXY = IXYToVXY( IndexToIXY( nIndex));
	return VXY;
}
inline int CUpturnQiju::GetQiziColor( int nIndex)
{
	int nID = m_pQizisLocation[nIndex];
	int nStyle = m_Qizis[nID].GetStyle();

	if( nStyle >0)
		return STDQIJU_RED;
	else
		return STDQIJU_BLACK;
}

inline bool CUpturnQiju::IsDynamicQizi( int nIndex)
{
	if( GetQiziColor( nIndex) != m_nDynamic)
		return false;
	return true;
}
inline void CUpturnQiju::AddPreIndexs(int nIndex)
{
	int nNum = m_nPreIndexs[0];
	this->m_nPreIndexs[nNum+1] = nIndex;
	m_nPreIndexs[0]++;

}
inline void CUpturnQiju::ChangeDynamic()
{
	m_nDynamic *= -1;
}

inline void CUpturnQiju::Wink(CView* pView)
{
	switch(m_nStatus)
	{
	case STDQIJU_STATUS_STATIC:
		break;
	case STDQIJU_STATUS_SELECTED1:
		m_Qizis[m_nSelectedID].Appear();
		UpdateSelected(pView);
		m_nStatus = STDQIJU_STATUS_SELECTED2;
		break;
	case STDQIJU_STATUS_SELECTED2:
		m_Qizis[m_nSelectedID].Disappear();
		UpdateSelected(pView);
		m_nStatus = STDQIJU_STATUS_SELECTED1;
		break;
	}
}

inline void CUpturnQiju::UpdateSelected(CView *pView)
{
	CPoint IXY = m_Qizis[m_nSelectedID].GetPosition();
	CPoint point1 =  IXYToVXY(IXY);
	CPoint point2 = point1;
	point1.Offset( -m_nStep/2-1,-m_nStep/2-1);
	point2.Offset( m_nStep/2+1,m_nStep/2+1);
	CRect r(point1,point2);
	pView->InvalidateRect(&r);
    pView->UpdateWindow();
}

inline bool CUpturnQiju::IsInOrbit(int nSelectedID, int nIndex)
{
	CPoint IXY1,IXY2;
	int dX,dY;
	IXY1 = m_Qizis[nSelectedID].GetPosition();
	IXY2 = IndexToIXY(nIndex);
    dX = abs( IXY1.x - IXY2.x);
	dY = abs( IXY1.y - IXY2.y);
    return dX+dY == 1;
} 
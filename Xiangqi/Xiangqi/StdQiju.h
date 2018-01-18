//StdQiju.h
//CStdQiju类（标准象棋棋局的头文件）
#pragma once
#include "Qizi.h"
#include "Resource.h"
#define STDQIJU_HOST_RED 1
#define STDQIJU_HOST_BLACK -1

#define STDQIJU_NULL 0
#define STDQIJU_RED 1
#define STDQIJU_BLACK -1

#define STDQIJU_NOQIZI -100

#define STDQIJU_NONE -10000

#define STDQIJU_STATUS_STATIC 1
#define STDQIJU_STATUS_SELECTED1 2
#define STDQIJU_STATUS_SELECTED2 3



class CStdQiju
{
public:
	CPoint m_ptImage;
	CPoint m_ptOrigin;
	int m_nStep;
    CDib m_Qipan;
	
protected:
	CQizi m_Qizis[32];
	int m_pQizisLocation[90];
	int m_nHost;
	int m_nSelectedID;
	int m_nPreSelectedID;
	int m_nPreIndex;
	int m_nPreIndex1;
	int m_nPreIndex2;
	int m_nPreIndex3;
	int m_nPreIndex4;
	int m_nDynamic;
	int m_nStatus;
    
protected:
	int IXYToIndex( CPoint IXY);
	CPoint IndexToIXY( int nIndex);
	CPoint IXYToVXY(CPoint IXY);
	CPoint IndexToVXY(int nIndex);
	int VXYToIndex( CPoint VXY);
	bool IsDynamicQizi(int nIndex);
	int GetQiziColor(int nIndex);
    void MoveQizi( int nSelectedID,int nDestIndex);
	void ChangeDynamic();
	void UpdateSelected(CView* pView);
	void UpdatePreSelected(CView* pView);
	void UpdatePreMove(CView* pView);
	void SelectQizi(CView* pView, int nID);
	void TurnToStatic(CView* pView);
	void Judge(CView* pView,CPoint VXY);

protected:
	int GetStyle(CPoint IXY);
	int GetColorFromIndex(int nIndex);
	int GetColorFromID(int nID);
	bool IsSat(CPoint IXY);
	bool IsInOrbit(int nID,int nIndex);
	bool IsInJuOrbit(int nID,int nIndex);
	bool IsInPaoOrbit(int nID,int nIndex);
	bool IsInMaOrbit(int nID,int nIndex);
	bool IsInShuaiOrbit(int nID,int nIndex);
	bool IsInBingOrbit(int nID,int nIndex);
	bool IsInXiangOrbit(int nID,int nIndex);
	bool IsInShiOrbit(int nID,int nIndex);
	bool IsInSelectedOrbit(int nIndex);


public:
	void Initial(int Insight[90],int nHost);
	bool IsInSight(CPoint VXY);
	void Display( CDC* pDC);
	void Action(CView* pView,CPoint VXY);
	void Wink(CView* pView);
	int GetStep();
	CPoint GetPreMove1();
	CPoint GetPreMove2();

	
};
inline CPoint CStdQiju::IndexToIXY( int nIndex)
{
	CPoint IXY;
	IXY.x = nIndex % 9;
	IXY.y = nIndex / 9;
	return IXY;
}
inline int CStdQiju::IXYToIndex(CPoint IXY)
{
	return IXY.y * 9 + IXY.x;
}

inline CPoint CStdQiju::IXYToVXY(CPoint IXY)
{
	CPoint VXY;
	VXY = m_ptOrigin;
	VXY.Offset(  m_nStep * IXY.x,m_nStep *IXY.y);
	return VXY;
}

inline CPoint CStdQiju::IndexToVXY(int nIndex)
{
	CPoint VXY = IXYToVXY( IndexToIXY( nIndex));
	return VXY;
}

inline int CStdQiju::GetQiziColor( int nIndex)
{
	int nID = m_pQizisLocation[nIndex];
	int nStyle = m_Qizis[nID].GetStyle();

	if( nStyle >0)
		return STDQIJU_RED;
	else
		return STDQIJU_BLACK;
}
inline bool CStdQiju::IsDynamicQizi( int nIndex)
{
	if( m_pQizisLocation[nIndex] == STDQIJU_NOQIZI)
	{
//		::AfxMessageBox(_T("空白"));
		return false;
	}
	if( GetQiziColor( nIndex) != m_nDynamic)
		return false;
	return true;
}
inline void CStdQiju::ChangeDynamic()
{
	m_nDynamic *= -1;
}
inline void CStdQiju::UpdateSelected(CView *pView)
{
	CPoint IXY = m_Qizis[m_nSelectedID].GetPosition();
	CPoint point1 =  IXYToVXY(IXY);
	CPoint point2 = point1;
	point1.Offset( -m_nStep/2,-m_nStep/2);
	point2.Offset( m_nStep/2,m_nStep/2);
	CRect r(point1,point2);
	pView->InvalidateRect(&r);
    pView->UpdateWindow();
}

inline void CStdQiju::Wink(CView* pView)
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


inline CPoint CStdQiju::GetPreMove1()
{
	if( m_nPreIndex1 !=  STDQIJU_NONE)
		return IndexToVXY( m_nPreIndex1);
	return CPoint(STDQIJU_NONE,STDQIJU_NONE);
}
inline CPoint CStdQiju::GetPreMove2()
{
	if( m_nPreIndex2 !=  STDQIJU_NONE)
		return IndexToVXY( m_nPreIndex2);
	return CPoint(STDQIJU_NONE,STDQIJU_NONE);
}
inline int CStdQiju::GetStep()
{
	return  m_nStep;
}

inline int CStdQiju::GetStyle(CPoint IXY)
{
	int nIndex = IXYToIndex(IXY);
	int nID = m_pQizisLocation[nIndex];
	return m_Qizis[nID].GetStyle();
}

inline bool CStdQiju::IsSat(CPoint IXY)
{
	int nIndex = IXYToIndex(IXY);
	int nID = m_pQizisLocation[nIndex];
	if(nID == STDQIJU_NOQIZI)
		return false;
	return true;
}

inline bool CStdQiju::IsInSelectedOrbit(int nIndex)
{

	return this->IsInOrbit(m_nSelectedID, nIndex);
}

inline int CStdQiju::GetColorFromIndex(int nIndex)
{
	int nID = m_pQizisLocation[nIndex];
	if( nID == STDQIJU_NOQIZI)
		return	STDQIJU_NULL;
	else
		return this->GetColorFromID(nID);
}
inline int CStdQiju::GetColorFromID(int nID)
{
	int nStyle = m_Qizis[nID].GetStyle();
	if(nStyle>0)
		return STDQIJU_RED;
	else 
		return STDQIJU_BLACK;
}




	




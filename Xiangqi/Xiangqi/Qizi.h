//Qizi.h
//CQizi类（象棋棋子类）的头文件
#pragma once

#include "stdafx.h"
#include "DIB.h"
#include "Resource.h"

//棋子的类型
#define	QIZI_RED_BING            1
#define QIZI_RED_PAO             2
#define QIZI_RED_MA              3
#define QIZI_RED_JU              4
#define QIZI_RED_XIANG           5
#define QIZI_RED_SHI             6
#define QIZI_RED_SHUAI           7

#define QIZI_BLACK_BING          -1
#define QIZI_BLACK_PAO           -2
#define QIZI_BLACK_MA            -3
#define QIZI_BLACK_JU            -4
#define QIZI_BLACK_XIANG         -5
#define QIZI_BLACK_SHI           -6
#define QIZI_BLACK_SHUAI         -7

//空与边界
#define QIZI_NULL                 0
#define QIZI_SOLID                100

//颜色
#define XIANG_COLOR_NULL               0
#define XIANG_COLOR_RED                1
#define XIANG_COLOR_BLACK             -1



class CQizi
{
public:
//	static CPoint ptOrigin;
//	static int nStep;
//	static CPoint ptYePosition;
private:
	int m_nStyle;
	CDib m_DIBImage;
	CDib m_DIBBackImage;
	CPoint m_ptPosition;
	int m_nExistID;
	bool m_bIsAlive;
	bool m_bDisplay;
	bool m_bFaceOut;
public:
	CQizi()
	{
		m_bIsAlive=true;
		m_bDisplay=true;
		m_bFaceOut=true;
	};
	CQizi(int nStyle,CPoint ptPosition);
	void Set(int nStyle,CPoint ptPosition);
	void Set(int nStyle,CPoint ptPosition,int nExistID);
	void Die();
	void Disappear();
	void Appear();
	void FaceOut();
	void FaceIn();
	int GetStyle();
	int GetExistID();
	bool IsFacingOut();
	CPoint GetPosition()
	{
		return m_ptPosition;
	};
	void MoveTo(CPoint IXY);

	
public:
	BOOL Display(CDC* pDC,CPoint VXY);
	void SetExistID(int nID);

};


inline BOOL CQizi::Display(CDC* pDC,CPoint VXY)
{
	if( m_bIsAlive&& m_bDisplay)
	{
		if( m_bFaceOut)
		     return  m_DIBImage.DisplayTransparent(pDC, VXY.x, VXY.y, RGB(255,255,255));
		else
			return m_DIBBackImage.DisplayTransparent(pDC,VXY.x,VXY.y,RGB(255,255,255));
	}
	else
		return false;

}

inline void CQizi::SetExistID(int nID)
{
	m_nExistID = nID;
}
	
	
inline void CQizi::Set(int nStyle,CPoint ptPosition,int nExistID)
{
	Set(nStyle,ptPosition);
	SetExistID(nExistID);
};

inline void CQizi::Die()
{

	m_bIsAlive = false;
};
inline void CQizi::Disappear()
{
	m_bDisplay = false;
};
inline void CQizi::Appear()
{
	m_bDisplay = true;
}


inline int CQizi::GetStyle()
{
	return m_nStyle;
};

inline int CQizi::GetExistID()
{
	return m_nExistID;
}

inline void CQizi::MoveTo(CPoint IXY)
{
	this->m_ptPosition = IXY;
}
inline bool CQizi::IsFacingOut()
{
	return this->m_bFaceOut;
}
inline void CQizi::FaceOut()
{
	m_bFaceOut = true;
}
inline void CQizi::FaceIn()
{
	m_bFaceOut = false;
}




//	int GetIndex()
//	{
//		return  m_ptPosition.x + m_ptPosition.y * 9;
//	}



			

//	static CPoint IdealXYToRealXY(CPoint XY)
//	{
//		CPoint rXY = ptYePosition;
 //       rXY.Offset( ptOrigin.x + XY.x * nStep,
//			        ptOrigin.y + XY.y * nStep);
//		return rXY;
//	};


//	void MoveTo(CPoint XY, CRect* pRectOut, CRect* pRectIn);
//	void MoveTo(CPoint XY, CRect* pRectOut, CRect* pRectIn, int nInsight[90]);
//    void MoveTo(CPoint XY,int nInsight[90]);

//	void IntelligentMoveTo(CPoint XY, CRect* pRectOut, CRect* pRectIn, int nQizisLocation[90]);
//	void IntelligentMoveTo(CPoint XY,int nQizisLocation[90]);
//	CRect GetLocation()
//	{
//		CPoint pt1,pt2;

//		pt1 = this->IdealXYToRealXY(m_ptPosition);
//
//		pt2 =this->IdealXYToRealXY(m_ptPosition);
 //       pt2.Offset( nStep/2, nStep/2 );

//		return CRect(pt1,pt2);
//	};




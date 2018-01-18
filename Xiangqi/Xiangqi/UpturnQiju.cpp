#include "StdAfx.h"
#include "UpturnQiju.h"

CUpturnQiju::CUpturnQiju(void)
{
	 m_nPaces = 0;
	 m_nPreIndexs[0]=0;
}

CUpturnQiju::~CUpturnQiju(void)
{
}

int CUpturnQiju::VXYToIndex(CPoint VXY)
{
	int X = (VXY.x - m_ptOrigin.x )/m_nStep;
	int Y = (VXY.y - m_ptOrigin.y )/m_nStep;

	if( m_ptOrigin.x + X * m_nStep + m_nStep/2  <VXY.x )
		X++;
    if( m_ptOrigin.y + Y * m_nStep + m_nStep/2  <VXY.y )
		Y++;
	
	int nIndex = IXYToIndex( CPoint(X,Y));
	return nIndex;
}

void CUpturnQiju::Initial()
{
	m_nStatus = STDQIJU_STATUS_STATIC;
	int Styles[] =
	{
		1,1,1,1,1,
		2,2,
		3,3,
		4,4,
		5,5,
		6,6,
		7,
		-1,-1,-1,-1,-1,
		-2,-2,
		-3,-3,
		-4,-4,
		-5,-5,
		-6,-6,
		-7
	};

	DWORD dwLeftNum = 31;
	double fMax= RAND_MAX;

	for(int i=0; i<32; i++)
	{
//		long ranNum = (::rand() + GetTickCount())%RAND_MAX;
//		DWORD n= (DWORD) ((double)dwLeftNum*(double)((double)ranNum/fMax));
		DWORD n = (::GetTickCount()+::rand()) % (dwLeftNum+1);
        int nStyle = Styles[n];
		m_Qizis[i].Set( nStyle, IndexToIXY(i), i);
		m_Qizis[i].FaceIn();
		m_pQizisLocation[i]=i;
		Styles[n] = Styles[dwLeftNum];
		dwLeftNum--;
//		::Delay(::rand()%2);
	}
}
void CUpturnQiju::Display(CDC *pDC)
{
	m_Qipan.Display(pDC,m_ptImage.x,m_ptImage.y);
	CPoint IXY;
	CPoint VXY2;
	for(int i=0; i<32; i++)
	{
		IXY = m_Qizis[i].GetPosition();
		VXY2 = IXYToVXY(IXY);
		VXY2.Offset(-m_nStep/2,-m_nStep/2);
		m_Qizis[i].Display(pDC,VXY2);
	}
}
bool CUpturnQiju::IsInSight(CPoint VXY)
{
	CPoint XY= m_ptOrigin;
	XY.Offset(-m_nStep/2,-m_nStep/2);
	if( VXY.x < XY.x )
		return false;
	if( VXY.y < XY.y )
		return false;
	XY= IXYToVXY( CPoint(7,3));
    XY.Offset(m_nStep/2,m_nStep/2);
	if( VXY.x > XY.x )
		return false;
	if( VXY.y > XY.y )
		return false;

	return true;
}

void CUpturnQiju::Action(CView *pView, CPoint VXY)
{
	switch(m_nStatus)
	{
	case STDQIJU_STATUS_STATIC:
		if( !IsInSight(VXY))
			return;
		else
		{
			int nIndex = VXYToIndex(VXY);
			switch( Judge( nIndex))
			{
			case UP_QIJU_FACEIN:
				{
					this->AddPreIndexs(nIndex);
					if( m_nPaces == 0)
						m_nDynamic = GetQiziColor( nIndex);
					int nID = this->m_pQizisLocation[nIndex];
					m_Qizis[nID].FaceOut();
					m_nPaces++;
					this->ChangeDynamic();
				}
				break;
			case UP_QIJU_IS_EMPTY:
			case UP_QIJU_IS_STATIC:
//				::AfxMessageBox(_T("非选定"));
				this->TurnToStatic(pView);
				break;
			case UP_QIJU_IS_DYNAMIC:
				{
					int nID = m_pQizisLocation[nIndex];
//					CString str;
//					str.Format(_T("%d"),nID);
//					::AfxMessageBox(str);
					this->SelectQizi(pView,nID);
				}
				break;
			}
		}
		break;


	case STDQIJU_STATUS_SELECTED1:
	case STDQIJU_STATUS_SELECTED2:
		if( !IsInSight(VXY))
			return;
		else
		{
			int nIndex = VXYToIndex(VXY);
			switch( Judge( nIndex))
			{
			case UP_QIJU_FACEIN:
				this->TurnToStatic(pView);
				break;
			case UP_QIJU_IS_DYNAMIC:
				{
					int nID = m_pQizisLocation[nIndex];
					if( nID == m_nSelectedID)
					{
						this->TurnToStatic(pView);
					}
					else
					{
						this->SelectQizi(pView,nID);
					}
				}
				break;
			case UP_QIJU_IS_STATIC:
				{
					if(!IsInOrbit(m_nSelectedID,nIndex))
					{
						this->TurnToStatic(pView);
					}
					else
					{

						switch( Compare(m_nSelectedID,nIndex) )
						{
						case 0:
							this->Annihilate( m_nSelectedID,nIndex );
							this->TurnToStatic(pView);
							this->ChangeDynamic();
							this->m_nPaces++;
							break;
						case 1:
							this->TurnToStatic(pView);
							break;
						case -1:
							this->MoveQizi(m_nSelectedID,nIndex);
							this->TurnToStatic(pView);
							this->ChangeDynamic();
							m_nPaces++;
							break;
						}
					}
				}

				break;
			case UP_QIJU_IS_EMPTY:
				if(!IsInOrbit(m_nSelectedID,nIndex))
				{
					this->TurnToStatic(pView);
				}
				else
				{
					this->MoveQizi(m_nSelectedID,nIndex);
					this->TurnToStatic(pView);
					this->ChangeDynamic();
					m_nPaces++;
					break;
				}
				break;


			}


		}
		break;
	}
	this->UpdatePreIndexs(pView);

}
int CUpturnQiju::Judge(int nIndex)
{
	int nID = this->m_pQizisLocation[nIndex];
	if(nID == STDQIJU_NOQIZI)
		return UP_QIJU_IS_EMPTY;
	else
	{
		if( !m_Qizis[nID].IsFacingOut() )
			return UP_QIJU_FACEIN;
		else
		{
			if( IsDynamicQizi( nIndex) )
				return UP_QIJU_IS_DYNAMIC;
			else
				return UP_QIJU_IS_STATIC;
		}
	}
}


void CUpturnQiju::MoveQizi(int nSelectedID, int nDestIndex)
{

	CPoint IXY = m_Qizis[nSelectedID].GetPosition();
    int nIndex = IXYToIndex(IXY);
	this->AddPreIndexs(nIndex);
	this->AddPreIndexs(nDestIndex);
	int nID = m_pQizisLocation[nDestIndex];

	if(nID != STDQIJU_NOQIZI)
	{
		
		m_Qizis[nID].Die();
	}

	m_Qizis[nSelectedID].MoveTo(IndexToIXY(nDestIndex));

    m_pQizisLocation[nIndex] = STDQIJU_NOQIZI;
	m_pQizisLocation[nDestIndex] = nSelectedID;


}
void CUpturnQiju::Annihilate(int nSelectedID, int nDestIndex)
{
	CPoint IXY = m_Qizis[nSelectedID].GetPosition();
    int nIndex = IXYToIndex(IXY);
	this->AddPreIndexs(nIndex);
	this->AddPreIndexs(nDestIndex);
	int nID = m_pQizisLocation[nDestIndex];

	m_Qizis[nID].Die();
    m_Qizis[nSelectedID].Die();

	m_pQizisLocation[nIndex] = STDQIJU_NOQIZI;
	m_pQizisLocation[nDestIndex] = STDQIJU_NOQIZI;
}


void CUpturnQiju::UpdatePreIndexs(CView *pView)
{
	CPoint point1,point2;
	CRect r;
	int nNum = m_nPreIndexs[0];
//	CString str;
//	str.Format(_T("更新数为%d"),nNum);
//	::AfxMessageBox(str);
	for(int i=1; i<=nNum; i++)
	{
		int nPre = m_nPreIndexs[i];
		point2 = point1 = IndexToVXY(nPre);
		point1.Offset( -m_nStep/2-1,-m_nStep/2-1);
		point2.Offset( m_nStep/2+1,m_nStep/2+1);
		r = CRect(point1,point2);
		pView->InvalidateRect(&r);
	}
	pView->UpdateWindow();
	m_nPreIndexs[0] = 0;
}
void CUpturnQiju::SelectQizi(CView *pView, int nID)
{
	switch( m_nStatus)
	{
	case  STDQIJU_STATUS_SELECTED1:
	case  STDQIJU_STATUS_SELECTED2:
		{
			int nIndex = IXYToIndex( m_Qizis[m_nSelectedID].GetPosition() );
			this->AddPreIndexs( nIndex);
			pView->KillTimer(1);
			m_Qizis[m_nSelectedID].Appear();
		}
		break;

	case  STDQIJU_STATUS_STATIC:
		break;
	}
	m_nSelectedID = nID;
	pView->SetTimer(1,500,NULL);
	m_nStatus = STDQIJU_STATUS_SELECTED2;
}

void CUpturnQiju::TurnToStatic(CView* pView)
{
	switch(this->m_nStatus)
	{
	case  STDQIJU_STATUS_STATIC:
		break;
	case  STDQIJU_STATUS_SELECTED1:
	case  STDQIJU_STATUS_SELECTED2:
		{
			int nIndex = IXYToIndex( m_Qizis[m_nSelectedID].GetPosition());
			pView->KillTimer(1);
			this->AddPreIndexs( nIndex);
				m_Qizis[m_nSelectedID].Appear();
			m_nStatus = STDQIJU_STATUS_STATIC;
		}
		break;
	}

}
int CUpturnQiju::Compare(int nID, int nIndex)
{
	int nSty1,nSty2;
	nSty1 = abs(m_Qizis[nID].GetStyle());
	int nID2 = this->m_pQizisLocation[nIndex];
	nSty2 = abs(m_Qizis[nID2].GetStyle());

	if( nSty1 == nSty2)
		return 0;
	switch( nSty1)
	{
	case  QIZI_RED_SHUAI:
		if( nSty2 == QIZI_RED_BING)
			return 1;
		else
			return -1;
		break;
	case QIZI_RED_BING:
		if( nSty2 == QIZI_RED_SHUAI)
			return -1;
		else
			return 1;
	default:
		if( nSty2 > nSty1)
			return 1;
		else
			return -1;
	}
}
		
















		


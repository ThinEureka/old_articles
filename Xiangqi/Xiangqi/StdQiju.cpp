//StdQiju.cpp
//CStdQiju类的执行文件
#include "stdafx.h"
#include "StdQiju.h"

int CStdQiju::VXYToIndex(CPoint VXY)
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

void CStdQiju::Initial(int Insight[90],int nHost)
{
	m_nHost = nHost;
    m_nDynamic = STDQIJU_RED;
	m_nStatus = STDQIJU_STATUS_STATIC;
	m_nPreIndex  =  STDQIJU_NONE;
	m_nPreIndex1 =  STDQIJU_NONE;
	m_nPreIndex2 =  STDQIJU_NONE;
	m_nPreIndex3 =  STDQIJU_NONE;
	m_nPreIndex4 =  STDQIJU_NONE;
	m_nSelectedID = STDQIJU_NONE;
    for(int i=0; i<90; i++)
		m_pQizisLocation[i] = STDQIJU_NOQIZI;
    int nStyle;
	int j=0;
	for(int i=0; i<90; i++)
	{
		nStyle = Insight[i] * m_nHost;
		if( nStyle!= QIZI_NULL && nStyle > -8 && nStyle < 8)
		{
			m_pQizisLocation[i] = j;
			m_Qizis[j].Set(nStyle,IndexToIXY(i),j);
			j++;
		}
	}
}

bool CStdQiju::IsInSight(CPoint VXY)
{
	CPoint XY= m_ptOrigin;
	XY.Offset(-m_nStep/2,-m_nStep/2);
	if( VXY.x < XY.x )
		return false;
	if( VXY.y < XY.y )
		return false;
	XY= IXYToVXY( CPoint(8,9));
    XY.Offset(m_nStep/2,m_nStep/2);
	if( VXY.x > XY.x )
		return false;
	if( VXY.y > XY.y )
		return false;

	return true;
}
void CStdQiju::Display(CDC* pDC)
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

void CStdQiju::MoveQizi(int nSelectedID, int nDestIndex)
{

	CPoint IXY = m_Qizis[nSelectedID].GetPosition();
	m_nPreIndex3 = m_nPreIndex1;
	m_nPreIndex4 = m_nPreIndex2;
	m_nPreIndex1 = IXYToIndex(IXY);
	m_nPreIndex2 = nDestIndex;
	m_pQizisLocation[m_nPreIndex1] = STDQIJU_NOQIZI;

	int nID = m_pQizisLocation[nDestIndex];
	
	if(nID != STDQIJU_NOQIZI)
	{
		
		m_Qizis[nID].Die();
	}

	m_pQizisLocation[nDestIndex] = nSelectedID;
	m_Qizis[nSelectedID].MoveTo(IndexToIXY(nDestIndex));


}
void CStdQiju::Action(CView *pView, CPoint VXY)
{
	switch(m_nStatus)
	{
	case STDQIJU_STATUS_STATIC:
		if( !IsInSight(VXY))
			return;

		else
		{
			int nIndex = VXYToIndex(VXY);
			if( !IsDynamicQizi(nIndex))
				return;
			else
			{
				int nID = m_pQizisLocation[nIndex];
				this->SelectQizi(pView,nID);
			}
		}
		break;

	case STDQIJU_STATUS_SELECTED1:
	case STDQIJU_STATUS_SELECTED2:
		this->Judge(pView,VXY);
		break;
	}

}
bool CStdQiju::IsInOrbit(int nID,int nIndex)
{
	int nStyle = m_Qizis[nID].GetStyle();
	switch(nStyle)
	{
	case QIZI_RED_JU:
	case QIZI_BLACK_JU:
		return IsInJuOrbit(nID,nIndex);
		break;
	case QIZI_RED_PAO:
	case QIZI_BLACK_PAO:
		return this->IsInPaoOrbit(nID,nIndex);
	case QIZI_RED_MA:
	case QIZI_BLACK_MA:
		return this->IsInMaOrbit(nID,nIndex);
	case QIZI_RED_SHUAI:
	case QIZI_BLACK_SHUAI:
		return this->IsInShuaiOrbit(nID,nIndex);
	case QIZI_RED_BING:
	case QIZI_BLACK_BING:
		return this->IsInBingOrbit(nID,nIndex);
	case QIZI_RED_XIANG:
	case QIZI_BLACK_XIANG:
		return this->IsInXiangOrbit(nID,nIndex);
	case QIZI_RED_SHI:
	case QIZI_BLACK_SHI:
		return this->IsInShiOrbit(nID,nIndex);

	}
	return true;
}

bool CStdQiju::IsInJuOrbit(int nID,int nIndex)
{
	int c1,c2;
	c1 = GetColorFromID(nID);
	c2 = this->GetColorFromIndex(nIndex);
	if( c2 == c1)
		return false;
	CPoint JuXY = m_Qizis[nID].GetPosition();
	CPoint SubmitXY = IndexToIXY(nIndex);
   int dX = SubmitXY.x - JuXY.x;
   int dY = SubmitXY.y - JuXY.y;
   int x1,x2,x;
   int y1,y2,y;
   CPoint point;
   bool bResult = true;
   if(dX==0)
   {
	   if(dY==0)
		   return false;
	   else
	   {
		   x=JuXY.x;
		 if(dY<0)
		 {
			y1 = SubmitXY.y;
			y2 = JuXY.y;
		 }
		 else
		 {
			y1 = JuXY.y;
		    y2 = SubmitXY.y;
		 }
		  for( y=y1+1; y<y2;  y++)
		   {
			   point = CPoint(x,y);
			   if( IsSat(point) )
			   {
				   bResult = false;
				   break;
			   }
		   }
		  return bResult;
	   }
   }
   if(dY == 0)
    {
		y=JuXY.y;
		if(dX<0)
	    {
			x1 = SubmitXY.x;
			x2 = JuXY.x;
		 }
		 else
		 {
			x1 = JuXY.x;
		    x2 = SubmitXY.x;
		 }
		  for( x=x1+1; x<x2;  x++)
		   {
			   point = CPoint(x,y);
			   if( IsSat(point) )
			   {
				   bResult = false;
				   break;
			   }
		   }
		  return bResult;
   }
   return false;
}
bool CStdQiju::IsInPaoOrbit(int nID,int nIndex)
{
	int c1,c2;
	c1 = GetColorFromID(nID);
	c2 = this->GetColorFromIndex(nIndex);

	if( c2 == c1)
		return false;
	if( c2 ==  STDQIJU_NULL)
	{  
		return this->IsInJuOrbit(nID,nIndex);
	}

	CPoint JuXY = m_Qizis[nID].GetPosition();
	CPoint SubmitXY = IndexToIXY(nIndex);
   int dX = SubmitXY.x - JuXY.x;
   int dY = SubmitXY.y - JuXY.y;
   int x1,x2,x;
   int y1,y2,y;
   CPoint point;
   int  nCount = 0;
   if(dX==0)
   {
	   x=JuXY.x;
	   if(dY<0)
	   {
		   y1 = SubmitXY.y;
		   y2 = JuXY.y;
	   }
	   else
	   {
		   y1 = JuXY.y;
		   y2 = SubmitXY.y;
	   }
	   for( y=y1+1; y<y2;  y++)
	   {
		   point = CPoint(x,y);
		   if( IsSat(point) )
			   nCount++;

	   }
		 if(nCount==1)
			 return true;
		 else 
			 return false;
   }

   else if(dY == 0)
    {
		y=JuXY.y;
		if(dX<0)
	    {
			x1 = SubmitXY.x;
			x2 = JuXY.x;
		 }
		 else
		 {
			x1 = JuXY.x;
		    x2 = SubmitXY.x;
		 }
		  for( x=x1+1; x<x2;  x++)
		   {
			   point = CPoint(x,y);
			   if( IsSat(point) )
				   nCount++;

		   }
		 if(nCount==1)
			 return true;
		 else
			 return false;
   }
   else
	   return false;
}
bool CStdQiju::IsInMaOrbit(int nID, int nIndex)
{
	int c1,c2;
	c1 = GetColorFromID(nID);
	c2 = this->GetColorFromIndex(nIndex);

	if( c2 == c1)
		return false;
	CPoint IXY = IndexToIXY(nIndex);
	CPoint MaXY = m_Qizis[nID].GetPosition();
	int dX = IXY.x - MaXY.x;
    int dY = IXY.y - MaXY.y;

	CPoint point = MaXY;
	point.Offset( dX/2,dY/2);
   if( IsSat(point) )
		return false;
	else
	{
		if( (abs(dX)+abs(dY) ==3) && dX*dY!=0)
			return true;
		else
			return false;

	}
}
bool CStdQiju::IsInShuaiOrbit(int nID,int nIndex)
{
	int c1,c2;
	c1 = GetColorFromID(nID);
	c2 = this->GetColorFromIndex(nIndex);

	if( c2 == c1)
		return false;

	CPoint IXY = IndexToIXY(nIndex);
	CPoint ShuaiXY = m_Qizis[nID].GetPosition();
	CPoint XY = IXY - CPoint(4,int(5.0+3.5*c1*m_nHost));
	int dX = IXY.x - ShuaiXY.x;
    int dY = IXY.y - ShuaiXY.y;
    
	if( XY.x/2==0 && XY.y/2==0 && abs(dX)+abs(dY)==1)
		return true;
	else
		return false;
}

bool CStdQiju::IsInBingOrbit(int nID,int nIndex)
{
	int c1,c2;
	c1 = GetColorFromID(nID);
	c2 = this->GetColorFromIndex(nIndex);

	if( c2 == c1)
		return false;
	int maxX = 1;
	CPoint IXY = IndexToIXY(nIndex);
	CPoint BingXY = m_Qizis[nID].GetPosition();
	if( (float(BingXY.y)-4.5)*c1*m_nHost>0)
		maxX = 0;

	int dX = IXY.x - BingXY.x;
    int dY = IXY.y - BingXY.y;
    
	if( abs(dX)+abs(dY)==1 &&c1*m_nHost*dY<=0 &&abs(dX)<=maxX)
		return true;
	else
		return false;
}
bool CStdQiju::IsInXiangOrbit(int nID,int nIndex)
{
	int c1,c2;
	c1 = GetColorFromID(nID);
	c2 = this->GetColorFromIndex(nIndex);

	if( c2 == c1)
		return false;
     
	CPoint IXY = IndexToIXY(nIndex);
	CPoint XiangXY = m_Qizis[nID].GetPosition();
	int dX = IXY.x - XiangXY.x;
    int dY = IXY.y - XiangXY.y;
    CPoint point = XiangXY;
	point.Offset(dX/2,dY/2);
	if( abs(dX)==2 && abs(dY)==2 && !IsSat(point) &&(float(IXY.y)-4.5)*c1*m_nHost>0)
		return true;
	else
		return false;
}
bool CStdQiju::IsInShiOrbit(int nID,int nIndex)
{
	int c1,c2;
	c1 = GetColorFromID(nID);
	c2 = this->GetColorFromIndex(nIndex);

	if( c2 == c1)
		return false;
	CPoint IXY = IndexToIXY(nIndex);
	CPoint ShiXY = m_Qizis[nID].GetPosition();
	CPoint XY = IXY - CPoint(4,int(5.0+3.5*c1*m_nHost));
	int dX = IXY.x -ShiXY.x;
    int dY = IXY.y - ShiXY.y;
    
	if( abs(dX)==1 && abs(dY)==1 && abs(XY.x )<=1 && abs(XY.y)<=1)
		return true;
	else
		return false;
}

     

void CStdQiju::SelectQizi(CView* pView,int nID)
{ 
   if(m_nSelectedID != STDQIJU_NONE)
   {
          m_nPreSelectedID = m_nSelectedID;
          m_nPreIndex  = IXYToIndex( m_Qizis[m_nSelectedID].GetPosition() );
   }
	m_nSelectedID = nID;

//    m_Qizis[m_nSelectedID].Disappear();
//	this->UpdateSelected(pView);
	if( m_nStatus ==STDQIJU_STATUS_SELECTED1 ||
		          m_nStatus == STDQIJU_STATUS_SELECTED2)
				  pView->KillTimer(1);
	pView->SetTimer(1,500,NULL);
	m_nStatus = STDQIJU_STATUS_SELECTED2;
	

}
void CStdQiju::TurnToStatic(CView* pView)
{
	pView->KillTimer(1);
	m_Qizis[m_nSelectedID].Appear();
	UpdatePreMove(pView);
	m_nStatus = STDQIJU_STATUS_STATIC;

}
void CStdQiju::UpdatePreMove(CView *pView)
{
	if(m_nPreIndex1!= STDQIJU_NONE )
	{
		CPoint point1 = IndexToVXY(m_nPreIndex1);
	    CPoint point2 = point1;
	    point1.Offset( -m_nStep/2,-m_nStep/2);
		point2.Offset( m_nStep/2,m_nStep/2);
		CRect r = CRect(point1,point2);
		pView->InvalidateRect(&r);

		point1 = IndexToVXY(m_nPreIndex3);
	    point2 = point1;
	    point1.Offset( -m_nStep/2,-m_nStep/2);
		point2.Offset( m_nStep/2,m_nStep/2);
		r = CRect(point1,point2);
	    pView->InvalidateRect(&r);
	}
	if(m_nPreIndex2!= STDQIJU_NONE )
	{
		CPoint point1 = IndexToVXY(m_nPreIndex2);
		CPoint point2 = point1;
		point1.Offset( -m_nStep/2,-m_nStep/2);
		point2.Offset( m_nStep/2,m_nStep/2);
		CRect r = CRect(point1,point2);
		pView->InvalidateRect(&r);

		point1 = IndexToVXY(m_nPreIndex4);
		point2 = point1;
		point1.Offset( -m_nStep/2,-m_nStep/2);
		point2.Offset( m_nStep/2,m_nStep/2);
		r = CRect(point1,point2);
		pView->InvalidateRect(&r);
	}
	pView->UpdateWindow();
}
void CStdQiju::UpdatePreSelected(CView *pView)
{
	
	if(m_nPreIndex!= STDQIJU_NONE )
	{
		CPoint point1 = IndexToVXY(m_nPreIndex);
	    CPoint point2 = point1;
	    point1.Offset( -m_nStep/2,-m_nStep/2);
		point2.Offset( m_nStep/2,m_nStep/2);
		CRect r = CRect(point1,point2);
		pView->InvalidateRect(&r);

		pView->UpdateWindow();
	}

}


void CStdQiju::Judge(CView* pView,CPoint VXY)
{
	if( !IsInSight(VXY))
			return;
	
   int nIndex = VXYToIndex(VXY);
   if( IsDynamicQizi( nIndex) )
   {
	    int nID = m_pQizisLocation[nIndex];
		if( nID == m_nSelectedID)
		{
			this->TurnToStatic(pView);
			this->UpdateSelected(pView);
		}
		else
		{
			this->SelectQizi(pView,nID);
			m_Qizis[m_nPreSelectedID].Appear();
			this->UpdatePreSelected(pView);
		}
   }
   else
   {
	   if( IsInSelectedOrbit(nIndex) )
	   {

		   this->MoveQizi(m_nSelectedID,nIndex);
		   this->TurnToStatic(pView);
			ChangeDynamic();
		}
	   else
	   {
			
			 this->TurnToStatic(pView);
			 this->UpdateSelected(pView);
			
	   }

   }
}









			   

		



    

	









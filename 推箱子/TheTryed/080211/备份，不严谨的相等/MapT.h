//MapT.h
//地图的头文件

#pragma once 
#include <iostream>
#include <vector>

const int EMPTY = 0;
const int MAN = 2;
const int BOX = 3;
const int SOLID = 1;
const int LEFT = 1;
const int RIGHT = 4;
const int UP = 2;
const int DOWN = 3;

class CPoint
{
private:
	int m_nX;
	int m_nY;
public:
	CPoint()
	{};
	CPoint(const int &nX,const int &nY):m_nX(nX),m_nY(nY)
	{};
public:
	int & eX()
	{
		return m_nX;
	};
	int & eY()
	{
		return m_nY;
	};
public:
	int ceX()const
	{
		return m_nX;
	};
	int ceY()const
	{
		return m_nY;
	};
public:
	CPoint Left()const
	{
		return CPoint( ceX()-1,ceY() );
	};
	CPoint Right()const
	{
		return CPoint( ceX()+1,ceY() );
	};
	CPoint Up()const
	{
		return CPoint( ceX(), ceY() - 1);
	};
	CPoint Down()const
	{
		return CPoint( ceX(), ceY() + 1);
	};
public:
	CPoint De(const int &nDirection)const
	{
		switch( nDirection)
		{
		case LEFT:
			return this->Left();
		case RIGHT:
			return this->Right();
		case UP:
			return this->Up();
		case DOWN:
			return this->Down();
		}
	};
	friend std::ostream & operator << ( std::ostream & os,const CPoint &Point)
	{
		os<<Point.ceX()<<" "<<Point.ceY();
		return os;
	};
};


struct CPushCase
{
	CPoint m_ptLocation;
	int m_nDirection;
	CPushCase(CPoint ptLocation,int nDirection):m_ptLocation(ptLocation),m_nDirection(nDirection)
	{};
};


template<int R,int L,int N>
class CMapT
{
private:
	int m_ppnSpace[R][L];
	CPoint m_ptBoxs[N];
	CPoint m_ptStart;
	std::vector<CPoint> m_vPath;
	int m_nStatus;
public:
	int m_nPreID;
//	int m_nID;
public:
	CMapT()
	{};
	~CMapT()
	{};
public:
	int ce(const int &nRow,const int &nLine)const
	{
		return m_ppnSpace[nRow-1][nLine-1];
	};
	int & e(const int &nRow,const int &nLine)
	{
		return m_ppnSpace[nRow-1][nLine-1];
	};
	int ce( const CPoint Point)const
	{
		return ce( Point.ceY(),Point.ceX() );
	};
	int & e( const CPoint  Point)
	{
		return e( Point.ceY(), Point.ceX() );
	};
	int Status()const
	{
		return m_nStatus;
	};
public:
	void Pick(const CPoint Point)
	{
		switch( this->ce( Point ))
		{
		case EMPTY:
			this->e( Point) = MAN;
			this->m_vPath.push_back(Point);
			break;
		case MAN:
			break;
		case SOLID:
			break;
		case BOX:
			break;
		}
	};
	void Judge(const CPoint Point)
	{
		Pick( Point.Left() );
		Pick( Point.Right() );
		Pick( Point.Up() );
		Pick( Point.Down() );
	};
	void Exploit()
	{
		while( !m_vPath.empty() )
		{
			m_vPath.pop_back();
		}
		Pick(m_ptStart);
		int nCount = 0;
		while( nCount < m_vPath.size() )
		{
			Judge( m_vPath.at(nCount) );
			++nCount;
		}
	};
public:
	bool CanPush(const int &nBox,const int &nDirection)const
	{
		int nTo = this->ce(m_ptBoxs[nBox].De(nDirection) );
		int nFrom = this->ce(m_ptBoxs[nBox].De(5-nDirection));
		return nFrom == MAN && ( nTo == MAN || nTo == EMPTY);

	};
	void Push(CMapT & Map,const int &nBox,const int &nDirection)const
	{
		for( int i = 0;i<R; ++ i)
		{
			for( int j=0; j<L; ++j)
			{
				switch( this->ce( 1+i,1+j) )
				{
				case MAN:
				case EMPTY:
				case BOX:
					Map.e(1+i,1+j) = EMPTY;
					break;
				case SOLID:
					Map.e(1+i,1+j) = SOLID;
					break;
				
				}
			}
		}
		for( int i = 0; i<N;++i)
		{
			Map.m_ptBoxs[i] = this->m_ptBoxs[i];
		}
		Map.m_ptBoxs[nBox] = this->m_ptBoxs[nBox].De(nDirection);
		for( int i = 0; i<N;++i)
		{
			Map.e( Map.m_ptBoxs[i] ) = BOX;
		}
		Map.m_ptStart = this->m_ptBoxs[nBox];
//		Map.Exploit();
		Map.m_nStatus = 0;

	};

	bool TryPush( CMapT & Map,int * pnStatus)
	{
		while( m_nStatus < N*4)
		{
			int nBox = m_nStatus/4;
			int nDirection = m_nStatus%4 + 1;
			if( CanPush(nBox,nDirection) )
			{
				Push(Map,nBox,nDirection);
				*pnStatus = ++m_nStatus;
				return true;
				
			}
			else
			{
				++m_nStatus;
			}
		}
		*pnStatus = m_nStatus;
		return false;

	};
public:
	void Create( const int Xs[],const CPoint ptBoxs[],const CPoint ptStart)
	{
		for( int i = 0;i<R;++i)
		{
			for( int j = 0; j<L; ++j)
			{
				this->e(1+i,1+j) = Xs[i*L + j];
			}
		}
		for( int i=0; i<N; ++i)
		{
			m_ptBoxs[i] = ptBoxs[i];
			this->e( m_ptBoxs[i] ) = BOX;
		}
		m_ptStart = ptStart;
		m_nStatus = 0;
		this->Exploit();
	};
	friend bool operator == ( const CMapT & Map1,const CMapT & Map2)
	{
		for( int i=0;i<N;++i)
		{
			if( Map2.ce( Map1.m_ptBoxs[i] ) != BOX )
				return false;
		}
		return true;
	}

	friend bool operator != ( const CMapT & Map1,const CMapT & Map2)
	{
		return !(Map1 == Map2);
	}
	friend std::ostream& operator <<( std::ostream &os,const CMapT & Map)
	{
		
		for( int i = 0;i<R;++i)
		{
			for( int j=0;j<L;++j)
			{
				os<<Map.ce(1+i,1+j);
			}
			os<<std::endl;
		}
		return os;
	}
public:
	CPushCase PushCase()const
	{
		int nBox = (m_nStatus-1)/4;
		int nDirection = (m_nStatus-1)%4 + 1;
		return CPushCase( m_ptBoxs[nBox],nDirection);

	};







	
};
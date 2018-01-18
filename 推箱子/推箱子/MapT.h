//MapT.h
//地图的头文件

#pragma once 
#include <iostream>
#include <vector>

const int EMPTY = 0;
const int EMPTY1 = 9;
const int MAN = 2;
const int MAN1 = 8;
const int BOX = 3;
const int SOLID = 1;
const int LEFT = 1;
const int RIGHT = 4;
const int UP = 2;
const int DOWN = 3;


//定义点的类，为了方便表示矩形区域内的一个位置
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
	//得到本点各个方向点的坐标。
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
	//输出到流
	friend std::ostream & operator << ( std::ostream & os,const CPoint &Point)
	{
		os<<Point.ceX()<<" "<<Point.ceY();
		return os;
	};
	//相等与不等的重载
	friend bool operator == ( const CPoint & Point1,const CPoint & Point2)
	{
		return Point1.ceX() == Point2.ceX() && Point1.ceY() == Point2.ceY();
	};
	friend bool operator != ( const CPoint & Point1,const CPoint & Point2)
	{
		return Point1.ceX() != Point2.ceX() || Point1.ceY() != Point2.ceY();
	}
};

//表示某一种的推法的结构
struct CPushCase
{
	CPoint m_ptLocation;
	int m_nDirection;
	CPushCase(CPoint ptLocation,int nDirection):m_ptLocation(ptLocation),m_nDirection(nDirection)
	{};
};

//表示一种游戏状态（或者说地图）
template<int R,int L,int N>
class CMapT
{
private:
	int m_ppnSpace[R][L];			//表示游戏布局的二维数组
	CPoint m_ptBoxs[N];				//表示当前地图的箱子的位置
	CPoint m_ptStart;				//人所站的位置
	std::vector<CPoint> m_vPath;	//一个临时的链表，用于标识MAN
	int m_nStatus;					//表示当前已试推过的状态。
public:
	int m_nPreID;					//当前地图的上游地图的ID
//	int m_nID;
public:
	CMapT()
	{};
	~CMapT()
	{};
public:
	//获取布局中物件的类型
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
	//为了判断人是否在所推箱子的反方向采用标识法，
	//引入下面三个函数，Pick直接做标识
	void Pick(const CPoint Point)
	{
		//如果类型是EMPTY与EMPTY1，则将其标识为MAN与MAN1
		switch( this->ce( Point ))
		{
		case EMPTY:
			this->e( Point) = MAN;
			this->m_vPath.push_back(Point);
			break;
		case EMPTY1:
			this->e( Point) = MAN1;
			this->m_vPath.push_back( Point);
			break;
		case MAN:
			break;
		case MAN1:
			break;
		case SOLID:
			break;
		case BOX:
			break;
		}
	};
	//标识相邻的四点
	void Judge(const CPoint Point)
	{
		Pick( Point.Left() );
		Pick( Point.Right() );
		Pick( Point.Up() );
		Pick( Point.Down() );
	};
	//调用此函数，可实现MAN的有效的标识
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
	//判断某一个箱子在指定方向上是否可推
	bool CanPush(const int &nBox,const int &nDirection)const
	{
		int nTo = this->ce(m_ptBoxs[nBox].De(nDirection) );
		int nFrom = this->ce(m_ptBoxs[nBox].De(5-nDirection));
		return ( nFrom == MAN || nFrom == MAN1 ) && ( nTo == MAN || nTo == EMPTY);

	};
	//通过推动箱子得到新的布局
	void Push(CMapT & Map,const int &nBox,const int &nDirection)const
	{
		//首先，先设置好地形（SOLID与EMPTY）
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
				case MAN1:
				case EMPTY1:
					Map.e( 1+i,1+j) = EMPTY1;
					break;
				case SOLID:
					Map.e(1+i,1+j) = SOLID;
					break;
				
				}
			}
		}
		//计算箱子的位置
		for( int i = 0; i<N;++i)
		{
			Map.m_ptBoxs[i] = this->m_ptBoxs[i];
		}
		Map.m_ptBoxs[nBox] = this->m_ptBoxs[nBox].De(nDirection);
		//将箱子放入地图中
		for( int i = 0; i<N;++i)
		{
			Map.e( Map.m_ptBoxs[i] ) = BOX;
		}
		//指定人所站的位置
		Map.m_ptStart = this->m_ptBoxs[nBox];
		Map.Exploit();
		//新生成的地图，其状态自然是0，从没有试推过
		Map.m_nStatus = 0;

	};
    //枚举一个地图所有可能的推法，共4*N种，如
	//果推法可行，则推动箱子，得到新的地图
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
	//利用传入的变量，初始化一个地图
	//Xs[]表示地形，ptBoxs[]表示地图的位置，ptStart表示人所站的位置
	void Create( const int Xs[],const CPoint ptBoxs[],const CPoint ptStart)
	{
		//设置地形
		for( int i = 0;i<R;++i)
		{
			for( int j = 0; j<L; ++j)
			{
				this->e(1+i,1+j) = Xs[i*L + j];
			}
		}
		//放置箱子
		for( int i=0; i<N; ++i)
		{
			m_ptBoxs[i] = ptBoxs[i];
			this->e( m_ptBoxs[i] ) = BOX;
		}
		//指定人所站的位置
		m_ptStart = ptStart;
		m_nStatus = 0;
		this->Exploit();				//将人可走到的空白标识为人，方便判断
	};
	friend bool operator == ( const CMapT & Map1,const CMapT & Map2)
	{
		//判断两个地图是否相等，只要两个地图中箱子位置是一样的，
		//并且Map1中人所站的位置是Map2中2可以走到的位置中的一个，那么
		//这两个地图就可以相等了，用不着二维数组中每个数都一样。
		for( int i=0;i<N;++i)
		{
			if( Map2.ce( Map1.m_ptBoxs[i] ) != BOX )
				return false;
		}

		int nSize1 = Map1.m_vPath.size();
		int nSize2 = Map2.m_vPath.size();

		if( nSize1 < nSize2)
		{
			for( int i = 0;i<nSize1;++i)
			{
				if( Map2.m_ptStart == Map1.m_vPath.at(i) )
					return true;
			}
		}
		else
		{
			for( int i = 0;i<nSize2;++i)
			{
				if( Map1.m_ptStart == Map2.m_vPath.at(i) )
					return true;
			}
		}

		return false;
	}

	friend bool operator != ( const CMapT & Map1,const CMapT & Map2)
	{
		return !(Map1 == Map2);

	}


	//将地图输出到流
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
	//根据当前的状态得到最近一次的推法
	CPushCase PushCase()const
	{
		int nBox = (m_nStatus-1)/4;
		int nDirection = (m_nStatus-1)%4 + 1;
		return CPushCase( m_ptBoxs[nBox],nDirection);

	};







	
};
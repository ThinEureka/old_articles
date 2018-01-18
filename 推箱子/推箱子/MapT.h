//MapT.h
//��ͼ��ͷ�ļ�

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


//�������࣬Ϊ�˷����ʾ���������ڵ�һ��λ��
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
	//�õ�����������������ꡣ
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
	//�������
	friend std::ostream & operator << ( std::ostream & os,const CPoint &Point)
	{
		os<<Point.ceX()<<" "<<Point.ceY();
		return os;
	};
	//����벻�ȵ�����
	friend bool operator == ( const CPoint & Point1,const CPoint & Point2)
	{
		return Point1.ceX() == Point2.ceX() && Point1.ceY() == Point2.ceY();
	};
	friend bool operator != ( const CPoint & Point1,const CPoint & Point2)
	{
		return Point1.ceX() != Point2.ceX() || Point1.ceY() != Point2.ceY();
	}
};

//��ʾĳһ�ֵ��Ʒ��Ľṹ
struct CPushCase
{
	CPoint m_ptLocation;
	int m_nDirection;
	CPushCase(CPoint ptLocation,int nDirection):m_ptLocation(ptLocation),m_nDirection(nDirection)
	{};
};

//��ʾһ����Ϸ״̬������˵��ͼ��
template<int R,int L,int N>
class CMapT
{
private:
	int m_ppnSpace[R][L];			//��ʾ��Ϸ���ֵĶ�ά����
	CPoint m_ptBoxs[N];				//��ʾ��ǰ��ͼ�����ӵ�λ��
	CPoint m_ptStart;				//����վ��λ��
	std::vector<CPoint> m_vPath;	//һ����ʱ���������ڱ�ʶMAN
	int m_nStatus;					//��ʾ��ǰ�����ƹ���״̬��
public:
	int m_nPreID;					//��ǰ��ͼ�����ε�ͼ��ID
//	int m_nID;
public:
	CMapT()
	{};
	~CMapT()
	{};
public:
	//��ȡ���������������
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
	//Ϊ���ж����Ƿ����������ӵķ�������ñ�ʶ����
	//������������������Pickֱ������ʶ
	void Pick(const CPoint Point)
	{
		//���������EMPTY��EMPTY1�������ʶΪMAN��MAN1
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
	//��ʶ���ڵ��ĵ�
	void Judge(const CPoint Point)
	{
		Pick( Point.Left() );
		Pick( Point.Right() );
		Pick( Point.Up() );
		Pick( Point.Down() );
	};
	//���ô˺�������ʵ��MAN����Ч�ı�ʶ
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
	//�ж�ĳһ��������ָ���������Ƿ����
	bool CanPush(const int &nBox,const int &nDirection)const
	{
		int nTo = this->ce(m_ptBoxs[nBox].De(nDirection) );
		int nFrom = this->ce(m_ptBoxs[nBox].De(5-nDirection));
		return ( nFrom == MAN || nFrom == MAN1 ) && ( nTo == MAN || nTo == EMPTY);

	};
	//ͨ���ƶ����ӵõ��µĲ���
	void Push(CMapT & Map,const int &nBox,const int &nDirection)const
	{
		//���ȣ������úõ��Σ�SOLID��EMPTY��
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
		//�������ӵ�λ��
		for( int i = 0; i<N;++i)
		{
			Map.m_ptBoxs[i] = this->m_ptBoxs[i];
		}
		Map.m_ptBoxs[nBox] = this->m_ptBoxs[nBox].De(nDirection);
		//�����ӷ����ͼ��
		for( int i = 0; i<N;++i)
		{
			Map.e( Map.m_ptBoxs[i] ) = BOX;
		}
		//ָ������վ��λ��
		Map.m_ptStart = this->m_ptBoxs[nBox];
		Map.Exploit();
		//�����ɵĵ�ͼ����״̬��Ȼ��0����û�����ƹ�
		Map.m_nStatus = 0;

	};
    //ö��һ����ͼ���п��ܵ��Ʒ�����4*N�֣���
	//���Ʒ����У����ƶ����ӣ��õ��µĵ�ͼ
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
	//���ô���ı�������ʼ��һ����ͼ
	//Xs[]��ʾ���Σ�ptBoxs[]��ʾ��ͼ��λ�ã�ptStart��ʾ����վ��λ��
	void Create( const int Xs[],const CPoint ptBoxs[],const CPoint ptStart)
	{
		//���õ���
		for( int i = 0;i<R;++i)
		{
			for( int j = 0; j<L; ++j)
			{
				this->e(1+i,1+j) = Xs[i*L + j];
			}
		}
		//��������
		for( int i=0; i<N; ++i)
		{
			m_ptBoxs[i] = ptBoxs[i];
			this->e( m_ptBoxs[i] ) = BOX;
		}
		//ָ������վ��λ��
		m_ptStart = ptStart;
		m_nStatus = 0;
		this->Exploit();				//���˿��ߵ��Ŀհױ�ʶΪ�ˣ������ж�
	};
	friend bool operator == ( const CMapT & Map1,const CMapT & Map2)
	{
		//�ж�������ͼ�Ƿ���ȣ�ֻҪ������ͼ������λ����һ���ģ�
		//����Map1������վ��λ����Map2��2�����ߵ���λ���е�һ������ô
		//��������ͼ�Ϳ�������ˣ��ò��Ŷ�ά������ÿ������һ����
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


	//����ͼ�������
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
	//���ݵ�ǰ��״̬�õ����һ�ε��Ʒ�
	CPushCase PushCase()const
	{
		int nBox = (m_nStatus-1)/4;
		int nDirection = (m_nStatus-1)%4 + 1;
		return CPushCase( m_ptBoxs[nBox],nDirection);

	};







	
};
//BoxMazeT.h
//�����ѽ��õ�������������������⡣

#pragma once
#include "MapT.h"

const int MAX = 100000;


//��װ����������Ϸ����ͼ�����������

template<int R,int L,int N>
class CBoxMazeT
{
private:
	CMapT<R,L,N> * m_poMap;
	CPoint m_ptAims[N];
	int m_nEndID;
	std::vector< CPushCase> m_vCase;

public:
	CBoxMazeT():m_poMap(NULL)
	{};
	void Clear()
	{
		if( m_poMap != NULL)
		{
			delete []m_poMap;
			m_poMap = NULL;
		}
	};
	~CBoxMazeT()
	{
		Clear();
	};
private:
	void Construct()
	{
		Clear();
		m_poMap = new CMapT<R,L,N>[MAX];

	};
public:
	//��ʼ������
	void Create( const int Xs[],const CPoint ptBoxs[],const CPoint ptStart,const CPoint ptAims[])
	{
		Construct();
		m_nEndID = 0;
		m_poMap[0].Create(Xs,ptBoxs,ptStart);
		m_poMap[0].m_nPreID = -1;
		for( int i = 0;i<N;++i)
		{
			m_ptAims[i] = ptAims[i];
		}
	};
public:
	bool CanExit(const CMapT<R,L,N> & Map)const
	{
		for( int i=0;i<N;++i)
		{
			if( Map.ce(m_ptAims[i]) != BOX )
				return false;
		}
		return true;
	};
	bool Contradict(const int &nID,const CMapT<R,L,N> &Map)const
	{
		for( int i=0; i<nID+1;++i)
		{
			if( m_poMap[nID-i] == Map )
				return true;
		}
		return false;
	};
	bool Try()
	{
		int nID = 0;
		int nStatus;
		int nTotalStatus = 4*N;
		while( nID >= 0 && m_nEndID<MAX)				//û���
		{	
			if( m_nEndID%100==0)
				std::cout<<m_nEndID<<std::endl;			//��ʾ�����Ľ���
			if( m_poMap[nID].TryPush( m_poMap[m_nEndID+1],&nStatus) )
			{
				
				if( CanExit( m_poMap[m_nEndID+1] ) )	//����ѵ�����ֹ״̬
				{
					m_poMap[m_nEndID+1].m_nPreID = nID;
					++m_nEndID;
//					std::cout<<"$$"<<m_nEndID<<std::endl;
					return true;
				}
				else
				{
					if( Contradict(m_nEndID,m_poMap[m_nEndID+1] ) )//����µĵ�ͼ���Ѵ��ڵ��ظ���
					{
//						std::cout<<"**"<<std::endl;
						if( nStatus < nTotalStatus)
						{
							continue;
						}
						else
						{
							nID = m_poMap[nID].m_nPreID;
							continue;
						}
					}
					else
					{
						//����õ����µĵ�ͼ
//						m_poMap[m_nEndID+1].Exploit();
						m_poMap[m_nEndID+1].m_nPreID = nID;
						nID = ++ m_nEndID;
						continue;
					}
				}
			}
			else       //��ǰ���Ƶ��Ʒ�����Ч��
			{
				
				if( nStatus < nTotalStatus )
					continue;
				else
				{
					//��˵�ǰ��ͼ�������Ʒ����������Ľ����Ƶ�����
					nID = m_poMap[nID].m_nPreID;
					continue;
				}
			}
		}
		return false;
	};
public:
	//���������Ľ��������Ʒ�����
	void Analyze()
	{
		int nID = m_nEndID;
		while( nID >= 0)
		{
			m_vCase.push_back( m_poMap[nID].PushCase() );
			nID = m_poMap[nID].m_nPreID;
//			std::cout<<m_poMap[nID]<<std::endl;
			
		}
		
	};
	//���������������ʵ��ĸ�ʽ���������Ʒ�����
	void Print( std::ostream& os,const int &nFlag = 1)
	{
		int nSize = Maze.m_vCase.size();
		
		for( int i=0; i<nSize;++i)
		{
			if( nFlag == 2)
				os<<"��"<<1+i<<"��: ";

			os<<Maze.m_vCase.at(nSize-1-i).m_ptLocation;
			if( nFlag == 1)
			{
				os<<"\t"<<Maze.m_vCase.at(nSize-1-i).m_nDirection;
			}
			else
			{
				switch( Maze.m_vCase.at(nSize-1-i).m_nDirection )
				{
				case 1:
					os<<"����";
					break;
				case 2:
					os<<"����";
					break;
				case 3:
					os<<"����";
					break;
				case 4:
					os<<"����";
					break;

				}
			}
			os<<std::endl;
		}		
	}
	friend std::ostream & operator << ( std::ostream & os, const CBoxMazeT<R,L,N> & Maze)
	{

		int nSize = Maze.m_vCase.size();
		
		for( int i=0; i<nSize;++i)
		{
			os<<Maze.m_vCase.at(nSize-1-i).m_ptLocation;
			os<<"\t"<<Maze.m_vCase.at(nSize-1-i).m_nDirection;
			os<<std::endl;
		}
		return os;
		
	};

};
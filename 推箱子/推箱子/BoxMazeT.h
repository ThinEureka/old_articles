//BoxMazeT.h
//利用已建好的描述来求解推箱子问题。

#pragma once
#include "MapT.h"

const int MAX = 100000;


//封装了推箱子游戏有向图的深度搜索法

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
	//初始化工作
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
		while( nID >= 0 && m_nEndID<MAX)				//没有穷尽
		{	
			if( m_nEndID%100==0)
				std::cout<<m_nEndID<<std::endl;			//显示搜索的进度
			if( m_poMap[nID].TryPush( m_poMap[m_nEndID+1],&nStatus) )
			{
				
				if( CanExit( m_poMap[m_nEndID+1] ) )	//如果搜到了终止状态
				{
					m_poMap[m_nEndID+1].m_nPreID = nID;
					++m_nEndID;
//					std::cout<<"$$"<<m_nEndID<<std::endl;
					return true;
				}
				else
				{
					if( Contradict(m_nEndID,m_poMap[m_nEndID+1] ) )//如果新的地图与已存在的重复了
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
						//如果得到了新的地图
//						m_poMap[m_nEndID+1].Exploit();
						m_poMap[m_nEndID+1].m_nPreID = nID;
						nID = ++ m_nEndID;
						continue;
					}
				}
			}
			else       //当前试推的推法是无效的
			{
				
				if( nStatus < nTotalStatus )
					continue;
				else
				{
					//穷尽了当前地图的所有推法，将搜索的焦点移到上游
					nID = m_poMap[nID].m_nPreID;
					continue;
				}
			}
		}
		return false;
	};
public:
	//根据搜索的结果整理好推法序列
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
	//下面两个函数以适当的格式输出解题的推法序列
	void Print( std::ostream& os,const int &nFlag = 1)
	{
		int nSize = Maze.m_vCase.size();
		
		for( int i=0; i<nSize;++i)
		{
			if( nFlag == 2)
				os<<"第"<<1+i<<"步: ";

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
					os<<"向左";
					break;
				case 2:
					os<<"向上";
					break;
				case 3:
					os<<"向下";
					break;
				case 4:
					os<<"向右";
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
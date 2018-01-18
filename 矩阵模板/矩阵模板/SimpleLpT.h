//SimpleLpT.h
//CSimpleLpT<T>的头文件
//Lp指的是线性规划，SimpleLp在这里指的是用单纯形法解线性规划。
#pragma once
#include "MTApp.h"

template<class T>
class CSimpleLpT
{
public:
	CMatrixT<T> m_mSource;
	CMatrixT<T> m_mCurrentBase;
	CMatrixT<T> m_mCurrentExBase;
	CMatrixT<T> m_mPerform;
	int m_nRow;
	int m_nLine;
	int m_nRotateTimes;
	int* m_pnBaseIndexs;
	int* m_pnTopIndexs;
	bool* m_pbIsInBase;
public:
	CSimpleLpT<T>():m_pbIsInBase(NULL),m_nRotateTimes(0)
	{};
private:
	void Clear();
	void Construct();
public:
	~CSimpleLpT<T>()
	{
		Clear();
	};
private:
	bool & eLineFlag(const int & nLine)
	{
		return m_pbIsInBase[nLine-1];
	};
public:
	void SetConstrain(const T* eArray,const int &nRow,const int &nLine);
	void SetAimMin(const T* eArray);

public:
	void IndicateBase( const int  pnBaseIndex[]);
private:
	void UpdateIndexs();

public:
	void UpdateBase();
	void UpdateExBase();
	void NewPerform()
	{
		m_mPerform = ~m_mCurrentExBase * m_mSource;
	};
	void FirstPerform();
	void Rotate(const int &nRow,const int &nLineIndex);
public:
	int Judge(int *pnRow,int *pnLineIndex);
	int SearchMinLine();
	int SearchDesiredRow(const int &nLineIndex);
public:
	int Perform(CMatrixT<T> &mSolution,const int &nMethod = 1,const int &nTimes = 100);
};
template<class T>
void CSimpleLpT<T>::Clear()
{
	if( m_pbIsInBase!=NULL)
	{
		delete[] m_pbIsInBase;
		delete[] m_pnBaseIndexs;
		delete[] m_pnTopIndexs;
		m_pbIsInBase = NULL;
	}

}

template<class T>
void CSimpleLpT<T>::Construct()
{
	Clear();
	m_nRotateTimes = 0;
	m_pbIsInBase = new bool[m_nLine];
	m_pnBaseIndexs = new int[m_nRow];
	m_pnTopIndexs = new int[m_nLine-m_nRow];
	
}

template<class T>
void CSimpleLpT<T>::SetConstrain(const T* eArray,const int &nRow,const int &nLine)
{
	
	m_nRow = nRow;
	m_nLine = nLine;
	this->Construct();
	m_mSource.Construct(nRow+1,nLine+1);
	for( int i=0;i<m_nRow;++i)
	{
		for( int j=0;j<m_nLine+1;++j)
		{
			m_mSource.e( 1+i,1+j) = *(eArray++);
		}
	}
	m_mCurrentBase.Construct(nRow,nRow);
	m_mCurrentExBase.Construct(nRow+1,nRow+1);
	
	
}


template<class T>
void CSimpleLpT<T>::UpdateBase()
{
	for( int i=0;i<m_nRow;++i)
	{
		for( int j=0; j<m_nRow;++j)
		{
			m_mCurrentBase.e( 1+j,1+i) = m_mSource.e( 1+j,m_pnBaseIndexs[i]);
		}
	}
}

template<class T>
void CSimpleLpT<T>::UpdateExBase()
{
	for( int i=0;i<m_nRow;++i)
	{
		for( int j=0; j<m_nRow+1;++j)
		{
			m_mCurrentExBase.e( 1+j,1+i) = m_mSource.e( 1+j,m_pnBaseIndexs[i]);
		    m_mCurrentExBase.e( 1+i,m_nRow+1) = 0;


		}
	}
	m_mCurrentExBase.e(m_nRow+1,m_nRow+1) = 1;
	
}


template<class T>
void CSimpleLpT<T>::SetAimMin(const T* eArray)
{
	for( int i=0;i<m_nLine;++i)
	{
		m_mSource.e(m_nRow+1,1+i) = eArray[i];
	}
	m_mSource.e(m_nRow+1,m_nLine+1) = 0;
}

template<class T>
void CSimpleLpT<T>::IndicateBase( const int  pnBaseIndex[])
{
	for( int i=0;i<m_nLine;++i)
	{
		eLineFlag(1+i) = false;
	}
	for( int i=0;i<m_nRow;++i)
	{
		eLineFlag( pnBaseIndex[i] ) = true;
	}

}


template<class T>
void CSimpleLpT<T>::UpdateIndexs()
{
	int nBase = 0;
	int nTop =0;
	for( int i=0;i<m_nLine;++i)
	{
		if( eLineFlag(1+i) )
		{
			m_pnBaseIndexs[nBase++] = 1+i;
		}
		else
		{
			m_pnTopIndexs[nTop++] = 1+i;
		}
	}
}

template<class T>
int CSimpleLpT<T>::Judge(int *pnRow,int *pnLineIndex)
{
	*pnLineIndex = SearchMinLine();
	
	//*pnLine == 0时，有一个判断数为零，有无穷多解
	if( *pnLineIndex == -2)
	{
		return 0;
	}
	//*pnLine == -1时，所有判断数都大于零，有一个最优解
	else if( *pnLineIndex == -1)
	{
		return 1;
	}
	//*pnLine > 0 时，有判断数小于零，迭代仍需进行
	else
	{
		*pnRow = SearchDesiredRow(*pnLineIndex);
		//当*pnRow==0时,存在着某一方向，沿方向知，Lp无下界
		if( *pnRow == 0)
			return -2;
		//*pnRow > 0时，可转到下一基可行解
		else
			return -1;

	}
}


template<class T>
int CSimpleLpT<T>::SearchMinLine()
{
	T tMin = m_mPerform.e(m_nRow+1,m_pnTopIndexs[0]);
	int nMinLine = 0;
	for( int i=0;i<m_nLine - m_nRow - 1;++i)
	{
		T tNew = m_mPerform.e(m_nRow+1,m_pnTopIndexs[1+i]);
		if( tNew < tMin)
		{
			tMin = tNew;
			nMinLine = 1+i;
		}
	}

	if( tMin == 0)
		return -2;
	else if(tMin > 0)
		return -1;
	else
		return nMinLine;
}


template<class T>
int CSimpleLpT<T>::SearchDesiredRow(const int &nLineIndex)
{
	T tDesired = 0;
	int nDesiredRow = 0;
	int nLine = m_pnTopIndexs[nLineIndex];
	for( int i=0;i<m_nRow;++i)
	{
		if( m_mPerform.e(1+i,nLine) > 0)
		{
			if( m_mPerform.e(1+i,m_nLine+1) == 0)
				return 1+i;
			T tTmp = m_mPerform.e(1+i,m_nLine+1)/m_mPerform.e(1+i,nLine);
			if( tTmp<tDesired&&tDesired>0 || nDesiredRow == 0)
			{
				tDesired = tTmp;
				nDesiredRow = 1+i;
			}
		} 
	}

	return nDesiredRow;
	


}

template<class T>
void CSimpleLpT<T>::Rotate(const int &nRow, const int &nLineIndex)
{
	int nNewLine = m_pnTopIndexs[nLineIndex];
	int nOldLine = m_pnBaseIndexs[nRow-1];
	this->eLineFlag(nNewLine) = true;
	this->eLineFlag( nOldLine) = false;
	m_pnBaseIndexs[nRow-1] = nNewLine;
	m_pnTopIndexs[nLineIndex] = nOldLine;
	::Rotate(m_mPerform,nRow,nNewLine);
	++m_nRotateTimes;
//	m_mPerform.Print();
//	cout<<endl;

}


template<class T>
int CSimpleLpT<T>::Perform(CMatrixT<T> &mSolution, const int &nMethod, const int &nTimes)
{
	switch( nMethod)
	{
	case 1:
		FirstPerform();
		int nRow,nLineIndex,nStatus = 0;
		int nFlag = Judge(&nRow,&nLineIndex);
		while( nFlag == -1 && m_nRotateTimes<nTimes)
		{
			Rotate(nRow,nLineIndex);
			nFlag = Judge(&nRow,&nLineIndex);
		}
		return nFlag;
	}
}

template<class T>
void CSimpleLpT<T>::FirstPerform()
{
	UpdateIndexs();
	UpdateExBase();
	NewPerform();

}
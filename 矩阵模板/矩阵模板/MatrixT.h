//MatrixT.h
//模板类CMatrixT< class T>的头文件


#pragma once
#include <iostream>
//#include "Fraction.h"
using namespace std;

template< class T>
class CVectorT;
template< class T>
class CMatrixT;

static const char ROW_INTERCHANGE = (char)0x01;
static const char ROW_MULTIPLY = (char)0x02;
static const char ROW_PLUS = (char)0x04;
static const char ROW_CHANGE = ROW_INTERCHANGE | ROW_MULTIPLY | ROW_PLUS;

static const char ALL_CHANGE = (char)0xff;
	
static const char LINE_INTERCHANGE = (char)0x10;
static const char LINE_MULTIPLY = (char)0x20;
static const char LINE_PLUS = (char)0x40;
static const char LINE_CHANGE = LINE_INTERCHANGE | LINE_MULTIPLY | LINE_PLUS;

static const char INTERCHANGE = ROW_INTERCHANGE | LINE_INTERCHANGE;
static const char MULTIPLY = ROW_MULTIPLY | LINE_MULTIPLY;
static const char PLUS = ROW_PLUS | LINE_PLUS;



template< class T>
struct CChangeCmdT
{
	

	char m_cStyle;
	int m_nDest;
	int m_nSrc;
	T m_tTimes;
    CChangeCmdT<T> operator ~()const
	{
		CChangeCmdT<T> rCmd = *this;

		switch( m_cStyle)
		{
		case ::ROW_INTERCHANGE:
		case ::LINE_INTERCHANGE:
			break;
		case ::ROW_MULTIPLY:
		case ::LINE_MULTIPLY:
			rCmd.m_tTimes = 1/m_tTimes;
			break;
		case ::ROW_PLUS:
		case ::LINE_PLUS:
			rCmd.m_tTimes = -m_tTimes;
			break;

		}
		return rCmd;
	};
	 CChangeCmdT<T> operator !()const
	{
		CChangeCmdT<T> rCmd = *this;

		switch( m_cStyle)
		{
		case ::ROW_INTERCHANGE:
			rCmd.m_cStyle = LINE_INTERCHANGE;
			break;
		case ::LINE_INTERCHANGE:
			rCmd.m_cStyle = ROW_INTERCHANGE;
			break;
		case ::ROW_MULTIPLY:
			rCmd.m_cStyle = LINE_MULTIPLY;
			break;
		case ::LINE_MULTIPLY:
			rCmd.m_cStyle = ROW_MULTIPLY;
			break;
		case ::ROW_PLUS:
			rCmd.m_cStyle = LINE_PLUS;
			break;
		case ::LINE_PLUS:
			rCmd.m_cStyle = ROW_PLUS;
			break;

		}
		return rCmd;
	};
	void Print()const;
	void Create1( const char &cFlag, const int &nDest, const int &nSrc)
	{		
		m_nDest = nDest;
		m_nSrc = nSrc;
		switch(cFlag)
		{
		case 'R':
		case 'r':
			m_cStyle = ::ROW_INTERCHANGE;
			break;
		case 'L':
		case 'l':
			m_cStyle = ::LINE_INTERCHANGE;
			break;
		}
	};
	void Create2( const T &tTimes, const char &cFlag, const int &nDest)
	{
		m_nDest = nDest;
		m_tTimes = tTimes;
		switch(cFlag)
		{
		case 'R':
		case 'r':
			m_cStyle = ::ROW_MULTIPLY;
			break;
		case 'L':
		case 'l':
			m_cStyle = ::LINE_MULTIPLY;
			break;
		}
	};

	void Create3( const char &cFlag, const int &nDest,const T &tTimes,const int &nSrc)
	{
		m_nDest = nDest;
		m_tTimes = tTimes;
		m_nSrc = nSrc;
		switch(cFlag)
		{
		case 'R':
		case 'r':
			m_cStyle = ::ROW_PLUS;
			break;
		case 'L':
		case 'l':
			m_cStyle = ::LINE_PLUS;
			break;
		}
	};

};


template< class T>
void CChangeCmdT<T>::Print()const
{
	switch( m_cStyle)
	{

	case ::ROW_INTERCHANGE:
		cout<<'R'<<m_nDest<<','<<m_nSrc;
		break;
	case ::ROW_MULTIPLY:
		cout<<m_tTimes<<" "<<'R'<<m_nDest;
		break;
	case ::ROW_PLUS:
		if( m_tTimes < 0)
			cout<<'R'<<m_nDest<<" "<<m_tTimes<<" "<<'R'<<m_nSrc;
		else
			cout<<'R'<<m_nDest<<" "<<'+'<<" "<<m_tTimes<<" "<<'R'<<m_nSrc;
		break;


	case ::LINE_INTERCHANGE:
		cout<<'L'<<m_nDest<<','<<m_nSrc;
		break;
	case ::LINE_MULTIPLY:
		cout<<m_tTimes<<" "<<'L'<<m_nDest;
		break;
	case ::LINE_PLUS:
		if( m_tTimes < 0)
			cout<<'L'<<m_nDest<<" "<<m_tTimes<<" "<<'L'<<m_nSrc;
		else
			cout<<'L'<<m_nDest<<" "<<'+'<<" "<<m_tTimes<<" "<<'L'<<m_nSrc;
		break;
	}

}

template<class T>
struct CCmdBlockT
{
	CCmdBlockT():m_pPreBlock(NULL),m_pNextBlock(NULL)
	{};
	static const int block_size = 5;
	CChangeCmdT<T> m_CmdArray[block_size];
	CCmdBlockT<T> * m_pPreBlock;
	CCmdBlockT<T> * m_pNextBlock;
private:
	CCmdBlockT( const CCmdBlockT &);
};

template<class T>
class CCmdSeriesT
{
private:
	CCmdBlockT<T> m_FirstBlock;
	CCmdBlockT<T> * m_pLastBlock;
	mutable CCmdBlockT<T> * m_pCurrentBlock;
	mutable int m_nCurrentIndex;
private:
	int m_nCmdNum;
public:
	int GetCommandNum()const
	{
		return m_nCmdNum;
	};

public:
	int m_nRank;
public:
	int m_nRankPosition;
	int m_nSwordPosition;
	int m_nCrossPosition;
	int m_nStandardPosition;

//	int m_nBlockNum;

public:
	void Initial()
	{
		m_nCmdNum = 0;
//	    m_nBlockNum = 1;

		m_nRank = 0;
		m_nRankPosition = 0;
		m_nSwordPosition = 0;
		m_nCrossPosition = 0;
		m_nStandardPosition = 0;
		m_FirstBlock.m_pNextBlock = NULL;

		m_pLastBlock = &m_FirstBlock;

		m_pCurrentBlock = &m_FirstBlock;
		m_nCurrentIndex = 0;
	};
public:
	CCmdSeriesT()
	{
		Initial();
	};
private:
	CCmdSeriesT( const CCmdSeriesT &);
public:
	~CCmdSeriesT()
	{
		Clear();
	};
public:
	void Clear();
private:
	void AddNewBlock();
public:
	bool AddNewCmd( const CChangeCmdT<T> &NewCmd,const bool &bBeAdded = true);


public:
	int GetCurrentPosition()const
	{
		
		CCmdBlockT<T> * pBlock = this->m_pCurrentBlock;
		int nSize = CCmdBlockT<T>::block_size;
		int nBlock = 1;
	
		while( pBlock->m_pPreBlock != NULL)
		{
			nBlock++;
			pBlock = pBlock->m_pPreBlock;
		}
		
		return ( nBlock -1)*nSize  + m_nCurrentIndex + 1;


	};

public:
	bool MovePointerForward()const;
	bool MovePointerBackward()const;
	bool MovePointerToHead()const
	{
		m_pCurrentBlock = (CCmdBlockT<T> *)&m_FirstBlock;
		m_nCurrentIndex = 0;
		if( m_nCmdNum == 0)
			return false;
		else
			return true;
	};
	bool MovePointerToTail()const
	{
		if( m_nCmdNum == 0)
		{
			this->MovePointerToHead();
			return false;
		}
		return this->MovePointerToPosition( m_nCmdNum);
	};
	bool MovePointerToPosition( const int &nPosition)const
	{
		int nSize = CCmdBlockT<T>::block_size;
		int nBlockNum = ( nPosition - 1)/nSize  + 1;
		this->MovePointerToHead();
		int nCount = 1;
		while( nCount < nBlockNum)
		{
			if( m_pCurrentBlock->m_pNextBlock == NULL)
			{
				m_nCurrentIndex = nSize - 1;
				return false;
			}
			else
			{
				m_pCurrentBlock = m_pCurrentBlock->m_pNextBlock;
				nCount++;
			}
		}
		m_nCurrentIndex = nPosition - ( nBlockNum -1) * nSize -1;
		return true;
	};
private:
	void MoveValidPointForward();
private:
	CChangeCmdT<T> &e()
	{
		return m_pCurrentBlock->m_CmdArray[m_nCurrentIndex];
	}
public:
	const CChangeCmdT<T> & e()const
	{
		return m_pCurrentBlock->m_CmdArray[m_nCurrentIndex];
	}
	const CChangeCmdT<T> & GetCurrentCmd()const
	{
		return m_pCurrentBlock->m_CmdArray[m_nCurrentIndex];
	}
public:
	void TurnOpposite();
	bool CreateByReverse( const CCmdSeriesT &Cs,const char &cFlag = 0);
	bool Create( const CCmdSeriesT &Cs,const char &cFlag = ALL_CHANGE);
private:
	bool ShouldBeAdded( const CChangeCmdT<T> &Cmd,const char &cFlag = ALL_CHANGE)const
	{
		return Cmd.m_cStyle & cFlag;

	}


public:
	void PrintCmds()const;
public:
	int CountCmds( const  char &cFlag = ALL_CHANGE)const;
	
	
};
template<class T>
void CCmdSeriesT<T>::Clear()
{
	CCmdBlockT<T> * pBlock1, *pBlock2;
	pBlock1 = m_FirstBlock.m_pNextBlock;
	while( pBlock1 != NULL)
	{
		pBlock2 = pBlock1;
		pBlock1 = pBlock1->m_pNextBlock;
		delete pBlock2;
	}
	m_FirstBlock.m_pNextBlock = NULL;


}


template<class T>
void CCmdSeriesT<T>::AddNewBlock()
{
	CCmdBlockT<T> * pNewBlock = new CCmdBlockT<T>;
	m_pLastBlock->m_pNextBlock = pNewBlock;
	pNewBlock->m_pPreBlock = m_pLastBlock;
	m_pLastBlock = pNewBlock;

}



template<class T>
bool CCmdSeriesT<T>::MovePointerForward() const
{
	

	if( m_nCurrentIndex != CCmdBlockT<T>::block_size-1)
	{
		m_nCurrentIndex++;
		return true;
		
	}
	else
	{
		if( m_pCurrentBlock->m_pNextBlock != NULL )
		{
			m_pCurrentBlock = m_pCurrentBlock->m_pNextBlock;
			m_nCurrentIndex = 0;
			return true;
		}
		else
		{
			return false;
		}
		
	}
}

template<class T>
bool CCmdSeriesT<T>::MovePointerBackward() const
{

	int nSize = CCmdBlockT<T>::block_size;
	if( m_nCurrentIndex != 0)
	{
		m_nCurrentIndex--;
		return true;

	}
	else
	{
		if( m_pCurrentBlock->m_pPreBlock != NULL)
		{
			m_pCurrentBlock = m_pCurrentBlock->m_pPreBlock;
			m_nCurrentIndex = nSize - 1;
			return true;
		}
		else
		{
			return false;
		}
	}

}
template<class T>
void CCmdSeriesT<T>::MoveValidPointForward()
{
	
	bool  bSpaceEnough =  MovePointerForward();
	if( bSpaceEnough)
	{
		return;
	}
	else
	{
		AddNewBlock();
		m_pCurrentBlock = m_pLastBlock;
		m_nCurrentIndex = 0;
		return;
		

	}
}

template<class T>
bool CCmdSeriesT<T>::AddNewCmd(const CChangeCmdT<T> &NewCmd,const bool &bBeAdded)
{
	if( bBeAdded)
	{
		e() = NewCmd;
		m_nCmdNum++;
		this->MoveValidPointForward();
		return true;
	}
	else
	{
		return false;
	}
}

template<class T>
void CCmdSeriesT<T>::TurnOpposite()
{
	int nCount = 0;
	this->MovePointerToHead();
	while( nCount < m_nCmdNum)
	{
		e() = !e();
		nCount++;
		this->MovePointerForward();
	}
}


template<class T>
bool CCmdSeriesT<T>::CreateByReverse(const CCmdSeriesT<T> &Cs,const char &cFlag)
{

	bool bBeAdded;
	bool rB = false;
	bool b;
	CChangeCmdT<T> Cmd;

	if( Cs.m_nCmdNum == 0)
	{
		return false;
	}
	else
	{
		this->Clear();
		this->Initial();
		Cs.MovePointerToTail();
		do
		{
			
			bBeAdded = Cs.ShouldBeAdded( Cs.e(), cFlag);
			if( bBeAdded)
			{
				Cmd = ~(Cs.e());
				this->AddNewCmd( Cmd);
				rB = true;
			}
			b = Cs.MovePointerBackward();
		}while( b);

		return rB;
	}
	
		
		

	
	
}

template<class T>
bool CCmdSeriesT<T>::Create(const CCmdSeriesT<T> &Cs,const char &cFlag)
{

	bool bBeAdded;
	bool rB = false;
	int nCount = 0 ;

	if( Cs.m_nCmdNum == 0)
	{
		return false;
	}
	else
	{
		this->Clear();
		this->Initial();
		Cs.MovePointerToHead();

		while( nCount < Cs.m_nCmdNum )
		{

			bBeAdded = Cs.ShouldBeAdded( Cs.e(), cFlag);
			rB = this->AddNewCmd( Cs.e(),bBeAdded) || rB;
			nCount++;
			Cs.MovePointerForward();

		}

		return rB;
	}			
	
}

template<class T>
void CCmdSeriesT<T>::PrintCmds()const
{
	int nCount = 0;
	this->MovePointerToHead();
	while( nCount < m_nCmdNum)
	{
		cout<<++nCount<<": ";
		e().Print();
		cout<<endl;
		this->MovePointerForward();
	}
}


template<class T>
int  CCmdSeriesT<T>::CountCmds(const char &cFlag) const
{
	int rNum = 0;
	this->MovePointerToHead();
	CChangeCmdT<T> Cmd;
	bool b;
	for( int i=0; i<m_nCmdNum;i++)
	{

		b = this->ShouldBeAdded( e(),cFlag);
		if(b)
		{
			rNum++;
		}
		this->MovePointerForward();
		
	}
	return rNum;
	
}



































template< class T>
class CMatrixT
{
protected:
	T* m_pOrigin; 
	int m_nRow;
	int m_nLine;
public:
	T& e( const int &x, const int &y)
	{
		int nPosition = ( x - 1)*m_nLine + y -1;
		return *(m_pOrigin + nPosition);
	};
public:
	T e( const int &x,const int &y)const
	{
		int nPosition = ( x - 1)*m_nLine + y -1;
		return *(m_pOrigin + nPosition);
	};
public:
	T Get( const int &x,const int &y)const
	{
		int nPosition = ( x - 1)*m_nLine + y -1;
		return *(m_pOrigin + nPosition);
	};
	int GetRow()const
	{
		return m_nRow;
	};
	int GetLine()const
	{
		return m_nLine;
	}
	int GetRank()const;
	T GetDeterminant()const;

		
public:
	CMatrixT():m_pOrigin(NULL)
	{};
	CMatrixT( const CMatrixT<T> &Matrix):m_pOrigin(NULL)
	{
		int nRow = Matrix.m_nRow;
		int nLine = Matrix.m_nLine;
		Construct( nRow,nLine);
		int nDimension = nRow*nLine;
		for( int i=0; i<nDimension; i++)
		{
			*( this->m_pOrigin + i) = *( Matrix.m_pOrigin + i);
		}
	};
	CMatrixT( const char &cFlag ,const int & nRow, const int &nLine = -1,const T& tEm = 1):m_pOrigin(NULL)
	{
		int row = nRow;
		int line = nLine;
		if( nLine < 0)
			line = row;
		this->Construct( row,line);
		switch( cFlag)
		{
		case 'E':
		case 'e':
			this->TurnToE();
			break;
		case 'O':
		case 'o':
			this->TurnToO();
			break;
		case 'D':
		case 'd':
			this->TurnToD( tEm);
			break;
		default:
			this->TurnToO();

		}



	}
	CMatrixT( T e_Array[], const int &nRow, const int &nLine):m_pOrigin(NULL)
	{
		Create(e_Array,nRow,nLine);
	};
public:
	virtual ~CMatrixT()
	{
		Clear();
	};
public:
	const CMatrixT<T> & operator = ( const CMatrixT<T>&Matrix );
protected:
	void Clear()
	{
		if( m_pOrigin != NULL)
			delete[] m_pOrigin;
		m_pOrigin = NULL;
	};
public:
	void Construct()
	{
		Clear();
		m_pOrigin = new T[ m_nRow*m_nLine];
	};
	void Construct( const int &nRow, const int &nLine)
	{
		m_nRow = nRow;
		m_nLine = nLine;
		Construct();
	};


public:
	void Create( T e_Array[], const int &nRow, const int &nLine);
	void TurnToE();
	void TurnToO();
	void TurnToD( const T &tEm);
public:
	void RowInterChange( const int &nRow1,const int &nRow2,
		            const int &nFirstLine,const int &nLastLine);
	void RowInterChange( const int &nRow1,const int &nRow2);

	void RowMultiply(  const int &nRow, const T &tTimes,
					   const int &nFirstLine, const int &nLastLine );
	void RowMultiply( const int &nRow, const T & tTimes );

	void RowPlus( const int &nDestRow, const int &nSrcRow,const T &tTimes,
		          const int &nFirstLine, const int &nLastLine);
	void RowPlus( const int &nDestRow, const int &nSrcRow, const T &tTimes);

	void LineInterChange( const int &nLine1,const int &nLine2,
		             const int &nFirstRow,const int &nLastRow);
	void LineInterChange( const int &nLine1,const int &nLine2);

	void LineMultiply( const int &nLine, const T &tTimes,
					   const int &nFirstRow,const int &nLastRow);
	void LineMultiply( const int &nLine, const T &tTimes);

	void LinePlus( const int &nDestLine, const int &nSrcLine, const T &tTimes,
		           const int &nFirstRow,const int &nLastRow);
	void LinePlus( const int &nDestLine, const int &nSrcLine, const T &tTimes);
public:
	void ChangeByCmd( const CChangeCmdT<T> &Cmd );
public:
	void ChangeByCmd( const CChangeCmdT<T> &Cmd,const int &nFirst,const int &nLast);
public:
	void ChangeByCS( const CCmdSeriesT<T> &CS, const int &nFirst = 1,const int &nLast = -1);

public:
	CMatrixT<T> TakeOut( const int &nFirstRow, const int &nFirstLine,
		                 const int &nRowNum,const int &nLineNum )const;
	void TakeIn( const CMatrixT<T> &SrcMatrix, const int &nFirstRow = 1,const int &nFirstLine = 1);
	
	CMatrixT<T> Unite( const CMatrixT<T> &SrcMatrix, const int nFlag = 1)const;
	

	

	
public:
	void Print()const;


public:
	const CMatrixT<T> & operator += ( const CMatrixT<T> &Matrix);
	const CMatrixT<T> & operator -= ( const CMatrixT<T> &Matrix);
	const CMatrixT<T> & operator *= ( const CMatrixT<T> &Matrix);
	const CMatrixT<T> & operator *= ( const T &k);
    
	CMatrixT<T> operator ! ()const;
	CMatrixT<T> operator ~()const;
	CMatrixT<T> operator -()const;
	bool operator == ( const CMatrixT<T> &Matrix )const;
	bool operator != ( const CMatrixT<T> &Matrix )const;

public:
	void BasicLineSimplify( CCmdSeriesT<T> *pCS , const int &nX, const int &nY);
	void ChangeToStairsF( CCmdSeriesT<T> *pCS, const int &nBorderLine = -1);
	void StairsToSwords( CCmdSeriesT<T> *pCS);
	void SwordsToCross( CCmdSeriesT<T> *pCS);
	void CrossToStandard( CCmdSeriesT<T> *pCS);
	void ChangeToStandardF(CCmdSeriesT<T> *pCS);
	void GetCS( CCmdSeriesT<T> *pCS )const;
	void SolveLinearEqs( CMatrixT<T> SolutionMs[],bool bResults[],const int &nEqNum = 1);
	bool SolveLinearEq( CMatrixT<T> &SolutionMatrix )const;
	bool SolveLinearEq(const CVectorT<T> *pVector,CMatrixT<T> &SolutionMatrix)const;
	void Simplify( CMatrixT<T> &Matrix, 
					const int &nBorderLine = -1,
					const int &nFlag = 1)const;

public:
	int FindSolidRow( const int &nX, const int &nY)const;


//public:
//	const CMatrixT<T> & operator += ( const CMatrix<T> &Matrix );


//	friend const CMatrixT<T> operator + ( const CMatrixT<T>&Matrix1,const CMatrixT<T>&Matrix2);
//	friend const CMatrixT<T> operator - ( const CMatrixT<T>&Matrix1,const CMatrixT<T>&Matrix2);

};
template< class T>
const CMatrixT<T> operator *( T &k, const CMatrixT<T> &Matrixconst);

template< class T>
inline const CMatrixT<T> operator *(const CMatrixT<T> &Matrix,const T &k);

template< class T>
const CMatrixT<T> operator *(const CMatrixT<T> &Matrix,const int &k);

template< class T>
const CMatrixT<T> operator *(const int &k,const CMatrixT<T> &Matrix);

template< class T>
inline const CMatrixT<T>  operator *(const CMatrixT<T> &Matrix1,const CMatrixT<T> &Matrix2);

template< class T>
CMatrixT<T> operator | ( const CMatrixT<T> &Matrix1,const CMatrixT<T> &Matrix2);

template< class T>
CMatrixT<T> operator || ( const CMatrixT<T> &Matrix1,const CMatrixT<T> &Matrix2);

template< class T>
void CMatrixT<T>::Create(T e_Array[], const int &nRow, const int &nLine)
{
	Construct( nRow,nLine);
	int nDimension = m_nRow*m_nLine;
	for( int i = 0; i<nDimension; i++)
	{
		*( m_pOrigin + i) = *( e_Array + i);
	}
}





template< class T>
void CMatrixT<T>::Print() const
{
	for( int i = 0; i<m_nRow; i++)
	{
		for( int j=0; j<m_nLine; j++)
		{
			cout<<e(1+i,1+j)<<'\t';
		}
		cout<<endl;
	}
	
}


template< class T>
const CMatrixT<T> & CMatrixT<T>::operator =(const CMatrixT<T> &Matrix)
{
	if( this != &Matrix)
	{
		int nRow = Matrix.m_nRow;
		int nLine = Matrix.m_nLine;
		Construct( nRow,nLine);
		int nDimension = nRow*nLine;
		for( int i=0; i<nDimension; i++)
		{
			*( this->m_pOrigin + i) = *( Matrix.m_pOrigin + i);
		}
	}
	return *this;


}

template< class T>
inline const CMatrixT<T>  operator +(const CMatrixT<T> &Matrix1,const CMatrixT<T> &Matrix2)
{
	CMatrixT<T> rMatrix = Matrix1;
	return rMatrix += Matrix2;
}


template< class T>
inline const CMatrixT<T> operator -(const CMatrixT<T> &Matrix1,const CMatrixT<T> &Matrix2)
{
	CMatrixT<T> rMatrix = Matrix1;
	return rMatrix -= Matrix2;
}



template< class T>
inline const CMatrixT<T> operator *(const CMatrixT<T> &Matrix,const T &k)
{
	CMatrixT<T> rMatrix = Matrix;
	return rMatrix *= k;
}

template< class T>
inline const CMatrixT<T> operator *(const T &k, const CMatrixT<T> &Matrix)
{
	CMatrixT<T> rMatrix = Matrix;
	return rMatrix *= k;
}


template< class T>
inline const CMatrixT<T> operator *(const CMatrixT<T> &Matrix,const int &k)
{
	CMatrixT<T> rMatrix = Matrix;
	return rMatrix *= T(k);
}

template< class T>
inline const CMatrixT<T> operator *( const int &k,const CMatrixT<T> &Matrix)
{
	CMatrixT<T> rMatrix = Matrix;
	return rMatrix *= T(k);
}






template< class T>
const CMatrixT<T> & CMatrixT<T>::operator +=( const CMatrixT<T> &Matrix )
{
	for( int i = 0; i<m_nRow; i++)
	{
		for( int j = 0; j<m_nLine; j++)
		{
		    this->e( 1+i ,1+j) += Matrix.e( 1+i, 1+j);
		}
	}
	return *this;
}
template< class T>
const CMatrixT<T> & CMatrixT<T>::operator -=( const CMatrixT<T> &Matrix )
{
	for( int i = 0; i<m_nRow; i++)
	{
		for( int j = 0; j<m_nLine; j++)
		{
		    this->e( 1+i ,1+j) -= Matrix.e( 1+i, 1+j);
		}
	}
	return *this;
}





template< class T>
const CMatrixT<T> & CMatrixT<T>::operator *=( const CMatrixT<T> &Matrix )
{
	int row = m_nRow;
	int line = Matrix.m_nLine;
	int dimension = m_nLine;
	CMatrixT<T>  rMatrix;
	rMatrix.Construct( row,line);
	T tSum;


	for( int i = 0; i< row; i++)
	{
		for( int j = 0; j<line; j++)
		{
			tSum = 0;
			for( int k = 0; k< dimension; k++)
				tSum += this->e( 1+i ,1+k) * Matrix.e( 1+k, 1+j);
			rMatrix.e( 1+i,1+j) = tSum;
		}
	}
	*this = rMatrix;
	return *this;
}

template< class T>
inline const CMatrixT<T>  operator *(const CMatrixT<T> &Matrix1,const CMatrixT<T> &Matrix2)
{
	CMatrixT<T> rMatrix(Matrix1);
	return rMatrix *= Matrix2;
}






template< class T>
const CMatrixT<T> & CMatrixT<T>::operator *=(const T &k)
{
	for( int i = 0; i<m_nRow; i++)
	{
		for( int j = 0; j<m_nLine; j++)
		{
		    this->e( 1+i ,1+j) *= k;
		}
	}
	return *this;
}



template< class T>
void CMatrixT<T>::TurnToE()
{
	for( int i = 0; i< m_nRow;i++)
	{
		for( int j=0; j<m_nLine;j++)
		{
			this->e( 1+i,1+j) = 0;
		}
	}
    for( int i = 0; i<m_nRow; i++)
	{
		this->e( 1+i,1+i) = 1;
	}
}

template< class T>
void CMatrixT<T>::TurnToO()
{
	for( int i = 0; i< m_nRow;i++)
	{
		for( int j=0; j<m_nLine;j++)
		{
			this->e( 1+i,1+j) = 0;
		}
	}

}

template< class T>
void CMatrixT<T>::TurnToD(const T &tEm)
{
	for( int i = 0; i< m_nRow;i++)
	{
		for( int j=0; j<m_nLine;j++)
		{
			this->e( 1+i,1+j) = tEm;
		}
	}

}
template< class T>
CMatrixT<T>   CMatrixT<T>::operator -()const
{
	CMatrixT<T> rMatrix;
	rMatrix.Construct( m_nRow,m_nLine);

	for( int i = 0; i<m_nRow; i++)
		for( int j = 0; j<m_nLine; j++)
		{
			rMatrix.e(1+i,1+j) = -e(1+i,1+j);
		}



	return rMatrix;
}

template< class T>
CMatrixT<T>   CMatrixT<T>::operator !()const
{
	CMatrixT<T> rMatrix;
	rMatrix.Construct( m_nLine,m_nRow);

	for( int i = 0; i<m_nRow; i++)
		for( int j = 0; j<m_nLine; j++)
		{
			rMatrix.e( 1+j,1+i) = this->e( 1+i,1+j);
		}



	return rMatrix;
}

template< class T>
bool CMatrixT<T>::operator == ( const CMatrixT<T> &Matrix )const
{
	if( m_nRow != Matrix.m_nRow || m_nLine != Matrix.m_nLine )
		return false;

	for( int i = 0; i<m_nRow; i++)
		for( int j=0; j<m_nLine; j++)
		{
			if( this->e( 1+i, 1+j) != Matrix.e( 1+i,1+j))
				return false;
		}
	return true;
}

template< class T>
inline bool CMatrixT<T>::operator != ( const CMatrixT<T> &Matrix )const
{
	return !( *this == Matrix);
}

template< class T>
void CMatrixT<T>::RowInterChange(const int &nRow1, const int &nRow2,
							const int &nFirstLine, const int &nLastLine)
{
	T tTmp;

	for( int j = 0; j< nLastLine - nFirstLine+1;j++)
	{
		tTmp = e( nRow1, nFirstLine + j);
		e( nRow1, nFirstLine + j) = e( nRow2, nFirstLine + j);
		e( nRow2, nFirstLine + j) = tTmp;
	}

}

template< class T>
void CMatrixT<T>::RowInterChange(const int &nRow1, const int &nRow2)
{
	RowInterChange( nRow1,nRow2,1,m_nLine);
}

template< class T>
void CMatrixT<T>::LineInterChange(const int &nLine1, const int &nLine2,
							const int &nFirstRow, const int &nLastRow)
{
	T tTmp;

	for( int i = 0; i< nLastRow - nFirstRow + 1;i++)
	{
		tTmp = e( nFirstRow + i,nLine1);
		e(  nFirstRow + i,nLine1) = e( nFirstRow + i,nLine2);
		e( nFirstRow + i ,nLine2) = tTmp;
	}

}


template< class T>
inline void CMatrixT<T>::LineInterChange(const int &nLine1, const int &nLine2)
{
	LineInterChange( nLine1,nLine2,1,m_nRow);
	

}


template< class T>
void CMatrixT<T>::RowMultiply(  const int &nRow, const T &tTimes,
							    const int &nFirstLine, const int &nLastLine )
{

	for( int i = 0; i< nLastLine - nFirstLine + 1;i++)
	{
		e( nRow , nFirstLine + i) *= tTimes;
	}

}

template< class T>
inline void CMatrixT<T>::RowMultiply(  const int &nRow, const T &tTimes)
{

	this->RowMultiply( nRow,tTimes,1,m_nLine);

}

template< class T>
void CMatrixT<T>::LineMultiply( const int &nLine, const T &tTimes, 
							    const int &nFirstRow, const int &nLastRow )
{
	for( int i = 0; i< nLastRow - nFirstRow + 1;i++)
	{
		e( nFirstRow + i,nLine) *= tTimes;
	}
}

template< class T>
inline void CMatrixT<T>::LineMultiply(const int &nLine, const T &tTimes)
{
   LineMultiply( nLine, tTimes, 1,m_nRow);
}

template< class T>
void CMatrixT<T>::RowPlus( const int &nDestRow, const int &nSrcRow, const T &tTimes, 
						   const int &nFirstLine, const int &nLastLine )
{
	for( int i = 0; i< nLastLine - nFirstLine + 1; i++)
	{
		e( nDestRow, nFirstLine + i) += tTimes * e( nSrcRow , nFirstLine + i);
	}
}

template< class T>
inline void CMatrixT<T>::RowPlus(const int &nDestRow, const int &nSrcRow, const T &tTimes)
{
	RowPlus( nDestRow,nSrcRow,tTimes, 1,m_nLine);
}

template< class T>
void CMatrixT<T>::LinePlus( const int &nDestLine, const int &nSrcLine,
						    const T &tTimes, const int &nFirstRow, const int &nLastRow )
{
	for( int i = 0; i< nLastRow - nFirstRow + 1; i++)
		e( nFirstRow + i, nDestLine) += tTimes * e( nFirstRow + i, nSrcLine);
}

template< class T>
inline void CMatrixT<T>::LinePlus(const int &nDestLine, const int &nSrcLine, const T &tTimes)
{
	LinePlus( nDestLine, nSrcLine ,tTimes, 1, m_nRow);
}

template< class T>
CMatrixT<T> CMatrixT<T>::TakeOut(const int &nFirstRow, const int &nFirstLine, 
								 const int &nRowNum, const int &nLineNum) const
{
	CMatrixT<T> rMatrix;
	rMatrix.Construct( nRowNum, nLineNum);
	for( int i = 0; i< nRowNum; i++)
		for( int j = 0; j< nLineNum; j++)
		{
			rMatrix.e( 1 + i,1+j) = this->e( nFirstRow + i,nFirstLine + j);
		}
	return rMatrix;
}

template< class T>
void CMatrixT<T>::TakeIn( const CMatrixT<T> &SrcMatrix,
								const int &nFirstRow, const int &nFirstLine )
{
	int nRowNum = SrcMatrix.m_nRow;
	int nLineNum = SrcMatrix.m_nLine; 
	
	for( int i = 0; i<nRowNum; i++)
	{
		for( int j = 0; j< nLineNum; j++) 
		{
			this->e( nFirstRow + i, nFirstLine + j) = SrcMatrix.e( 1+ i,1+j);

		}
	}
}

template< class T>
CMatrixT<T> CMatrixT<T>::Unite(const CMatrixT<T> &SrcMatrix, const int nFlag)const//nFlag default= 1
{
	int nRow,nLine;
    CMatrixT<T> rMatrix;

	switch( nFlag )
	{
	case 1:
		nRow = m_nRow;
		nLine = m_nLine + SrcMatrix.m_nLine;
		rMatrix.Construct( nRow,nLine);
		rMatrix.TakeIn( *this);
		rMatrix.TakeIn( SrcMatrix,  1,m_nLine + 1);
		break;
	case 2:
		nRow = m_nRow  + SrcMatrix.m_nRow;
		nLine = m_nLine;
		rMatrix.Construct( nRow,nLine);
		rMatrix.TakeIn(*this);
//		rMatrix.TakeIn( *this);
		rMatrix.TakeIn( SrcMatrix, m_nRow + 1, 1);
		break;




	}
	return rMatrix;
	
}

template< class T>
inline CMatrixT<T> operator | ( const CMatrixT<T> &Matrix1,const CMatrixT<T> &Matrix2)
{
	return Matrix1.Unite( Matrix2);

}

template< class T>
CMatrixT<T> operator || ( const CMatrixT<T> &Matrix1,const CMatrixT<T> &Matrix2)
{
	return Matrix1.Unite( Matrix2,2);
}


template< class T>
void CMatrixT<T>::ChangeByCmd(const CChangeCmdT<T> &Cmd)
{
	char cStyle = Cmd.m_cStyle;
	switch( cStyle)
	{
	case ::ROW_INTERCHANGE:
		this->RowInterChange( Cmd.m_nDest, Cmd.m_nSrc);
		break;
	case ::ROW_MULTIPLY:
		this->RowMultiply( Cmd.m_nDest,Cmd.m_tTimes);
		break;
	case ::ROW_PLUS:
		this->RowPlus( Cmd.m_nDest,Cmd.m_nSrc,Cmd.m_tTimes);
		break;

	case ::LINE_INTERCHANGE:
		this->LineInterChange( Cmd.m_nDest, Cmd.m_nSrc);
		break;
	case ::LINE_MULTIPLY:
		this->LineMultiply( Cmd.m_nDest,Cmd.m_tTimes);
		break;
	case ::LINE_PLUS:
		this->LinePlus( Cmd.m_nDest,Cmd.m_nSrc,Cmd.m_tTimes);
		break;

	}


}

template< class T>
void CMatrixT<T>::ChangeByCmd(const CChangeCmdT<T> &Cmd, const int &nFirst, const int &nLast)
{
	char cStyle = Cmd.m_cStyle;
	switch( cStyle)
	{
	case ::ROW_INTERCHANGE:
		this->RowInterChange( Cmd.m_nDest, Cmd.m_nSrc, nFirst, nLast);
		break;
	case ::ROW_MULTIPLY:
		this->RowMultiply( Cmd.m_nDest,Cmd.m_tTimes,nFirst,nLast);
		break;
	case ::ROW_PLUS:
		this->RowPlus( Cmd.m_nDest,Cmd.m_nSrc,Cmd.m_tTimes,nFirst,nLast);
		break;

	case ::LINE_INTERCHANGE:
		this->LineInterChange( Cmd.m_nDest, Cmd.m_nSrc, nFirst,nLast);
		break;
	case ::LINE_MULTIPLY:
		this->LineMultiply( Cmd.m_nDest,Cmd.m_tTimes,nFirst,nLast);
		break;
	case ::LINE_PLUS:
		this->LinePlus( Cmd.m_nDest,Cmd.m_nSrc,Cmd.m_tTimes, nFirst, nLast);
		break;

	}


}


template< class T>
void CMatrixT<T>::ChangeByCS(const CCmdSeriesT<T> &CS, const int &nFirst, const int &nLast )
{
//	int nCurrentPosition = CS.GetCurrentPosition();
	int nPosition1 = nFirst;
	int nPosition2 = nLast;
	int nCmdNum = CS.GetCommandNum();
	if(nLast < 0 || nLast>nCmdNum)
		nPosition2 = nCmdNum;
	int nNum = nPosition2 - nPosition1 + 1;
	int nCount = 0;
	CS.MovePointerToPosition( nPosition1);
	while( nCount < nNum)
	{
		this->ChangeByCmd( CS.e() );
		nCount++;
		CS.MovePointerForward();
	}
//	CS.MovePointerToPosition(nCurrentPosition);
		
}



template< class T>
inline void CMatrixT<T>::BasicLineSimplify(CCmdSeriesT<T> *pCS, const int &nX, const int &nY)
{
	CChangeCmdT<T> Cmd;
	for( int i = 0; i< m_nRow - nX; i++)
	{
		Cmd.Create3( 'R', nX+1+i, -e(nX+1+i,nY)/e(nX,nY), nX);
		this->ChangeByCmd( Cmd, nY + 1, m_nLine);
		e( nX+1+i, nY) = 0;
		pCS->AddNewCmd(Cmd);

	}
}


template< class T>
inline int CMatrixT<T>::FindSolidRow(const int &nX,const int &nY)const
{
	for( int i = 0; i<m_nRow - nX + 1;i++)
	{
		if( e(nX+i,nY) != 0)
			return nX + i;
	}
	return -1;
}

template< class T>
void CMatrixT<T>::GetCS(CCmdSeriesT<T> *pCS)const
{
	CMatrixT<T> cMatrix = *this;
	cMatrix.ChangeToStandardF(pCS);
}

template< class T>
void CMatrixT<T>::ChangeToStairsF(CCmdSeriesT<T> *pCS,const int &nBorderLine)
{

	int nSolved = 0;
	int nTryed;

	int nValidRow = m_nRow;
	int nValidLine = nBorderLine;

	if( nValidLine < 0)
	{
		nValidLine = m_nLine;
	}
	
	CChangeCmdT<T> Cmd;

	pCS->Clear();
	pCS->Initial();

	while( nSolved<nValidLine && nSolved<nValidRow)
	{
		nTryed = nSolved + 1;
		int nSolidRow = this->FindSolidRow( nTryed,nTryed);

		if( nSolidRow < 0)
		{
			if( nTryed != nValidLine)
			{
				Cmd.Create1( 'L',nTryed,nValidLine);
				this->ChangeByCmd( Cmd );
				pCS->AddNewCmd( Cmd);
				nValidLine--;
			}
			else
			{
				nValidLine--;
			}
			continue;
		}

		else if( nSolidRow >nTryed )
		{
			Cmd.Create1( 'R', nTryed,nSolidRow);

			this->ChangeByCmd( Cmd,nTryed,m_nLine);
			pCS->AddNewCmd( Cmd);
		}



		this->BasicLineSimplify(pCS,nTryed,nTryed);
		nSolved++;

	}

	pCS->m_nRank = nSolved;
	pCS->m_nRankPosition =  pCS->GetCommandNum();

}

template< class T>
inline int CMatrixT<T>::GetRank() const
{
	CMatrixT<T> cMatrix = *this;
	CCmdSeriesT<T> CS;

	cMatrix.ChangeToStairsF(&CS);
	return CS.m_nRank;
}


template< class T>
T CMatrixT<T>::GetDeterminant() const
{
	T rT = 1;
	CMatrixT<T> cMatrix = *this;
	CCmdSeriesT<T> CS;

	cMatrix.ChangeToStairsF(&CS);


	int nInterCount = CS.CountCmds( ::INTERCHANGE);

	for( int i = 0; i< m_nRow;i++)
	{
		rT *= cMatrix.e( 1+i,1+i);
		
	}

	if( nInterCount % 2)
	{
		rT = - rT;
	}

	return rT;
}


template< class T>
inline void CMatrixT<T>::StairsToSwords(CCmdSeriesT<T> *pCS)
{
	CChangeCmdT<T> Cmd;
	int nRank = pCS->m_nRank;

	for( int i=0;i<nRank;i++)
	{
		Cmd.Create2( 1/e(1+i,1+i),'R',1+i);
		e(1+i,1+i) = 1;
		this->ChangeByCmd( Cmd,i+2,m_nLine);
		pCS->AddNewCmd(Cmd);
	}
	pCS->m_nSwordPosition = pCS->GetCommandNum();
}


template< class T>
inline void CMatrixT<T>::SwordsToCross(CCmdSeriesT<T> *pCS)
{
	CChangeCmdT<T> Cmd;
	int nRank = pCS->m_nRank;

	for( int i=0; i<nRank -1;i++)
	{
		for( int j=0; j<nRank - i -1; j++)
		{
			Cmd.Create3( 'R',1+j,-e(1+j,nRank-i)/e(nRank-i,nRank-i),nRank - i);
			e( 1+j, nRank - i) = 0;
			this->ChangeByCmd( Cmd,nRank+1,m_nLine);
			pCS->AddNewCmd(Cmd);
		}
	}

	pCS->m_nCrossPosition = pCS->GetCommandNum();


}

template< class T>
inline void CMatrixT<T>::CrossToStandard(CCmdSeriesT<T> *pCS)
{
	CChangeCmdT<T> Cmd;
	int nRank = pCS->m_nRank;
	for( int i=0; i<nRank; i++)
	{
		for( int j=0; j<m_nLine-nRank;j++)
		{
			Cmd.Create3( 'L',nRank+1+j,-e(1+i,nRank + 1+j),1+i);
			e( 1+i,nRank + 1+j) = 0;
			pCS->AddNewCmd( Cmd);
		}
	}
	pCS->m_nStandardPosition = pCS->GetCommandNum();

}


template< class T>
inline void CMatrixT<T>::ChangeToStandardF(CCmdSeriesT<T> *pCS)
{
	this->ChangeToStairsF(pCS);
	this->StairsToSwords(pCS);
	this->SwordsToCross(pCS);
	this->CrossToStandard(pCS);
}

template<class T>
CMatrixT<T> CMatrixT<T>::operator ~() const
{
	CMatrixT<T> rMatrix = CMatrixT<T>('E',m_nRow);
	CMatrixT<T> cMatrix = *this;
	CCmdSeriesT<T> CS;
	cMatrix.ChangeToStandardF(&CS);
	rMatrix.ChangeByCS( CS);


	return rMatrix;
	
}

template< class T>
void CMatrixT<T>::SolveLinearEqs( CMatrixT<T> SolutionMs[], bool bResults[], const int &nEqNum)
{

	CCmdSeriesT<T> CS,reCS;
	int nUnkownNum = m_nLine - nEqNum;
	bool b = false;

	this->ChangeToStairsF( &CS,nUnkownNum);
	int nRank = CS.m_nRank;
	int nF = nUnkownNum - nRank;

	for( int i = 0;i<nEqNum;i++)
	{
		if( this->FindSolidRow( nRank + 1 ,nUnkownNum + 1 +i) > 0)
		{
			bResults[i] = false;
			
		}
		else
		{
			bResults[i] = true;
			b = true;
		}
	}
	if( b == false)
	{
		return;

	}
	this->StairsToSwords(&CS);
	this->SwordsToCross(&CS);
	CMatrixT E( 'E',nF);
	CMatrixT F = this->TakeOut( 1,nRank+1,nRank,nF);
	F *= -1;

	for( int i = 0; i<nEqNum; i++)
	{
		//针对列ｎ＿ＵｎｋｏｗｎＮｕｍ　＋　１＋ｉ
		if( bResults[i] == false)
		{
			continue;
		}
		else
		{
			SolutionMs[i].Construct( nUnkownNum,nUnkownNum - nRank + 1);

			//对常数项进行赋值
			//从１到nRank行，共nRank项
			//第ｊ次针对 e( 1+j, nUnkownNum + 1 + i)
			for( int j = 0;j<nRank; j++)
			{
				SolutionMs[i].e( 1+j ,1)  = this->e( 1+j , nUnkownNum + 1 + i);
				
			}
			for( int j = 0;j<nUnkownNum - nRank ;j++)
			{
				SolutionMs[i].e( nRank + 1+j ,1)  = 0;
				
			}
	
			//右下角的单位矩阵赋值  
				SolutionMs[i].TakeIn( E,nUnkownNum - nF + 1,2);

			//再：
			//对于自由的未知数进行移项，做法从系数矩中取出，乘上１，再赋给解矩阵
			//
			SolutionMs[i].TakeIn( F, 1,2); 

			//最后，我们要对解矩中未知数的次序进行还原

			reCS.CreateByReverse( CS,::LINE_INTERCHANGE);
			reCS.TurnOpposite();
			SolutionMs[i].ChangeByCS(reCS);


		}
	}


}



template< class T>
bool CMatrixT<T>::SolveLinearEq(CMatrixT<T> &SolutionMatrix)const
{
	CMatrixT<T> cMatrix = *this;
	bool rB;
	cMatrix.SolveLinearEqs( &SolutionMatrix,&rB);
	return rB;
}

template< class T>
void CMatrixT<T>::Simplify( CMatrixT<T> &Matrix, 
						   const int &nBorderLine  ,const int &nFlag )const
{
	int nBorder;
	nBorder = nBorderLine;
	if( nBorder < 0 || abs( nFlag) == 4 || nBorder>m_nLine)
	{
		nBorder = m_nLine;
	}
	Matrix = *this;
	CCmdSeriesT<T> CS;
	Matrix.ChangeToStairsF( &CS,nBorder);

	switch(nFlag)
	{
	case 1:
	case -1:
		break;
	case 2:
	case -2:
		Matrix.StairsToSwords(&CS);
		break;
	case 3:
	case -3:
		Matrix.StairsToSwords(&CS);
		Matrix.SwordsToCross(&CS);
		break;
	case 4:
	case -4:
		Matrix.StairsToSwords(&CS);
		Matrix.SwordsToCross(&CS);
		Matrix.CrossToStandard(&CS);	
		break;
	
	}
	if( nFlag > 0)
	{
		return;
	}
	else
	{
		CCmdSeriesT<T> reCS;
		reCS.CreateByReverse( CS, LINE_INTERCHANGE);
		Matrix.ChangeByCS(reCS);
	}
}



template< class T>
class CVectorT
{
protected:
	T * m_pOrigin;
	int m_nDimension;
public:
	CVectorT():m_pOrigin(NULL){};
	CVectorT( const CVectorT<T> &Vector):m_pOrigin(NULL)
	{
		Create(Vector);
	};
	CVectorT( const int &nDimension , const T &Te):m_pOrigin(NULL)
	{
		Create(nDimension,Te);
	};
	CVectorT( T e_Array[], const int &nDimension,const int &nStep = 1):m_pOrigin(NULL)
	{
		Create(e_Array,nDimension,nStep);
	};
	CVectorT( const CMatrixT<T> &Matrix,const int &nPosition,const int &nFlag = 1):m_pOrigin(NULL)
	{
		Create(Matrix,nPosition,nFlag);
	};
public:
	~CVectorT()
	{
		Clear();
	};
public:
	operator CMatrixT<T>()const;
	const CVectorT<T>  operator -()const
	{
		CVectorT<T> rVector;
		rVector.Construct(m_nDimension);
		for( int i =0;i<m_nDimension;i++)
		{
			rVector.e(1+i) = -e(1+i);
		}
		return rVector;
	};
	const CVectorT<T> & operator +=( const CVectorT<T> &Vector)
	{
		for( int i = 0; i<m_nDimension; i++)
		{
			this->e( 1+i) += Vector.e(1+i);
		}
		return *this;
	};
	const CVectorT<T> & operator -=( const CVectorT<T> &Vector)
	{
		for( int i = 0; i<m_nDimension; i++)
		{
			this->e( 1+i) -= Vector.e(1+i);
		}
		return *this;
	};

	const CVectorT<T> & operator *=( const T &tTimes)
	{
		for( int i = 0; i<m_nDimension; i++)
		{
			this->e( 1+i) *= tTimes;
		}
		return *this;
	};
	const CVectorT<T> & operator /=( const T &tTimes)
	{
		for( int i = 0; i<m_nDimension; i++)
		{
			this->e( 1+i) /= tTimes;
		}
		return *this;
	};

public:
	void Clear()
	{
		if(m_pOrigin!=NULL)
		{
			delete[] m_pOrigin;
			m_pOrigin = NULL;
		}
	};
	void Construct()
	{
		Clear();
		m_pOrigin = new T[m_nDimension];

	};
	void Construct( const int &nDimension)
	{
		m_nDimension = nDimension;
		Construct();
	};
	T &e(const int &nIndex)
	{
		return *(m_pOrigin + nIndex - 1);
	};
	T e(const int &nIndex)const
	{
		return *(m_pOrigin + nIndex -1);
	};
	void Create( T e_Array[], const int &nDimension,const int &nStep = 1);
	void Create( const CMatrixT<T> &Matrix,const int &nPosition,const int &nFlag = 1);
	void Create( const CMatrixT<T> &Matrix);
	void Create( const int &nDimension , const T &Te=0);
	void Create( const CVectorT<T> &Vector);
	const CVectorT & operator = ( const CVectorT<T> &Vector)
	{
		Create(Vector);
		return *this;
	};
	void Print()const;
public:
	int GetDimension()const
	{
		return m_nDimension;
	};
	T Length()const;
public:
	CVectorT<T> TakeOut( const int &nFirstD, const int &nDimension)const
	{
		CVectorT<T> rVector = *this;
		rVector.Construct(nDimension);
		for( int i = 0; i<nDimension; i++)
		{
			rVector.e( 1+i) = this->e( nFirstD + i);
		}
		return rVector;
	};
};

template<class T>
const CVectorT<T> operator +(const CVectorT<T> &Vector1,const CVectorT<T> &Vector2)
{
	CVectorT<T> rVector = Vector1;
	return rVector += Vector2;
}

template<class T>
const CVectorT<T> operator -(const CVectorT<T> &Vector1,const CVectorT<T> &Vector2)
{
	CVectorT<T> rVector = Vector1;
	return rVector -= Vector2;
}

template<class T>
const CVectorT<T> operator *(const CVectorT<T> &Vector,const T &tTimes)
{
	CVectorT<T> rVector = Vector;
	return rVector *= tTimes;
}

template<class T>
const CVectorT<T> operator /(const CVectorT<T> &Vector,const T &tTimes)
{
	CVectorT<T> rVector = Vector;
	return rVector /= tTimes;
}

template<class T>
const CVectorT<T> operator *(const CVectorT<T> &Vector,const int &tTimes)
{
	CVectorT<T> rVector = Vector;
	return rVector *= T(tTimes);
}

template<class T>
const CVectorT<T> operator /(const CVectorT<T> &Vector,const int &tTimes)
{
	CVectorT<T> rVector = Vector;
	return rVector /= T(tTimes);
}


template<class T>
const CVectorT<T> operator *(const T &tTimes,const CVectorT<T> &Vector)
{
	CVectorT<T> rVector = Vector;
	return rVector *= tTimes;
}



template<class T>
const CVectorT<T> operator *(const int &tTimes,const CVectorT<T> &Vector)
{
	CVectorT<T> rVector = Vector;
	return rVector *= T(tTimes);
}



template<class T>
T operator *(const CVectorT<T> &Vector1,const CVectorT<T> &Vector2)
{
	T rSum = 0;
	int nDimension = Vector1.GetDimension();
	for( int i =0; i<nDimension; i++)
	{
		rSum += Vector1.e(1+i) * Vector2.e(1+i);
	}
	return rSum;
}









template< class T>
 void CVectorT<T>::Create( T e_Array[], const int &nDimension, const int &nStep)
{
	Construct(nDimension);
	for( int i = 0; i<nDimension; i++)
	{
		e( 1+i) = *(e_Array + i*nStep);
	}
}

 template< class T>
 void CVectorT<T>::Create(const CMatrixT<T> &Matrix, const int &nPosition, const int &nFlag = 1)
 {
	 const int &nLine = Matrix.GetLine();
	 const int &nRow  = Matrix.GetRow();
	 switch(nFlag)
	 {
	 case 1:
		 Construct(nRow);
		 for( int i=0; i<m_nDimension; i++)
		 {
			 this->e(1+i) = Matrix.e( 1+i,nPosition);
		 }
		 break;
	 case 2:
		 Construct(nLine);
		 for(int i=0; i<m_nDimension; i++)
		 {
			 this->e(1+i) = Matrix.e(nPosition,1+i);
		 }
		 break;
	 case 3:
		 Construct(nRow*nLine);
		 for( int i =0; i<nRow; i++)
		 {
			 for( int j=0; j<nLine; j++)
			 {
				 this->e( 1+ i*nLine + j) = Matrix.e( 1+i,1+j);
			 }
		 }
		 break;


	 }
 }

  template< class T>
  void CVectorT<T>::Create(const int &nDimension, const T &Te)
  {
	  Construct(nDimension);
	  for( int i=0; i<m_nDimension;i++)
	  {
		  e(1+i) = Te;
	  }
  }

 template< class T>
 void CVectorT<T>::Create(const CMatrixT<T> &Matrix)
 {
	 const int &nLine = Matrix.GetLine();
	 const int &nRow  = Matrix.GetRow();
	 Construct(nRow*nLine);
	 for( int i =0; i<nRow; i++)
	 {
		 for( int j=0; j<nLine; j++)
		 {
			 this->e( 1+ i*nLine + j) = Matrix.e( 1+i,1+j);
		 }
	 }

 }


  template< class T>
  void CVectorT<T>::Create(const CVectorT<T> &Vector)
  {
	  Construct( Vector.m_nDimension);
	  for( int i=0; i<m_nDimension; i++)
	  {
		  this->e(1+i) = Vector.e(1+i);
	  }
  }

 template< class T>
 void CVectorT<T>::Print()const
 {
	 for( int i = 0 ; i<m_nDimension; i++)
	 {
		 cout<<e(1+i)<<'\t';

	 }
	 cout<<endl;
 }


 template< class T>
CVectorT<T>::operator CMatrixT<T>() const
{
	CMatrixT<T> rMatrix;
	rMatrix.Create( m_pOrigin,m_nDimension,1);
	return rMatrix;
}





template< class T>
bool CMatrixT<T>::SolveLinearEq(const CVectorT<T> *pVector, CMatrixT<T> &SolutionMatrix) const
{
	bool rB;
	CMatrixT<T> cMatrix;
	CVectorT<T> vF;
	if( pVector == NULL)
	{
		vF.Create(m_nRow);
		cMatrix = *this | (CMatrixT<T>)vF;
		cMatrix.SolveLinearEqs( &SolutionMatrix,&rB);


	}
	else
	{
		cMatrix = *this | (CMatrixT<T>)(*pVector);
//		cMatrix.Print();
		cMatrix.SolveLinearEqs( &SolutionMatrix,&rB);
	}
	return rB;
}

template< class T>
const CVectorT<T> operator *(const CMatrixT<T> &Matrix,const CVectorT<T> &Vector)
{
	CVectorT<T> rVector;
	rVector.Create( Matrix * (CMatrixT<T>)Vector );
	return rVector;
}
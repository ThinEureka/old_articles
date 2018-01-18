//MTApp.h
//利用MatrixT解决一些问题的函数（包括产生某些特殊矩阵的ones)的定义。

#pragma once

#include "MatrixT.h"


namespace tmpn0
{
	template< class T>
	inline void StepMultiply( const T Xs[], T Ps[],const int &nRank)
	{
		for( int i = 0; i<nRank; ++i)
		{
			Ps[i] *= Xs[i];
		}
	}
	inline int CountYes( const bool Bs[],const int &nRank)
	{
		int nR = 0;
		for( int i = 0; i<nRank; ++i)
		{
			if( Bs[i])
				++nR;
		}
		return nR;
	}
}


template< class T>
void CreateSM( const T Xs[],const int &nRank,CMatrixT<T> &SM);
//名字起源：
//首先要提到这里创建的矩阵的出身。
//多项式线性拟合过程，实际上是在一个指定的线性空间内，寻找一个合适的投影向量for某一目标向量。
//在这里此线性空间由拟合对象的自变量以及拟合所需的阶数决定，目标向量由拟合对象的变量决定。
//可以这样描述：若拟合对象是（x1,y1),(x2,y2),...(xm,ym)，拟合的阶数为n。
//则前之所述线性空间为下列n个向量构成的线性空间。
// e0=(1,1,...,1)T,e1=(x1,x2,...,xm)T,...en=(x1^n-1,x2^n-1,...xm^n-1)T
//当m>=n时，这n个向量线性无关，也就是该空间的一组基。
//目标向量为：y=(y1,y2,...,ym)
//下面所要定义的函数就是构建一个矩阵which描述上面所说的线性空间，显然这一矩阵
//就是前之所述的向量的排列。（当这一矩阵已经求出来了，这个拟合的过程只是一个解方程的过程which
//利用矩阵类的功能只是一个statement而已。）
//上面是这一矩阵的出身。
//从上面的描述可以看出来，这一矩阵的每一列是前一列（if有的话）的xj倍，就像台阶一样。
//在我的思维习惯与描述方式下，这就像一排逐级上升的台阶，于是就命之名为StepMatrix，
//为了使用的方便，简称为SM，相信这样的简称对于以后是一种必要的记忆for这一矩阵实际太常见了。
//说明完毕。还是将声明与定义分开好了。


//前面声明说明的一个拟合的情况是一种最常见的情况when从0次到n-1次的项都参与拟合。
//有时候我们并不需要所有的项，这就是底下要处理的情况。
//但这里不再走得更远，所有的工作仅限于一元多项式的拟合。
// m for Xs,and n for Bs and  for the highest power n-1
template<class T>
void CreateSM( const T Xs[],const bool Bs[],const int &m,const int &n,CMatrixT<T> &SM)
{
	using namespace tmpn0;

	T* Ps = new T [m];
	for( int i = 0;i<m;++i)
	{
		Ps[i] = 1;
	}

	if( !Bs)
	{
		SM.Construct(m,n);
	
		for( int i = 0;i<n; ++i)
		{
			for( int j = 0; j<m; ++j)
			{

				SM.e(1+j,1+i) = Ps[j];
			}
			if( i != n-1)
			{
				StepMultiply(Xs,Ps,m);
			}
		}
	}
	else
	{
		int nRank = CountYes(Bs,n);
		SM.Construct(m,nRank);

		int i = 0;
		int k = 0;
		while( i<n)
		{
			if( Bs[i] )
			{
				for( int j = 0;j<m; ++j)
				{
					SM.e(1+j,1+k) = Ps[j];
				
				}
					++k;
				if( i!= n-1)
				{
					StepMultiply(Xs,Ps,m);
				}
				++i;
			}
			else
			{
				while( Bs[i] == false && i<n)
				{
					if( i!= n-1)
					{
						StepMultiply(Xs,Ps,m);
					}

					++i;
				}
			}
	
		
		}

	}
	delete []Ps;
}



template <class T>
void Rotate(CMatrixT<T>& Matrix,const int &nRow,const int &nLine)
{
	T tE = Matrix.e(nRow,nLine);
	int nRowNum = Matrix.GetRow();
	int nLineNum = Matrix.GetLine();
	Matrix.RowMultiply(nRow,1/tE,1,nLine-1);
	Matrix.RowMultiply(nRow,1/tE,nLine+1,nLineNum);
	Matrix.e(nRow,nLine) = 1;

	for( int i=0;i<nRow-1;++i)
	{
		Matrix.RowPlus(1+i,nRow,-Matrix.e(1+i,nLine),1,nLine-1);
		Matrix.RowPlus(1+i,nRow,-Matrix.e(1+i,nLine),nLine+1,nLineNum);
		Matrix.e(1+i,nLine) = 0;
	}

	for( int i=0;i<nRowNum-nRow;++i)
	{
		Matrix.RowPlus(nRow+1+i,nRow,-Matrix.e(nRow+1+i,nLine),1,nLine-1);
		Matrix.RowPlus(nRow+1+i,nRow,-Matrix.e(nRow+1+i,nLine),nLine+1,nLineNum);
		Matrix.e(nRow+1+i,nLine) = 0;
	}




}

//膨胀一个矩阵，即将一个n阶方形矩阵的每一个元素，用一个n阶方阵来代替，
//而所说的方阵是这样的：用这一元素乘上n阶的单位矩阵。
//如此，便得到一个n×n的方阵。
//这样做的意义在于，得到的新的方阵的行列式是原来方阵行列式的n次方，
//对于上面所说的结论，我并没有得到证明，但是通过比较多的计算有充分的理由相信这是对的。
template< typename T>
void ExpandMatrix(const CMatrixT<T> &mSrc,CMatrixT<T> &mDest)
{
	int nDimension = mSrc.GetRow();
	int nRow = nDimension*nDimension;
	mDest.Construct(nRow,nRow);
	CMatrixT<T> E('E',nDimension);

	for( int i=0;i<nDimension;++i)
		for( int j=0;j<nDimension;++j)
		{
			mDest.TakeIn( mSrc.e(1+i,1+j)*E,1+nDimension*i,1+nDimension*j);
		}
}


//当源矩阵是一个方阵时，得到的新矩阵的行列式，为源矩阵的nRank次方
template< typename T>
void LooseMatrix(const CMatrixT<T> &mSrc,CMatrixT<T> &mDest,int nRank)
{
	int nRow = mSrc.GetRow(); 
	int nLine = mSrc.GetLine();
	mDest.Construct(nRow*nRank,nLine*nRank);
	CMatrixT<T> E('E',nRank);

	for( int i=0;i<nRow;++i)
		for( int j=0;j<nLine;++j)
		{
			mDest.TakeIn( mSrc.e(1+i,1+j)*E,1+nRank*i,1+nRank*j);
		}
}



template<typename T>
void CreateMUnitMatrix( const CMatrixT<T> &mSrc, CMatrixT<T> &mDest)
{
	int nDimension = mSrc.GetRow();
	int nRow = nDimension*nDimension;
	mDest.Construct(nRow,nRow);
	mDest.TurnToO();

	for( int i=0; i<nDimension;++i)
	{
		mDest.TakeIn( mSrc,1+nDimension*i,1+nDimension*i);
	}

}

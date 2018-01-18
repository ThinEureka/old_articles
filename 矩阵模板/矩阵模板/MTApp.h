//MTApp.h
//����MatrixT���һЩ����ĺ�������������ĳЩ��������ones)�Ķ��塣

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
//������Դ��
//����Ҫ�ᵽ���ﴴ���ľ���ĳ���
//����ʽ������Ϲ��̣�ʵ��������һ��ָ�������Կռ��ڣ�Ѱ��һ�����ʵ�ͶӰ����forĳһĿ��������
//����������Կռ�����϶�����Ա����Լ��������Ľ���������Ŀ����������϶���ı���������
//������������������϶����ǣ�x1,y1),(x2,y2),...(xm,ym)����ϵĽ���Ϊn��
//��ǰ֮�������Կռ�Ϊ����n���������ɵ����Կռ䡣
// e0=(1,1,...,1)T,e1=(x1,x2,...,xm)T,...en=(x1^n-1,x2^n-1,...xm^n-1)T
//��m>=nʱ����n�����������޹أ�Ҳ���Ǹÿռ��һ�����
//Ŀ������Ϊ��y=(y1,y2,...,ym)
//������Ҫ����ĺ������ǹ���һ������which����������˵�����Կռ䣬��Ȼ��һ����
//����ǰ֮���������������С�������һ�����Ѿ�������ˣ������ϵĹ���ֻ��һ���ⷽ�̵Ĺ���which
//���þ�����Ĺ���ֻ��һ��statement���ѡ���
//��������һ����ĳ���
//��������������Կ���������һ�����ÿһ����ǰһ�У�if�еĻ�����xj��������̨��һ����
//���ҵ�˼άϰ����������ʽ�£������һ����������̨�ף����Ǿ���֮��ΪStepMatrix��
//Ϊ��ʹ�õķ��㣬���ΪSM�����������ļ�ƶ����Ժ���һ�ֱ�Ҫ�ļ���for��һ����ʵ��̫�����ˡ�
//˵����ϡ����ǽ������붨��ֿ����ˡ�


//ǰ������˵����һ����ϵ������һ����������when��0�ε�n-1�ε��������ϡ�
//��ʱ�����ǲ�����Ҫ���е������ǵ���Ҫ����������
//�����ﲻ���ߵø�Զ�����еĹ���������һԪ����ʽ����ϡ�
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

//����һ�����󣬼���һ��n�׷��ξ����ÿһ��Ԫ�أ���һ��n�׷��������棬
//����˵�ķ����������ģ�����һԪ�س���n�׵ĵ�λ����
//��ˣ���õ�һ��n��n�ķ���
//���������������ڣ��õ����µķ��������ʽ��ԭ����������ʽ��n�η���
//����������˵�Ľ��ۣ��Ҳ�û�еõ�֤��������ͨ���Ƚ϶�ļ����г�ֵ������������ǶԵġ�
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


//��Դ������һ������ʱ���õ����¾��������ʽ��ΪԴ�����nRank�η�
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

//INT.h

/*
#pragma once
#ifndef INT_H
#define INT_H


#include "REAL.h"

class INT:public REAL
{
public:
	INT()
	{};

	INT(typeSignUnit suNum):REAL(suNum)
	{};

	INT(typeUnit uNum):REAL(uNum)
	{};
	INT(const REAL &R)
	{
		m_nSign = R.Sign();
		m_nFeet = std::max( R.Feet(),0);
		m_nHead = std::max( R.Head(),1);
		Construct(m_nFeet,m_nHead);
		if( m_nHead == 1)
		{
			e(0) = R.ce(0);
		}
		else
		{
			memcpy( static_cast<void *>( p(Feet()) ), 
				static_cast<const void *>( R.cp(Feet()) ),
				(Size() )*sizeof(typeUnit) );
		}
	};
public:
	friend INT & Plus( const INT &N1,const INT &N2,INT &N)
	{
		REAL::Plus(static_cast<const REAL &>( N1),static_cast<const REAL &>(N2),static_cast<REAL &>(N));
		return N;
	};
	friend INT & Subtract( const INT &N1,const INT &N2,INT &N)
	{
		REAL::Subtract(static_cast<const REAL &>( N1),static_cast<const REAL &>(N2),static_cast<REAL &>(N));
		return N;
	};
	friend INT & Multiply( const INT &N1,const INT &N2,INT &N)
	{
		REAL::Multiply(static_cast<const REAL &>( N1),static_cast<const REAL &>(N2),static_cast<REAL &>(N));
		return N;
	};
	friend INT & Divide( const INT &N1,const INT &N2,INT &N,INT &M)
	{
		int nRealPrec;
		Divide(static_cast<const REAL &>( N1),static_cast<const REAL &>(N2),static_cast<REAL &>(N),
			   0,nRealPrec);
		N.TruncateFeet(0);
		M = N1 - N2*N;
		return N;
	};
	friend INT operator + ( const INT &N1,const INT &N2)
	{
		INT N;
		Plus(N1,N2,N);
		return N;
	};
	friend INT operator - ( const INT &N1,const INT &N2)
	{
		INT N;
		Subtract(N1,N2,N);
		return N;
	};
	friend INT operator * ( const INT &N1,const INT &N2)
	{
		INT N;
		Multiply(N1,N2,N);
		return N;
	};
	friend INT operator / ( const INT &N1,const INT &N2)
	{
		INT N;
		INT M;
		Divide(N1,N2,N,M);
		return N;
	};
	friend INT operator % ( const INT &N1,const INT &N2)
	{
		INT M;
		INT N;
		Divide(N1,N2,N,M);
		return M;
	};
public:
	INT& operator +=( const INT &N1)
	{
		INT N2 = *this;
		return Plus(N1,N2,*this);
	};
	INT& operator -=( const INT &N1)
	{
		INT N2 = *this;
		return Subtract(N2,N1,*this);
	};
	INT& operator *=( const INT &N1)
	{
		INT N2 = *this;
		return Multiply(N1,N2,*this);
	};
	INT& operator /=( const INT &N1)
	{
		INT N2 = *this;
		INT M;
		return Divide(N2,N1,*this,M);
	};
	INT& operator %=( const INT &N1)
	{
		INT N2 = *this;
		INT N;
		Divide(N2,N1,N,*this);
		return *this;
	};
public:
	INT&  operator <<=(int nBit)
	{
		REAL::operator <<=(nBit);
		return *this;
	};
	INT&  operator >>=(int nBit)
	{
		REAL::operator >>=(nBit);
		TruncateFeet(0);
		return *this;

	};
	friend INT operator <<(const INT &N1,int nBit)const
	{
		INT N = N1;
		N <<= nBit;
		return N;
	};
	friend INT operator >>(const INT &N1,int nBit)const
	{
		INT N = N1;
		N >>= nBit;
		return N;
	};
public:
	INT operator +()const
	{
		return *this;
	};
	INT operator -()const
	{
		INT N;
		N = *this;
		N.Minus();
		return N;
	};
	INT& operator ++()const
	{
		INT N = *this;
		return Plus(N,1,*this);
	};
	INT& operator --()const
	{
		INT N = *this;
		return Plus(N,-1,*this);
	};
	INT operator ++(int)const
	{
		INT N = *this;
		Plus(N,1,*this);
		return N;
	};
	INT operator --(int)const
	{
		INT N = *this;
		Plus(N,-1,*this);
		return N;
	};
	operator bool()const
	{
		return !IsZero();
	};




};


#endif

*/
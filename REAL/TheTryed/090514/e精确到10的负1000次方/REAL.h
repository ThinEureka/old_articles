//REAL.h

#pragma once
#ifndef REAL_H
#define REAL_H

#include <string>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <cassert>
#include <string>

namespace zju04nycs
{
	class REAL
	{
	protected:
		typedef unsigned __int64 typeLink;
		typedef unsigned __int32 typeUnit;
		typedef __int32 typeSignUnit;
		typedef __int64 typeSignLink;
		static const int sc_nUnitBit = 32;

	private:
		static int s_nPrec;
	private:
		static int s_nDisWidth;
		static char s_cDisBase;
		static bool s_bDisFill;
	public:
		static REAL PLUS;	static REAL SUB;	static REAL MUL;	static REAL DIV;	static REAL TMP;
		static REAL PLUS1;	static REAL SUB1;	static REAL MUL1;	static REAL DIV1;	static REAL TMP1;
		static REAL PLUS2;	static REAL SUB2;	static REAL MUL2;	static REAL DIV2;	static REAL TMP2;
	private:
		int m_nHead;
		int m_nFeet;
		int m_nTop;
		int m_nBottom;
		typeUnit * m_puOrigin;
		int m_nSign;

	public:
		REAL()
		{
			m_nSign = 1;
			Construct(0,1);
			m_nFeet = 0;
			m_nHead = 1;
			e(0) = 0;	
		};

		REAL(typeSignUnit suNum,int nFeet = 0)
		{

			m_nFeet = nFeet;
			m_nHead = nFeet + 1;
			Construct(Feet(),Head());

			if( suNum>= 0)
			{
				m_nSign = 1;
				e(Feet()) = suNum;
			}
			else
			{
				m_nSign = -1;
				e(Feet()) = -suNum;
			}

			if( !suNum)
			{
				Fit();
			}

		};

		REAL(typeUnit uNum,int nFeet = 0)
		{
			m_nSign = 1;
			m_nFeet = nFeet;
			m_nHead = nFeet + 1;

			Construct(Feet(),Head());
			e(Feet()) = uNum;
			if( !uNum)
			{
				Fit();
			}

		};

		REAL(const REAL & R)
		{
			m_nSign = R.Sign();
			Construct( R.Feet(),R.Head());
			m_nFeet = R.Feet();
			m_nHead = R.Head();

			memcpy( static_cast<void *>( p(Feet()) ),static_cast<const void *>( R.cp(R.Feet()) ),Size()*sizeof(typeUnit));

		};

		REAL & operator = ( const REAL & R)
		{
			if( this != &R)
			{

				m_nSign = R.Sign();
				ReConstruct( R.Feet(),R.Head());
				m_nFeet = R.Feet();
				m_nHead = R.Head();

				memcpy( static_cast<void *>( p(Feet()) ),static_cast<const void *>( R.cp(R.Feet()) ),Size()*sizeof(typeUnit));
			}
			return *this;
		};

	public:
		~REAL()
		{
			delete[] m_puOrigin;
		};

	private:
		REAL& Construct( int nBottom,int nTop)
		{
			m_nBottom = nBottom;
			m_nTop = nTop;
			m_puOrigin = new typeUnit[Cap()];
			return *this;
		};
		REAL& ReConstruct( int nBottom,int nTop)
		{
			int nOldCap = Cap();

			if( nOldCap < nTop - nBottom )
			{
				m_nBottom = nBottom;
				m_nTop = nTop;
				delete[] m_puOrigin;
				m_puOrigin = new typeUnit[Cap()];
			}
			else
			{
				m_nBottom = nBottom;
				m_nTop = nBottom + nOldCap;
			}
			return *this;
		};
		REAL& LookUp( int nBlock)
		{
			if( Head() + nBlock < Top() )
			{
				return *this;
			}
			if( Size() + nBlock <= Cap() )
			{
				for( int i = 0; i< Size(); ++i)
				{
					m_puOrigin[i] = ce(Feet() + i);
				}
				int nCap = Cap();
				m_nBottom = Feet();
				m_nTop = Bottom() + nCap;
			}
			else
			{
				typeUnit * puNewMem = new typeUnit[Size() + nBlock ];
				memcpy( static_cast<void *>( puNewMem),static_cast<const void *>( cp(Feet()) ), Size()*sizeof(typeUnit));
				delete[] m_puOrigin;
				m_puOrigin = puNewMem;
				m_nBottom = Feet();
				m_nTop = Bottom() + Size() + nBlock;
			}
			return *this;

		};
		REAL& LookDown( int nBlock)
		{
			if( Feet() - nBlock >= Bottom() )
			{
				return *this;
			}
			if( Size() + nBlock <= Cap() )
			{
				for( int i = 0; i< Size(); ++i)
				{
					m_puOrigin[Cap()-1-i] = ce(Head()-1 - i);
				}
				int nCap = Cap();
				m_nTop = Head();
				m_nBottom = Top() - nCap;
			}
			else
			{
				typeUnit * puNewMem = new typeUnit[Size() + nBlock ];
				memcpy( static_cast<void *>( puNewMem + nBlock),static_cast<const void *>( cp(Feet()) ), Size()*sizeof(typeUnit));
				delete[] m_puOrigin;
				m_puOrigin = puNewMem;
				m_nTop = Head();
				m_nBottom = Feet() - nBlock;
			}
			return *this;

		};
		typeUnit & e(int nIndex)
		{
			assert( nIndex >= Bottom());
			assert( nIndex < Top());
			return *p(nIndex);
		};
		typeUnit ce(int nIndex)const
		{
			if( nIndex >= Head() || nIndex < Feet() )
			{
				return 0;
			}
			else
			{
				return *cp(nIndex);
			}

		};

	private:

		int Sign()const
		{
			return m_nSign;
		};
		int Top()const
		{
			return m_nTop;
		};
		int Bottom()const
		{
			return m_nBottom;
		};
		int Size()const
		{
			return Head() - Feet();
		};

		int Head()const
		{
			return m_nHead;
		};
		int Feet()const
		{
			return m_nFeet;
		};
		int Cap()const
		{
			return Top() - Bottom();
		};

		const typeUnit * cp(int nIndex)const
		{
			return m_puOrigin + nIndex - Bottom();
		};
		typeUnit * p(int nIndex)
		{
	//		assert( nIndex >= Bottom());
	//		assert( nIndex < Top());
			return m_puOrigin + nIndex - Bottom();
		};

	private:
		//	void TopExpand(int nBlock)
		//	{
		//		m_nTop += nBlock;
		//		typeUnit * puNewMem = new typeUnit[Cap()];
		//		memcpy( static_cast<void *>( puNewMem + Feet() - Bottom()),static_cast<const void *>( cp(Feet()) ), Size()*sizeof(typeUnit));
		//		delete[] m_puOrigin;
		//		m_puOrigin = puNewMem;
		//	};
		REAL& SetHead()
		{
			e(m_nHead++) = 1;
			return *this;
		};
		REAL& Fit()
		{
			int nF = Feet();

			while( nF < Head() && !ce(nF) )
			{
				++nF;
			}
			if( nF == Head())
			{
				Zero();
				return *this;
			}
			//如运行至此，则值不为零，而必有下限，无需判断界限
			int nH = Head();
			while( !ce(nH-1))
			{
				--nH;
			}
			m_nHead = nH;
			m_nFeet = nF;
			return *this;
		};
	public:
		friend int AbsCompare(const REAL &R1,const REAL &R2);

	public:
		bool IsZero()const
		{
			return Head() == 1 && Feet()==0  && ce(0) == 0;
		};
		bool IsInt()const
		{
			return Feet() == 0;
		};
	public:
		friend REAL& Plus( const REAL &R1,const REAL &R2,REAL &R);
		friend REAL& Subtract( const REAL &R1,const REAL &R2,REAL &R)
		{
			Plus( R1,const_cast<REAL &>(R2).Minus(),R);
			const_cast<REAL &>(R2).Minus();
			return R;
		};
		friend REAL& Multiply ( const REAL &R1,const REAL &R2,REAL &R);
		friend REAL& Reverse(const REAL &C,REAL &Rn,int nPrec,int &nRealPrec);
		friend REAL& Divide(const REAL &R1,const REAL &R2,REAL &R,int nPrec,int &nRealPrec)
		{
			int nP = R1.Lead();
			int nK = nPrec - nP;
			REAL R3;
			Reverse(R2,R3,nK,nRealPrec);
			nRealPrec += nP;
			return Multiply( R1,R3,R);

		};
		friend REAL& Reverse(const REAL &C,REAL &Rn,int &nRealPrec)
		{
			return Reverse(C,Rn,s_nPrec,nRealPrec);
		};
		friend REAL& Divide(const REAL &R1,const REAL &R2,REAL &R,int &nRealPrec)
		{
			return Reverse(R1,R,s_nPrec,nRealPrec);

		};
	public:
		static int Precision(int nPrec)
		{
			int nOldPrec = s_nPrec;
			s_nPrec = nPrec;
			return nOldPrec;
		};
		static int Precision()
		{
			return s_nPrec;
		};
		static int DisWidth( int nWidth)
		{
			if( nWidth <= 0)
			{
				nWidth = 6;
			}
			int nOldWidth = s_nDisWidth;
			s_nDisWidth = nWidth;
			return nOldWidth;
		};
		static int DisWidth()
		{
			return s_nDisWidth;
		};
		static int DisBase(char cBase)
		{
			if( cBase != 'b' && cBase != 'B' && cBase != 'h' && cBase != 'H' && cBase != 'D' && cBase != 'd')
			{
				cBase = 'D';
			}
			char cOldBase = cBase;
			s_cDisBase = cBase;
			return cOldBase;
		};
		static int DisBase()
		{
			return s_cDisBase;
		};
		static bool DisFill( bool bFill)
		{
			bool bOldFill  = s_bDisFill;
			s_bDisFill = bFill;
			return bOldFill;
		};
		static bool DisFill()
		{
			return s_bDisFill;
		};
	public:
		friend REAL operator + ( const REAL &R1,const REAL &R2)
		{
			REAL R;
			Plus( R1,R2,R);
			return R;
		};
		friend REAL operator - ( const REAL &R1,const REAL &R2)
		{
			REAL R;
			Subtract( R1,R2,R);
			return R;
		};
		friend REAL operator * ( const REAL &R1,const REAL &R2)
		{
			REAL R;
			Multiply( R1,R2,R);
			return R;
		};
		friend REAL operator / ( const REAL &R1,const REAL &R2)
		{
			REAL R;
			int nRealPrec;
			Divide( R1,R2,R,nRealPrec);
			return R;
		};
	public:
		REAL& operator += (const REAL &R)
		{
			REAL R2 = *this;
			return Plus(R2,R,*this);
		};
		REAL& operator -= (const REAL &R)
		{
			REAL R2 = *this;
			return Subtract(R2,R,*this);
		};
		REAL& operator *= (const REAL &R)
		{
			REAL R2 = *this;
			return Multiply(R2,R,*this);
		};
		REAL& operator /= (const REAL &R)
		{
			const REAL R2 = *this;
			int nRealPrec;
			return Divide(R2,R,*this,nRealPrec);
		};
	public:
		REAL operator +()const
		{
			return *this;
		};
		REAL operator -()const
		{
			REAL R;
			R = *this;
			R.Minus();
			return R;
		}

	public:
		friend bool operator == ( const REAL &R1,const REAL &R2)
		{
			return R1.Sign() == R2.Sign() && AbsCompare(R1,R2) == 0;
		};
		friend bool operator != ( const REAL &R1,const REAL &R2)
		{
			return !(R1==R2);
		};
		friend bool operator < ( const REAL &R1,const REAL &R2)
		{
			return R1.Sign() < R2.Sign() || R1.Sign() == R2.Sign() && R1.Sign()*AbsCompare(R1,R2)<0;
		};
		friend bool operator > ( const REAL &R1,const REAL &R2)
		{
			return R1.Sign() > R2.Sign() || R1.Sign() == R2.Sign() && R1.Sign()*AbsCompare(R1,R2)>0;
		};
		friend bool operator <= ( const REAL &R1,const REAL &R2)
		{
			return R1 < R2 || R1 == R2;
		};
		friend bool operator >= ( const REAL &R1,const REAL &R2)
		{
			return R1 > R2 || R1 == R2;
		};
	private:
		REAL& TruncateFeet(int nFeet)
		{
			if( nFeet >= Head() )
			{
				Zero();
			}
			else if( nFeet > Feet() )
			{
				m_nFeet = nFeet;
				Fit();
			}
			return *this;
		};
	private:
		static int PosToBlock( int nP)
		{
			int nFeet;
			if( nP < 0)
			{
				nFeet = nP/sc_nUnitBit;
				if( nP % sc_nUnitBit)
				{
					--nFeet;
				}
			}
			else
			{
				nFeet = nP /sc_nUnitBit;

			}
			return nFeet;
		}
	public:
		int Lead()const
		{
			typeUnit U = ce( Head() - 1);
			typeUnit uMask = 1 << (sc_nUnitBit - 1);
			int nBit = sc_nUnitBit - 1;
			while( !(uMask&U) && nBit>=0)
			{
				--nBit;
				U <<= 1;
			}
			return sc_nUnitBit * ( Head() - 1) + nBit + 1;
		};
		int Seat()const
		{
			if( IsZero())
			{
				return 0;
			}
			typeUnit U = ce(Feet());
			for( int i = 0;i<sc_nUnitBit;++i)
			{
				if( U & 1)
				{
					return sc_nUnitBit*Feet() + i;
				}
			}

		};
		bool Bit(int nPos)const
		{
			int nBlock = PosToBlock(nPos);
			return ce(nBlock) & ( 1<< nPos - nBlock * sc_nUnitBit) ;
		};
		REAL& Truncate(int nP)
		{
			int nFeet = PosToBlock(nP);
			TruncateFeet( nFeet );
			if( nFeet == Feet() )
			{
				int nBit = nP - nFeet*sc_nUnitBit;
				typeUnit U = 0;
				for( int i = 0; i< nBit; ++i)
				{
					U <<= 1;
					U += 1;
				}
				e( Feet()) &= ~U;
				Fit();
				return *this;
			}
			else
			{
				return *this;
			}

		};
	public:
		REAL& Zero()
		{
			m_nFeet = 0;
			m_nHead = 1;
			m_nSign = 1;
			ReConstruct(m_nFeet,m_nHead);
			e(0) = 0;
			return *this;
		};
		REAL & Minus()
		{
			m_nSign = - m_nSign;
			return *this;
		};

	public:
		friend REAL operator << ( const REAL &R1,int n)
		{
			REAL R = R1;
			R <<= n;
			return R;
		};
		friend REAL operator >> ( const REAL &R1,int n)
		{
			REAL R = R1;
			R >>= n;
			return R;
		};
	public:
		REAL & operator <<= (int nBit);
		REAL & operator >>= (int nBit);
	public:
		friend bool StringToReal( const std::string &s,REAL &R,int nPrec,char cBase);
		friend bool StringToReal( const std::string &s,REAL &R,int nPrec)
		{
			return StringToReal(s,R,nPrec,s_cDisBase);
		}
		friend bool StringToReal( const std::string &s,REAL &R)
		{
			return StringToReal(s,R,s_nPrec,s_cDisBase);
		};
		friend void StringToI(const std::string s,int nBegin,int nEnd,int nExp,REAL &R,char cBase );
		friend void StringToF(const std::string s,int nBegin,int nEnd,int nExp,int nPrec,REAL &R,char cBase);
		friend void RealToString( const REAL &R,std::string &s,int nWidth,char cBase,bool bFill);
		friend void RealToString( const REAL &R,std::string &s)
		{
			RealToString(R,s,s_nDisWidth,s_cDisBase,s_bDisFill);
		};
		friend void IToString( const REAL &R,std::string &s,int &nExp,int nSpace,char cBase);
		friend void FToString( const REAL &R,std::string &s,int &nExp,int nSpace,bool bTopZero,char cBase);

		friend std::ostream & operator << ( std::ostream &os, const REAL &R)
		{
			std::string s;
			RealToString(R,s);
			return os<<s;
		};
		friend std::istream & operator >> ( std::istream &is, REAL &R)
		{
			std::string s;
			is>>s;
			if( !StringToReal(s,R) )
			{
				is.setstate(std::ios::failbit);
			}
			return is;
		};

	private:
		static void FixDigit(std::string &s,int nDotPos,int nWidth,bool bFill)
		{
			bool bDot = false;

			if( nWidth <= 0 || s.empty())
			{
				s.clear();
				return;
			}
			else
			{
				if( nDotPos <= 0)
				{
					s = "0." + std::string(0-nDotPos,'0') + s;
					bDot = true;
				}
				else if( nDotPos >= 1 && nDotPos < s.size() )
				{
					s.insert(nDotPos,".");
					bDot = true;
				}
				else if( nDotPos == s.size() )
				{
					// do nothing;
				}
				else
				{
					s += std::string(nDotPos-s.size(),'0');
				}
			}
			if( bFill)
			{
				int nDigitSize = s.size();
				if( bDot)
				{
					--nDigitSize;
				}
				if( nDigitSize < nWidth)
				{
					if( !bDot)
					{
						s += ".";
					}
					s += std::string(nWidth-nDigitSize,'0');
				}
			}
		};
		static void FloatDigit(std::string &s,int nDotPos,int nWidth,char cBase,bool bFill)
		{
			if( nWidth <= 0 || s.empty())
			{
				s.clear();
				return;
			}
			bool bDot = false;
			int nExp = nDotPos - 1;
			if( s.size() != 1)
			{
				s.insert(1,".");
				bDot = true;
			}

			if( bFill)
			{
				int nDigitSize = s.size();
				if( bDot)
				{
					--nDigitSize;
				}
				if( nDigitSize < nWidth)
				{
					if( !bDot)
					{
						s += ".";
					}
					s += std::string(nWidth-nDigitSize,'0');
				}
			}
			s = s + "e" + REAL::IntToString(nExp,cBase);

		};



		static void RoundDigit(std::string &s,int &nExp,char cBase)
		{
			if( s.empty())
			{
				return;
			}
			else
			{
				int bCarry = false;
				if( DigitToInt( s.at( s.size() - 1) ) >= Base(cBase)/2)
				{
					bCarry = true;
				}
				s[ s.size() - 1] = '0';
				int nPos = s.size() - 2;
				while( bCarry && nPos >= 0)
				{
					if( DigitToInt(s[nPos]) == Base(cBase) - 1)
					{
						s[nPos] = '0';
						bCarry = true;
					}
					else
					{
						s[nPos] = IntToDigit(DigitToInt(s[nPos]) + 1);
						bCarry = false;
					}
					--nPos;
				}
				if( bCarry)
				{
					s.insert(0,"1");
				}

			}
			int nPos = s.find_last_not_of("0");
			nExp += s.size() - (nPos+1);
			s.erase(nPos + 1);		
		};
		static int  StringToInt(const std::string &s,int nBegin,int nEnd,char cBase)
		{
			for( int i = nBegin;i<nEnd;++i)
			{
				if( s.at(i) == '0')
				{
					++nBegin;
				}
				else
				{
					break;
				}
			}
			int nSum = 0;
			for( int i = nBegin;i<nEnd;++i)
			{
				nSum = Base(cBase)*nSum + DigitToInt(s.at(i));
			}
			return nSum;
		};
		static std::string IntToString( int n,char cBase)
		{
			std::string s;
			if( !n)
			{
				return std::string("0");
			}
			int nSign = 1;
			if( n < 0)
			{
				nSign = -1;
				n = -n;
			}
			while( n)
			{
				s += IntToDigit(n%Base(cBase));
				n /= Base(cBase);
			}
			if( n == -1)
			{
				s += '-';
			}
			std::reverse(s.begin(),s.end());
			return s;
		};
	private:
		static int Base(char cBase)
		{
			switch( cBase)
			{
			case 'D':case 'd':
				return 10;
			case 'H':case 'h':
				return 16;
			case 'b':case 'B':
				return 2;
			default:
				return 0;
			}
		};

		static int DigitToInt( char c)
		{

			if( c >= '0' && c<= '9')
			{
				return c - '0';
			}
			else if( c >= 'A' && c<= 'F')
			{
				return 10 + c - 'A';
			}
			else
			{
				return 0;
			}
		};

		static char IntToDigit( int n)
		{
			int s[] = 
			{
				'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'
			};
			assert( n >= 0 && n <16);
			return s[n];
		};

		static bool IsDigit(char c,char cBase)
		{
			switch( cBase)
			{
			case 'D':case 'd':
				return '0' <= c && c <= '9';
			case 'H':case 'h':
				return '0'<= c && c <= '9' ||  'A'<= c && c <= 'F';
			case 'b':case 'B':
				return c == '0' || c == '1';
			}
			return false;

		};
		static const REAL & TenExp(int nExp)
		{

			static REAL RTenExp = 1;
			static int s_nExp = 0;
			const REAL Ten = 10;

			if( s_nExp <= nExp)
			{
				for( int i = s_nExp; i<nExp;++i)
				{
					Multiply( TMP = RTenExp,Ten,RTenExp);
				}
				s_nExp = nExp;
				return RTenExp;
			}

			else
			{
				RTenExp = 1;
				for( int i = 0;i<nExp;++i)
				{
					Multiply(TMP = RTenExp,Ten,RTenExp);
				}
				s_nExp = nExp;
				return RTenExp;
			}
		};
		static void Tenth(REAL &V1,int nPrec)
		{

			static int s_nRealPrec;
			static int s_bFirst = true;
			const REAL Ten = 10;
			static REAL s_V1;

			if( s_bFirst)
			{
				Reverse(Ten,s_V1,nPrec,s_nRealPrec);
				V1 = s_V1;
				s_bFirst = false;
			}
			else
			{
				if( s_nRealPrec == nPrec )
				{
					V1 = s_V1;
				}
				else if ( s_nRealPrec < nPrec )
				{
					V1 = s_V1;
					V1.Truncate(nPrec-1);
				}
				else
				{
					Reverse(Ten,s_V1,nPrec,s_nRealPrec);
					V1 = s_V1;
				}
			}

		};

	private:
		void AddLink(typeLink L, int nPos)
		{

			//只函数只在乘法中使用，故已确定范围，位数能满足需求。
			typeLink LSum = 0;
			typeLink L1 = ce(nPos);
			typeLink L2 = static_cast<typeLink>( static_cast<typeUnit>(L) );
			LSum = L1 + L2;
			e(nPos) = static_cast<typeUnit>( LSum);

			L1 = ce(nPos+1);
			L2 = static_cast<typeLink>( static_cast<typeUnit>(L>>sc_nUnitBit) );
			LSum = L1 + L2 + (LSum>>sc_nUnitBit);
			e(nPos+1) = static_cast<typeUnit>(LSum);

			for( int i = nPos + 2; i < Head(); ++i)
			{
				if( LSum >> sc_nUnitBit)
				{
					L1 = ce(i);
					LSum = L1 + (LSum>>sc_nUnitBit);
					e(i) = static_cast<typeUnit>(LSum);
				}
				else
				{
					break;
				}
			}
			//已确定范围，不会进位
			//暂时不调整Head与Feet

		};

	};

}
#endif 
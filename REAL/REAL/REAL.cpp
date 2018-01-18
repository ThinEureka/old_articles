//REAL.cpp

#include "REAL.h"
namespace zju04nycs
{

	int REAL::s_nPrec = - sc_nUnitBit * 120;
	int REAL::s_nDisWidth = 6;
	char REAL::s_cDisBase = 'D';
	bool REAL::s_bDisFill = false;

	REAL REAL::PLUS;	REAL REAL::SUB;		REAL REAL::MUL;		REAL REAL::DIV;		REAL REAL::TMP;
	REAL REAL::PLUS1;	REAL REAL::SUB1;	REAL REAL::MUL1;	REAL REAL::DIV1;	REAL REAL::TMP1;
	REAL REAL::PLUS2;	REAL REAL::SUB2;	REAL REAL::MUL2;	REAL REAL::DIV2;	REAL REAL::TMP2;

	REAL& Plus ( const REAL &R1,const REAL &R2,REAL &R)
	{

		if( R1.IsZero())
		{
			return R = R2;
		}
		else if(R2.IsZero())
		{
			return R = R1;
		}
		else
		{
			const REAL *pHead = &R1, *pHigh = &R2;
			const REAL *pFeet = &R1, *pLow = &R2;

			//Feet < Low ; High < Head
			if( R1.Head() < R2.Head() )
			{
				std::swap(pHead,pHigh);
			}
			if( R1.Feet() > R2.Feet() )
			{
				std::swap(pFeet,pLow);
			}
			////nFeet <= nLow <= nHigh <= nHead
			const int nHead = pHead->Head();
			const int nFeet = pFeet->Feet();
			const int nLow = std::min( pHigh->Head(),pLow->Feet());
			const int nHigh = std::max( pHigh->Head(),pLow->Feet());

			if( ( R1.Sign() == 1)&&( R2.Sign() == 1) || (R1.Sign() == -1)&&( R2.Sign() == -1) )
			{


				R.m_nSign = R1.Sign();
				R.ReConstruct(nFeet,nHead + 1);
				R.m_nFeet = nFeet;
				R.m_nHead = nHead;

				//非公共位
				memcpy( static_cast<void *>( R.p(nHigh) ), 
					static_cast<const void *>( pHead->cp(nHigh) ),
					(nHead - nHigh )*sizeof(REAL::typeUnit) );

				memcpy( static_cast<void *>( R.p(nFeet) ), 
					static_cast<const void *>( pFeet->cp(nFeet) ),
					(nLow - nFeet )*sizeof(REAL::typeUnit) );
				memset( static_cast<void *>( R.p(nLow)),0,(pLow->Feet() - nLow)*sizeof(REAL::typeUnit));

				//公共位，区别于上面的nFeet与nHigh，在错位（pLow->Feet() >＝ pHigh->Head()）
				//的情况下，下面的代码就不执行。
				REAL::typeLink L = 0,L1,L2;
				for( int i = pLow->Feet(); i < pHigh->Head(); ++i)
				{
					L1 = R1.ce(i);
					L2 = R2.ce(i);
					L = L1 + L2 + (L>>REAL::sc_nUnitBit);
					R.e(i) = static_cast<REAL::typeUnit>(L);
				}

				//高非公位进位
				for( int i = pHigh->Head(); i < nHead; ++i)
				{
					if( L>>REAL::sc_nUnitBit)
					{
						L1 = R.ce(i);
						L = L1 + (L>>REAL::sc_nUnitBit);
						R.e(i) = static_cast<REAL::typeUnit>(L);
					}
					else
					{
						break;
					}
				}

				//最高进位
				if( L>>REAL::sc_nUnitBit)
				{
					R.SetHead();
				}


			}

			else
			{

				const REAL *pR1 = &R1, *pR2 = &R2;

				if( AbsCompare( R1,R2) < 0)
				{
					std::swap(pR1,pR2);
				}

				R.m_nSign = pR1->Sign();
				R.ReConstruct(nFeet,nHead);
				R.m_nFeet = nFeet;
				R.m_nHead = nHead;

				//两值相减，复制保持不变的位
				memcpy( static_cast<void *>( R.p(nHigh) ), 
					static_cast<const void *>( pR1->cp(nHigh) ),
					(nHead - nHigh )*sizeof(REAL::typeUnit) );
				memcpy( static_cast<void *>( R.p(pFeet->Feet()) ), 
					static_cast<const void *>( pR1->cp(pR1->Feet()) ),
					(pR2->Feet() - nFeet )*sizeof(REAL::typeUnit) );
				memset( static_cast<void *>( R.p(nLow)),0,(pLow->Feet() - nLow)*sizeof(REAL::typeUnit));


				//pR2（绝对值小者）的每一位都要参与运算
				REAL::typeLink L,L1,L2;
				L = 1;
				L <<= REAL::sc_nUnitBit;
				const REAL::typeLink LC = L;

				for( int i = pR2->Feet(); i < pR2->Head(); ++i)
				{
					L1 = pR1->ce(i);
					L2 = pR2->ce(i);
					L = LC + L1 - L2 - ( static_cast<REAL::typeLink>(1) - (L>>REAL::sc_nUnitBit) );
					R.e(i) = static_cast<REAL::typeUnit>(L);
				}

				//高非公共位
				for( int i = nHigh; i < pR1->Head(); ++i)
				{
					if( !(L >> REAL::sc_nUnitBit) )
					{
						L1 = pR1->ce(i);
						L = LC + L1 - ( static_cast<REAL::typeLink>(1) - (L>>REAL::sc_nUnitBit));
						R.e(i) = static_cast<REAL::typeUnit>(L);
					}
					else
					{
						break;
					}
				}

				//绝对值已判，够减，再无借位

			}
			return R.Fit();
		}

	}


	int AbsCompare(const REAL &R1,const REAL &R2)
	{
		if( R1.IsZero() )
		{
			if( R2.IsZero() )
			{
				return 0;
			}
			else
			{
				return -1;
			}
		}

		if( R2.IsZero() )
		{
			return 1;
		}

		else
		{

			int nHead = std::max( R1.Head(),R2.Head());
			int nFeet = std::min( R1.Feet(),R2.Feet());
			for( int i = nHead - 1; i>=nFeet; --i)
			{
				if( R1.ce(i) > R2.ce(i) )
				{
					return 1;
				}
				else if( R1.ce(i) < R2.ce(i) )
				{
					return -1;
				}
			}
			return 0;
		}


	}


	REAL& Multiply ( const REAL &R1,const REAL &R2,REAL &R)
	{
		if( R1.IsZero() || R2.IsZero() )
		{
			return R.Zero();
		}
		else
		{
			R.m_nSign = (R1.Sign() == R2.Sign() ? 1:-1);
			R.ReConstruct( R1.Feet() + R2.Feet(), R1.Head() + R2.Head());
			R.m_nFeet = R1.Feet() + R2.Feet();
			R.m_nHead = R1.Head() + R2.Head();
			memset( static_cast<void*>( R.p( R.Feet())), 0,R.Size()*sizeof(REAL::typeUnit));

			REAL::typeLink L1,L2,L;

			for( int i = R1.Feet(); i<R1.Head(); ++i)
			{
				L1 = R1.ce(i);
				for( int j = R2.Feet(); j < R2.Head(); ++j)
				{
					L2 = R2.ce(j);
					L = L1*L2;
					R.AddLink(L,i+j);
				}
			}

			R.Fit();
		}
		return R;
	}
	//以下代码留作比较查看
	/*

	REAL& Reverse(const REAL &C,REAL &Rn,int nPrec,int &nRealPrec)
	{


	bool bMinus = false;
	if( C.Sign() == -1 )
	{
	const_cast< REAL & >(C).Minus();
	bMinus = true;
	}
	int nK = std::min( nPrec, -C.Lead()-1);
	nRealPrec = nK;
	int nFeet = REAL::PosToBlock(nK - 1);

	const REAL U1 = 1;
	const REAL U2 = 2;
	const REAL U2_e_k = U1 << nK;
	static REAL CU2_e_k;
	Multiply( C,U2_e_k,CU2_e_k);

	Rn = U1;
	Rn <<= (nK + 1);

	static REAL NCRn;
	Multiply( C,Rn,NCRn).Minus();

	static REAL U1_p_NCRn;
	static REAL U2_p_NCRn;
	static REAL Rn_p_1;
	// 1 - CRn = 1 + (-CRn)
	Plus( U1, NCRn, U1_p_NCRn);

	// 1 - C*Rn >= C*2^k 
	while( AbsCompare( U1_p_NCRn,CU2_e_k ) >= 0)
	{
	// 2 - C*Rn = 2 + ( - C*Rn)
	Plus( U2,NCRn,U2_p_NCRn);
	// R(n+1) = Rn*( 2 - CRn) = Rn*( 2 + (-CRn) )
	Multiply( Rn,U2_p_NCRn,Rn_p_1);
	//为减少内存操作，用Rn_p_1即(R(n+1)来截断是合适的
	Rn_p_1.TruncateFeet( nFeet);
	Rn = Rn_p_1;

	Multiply( C,Rn,NCRn).Minus();
	// 1 - CRn = 1 + (-CRn)
	Plus( U1, NCRn, U1_p_NCRn);

	}
	if( bMinus )
	{
	const_cast< REAL & >(C).Minus();
	Rn.Minus();
	}
	//Head() 与 Feet() 已经 Fit
	return R;

	}


	*/
	REAL& Reverse(const REAL &C,REAL &R,int nPrec,int &nRealPrec)
	{


		bool bMinus = false;
		if( C.Sign() == -1 )
		{
			const_cast< REAL & >(C).Minus();
			bMinus = true;
		}
		//C!=0，所以C.Lead()计算正常，为最高的非零bit+1。
		int nK = std::min( nPrec, -C.Lead()-1);
		nRealPrec = nK;
		int nFeet = REAL::PosToBlock(nK - 1);

		const REAL One = 1;
		REAL C2e_k = C;
		C2e_k <<= nK;

		static REAL SUB;
		static REAL MUL;
		static REAL PLUS;

		R = One;
		R <<= -C.Lead();
		

		while( AbsCompare( Subtract(One,Multiply(C,R,MUL),SUB),C2e_k ) >= 0)
		{
			// x(2-cx) = x( (1-cx) + 1)
			R = Multiply( R,Plus( SUB,One,PLUS),MUL).TruncateFeet(nFeet);

		}
		if( bMinus )
		{
			const_cast< REAL & >(C).Minus();
			R.Minus();
		}
		return R;

	}



	REAL & REAL::operator <<= (int nBit)
	{

		if ( nBit > 0)
		{
			int nBlock = nBit / sc_nUnitBit;
			int nTip = nBit - nBlock * sc_nUnitBit;
			m_nFeet += nBlock;
			m_nHead += nBlock;
			m_nBottom += nBlock;
			m_nTop += nBlock;

			if( !nTip)
			{
				return *this;
			}
			else
			{
				typeLink L = ce( Head() - 1);
				L <<= nTip;
				int nBlock;
				if( L >> sc_nUnitBit)
				{
					LookUp(1);
					++m_nHead;
					e(Head()-1) = static_cast<typeUnit>(L >> sc_nUnitBit);
					e( Head() - 2) = static_cast<typeUnit>(L);
					nBlock = Head() - 3;
				}
				else
				{
					e(Head()-1) = static_cast<typeUnit>(L);
					nBlock = Head() - 2;
				}
				for( int i = nBlock; i>= Feet(); --i)
				{
					L = ce(i);
					L <<= nTip;

					e(i+1) |= static_cast<typeUnit>( L>>sc_nUnitBit);
					e(i) = static_cast<typeUnit>(L);

				}
				Fit();
				return *this;

			}
		}
		else if( nBit < 0)
		{
			return *this >>= -nBit;
		}
		else
		{
			return *this;
		}
	}

	REAL & REAL::operator >>= (int nBit)
	{
		if ( nBit > 0)
		{
			int nBlock = nBit / sc_nUnitBit;
			int nTip = nBit - nBlock * sc_nUnitBit;
			m_nFeet -= nBlock;
			m_nHead -= nBlock;
			m_nBottom -= nBlock;
			m_nTop -= nBlock;

			if( !nTip)
			{
				return *this;
			}
			else
			{
				typeLink L = ce(Feet());
				L <<= sc_nUnitBit;
				L >>= nTip;
				int nBlock;
				if( static_cast<typeUnit>(L) )
				{
					LookDown(1);
					--m_nFeet;
					e(Feet()) = static_cast<typeUnit>(L);
					e(Feet()+1) = static_cast<typeUnit>(L>>sc_nUnitBit);
					nBlock = Feet() + 2;
				}
				else
				{
					e(Feet()) = static_cast<typeUnit>(L>>sc_nUnitBit);
					nBlock = Feet() + 1;
				}
				for( int i = nBlock; i< Head(); ++i)
				{
					L = ce(i);
					L <<= sc_nUnitBit;
					L >>= nTip;

					e(i-1) |= static_cast<typeUnit>(L);
					e(i) = static_cast<typeUnit>(L>>sc_nUnitBit);
				}
				Fit();
				return *this;

			}
		}
		else if( nBit < 0)
		{
			return *this <<= -nBit;
		}
		else
		{
			return *this;
		}
	}


	bool  StringToReal( const std::string &s,REAL &R,int nPrec,char cBase)
	{
		enum { begin,sign,first_int,prefix_dot,infix_dot,second_int,exp,
			sign_exp,third_int,error}status = begin;

		int nSign = 1;
		int nSignExp = 1;
		int nFirstIntBegin = 0;
		int nFirstIntEnd = 0;
		int nSecondIntBegin = 0;
		int nSecondIntEnd = 0;
		int nThirdIntBegin = 0;
		int nThirdIntEnd = 0;

		for(int i = 0;i<s.size()&&status!=error;++i)
		{
			switch( status)
			{
			case begin:
				if(s.at(i)=='-')
				{
					nSign = -1;
					status = sign;
				}		
				else if( s.at(i) == '+')
				{
					status = sign;
				}
				else if( REAL::IsDigit(s.at(i),cBase))
				{
					nFirstIntBegin = i;
					nFirstIntEnd = i+1;
					status = first_int;
				}
				else if( s.at(i) == '.')
				{
					status = prefix_dot;
				}
				else
				{
					status = error;
				}
				break;

			case sign:
				if(REAL::IsDigit(s.at(i),cBase))
				{
					nFirstIntBegin = i;
					nFirstIntEnd = i+1;
					status = first_int;
				}
				else if(s.at(i) == '.')
				{
					status = prefix_dot;
				}
				else
				{
					status = error;
				}
				break;

			case first_int:
				if(REAL::IsDigit(s.at(i),cBase))
				{
					++nFirstIntEnd;
					//状态不变
				}
				else if(s.at(i) == '.')
				{
					status = infix_dot;
				}
				else if(s.at(i) == 'e')
				{
					status = exp;
				}
				else
				{
					status = error;
				}
				break;
			case prefix_dot:
				if( REAL::IsDigit(s.at(i),cBase))
				{
					nSecondIntBegin = i;
					nSecondIntEnd = i+1;
					status = second_int;
				}
				else
				{
					status = error;
				}
				break;
			case infix_dot:
				if( REAL::IsDigit(s.at(i),cBase))
				{
					nSecondIntBegin = i;
					nSecondIntEnd = i+1;
					status = second_int;
				}
				else if(s.at(i) == 'e')
				{
					status = exp;
				}
				else
				{
					status = error;
				}
				break;
			case second_int:
				if( REAL::IsDigit(s.at(i),cBase))
				{
					++nSecondIntEnd;
					//状态不变
				}
				else if(s.at(i) == 'e')
				{
					status = exp;
				}
				else
				{
					status = error;
				}
				break;
			case exp:
				if( s.at(i) == '+')
				{
					status = sign_exp;
				}
				else if( s.at(i) == '-')
				{
					nSignExp = -1;
					status = sign_exp;
				}
				else if( REAL::IsDigit(s.at(i),cBase))
				{
					nThirdIntBegin = i;
					nThirdIntEnd = i+1;
					status = third_int;
				}
				else
				{
					status = error;
				}
				break;
			case sign_exp:
				if( REAL::IsDigit(s.at(i),cBase))
				{
					nThirdIntBegin = i;
					nThirdIntEnd = i+1;
					status = third_int;
				}
				else
				{
					status = error;
				}
				break;
			case third_int:
				if( REAL::IsDigit(s.at(i),cBase))
				{
					++nThirdIntEnd;
					//状态不变
				}
				else
				{
					status = error;
				}
				break;
			default:
				status = error;

			}
		}

		switch( status)
		{
		case first_int:
		case infix_dot:
		case second_int:
		case third_int:
			break;
		default:
			return false;
		}
		int nExp = REAL::StringToInt(s,nThirdIntBegin,nThirdIntEnd,cBase);

		if( nSignExp == -1)
		{
			nExp = -nExp;
		}

		static REAL Left,Right,S1;

		StringToI(s,nSecondIntBegin,nSecondIntEnd,nExp-(nSecondIntEnd-nSecondIntBegin),Right,cBase);
		StringToI(s,nFirstIntBegin,nFirstIntEnd,nExp,Left,cBase);

		//这个函数之后要跨跃两个函数，所以不能使用PLUS变量
		Plus(Left,Right,S1);

		StringToF(s,nFirstIntBegin,nFirstIntEnd,nExp+(nFirstIntEnd-nFirstIntBegin),nPrec-1,Right,cBase);
		StringToF(s,nSecondIntBegin,nSecondIntEnd,nExp,nPrec-1,Left,cBase);

		//此处可以使用PLUS变量
		Plus(S1,Plus(Left,Right,REAL::PLUS),R);
		if( !R.IsZero())
		{
			R.m_nSign = nSign;
		}
		return true;

	}
	void StringToI(const std::string s,int nBegin,int nEnd,int nExp,REAL &R,char cBase)
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
		for( int i = nEnd - 1;i>=nBegin;--i)
		{
			if( s.at(i) == '0')
			{
				--nEnd;
				++nExp;
			}
			else
			{
				break;
			}
		}
		if( nExp < 0)
		{
			nEnd += nExp;
			nExp = 0;
		}
		if( nEnd <= nBegin)
		{
			R.Zero();
			return;
		}
		switch(cBase)
		{
		case 'D':case 'd':
			{
				const REAL Ten = 10;
				static REAL RT;
				RT.Zero();
				for( int i = nBegin;i<nEnd;++i)
				{
					Plus( Multiply(RT,Ten,REAL::MUL),REAL::DigitToInt(s.at(i)),RT);
				}

				Multiply(RT,REAL::TenExp(nExp),R);
			}
			return;
		case 'b':case 'B':
			{
				int nCap = (nEnd-nBegin)/REAL::sc_nUnitBit;
				if( nEnd - nBegin != nCap * REAL::sc_nUnitBit)
				{
					++nCap;
				}
				R.ReConstruct(0,nCap);
				R.m_nSign = 1;
				R.m_nFeet = 0;
				R.m_nHead = nCap;

				int nPhase = 0;
				int nBlock = 0;
				R.e(0) = 0;

				for( int i = nEnd - 1;i>=nBegin;--i)
				{
					R.e(nBlock) |= REAL::DigitToInt(s.at(i))<<nPhase;
					++nPhase;
					if( nPhase == REAL::sc_nUnitBit)
					{
						nPhase = 0;
						++nBlock;
						R.e(nBlock) = 0;
					}
				}
				//已砍头去尾，无需fit
				//		R.Fit();
				R <<= nExp;

			}
			return;
		case 'H':case 'h':
			{
				int nCap = (nEnd-nBegin)/(REAL::sc_nUnitBit/4);
				if( nEnd - nBegin != nCap*(REAL::sc_nUnitBit/4))
				{
					++nCap;
				}
				R.ReConstruct(0,nCap);
				R.m_nSign = 1;
				R.m_nFeet  = 0;
				R.m_nHead = nCap;
				R.e(0) = 0;

				int nPhase = 0;
				int nBlock = 0;
				for( int i = nEnd - 1;i >= nBegin;--i)
				{
					R.e(nBlock) |= REAL::DigitToInt(s.at(i)) << nPhase;
					nPhase += 4;
					if( nPhase == REAL::sc_nUnitBit)
					{
						nPhase = 0;
						++nBlock;
						R.e(nBlock) = 0;
					}
				}
				//已砍头去尾，无需fit
				//		R.Fit();
			}
			R <<= 4*nExp;
			return;
		default:
			R.Zero();
			return;
		}
	}
	void StringToF(const std::string s,int nBegin,int nEnd,int nExp,int nPrec,REAL &R,char cBase)
	{

		for( int i = nEnd - 1;i>=nBegin;--i)
		{
			if( s.at(i) == '0')
			{
				--nEnd;
			}
			else
			{
				break;
			}
		}
		for( int i = nBegin;i<nEnd;++i)
		{
			if( s.at(i) == '0')
			{
				++nBegin;
				--nExp;
			}
			else
			{
				break;
			}
		}
		if( nExp > 0)
		{
			nBegin += nExp;
			nExp = 0;
		}
		if( nBegin >= nEnd)
		{
			R.Zero();
			return;
		}
		switch(cBase)
		{
		case 'D':case 'd':
			{
				static REAL V1,Vk;
				const REAL One = 1;
				//用V1作为0.1的近似值
				// 0 < 0.1 - V1 < e1 = e
				// 0< VnV1 - V(n+1) < e
				// 从而 0.1^(n+1) - V(n+1) = 0.1^(n+1) - VnV1 + VnV1 - V(n+1)
				// < e + 0.1^(n+1)  - (0.1^n-e(n))(0.1 - e1)
				// = e + 0.1^n*e1 + 0.1e(n) - e1e(n)
				// < e + 0.1^n*e1 + 0.1e(n)
				//从而e2 < 1.2e,e3 < 1.13e，用数学归纳法得e(n) < 1.2e
				//从而总误差 D < (nEnd - nBegin)*9 * 1.2e
				//要使  D < 2^nPrec，那么 只要 (nEnd - nBegin)*9 * 1.2e < 2^nPrec
				//那么只要 e < 2^t ，其中 t = nPrec - 4 - [(nEnd-nBegin)的最高非零bit+1]
				//即 t = nPrec - 4 - (nEnd-nBegin).Lead()
				// nEnd - nBegin != 0，所以Lead计算正常
				//再利用算出的精度位t，代入函数TenthExp计算出V1与Vk，k是非零的最高位
				//在这里就是nExp - 1，当nExp = 0时，就是正常的-1位。
				int t = nPrec - 4 - REAL(nEnd-nBegin).Lead();
				REAL::Tenth(V1,t);
				Vk = V1;
				int dump;
				Reverse( REAL::TenExp(-nExp),Vk,t,dump);
				Multiply( REAL::TMP = Vk,V1,Vk).TruncateFeet(t-1);		
				R.Zero();
				for( int i = nBegin;i<nEnd;++i)
				{
					Plus(REAL::TMP=R,Multiply(Vk,REAL::DigitToInt(s.at(i)),REAL::MUL),R);
					Vk = Multiply(Vk,V1,REAL::MUL).TruncateFeet( REAL::PosToBlock(t-1));
				}
			}
			return;
		case 'b':case 'B':
			{
				int nCap = (nEnd-nBegin)/REAL::sc_nUnitBit;
				if( nEnd-nBegin != nCap*REAL::sc_nUnitBit)
				{
					++nCap;
				}
				R.ReConstruct(-nCap,0);
				R.m_nSign  = 1;
				R.m_nFeet = -nCap;
				R.m_nHead = 0;
				int nPhase = REAL::sc_nUnitBit - 1;
				int nBlock = -1;
				R.e(nBlock) = 0;

				for( int i= nBegin;i<nEnd;++i)
				{
					R.e(nBlock) |= REAL::DigitToInt(s.at(i)) << nPhase;
					--nPhase;
					if( nPhase < 0)
					{
						nPhase = REAL::sc_nUnitBit - 1;
						--nBlock;
						R.e(nBlock) = 0;
					}
				}
			}
			//已砍头去尾，无需fit
			//		R.Fit();
			R <<= nExp;
			return;
		case 'h':case 'H':
			{
				int nCap = (nEnd-nBegin)/(REAL::sc_nUnitBit/4);
				if( nEnd - nBegin != nCap * (REAL::sc_nUnitBit/4) )
				{
					++nCap;
				}
				R.ReConstruct(-nCap,0);
				R.m_nSign = 1;
				R.m_nFeet = -nCap;
				R.m_nHead = 0;

				int nBlock = -1;
				int nPhase = REAL::sc_nUnitBit - 4;
				R.e(-1) = 0;

				for( int i = nBegin;i<nEnd;++i)
				{
					R.e(nBlock) |= REAL::DigitToInt(s.at(i)) << nPhase;
					nPhase -= 4;
					if( nPhase < 0)
					{
						nPhase = REAL::sc_nUnitBit - 4;
						--nBlock;
						R.e(nBlock) = 0;
					}
				}
			}
			//已砍头去尾，无需fit
			//		R.Fit();
			R <<= 4*nExp;
			return;
		default:
			R.Zero();
			return;
		}

	}

	void IToString( const REAL &R,std::string &s,int &nHeight,int nSpace,char cBase)
	{
		s.clear();
		if( nSpace <= 0)
		{
			nHeight = 0;
			return;
		}
		static REAL I;
		const REAL Ten = 10;
		I = R;
		I.m_nSign = 1;
		I.TruncateFeet(0);
		if( I.IsZero())
		{
			nHeight = 0;
			return;
		}
		switch(cBase)
		{
		case 'd':case 'D':
			{
				int nLead = I.Lead();
				s.resize(nLead/3+1);
				int nLen = 1;
				int nBlock = I.Head() - 1;
				REAL::typeUnit uMask = 1 << nLead - 1 - REAL::PosToBlock(nLead-1) * REAL::sc_nUnitBit;
				for( int i = nLead- 1;i>= 0;--i)
				{
					REAL::typeUnit uE = I.ce(nBlock) & uMask;
					uMask >>= 1;
					if( !uMask )
					{
						--nBlock;
						uMask = 1 << REAL::sc_nUnitBit - 1;
					}
					for( int j = 0;j<nLen;++j)
					{
						s[j] *= 2;
					}
					if( uE)
					{
						s[0] += 1;
					}
					int nCarry = 0;
					for( int j = 0;j<nLen;++j)
					{
						s[j] += nCarry;
						if( s[j] >= 10)
						{
							nCarry = s[j]/10;
							s[j] -= nCarry*10;
						}
						else
						{
							nCarry = 0;
						}
					}
					if( nCarry)
					{
						s[nLen++] = nCarry;
					}
				}
				s.erase(nLen);
				if( s.size() > nSpace )
				{
					s.erase(0,s.size() - nSpace);
				}
				for( int i = 0;i< s.size();++i)
				{
					s[i] += '0';
				}
				s.erase( 0,s.find_first_not_of("0") );
				std::reverse(s.begin(),s.end());
				nHeight = nLen - s.size();
			}break;
		case 'b':case 'B':
			{
				int nLead = I.Lead();
				int nSize = std::min(nLead,nSpace);
				for( int i = 0;i<nSize;++i)
				{
					s += REAL::IntToDigit( I.Bit(nLead-1-i));
				}
				nHeight = nLead - nSize;
			}
			break;
		case 'h':case 'H':
			{
				int nBlock = I.Head() - 1;
				int nPhase = REAL::sc_nUnitBit  - 4;
				int nPad = REAL::sc_nUnitBit/4 - 1;
				for( int i = 0; i< nSpace + nPad; ++i)
				{
					REAL::typeUnit uE = I.ce(nBlock) & (0xF << nPhase);
					s += REAL::IntToDigit(uE>>nPhase);
					nPhase -= 4;
					if( nPhase < 0)
					{
						nPhase = REAL::sc_nUnitBit  - 4;
						--nBlock;
					}
				}
				int nIndex = s.find_first_not_of("0");
				s.erase(0,nIndex);
				if( s.size() > nSpace)
				{
					s.erase(nSpace);
				}
				nHeight = I.Head() *(REAL::sc_nUnitBit/4) - nIndex;
				if( nHeight < nSpace)
				{
					s.erase(nHeight);
				}
				nHeight -= s.size();
			}
			break;
		default:
			break;
		}
	}


	void FToString( const REAL &R,std::string &s,int &nDepth,int nSpace,bool bTopZero,char cBase)
	{
		static REAL F;
		const REAL Ten = 10;
		s.clear();
		if(nSpace <= 0 || R.Feet() >= 0)
		{
			nDepth = 0;
			return;
		}
		int nFeet = R.Feet();
		int nHead = std::min(0,R.Head());
		F.ReConstruct(F.m_nFeet = nFeet,F.m_nHead = nHead);
		F.m_nSign = 1;
		memcpy( static_cast<void *>( F.p(nFeet) ),
			static_cast<const void *>( R.cp(nFeet) ),F.Size()*sizeof(REAL::typeUnit));
		F.Fit();

		switch( cBase)
		{
		case 'd':case 'D':
			{
				nDepth = 0;
				while(s.size() < nSpace && !F.IsZero())
				{
					Multiply(F,Ten,REAL::MUL);
					F = REAL::MUL;
					int nTip = REAL::MUL.ce(0);
					if( bTopZero)
					{
						if( nTip)
						{
							s += REAL::IntToDigit(nTip);
							bTopZero = false;
							F.e(0) = 0;
							F.Fit();
						}
						else
						{
							--nDepth;
						}				
					}
					else
					{
						s += REAL::IntToDigit(nTip);
						if( nTip)
						{
							F.e(0) = 0;
							F.Fit();
						}
					}
				}
				if(s.empty() )
				{
					nDepth = 0;
				}	
				//剔掉零
				s.erase(s.find_last_not_of("0") + 1);
			}break;
		case 'b':case 'B':
			{
				int nTopBit;
				if( bTopZero)
				{
					nDepth = nTopBit = F.Lead();
				}
				else
				{
					nDepth = nTopBit = 0;
				}
				for( int i = 0;i<nSpace;++i)
				{
					s += REAL::IntToDigit(F.Bit(nTopBit - 1 - i));
				}
				s.erase(s.find_last_not_of("0") + 1);
			}break;
		case 'h':case 'H':
			{
				int nBlock;
				if( bTopZero )
				{
					nBlock = F.Head() - 1;
				}
				else
				{
					nBlock = - 1;
					nDepth = 0;
				}
				int nPhase = REAL::sc_nUnitBit - 4;
				int nPad = REAL::sc_nUnitBit/4 - 1;
				for( int i = 0;i<nSpace + nPad;++i)
				{
					REAL::typeUnit uE = F.ce(nBlock) & (0xF << nPhase);
					s += REAL::IntToDigit(uE >> nPhase);
					nPhase -= 4;
					if( nPhase < 0)
					{
						nPhase = REAL::sc_nUnitBit - 4;
						--nBlock;
					}
				}
				int nIndex = s.find_first_not_of("0");
				if( bTopZero)
				{
					nDepth = F.Head() * REAL::sc_nUnitBit/4 - nIndex;
					s.erase(0,nIndex);
				}
				s.erase(s.find_last_not_of("0") + 1);
				if( s.size() > nSpace)
				{
					s.erase(nSpace);
				}
			}break;
		default:
			break;
		}
	}


	void RealToString( const REAL &R,std::string &s,int nWidth,char cBase,bool bFill)
	{
		if( nWidth <= 0)
		{
			s.clear();
			return;
		}
		static REAL I,F;
		std::string s1,s2;
		int nHeight,nDepth,nExp;
		IToString(R,s1,nHeight,nWidth+1,cBase);
		if( s1.empty() )
		{
			FToString(R,s,nDepth,nWidth + 1,true,cBase);
			nExp = nDepth - s.size();
		}
		else
		{
			FToString(R,s2,nDepth,nWidth + 1-(s1.size()+nHeight),false,cBase);
			if( s2.empty() )
			{
				s = s1;
				nExp = nHeight;
			}
			else
			{
				s = s1 + std::string(nHeight-nDepth,'0') + s2;
				nExp = nDepth - s2.size();
			}
		}
		if( s.size() < nWidth + 1)
		{
			int nZeroSize = nWidth + 1 - s.size();
			s += std::string(nZeroSize,'0');
			nExp -= nZeroSize;
		}
		REAL::RoundDigit(s,nExp,cBase);
		if( s.empty() )
		{
			s = "0";
			nExp = 0;
		}
		int nDotPos = s.size() + nExp;
		if( nDotPos <= 0)
		{
			if( 1 + (0-nDotPos) + s.size() <= nWidth)
			{
				REAL::FixDigit(s,nDotPos,nWidth,bFill);
			}
			else
			{
				REAL::FloatDigit(s,nDotPos,nWidth,cBase,bFill);
			}
		}
		else if( nDotPos >= 1 && nDotPos <= s.size() )
		{
			REAL::FixDigit(s,nDotPos,nWidth,bFill);
		}
		else
		{
			if( nDotPos <= nWidth)
			{
				REAL::FixDigit(s,nDotPos,nWidth,bFill);
			}
			else
			{
				REAL::FloatDigit(s,nDotPos,nWidth,cBase,bFill);
			}
		}
		if( R.Sign() == -1)
		{
			s.insert(0,"-");
		}

	}

}
#include "INT.h"
#include "REAL.h"
#include <fstream>
using namespace zju04nycs;
using namespace std;

#define NPV(x) cout<<(#x)<<":"<<(x)<<endl;

void Print(int n=30)
{ 
	cout<<endl;
	for( int i=0;i<n;i++)
	{
		cout<<'*';
	}
	cout<<endl;

}
void Far(int k,ostream &os = cout)
{
	static REAL R = 1;
	static int n = 0;
	if( n > k)
	{
		n = 0;
	}	
	for( int i = n ;i<k;++i)
	{
		R *= (i+1);
	}
	n = k;
	os<<k<<"!:"<<R<<endl;
	
}

void TestPlus(const REAL & N,ostream &os = cout)
{
	REAL S = 0;
	for( REAL I = 1;I<=N; I += 1)
	{
		S += I;
		assert( S == I*(I+1)>>1);
	//	os<<"S("<<I<<")= "<<S<<endl;
	}
}

REAL R1,R2,R;

void Test1()
{
	ofstream out(L"¼Ó·¨²âÊÔ1.txt");
	NPV( REAL(1) >> 4);
	REAL::DisWidth(100);
	do
	{
		cin>>R;
		cout<<"D:"<<R<<endl;
		REAL::DisBase('b');
		cout<<"B"<<R<<endl;
		REAL::DisBase('h');
		cout<<"H:"<<R<<endl;
		REAL::DisBase('D');

	}while( R != 0);
}
void Test2()
{
	REAL::DisWidth(100);
	do
	{
		cout<<"D"<<endl;
		REAL::DisBase('D');
		cin>>R;
		cout<<"D"<<R<<endl;
		REAL::DisBase('b');
		cout<<"B"<<R<<endl;
		REAL::DisBase('h');
		cout<<"H:"<<R<<endl;

		cout<<"B"<<endl;
		REAL::DisBase('b');
		cin>>R;
		cout<<"B"<<R<<endl;
		REAL::DisBase('d');
		cout<<"D"<<R<<endl;
		REAL::DisBase('h');
		cout<<"H:"<<R<<endl;

		cout<<"H"<<endl;
		REAL::DisBase('H');
		cin>>R;
		cout<<"h"<<R<<endl;
		REAL::DisBase('d');
		cout<<"D"<<R<<endl;
		REAL::DisBase('b');
		cout<<"b:"<<R<<endl;

	}while( R!=0);
}

void e(int p = 1000)
{
	REAL Ten_e_p = 1;
	const REAL Ten = 10;
	for( int i = 0;i<p;++i)
	{
		Multiply(REAL::TMP = Ten_e_p,Ten,Ten_e_p);
	}
	int nP = Ten_e_p.Lead();
	int n = 1;
	REAL FAC = 1;
	REAL FAC_N = 1;
	const REAL One;
	while( FAC_N.Lead() < nP+1)
	{
		++n;
		FAC *= n;
		FAC_N = FAC * n;
	}
	int k = - nP - 1 - REAL(n-2).Lead();
	FAC = 1;
	REAL E = 2;
	int r;
	for( int i = 2; i<= n;++i)
	{
		Multiply(REAL::TMP = FAC,i,FAC);
		Plus( REAL::TMP = E,Reverse(FAC,REAL::DIV,k,r),E);
	}
	REAL::DisWidth(p + 2);
	NPV(E);
}
REAL e_m_k(int m,int Tm)
{
	REAL E = 1;
	int n = 1;
	REAL M = 2;
	const REAL One;
	while( M.Lead() < m*(n+1) - Tm + 3)
	{
		++n;
		M*=(n+1);
	}
	int k = Tm - 1 - REAL(n-1).Lead();
	REAL X = 1;
	M = 1;
	int r;
	for( int i = 1; i<= n;++i)
	{
		X <<= m;
		Multiply(REAL::TMP = M,i,M);
		Plus(REAL::TMP = E,Divide(X,M,REAL::DIV,k,r),E);
	}
	return E;

}

REAL e_spin( int m ,int Tm,int Em)
{
	if( m - 1 == Em)
	{
		REAL R = e_m_k(m-1,Tm - 3);
		return R*R.Hold(Tm-1);
	}
	else
	{
		REAL R = e_spin(m-1,Tm-3,Em);
		return R*R.Hold(Tm-1);
	}
}

void e1()
{
	do
	{
		int p,m;
		cin>>p>>m;
		REAL Ten_e_p = 1;
		const REAL Ten = 10;
		for( int i = 0;i<p+1;++i)
		{
			Multiply(REAL::TMP = Ten_e_p,Ten,Ten_e_p);
		}
		int nP = Ten_e_p.Lead();
		REAL::DisWidth(p + 2);
		REAL::DisFill(true);
		cout<<"e("<<p<<")= "<<e_spin(0,-nP,m)<<endl;
	}while(1);
}

void Test3()
{
	NPV(REAL(1,-5));
	NPV(REAL(1,-4));
	NPV(REAL(1,-3));
	NPV(REAL(1,-2));
	NPV(REAL(1,-1));
	NPV(REAL(1,0));
	NPV(REAL(1,1));
	NPV(REAL(1,2));
	NPV(REAL(1,3));
	NPV(REAL(1,4));
	NPV(REAL(1,5));
	NPV(REAL(1,6));
}
void Test4()
{
	///	Test2();
	int k;
	do
	{
		cin>>k;
		e(k);
	}while(k);

}
void Test5()
{
	int k;
	REAL::DisWidth(1000);
	do
	{
		cin>>k;
		Far(k);
	}while(k);
}
void Test6()
{
	REAL R;
	do
	{
		cin>>R;
		NPV(R);
	}while(R!=0);
}
int main()
{
	///	Test2();
	e1();
	return 0;
	

}
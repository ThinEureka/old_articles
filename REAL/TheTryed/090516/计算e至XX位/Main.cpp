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
int main()
{
	
///	Test2();
	int k;
	do
	{
		cin>>k;
		e(k);
	}while(k);
	return 0;
}
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
int main()
{
	REAL Ten_e_1000 = 1;
	const REAL Ten = 10;
	for( int i = 0;i<1000;++i)
	{
		Multiply(REAL::TMP = Ten_e_1000,Ten,Ten_e_1000);
	}
	int nP = Ten_e_1000.Lead();
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
	REAL::DisWidth(60);
	for( int i = 2; i<= n;++i)
	{
		Multiply(REAL::TMP = FAC,i,FAC);
		if( i % 10 == 0)
		{
			cout<<"E"<<i<<"="<<E<<endl;
		}
		Plus( REAL::TMP = E,Reverse(FAC,REAL::DIV,k,r),E);
	}
	REAL::DisWidth(1003);
	NPV(E);
	return 0;
}
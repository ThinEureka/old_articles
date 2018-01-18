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

	REAL R1,R2,R3,R4,R5,R6,R7,R8,R9,R10;
	R1 = 1;
	R2 = 9;
	R3 = -1534;
	R4 = 9878;
//	REAL::DisFill(true);
	REAL::DisWidth(10000);
	ofstream out(L"½×³Ë±í3.txt");
	for( int i = 0 ;i <100;++i)
		Far(1 + i,out);
	for( int i = 200;i<=1000;i+=100)
		Far(i,out);
	Far(2000,out);
	Print();
	return 0;
}
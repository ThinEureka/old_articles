//Main.cpp
//主程序所在文件
#include "Map.h"
#include <iostream>
using namespace std;

void PrintArray( const int nArray[],int nDimension)
{
	for(int i=0;i<nDimension;++i)
	{
		cout<<nArray[i]<<'\t';
	}
	cout<<endl;
}

int CountReverse( const int nArray[],int nDimension)
{
	int nR = 0;
	for( int i=0;i<nDimension-1;++i)
	{
		for( int j = i+1;j<nDimension;++j)
		{
			if( nArray[i] >nArray[j])
				++nR;
		}
	}
	return nR;
}

void NumOfReverse(int n)
{
	static CMap Map;
	Map.Create(n);
	Map.Allocate();
	int m = n*(n-1)/2+1;
	int *nYs = new int[m];
	for( int i=0;i<m;++i)
	{
		nYs[i] = 0; 
	}
	int *nXs = new int[n];
	while( Map.GetValue(nXs))
	{
		++(nYs[ CountReverse(nXs,n) ]);
	}
	PrintArray(nYs,m);
	delete[] nXs;
	delete[] nYs;

}
void main()
{
	int n;
	cin>>n;
	while( n!=0)
	{
		NumOfReverse(n);
		cin>>n;
	}
}
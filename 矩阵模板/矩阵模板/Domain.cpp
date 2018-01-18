//Domain.cpp
//模板类试验的主函数

#include "tmp.h"
#include "Fraction.h"
#include <math.h>
#include"MatrixT.h"
#include "PolyApproxT.h"
#include "MTApp.h"
#include "SimpleLpT.h"

#define MNPV(x) cout<<#x":"<<endl;\
				(x).Print();
#define NPV(v) cout<<#v<<" "<<(v)<<endl

void Print(int n=30,char c='*')
{ 
	cout<<endl;
	for( int i=0;i<n;i++)
	{
		cout<<c;
	}
	cout<<endl;
	
}



	typedef double T;

	typedef CMatrixT<T> CMatrix;
	typedef CChangeCmdT<T> CChangeCmd;
	typedef CCmdSeriesT<T> CCmdSeries;
	typedef CPolynomialT<T> CPolynomial;
	typedef CPolyApproxT<T> CPolyApprox;
//	typedef CVectorT<T> CVector;
	typedef CSimpleLpT<T> CSimpleLp;


void Test091030()
{
	/*CMatrix A,X,B,P,C,D,E,F,G,lA,lB;

	T t[] =
	{
		1,0,0,0,1,
		T(1,2),1,0,0,0,
		T(1,3),T(1,2),1,0,0,
		T(1,4),T(1,3),T(1,2),1,0
	};
	A.Create(t,4,5);
	A.SolveLinearEq(X);
	MNPV(A);
	MNPV(X);*/
}


	void main()
	{	
		//Test091030();
		
		T t[11*12]=
		{
			
		};

		T s[4 * 4] = {
			-1.54588e-006, 2.79155e-006, 1, 0, -0.439261, -0.898359, 1.7897e-006, 0, 0.898359, -0.439261, 2.61784e-006, 0, -10.4201, 11.6806, -4.8267e-005, 1
		};

		T n1[4 * 4] = {
			1.19209e-006, -0.848326, -0.529474, 0, 1, 9.53674e-007, 7.7486e-007, 0, -1.19209e-007, -0.529474, 0.848326, 0, 2.30621e-008, -0.527611, 158.912, 1
		};

		T n2[4 * 4] = {
			0, -1.3868e-006, 1, 0, 1, 1.3868e-006, 0, 0, -1.3868e-006, 1, 1.3868e-006, 0, 0, 0, 0, 1
		};

		CMatrix A, X, B, P, C, D, E, F, G, lA, lB, N1T, N2T, ST;
		ST.Create(s, 4, 4);
		N1T.Create(n1, 4, 4);
		N2T.Create(n2, 4, 4);

		B = ST * N2T * N1T;
		MNPV(ST);
		MNPV(N1T);
		MNPV(N2T);
		MNPV(N2T * N1T);
		MNPV(B);

		return;

	A.Create(t,11, 12);
	for (int i = 1; i <= 11; ++i)
	{
		A.e(1, i) = (i-1) / 3 + 5;
	}
	A.e(1, 12) = 10;

	for (int i = 1; i <=11; ++i)
	{
		A.e(2, i) = (i - 1) %3 + 2;
	}
	A.e(2, 12) = 3;


	for (int row = 3; row <= 5; ++row)
	{
		for (int j = 1; j <=11; ++j)
		{
			A.e(row, j) = 0;
			if (j % 3 == 1 && row == 3
				|| j % 3 == 2 && row == 4
		        || j % 3 == 0 && row == 5)
			{
				A.e(row, j) = 1;
			}
		}

		A.e(row, 12) = 1;
	}

	for (int row = 6; row <= 11; ++row)
	{
		for (int j = 1; j <=11; ++j)
		{
			A.e(row, j) = 0;
			if (j % 5 == row % 5)
			{
				A.e(row, j) = 1;
			}
		}

		A.e(row, 12) = 1;
	}

	/*for (int i = 1; i <= 16; ++i)
	{
		A.e(11, i) = 1;
	}*/
	
	MNPV(A);
	bool bRet = A.SolveLinearEq(X);
	MNPV(X);
	/*B = A.TakeOut(1,1,3,3);
	MNPV(B);
	MNPV(X);*/
	//MNPV(B*X);



	

	int g;
	std::cin>>g;



	
	



	
	
	

	







	











	

	
	
}
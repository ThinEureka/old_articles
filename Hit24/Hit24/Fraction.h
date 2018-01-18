#ifndef FRACTION_H
#define FRACTION_H
#include<iostream>
using namespace std;
class CFraction
{
private:
	int Numerator;
	int Denominator;
public:
	CFraction(){};
	CFraction(int x,int y);
	CFraction(int x)
	{
		this->Numerator=x;
		this->Denominator=1;
	};
public:
//	 explicit operator double()
//	{
//		return Numerator/Denominator;
//	};h
	int GetInt()const {return Numerator/Denominator;};
	double GetFloat()const{return double(Numerator)/Denominator;};
	
friend inline ostream& operator <<(ostream& os,const CFraction &f);
friend inline istream& operator >>(istream& is,CFraction &f);

friend CFraction operator +(const CFraction &f1,const CFraction &f2);
friend CFraction operator -(const CFraction &f1,const CFraction &f2);
friend CFraction operator *(const CFraction &f1,const CFraction &f2);
friend CFraction operator /(const CFraction &f1,const CFraction &f2);

public:
	CFraction operator-()const;
	CFraction operator+=(const CFraction &f);
	CFraction operator-=(const CFraction &f);
	CFraction operator*=(const CFraction &f);
	CFraction operator/=(const CFraction &f);

	bool operator==(const CFraction &f)const;
	bool operator!=(const CFraction &f)const;
	bool operator> (const CFraction &f)const;
	bool operator>=(const CFraction &f)const;
	bool operator< (const CFraction &f)const;
	bool operator<=(const CFraction &f)const;
};
int MaxCommonDivisor(int m,int n);
inline CFraction::CFraction(int x,int y)
	{
		if(y==0)
		{
			cout<<"%%·ÖÄ¸ÎªÁã%%"<<endl;
			return;
		}

		if(x==0)
		{
			y=1;
		}
		else
		{
			int MCD=MaxCommonDivisor(x,y);
			x/=MCD;
			y/=MCD;
		    if(y<0)
			{
				x=-x;
				y=-y;
			}
		}
		CFraction::Numerator=x;
		CFraction::Denominator=y;

}
inline ostream& operator <<(ostream& os,const CFraction &f)
{
	if(f.Numerator==0)
		os<<0;
	else if(f.Denominator==1)
		os<<f.Numerator;
	else
		os<<f.Numerator<<'/'<<f.Denominator;
	return os;
}
inline istream& operator >>(istream& is,CFraction &f)
{
	char c;
	do
	{
		cin>>f.Numerator;
		cin>>c;
		cin>>f.Denominator;
	}
	while(c!='/');
	return is;
}
#endif
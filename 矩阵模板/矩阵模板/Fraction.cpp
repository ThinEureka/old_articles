
#include"Fraction.h"

long long MaxCommonDivisor(long long m,long long n)
{
	if(m<0)
		m=-m;
	if(n<0)
		n=-n;
	if(m<n)
	{
		long long temp=m;
		m=n;
		n=temp;
	}
	long long rem;
	while(n>0)
	{ 
		rem=m%n;
		m=n;
		n=rem;
	}
	return m;
}
CFraction operator +(const CFraction &f1,const CFraction &f2)
{
	return CFraction(f1.Numerator*f2.Denominator+f1.Denominator*f2.Numerator,
		             f1.Denominator*f2.Denominator);
}

CFraction operator -(const CFraction &f1,const CFraction &f2)
{
	return CFraction(f1.Numerator*f2.Denominator-f1.Denominator*f2.Numerator,
		             f1.Denominator*f2.Denominator);
}
CFraction operator *(const CFraction &f1,const CFraction &f2)
{
	return CFraction(f1.Numerator*f2.Numerator,
		             f1.Denominator*f2.Denominator);
}
CFraction operator /(const CFraction &f1,const CFraction &f2)
{
	return CFraction(f1.Numerator*f2.Denominator,
		             f1.Denominator*f2.Numerator);
}
CFraction CFraction::operator-()const
{
	return 0-*this;
}

CFraction CFraction::operator+=(const CFraction &f)
{
	*this=*this+f;
	return *this;
}
CFraction CFraction::operator-=(const CFraction &f)
{
	*this=*this-f;
	return *this;
}
CFraction CFraction::operator*=(const CFraction &f)
{
	*this=*this*f;
	return *this;
}
CFraction CFraction::operator/=(const CFraction &f)
{
	*this=*this/f;
	return *this;
}





bool CFraction::operator==(const CFraction &f)const
{
	if(this->Numerator==f.Numerator&&this->Denominator==f.Denominator)
		return true;
	return false;
}
bool  CFraction::operator!=(const CFraction &f)const
{
	return !this->operator ==(f);
}
bool  CFraction::operator>(const CFraction &f)const
{
	CFraction tft=*this-f;
	if(tft.Numerator>0)
		return true;
	else 
		return false;

}
bool  CFraction::operator>=(const CFraction &f)const
{
	CFraction tft=*this-f;
	if(tft.Numerator>=0)
		return true;
	else 
		return false;

}
		
bool  CFraction::operator<(const CFraction &f)const
{
	CFraction tft=*this-f;
	if(tft.Numerator<0)
		return true;
	else 
		return false;

}	
bool  CFraction::operator<=(const CFraction &f)const
{
	CFraction tft=*this-f;
	if(tft.Numerator<=0)
		return true;
	else 
		return false;

}	

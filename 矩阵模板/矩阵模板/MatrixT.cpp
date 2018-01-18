//MatrixT.cpp
//模板类CMatrixT<class T>的执行文件

#include "MatrixT.h"
#include "math.h"

long double CVectorT<long double>::Length() const
{
	return ::sqrt((*this) * (*this));
}


double CVectorT<double>::Length() const
{
	return ::sqrt((*this) * (*this));
}

float CVectorT<float>::Length() const
{
	return ::sqrt((*this) * (*this));
}



	





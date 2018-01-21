//Map.h
//类CMap的头文件
#pragma once
#include <iostream>

class CMap
{
private:
	int m_nDimension;
	int m_nStatus;
	int* m_nSubArray;
	int* m_nTmpXs;
	int m_nFirstItem;
	CMap * m_pSubMap;
public:
	CMap():m_nSubArray(NULL),m_nTmpXs(NULL),m_pSubMap(NULL)
	{
	};
	~CMap()
	{
		if( m_nSubArray)
		{
			delete[] m_nSubArray;
			delete[] m_nTmpXs;
			delete m_pSubMap;
		}
	};
public:
	void Create(int nDimension);
public:
	void Allocate();
private:
	bool UpDateSub();
	void FillSub();
public:
	bool GetValue(int * Xs);

};
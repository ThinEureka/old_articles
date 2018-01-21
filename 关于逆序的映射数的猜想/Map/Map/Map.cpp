//Map.cpp
//类CMap的执行文件
#include "Map.h"

void CMap::Create(int nDimension)
{

	if( this->m_nSubArray)
	{
		delete[] m_nSubArray;
		delete[] m_nTmpXs;
		delete m_pSubMap;
	}

	m_nDimension = nDimension;
	if( m_nDimension > 1)
	{
		m_nSubArray = new int[m_nDimension-1];
		m_nTmpXs = new int[m_nDimension-1];
		m_pSubMap = new CMap;
		m_pSubMap->Create(m_nDimension-1);
	}

}

void CMap::Allocate()
{
	if( m_nDimension > 1)
	{
		m_nFirstItem = 0;
		FillSub();
		m_pSubMap->Allocate();
	}
	m_nStatus = 0;

}

bool CMap::UpDateSub()
{
	if( m_nFirstItem == m_nDimension-1 )
		return false;
	else
	{
		++m_nFirstItem;
		FillSub();
		m_pSubMap->Allocate();
		return true;
	}
}


void CMap::FillSub()
{
	int nTmpPos = 0;
	for( int i=0;i<m_nDimension-1;++i)
	{
		if( nTmpPos != m_nFirstItem)
		{
			m_nSubArray[i] = nTmpPos++;
		}
		else
		{
			m_nSubArray[i] = ++nTmpPos;
			++nTmpPos;
		}
	}
}

bool CMap::GetValue(int *Xs)
{
	if( m_nDimension == 1)
	{
		switch( m_nStatus)
		{
		case 0:
			Xs[0] = 0;
			++m_nStatus;
			return true;
		case 1:
			return false;

		}
	}
	else
	{
		if( m_pSubMap->GetValue(m_nTmpXs) )
		{
			Xs[0] = m_nFirstItem;
			for( int i=0; i<m_nDimension -1;++i)
			{
				Xs[1+i] = m_nSubArray[ m_nTmpXs[i] ];
			}
			++m_nStatus;
			return true;
		}
		else
		{
			if( UpDateSub())
			{
				return GetValue(Xs);
			}
			else
			{
				return false;
			}
		}
	}
}
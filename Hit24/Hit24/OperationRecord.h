//OperationRecord.h
//COperationRecord 类的头文件



#pragma once

#include "Fraction.h"

#define ORIGIN 'O'






class COperationRecord
{
private:
	CFraction m_cfRoot;
	char m_cOperator;
	COperationRecord * m_pLeftRecord;
	COperationRecord * m_pRightRecord;
public:
	COperationRecord(){};
	COperationRecord( CFraction cfRoot ) : m_cfRoot( cfRoot )
	{

		m_cOperator = ORIGIN;
		m_pLeftRecord = m_pRightRecord = NULL;
	};
	COperationRecord ( int nRoot) : m_cfRoot( CFraction( nRoot ) )
	{
		m_cOperator = ORIGIN;
		m_pLeftRecord = m_pRightRecord = NULL;
	};
	void Print()const;
	void Show()const
	{
		Print();
		cout<< '=';
		cout<< this->m_cfRoot;
	};
	CFraction GetValue()const
	{
		return m_cfRoot;
	};
	
friend  COperationRecord operator + ( const COperationRecord &LeftRecord , const COperationRecord &RightRecord);              
friend  COperationRecord operator - ( const COperationRecord &LeftRecord , const COperationRecord &RightRecord);
friend  COperationRecord operator * ( const COperationRecord &LeftRecord , const COperationRecord &RightRecord);
friend  COperationRecord operator / ( const COperationRecord &LeftRecord , const COperationRecord &RightRecord);




};




struct  CRecordListUnit
{
	COperationRecord Record;
	CRecordListUnit* pNextUnit;
};



class CRecordList
{
private:
	CRecordListUnit* pFirstUnit;
	CRecordListUnit* pLastUnit;
	bool bSelfDestroy;
public:
	CRecordList():pFirstUnit(NULL),pLastUnit(NULL),bSelfDestroy(true){};
	~CRecordList()
	{
		if(bSelfDestroy)
			Clear();
	};
public:
	void Create( const COperationRecord &Record1, const COperationRecord &Record2);
	void Create( const COperationRecord &Record)
	{
		this->pFirstUnit = this->pLastUnit = new CRecordListUnit;
		pFirstUnit->pNextUnit = NULL;
		pFirstUnit->Record = Record;
	};
	void Create( const CRecordList &List,const COperationRecord &Record);
	void Create( const COperationRecord &Record1,const COperationRecord &Record2,
		         const COperationRecord &Record3);
	void Append( const CRecordList &List)
	{
		if( !pFirstUnit)
		{
			pFirstUnit = List.pFirstUnit;
			pLastUnit = List.pLastUnit;
			return;
		}
		if( List.pFirstUnit)
		{
			pLastUnit->pNextUnit = List.pFirstUnit;
			pLastUnit = List.pLastUnit;
		}

	};
public:
	bool CheckRecord( const COperationRecord &Record)const;
	void AddRecord( const COperationRecord &Record);
	void ReceiveRecord(const COperationRecord &Record)
    {
		if( this->CheckRecord( Record ))
			this->AddRecord( Record );
    };
public:
	void Clear();
	void UnAttach()
	{
		pFirstUnit = pLastUnit = NULL;
	};

public:
	void SelfDestroy( bool bDone = true)
	{
		bSelfDestroy = bDone;
	};
public:
	void Show()const;

public:
	bool Hit( const CFraction &cfAim, COperationRecord* pRecord =NULL);
friend bool Hit( const CRecordList &List1,const CRecordList &List2,const CFraction &cfAim ,
				  COperationRecord* pRecord = NULL);
	
};










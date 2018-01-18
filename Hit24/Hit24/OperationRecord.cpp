// OperationRecord.cpp
// COperationRecord 类的执行文件

#include "OperationRecord.h"


void COperationRecord::Print()const
{

	switch( m_cOperator)
	{
	case ORIGIN:
		cout<< this->m_cfRoot;
		break;
	default:
		{
			if( this->m_pLeftRecord->m_cOperator == ORIGIN)
				this->m_pLeftRecord->Print();
			else
			{
				cout<< '(';
				this->m_pLeftRecord->Print();
				cout<< ')';
			}
			
			cout<< ' '<< this->m_cOperator<<' ';
			if( this->m_pRightRecord->m_cOperator == ORIGIN)
				this->m_pRightRecord->Print();
			else
			{
				cout<< '(';
				this->m_pRightRecord->Print();
				cout<< ')';
			}
		}
	}
}


COperationRecord operator + ( const COperationRecord& LeftRecord,
		                                 const COperationRecord& RightRecord)
{
		COperationRecord rRecord;
		rRecord.m_cOperator = '+';
		rRecord.m_pLeftRecord = (COperationRecord *)&LeftRecord;
		rRecord.m_pRightRecord = (COperationRecord *)&RightRecord;
		rRecord.m_cfRoot = LeftRecord.m_cfRoot  + RightRecord.m_cfRoot;
		return rRecord;
}

COperationRecord operator - ( const COperationRecord& LeftRecord,
		                                 const COperationRecord& RightRecord)
{
		COperationRecord rRecord;
		rRecord.m_cOperator = '-';

		rRecord.m_pLeftRecord = (COperationRecord *)&LeftRecord;
		rRecord.m_pRightRecord = (COperationRecord *)&RightRecord;
		rRecord.m_cfRoot = LeftRecord.m_cfRoot  - RightRecord.m_cfRoot;
		return rRecord;
		

}

COperationRecord operator * ( const COperationRecord& LeftRecord,
		                                 const COperationRecord& RightRecord)
{
		COperationRecord rRecord;
		rRecord.m_cOperator = '*';
		rRecord.m_pLeftRecord = (COperationRecord *)&LeftRecord;
		rRecord.m_pRightRecord = (COperationRecord *)&RightRecord;
		rRecord.m_cfRoot = LeftRecord.m_cfRoot  * RightRecord.m_cfRoot;
		return rRecord;
}

COperationRecord operator / ( const COperationRecord& LeftRecord,
		                                 const COperationRecord& RightRecord)
{
		COperationRecord rRecord;
		rRecord.m_cOperator = '/';
		rRecord.m_pLeftRecord = (COperationRecord *)&LeftRecord;
		rRecord.m_pRightRecord = (COperationRecord *)&RightRecord;
		rRecord.m_cfRoot = LeftRecord.m_cfRoot  / RightRecord.m_cfRoot;
		return rRecord;
}


bool CRecordList::CheckRecord(const COperationRecord &Record)const
{
	CRecordListUnit *pUnit = this->pFirstUnit;
	bool unset = true;
	while( pUnit )
	{
		if( pUnit->Record.GetValue()  == Record.GetValue() )
		{
			unset = false;
			break;
		}
		pUnit = pUnit->pNextUnit;
	}
	return unset;
}


void CRecordList::AddRecord(const COperationRecord &Record)
{

	CRecordListUnit* pUnit = new CRecordListUnit;
	pUnit->Record = Record;
	pUnit->pNextUnit = NULL;

	if( !pFirstUnit )
		pFirstUnit = pLastUnit = pUnit;


	else
	{
		pLastUnit->pNextUnit = pUnit;
		pLastUnit = pUnit;

	}

    return;

}



void CRecordList::Create(const COperationRecord &Record1, const COperationRecord &Record2)
{
	COperationRecord Record;

	Record = Record1 + Record2;
	this->ReceiveRecord( Record );

	Record = Record1 - Record2;
	this->ReceiveRecord( Record );

	Record = Record2 - Record1;
	this->ReceiveRecord( Record );

	Record = Record1 * Record2;
	this->ReceiveRecord( Record );

	if( Record2.GetValue()!= 0)
	{
		Record = Record1 / Record2;
		this->ReceiveRecord( Record );
	}

	if( Record1.GetValue()!= 0)
	{
		Record = Record2 / Record1;
		this->ReceiveRecord( Record );
	}
}



void CRecordList::Create( const CRecordList &List,const COperationRecord &Record)
{
	CRecordList tmpList;
	tmpList.SelfDestroy( false );

	CRecordListUnit* pUnit = List.pFirstUnit;

	while( pUnit)
	{
		tmpList.Create( pUnit->Record ,Record);
		this->Append( tmpList );
		tmpList.UnAttach();
		pUnit = pUnit->pNextUnit;
	}
}


void CRecordList::Create( const COperationRecord &Record1,const COperationRecord &Record2,
		         const COperationRecord &Record3)
{
		CRecordList tmpList1,tmpList2;
		tmpList1.SelfDestroy( false);
		tmpList2.SelfDestroy( false);

		tmpList1.Create( Record1,Record2);
		tmpList2.Create( tmpList1, Record3);
		this->Append( tmpList2);
		tmpList1.UnAttach();
		tmpList2.UnAttach();

		tmpList1.Create( Record1,Record3);
		tmpList2.Create( tmpList1, Record2);
		this->Append( tmpList2);
		tmpList1.UnAttach();
		tmpList2.UnAttach();

		tmpList1.Create( Record3,Record2);
		tmpList2.Create( tmpList1, Record1);
		this->Append( tmpList2);
		tmpList1.UnAttach();
		tmpList2.UnAttach();
};



void CRecordList::Show() const
{
	CRecordListUnit *pUnit = this->pFirstUnit;

	while( pUnit )
	{
		pUnit->Record.Show();
		cout<<endl;
		pUnit = pUnit->pNextUnit;
	}
}



void CRecordList::Clear()
{
	CRecordListUnit *pUnit1 = this->pFirstUnit;
    CRecordListUnit *pUnit2;
	while( pUnit1 )
	{
        pUnit2 = pUnit1;
		pUnit1 = pUnit1->pNextUnit;
		delete pUnit2;
		
	}
	this->pFirstUnit = this->pLastUnit = NULL;
}



bool CRecordList::Hit(const CFraction &cfAim, COperationRecord *pRecord )
{
	CRecordListUnit* pUnit;

	pUnit = this->pFirstUnit;
	while( pUnit)
	{
		if( pUnit->Record.GetValue() == cfAim )
		{
			if( pRecord )
				*pRecord = pUnit->Record;
			return true;
		}
		
		pUnit = pUnit->pNextUnit;

	}
	return false;
}






bool Hit( const CRecordList &List1,const CRecordList &List2,const CFraction &cfAim ,
		  COperationRecord* pRecord )
{
	CRecordListUnit* pUnit1,* pUnit2;
    CRecordList tmpList;

	pUnit1 = List1.pFirstUnit;
	while( pUnit1)
	{
		pUnit2 = List2.pFirstUnit;

		while( pUnit2)
		{
			tmpList.Create( pUnit1->Record,pUnit2->Record);
			if( tmpList.Hit( cfAim,pRecord ) )
				return true;
			tmpList.Clear();
			pUnit2 = pUnit2->pNextUnit;
		}
		pUnit1 = pUnit1->pNextUnit;
	}

	return false;
}
















	

	







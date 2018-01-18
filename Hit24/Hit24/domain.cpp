//domain.cpp 
// ��ʮ�ĵ�����������


#include "OperationRecord.h"

bool Hit24( const CFraction &cf1,const CFraction &cf2,const CFraction &cf3, const CFraction &cf4,
		  COperationRecord* pRecord = NULL)
{
	CRecordList List1,List2;
	COperationRecord r1(cf1), r2(cf2), r3(cf3), r4(cf4);

    List1.Create( r1, r2);
	List2.Create( r3, r4);
	if( Hit( List1,List2,24,pRecord) )
	{
		if( pRecord)
			pRecord->Show();
		return true;
	}
	List1.Clear();
	List2.Clear();

	List1.Create( r1, r3);
	List2.Create( r2, r4);
	if( Hit( List1,List2,24,pRecord) )
	{
		if( pRecord)
			pRecord->Show();
		return true;
	}
	List1.Clear();
	List2.Clear();

	List1.Create( r1, r4);
	List2.Create( r3, r2);
	if( Hit( List1,List2,24,pRecord) )
	{
		if( pRecord)
			pRecord->Show();
		return true;
	}
	List1.Clear();
	List2.Clear();

	List1.Create( r1);
	List2.Create( r2, r3,r4);
	if( Hit( List1,List2,24,pRecord) )
	{
		if( pRecord)
			pRecord->Show();
		return true;
	}
	List1.Clear();
	List2.Clear();

	List1.Create( r2);
	List2.Create( r1, r3,r4);
	if( Hit( List1,List2,24,pRecord) )
	{
		if( pRecord)
			pRecord->Show();
		return true;
	}
	List1.Clear();
	List2.Clear();

	List1.Create( r3);
	List2.Create( r1, r2,r4);
	if( Hit( List1,List2,24,pRecord) )
	{
		if( pRecord)
			pRecord->Show();
		return true;
	}
	List1.Clear();
	List2.Clear();

	List1.Create( r4);
	List2.Create( r1, r3,r2);
	if( Hit( List1,List2,24,pRecord) )
	{
		if( pRecord)
			pRecord->Show();
		return true;
	}
	List1.Clear();
	List2.Clear();
	return false;


}

void main()
{
	cout<<endl;
	cout<<"                  *****************��ʮ�ĵ�********************"<<endl<<endl;
	int nSelect,r1,r2,r3,r4;
	COperationRecord r;
    do
	{
		cout<<"1 �Զ��ж��ĸ������Ƿ����ͨ����������õ�24,�������㷨"<<endl;
		cout<<"2 �˳�"<<endl;
		cin>>nSelect;
		if( nSelect == 1)
		{
			cout<<"�������ĸ�����"<<endl;
			cin>>r1>>r2>>r3>>r4;
		   if( !Hit24(r1,r2,r3,r4,&r) )
		   {
			 cout<<"���ĸ�������ͨ����������õ�24"<<endl;
		   }
		   cout<<endl;
		}
	}
	while( nSelect!=2);

    
}
//domain.cpp 
// 二十四点程序的主函数


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
	cout<<"                  *****************二十四点********************"<<endl<<endl;
	int nSelect,r1,r2,r3,r4;
	COperationRecord r;
    do
	{
		cout<<"1 自动判断四个整数是否可以通过四则运算得到24,并给出算法"<<endl;
		cout<<"2 退出"<<endl;
		cin>>nSelect;
		if( nSelect == 1)
		{
			cout<<"请输入四个整数"<<endl;
			cin>>r1>>r2>>r3>>r4;
		   if( !Hit24(r1,r2,r3,r4,&r) )
		   {
			 cout<<"此四个数不能通过四则运算得到24"<<endl;
		   }
		   cout<<endl;
		}
	}
	while( nSelect!=2);

    
}
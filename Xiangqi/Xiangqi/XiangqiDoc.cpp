// XiangqiDoc.cpp : CXiangqiDoc ���ʵ��
//

#include "stdafx.h"
#include "Xiangqi.h"

#include "XiangqiDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CXiangqiDoc

IMPLEMENT_DYNCREATE(CXiangqiDoc, CDocument)

BEGIN_MESSAGE_MAP(CXiangqiDoc, CDocument)
END_MESSAGE_MAP()


// CXiangqiDoc ����/����

CXiangqiDoc::CXiangqiDoc()
{
	// TODO: �ڴ����һ���Թ������
		CDib bm;
	bm.Load(IDR_DIB_RED_BING ,_T("DIB"));

}

CXiangqiDoc::~CXiangqiDoc()
{
}

BOOL CXiangqiDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CXiangqiDoc ���л�

void CXiangqiDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CXiangqiDoc ���

#ifdef _DEBUG
void CXiangqiDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CXiangqiDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CXiangqiDoc ����

// XiangqiDoc.cpp : CXiangqiDoc 类的实现
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


// CXiangqiDoc 构造/析构

CXiangqiDoc::CXiangqiDoc()
{
	// TODO: 在此添加一次性构造代码
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

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CXiangqiDoc 序列化

void CXiangqiDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CXiangqiDoc 诊断

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


// CXiangqiDoc 命令

// XiangqiDoc.h : CXiangqiDoc ��Ľӿ�
//


#pragma once
#include "DIB.h"

class CXiangqiDoc : public CDocument
{
protected: // �������л�����
	CXiangqiDoc();
	DECLARE_DYNCREATE(CXiangqiDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CXiangqiDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};



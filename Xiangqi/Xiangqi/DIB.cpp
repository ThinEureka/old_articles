//DIB.cpp
//CDib类的执行文件

#include "stdafx.h"
#include "DIB.h"

IMPLEMENT_SERIAL(CDib,CObject,1)
CDib::CDib()
{
	m_hDIB =  NULL;
	m_pPalette = NULL;
	m_pBitmap = NULL;

}

void CDib::Destroy()
{
	if(m_hDIB!=NULL)
	{
		DestroyDIB(m_hDIB);
		m_hDIB=NULL;
	}
}

BOOL CDib::BuildPalette()
{
	if(m_pPalette!=NULL)
	{
		delete m_pPalette;
		m_pPalette=NULL;
	}
	HPALETTE hPal=::CreateDIBPalette(m_hDIB);
	if(!hPal)
		return false;

	m_pPalette= new CPalette;
	m_pPalette->Attach(hPal);

	return true;
}

BOOL CDib::BuildBitmap()
{

	if(m_pBitmap!=NULL)
	{
		::AfxMessageBox(_T("位图指针不为空"));
		delete m_pBitmap;
		m_pBitmap=NULL;
		m_hBitmap=NULL;
	}
	m_hBitmap=::DIBToDIBSection(m_hDIB);

	if(m_hBitmap==NULL)
		return false;

	m_pBitmap= new CBitmap;
	m_pBitmap->Attach(m_hBitmap);

	return true;
}

BOOL CDib::UpdateInternal()
{
	this->BuildPalette();
	return this->BuildBitmap();
}

BOOL CDib::Create(DWORD dwWidth, DWORD dwHeight)
{
	HDIB hDIB=::CreateDefaultDIB(dwWidth,dwHeight);
	if(!hDIB)
		return false;
	Destroy();
	m_hDIB=hDIB;
	return UpdateInternal();
}

BOOL CDib::Create(DWORD dwWidth, DWORD dwHeight, WORD wBitCount)
{
	HDIB hDIB=::CreateDIB(dwWidth,dwHeight,wBitCount);
	if(!hDIB)
		return false;
	Destroy();
	m_hDIB=hDIB;
	return UpdateInternal();
}

BOOL CDib::Create(LPBYTE lpDIB)
{
	if(!lpDIB)
	{   
			::AfxMessageBox(_T("获得指针无效"));

		return false;
	}

	DWORD dwSize=::DIBBlockSize(lpDIB);
	HDIB hDIB=::GlobalAlloc(GHND,dwSize);

	if(!hDIB)
	{
		::AfxMessageBox(_T("获得句柄无效"));
		return false;
	}
	LPBYTE lpBi=(LPBYTE)::GlobalLock(hDIB);

	if(!lpBi)
		return false;

	::CopyMemory(lpBi,lpDIB,dwSize);
	::GlobalUnlock(hDIB);

	this->Destroy();
	m_hDIB=hDIB;
	return this->UpdateInternal();
}

BOOL CDib::Create(LPBYTE lpDIB, WORD wBitCount)
{
	if(lpDIB==NULL)
		return false;
	if(!Create(lpDIB))
		return false;

	WORD wBits=((LPBITMAPINFOHEADER)lpDIB)->biBitCount;
	if(wBits==wBitCount)
		return true;

	HDIB hNewDIB=::ConvertDIBFormat(m_hDIB,wBitCount,NULL);
	if(!hNewDIB)
		return false;

	this->Destroy();
	m_hDIB=hNewDIB;
	return this->UpdateInternal();
}

BOOL CDib::Create(HBITMAP hBitmap)
{
	::AfxMessageBox(_T("创建ＤＩＢ中"));
	if(!hBitmap)
	{
		::AfxMessageBox(_T("位图句柄为空"));
		return false;
	}
	HDIB hDIB=::DIBSectionToDIB(hBitmap);
	if(!hDIB)
	{
		::AfxMessageBox(_T("创建ＤＩＢ失败"));
		return false;
	}

	this->Destroy();
	m_hDIB=hDIB;
	return this->UpdateInternal();

}

BOOL CDib::Create(HBITMAP hBitmap, WORD wBitCount)
{
	HDIB hNewDIB;
	if(!hBitmap)
		return false;
	HDIB hDIB=::DIBSectionToDIB(hBitmap);
	if(!hDIB)
		return false;
	DIBSECTION ds;
	::GetObject(hBitmap,sizeof(DIBSECTION),&ds);
	if(wBitCount==ds.dsBmih.biBitCount)
		hNewDIB=hDIB;
	else
	{
		hNewDIB=::ConvertDIBFormat(hDIB,wBitCount,NULL);
		::GlobalFree(hDIB);
	}
	if(!hNewDIB)
		return false;

	this->Destroy();
	m_hDIB=hNewDIB;
	return this->UpdateInternal();
}

BOOL CDib::Create(HBITMAP hDDB, HPALETTE hPal)
{
	if(!hDDB)
		return false;
	HDIB hDIB=::BitmapToDIB(hDDB,hPal);
	if(!hDIB)
		return false;
	this->Destroy();
	m_hDIB=hDIB;
	return this->UpdateInternal();
}

BOOL CDib::Create(HBITMAP hDDB, HPALETTE hPal, WORD wBitCount)
{
	if(!hDDB)
		return false;
	HDIB hDIB=::BitmapToDIB(hDDB,hPal,wBitCount);
	if(!hDIB)
		return false;
	this->Destroy();
	m_hDIB=hDIB;
	return this->UpdateInternal();
}


BOOL CDib::Create(CRect rcScrn)
{
	HDIB hDIB=::CopyScreenToBitmap(rcScrn);

	if(!hDIB)
		return false;
	this->Destroy();
	m_hDIB=hDIB;
	return this->UpdateInternal();
}


BOOL CDib::Create(HWND hWnd, WORD fPrintArea)
{

	HDIB hDIB=::CopyWindowToBitmap(hWnd, fPrintArea);

	if(!hDIB)
		return false;
	this->Destroy();
	m_hDIB=hDIB;
	return this->UpdateInternal();
}

BOOL CDib::Create(HWND hWnd, CRect rcClient)
{

	HDIB hDIB=::CopyClientRectToBitmap(hWnd,rcClient);

	if(!hDIB)
		return false;
	this->Destroy();
	m_hDIB=hDIB;
	return this->UpdateInternal();
}

BOOL CDib::Attach(HDIB hDIB)
{
	if(hDIB==NULL)
		return false;
	m_hDIB=hDIB;
	this->Destroy();
	m_hDIB=hDIB;
	return this->UpdateInternal();
}

HDIB CDib::Detach()
{
	if(m_hDIB==NULL)
		return NULL;
	HDIB hDIB=m_hDIB;
	if(m_pBitmap!=NULL)
	{
		delete m_pBitmap;
		m_pBitmap=NULL;

	}
	if(m_pPalette!=NULL)
	{
		delete m_pPalette;
		m_pPalette=NULL;
	}
	return hDIB;
}

CDib::~CDib()
{
	this->Destroy();

	if(m_pBitmap!=NULL)
	{
		delete m_pBitmap;
		m_pBitmap=NULL;

	}
	if(m_pPalette!=NULL)
	{
		delete m_pPalette;
		m_pPalette=NULL;
	}

}
BOOL CDib::Read(CFile *pFile)
{
	//	WaiCursorBegin();


	LPBITMAPINFOHEADER lpBi;
	DWORD dwSize;
	HDIB hDIB;
	TRY
	{
		BITMAPFILEHEADER bmfHdr;
		pFile->Read(&bmfHdr,sizeof(BITMAPFILEHEADER));

		if(bmfHdr.bfType!=0x4d42)
		{
			//			WaitCursorEnd();
			return false;
		}
		DWORD dwLen=pFile->GetLength();
		if(bmfHdr.bfSize!=dwLen)
			bmfHdr.bfSize=dwLen;


		dwSize=dwLen-sizeof(BITMAPFILEHEADER);
		hDIB=GlobalAlloc(GHND,dwSize);
		lpBi=(LPBITMAPINFOHEADER)::GlobalLock(hDIB);
		DWORD dwCount=pFile->Read(lpBi,dwSize);

		if(dwCount!=dwSize)
		{
			::GlobalUnlock(hDIB);
			::GlobalFree(hDIB);
			//			WaitCursorEnd();
			return false;
		}


		if(lpBi->biSize!=sizeof(BITMAPINFOHEADER))
		{
			::GlobalUnlock(hDIB);
			::GlobalFree(hDIB);
			//			WaitCursorEnd();
			return false;
		}


		int nNumColors=(UINT)lpBi->biClrUsed;
		if(nNumColors==0)
		{
			if(lpBi->biBitCount!=24)
				nNumColors=1<<lpBi->biBitCount;
		}
		if(lpBi->biClrUsed==0)
			lpBi->biClrUsed=nNumColors;

		if(lpBi->biSizeImage==0)
			lpBi->biSizeImage=((((lpBi->biWidth*(DWORD)lpBi->biBitCount)+31)&~31)>>3)*
			lpBi->biHeight;
	}
	CATCH(CException,e)
	{
		::GlobalUnlock(hDIB);
		::GlobalFree(hDIB);
		//		WaitCursorEnd();
		return false;
	}
	END_CATCH

		BOOL bSuccess=this->Create((LPBYTE)lpBi);
	::GlobalUnlock(hDIB);
	::GlobalFree(hDIB);
	//	WaitCursorEnd();
	return bSuccess;
}
BOOL CDib::Write(CFile *pFile)
{
	//	WaitCursorBegin();

	BITMAPFILEHEADER bmfHdr;
	LPBITMAPINFOHEADER lpBi;
	DWORD dwDIBSize;


	lpBi=(LPBITMAPINFOHEADER)::GlobalLock(m_hDIB);

	if(!lpBi)
	{
		::GlobalUnlock(m_hDIB);
		//		WaitCurSorEnd();
		return false;
	}

	if(lpBi->biSize!=sizeof(BITMAPINFOHEADER))
	{
		::GlobalUnlock(m_hDIB);
		//		WaitCurSorEnd();
		return false;
	}


	bmfHdr.bfType=0x4d42;

	dwDIBSize=*(LPBYTE)lpBi+::PaletteSize((LPBYTE)lpBi);

	if((lpBi->biCompression==BI_RLE8)||(lpBi->biCompression==BI_RLE4))
		dwDIBSize+=lpBi->biSizeImage;



	else
	{
		DWORD dwBmBitsSize;


		dwBmBitsSize=WIDTHBYTES(lpBi->biWidth*lpBi->biBitCount)*lpBi->biHeight;

		dwDIBSize+=dwBmBitsSize;
		lpBi->biSizeImage=dwDIBSize;
	}


	bmfHdr.bfSize=dwDIBSize+sizeof(BITMAPFILEHEADER);

	bmfHdr.bfReserved1=0;
	bmfHdr.bfReserved2=0;


	bmfHdr.bfOffBits=(DWORD)sizeof(BITMAPFILEHEADER)+lpBi->biSize+
		::PaletteSize((LPBYTE)lpBi);

	TRY
	{
		pFile->Write(&bmfHdr,sizeof(BITMAPFILEHEADER));
		//	    pFile->WriteHuge(lpBi,dwDIBSize);
		pFile->Write(lpBi,dwDIBSize);
	}
	CATCH(CException,e)
	{
		::GlobalUnlock(m_hDIB);
		//		WaitCursorEnd();
		return false;
	}
	END_CATCH

		::GlobalUnlock(m_hDIB);
	//	WaitCurSorEnd();
	return true;
}
BOOL CDib::Load(LPCTSTR lpszDIBFile)
{
	TRY
	{
		CFile file(lpszDIBFile,CFile::modeRead|CFile::shareDenyNone);

		if(!this->Read(&file))
			return false;
	}
	CATCH(CException,e)
	{
		return false;
	}
	END_CATCH
		return true;
}
BOOL CDib::Save(LPCTSTR lpszDIBFile)
{
	TRY
	{
		CFile file(lpszDIBFile,CFile::modeCreate|CFile::modeWrite);
		if(!Write(&file))
			return false;
	}
	CATCH(CException,e)
	{
		return false;
	}
	END_CATCH
		return true;
}

void CDib::Serialize(CArchive &ar)
{
	CObject::Serialize(ar);
	ar.Flush();
	if(ar.IsStoring())
	{
		Write(ar.GetFile());
	}
	else
	{
		Read(ar.GetFile());
	}
}
BOOL CDib::Load(LPCTSTR lpszDIBRes, LPCTSTR lpszDIBType)
{
	HINSTANCE hInst=::AfxGetInstanceHandle();
	HRSRC hRes=::FindResource(hInst,lpszDIBRes,lpszDIBType);
	HGLOBAL hData = ::LoadResource(hInst,hRes);


	if( hRes==NULL || hData==NULL)
		return false;

	LPBYTE lpBuf = (LPBYTE)::LockResource(hData);

	//	if(((LPBITMAPFILEHEADER)lpBuf)->bfType!=DIB_HEADER_MARKER )
	if(((LPBITMAPFILEHEADER)lpBuf)->bfType!=0x4d42)
	{
		::AfxMessageBox(_T("非ＤＩＢ文件"));
		return false;
	}
//	::AfxMessageBox(_T("已获得文件指针"));

	LPBYTE lpDIB = lpBuf + sizeof(BITMAPFILEHEADER);

	return this->Create(lpDIB);
}


BOOL CDib::Load(UINT uIDs, LPCTSTR lpszDIBType)
{
	LPCTSTR lpszDIBRes = MAKEINTRESOURCE(uIDs);

	return this->Load(lpszDIBRes,lpszDIBType);
}

BOOL CDib::IsEmpty()
{
	if(m_hDIB==NULL)
		return true;

	if(!::GlobalLock(m_hDIB))
		return true;

	::GlobalUnlock( m_hDIB );
	return false;
}

DWORD CDib::GetCompression()
{
	LPBITMAPINFOHEADER lpBi = (LPBITMAPINFOHEADER)::GlobalLock( m_hDIB );

	if(!lpBi)
	{
		::GlobalUnlock( m_hDIB );
		return 0;
	}
	::GlobalUnlock( m_hDIB );
	return lpBi->biCompression;
}

WORD CDib::GetBitCount()
{
	return ::DIBBitCount( m_hDIB );
}

LONG CDib::GetWidth()
{
	return ::DIBWidth( m_hDIB );
}

LONG CDib::GetHeight()
{
	return ::DIBHeight( m_hDIB );
}

LONG CDib::GetWidthBytes()
{
	return ::BytesPerLine( m_hDIB );
}

WORD CDib::GetColorNumber()
{
	return ::DIBNumColors( m_hDIB );
}

WORD CDib::GetPaletteSize()
{
	return ::PaletteSize( m_hDIB );
}

CBitmap* CDib::GetBitmap()
{
	return m_pBitmap;
}

CPalette* CDib::GetPalette()
{
	return m_pPalette;
}

HANDLE CDib::GetHandle()
{
	return m_hDIB;
}

LPBYTE CDib::GetBitsPtr()
{
	return ::FindDIBBits( m_hDIB );
}
LONG CDib::GetPixelOffset(LONG x, LONG y)
{
	if(GetBitCount()>8)
	{
		int nTimes=GetBitCount()/8;
//		CString str;
//		str.Format(_T("步长为%d"), x*nTimes);
//	  ::AfxMessageBox(str);
		return (GetHeight()-y-1)*GetWidthBytes() + x*nTimes;
	}

	return (GetHeight()-y-1)*GetWidthBytes() + x/( 8/GetBitCount());
}
COLORREF CDib::GetPixel(LONG x, LONG y)
{
	COLORREF cCorlor;

	switch(this->GetBitCount())
	{
	case 1:

		if( 1<<( 7 - x%8 ) & *(BYTE*)( GetBitsPtr() + GetPixelOffset(x,y))  )
			cCorlor=RGB( 255,255,255 );
		else
			cCorlor=RGB( 0,0,0);

		break;

	case 4:
		{
			PALETTEENTRY pe[16];
			m_pPalette->GetPaletteEntries( 0,16,pe );
			int nIndex = (*(BYTE*)( GetBitsPtr() + GetPixelOffset(x,y)))&
				( x%2 ? 0x0f : 0xf0 )>>( x%2 ? 0 : 4);
			cCorlor = RGB(
				pe[nIndex].peRed,
				pe[nIndex].peGreen,
				pe[nIndex].peBlue
				);
			CString str;
	//		str.Format(_T("三原色分别为%d,%d,%d"),	
	//			pe[nIndex].peRed,
	//			pe[nIndex].peGreen,
	//				pe[nIndex].peBlue);
	//		::AfxMessageBox(str);
		}

		break;
	case 8:
		{
			

			PALETTEENTRY pe[256];
			m_pPalette->GetPaletteEntries( 0,256,pe );
			int nIndex = *(BYTE*)( GetBitsPtr() + GetPixelOffset(x,y));
			cCorlor = RGB(
				pe[nIndex].peRed,
				pe[nIndex].peGreen,
				pe[nIndex].peBlue
				);
			CString str;
	//		str.Format(_T("三原色分别为%d,%d,%d"),	
	//			pe[nIndex].peRed,
	//			pe[nIndex].peGreen,
	//				pe[nIndex].peBlue);
	//		::AfxMessageBox(str);
		}
		break;
	default:
		
			
		cCorlor = RGB(
			*(BYTE*)( GetBitsPtr() + GetPixelOffset(x,y)+2 ),
			*(BYTE*)( GetBitsPtr() + GetPixelOffset(x,y)+1),
			*(BYTE*)( GetBitsPtr() + GetPixelOffset(x,y))
			);
	//	{
	//		CString str;
	//		str.Format(_T("三原色分别为%d,%d,%d"),	
	//		*(BYTE*)( GetBitsPtr() + GetPixelOffset(x,y)+2 ),
	//		*(BYTE*)( GetBitsPtr() + GetPixelOffset(x,y)+1 ),
	//		*(BYTE*)( GetBitsPtr() + GetPixelOffset(x,y) )
	//		);
	//		::AfxMessageBox(str);
	//	}
		
		break;
	}

	return cCorlor;
}

BOOL  CDib::ConvertFormat(WORD wBitCount)
{
	if(this->IsEmpty())
		return false;

	if( this->GetBitCount() == wBitCount )
		return true;

	HDIB hNewDIB = ::ConvertDIBFormat( m_hDIB,wBitCount,NULL );
	if(!hNewDIB)
		return false;

	this->Destroy();
	m_hDIB = hNewDIB;
	return true;
}


BOOL CDib::Display(CDC *pDC, int x, int y, DWORD dwRop)
{
	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);


	CBitmap* pOldBmp= MemDC.SelectObject(m_pBitmap);

	CPalette* pOldPal= pDC->SelectPalette(m_pPalette,true);
	pDC->RealizePalette();

	BOOL bSuccess = pDC->BitBlt(
		x,y,
		this->GetWidth(),this->GetHeight(),
		&MemDC,0,0,
		dwRop
		);
	MemDC.SelectObject( pOldBmp );

	pDC->SelectPalette(pOldPal,true);

	return bSuccess;
}





BOOL CDib::Display( CDC* pDC,int xDest,int yDest,int nWidthDest,
				   int nHeightDest,int xSrc,int ySrc,DWORD dwRop)
{
	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);


	CBitmap* pOldBmp= MemDC.SelectObject(m_pBitmap);

	CPalette* pOldPal= pDC->SelectPalette(m_pPalette,true);
	pDC->RealizePalette();

	BOOL bSuccess = pDC->BitBlt(
		xDest,yDest,
		nWidthDest,nHeightDest,
		&MemDC,xSrc,ySrc,
		dwRop
		);


	MemDC.SelectObject( pOldBmp );

	pDC->SelectPalette(pOldPal,true);

	return bSuccess;
}

BOOL CDib::Display( CDC* pDC,
				   int xDest,int yDest,int nWidthDest,int nHeightDest,
				   int xSrc,int ySrc,int nWidthSrc,int nHeightSrc,
				   DWORD dwRop)
{
	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);


	CBitmap* pOldBmp= MemDC.SelectObject(m_pBitmap);

	CPalette* pOldPal= pDC->SelectPalette(m_pPalette,true);
	pDC->RealizePalette();

	BOOL bSuccess=pDC->StretchBlt(
		xDest,yDest,nWidthDest,nHeightDest,
		&MemDC,xSrc,ySrc,nWidthSrc,nHeightSrc,
		dwRop
		);

	MemDC.SelectObject( pOldBmp );

	pDC->SelectPalette(pOldPal,true);

	return bSuccess;
}

BOOL CDib::Display(CDC *pDC, CRect rcDest, CRect rcSrc, DWORD dwRop)
{
	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);


	CBitmap* pOldBmp= MemDC.SelectObject(m_pBitmap);

	CPalette* pOldPal= pDC->SelectPalette(m_pPalette,true);
	pDC->RealizePalette();

	BOOL bSuccess=pDC->StretchBlt(
		rcDest.left,rcDest.top,
		rcDest.Width(),rcDest.Height(),
		&MemDC,
		rcSrc.left,rcSrc.top,
		rcSrc.Width(),rcSrc.Height(),
		dwRop
		);

	MemDC.SelectObject( pOldBmp );

	pDC->SelectPalette(pOldPal,true);

	return bSuccess;
}

BOOL CDib::DisplayPalette(CDC *pDC, CRect rc)
{
	return ::DisplayPalette( pDC->GetSafeHdc(),&rc,(HPALETTE)m_pPalette->GetSafeHandle());
}

CDC* CDib::BeginPaint(CDC *pDC)
{
	m_pMemDC= new CDC;
	m_pMemDC->CreateCompatibleDC(pDC);

	m_pPaletteTmp= m_pMemDC->SelectPalette(m_pPalette,true);
	m_pMemDC->RealizePalette();

	m_pBitmapTmp=(CBitmap*)m_pMemDC->SelectObject( m_pBitmap );
	return m_pMemDC;
}

void CDib::EndPaint()
{
	m_pMemDC->SelectObject(m_pBitmapTmp);
	m_pMemDC->SelectPalette(m_pPaletteTmp,true);
	delete m_pMemDC;

	this->Create(m_hBitmap);
}
BOOL CDib::Display(int nEffect, CDC *pDC, int x, int y, 
				   int nDeta, DWORD dwDelayTime, DWORD dwRop)
{
	if(nEffect == EFFECT_FADE)
		return this->DisplayFadeIn(pDC,x,y,nDeta,dwDelayTime);
	else if(nEffect == EFFECT_MOSAIC)
		return this->DisplayMosaicIn(pDC,x,y,nDeta,dwDelayTime,dwRop);
	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);
	CBitmap* pOldBmp = MemDC.SelectObject(m_pBitmap);

	CPalette* pOldPal= pDC->SelectPalette( m_pPalette,true);
	pDC->RealizePalette();

	LONG i=0;
	BOOL bDone=false;

	while(!bDone)
	{
		switch(nEffect)
		{
		case EFFECT_SCANDOWN:
			if( i>GetHeight())
			{
				i=GetHeight();
				bDone=true;
			}

			pDC->BitBlt( x,y+i,GetWidth(),nDeta,&MemDC,0,i,dwRop);

			i+=nDeta;
			break;
		case EFFECT_SCANUP:
			if( i>GetHeight())
			{
				i=GetHeight();
				bDone=true;
			}

			pDC->BitBlt( x,y+GetHeight()-i,GetWidth(),nDeta,&MemDC,0,GetHeight()-i,dwRop);

			i+=nDeta;
			break;
		case EFFECT_SCANRIGHT:
			if( i>GetWidth())
			{
				i=GetWidth();
				bDone=true;
			}

			pDC->BitBlt(x+i,y,nDeta,GetHeight(),&MemDC,i,0,dwRop);

			i+=nDeta;
			break;

		case EFFECT_SCANLEFT:
			if( i>GetWidth())
			{
				i=GetWidth();
				bDone=true;
			}

			pDC->BitBlt(x+GetWidth()-i,y,nDeta,GetHeight(),&MemDC,GetWidth()-i,0,dwRop);

			i+=nDeta;
			break;


		case EFFECT_VSPLITSCAN:
			if( i>GetHeight()/2)
			{
				i=GetHeight()/2;
				bDone=true;
			}

			pDC->BitBlt( x,y+i,GetWidth(),nDeta,&MemDC,0,i,dwRop);
			pDC->BitBlt( x,y+GetHeight()-i,GetWidth(),nDeta,&MemDC,0,GetHeight()-i,dwRop);

			i+=nDeta;
			break;

		case EFFECT_HSPLITSCAN:
			if( i>GetWidth()/2)
			{
				i=GetWidth()/2;
				bDone=true;
			}

			pDC->BitBlt(x+i,y,nDeta,GetHeight(),&MemDC,i,0,dwRop);
			pDC->BitBlt(x+GetWidth()-i,y,nDeta,GetHeight(),&MemDC,GetWidth()-i,0,dwRop);
			i+=nDeta;
			break;

		case EFFECT_MOVEDOWN:
			if(i>GetHeight())
			{
				i=GetHeight();
				bDone=true;
			}
			pDC->BitBlt(x,y,GetWidth(),nDeta+i,&MemDC,0,GetHeight()-i,dwRop);

			i+=nDeta;
			break;
		case EFFECT_MOVEUP:
			if(i>GetHeight())
			{
				i=GetHeight();
				bDone=true;
			}
			pDC->BitBlt(x,y+GetHeight()-i,GetWidth(),i,&MemDC,0,0,dwRop);

			i+=nDeta;
			break;
		case EFFECT_MOVERIGHT:
			if(i>GetWidth())
			{
				i=GetWidth();
				bDone=true;
			}
			pDC->BitBlt(x,y,i,GetHeight(),&MemDC,GetWidth()-i,0,dwRop);

			i+=nDeta;
			break;
		case EFFECT_MOVELEFT:
			if(i>GetWidth())
			{
				i=GetWidth();
				bDone=true;
			}
			pDC->BitBlt(x+GetWidth()-i,y,i,GetHeight(),&MemDC,0,0,dwRop);

			i+=nDeta;
			break;
		case EFFECT_VSPLITMOVE:

			if(i>GetHeight()/2)
			{
				i=GetHeight()/2;
				bDone=true;
			}
			pDC->BitBlt(x,y,GetWidth(),nDeta+i,&MemDC,0,GetHeight()/2-i,dwRop);
			pDC->BitBlt(x,y+GetHeight()-i,GetWidth(),i+nDeta,&MemDC,0,GetHeight()/2,dwRop);

			i+=nDeta;
			break;
	case EFFECT_HSPLITMOVE:
			if(i>GetWidth()/2)
			{
				i=GetWidth()/2;
				bDone=true;
			}
			pDC->BitBlt(x,y,i+1,GetHeight(),&MemDC,GetWidth()/2-i,0,dwRop);
			pDC->BitBlt(x+GetWidth()-i,y,i+1,GetHeight(),&MemDC,GetWidth()/2,0,dwRop);

			i+=nDeta;
			break;

	case EFFECT_VCROSSMOVE:

			if(i>GetHeight())
			{
				i=GetHeight();
				bDone=true;
			}
			pDC->BitBlt(x,y,GetWidth()/2,i,&MemDC,0,GetHeight()-i,dwRop);
			pDC->BitBlt(x+GetWidth()/2,y+GetHeight()-i,GetWidth()/2+1,
				i,&MemDC,GetWidth()/2,0,dwRop);

			i+=nDeta;
			break;



			i+=nDeta;
			break;
	case EFFECT_HCROSSMOVE:
		if(i>GetWidth())
			{
				i=GetWidth();
				bDone=true;
			}
			pDC->BitBlt(x,y,i,GetHeight()/2,&MemDC,GetWidth()-i,0,dwRop);
			pDC->BitBlt(x+GetWidth()-i,y+GetHeight()/2,i,GetHeight()/2+1,&MemDC,
				0,GetHeight()/2,dwRop);


			i+=nDeta;
			break;

	case EFFECT_VRASTER:
		{
			if(i>GetHeight())
			{
				i=GetHeight();
				bDone=true;
			}

			LONG j=0;
			BOOL bQuitLoop=false;
			while(!bQuitLoop)
			{
				if(j>GetWidth())
				{
					j=GetWidth();
					bQuitLoop=true;
				}

				pDC->BitBlt(x+j,y,nDeta,i,&MemDC,j,GetHeight()-i,dwRop);

				j+=nDeta;
				if(j>GetWidth())
				{
					j=GetWidth();
					bQuitLoop=true;
				}

				pDC->BitBlt(x+j,y+GetHeight()-i,nDeta,i,&MemDC,
					        j,0,dwRop);
				j+=nDeta;
			}

			i++;
			break;
		}
	case EFFECT_HRASTER:

		{
			if(i>GetWidth())
			{
				i=GetWidth();
				bDone=true;
			}

			LONG j=0;
			BOOL bQuitLoop=false;
			while(!bQuitLoop)
			{
				if(j>GetHeight())
				{
					j=GetHeight();
					bQuitLoop=true;
				}

				pDC->BitBlt(x,y+j,i,nDeta,&MemDC,GetWidth()-i,j,dwRop);

				j+=nDeta;
				if(j>GetHeight())
				{
					j=GetHeight();
					bQuitLoop=true;
				}

				pDC->BitBlt(x+GetWidth()-i,y+j,i,nDeta,&MemDC,
					        0,j,dwRop);
				j+=nDeta;
			}

			i++;
			break;
		}
	case EFFECT_HBLIND:
		{
			if(i>nDeta)
			{
				i=nDeta;
				bDone=true;
			}

			for(LONG j=i;j<=GetHeight();j+=nDeta)
				pDC->BitBlt(x,y+j,GetWidth(),1,&MemDC,0,j,dwRop);
			i++;
			break;
		}
	case EFFECT_VBLIND:
		{
			if(i>nDeta)
			{
				i=nDeta;
				bDone=true;
			}

			for(LONG j=i;j<=GetWidth();j+=nDeta)
				pDC->BitBlt(x+j,y,1,GetHeight(),&MemDC,j,0,dwRop);
			i++;
			break;
		}

		default:
			bDone=true;
			break;
		}

		::Delay(dwDelayTime);

	}
	MemDC.SelectObject(pOldBmp);
	pDC->SelectPalette(pOldPal,true);

	return true;
}
BOOL CDib::Clear(int nEffect, CDC *pDC, int x, int y, int nDeta, DWORD dwDelayTime)
{
	if(nEffect == EFFECT_FADE)
		return this->DisplayFadeOut(pDC,x,y,nDeta,dwDelayTime);
	else if(nEffect == EFFECT_MOSAIC)
		return this->DisplayMosaicOut(pDC,x,y,nDeta,dwDelayTime);
	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);
	CBitmap* pOldBmp=MemDC.SelectObject( m_pBitmap);

	CPalette* pOldPal = pDC->SelectPalette(m_pPalette,true);
	pDC->RealizePalette();

	pDC->BitBlt( x,y,GetWidth(),GetHeight(),&MemDC,0,0,SRCCOPY);


	CBrush Brush(::GetSysColor(COLOR_WINDOW));
	CBrush* pOldBrush = pDC->SelectObject( &Brush);
	CPen Pen( PS_SOLID,1,GetSysColor(COLOR_WINDOW));
	CPen* pOldPen = pDC->SelectObject( &Pen);

	LONG i=0;
	BOOL bDone = false;

	while(!bDone)
	{
		switch(nEffect)
		{
		case EFFECT_SCANDOWN:
			if(i>GetHeight())
			{
				i=GetHeight();
				bDone=true;
			}
			pDC->Rectangle(x,y+i,x+GetWidth(),y+i+nDeta);
			i+= nDeta;
			break;

		case EFFECT_SCANUP:
			if(i>GetHeight())
			{
				i=GetHeight();
				bDone=true;
			}
			pDC->Rectangle(x,y+GetHeight()-i,x+GetWidth(),y+GetHeight()-i+nDeta);
			i+= nDeta;
			break;

		case EFFECT_SCANRIGHT:
			if(i>GetWidth())
			{
				i=GetWidth();
				bDone=true;
			}
			pDC->Rectangle(x+i,y,x+i+nDeta,y+GetHeight());
			i+= nDeta;
			break;

		case EFFECT_SCANLEFT:
			if(i>GetWidth())
			{
				i=GetWidth();
				bDone=true;
			}
			pDC->Rectangle( x+GetWidth()-i,y,
				            x+GetWidth()-i+nDeta,y+GetHeight());
			i+= nDeta;
			break;

		case EFFECT_VSPLITSCAN:
			if(i>GetHeight()/2)
			{
				i=GetHeight()/2;
				bDone=true;
			}
			pDC->Rectangle(x,y+i,x+GetWidth(),y+i+nDeta);
			pDC->Rectangle(x,y+GetHeight()-i,x+GetWidth(),y+GetHeight()-i+nDeta);

			i+= nDeta;
			break;

		case EFFECT_HSPLITSCAN:
			if(i>GetWidth()/2)
			{
				i=GetWidth()/2;
				bDone=true;
			}
			pDC->Rectangle(x+i,y,x+i+nDeta,y+GetHeight());
			pDC->Rectangle( x+GetWidth()-i,y,
				            x+GetWidth()-i+nDeta,y+GetHeight());

			i+= nDeta;
			break;
		case EFFECT_MOVEDOWN:
			{
				i+= nDeta;
				if(i>GetHeight())
				{
					i=GetHeight();
					bDone=true;
				}
				pDC->BitBlt( x,y+i,GetWidth(),GetHeight()-i,&MemDC,0,0,SRCCOPY);
				pDC->Rectangle( x,y+i-nDeta,x+GetWidth(),y+i);
				break;
			}
		case EFFECT_MOVEUP:
			{
				i+= nDeta;
				if(i>GetHeight())
				{
					i=GetHeight();
					bDone=true;
				}
				pDC->BitBlt( x,y,GetWidth(),GetHeight()-i,&MemDC,0,i,SRCCOPY);
				pDC->Rectangle( x,y+GetHeight()-i,x+GetWidth(),y+GetHeight());
				break;
			}
		case EFFECT_MOVERIGHT:
			{
				i+= nDeta;
				if(i>GetWidth())
				{
					i=GetWidth();
					bDone=true;
				}
				pDC->BitBlt( x+i,y,GetWidth()-i,GetHeight(),&MemDC,0,0,SRCCOPY);
				pDC->Rectangle( x+i-nDeta,y,x+i,y+GetHeight());
				break;
		    }
		case EFFECT_MOVELEFT:
			{
				i+= nDeta;
				if(i>GetWidth())
				{
					i=GetWidth();
					bDone=true;
				}
				pDC->BitBlt( x,y,GetWidth()-i,GetHeight(),&MemDC,i,0,SRCCOPY);
				pDC->Rectangle( x+GetWidth()-i,y,x+GetWidth(),y+GetHeight());
				break;
		    }
		case EFFECT_VCROSSMOVE:
			{
				i+= nDeta;
				if(i>GetHeight())
				{
					i=GetHeight();
					bDone=true;
				}
				pDC->BitBlt( x,y+i,GetWidth()/2,GetHeight()-i,&MemDC,0,0,SRCCOPY);
				pDC->Rectangle( x,y+i-nDeta,x+GetWidth(),y+i);

				pDC->BitBlt( x+GetWidth()/2,y,GetWidth()/2,GetHeight()-i,
					        &MemDC,GetWidth()/2,i,SRCCOPY);
				pDC->Rectangle( x+GetWidth()/2,y+GetHeight()-i,x+GetWidth(),y+GetHeight());
				break;
			}
		case EFFECT_HCROSSMOVE:
			{
				i+= nDeta;
				if(i>GetWidth())
				{
					i=GetWidth();
					bDone=true;
				}
				pDC->BitBlt( x+i,y,GetWidth()-i,GetHeight()/2,&MemDC,0,0,SRCCOPY);
				pDC->Rectangle( x,y,x+i,y+GetHeight()/2);

				pDC->BitBlt( x,y+GetHeight()/2,GetWidth()-i,GetHeight()/2,
					        &MemDC,i,GetHeight()/2,SRCCOPY);
				pDC->Rectangle( x+GetWidth()-i,y+GetHeight()/2,x+GetWidth(),y+GetHeight());
				break;
			}
		case EFFECT_VSPLITMOVE:
			{
				i+=nDeta;
				if(i>GetHeight()/2)
				{
					i=GetHeight()/2;
					bDone=true;
				}

				pDC->BitBlt( x,y,GetWidth(),GetHeight()/2-i,&MemDC,
					         0,i,SRCCOPY);

                pDC->BitBlt( x,y+GetHeight()/2+i,GetWidth(),GetHeight()/2-i,&MemDC,
					         0,GetHeight()/2,SRCCOPY);

				pDC->Rectangle(x,y+GetHeight()/2-i,x+GetWidth(),y+GetHeight()/2+i+1);

				break;
			}
		case EFFECT_HSPLITMOVE:
			{
				i+=nDeta;
				if(i>GetWidth()/2)
				{
					i=GetWidth();
					bDone=true;
				}

				pDC->BitBlt( x,y,GetWidth()/2-i,GetHeight(),&MemDC,
					         i,0,SRCCOPY);

                pDC->BitBlt( x+GetWidth()/2+i,y,GetWidth()/2-i,GetHeight(),&MemDC,
					         GetWidth()/2,0,SRCCOPY);

				pDC->Rectangle( x+GetWidth()/2-i,y,x+GetWidth()/2+i,y+GetHeight());

				break;
			}
		case  EFFECT_VRASTER:
		{

			if(i>GetHeight())
			{
				i=GetHeight();
				bDone=true;
			}

			LONG j=0;
			BOOL bQuitLoop=false;

			while(!bQuitLoop)
			{
				if(j>GetWidth())
				{
					j=GetWidth();
					bQuitLoop=true;
				}
				pDC->BitBlt( x+j,y+i,nDeta,GetHeight()-i,&MemDC,j,0,SRCCOPY);
				pDC->Rectangle( x+j,y,x+j+nDeta,y+i);
				j+=nDeta;


				
				if(j>GetWidth())
				{
					j=GetWidth();
					bQuitLoop=true;
				}

				pDC->BitBlt( x+j,y,nDeta,GetHeight()-i,&MemDC,j,i,SRCCOPY);
				pDC->Rectangle( x+j,y+GetHeight()-i,x+j+nDeta,y+GetHeight());
				j+=nDeta;
			}

			i++;
			break;
		}
		case EFFECT_HRASTER:

		{
			if(i>GetWidth())
			{
				i=GetWidth();
				bDone=true;
			}

			LONG j=0;
			BOOL bQuitLoop=false;
			while(!bQuitLoop)
			{
				if(j>GetHeight())
				{
					j=GetHeight();
					bQuitLoop=true;
				}

				pDC->BitBlt( x+i,y+j,GetWidth()-i,nDeta,&MemDC,0,j,SRCCOPY);
				pDC->Rectangle( x,y+j,x+i,y+j+nDeta);

				j+=nDeta;
				if(j>GetHeight())
				{
					j=GetHeight();
					bQuitLoop=true;
				}

				pDC->BitBlt( x,y+j,GetWidth()-i,nDeta,&MemDC,i,j,SRCCOPY);
				pDC->Rectangle( x+GetWidth()-i,y+j,x+GetWidth(),y+j+nDeta);
				j+=nDeta;
			}

			i++;
			break;
		}
		case EFFECT_HBLIND:
		{
			if(i>nDeta)
			{
				i=nDeta;
				bDone=true;
			}

			for(LONG j=i;j<=GetHeight();j+=nDeta)
				pDC->Rectangle( x,y+j,x+GetWidth(),y+j+1);
			i++;
			break;
		}
		case EFFECT_VBLIND:
		{
			if(i>nDeta)
			{
				i=nDeta;
				bDone=true;
			}

			for(LONG j=i;j<=GetWidth();j+=nDeta)
				pDC->Rectangle( x+j,y,x+j+1,y+GetHeight());
			i++;
			break;
		}










		default: bDone=true;
			break;

		}
	Delay(dwDelayTime);
		
	}

	MemDC.SelectObject( pOldBmp);
	pDC->SelectPalette( pOldPal,true );
	pDC->SelectObject( pOldBrush);
	pDC->SelectObject( pOldPen);

	return true;
	
}
BOOL CDib::DisplayMosaicIn(CDC *pDC, int xx, int yy, int nTileSize, 
						   DWORD dwDelayTime, DWORD dwRop)
{


	 DWORD dwTileNum = ( GetWidth()/nTileSize + ( GetWidth()%nTileSize ? 1:0 ))*
		                (GetHeight()/nTileSize + ( GetHeight()%nTileSize ? 1:0 ));



	POINT *point =new POINT[dwTileNum];

	if( point==NULL)
		return false;

	LONG x=0,y=0;

	for( DWORD i=0;i<dwTileNum;++i)
	{
		point[i].x=x;
		point[i].y=y;

		x+= nTileSize;
		if( x>GetWidth())
		{
			x=0;
			y+= nTileSize;
		}
	}

	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);
	CBitmap* pOldBmp=MemDC.SelectObject( m_pBitmap);

	CPalette* pOldPal = pDC->SelectPalette(m_pPalette,true);
	pDC->RealizePalette();


	double fMax= RAND_MAX;

	BOOL bDone = false;

	while(!bDone)
	{
		
		DWORD n= (DWORD) ((double)dwTileNum*(double)((double)::rand()/fMax));

	


	    x=point[n].x;
		y=point[n].y;



		pDC->BitBlt( xx+x,yy+y,nTileSize,nTileSize,&MemDC,x,y,dwRop);
		dwTileNum--;
		bDone = !dwTileNum;
		
    
		point[n].x= point[dwTileNum].x;
		point[n].y= point[dwTileNum].y;
		  

		Delay(dwDelayTime);
	}

       
	MemDC.SelectObject( pOldBmp);
	pDC->SelectPalette( pOldPal,true );



	delete[]point;
	return true;
}

BOOL CDib::DisplayMosaicOut(CDC *pDC, int xx, int yy, int nTileSize, DWORD dwDelayTime)
{

	
	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);
	CBitmap* pOldBmp=MemDC.SelectObject( m_pBitmap);

	CPalette* pOldPal = pDC->SelectPalette(m_pPalette,true);
	pDC->RealizePalette();

	pDC->BitBlt( xx,yy,GetWidth(),GetHeight(),&MemDC,0,0,SRCCOPY);

	 DWORD dwTileNum = ( GetWidth()/nTileSize + ( GetWidth()%nTileSize ? 1:0 ))*
		                (GetHeight()/nTileSize + ( GetHeight()%nTileSize ? 1:0 ));



	POINT *point =new POINT[dwTileNum];

	if( point==NULL)
		return false;

	LONG x=0,y=0;

	for( DWORD i=0;i<dwTileNum;++i)
	{
		point[i].x=x;
		point[i].y=y;

		x+= nTileSize;
		if( x>GetWidth())
		{
			x=0;
			y+= nTileSize;
		}
	}

	CBrush Brush(::GetSysColor(COLOR_WINDOW));
	CBrush* pOldBrush = pDC->SelectObject( &Brush);
	CPen Pen( PS_SOLID,1,GetSysColor(COLOR_WINDOW));
	CPen* pOldPen = pDC->SelectObject( &Pen);

	double fMax= RAND_MAX;

	BOOL bDone = false;

	while(!bDone)
	{
		
		DWORD n= (DWORD) ((double)dwTileNum*(double)((double)::rand()/fMax));

	


	    x=point[n].x;
		y=point[n].y;



		pDC->Rectangle( xx+x,yy+y,xx+x+nTileSize+1,yy+y+nTileSize+1);
		dwTileNum--;
		bDone = !dwTileNum;
		
    
		point[n].x= point[dwTileNum].x;
		point[n].y= point[dwTileNum].y;
		  

		Delay(dwDelayTime);
	}

       
	MemDC.SelectObject( pOldBmp);
	pDC->SelectPalette( pOldPal,true );
	pDC->SelectObject( pOldBrush);
	pDC->SelectObject( pOldPen);


	delete[]point;
	return true;
}

BOOL CDib::DisplayFadeIn(CDC *pDC, int x, int y, int nDeta, DWORD dwDelayTime)
{
	HDIB hDIB = CopyHandle( m_hDIB);

	WORD wNumColors = this->GetColorNumber();

	BITMAPINFO *pBmi = (BITMAPINFO*)::GlobalLock( m_hDIB );
	BITMAPINFO *pBmi0 = (BITMAPINFO*)::GlobalLock( hDIB );

	if( !pBmi )
		return false;

	if( !pBmi0 )
		return false;

	BOOL bDone = false;
	int nDeltaR , nDeltaG, nDeltaB;

	if( wNumColors)
	{
		for( WORD i=0; i<wNumColors; i++)
		{
			pBmi->bmiColors[i].rgbRed=0;
			pBmi->bmiColors[i].rgbGreen=0;
			pBmi->bmiColors[i].rgbBlue=0;
		}

		this->UpdateInternal();
		this->Display( pDC,x,y);


		while( !bDone )
		{
			bDone = true;
			for(WORD i=0; i<wNumColors; i++)
			{
				nDeltaR = UPVALUE( pBmi0->bmiColors[i].rgbRed-
					              pBmi->bmiColors[i].rgbRed ,nDeta );
				nDeltaG = UPVALUE( pBmi0->bmiColors[i].rgbGreen-
					              pBmi->bmiColors[i].rgbGreen ,nDeta );
				nDeltaB = UPVALUE( pBmi0->bmiColors[i].rgbBlue-
					              pBmi->bmiColors[i].rgbBlue ,nDeta );

				if( !::Fade( nDeltaR,nDeltaG,nDeltaB,
					         pBmi0->bmiColors[i].rgbRed,
							 pBmi0->bmiColors[i].rgbGreen,
							 pBmi0->bmiColors[i].rgbBlue,

							 &(pBmi->bmiColors[i].rgbRed),
							 &(pBmi->bmiColors[i].rgbGreen),
							 &(pBmi->bmiColors[i].rgbBlue)
							 )  )
				bDone = false;
			}

			this->UpdateInternal();
			this->Display( pDC,x,y);
			Delay(dwDelayTime);
			nDeta--;
		}
	}


	else
	{
		dwDelayTime /= 10;

		LPBITMAPINFOHEADER lpBi = (LPBITMAPINFOHEADER) pBmi;
		LPBYTE lpBits = (LPBYTE)lpBi + lpBi->biSize;
		LPBYTE lpBits0 = (LPBYTE)pBmi0 + lpBi->biSize;

		int nDelta = WIDTHBYTES( lpBi->biWidth * lpBi->biBitCount )-
			                     lpBi->biWidth * lpBi->biBitCount / 8;
		int nx,ny;

		for(ny=0; ny<lpBi->biHeight; ny++)
		{
			for(nx=0; nx<lpBi->biWidth; nx++)
			{
				*lpBits++ = 0;
				*lpBits++ = 0;
				*lpBits++ = 0;
			}

			lpBits += nDelta;
		}

		lpBits = (LPBYTE)lpBi + lpBi->biSize;
		this->UpdateInternal();
		this->Display(pDC, x, y);


		BYTE r, g, b, r0, g0, b0;
		
		while(!bDone)
		{
			bDone = true;
			for(ny=0; ny<lpBi->biHeight; ny++)
			{
				for(nx=0; nx<lpBi->biWidth; nx++)
				{
					r0=(BYTE)*lpBits0++;
					g0=(BYTE)*lpBits0++;
					b0=(BYTE)*lpBits0++;
					r =(BYTE)*lpBits;
					g =(BYTE)*(lpBits + 1);
					b =(BYTE)*(lpBits + 2);

					nDeltaR = UPVALUE(r0-r,nDeta);
					nDeltaG = UPVALUE(g0-g,nDeta);
					nDeltaB = UPVALUE(b0-b,nDeta);

					if( !Fade(nDeltaR, nDeltaG, nDeltaB,r0,g0,b0,&r,&g,&b))
						bDone =false;

					*lpBits++ = r;
					*lpBits++ = g;
					*lpBits++ = b;


				}


				lpBits += nDelta;
				lpBits0 += nDelta;

			}
			lpBits = (LPBYTE)lpBi + lpBi->biSize;
		    lpBits0 = (LPBYTE)pBmi0 + lpBi->biSize;
			this->UpdateInternal();


		    this->Display(pDC, x, y);
			Delay(dwDelayTime);
			nDeta--;
		}
	}

		::GlobalUnlock(hDIB);
		::GlobalUnlock(m_hDIB);

		m_hDIB = CopyHandle(hDIB);
		::GlobalFree(hDIB);
		return true;
}

BOOL CDib::DisplayFadeOut(CDC *pDC, int x, int y, int nDeta, DWORD dwDelayTime)
{
	Display(pDC, x, y );

	HDIB hDIB = CopyHandle( m_hDIB);

	WORD wNumColors = this->GetColorNumber();

	BITMAPINFO *pBmi = (BITMAPINFO*)::GlobalLock( m_hDIB );
//	BITMAPINFO *pBmi0 = (BITMAPINFO*)::GlobalLock( hDIB );

	if( !pBmi )
		return false;

//	if( !pBmi0 )
//		return false;

	BOOL bDone = false;
	int nDeltaR , nDeltaG, nDeltaB;

	if( wNumColors)
	{
//		for( WORD i=0; i<wNumColors; i++)
//		{
//			pBmi->bmiColors[i].rgbRed=0;
//			pBmi->bmiColors[i].rgbGreen=0;
//			pBmi->bmiColors[i].rgbBlue=0;
//		}
//
//		this->UpdateInternal();
//		this->Display( pDC,x,y);


		while( !bDone )
		{
			bDone = true;
			for(WORD i=0; i<wNumColors; i++)
			{
				nDeltaR = -UPVALUE( pBmi->bmiColors[i].rgbRed ,nDeta );
				nDeltaG = -UPVALUE( pBmi->bmiColors[i].rgbGreen ,nDeta );
				nDeltaB = -UPVALUE( pBmi->bmiColors[i].rgbBlue ,nDeta );

				if( !::Fade( nDeltaR,nDeltaG,nDeltaB,
					         0,0,0,
							 &(pBmi->bmiColors[i].rgbRed),
							 &(pBmi->bmiColors[i].rgbGreen),
							 &(pBmi->bmiColors[i].rgbBlue)
							 )  )
				bDone = false;
			}

			this->UpdateInternal();
			this->Display( pDC,x,y);
			Delay(dwDelayTime);
			nDeta--;
		}
	}


	else
	{
		dwDelayTime /= 10;

		LPBITMAPINFOHEADER lpBi = (LPBITMAPINFOHEADER) pBmi;
		LPBYTE lpBits = (LPBYTE)lpBi + lpBi->biSize;
//		LPBYTE lpBits0 = (LPBYTE)pBmi0 + lpBi->biSize;

		int nDelta = WIDTHBYTES( lpBi->biWidth * lpBi->biBitCount )-
			                     lpBi->biWidth * lpBi->biBitCount / 8;
		int nx,ny;

//		for(ny=0; ny<lpBi->biHeight; ny++)
//		{
//			for(nx=0; nx<lpBi->biWidth; nx++)
//			{
//				*lpBits++ = 0;
//				*lpBits++ = 0;
//				*lpBits++ = 0;
//			}

//			lpBits += nDelta;
//		}

		lpBits = (LPBYTE)lpBi + lpBi->biSize;
		this->UpdateInternal();
		this->Display(pDC, x, y);


		BYTE r, g, b; //r0, g0, b0;
		
		while(!bDone)
		{
			bDone = true;
			for(ny=0; ny<lpBi->biHeight; ny++)
			{
				for(nx=0; nx<lpBi->biWidth; nx++)
				{
	//				r0=(BYTE)*lpBits0++;
	//				g0=(BYTE)*lpBits0++;
	//				b0=(BYTE)*lpBits0++;
					r =(BYTE)*lpBits;
					g =(BYTE)*(lpBits + 1);
					b =(BYTE)*(lpBits + 2);

					nDeltaR = -UPVALUE(r,nDeta);
					nDeltaG = -UPVALUE(g,nDeta);
					nDeltaB = -UPVALUE(b,nDeta);

					if( !Fade(nDeltaR, nDeltaG, nDeltaB,0,0,0,&r,&g,&b))
						bDone =false;

					*lpBits++ = r;
					*lpBits++ = g;
					*lpBits++ = b;


				}


				lpBits += nDelta;
//				lpBits0 += nDelta;

			}
			lpBits = (LPBYTE)lpBi + lpBi->biSize;
//		    lpBits0 = (LPBYTE)pBmi0 + lpBi->biSize;
			this->UpdateInternal();

		    this->Display(pDC, x, y);
			Delay(dwDelayTime);
			nDeta--;
		}
	}

//		::GlobalUnlock(hDIB);
		::GlobalUnlock(m_hDIB);

		m_hDIB = CopyHandle(hDIB);
		::GlobalFree(hDIB);
//		this->UpdateInternal();
		return true;
}

BOOL CDib::DisplayTransparent(CDC *pDC, int x, int y, COLORREF cTransparentColor)
{
	CPalette* pOldPal = pDC->SelectPalette(m_pPalette,true);
	pDC->RealizePalette();
	::DrawTransparentBitmap(pDC->GetSafeHdc(),m_hBitmap,x,y,cTransparentColor);
	pDC->SelectPalette(pOldPal,true);
	return true;
}



				








							 






	










		                




























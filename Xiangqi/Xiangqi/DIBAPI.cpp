//DIBAPI.cpp
//操作DIB
#pragma once
#include "stdafx.h"
#include "DIBAPI.h"
#include <math.h>
/*
#define IS_WIN30_DIB(lpbi) ((*(LPDWORD)lpbi)==sizeof(BITMAPINFOHEADER))
#define WIDTHBYTES(bits) (((bits+31)/32)*4)
#define HDIB HANDLE
HANDLE LoadDIB(LPCTSTR lpFileName);
HANDLE ReadDIBFile(HANDLE hFile);
LPBYTE LoadDIBFromResource(LPTSTR lpszBitmap);
HPALETTE CreateDIBPalette(LPBYTE lpBi);
WORD DIBNumColors(LPBYTE lpDIB);
BOOL PaintDIB(HDC hDC,LPRECT lpDCRect,HANDLE hDIB,LPRECT lpDIBRect,
			  HPALETTE hPal,DWORD dwRop);
BOOL DisplayDIB(CDC* pDC,LPTSTR lpszBitmap);
BOOL SaveDIB(HANDLE hDIB,LPCTSTR lpFileName);
HDIB CreateDIB(DWORD dwWidth,DWORD dwHeight,WORD wBitCount);
HDIB CreateDefaultDIB(DWORD dwWidth,DWORD dwHeight);
HPALETTE CopyPalette(HPALETTE hPalScr);
BOOL DisplayPalette(HDC hDC,LPRECT lpRect,HPALETTE hPal);
HPALETTE GetSystemPalette();
void DestroyDIB(HDIB hDIB);
DWORD BytesPerLine(LPBYTE lpDIB);
DWORD BytesPerLine(HDIB hDIB);
DWORD DIBBlockSize(LPBYTE lpDIB);
DWORD DIBBlockSize(HDIB hDIB);
DWORD DIBHeight(LPBYTE lpDIB);
DWORD DIBHeight(HDIB hDIB);
DWORD DIBWidth(LPBYTE lpDIB);
DWORD DIBWidth(HDIB hDIB);
WORD DIBNumColors(HDIB hDIB);
WORD DIBBitCount(LPBYTE lpDIB);
WORD DIBBitCount(HDIB hDIB);
LPBYTE FindDIBBits(LPBYTE lpDIB);
WORD PaletteSize(LPBYTE lpDIB);
WORD PaletteSize(HDIB hDIB);
HBITMAP DIBToDIBSection(LPBYTE lpDIB);
HBITMAP DIBToDIBSection(HDIB hDIB);
HDIB DIBSectionToDIB(HBITMAP hBitmap);
HDIB ConvertDIBFormat(LPBYTE lpSrcDIB,UINT nWidth,UINT nHeight,
                      UINT nbpp,BOOL bStretch,HPALETTE hPalSrc);
HDIB ConvertDIBFormat(HDIB hDIB,UINT nWidth,UINT nHeight,
                      UINT nbpp,BOOL bStretch,HPALETTE hPalSrc);
HDIB ConvertDIBFormat(LPBYTE lpStreDIB,UINT nbpp,HPALETTE hPalSrc);
HDIB ConvertDIBFormat(HDIB hDIB,UINT nbpp,HPALETTE hPalSrc);
HDIB BitmapToDIB(HBITMAP hDDB,HPALETTE hPal);
HDIB BitmapToDIB(HBITMAP hDDB,HPALETTE hPal,WORD wBitCount);
HDIB ChangeBitmapFormat(HBITMAP hDDB,WORD biBits,
                         DWORD biCompression,HPALETTE hPal);
HBITMAP DIBToBitmap(HDIB hDIB,HPALETTE hPal);
HDIB ChangeDIBFormat(HDIB hDIB,WORD wBitCount,DWORD dwCompression);
HBITMAP CopySreenToBitmap(LPRECT);
HBITMAP CopyWindowToBitmap(HWND hWnd,WORD fPrintArea);
HBITMAP CopyClientRectToBitmap(HWND hWnd,LPRECT lpRect);
HDIB CopySreenToDIB(LPRECT);
HDIB CopyWindowToDIB(HWND hWnd,WORD fPrintArea);
HDIB CopyClientRectToDIB(HWND hWnd,LPRECT lpRect);

*/
HANDLE LoadDIB(LPCTSTR lpFileName)
{
	HANDLE hDIB;
	HANDLE hFile;
	::SetCursor(::LoadCursor(NULL,IDC_WAIT));
	if((hFile=::CreateFile(lpFileName,
		                   GENERIC_READ,
						   FILE_SHARE_READ,
						   NULL,
						   OPEN_EXISTING,
						   FILE_ATTRIBUTE_NORMAL|
						   FILE_FLAG_SEQUENTIAL_SCAN,
						   NULL))!=INVALID_HANDLE_VALUE)
	{
		hDIB=ReadDIBFile(hFile);
		::CloseHandle(hFile);
		::SetCursor(::LoadCursor(NULL,IDC_ARROW));
		return hDIB;
	}
	else
	{
		::AfxMessageBox(_T("在LoadDIB，无法获得文件句柄"));
		::SetCursor(::LoadCursor(NULL,IDC_ARROW));
		return NULL;
	}
}
HANDLE ReadDIBFile(HANDLE hFile)
{
	BITMAPFILEHEADER bmfHeader;
	DWORD dwBitsSize;
	UINT nNumColors;
	HANDLE hDIB;
	HANDLE hDIBmp;
	LPBITMAPINFOHEADER lpBi;
	DWORD dwOffBits;
	DWORD dwRead;

	dwBitsSize=::GetFileSize(hFile,NULL);
	hDIB=::GlobalAlloc(GMEM_MOVEABLE,(DWORD)(sizeof(BITMAPINFOHEADER)+
		                             256*sizeof(RGBQUAD)));
	if(!hDIB)
	{
		::AfxMessageBox(_T("在ReadDIBFile中，无法为句柄分配内存"));
		return NULL;
	}
	lpBi=(LPBITMAPINFOHEADER)::GlobalLock(hDIB);
	if(!lpBi)
	{
		::GlobalFree(hDIB);
		::AfxMessageBox(_T("在ReadDIBFile中，无法将句柄转化为指针"));
		return NULL;
	}
	if(!::ReadFile( hFile,
		            (LPBYTE)&bmfHeader,
					sizeof(BITMAPFILEHEADER),
					&dwRead,
					NULL))
	{
		::AfxMessageBox(_T("在ReadDIBFile中，无法从文件中读取数据"));
		goto ErrExit;
	}
	if(bmfHeader.bfType!=0x4d42)
	{   
		::AfxMessageBox(_T("在ReadDIBFile中，文件类型不符"));
		goto ErrExit;
	}


	if(!::ReadFile( hFile,
		            (LPBYTE)lpBi,
					sizeof(BITMAPINFOHEADER),
					&dwRead,
					NULL))
	{
		::AfxMessageBox(_T("在ReadDIBFile中，无法读取文件信息"));
	    goto ErrExit;
	}

	if(sizeof(BITMAPINFOHEADER)!=dwRead)
	{
		::AfxMessageBox(_T("在ReadDIBFile中，读取文件信息过程，读取字节数不符"));
		goto ErrExit;
	}


	if(lpBi->biSize==sizeof(BITMAPCOREHEADER))
		goto ErrExit;


		           
	if(!(nNumColors=(UINT)lpBi->biClrUsed))
	{
		if(lpBi->biBitCount!=24)
			nNumColors=1<<lpBi->biBitCount;
	}
	if(lpBi->biClrUsed==0)
		lpBi->biClrUsed=nNumColors;
	if(lpBi->biSizeImage==0)
		lpBi->biSizeImage=WIDTHBYTES(lpBi->biWidth*lpBi->biBitCount) *lpBi->biHeight;


	::GlobalUnlock(hDIB);
	hDIBmp=::GlobalReAlloc(hDIB,lpBi->biSize+
		                           nNumColors*sizeof(RGBQUAD)+
								   lpBi->biSizeImage,
								   0);
	if(!hDIBmp)
		goto ErrExitNoUnlock;
	else
		hDIB=hDIBmp;

	lpBi=(LPBITMAPINFOHEADER)::GlobalLock(hDIB);
	ReadFile( hFile,
		     (LPBYTE)lpBi+lpBi->biSize,
		     nNumColors*sizeof(RGBQUAD),
			  &dwRead,
			  NULL);

	dwOffBits=lpBi->biSize+
		        nNumColors*sizeof(RGBQUAD);

	::SetFilePointer(hFile,bmfHeader.bfOffBits,NULL,FILE_BEGIN);
	if(::ReadFile(hFile,(LPBYTE)lpBi+dwOffBits,lpBi->biSizeImage,&dwRead,NULL))
		goto OKExit;


ErrExit:
	::GlobalUnlock(hDIB);
ErrExitNoUnlock:
	::GlobalFree(hDIB);
	return NULL;
OKExit:
	::GlobalUnlock(hDIB);
	return hDIB;
}
LPBYTE LoadDIBFromResource(LPTSTR lpszBitmap)
{
	HINSTANCE hInst=::AfxGetInstanceHandle();
	HRSRC hRes=::FindResource(hInst,lpszBitmap,_T("DIB"));
	if(hRes==NULL)
		return NULL;
	HGLOBAL hData=::LoadResource(hInst,hRes);

	return (LPBYTE)::LockResource(hData);
}
HPALETTE CreateDIBPalette(LPBYTE lpBi)
{
	LPLOGPALETTE lpLogPal;
	HANDLE hLogPal;
	HPALETTE hPal=NULL;
	LPBITMAPINFO lpBmi;
	LPBITMAPCOREINFO lpBmc;
	BOOL bWinStyleDIB;
	int i,wNumColors;

	if(!lpBi)
	{
		  ::AfxMessageBox(_T("位于创位图调色板函数中，无位图句柄无效"));
		return NULL;
	}
	lpBmi=(LPBITMAPINFO)lpBi;
	lpBmc=(LPBITMAPCOREINFO)lpBi;


	wNumColors=DIBNumColors(lpBi);


	bWinStyleDIB=IS_WIN30_DIB(lpBi);
	

	if(wNumColors)
	{
//		::AfxMessageBox(_T("位于创位图调色板函数中，位图颜色不为零"));
		hLogPal=::GlobalAlloc(GHND,sizeof(LOGPALETTE)+
			                       sizeof(PALETTEENTRY)*wNumColors);
		if(!hLogPal)
		{
//			::AfxMessageBox(_T("位于创位图调色板函数中，逻辑调色板句柄无效"));
			return NULL;
		}

		lpLogPal=(LPLOGPALETTE)::GlobalLock(hLogPal);


		lpLogPal->palVersion=0x300;
		lpLogPal->palNumEntries=wNumColors;
//		CString str;
//		str.Format(_T("颜色数为%d"),wNumColors);
//		::AfxMessageBox(str);

		for(i=0;i<wNumColors;i++)
		{
			if(bWinStyleDIB)
			{
				lpLogPal->palPalEntry[i].peRed=
					lpBmi->bmiColors[i].rgbRed;
				lpLogPal->palPalEntry[i].peGreen=
					lpBmi->bmiColors[i].rgbGreen;
				lpLogPal->palPalEntry[i].peBlue=
					lpBmi->bmiColors[i].rgbBlue;
				lpLogPal->palPalEntry[i].peFlags=0;
			}
			else
			{
				lpLogPal->palPalEntry[i].peRed=
					lpBmc->bmciColors[i].rgbtRed;
				lpLogPal->palPalEntry[i].peGreen=
					lpBmc->bmciColors[i].rgbtGreen;
				lpLogPal->palPalEntry[i].peBlue=
					lpBmc->bmciColors[i].rgbtBlue;
				lpLogPal->palPalEntry[i].peFlags=
					0;
			}
		}
	

	
		hPal=::CreatePalette(lpLogPal);
		if(!hPal)
		{
			::GlobalUnlock(hLogPal);
			::GlobalFree(hLogPal);
			return NULL;
		}
		::GlobalUnlock(hLogPal);
		::GlobalFree(hLogPal);
		return hPal;
	}
	else
	{
		return NULL;
	}
}
HPALETTE CreateDIBPalette(HDIB hDIB)
{
		LPBYTE lpDIB=(LPBYTE)::GlobalLock(hDIB);
		return CreateDIBPalette(lpDIB);
}

WORD DIBNumColors(LPBYTE lpDIB)
{
	WORD wBitCount;
	if(IS_WIN30_DIB(lpDIB))
	{
//		::AfxMessageBox(_T("位于判断位图颜色函数中，位图类型为WIN型"));
		DWORD dwClrUse;
		dwClrUse=((LPBITMAPINFOHEADER)lpDIB)->biClrUsed;
		CString str1;
	   str1.Format(_T("位数为%d"),dwClrUse);
 //       ::AfxMessageBox(str1);
//		::AfxMessageBox(_T("位于判断位图颜色函数中，已获取位图信息"));
		if(dwClrUse)
		{
//			::AfxMessageBox(_T("位于判断位图颜色函数中，使用颜色数不为零"));
			return (WORD)dwClrUse;
		}
	}
	if(IS_WIN30_DIB(lpDIB))
		wBitCount=((LPBITMAPINFOHEADER)lpDIB)->biBitCount;
	else
		wBitCount=((LPBITMAPCOREHEADER)lpDIB)->bcBitCount;
//	CString str;
//	str.Format(_T("位数为%d"),wBitCount);
//	::AfxMessageBox(str);
	switch(wBitCount)
	{
	case 1:
		return 2;
	case 2:
		return 4;
	case 4:
		return 16;
	case 8:
		return 256;
//	case 24:
//		return 256;
	default:
		return 0;
	}
}
BOOL PaintDIB(HDC hDC,LPRECT lpDCRect,HANDLE hDIB,LPRECT lpDIBRect,
			  HPALETTE hPal,DWORD dwRop)
{
	LPBYTE lpDIBHdr;
	LPBYTE lpDIBBits;
	BOOL bSuccess=false;
	HPALETTE hOldPal=NULL;
  
	if(!hDIB)
	{
		::AfxMessageBox(_T("位于整刷位图函数中，位图句柄为空"));
		return false;
	}
	lpDIBHdr=(LPBYTE)::GlobalLock(hDIB);
	lpDIBBits=lpDIBHdr+sizeof(BITMAPINFOHEADER)+
		               DIBNumColors(lpDIBHdr)*sizeof(RGBQUAD);
	if(!hPal)
	{
		 
		hPal=CreateDIBPalette(lpDIBHdr);
//		::AfxMessageBox(_T("位于整刷位图函数中,调色板句柄无效"));
	}
	if(hPal)
	{
		hOldPal=::SelectPalette(hDC,hPal,true);
		::RealizePalette(hDC);
	}

	::SetStretchBltMode(hDC,COLORONCOLOR);

	bSuccess=::StretchDIBits(
		                     hDC,
		                     lpDCRect->left,
							 lpDCRect->top,
							 lpDCRect->right-lpDCRect->left,
							 lpDCRect->bottom-lpDCRect->top,
							 lpDIBRect->left,
							 ((LPBITMAPINFOHEADER)lpDIBHdr)->biHeight-
							 lpDIBRect->top-(lpDIBRect->bottom-lpDIBRect->top),
							 lpDIBRect->right-lpDIBRect->left,
							 lpDIBRect->bottom-lpDIBRect->top,
							 lpDIBBits,
							 (LPBITMAPINFO)lpDIBHdr,
							 DIB_RGB_COLORS,
							 SRCCOPY
							 );
	::GlobalUnlock(hDIB);
	if(hOldPal)
		::SelectPalette(hDC,hOldPal,false);
	::GlobalUnlock(hDIB);
	return bSuccess;
}
BOOL DisplayDIB(CDC* pDC,LPTSTR lpszBitmap)
{
	LPBITMAPINFO lpDIB;
	LPSTR lpBits;
	CPalette* pPal,*pOldPal;

	lpDIB=(LPBITMAPINFO)LoadDIBFromResource(lpszBitmap);
	if(lpDIB==NULL)
		return false;
	lpBits=(LPSTR)lpDIB+sizeof(BITMAPINFOHEADER)+
		                DIBNumColors((LPBYTE)lpDIB)*sizeof(RGBQUAD);
	pPal->FromHandle(CreateDIBPalette((LPBYTE)lpDIB));
	pOldPal=pDC->SelectPalette(pPal,true);
	pDC->RealizePalette();

	::SetDIBitsToDevice(
		                pDC->GetSafeHdc(),
						0,0,
						(int)lpDIB->bmiHeader.biWidth,
						(int)lpDIB->bmiHeader.biHeight,
						0,0,
						0,(UINT)lpDIB->bmiHeader.biHeight,
						lpBits,
						lpDIB,
						DIB_RGB_COLORS
						);
	pDC->SelectPalette(pOldPal,false);
	return true;
}
BOOL SaveDIB(HANDLE hDIB,LPCTSTR lpFileName)
{
	BITMAPFILEHEADER bmfHdr;
	LPBITMAPINFOHEADER lpBI;
	HANDLE fh;
	DWORD dwDIBSize;
	DWORD dwWritten;

	if(!hDIB)
		return false;
	
	fh=::CreateFile(
		            lpFileName,
					GENERIC_WRITE,
					0,
					NULL,
					CREATE_ALWAYS,
					FILE_ATTRIBUTE_NORMAL|
					FILE_FLAG_SEQUENTIAL_SCAN,
					NULL
					);
	if(fh==INVALID_HANDLE_VALUE)
		return false;

	lpBI=(LPBITMAPINFOHEADER)::GlobalLock(hDIB);
	if(!lpBI)
	{
		::CloseHandle(fh);
		return false;
	}
	if(lpBI->biSize!=sizeof(BITMAPINFOHEADER))
	{
		::GlobalUnlock(hDIB);
		::CloseHandle(fh);
		return false;
	}


	bmfHdr.bfType=0x4d42;
	dwDIBSize=
		*((LPDWORD)lpBI)+
		         DIBNumColors((LPBYTE)lpBI)*sizeof(RGBQUAD);

	if(
		(lpBI->biCompression==BI_RLE8)||
		 (lpBI->biCompression==BI_RLE4)
		 )
		 dwDIBSize+=lpBI->biSizeImage;
	else
	{
		DWORD dwBmBitsSize;
		dwBmBitsSize=WIDTHBYTES((lpBI->biWidth)*(DWORD)(lpBI->biBitCount)) *lpBI->biHeight;
		dwDIBSize+=dwBmBitsSize;
		lpBI->biSizeImage=dwBmBitsSize;
	}

	bmfHdr.bfSize=dwDIBSize+sizeof(BITMAPFILEHEADER);
	bmfHdr.bfReserved1=0;
	bmfHdr.bfReserved2=0;

	bmfHdr.bfOffBits=(DWORD)sizeof(BITMAPFILEHEADER)+
		                   lpBI->biSize+
						   DIBNumColors((LPBYTE)lpBI)*sizeof(RGBQUAD);
	::WriteFile(fh,(LPBYTE)&bmfHdr,
		        sizeof(BITMAPFILEHEADER),
				&dwWritten,
				NULL
				);
	::WriteFile(fh,(LPBYTE)lpBI,dwDIBSize,&dwWritten,NULL);

	::GlobalUnlock(hDIB);
	::CloseHandle(fh);
	if(dwWritten==0)
		return false;
	else
		return true;

}
HDIB CreateDIB(DWORD dwWidth,DWORD dwHeight,WORD wBitCount)
{
	BITMAPINFOHEADER Bi;
	LPBITMAPINFOHEADER lpBi;
	DWORD dwLen;
	HDIB hDIB;
	DWORD dwBytesPerLine;
	if(wBitCount!=-1)
	{
	if(wBitCount<=1)
		wBitCount=1;
	else if(wBitCount<=4)
		wBitCount=4;
	else if(wBitCount<=8)
		wBitCount=8;
	else if(wBitCount<=24)
		wBitCount=24;
	else
		wBitCount=4;
	Bi.biSize=sizeof(BITMAPINFOHEADER);
	Bi.biWidth=dwWidth;
	Bi.biHeight=dwHeight;
	Bi.biPlanes=1;
	Bi.biBitCount=wBitCount;
	Bi.biCompression=BI_RGB;
	Bi.biSizeImage=0;
	Bi.biXPelsPerMeter=0;
	Bi.biYPelsPerMeter=0;
	Bi.biClrUsed=0;
	Bi.biClrImportant=0;
	dwBytesPerLine=WIDTHBYTES(wBitCount*dwWidth);
	dwLen=Bi.biSize+DIBNumColors((LPBYTE)&Bi)*sizeof(RGBQUAD)+
		(dwBytesPerLine*dwHeight);
	hDIB=::GlobalAlloc(GHND,dwLen);
	if(!hDIB)
		return NULL;
	lpBi=(LPBITMAPINFOHEADER)::GlobalLock(hDIB);
	*lpBi=Bi;
	::GlobalUnlock(hDIB);
	return hDIB;
	}
	else
	{
		return NULL;
	}


}
HPALETTE CopyPalette(HPALETTE hPalScr)
{
	PLOGPALETTE pLogPal;
	int iNumEntries=0;
	HPALETTE hPal;
	HANDLE h;

	iNumEntries=::GetPaletteEntries(hPalScr,0,iNumEntries,NULL);
	if(iNumEntries==0)
		return (HPALETTE)NULL;
	h=::GlobalAlloc(GHND,sizeof(DWORD)+sizeof(PALETTEENTRY)*iNumEntries);
	if(!h)
		return (HPALETTE)NULL;
	pLogPal=(PLOGPALETTE)::GlobalLock(h);
	if(!h)
		return (HPALETTE)NULL;
	pLogPal->palVersion=0x300;
	::GetPaletteEntries(hPalScr,0,iNumEntries,pLogPal->palPalEntry);
	hPal=::CreatePalette(pLogPal);


	::GlobalUnlock(h);
	::GlobalFree(h);
	return hPal;
}
BOOL DisplayPalette(HDC hDC,LPRECT lpRect,HPALETTE hPal)
{
	if(!hPal)
		return false;
	int nEntries;
	PALETTEENTRY pe[256];
	nEntries=::GetPaletteEntries(hPal,0,256,pe);
	int nSqr=(int)sqrt((double)nEntries);
	int nWidth=(lpRect->right-lpRect->left)/nSqr;
	int nHeight=(lpRect->bottom-lpRect->top)/nSqr;
	lpRect->right=lpRect->left+nWidth*nSqr;
	lpRect->bottom=lpRect->top+nHeight*nSqr;

	HPALETTE hOldPal=::SelectPalette(hDC,hPal,false);
	::RealizePalette(hDC);
	HBRUSH hBrush,hOldBrush;
	int x,y;
	for(int i=0;i<nEntries;++i)
	{
		x=i%nSqr;
		y=i/nSqr;
		hBrush=::CreateSolidBrush(RGB(pe[i].peRed,pe[i].peGreen,pe[i].peBlue));
		hOldBrush=(HBRUSH)SelectObject(hDC,hBrush);
		::Rectangle(hDC,lpRect->left+x*nWidth,
			lpRect->top+y*nHeight,
			lpRect->left+(x+1)*nWidth,
			lpRect->top+(y+1)*nHeight);
		::SelectObject(hDC,hOldBrush);
		::DeleteObject(hBrush);
	}
	::SelectPalette(hDC,hOldPal,false);
	return true;
}
int PalEnOnDevice(HDC hDC)
{
	int nColors;
	nColors=(1<<::GetDeviceCaps(hDC,BITSPIXEL)*::GetDeviceCaps(hDC,PLANES));
//	assert(nColors);
	return nColors;
};
HPALETTE GetSystemPalette()
{
	HDC hDC;
	static HPALETTE hPal=NULL;
	HANDLE hLogPal;
	LPLOGPALETTE lpLogPal;
	int nColors;
	hDC=GetDC(NULL);
	if(!hDC)
		return (HPALETTE)NULL;
	nColors=PalEnOnDevice(hDC);
	hLogPal=::GlobalAlloc(GHND,sizeof(DWORD)+sizeof(PALETTEENTRY)*nColors);
	if(!hLogPal)
		return (HPALETTE)NULL;
	lpLogPal=(LPLOGPALETTE)GlobalLock(hLogPal);
	lpLogPal->palVersion=0x300;
	lpLogPal->palNumEntries=nColors;

	::GetSystemPaletteEntries(hDC,0,nColors,lpLogPal->palPalEntry);
	hPal=::CreatePalette(lpLogPal);

	::GlobalUnlock(hLogPal);
	::GlobalFree(hLogPal);
	::ReleaseDC(NULL,hDC);
	return hPal;
}
HDIB CreateDefaultDIB(DWORD dwWidth,DWORD dwHeight)
{
	HDC hDC=GetDC(NULL);
	if(!hDC)
		return NULL;
	int nDeviceBitsPixel=GetDeviceCaps(hDC,BITSPIXEL);
	HDIB hDIB=::GlobalAlloc(GHND,nDeviceBitsPixel);
	LPBITMAPINFO lpBmi=(LPBITMAPINFO)::GlobalLock(hDIB);
	LPBYTE lpDIBBits=(LPBYTE)lpBmi+sizeof(BITMAPINFOHEADER)+
		                DIBNumColors((LPBYTE)lpBmi)*sizeof(RGBQUAD);
	DWORD dwBytesPerLine=WIDTHBYTES(nDeviceBitsPixel*lpBmi->bmiHeader.biWidth);
	DWORD dwBitsSize=lpBmi->bmiHeader.biHeight*dwBytesPerLine;
	for(DWORD l=0;l<dwBitsSize;l++)
		lpDIBBits[l]=0xff;
	if(nDeviceBitsPixel>8)
	{
		::GlobalUnlock(hDIB);
		::ReleaseDC(NULL,hDC);
		return hDIB;
	}
	int nColors=::PalEnOnDevice(hDC);
	PALETTEENTRY pe[256];
	::GetSystemPaletteEntries(hDC,0,nColors,pe);
	for(int i=0;i<nColors;i++)
	{
		lpBmi->bmiColors[i].rgbRed=pe[i].peRed;
		lpBmi->bmiColors[i].rgbGreen=pe[i].peGreen;
		lpBmi->bmiColors[i].rgbBlue=pe[i].peBlue;
		lpBmi->bmiColors[i].rgbReserved=0;
	}
	::GlobalUnlock(hDIB);
	::ReleaseDC(NULL,hDC);
	return hDIB;
}
void DestroyDIB(HDIB hDIB)
{
	::GlobalFree(hDIB);
}
WORD DIBNumColors(HDIB hDIB)
{
	LPBYTE lpDIB=(LPBYTE)::GlobalLock(hDIB);
	return::DIBNumColors(lpDIB);
}
DWORD BytesPerLine(LPBYTE lpDIB)
{
	CString str;
//		str.Format(_T("查看值1为%d"),
//		               ((LPBITMAPINFOHEADER)lpDIB)->biWidth);
//		::AfxMessageBox(str);
	//return WIDTHBYTES(((LPBITMAPINFOHEADER)lpDIB)->biWidth*
	//	               ((LPBITMAPINFOHEADER)lpDIB)->biPlanes*
	//				   ((LPBITMAPINFOHEADER)lpDIB)->biBitCount);

		return WIDTHBYTES(((LPBITMAPINFOHEADER)lpDIB)->biWidth*
		               
					   ((LPBITMAPINFOHEADER)lpDIB)->biBitCount);
		
}

DWORD BytesPerLine(HDIB hDIB)
{
	LPBYTE lpDIB=(LPBYTE)::GlobalLock(hDIB);
	DWORD result=::BytesPerLine(lpDIB);
	::GlobalUnlock( hDIB );
	return result;
}
DWORD DIBHeight(LPBYTE lpDIB)
{
	return ((LPBITMAPINFOHEADER)lpDIB)->biHeight;
}
DWORD DIBHeight(HDIB hDIB)
{
	if(!hDIB)
		return 0;
	LPBYTE lpDIB=(LPBYTE)::GlobalLock(hDIB);
	DWORD result = DIBHeight(lpDIB);
	::GlobalUnlock( hDIB );
	return result;

}
DWORD DIBWidth(LPBYTE lpDIB)
{
	return ((LPBITMAPINFOHEADER)lpDIB)->biWidth;
}
DWORD DIBWidth(HDIB hDIB)
{
	if(!hDIB)
		return 0;
	LPBYTE lpDIB=(LPBYTE)::GlobalLock(hDIB);
	DWORD result = DIBWidth(lpDIB);
	::GlobalUnlock( hDIB );
	return result;

}
WORD DIBBitCount(LPBYTE lpDIB)
{
	return ((LPBITMAPINFOHEADER)lpDIB)->biBitCount;
}
WORD DIBBitCount(HDIB hDIB)
{
	if(!hDIB)
		return 0;
	LPBYTE lpDIB=(LPBYTE)::GlobalLock(hDIB);
	WORD result = DIBBitCount(lpDIB);
	::GlobalUnlock( hDIB );
	return result;
}
LPBYTE FindDIBBits(LPBYTE lpDIB)
{
	return lpDIB+((LPBITMAPINFOHEADER)lpDIB)->biSize+
		           DIBNumColors(lpDIB)*sizeof(RGBQUAD);
}
LPBYTE FindDIBBits(HDIB hDIB)
{
	LPBYTE lpDIB=(LPBYTE)::GlobalLock(hDIB);
	return ::FindDIBBits(lpDIB);
}
DWORD DIBBlockSize(LPBYTE lpDIB)
{
	if(((LPBITMAPINFOHEADER)lpDIB)->biSizeImage==0)
	{
	//	::AfxMessageBox(_T("位图尺寸项标为零"));
		((LPBITMAPINFOHEADER)lpDIB)->biSizeImage=
			::BytesPerLine(lpDIB)*(((LPBITMAPINFOHEADER)lpDIB)->biHeight);
	//	CString str;
	//	str.Format(_T("查看值为%d"),BytesPerLine(lpDIB));
	//	::AfxMessageBox(str);
	}
	
	return((LPBITMAPINFOHEADER)lpDIB)->biSize+::PaletteSize(lpDIB)+
		((LPBITMAPINFOHEADER)lpDIB)->biSizeImage;


}
DWORD DIBBlockSize(HDIB hDIB)
{
	LPBYTE lpDIB=(LPBYTE)::GlobalLock(hDIB);
	DWORD result = DIBBlockSize(lpDIB);
	::GlobalUnlock( hDIB );
	return result;
}
WORD PaletteSize(LPBYTE lpDIB)
{
	return ::DIBNumColors(lpDIB)*sizeof(RGBQUAD);
}


WORD PaletteSize(HDIB hDIB)
{
		LPBYTE lpDIB=(LPBYTE)::GlobalLock(hDIB);
		WORD result = PaletteSize(lpDIB);
		::GlobalUnlock( hDIB );
	return result;

}
BOOL PaintBitmap(HDC hDC,LPRECT lpDCRect,HBITMAP hDDB,
                LPRECT lpDDBRect,HPALETTE hPal,DWORD dwRop)
{
	HDC hMemDC;
	HBITMAP hOldBitmap;
	HPALETTE hOldPal1=NULL;
	HPALETTE hOldPal2=NULL;
	BOOL bSuccess=false;

	hMemDC=::CreateCompatibleDC(hDC);
	if(!hMemDC)
		return false;
	if(hPal)
	{
		hOldPal1=::SelectPalette(hMemDC,hPal,true);
		hOldPal2=::SelectPalette(hDC,hPal,true);
		::RealizePalette(hDC);
	}
	hOldBitmap=(HBITMAP)::SelectObject(hMemDC,hDDB);
	::SetStretchBltMode(hDC,COLORONCOLOR);
	if(((lpDCRect->right-lpDCRect->left)==(lpDDBRect->right-lpDDBRect->left))&&
		((lpDCRect->bottom-lpDCRect->top)==(lpDDBRect->bottom-lpDDBRect->top)))
		bSuccess=::BitBlt(hDC,lpDCRect->left,lpDCRect->top,
		                  lpDCRect->right-lpDCRect->left,
                          lpDCRect->bottom-lpDCRect->top,
						  hMemDC,lpDDBRect->left,lpDDBRect->top,dwRop);
	::SelectObject(hMemDC,hOldBitmap);
	if(hOldPal1)
		::SelectPalette(hMemDC,hOldPal1,false);
	if(hOldPal2)
		::SelectPalette(hDC,hOldPal1,false);
	::DeleteDC(hMemDC);
	return bSuccess;
}
HBITMAP DIBToDIBSection(LPBYTE lpDIB)
{  

	LPBYTE lpSrcBits;
	HDC hDC=NULL,hSrcDC;
	HBITMAP hSrcBitmap,hOldSrcBitmap;
	DWORD dwSrcBitsSize;

	LPBITMAPINFO lpSrcDIB=(LPBITMAPINFO)lpDIB;
	if(!lpSrcDIB)
		return NULL;
	hDC=GetDC(NULL);
	hSrcBitmap=::CreateDIBSection(hDC,lpSrcDIB,DIB_RGB_COLORS,(void **)&lpSrcBits,NULL,0);
	hSrcDC=::CreateCompatibleDC(hDC);
	dwSrcBitsSize=lpSrcDIB->bmiHeader.biHeight*
		::BytesPerLine((LPBYTE)&(lpSrcDIB->bmiHeader));
	::memcpy(lpSrcBits,::FindDIBBits((LPBYTE)&(lpSrcDIB->bmiHeader)),dwSrcBitsSize);
	hOldSrcBitmap=(HBITMAP)::SelectObject(hSrcDC,hSrcBitmap);
	if(lpSrcDIB->bmiHeader.biBitCount<=8)
	{
		::SetDIBColorTable(hSrcDC,0,1<<lpSrcDIB->bmiHeader.biBitCount,lpSrcDIB->bmiColors);
	}
	::SelectObject(hSrcDC,hOldSrcBitmap);
	::DeleteDC(hSrcDC);
	::ReleaseDC(NULL,hDC);
	::GdiFlush();
	return hSrcBitmap;

}
HBITMAP DIBToDIBSection(HDIB hDIB)
{
		LPBYTE lpDIB=(LPBYTE)::GlobalLock(hDIB);
		HBITMAP result = DIBToDIBSection(lpDIB);
		::GlobalUnlock( hDIB );
	   return result;
}
HDIB DIBSectionToDIB(HBITMAP hBitmap)
{
	HDC hDC=NULL,hSrcDC;
	HBITMAP hOldSrcBitmap;
	HANDLE hNewDIB;
	LPBITMAPINFO lpBmi=NULL;
	DWORD dwSize;
	DIBSECTION ds;
	DWORD dwColorNum;

	::GetObject(hBitmap,sizeof(DIBSECTION),&ds);
	dwColorNum=ds.dsBmih.biClrUsed;
	if((dwColorNum==0)&&ds.dsBmih.biBitCount<=8)
		dwColorNum=1<<ds.dsBmih.biBitCount;
	dwSize=sizeof(BITMAPINFOHEADER)+dwColorNum*(sizeof(RGBQUAD))+ds.dsBmih.biSizeImage;


	hNewDIB=::GlobalAlloc(GHND,dwSize);
	if(!hNewDIB)
		return NULL;

	lpBmi=(LPBITMAPINFO)::GlobalLock(hNewDIB);
	if(!lpBmi)
		return NULL;

	lpBmi->bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	lpBmi->bmiHeader.biWidth=ds.dsBmih.biWidth;
	lpBmi->bmiHeader.biHeight=ds.dsBmih.biHeight;
	lpBmi->bmiHeader.biPlanes=1;
	lpBmi->bmiHeader.biBitCount=ds.dsBmih.biBitCount;
	lpBmi->bmiHeader.biCompression=ds.dsBmih.biCompression;
	lpBmi->bmiHeader.biSizeImage=ds.dsBmih.biSizeImage;
	lpBmi->bmiHeader.biXPelsPerMeter=ds.dsBmih.biXPelsPerMeter;
	lpBmi->bmiHeader.biYPelsPerMeter=ds.dsBmih.biYPelsPerMeter;
	lpBmi->bmiHeader.biClrUsed=ds.dsBmih.biClrUsed;
	lpBmi->bmiHeader.biClrImportant=ds.dsBmih.biClrImportant;


	hDC=GetDC(NULL);
	if(!::GetDIBits(hDC,
		            hBitmap,
					0,
					ds.dsBmih.biHeight,
					(LPBYTE)lpBmi+(WORD)lpBmi->bmiHeader.biSize+(dwColorNum*sizeof(RGBQUAD)),
					lpBmi,
					(DWORD)DIB_RGB_COLORS
					)
		)
	{
		::GlobalUnlock(hNewDIB);
		::ReleaseDC(NULL,hDC);
		return NULL;
	}


	hSrcDC=::CreateCompatibleDC(hDC);
	hOldSrcBitmap=(HBITMAP)SelectObject(hDC,hBitmap);

	if(lpBmi->bmiHeader.biBitCount<=8)
		::GetDIBColorTable(hDC,0,1<<lpBmi->bmiHeader.biBitCount,lpBmi->bmiColors);


	::SelectObject(hDC,hOldSrcBitmap);
	::DeleteDC(hSrcDC);
	::ReleaseDC(NULL,hDC);
	::GlobalUnlock(hNewDIB);
	return hNewDIB;
}
BOOL CopyColorTable(LPBITMAPINFO lpTgt,LPBITMAPINFO lpSrc,HPALETTE hPalSrc)
{
	switch(lpTgt->bmiHeader.biBitCount)
	{
	case 8:
		if(hPalSrc)
		{
			PALETTEENTRY pe[256];
			UINT i;

			::GetPaletteEntries(hPalSrc,0,256,pe);

			for(i=0;i<256;i++)
			{
				lpTgt->bmiColors[i].rgbRed=pe[i].peRed;
				lpTgt->bmiColors[i].rgbGreen=pe[i].peGreen;
				lpTgt->bmiColors[i].rgbBlue=pe[i].peBlue;
				lpTgt->bmiColors[i].rgbReserved=0;
			}
		}
		else
		{
			if(lpSrc->bmiHeader.biBitCount==8)
			{
				::memcpy(lpTgt->bmiColors,lpSrc->bmiColors,256*sizeof(RGBQUAD));
			}
			/*
		
			else
			{
				HPALETTE hPal;
				HDC hDC=GetDC(NULL);
				HPALETTE pe[256];
				UINT i;

				hPal=CreateOctreePalette((LPBYTE)lpSrc,236,8);
				if(!hPal)
					hPal=::CreateHalftonePalette(hDC);

				::ReleaseDC(NULL,hDC);


				::GetPaletteEntries(hPal,0,256,pe);
				::DeleteObject(hPal);

				
			for(i=0;i<256;i++)
			{
				lpTgt->bmiColors[i].rgbRed=pe[i].peRed;
				lpTgt->bmiColors[i].rgbGreen=pe[i].peGreen;
				lpTgt->bmiColors[i].rgbBlue=pe[i].peBlue;
				lpTgt->bmiColors[i].rgbReserved=0;
			}
			}
			*/
		}
		break;//end of 8pp
	case 4:
		if(hPalSrc)
		{
			PALETTEENTRY pe[16];
			UINT i;

			::GetPaletteEntries(hPalSrc,0,16,pe);

			for(i=0;i<16;i++)
			{
				lpTgt->bmiColors[i].rgbRed=pe[i].peRed;
				lpTgt->bmiColors[i].rgbGreen=pe[i].peGreen;
				lpTgt->bmiColors[i].rgbBlue=pe[i].peBlue;
				lpTgt->bmiColors[i].rgbReserved=0;
			}
		}
		else
		{
			if(lpSrc->bmiHeader.biBitCount==8)
			{
				::memcpy(lpTgt->bmiColors,lpSrc->bmiColors,256*sizeof(RGBQUAD));
			}
			else
			{
				
				HPALETTE hPal;
				PALETTEENTRY pe[256];
				UINT i;

				hPal=(HPALETTE)::GetStockObject(DEFAULT_PALETTE);
				GetPaletteEntries(hPalSrc,0,16,pe);
				for(i=0;i<16;i++)
				{
					lpTgt->bmiColors[i].rgbRed=pe[i].peRed;
				    lpTgt->bmiColors[i].rgbGreen=pe[i].peGreen;
				    lpTgt->bmiColors[i].rgbBlue=pe[i].peBlue;
				    lpTgt->bmiColors[i].rgbReserved=pe[i].peFlags;
				}
			}
		}
			break;//end of 4bpp;
	case 1:
		lpTgt->bmiColors[0].rgbRed=0;
		lpTgt->bmiColors[0].rgbGreen=0;
		lpTgt->bmiColors[0].rgbBlue=0;
		lpTgt->bmiColors[0].rgbReserved=0;
        lpTgt->bmiColors[1].rgbRed=255;
		lpTgt->bmiColors[1].rgbGreen=255;
		lpTgt->bmiColors[1].rgbBlue=255;
		lpTgt->bmiColors[1].rgbReserved=0;
		break;


	case 16:
	case 24:
	case 32:
	default:
		break;
	}
		return true;
}


HDIB ConvertDIBFormat(LPBYTE lpSrcDIB,UINT nWidth,UINT nHeight,
                      UINT nbpp,BOOL bStretch,HPALETTE hPalSrc)
{
	 LPBITMAPINFO lpSrcDIBmi=(LPBITMAPINFO)lpSrcDIB;
     LPBITMAPINFO lpBmi=NULL;
	 LPBYTE lpSrcBits,lpTgtBits,lpResult;
	 HDC hDC=NULL,hSrcDC,hTgtDC;
	 HBITMAP hSrcBitmap,hTgtBitmap,hOldSrcBitmap,hOldTgtBitmap;
	 DWORD dwSrcBitsSize,dwTgtBitsSize,dwTgtHeaderSize,dwColorNum;
	 HDIB hDIB;


	 if(nbpp<=8)
		 dwColorNum=1<<lpSrcDIBmi->bmiHeader .biBitCount;
	 else
		 dwColorNum=0;

	 dwTgtHeaderSize=sizeof(BITMAPINFO)+dwColorNum*sizeof(RGBQUAD);
	 lpBmi=(LPBITMAPINFO)::malloc(dwTgtHeaderSize);



	 lpBmi->bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	 lpBmi->bmiHeader.biWidth=nWidth;
	 lpBmi->bmiHeader.biHeight=nHeight;
	 lpBmi->bmiHeader.biPlanes=1;
	 lpBmi->bmiHeader.biBitCount=nbpp;
	 lpBmi->bmiHeader.biCompression=BI_RGB;
	 lpBmi->bmiHeader.biSizeImage=0;
	 lpBmi->bmiHeader.biXPelsPerMeter=0;
	 lpBmi->bmiHeader.biYPelsPerMeter=0;
	 lpBmi->bmiHeader.biClrUsed=0;
	 lpBmi->bmiHeader.biClrImportant=0;


	 if(!::CopyColorTable(lpBmi,lpSrcDIBmi,hPalSrc))
	 {
		 ::free(lpBmi);
		 return NULL;
	 }


	 hDC=::GetDC(NULL);
	 hTgtBitmap=::CreateDIBSection(hDC,lpBmi,DIB_RGB_COLORS,(void**)&lpTgtBits,NULL,0);
	 hSrcBitmap=::CreateDIBSection(hDC,lpSrcDIBmi,DIB_RGB_COLORS,(void**)&lpSrcBits,NULL,0);


	 hSrcDC=::CreateCompatibleDC(hDC);
	 hTgtDC=::CreateCompatibleDC(hDC);


	 dwSrcBitsSize=lpSrcDIBmi->bmiHeader.biHeight*::BytesPerLine((LPBYTE)&(lpSrcDIBmi->bmiHeader));
	 dwTgtBitsSize=lpBmi->bmiHeader.biHeight*::BytesPerLine((LPBYTE)&(lpBmi->bmiHeader));

	 ::memcpy(lpSrcBits,::FindDIBBits((LPBYTE)lpSrcDIBmi),dwSrcBitsSize);
	 lpBmi->bmiHeader.biSizeImage=dwTgtBitsSize;

	 hOldSrcBitmap=(HBITMAP)::SelectObject(hSrcDC,hSrcBitmap);
	 hOldTgtBitmap=(HBITMAP)::SelectObject(hTgtDC,hTgtBitmap);

	 if(lpSrcDIBmi->bmiHeader.biBitCount<=8)
		 ::SetDIBColorTable(hSrcDC,0,1<<lpSrcDIBmi->bmiHeader.biBitCount,lpSrcDIBmi->bmiColors);
	 
	 if(lpBmi->bmiHeader.biBitCount<=8)
		 ::SetDIBColorTable(hTgtDC,0,1<<lpBmi->bmiHeader.biBitCount,lpBmi->bmiColors);



	 if((lpSrcDIBmi->bmiHeader.biWidth==lpBmi->bmiHeader.biWidth)&&
		 (lpSrcDIBmi->bmiHeader.biHeight==lpBmi->bmiHeader.biHeight))
	 {
		 ::BitBlt(hTgtDC,0,0,
			      lpBmi->bmiHeader.biWidth,lpBmi->bmiHeader.biHeight,
			      hSrcDC,0,0,SRCCOPY);
	 }
	 else
	 {
		 if(bStretch)
		 {
			 ::SetStretchBltMode(hTgtDC,COLORONCOLOR);
			 ::StretchBlt(hTgtDC,0,0,
				         lpBmi->bmiHeader.biWidth,lpBmi->bmiHeader.biHeight,
						 hSrcDC,0,0,
						 lpSrcDIBmi->bmiHeader.biWidth, lpSrcDIBmi->bmiHeader.biHeight,
						 SRCCOPY);
		 }
		 else
		 {
			 ::BitBlt(hTgtDC,0,0,
			      lpBmi->bmiHeader.biWidth,lpBmi->bmiHeader.biHeight,
			      hSrcDC,0,0,SRCCOPY);
		 }
	 }

	 ::SelectObject(hSrcDC,hOldSrcBitmap);
	 ::SelectObject(hTgtDC,hOldTgtBitmap);

	 ::DeleteDC(hSrcDC);
	 ::DeleteDC(hTgtDC);
	 ::ReleaseDC(NULL,hDC);

	 ::GdiFlush();


	 hDIB=::GlobalAlloc(GHND,dwTgtHeaderSize+dwTgtBitsSize);
	 lpResult=(LPBYTE)::GlobalLock(hDIB);

	 ::memcpy(lpResult,lpBmi,dwTgtHeaderSize);
	 ::memcpy(::FindDIBBits((LPBYTE)lpResult),lpTgtBits,dwTgtBitsSize);


	 ::DeleteObject(hSrcBitmap);
	 ::DeleteObject(hTgtBitmap);

	 free(lpBmi);
	 ::GlobalUnlock(hDIB);
	 return hDIB;
}
HDIB ConvertDIBFormat(HDIB hDIB,UINT nWidth,UINT nHeight,
                      UINT nbpp,BOOL bStretch,HPALETTE hPalSrc)
{
	LPBYTE lpDIB=(LPBYTE)::GlobalLock(hDIB);
	HDIB result = ConvertDIBFormat(lpDIB,nWidth,nHeight,
                      nbpp,bStretch,hPalSrc);
	::GlobalUnlock( hDIB );
	return result;
}
HDIB ConvertDIBFormat(LPBYTE lpSrcDIB,UINT nbpp,HPALETTE hPalSrc)
{
	return ::ConvertDIBFormat(lpSrcDIB,
		         ((LPBITMAPINFO)lpSrcDIB)->bmiHeader.biWidth,((LPBITMAPINFO)lpSrcDIB)->bmiHeader.biHeight,
				 nbpp,0,hPalSrc);
}
HDIB ConvertDIBFormat(HDIB hDIB,UINT nbpp,HPALETTE hPalSrc)
{
	LPBYTE lpDIB=(LPBYTE)::GlobalLock(hDIB);
	HDIB result = ConvertDIBFormat(lpDIB,nbpp,hPalSrc);
	::GlobalUnlock( hDIB );
	return result;
}

HDIB BitmapToDIB(HBITMAP hDDB,HPALETTE hPal)
{
	BITMAP Bm;
	BITMAPINFOHEADER Bi;
	LPBITMAPINFOHEADER lpBi;
	DWORD dwLen;
	HANDLE hDIB,h;
	HDC hDC;
	WORD biBits;


	if(!hDDB)
		return NULL;

	if(!::GetObject(hDDB,sizeof(Bm),(LPBYTE)&Bm))
		return NULL;

	if(hPal==NULL)
		hPal=(HPALETTE)::GetStockObject(DEFAULT_PALETTE);


	biBits=Bm.bmPlanes*Bm.bmBitsPixel;

	if(biBits<=1)
		biBits=1;
	else if(biBits<=4)
		biBits=4;
	else if(biBits<=8)
		biBits=8;
	else 
		biBits=24;

	Bi.biSize=sizeof(BITMAPINFOHEADER);
	Bi.biWidth=Bm.bmWidth;
	Bi.biHeight=Bm.bmHeight;
	Bi.biPlanes=1;
	Bi.biBitCount=biBits;
	Bi.biCompression=BI_RGB;
	Bi.biSizeImage=0;
	Bi.biXPelsPerMeter=0;
	Bi.biYPelsPerMeter=0;
	Bi.biClrUsed=0;
	Bi.biClrImportant=0;

	dwLen=Bi.biSize+::PaletteSize((LPBYTE)&Bi);


	hDC=GetDC(NULL);

	hPal=::SelectPalette(hDC,hPal,false);
	::RealizePalette(hDC);

	hDIB=::GlobalAlloc(GHND,dwLen);
	if(!hDIB)
	{
		::SelectPalette(hDC,hPal,true);
		::RealizePalette(hDC);
		::ReleaseDC(NULL,hDC);
		return NULL;
	}

	lpBi=(LPBITMAPINFOHEADER)::GlobalLock(hDIB);
	*lpBi=Bi;

	::GetDIBits(hDC,hDDB,0,Bi.biHeight,NULL,(LPBITMAPINFO)lpBi,DIB_RGB_COLORS);


	Bi=*lpBi;
	::GlobalUnlock(hDIB);

	if(Bi.biSizeImage==0)
		Bi.biSizeImage=WIDTHBYTES((DWORD)Bi.biWidth*Bi.biBitCount)*Bm.bmHeight;


	dwLen=Bi.biSize+::PaletteSize((LPBYTE)&Bi)+Bi.biSizeImage;
	
	if(h=::GlobalReAlloc(hDIB,dwLen,0))
		hDIB=h;

	else
	{
		::GlobalFree(hDIB);
		hDIB=NULL;
		::SelectPalette(hDC,hPal,true);
		::RealizePalette(hDC);
		::ReleaseDC(NULL,hDC);
		return NULL;
	}


	lpBi=(LPBITMAPINFOHEADER)::GlobalLock(hDIB);


	if(::GetDIBits(hDC,hDDB,
		           0,Bi.biHeight,
				   (LPBYTE)lpBi+(WORD)lpBi->biSize+::PaletteSize((LPBYTE)lpBi),
				   (LPBITMAPINFO)lpBi,DIB_RGB_COLORS)==0)
	{
		::GlobalUnlock(hDIB);
		hDIB=NULL;
		::SelectPalette(hDC,hPal,true);
		::RealizePalette(hDC);
		::ReleaseDC(NULL,hDC);
		return NULL;
	}


	Bi=*lpBi;

	::GlobalUnlock(hDIB);
	::SelectPalette(hDC,hPal,true);
	::RealizePalette(hDC);
	::ReleaseDC(NULL,hDC);

	return hDIB;
	}
HDIB BitmapToDIB(HBITMAP hDDB,HPALETTE hPal,WORD wBitCount)
{
	HDIB hNewDIB;
	if(!hDDB)
		return NULL;
	BITMAP Bm;
	::GetObject(hDDB,sizeof(Bm),&Bm);
	int biBits=Bm.bmBitsPixel*Bm.bmPlanes;

	if(biBits<=1)
		biBits=1;
	else if(biBits<=4)
		biBits=4;
	else if(biBits<=8)
		biBits=8;
	else 
		biBits=24;

	HDIB hDIB=::BitmapToDIB(hDDB,hPal);
	if(!hDIB)
		return NULL;

	if(wBitCount==biBits)
		hNewDIB=hDIB;
	else
	{
		hNewDIB=::ConvertDIBFormat(hDIB,wBitCount,hPal);

		::GlobalFree(hDIB);
	}
	
	return hNewDIB;
}
HBITMAP DIBToBitmap(HDIB hDIB,HPALETTE hPal)
{
	LPBYTE lpDIBHdr,lpDIBBits;
	HBITMAP hBitmap;
	HDC hDC;
	HPALETTE hOldPal=NULL;


	if(!hDIB)
		return NULL;


	lpDIBHdr=(LPBYTE)::GlobalLock(hDIB);
	lpDIBBits=::FindDIBBits(lpDIBHdr);

	hDC=GetDC(NULL);

	if(!hDC)
	{
		::GlobalUnlock(hDIB);
		return NULL;
	}


	if(hPal)
	{
		hOldPal=(HPALETTE)::SelectPalette(hDC,hPal,false);
		::RealizePalette(hDC);
	}


	hBitmap=::CreateDIBitmap(hDC,
		             (LPBITMAPINFOHEADER)lpDIBHdr,
					 CBM_INIT,
					 lpDIBBits,
					 (LPBITMAPINFO)lpDIBHdr,
					 DIB_RGB_COLORS);

	if(hOldPal)
		::SelectPalette(hDC,hOldPal,false);

	::ReleaseDC(NULL,hDC);
	::GlobalUnlock(hDIB);


	return hBitmap;
}
HDIB ChangeBitmapFormat(HBITMAP hDDB,WORD biBits,
                         DWORD biCompression,HPALETTE hPal)
{
	BITMAP Bm;
	BITMAPINFOHEADER Bi;
	LPBITMAPINFOHEADER lpBi;
	DWORD dwLen;
	HANDLE hDIB,h;
	HDC hDC;

	if(!hDDB)
		return NULL;
	
	if(hPal==NULL)
		hPal=(HPALETTE)::GetStockObject(DEFAULT_PALETTE);

	::GetObject(hDDB,sizeof(Bm),(LPBYTE)&Bm);

	if(biBits==0)
	{
		biBits=Bm.bmPlanes*Bm.bmBitsPixel;

		if(biBits<=1)
		biBits=1;
	else if(biBits<=4)
		biBits=4;
	else if(biBits<=8)
		biBits=8;
	else 
		biBits=24;
	}

	Bi.biSize=sizeof(BITMAPINFOHEADER);
	Bi.biWidth=Bm.bmWidth;
	Bi.biHeight=Bm.bmHeight;
	Bi.biPlanes=1;
	Bi.biBitCount=biBits;
	Bi.biCompression=biCompression;
	Bi.biSizeImage=0;
	Bi.biXPelsPerMeter=0;
	Bi.biYPelsPerMeter=0;
	Bi.biClrUsed=0;
	Bi.biClrImportant=0;


	dwLen=Bi.biSize+::PaletteSize((LPBYTE)&Bi);


	hDC=GetDC(NULL);

	HPALETTE hPalT=::SelectPalette(hDC,hPal,false);
	::RealizePalette(hDC);

	hDIB=::GlobalAlloc(GHND,dwLen);
	if(!hDIB)
	{
		::SelectPalette(hDC,hPalT,true);
		::RealizePalette(hDC);
		::ReleaseDC(NULL,hDC);
		return NULL;
	}

	lpBi=(LPBITMAPINFOHEADER)::GlobalLock(hDIB);
	*lpBi=Bi;

	::GetDIBits(hDC,hDDB,0,Bi.biHeight,NULL,(LPBITMAPINFO)lpBi,DIB_RGB_COLORS);


	Bi=*lpBi;
	::GlobalUnlock(hDIB);

	if(Bi.biSizeImage==0)
    {
    
		Bi.biSizeImage=WIDTHBYTES((DWORD)Bi.biWidth*Bi.biBitCount)*Bm.bmHeight;
                if(biCompression!=BI_RGB)
                    Bi.biSizeImage=(Bi.biSizeImage*3)/2;

    }
	dwLen=Bi.biSize+::PaletteSize((LPBYTE)&Bi)+Bi.biSizeImage;
	
	if(h=::GlobalReAlloc(hDIB,dwLen,0))
		hDIB=h;

	else
	{
		::GlobalFree(hDIB);
		hDIB=NULL;
		::SelectPalette(hDC,hPalT,true);
		::RealizePalette(hDC);
		::ReleaseDC(NULL,hDC);
		return NULL;
	}


	lpBi=(LPBITMAPINFOHEADER)::GlobalLock(hDIB);


	if(::GetDIBits(hDC,hDDB,
		           0,Bi.biHeight,
				   (LPBYTE)lpBi+(WORD)lpBi->biSize+::PaletteSize((LPBYTE)lpBi),
				   (LPBITMAPINFO)lpBi,DIB_RGB_COLORS)==0)
	{
		::GlobalUnlock(hDIB);
		hDIB=NULL;
		::SelectPalette(hDC,hPal,true);
//		::RealizePalette(hDC);
		::ReleaseDC(NULL,hDC);
		return NULL;
	}


	Bi=*lpBi;

	::GlobalUnlock(hDIB);
	::SelectPalette(hDC,hPal,true);
//	::RealizePalette(hDC);
	::ReleaseDC(NULL,hDC);

	return hDIB;
}
HDIB ChangeDIBFormat(HDIB hDIB,WORD wBitCount,DWORD dwCompression)
{
	
	HBITMAP hBitmap;
	HDIB hNewDIB=NULL;
	HPALETTE hPal;
	

	if(!hDIB)
		return NULL;
	hPal=::CreateDIBPalette(hDIB);

	if(hPal==0)
		hPal=(HPALETTE)::GetStockObject(DEFAULT_PALETTE);

	hBitmap=::DIBToBitmap(hDIB,hPal);
	if(!hBitmap)
	{
		::DeleteObject(hPal);
		return NULL;
	}

	hNewDIB=::ChangeBitmapFormat(hBitmap,wBitCount,dwCompression,hPal);

	::DeleteObject(hBitmap);
	::DeleteObject(hPal);



	return hNewDIB;



}
BOOL MapDIBColorsToPalette(HDIB hDIB,HPALETTE hPal)
{
	if(!hDIB)
		return false;
	if(!hPal)
		return false;

	LPBITMAPINFOHEADER lpBi=(LPBITMAPINFOHEADER)::GlobalLock(hDIB);

	if(!lpBi)
		return false;

	LPRGBQUAD pctThis=(LPRGBQUAD)((LPBYTE)lpBi+lpBi->biSize);

	BYTE imap[256];
	for(int i=0;i<256;i++)
	{
		imap[i]=(BYTE)::GetNearestPaletteIndex(hPal,
			                                   RGB(
											   pctThis->rgbRed,
											   pctThis->rgbGreen,
											   pctThis->rgbBlue)
											   );
		pctThis++;
	}
	LPBYTE lpBits=(LPBYTE)lpBi+lpBi->biSize+::PaletteSize((LPBYTE)lpBi);
	int iSize=WIDTHBYTES(lpBi->biBitCount*lpBi->biWidth)*lpBi->biHeight;

	while(iSize--)
	{
		*lpBits=imap[*lpBits];
		lpBits++;
	}

	PALETTEENTRY pe[256];
	::GetPaletteEntries(hPal,0,256,pe);
	pctThis=(LPRGBQUAD)((LPBYTE)lpBi+lpBi->biSize);

	for(int i=0;i<256;i++)

	{
		pctThis->rgbRed=pe[i].peRed;
		pctThis->rgbGreen=pe[i].peGreen;
		pctThis->rgbBlue=pe[i].peBlue;
		pctThis++;
	}
	::GlobalUnlock(hDIB);

	return true;
}
HBITMAP CopyScreenToBitmap(LPRECT lpRect)
{
	HDC hMemDC,hScrDC;
	HBITMAP hBitmap,hOldBitmap;
	int nX1,nY1,nX2,nY2;
	int nWidth,nHeight;
	int xScrn,yScrn;

	if(::IsRectEmpty(lpRect))
		return NULL;


	hScrDC=::CreateDC(_T("DISPLAY"),NULL,NULL,NULL);
	hMemDC=::CreateCompatibleDC(hScrDC);


	nX1=lpRect->left;
	nY1=lpRect->top;
	nX2=lpRect->right;
	nY2=lpRect->bottom;

	xScrn=::GetDeviceCaps(hScrDC,HORZRES);
	
    yScrn=::GetDeviceCaps(hScrDC,VERTRES);

	if(nX1<0)
		nX1=0;
	if(nY1<0)
		nY1=0;
	if(nX2>xScrn)
		nX2=xScrn;
	if(nY2>yScrn)
		nY2=yScrn;

	nWidth=nX2-nX1;
	nHeight=nY2-nY1;

	hBitmap=::CreateCompatibleBitmap(hScrDC,nWidth,nHeight);
    hOldBitmap=(HBITMAP)::SelectObject(hMemDC,hBitmap);

	::BitBlt(hMemDC,0,0,nWidth,nHeight,hScrDC,nX1,nY1,SRCCOPY);

	hBitmap=(HBITMAP)::SelectObject(hMemDC,hOldBitmap);

	::DeleteDC(hScrDC);
	::DeleteDC(hMemDC);

	return hBitmap;
}

HBITMAP CopyWindowToBitmap(HWND hWnd,WORD fPrintArea)
{
	HBITMAP hBitmap=NULL;

	if(!hWnd)
		return NULL;

	switch(fPrintArea)
	{
	case PW_WINDOW:
		{
			RECT rectWnd;

			::GetWindowRect(hWnd,&rectWnd);
			hBitmap=::CopyScreenToBitmap(&rectWnd);
			break;
		}
	case PW_CLIENT:
		{
			RECT rectClient;
			POINT pt1,pt2;

			::GetClientRect(hWnd,&rectClient);
			pt1.x=rectClient.left;
			pt1.y=rectClient.top;
			pt2.x=rectClient.right;
			pt2.y=rectClient.bottom;

			::ClientToScreen(hWnd,&pt1);
			::ClientToScreen(hWnd,&pt2);

			rectClient.left=pt1.x;
			rectClient.top=pt1.y;
			rectClient.right=pt2.x;
			rectClient.bottom=pt2.y;


			hBitmap=::CopyScreenToBitmap(&rectClient);
			break;
		}
	default:
		return NULL;
	}
	return hBitmap;
}
HBITMAP CopyClientRectToBitmap(HWND hWnd,LPRECT lpRect)
{
	HBITMAP hBitmap=NULL;


	if(!hWnd)
		return NULL;

	POINT pt1,pt2;
	pt1.x=lpRect->left;
	pt1.y=lpRect->top;
	pt2.x=lpRect->right;
	pt2.y=lpRect->bottom;

	::ClientToScreen(hWnd,&pt1);
	::ClientToScreen(hWnd,&pt2);

	lpRect->left=pt1.x;
	lpRect->top=pt1.y;
	lpRect->right=pt2.x;
	lpRect->bottom=pt2.y;


	hBitmap=::CopyScreenToBitmap(lpRect);

	pt1.x=lpRect->left;
	pt1.y=lpRect->top;
	pt2.x=lpRect->right;
	pt2.y=lpRect->bottom;

	::ScreenToClient(hWnd,&pt1);
	::ScreenToClient(hWnd,&pt2);

	lpRect->left=pt1.x;
	lpRect->top=pt1.y;
	lpRect->right=pt2.x;
	lpRect->bottom=pt2.y;
    


	return hBitmap;
}


HDIB CopySreenToDIB(LPRECT lpRect)
{
	HBITMAP hBitmap=NULL;
	HPALETTE hPal;
	HDIB hDIB=NULL;
//   ::AfxMessageBox(_T("在将屏幕转化为ＤＩＢ的函数中，首"));

	hBitmap=::CopyScreenToBitmap(lpRect);
    if(!hBitmap)
	{
	  ::AfxMessageBox(_T("在将屏幕转化为ＤＩＢ的函数中，获取ＤＤＢ无效"));
		return NULL;
	}

	hPal=::GetSystemPalette();

	hDIB=::BitmapToDIB(hBitmap,hPal);

	::DeleteObject(hPal);
	::DeleteObject(hBitmap);

	return hDIB;
}
HDIB CopyWindowToDIB(HWND hWnd,WORD fPrintArea)
{
	HBITMAP hBitmap=NULL;
	HPALETTE hPal;
	HDIB hDIB=NULL;

	hBitmap=::CopyWindowToBitmap(hWnd,fPrintArea);

    if(!hBitmap)
	{
	
		return NULL;
	}

	hPal=::GetSystemPalette();
    
	hDIB=::BitmapToDIB(hBitmap,hPal);

    ::DeleteObject(hPal);
	::DeleteObject(hBitmap);

	return hDIB;

}
HDIB CopyClientRectToDIB(HWND hWnd,LPRECT lpRect)
{
	
	HBITMAP hBitmap=NULL;
	HPALETTE hPal;
	HDIB hDIB=NULL;

	hBitmap=::CopyClientRectToBitmap(hWnd,lpRect);

    if(!hBitmap)
		return NULL;

	hPal=::GetSystemPalette();
    
	hDIB=::BitmapToDIB(hBitmap,hPal);

    ::DeleteObject(hPal);
	::DeleteObject(hBitmap);

	return hDIB;
}
VOID Delay( DWORD dwDelayTime)
{
	DWORD dwTimeBegin,dwTimeEnd;

	dwTimeBegin = ::GetTickCount();
	do
	{
		dwTimeEnd= ::GetTickCount();
	}
	while(dwTimeEnd - dwTimeBegin<dwDelayTime);
}

BOOL Fade(int nDeltaR, int nDeltaG, int nDeltaB,
          BYTE rm,BYTE gm,BYTE bm,
          BYTE *r,BYTE *g,BYTE *b)
{
	int R = *r + nDeltaR;
	int G = *g + nDeltaG;
	int B = *b + nDeltaB;

	if( nDeltaR<0)
		*r=(BYTE)BOUND( R,rm,255 );
	else
		*r=(BYTE)BOUND (R,0,rm);

	if( nDeltaG<0)
		*g=(BYTE)BOUND( G,gm,255 );
	else
		*g=(BYTE)BOUND (G,0,gm);

	if( nDeltaB<0)
		*b=(BYTE)BOUND( B,bm,255 );
	else
		*b=(BYTE)BOUND (B,0,bm);

	if( *r==rm && *g==gm && *b==bm)
		return true;
	else 
		return false;
}

HANDLE CopyHandle( HANDLE h)
{
	if(h==NULL)
		return NULL;

	DWORD dwLen=::GlobalSize((HGLOBAL)h);
	
	HANDLE hCopy=::GlobalAlloc(GHND,dwLen);
	if(hCopy==NULL)
		return NULL;


	void* lp=::GlobalLock(h);
	void* lpCopy=::GlobalLock(hCopy);

	::CopyMemory( lpCopy,lp,dwLen);

	::GlobalUnlock(h);
	::GlobalUnlock(hCopy);

	return hCopy;
}
void DrawTransparentBitmap(HDC hDC,HBITMAP hBitmap,LONG xStart,LONG yStart,
						   COLORREF cTransparentColor)
{
	BITMAP Bm;
	COLORREF cColor;
	HBITMAP bmAndBack,bmAndObject,bmAndMem,bmSave;
	HBITMAP bmBackOld,bmObjectOld,bmMemOld,bmSaveOld;
	HDC hDCMem,hDCBack,hDCObject,hDCTemp,hDCSave;
	POINT ptSize;

	hDCTemp = ::CreateCompatibleDC(hDC);
	::SelectObject(hDCTemp,hBitmap);

	::GetObject(hBitmap,sizeof(BITMAP),&Bm);
	ptSize.x = Bm.bmWidth;
	ptSize.y = Bm.bmHeight;
	::DPtoLP(hDCTemp,&ptSize,1);


    hDCMem=CreateCompatibleDC(hDC);
	hDCObject=CreateCompatibleDC(hDC);
	hDCBack=CreateCompatibleDC(hDC);
	hDCSave=CreateCompatibleDC(hDC);

	bmAndBack = ::CreateBitmap(ptSize.x,ptSize.y,1,1,NULL);
	bmAndObject = ::CreateBitmap(ptSize.x,ptSize.y,1,1,NULL);

	bmAndMem = ::CreateCompatibleBitmap(hDC,ptSize.x,ptSize.y);
	bmSave = ::CreateCompatibleBitmap(hDC,ptSize.x,ptSize.y);

	bmBackOld = (HBITMAP)SelectObject(hDCBack,bmAndBack);
	bmObjectOld = (HBITMAP)SelectObject(hDCObject,bmAndObject);
	bmMemOld = (HBITMAP)SelectObject(hDCMem,bmAndMem);
	bmSaveOld = (HBITMAP)SelectObject(hDCSave,bmSave);

	::SetMapMode(hDCTemp,::GetMapMode(hDC));


	::BitBlt( hDCSave, 0, 0, ptSize.x ,ptSize.y ,hDCTemp, 0,0, SRCCOPY);

	cColor = ::SetBkColor(hDCTemp, cTransparentColor);


	::BitBlt( hDCObject,0, 0, ptSize.x ,ptSize.y ,hDCTemp, 0,0, SRCCOPY);

	::SetBkColor(hDCTemp,cColor);

	::BitBlt( hDCBack, 0, 0, ptSize.x ,ptSize.y ,hDCObject, 0,0, NOTSRCCOPY);

	::BitBlt( hDCMem, 0, 0, ptSize.x ,ptSize.y ,hDC, xStart,yStart, SRCCOPY);


	::BitBlt( hDCMem, 0, 0, ptSize.x ,ptSize.y ,hDCObject, 0,0, SRCAND);


	::BitBlt( hDCTemp, 0, 0, ptSize.x ,ptSize.y ,hDCBack, 0,0, SRCAND);


	::BitBlt( hDCMem, 0, 0, ptSize.x ,ptSize.y ,hDCTemp, 0,0, SRCINVERT);


	//技巧操作结束，恢复老

	::BitBlt( hDC, xStart, yStart, ptSize.x ,ptSize.y ,hDCMem, 0,0, SRCCOPY);


	::BitBlt( hDCTemp, 0, 0, ptSize.x ,ptSize.y ,hDCSave, 0,0, SRCCOPY);


	::DeleteObject(::SelectObject(hDCBack,bmBackOld));
	::DeleteObject(::SelectObject(hDCObject,bmObjectOld));
	::DeleteObject(::SelectObject(hDCMem,bmMemOld));
	::DeleteObject(::SelectObject(hDCSave,bmSaveOld));

	::DeleteDC(hDCBack);
	::DeleteDC(hDCObject);
	::DeleteDC(hDCMem);
	::DeleteDC(hDCSave);
	::DeleteDC(hDCTemp);
}


















			























	 








	 
		







			




		                  




		








	
	


















































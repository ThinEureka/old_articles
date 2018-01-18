//DIB.h
//CDib类的头文件


#pragma once
#include "stdafx.h"
#include "DIBAPI.h"

#define EFFECT_SCANDOWN 1
#define EFFECT_SCANUP 2
#define EFFECT_SCANRIGHT 3
#define EFFECT_SCANLEFT 4 

#define EFFECT_VSPLITSCAN 5
#define EFFECT_HSPLITSCAN 6

#define EFFECT_MOVEDOWN 7
#define EFFECT_MOVEUP 8
#define EFFECT_MOVERIGHT 9
#define EFFECT_MOVELEFT 10

#define EFFECT_VSPLITMOVE 11
#define EFFECT_HSPLITMOVE 12

#define EFFECT_VCROSSMOVE 13
#define EFFECT_HCROSSMOVE 14

#define EFFECT_VRASTER 15
#define EFFECT_HRASTER 16

#define EFFECT_VBLIND 17
#define EFFECT_HBLIND 18

#define EFFECT_MOSAIC 19
#define EFFECT_FADE 20







class CDib:public CObject
{
	DECLARE_SERIAL(CDib)
public:
	CDib();
	BOOL Create(DWORD dwWidth,DWORD dwHeight);
	BOOL Create(DWORD dwWidth,DWORD dwHeight,WORD wBitCount);

	BOOL Create(LPBYTE lpDIB);
	BOOL Create(LPBYTE lpDIB,WORD wBitCount); 

	BOOL Create(HBITMAP hBitmap);
	BOOL Create(HBITMAP hBitmap,WORD wBitCount);

	BOOL Create(HBITMAP hDDB,HPALETTE hPal);
	BOOL Create(HBITMAP hDDB,HPALETTE hPal,WORD wBitCount);

	BOOL Create(CRect rcScrn);
	BOOL Create(HWND hWnd,WORD fPrintArea);
	BOOL Create(HWND hWnd,CRect rcClient);

	BOOL Attach(HDIB hDIB);

	BOOL Load(UINT uIDs,LPCTSTR lpszDIBType);
	BOOL Load(LPCTSTR lpszDIBRes,LPCTSTR lpszDIBType);

	BOOL Load(LPCTSTR lpszDIBFile);

	BOOL Save(LPCTSTR lpszDIBFile);

	BOOL Read(CFile* pFile);
	BOOL Write(CFile* pFile);


	virtual ~CDib();

	void Destroy();
	HDIB Detach();

	virtual void Serialize(CArchive &ar);

	BOOL Display( CDC* pDC,int xDest,int yDest,int nWidthDest,
		          int nHeightDest,int xSrc,int ySrc,DWORD dwRop=SRCCOPY);
	BOOL Display( CDC* pDC,
		          int xDest,int yDest,int nWidthDest,int nHeightDest,
				  int xSrc,int ySrc,int nWidthSrc,int nHeightSrc,
				  DWORD dwRop=SRCCOPY);
	BOOL Display( CDC* pDC,
		          int x,int y,
				  DWORD dwRop=SRCCOPY);
	BOOL Display( CDC* pDC,
		          CRect rcDest,CRect rcSrc,
				  DWORD dwRop=SRCCOPY);


	BOOL DisplayTransparent(CDC* pDC, int x, int y,COLORREF cTransparentColor);
    BOOL Display( int nEffect, CDC* pDC, int x,int y,int nDeta,DWORD dwDelayTime,
    DWORD dwRop=SRCCOPY);
    BOOL Clear( int nEffect, CDC* pDC, int x,int y,int nDeta,DWORD dwDelayTime);
private:


	BOOL DisplayFadeIn(CDC* pDC,int x,int y,int nDeta,DWORD dwDelayTime);
    BOOL DisplayFadeOut(CDC* pDC,int x,int y,int nDeta,DWORD dwDelayTime);

    BOOL DisplayMosaicIn(CDC* pDC,int xx,int yy,int nTileSize,DWORD dwDelayTime,
    DWORD dwRop=SRCCOPY);
   BOOL DisplayMosaicOut(CDC* pDC,int xx,int yy,int nTileSize,DWORD dwDelayTime);


public:
    
	BOOL DisplayPalette( CDC* pDC,CRect rc);

	CDC* BeginPaint(CDC* pDC);
	void EndPaint();

	BOOL BuildBitmap();
	BOOL BuildPalette();

	BOOL IsEmpty();

	DWORD GetCompression();
	WORD GetBitCount();
    LONG GetWidth();
	LONG GetHeight();
	LONG GetWidthBytes();
	WORD GetColorNumber();
	WORD GetPaletteSize();
	CBitmap* GetBitmap();
	CPalette* GetPalette();
	HANDLE GetHandle();
	LPBYTE GetBitsPtr();
	COLORREF GetPixel(LONG x,LONG y);
    LONG GetPixelOffset(LONG x,LONG y);
	BOOL ConvertFormat(WORD wBitCount);


private:
	BOOL UpdateInternal();
public:
	HDIB m_hDIB;
	HBITMAP m_hBitmap;
	CPalette* m_pPalette;
	CBitmap* m_pBitmap;


private:
	CDC* m_pMemDC;
	CBitmap* m_pBitmapTmp;
	CPalette* m_pPaletteTmp;
};



		                 





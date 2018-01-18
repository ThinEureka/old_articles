//DIBAPI.h
//DIBµÄº¯Êý¿â
#include "stdafx.h"
#pragma once
#define IS_WIN30_DIB(lpbi) ((*(LPDWORD)lpbi)==sizeof(BITMAPINFOHEADER))
#define WIDTHBYTES(bits) (((bits+31)/32)*4)
#define HDIB HANDLE
#define PW_WINDOW 1
#define PW_CLIENT 2
#define DIB_HEADER_MARKER
#define BOUND(x,mn,mx)   ((x)<(mn)?(mn):((x)>(mx)?(mx):(x)))
#define UPVALUE(x,n) ((x)/((n)?(n):1)+((x)%((n)?(n):1)?1:0))


HANDLE LoadDIB(LPCTSTR lpFileName);
HANDLE ReadDIBFile(HANDLE hFile);
LPBYTE LoadDIBFromResource(LPTSTR lpszBitmap);
HPALETTE CreateDIBPalette(LPBYTE lpBi);
HPALETTE CreateDIBPalette(HDIB hDIB);
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
LPBYTE FindDIBBits(HDIB hDIB);
WORD PaletteSize(LPBYTE lpDIB);
WORD PaletteSize(HDIB hDIB);
BOOL PaintBitmap(HDC hDC,LPRECT lpDCRect,HBITMAP hDDB,
                LPRECT lpDDBRect,HPALETTE hPal,DWORD dwRop);
HBITMAP DIBToDIBSection(LPBYTE lpDIB);
HBITMAP DIBToDIBSection(HDIB hDIB);
HDIB DIBSectionToDIB(HBITMAP hBitmap);
HDIB ConvertDIBFormat(LPBYTE lpSrcDIB,UINT nWidth,UINT nHeight,
                      UINT nbpp,BOOL bStretch,HPALETTE hPalSrc);
HDIB ConvertDIBFormat(HDIB hDIB,UINT nWidth,UINT nHeight,
                      UINT nbpp,BOOL bStretch,HPALETTE hPalSrc);
HDIB ConvertDIBFormat(LPBYTE lpSrcDIB,UINT nbpp,HPALETTE hPalSrc);
HDIB ConvertDIBFormat(HDIB hDIB,UINT nbpp,HPALETTE hPalSrc);
HDIB BitmapToDIB(HBITMAP hDDB,HPALETTE hPal);
HDIB BitmapToDIB(HBITMAP hDDB,HPALETTE hPal,WORD wBitCount);
HDIB ChangeBitmapFormat(HBITMAP hDDB,WORD biBits,
                         DWORD biCompression,HPALETTE hPal);
HBITMAP DIBToBitmap(HDIB hDIB,HPALETTE hPal);
HDIB ChangeDIBFormat(HDIB hDIB,WORD wBitCount,DWORD dwCompression);
BOOL CopyColorTable(LPBITMAPINFO lpTgt,LPBITMAPINFO lpSrc,HPALETTE hPalSrc);
BOOL MapDIBColorsToPalette(HDIB hDIB,HPALETTE hPal);
HBITMAP CopyScreenToBitmap(LPRECT lpRect);
HBITMAP CopyWindowToBitmap(HWND hWnd,WORD fPrintArea);
HBITMAP CopyClientRectToBitmap(HWND hWnd,LPRECT lpRect);
HDIB CopySreenToDIB(LPRECT lpRect);
HDIB CopyWindowToDIB(HWND hWnd,WORD fPrintArea);
HDIB CopyClientRectToDIB(HWND hWnd,LPRECT lpRect);
VOID Delay( DWORD dwDelayTime);
BOOL Fade(int nDeltaR, int nDeltaG, int nDeltaB, BYTE rm,BYTE gm,BYTE bm,BYTE *r,BYTE *g,BYTE *b);
HANDLE CopyHandle( HANDLE h);
void DrawTransparentBitmap(HDC hDC,HBITMAP hBitmap,LONG xStart,LONG yStart,COLORREF cTransparentColor);




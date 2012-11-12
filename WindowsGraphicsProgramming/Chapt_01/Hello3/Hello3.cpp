//-----------------------------------------------------------------------------------//
//              Windows Graphics Programming: Win32 GDI and DirectDraw               //
//                             ISBN  0-13-086985-6                                   //
//                                                                                   //
//  Written            by  Yuan, Feng                             www.fengyuan.com   //
//  Copyright (c) 2000 by  Hewlett-Packard Company                www.hp.com         //
//  Published          by  Prentice Hall PTR, Prentice-Hall, Inc. www.phptr.com      //
//                                                                                   //
//  FileName   : hello3.cpp						                                     //
//  Description: Hello World Demo 3, C++ window, Chapter 1                           //
//  Version    : 1.00.000, May 31, 2000                                              //
//-----------------------------------------------------------------------------------//

#define STRICT
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <assert.h>
#include <tchar.h>

#include "..\..\include\win.h"

const TCHAR szMessage[] = _T("Hello, World !");
const TCHAR szFace[]    = _T("Times New Roman");
const TCHAR szHint[]    = _T("Press ESC to quit.");
const TCHAR szProgram[] = _T("HelloWorld3");

void CenterText(HDC hDC, int x, int y, LPCTSTR szFace, LPCTSTR szMessage, int point)
{
    HFONT hFont = CreateFont(
        - point * GetDeviceCaps(hDC, LOGPIXELSY) / 72,
        0, 0, 0, FW_BOLD, TRUE, FALSE, FALSE, 
        ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, 
        PROOF_QUALITY, VARIABLE_PITCH, szFace);
    assert(hFont);

    HGDIOBJ hOld = SelectObject(hDC, hFont);

    SetTextAlign(hDC, TA_CENTER | TA_BASELINE);
	//SetTextAlign(hDC, GetTextAlign(hDC) & (~TA_CENTER));

    SetBkMode(hDC, TRANSPARENT);
    SetTextColor(hDC, RGB(0, 0, 0xFF));
    TextOut(hDC, x, y, szMessage, _tcslen(szMessage));

    SelectObject(hDC, hOld);
    DeleteObject(hFont);
}


class KHelloWindow : public KWindow
{
    void OnKeyDown(WPARAM wParam, LPARAM lParam)
    {
        if ( wParam==VK_ESCAPE )
            PostMessage(m_hWnd, WM_CLOSE, 0, 0);
    }

    void OnDraw(HDC hDC)
    {
		int x,y;
		// 		x = GetDeviceCaps(hDC, HORZRES)/2;
		// 		y = GetDeviceCaps(hDC, VERTRES)/2;
		RECT rcClient;
		GetClientRect(m_hWnd, &rcClient);
		x = (rcClient.right - rcClient.left) / 2;
		y = (rcClient.bottom - rcClient.top) / 2;

		MoveToEx(hDC, 0, y, NULL);
		LineTo(hDC, rcClient.right - rcClient.left, y);

		MoveToEx(hDC, x, 0, NULL);
		LineTo(hDC, x, rcClient.bottom - rcClient.top);

        TextOut(hDC, 0, 0, szHint, lstrlen(szHint));		
        CenterText(hDC, x, y, szFace, szMessage, 72);
    }

public:
    
};


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, 
                   LPSTR lpCmd, int nShow)
{
    KHelloWindow win;
    
	int nSystemMetricsX = GetSystemMetrics(SM_CXSCREEN);
	int nSystemMetricsY = GetSystemMetrics(SM_CYSCREEN);
	int nWidth = GetSystemMetrics(SM_CXSCREEN) / 5 * 3;
	int nHeight = GetSystemMetrics(SM_CYSCREEN) / 5 * 3;

    win.CreateEx(0, szProgram, szProgram,
	             WS_POPUP,
	             (nSystemMetricsX-nWidth)/2, (nSystemMetricsY-nHeight)/2,
	             nWidth, nHeight,
	             NULL, NULL, hInst);
        
    win.ShowWindow(nShow);

    win.UpdateWindow();
  
    return win.MessageLoop();
}
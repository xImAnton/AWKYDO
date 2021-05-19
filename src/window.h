#pragma once
#include <Windows.h>
#include <tchar.h>


namespace AWKYDO {
    namespace Window {
        BOOL RegisterWindowClass(HINSTANCE hInst, WNDPROC wndProc);
        HWND CreateOverlayWindow(HINSTANCE hInst, HWND hDesktop);
        static LPCTSTR sWindowClassName = _T("AWKYDO_Overlay");
    }
}
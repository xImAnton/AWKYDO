#include "DesktopWindow.h"


BOOL bFoundSysHeader32 = FALSE;


BOOL IsWindowOfClass(HWND hWnd, LPSTR className) {
    LPSTR windowClassName = new TCHAR[255];
    RealGetWindowClass(hWnd, windowClassName, 255);

    return _tcsicmp(className, className) == 0;
}


BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam) {
    //std::wstring a;
    LPSTR className = new TCHAR[255];
    //a.resize(255);
    int size = RealGetWindowClass(hwnd, className, 255);
    //a.resize(size);

    if (bFoundSysHeader32) {
        std::cout << "found" << std::endl;
        HWND* hWorkerW = reinterpret_cast<HWND*>(lParam);
        *hWorkerW = hwnd;
        return FALSE;
    }

    if (_tcsicmp(className, _T("sysheader32")) == 0) {
        bFoundSysHeader32 = TRUE;
    }

    return TRUE;
}

HWND AWKYDO::Desktop::GetDesktopBackgroundHandle() {
    HWND desktopWindow = GetDesktopWindow();

    HWND folderView = nullptr;
    EnumChildWindows(desktopWindow, EnumChildProc, reinterpret_cast<LPARAM>(&folderView));
    std::cout << "getbg" << std::endl;

    return folderView;
}
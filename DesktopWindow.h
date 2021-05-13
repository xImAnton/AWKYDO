#pragma once

#include <Windows.h>
#include <iostream>
#include <string>


// AWKYDO will keep your Desktop organized

/*
Gdiplus::Rect rSelection = { 100, 100, 200, 200 };


void DrawDesktop(HWND hDesktop) {
    HDC hDC = GetDC(hDesktop);
    Gdiplus::Graphics g(hDC);

    Gdiplus::SolidBrush brush(Gdiplus::Color(70, 0, 102, 204));
    g.FillRectangle(&brush, rSelection);

    Gdiplus::Pen pen(Gdiplus::Color(255, 50, 50, 127));
    g.DrawRectangle(&pen, rSelection);
}*/


/*int main() {
    try {
        // init com pointers if not --> access violation
        CComInit coInit;
        // get desktop folder view
        CComPtr<IFolderView2> desktopFolderView = GetDesktopFolderView();

        // get item size and view mode
        FOLDERVIEWMODE viewMode = FVM_AUTO;
        int iconSize = 0;
        ThrowIfFailed(desktopFolderView->GetViewModeAndIconSize(&viewMode, &iconSize), "GetViewModeAndIconSize failed");
        std::cout << "Current View mode: " << viewMode << ", icon size: " << iconSize << std::endl;

        // modify item size and view mode
        ThrowIfFailed(desktopFolderView->SetViewModeAndIconSize(FVM_AUTO, iconSize), "SetViewModeAndIconSize failed");

        HWND desktopHandle = desktop::GetDesktopHandle();
        desktopHandle = (HWND) 0x10202;
        std::cout << "DesktopHandle: " << desktopHandle << std::endl;

        // initialise GDI+
        Gdiplus::GdiplusStartupInput gdiplusStartupInput;
        ULONG_PTR gdiplusToken;
        Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

        DrawDesktop(desktopHandle);

        Gdiplus::GdiplusShutdown(gdiplusToken);
        return 0;
    } catch (std::system_error const& e) {
        std::cout << "ERROR: " << e.what() << ", error code: " << e.code() << "\n";
        return 1;
    }
}*/

namespace Desktop {
	HWND GetDesktopHandle();
}
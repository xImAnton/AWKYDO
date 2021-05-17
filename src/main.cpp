#include <tchar.h>
#include <Windows.h>
#include <gdiplus.h>
#include "awklib.h"
#include "DesktopWindow.h"

#pragma comment (lib, "Gdiplus.lib")

VOID DrawIconOnDesktop(HDC hDC, PAINTSTRUCT* ps) {
    Gdiplus::Graphics graphics(hDC);

    // load app logo and display to desktop background
    auto* im = new Gdiplus::Image(_T("resources/icon.png"));

    // calculate image positions
    UINT screenHeight = ps->rcPaint.bottom - ps->rcPaint.top - 1;
    UINT newHeight = screenHeight / 3;
    UINT newWidth = newHeight * (im->GetWidth() / im->GetHeight());
    int x = (ps->rcPaint.right - ps->rcPaint.left - newWidth - 1) / 2;
    int y = (ps->rcPaint.bottom - ps->rcPaint.top - newHeight - 1) / 2;
    Gdiplus::Rect destRect(x, y, newWidth, newHeight);

    // draw logo
    graphics.DrawImage(im, destRect);
}


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
    // first, get the desktop background handle
    HWND hDesktop = AWKYDO::Desktop::GetDesktopBackgroundHandle();
    std::cout << "Desktop Handle: " << hDesktop << std::endl;

    // Initialize GDI+.
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    // register window class
    AWKYDO::Window::RegisterWindowClass(hInstance, WndProc);
    // spawn a new overlay window
    HWND hWindow = AWKYDO::Window::CreateOverlayWindow(hInstance, hDesktop);

    // spawn notification bar icon
    AWKYDO::Notification::RegisterNotificationIcon(hWindow);

    // show window
    ShowWindow(hWindow, nShowCmd);
    UpdateWindow(hWindow);

    // message loop
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // close our window
    CloseWindow(hWindow);

    // shutdown GDI+
    Gdiplus::GdiplusShutdown(gdiplusToken);

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        // paint msg
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hDc = BeginPaint(hWnd, &ps);
            DrawIconOnDesktop(hDc, &ps);
            EndPaint(hWnd, &ps);
            return 0;
        }
        // events for our notification icon
        case AWK_WM_ICON_NOTIFY: {
            switch (lParam) {
                // when clicked
                case WM_LBUTTONDOWN:
                case WM_RBUTTONDOWN: {
                    /*HMENU hContextMenu = CreatePopupMenu();
                    InsertMenu(hContextMenu, 0, MF_BYPOSITION | MF_STRING, 0xb1f891ac45c5451a, _T("Exit"));
                    TrackPopupMenu(hContextMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, 0, 0, 0, hWnd, NULL);*/
                    // End AWKYDO when notification icon is clicked
                    PostQuitMessage(0);
                    }
                default: {}
            }
            return 0;
        }
        default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}
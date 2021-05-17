#include <tchar.h>
#include <Windows.h>
#include <gdiplus.h>
#include "window.h"
#include "DesktopWindow.h"
#include "notification.h"

#pragma comment (lib, "Gdiplus.lib")

VOID DrawLogoOnDesktop(HDC hDC, PAINTSTRUCT* ps) {
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
    HWND hDesktop = AWKYDO::GetDesktopBackgroundHandle();
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
    NOTIFYICONDATA nidNotifyIcon = { };
    AWKYDO::Notification::GetNotificationIcon(hWindow, &nidNotifyIcon);
    Shell_NotifyIcon(NIM_ADD, &nidNotifyIcon);

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

    // remove notification icon
    Shell_NotifyIcon(NIM_DELETE, &nidNotifyIcon);

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
            DrawLogoOnDesktop(hDc, &ps);
            EndPaint(hWnd, &ps);
            return 0;
        }
        // events for our notification icon
        case AWK_WM_ICON_NOTIFY: {
            switch (lParam) {
                // when clicked
                case WM_LBUTTONDOWN:
                case WM_RBUTTONDOWN: {
                    HMENU hContextMenu = CreatePopupMenu();
                    InsertMenu(hContextMenu, 0, MF_BYPOSITION | MF_STRING, AWK_UID_NOTIFY_MENU_EXIT, _T("Exit"));
                    InsertMenu(hContextMenu, 1, MF_BYPOSITION | MF_STRING, AWK_UID_NOTIFY_MENU_OPENCFG, _T("Open Settings"));
                    POINT pCursorPos;
                    GetCursorPos(&pCursorPos);
                    TrackPopupMenu(hContextMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, pCursorPos.x, pCursorPos.y, 0, hWnd, nullptr);
                    }
                default: {}
            }
            return 0;
        }
        case WM_COMMAND: {
            if (lParam == 0) { // Menu action
                switch (wParam) {
                    case AWK_UID_NOTIFY_MENU_EXIT:
                        PostQuitMessage(0);
                    case AWK_UID_NOTIFY_MENU_OPENCFG:
                    default: {
                    }
                }
                return 0;
            }
        }
        default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}
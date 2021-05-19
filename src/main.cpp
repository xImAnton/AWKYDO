#include <tchar.h>
#include <Windows.h>
#include <gdiplus.h>
#include "window.h"
#include "DesktopWindow.h"
#include "notification.h"

#pragma comment (lib, "Gdiplus.lib")


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
    // Initialize GDI+.
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    WNDCLASSEX wcex;

    wcex.hInstance = hInstance;
    wcex.lpszClassName = AWKYDO::Window::sWindowClassName;
    wcex.lpfnWndProc = WndProc;
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.lpszMenuName = nullptr;
    wcex.cbWndExtra = 0;
    wcex.cbClsExtra = 0;
    wcex.hbrBackground = (HBRUSH) COLOR_BACKGROUND;
    wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);

    if (!RegisterClassEx(&wcex)) {
        MessageBox(nullptr, _T("Call to RegisterClassEx failed!"), AWKYDO::Window::sWindowClassName, NULL);
        return 1;
    }

    HWND hDesktop = AWKYDO::GetDesktopBackgroundHandle();
    std::cout << hDesktop << std::endl;

    HWND hWindow = CreateWindowEx(
            WS_EX_LAYERED | WS_EX_NOACTIVATE | WS_EX_TRANSPARENT,
            AWKYDO::Window::sWindowClassName,
            nullptr,
            WS_VISIBLE,
            CW_USEDEFAULT, CW_USEDEFAULT, 500, 500,
            nullptr,
            nullptr,
            hInstance,
            nullptr
    );

    if (!hWindow) {
        MessageBox(nullptr, _T("Call to CreateWindowEx failed!"), AWKYDO::Window::sWindowClassName, NULL);
        return 1;
    }

    /* since the windows api doesn't let us create a layered window with WS_CHILD style in the CreateWindowEx Call,
     * we have to set it afterwards. */
    if (!SetParent(hWindow, hDesktop)) {
        MessageBox(nullptr, _T("Call to SetParent failed!"), AWKYDO::Window::sWindowClassName, NULL);
        return 1;
    }

    ShowWindow(hWindow, nShowCmd);

    RECT rcWnd;
    GetWindowRect(hWindow, &rcWnd);
    SIZE szWnd = { rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top };
    HDC hdc = GetDC(hWindow);
    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP bmMem = CreateCompatibleBitmap(hdc, szWnd.cx, szWnd.cy);
    SelectObject(hdcMem, bmMem);

    auto* im = new Gdiplus::Image(_T("resources/icon.png"));
    Gdiplus::Graphics graphics(hdcMem);
    Gdiplus::Rect rcImgDest(0, 0, szWnd.cx, szWnd.cy);

    graphics.DrawImage(im, rcImgDest);

    HDC hdcScreen = GetDC(nullptr);
    POINT ptSrc = { 0, 0 };


    BLENDFUNCTION bf;
    bf.AlphaFormat = AC_SRC_ALPHA;
    bf.BlendFlags = 0;
    bf.BlendOp = AC_SRC_OVER;
    bf.SourceConstantAlpha = 255;
    UpdateLayeredWindow(hWindow, hdcScreen, &ptSrc, &szWnd, hdcMem, &ptSrc, 0, &bf, 2);

    DeleteDC(hdcMem);
    DeleteObject(bmMem);

    // spawn notification bar icon
    NOTIFYICONDATA nidNotifyIcon = { };
    AWKYDO::Notification::GetNotificationIcon(hWindow, &nidNotifyIcon);
    Shell_NotifyIcon(NIM_ADD, &nidNotifyIcon);
    // message loop
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // close our window
    DeleteObject(hWindow);

    // remove notification icon
    Shell_NotifyIcon(NIM_DELETE, &nidNotifyIcon);

    // shutdown GDI+
    Gdiplus::GdiplusShutdown(gdiplusToken);

    return (int)msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
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
                        break;
                    case AWK_UID_NOTIFY_MENU_OPENCFG:
                        printf("open cfg");
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
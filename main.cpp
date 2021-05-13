#include <tchar.h>
#include <Windows.h>
#include <gdiplus.h>
#include "awklib.h"

#pragma comment (lib,"Gdiplus.lib")

VOID DrawIconOnDesktop(HDC hDC, PAINTSTRUCT* ps) {
    // HBRUSH brush = CreateSolidBrush(RGB(50, 50, 80));
    // FillRect(hDC, &(ps->rcPaint), brush);

    Gdiplus::Graphics graphics(hDC);
    Gdiplus::Image* im = new Gdiplus::Image(L"icon.png");

    UINT screenHeight = ps->rcPaint.bottom - ps->rcPaint.top - 1;

    UINT newHeight = screenHeight / 3;
    UINT newWidth = newHeight * (im->GetWidth() / im->GetHeight());

    Gdiplus::Rect destRect((ps->rcPaint.right - ps->rcPaint.left - newWidth - 1) / 2, (ps->rcPaint.bottom - ps->rcPaint.top - newHeight - 1) / 2, newWidth, newHeight);

    graphics.DrawImage(im, destRect);
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
    // check if desktop background always has this handle, if no --> not gud
    HWND hDesktop = (HWND)0x10202;

    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;

    // Initialize GDI+.
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    AWKYDO::Window::RegisterWindowClass(hInstance, WndProc);
    HWND hWindow = AWKYDO::Window::CreateOverlayWindow(hInstance, hDesktop);

    AWKYDO::Notification::RegisterNotificationIcon(hWindow);
    
    ShowWindow(hWindow, nShowCmd);
    UpdateWindow(hWindow);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    Gdiplus::GdiplusShutdown(gdiplusToken);

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    switch (uMsg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hDc = BeginPaint(hWnd, &ps);

            DrawIconOnDesktop(hDc, &ps);

            EndPaint(hWnd, &ps);
            return 0;
        }
        case AWK_WM_ICONNOTIFY: {

            switch (lParam) {
            case WM_LBUTTONDOWN:
            case MKF_RIGHTBUTTONDOWN: {
                /*HMENU hContextMenu = CreatePopupMenu();
                InsertMenu(hContextMenu, 0, MF_BYPOSITION | MF_STRING, 0xb1f891ac45c5451a, L"Exit");
                TrackPopupMenu(hContextMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, 0, 0, 0, hWnd, NULL);*/
                // End AWKYDO when notification icon is clicked
                PostQuitMessage(0);
            }
            }
            return 0;
        }
        default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}
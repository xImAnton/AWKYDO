#include "window.h"


/***
 * registers the class for our overlay window to windows
 * @param hInst HINSTANCE of our process
 * @param wndProc main window procedure
 * @return whether the class was registered successful
 */
BOOL AWKYDO::Window::RegisterWindowClass(HINSTANCE hInst, WNDPROC wndProc) {
    WNDCLASS wc = { 0 };

    wc.lpfnWndProc = wndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = sWindowClassName;

    if (!RegisterClass(&wc)) {
        MessageBox(nullptr, _T("Call to RegisterClass failed!"), sWindowClassName, NULL);
        return FALSE;
    }
    return TRUE;
}


/***
 * creates a desktop background overlay window that is used to display
 * the AWKYDO logo and the groups of items
 * @param hInst HINSTANCE of our process
 * @param hDesktop handle of the desktop background window
 * @return a handle to the newly created window
 */
HWND AWKYDO::Window::CreateOverlayWindow(HINSTANCE hInst, HWND hDesktop) {
    // get desktop boundaries
    RECT rectDesktopWindow;
    GetClientRect(hDesktop, &rectDesktopWindow);

    return CreateWindowEx(
        // extended style: NOACTIVATE --> not focusable, transparent
        WS_EX_NOACTIVATE,

        // our window class
        sWindowClassName,

        // no window title
        nullptr,

        // window styles
        WS_CHILD | WS_VISIBLE,

        // boundaries of desktop background window
        rectDesktopWindow.left, rectDesktopWindow.top, rectDesktopWindow.right, rectDesktopWindow.bottom,

        // desktop window as parent
        hDesktop,

        // no menubar
        nullptr,

        // module instance
        hInst,

        // lpParam
        nullptr);
}
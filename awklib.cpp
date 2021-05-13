#include "awklib.h"

void AWKYDO::Notification::GetNotificationIcon(HWND hWnd, NOTIFYICONDATA* nid) {
	HICON hIcon = static_cast<HICON>(LoadImage(NULL, TEXT("icon.ico"), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR | LR_SHARED | LR_DEFAULTSIZE | LR_LOADFROMFILE));

	nid->cbSize = sizeof(nid);
	nid->hWnd = hWnd;
	nid->guidItem = AWK_GUID_NOTIFICATIONICON;
	nid->uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE | NIF_GUID;
	nid->uCallbackMessage = AWK_WM_ICONNOTIFY;
	nid->hIcon = hIcon;

	// tooltip
	StringCchCopy(nid->szTip, ARRAYSIZE(nid->szTip), L"AWKYDO");
}

void AWKYDO::Notification::RegisterNotificationIcon(HWND hWnd) {
	NOTIFYICONDATA nid = { };
	AWKYDO::Notification::GetNotificationIcon(hWnd, &nid);
	Shell_NotifyIcon(NIM_ADD, &nid);
}

BOOL AWKYDO::Window::RegisterWindowClass(HINSTANCE hInst, WNDPROC wndProc) {
	WNDCLASS wc = { 0 };

	wc.lpfnWndProc = wndProc;
	wc.hInstance = hInst;
	wc.lpszClassName = sWindowClassName;

	if (!RegisterClass(&wc)) {
		MessageBox(NULL, L"Call to RegisterClass failed!", sWindowClassName, NULL);
		return FALSE;
	}
	return TRUE;
}

RECT GetDesktopWindowRect(HWND hWnd) {
	RECT rectClient;
	GetWindowRect(hWnd, &rectClient);
	return { 0, 0, rectClient.right - rectClient.left - 1, rectClient.bottom - rectClient.top - 1 };
}

HWND AWKYDO::Window::CreateOverlayWindow(HINSTANCE hInst, HWND hDesktop) {
	RECT rectDesktopWindow = GetDesktopWindowRect(hDesktop);

	return CreateWindowEx(WS_EX_NOACTIVATE, // extended style
		sWindowClassName, // window class
		NULL, // title
		WS_CHILD | WS_VISIBLE, // styles
		rectDesktopWindow.left, rectDesktopWindow.top, rectDesktopWindow.right, rectDesktopWindow.bottom,
		hDesktop, // parent
		NULL, // menu
		hInst, // module instance
		NULL); // lpParam
}
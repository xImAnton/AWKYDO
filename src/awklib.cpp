#include "awklib.h"

void AWKYDO::Notification::GetNotificationIcon(HWND hWnd, NOTIFYICONDATA* nid) {
	HICON hIcon = static_cast<HICON>(LoadImage(nullptr, TEXT("resources/icon.ico"), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR | LR_SHARED | LR_DEFAULTSIZE | LR_LOADFROMFILE));

	nid->cbSize = sizeof(nid);
	nid->hWnd = hWnd;
	nid->guidItem = AWK_GUID_NOTIFICATION_ICON;
	nid->uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE | NIF_GUID;
	nid->uCallbackMessage = AWK_WM_ICON_NOTIFY;
	nid->hIcon = hIcon;

	// tooltip
	StringCchCopy(nid->szTip, ARRAYSIZE(nid->szTip), "AWKYDO");
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
		MessageBox(nullptr, (LPCSTR)L"Call to RegisterClass failed!", sWindowClassName, NULL);
		return FALSE;
	}
	return TRUE;
}

RECT GetDesktopWindowRect(HWND hWnd) {
	RECT rectClient;
	GetClientRect(hWnd, &rectClient);
	return rectClient;
}

HWND AWKYDO::Window::CreateOverlayWindow(HINSTANCE hInst, HWND hDesktop) {
	RECT rectDesktopWindow = GetDesktopWindowRect(hDesktop);

	return CreateWindowEx(WS_EX_NOACTIVATE, // extended style
		sWindowClassName, // window class
		nullptr, // title
		WS_CHILD | WS_VISIBLE, // styles
		rectDesktopWindow.left, rectDesktopWindow.top, rectDesktopWindow.right, rectDesktopWindow.bottom,
		hDesktop, // parent
		nullptr, // menu
		hInst, // module instance
		nullptr); // lpParam
}
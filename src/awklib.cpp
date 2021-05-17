#include "awklib.h"


/***
 * Setups a new NOTIFYICONDATA with the data used for our notification icon
 * @param hWnd the handle to our main window
 * @param nid pointer to the NOTIFYICONDATA to setup
 */
void AWKYDO::Notification::GetNotificationIcon(HWND hWnd, NOTIFYICONDATA* nid) {
    // load icon
	HICON hIcon = static_cast<HICON>(LoadImage(nullptr, _T("resources/icon.ico"), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR | LR_SHARED | LR_DEFAULTSIZE | LR_LOADFROMFILE));

	nid->cbSize = sizeof(*nid);

	// handle to main window
	nid->hWnd = hWnd;

	// icon id
	nid->guidItem = AWK_GUID_NOTIFICATION_ICON;

    // message to send on events
    nid->uCallbackMessage = AWK_WM_ICON_NOTIFY;

    // set icon
    nid->hIcon = hIcon;

    // set icon tooltip
    StringCchCopy(nid->szTip, ARRAYSIZE(nid->szTip), _T("AWKYDO"));

	// icon --> show a icon
	// tip --> show a tooltip
	// message --> send a message to main window on events
	// guid --> id is valid
	nid->uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE | NIF_GUID;
}


/***
 * Register the notification icon for AWKYDO to windows
 * @param hWnd handle to our main window
 */
void AWKYDO::Notification::RegisterNotificationIcon(HWND hWnd) {
	NOTIFYICONDATA nid = { };
	// populate icon data
	AWKYDO::Notification::GetNotificationIcon(hWnd, &nid);

	// register icon
	Shell_NotifyIcon(NIM_ADD, &nid);
}


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
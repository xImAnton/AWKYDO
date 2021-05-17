#include "notification.h"


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
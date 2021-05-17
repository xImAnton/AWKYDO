#include "DesktopWindow.h"

/***
 * Checks if the specified window has the specified class name
 * @param hWnd the window to check
 * @param className the class name to compare
 * @return whether the window has the class
 */
BOOL IsWindowOfClass(HWND hWnd, LPCTSTR className) {
    LPTSTR classNam = new TCHAR[255];
    RealGetWindowClass(hWnd, classNam, 255);
    return _tcscmp(classNam, className) == 0;
}


/***
 * Data that is passed to the EnumWindowProc for finding the desktop background handle
 */
struct EnumTopLevelWindowPayload {
    /***
     * The handle to the found desktop handle
     */
    HWND backgroundWorkerW;

    /***
     * whether the window that displays the desktop items has been found.
     * It is displayed directly above the background so we can take the next window
     * as background window once we found this
     */
    BOOL foundDesktopIcons;
};


/***
 * EnumChildWindowProc for iterating over the children of all WorkerW windows
 * for finding the desktop icon view.
 * The Desktop icon folder view is wrapped in a WorkerW window.
 * Sets the bool foundDesktopIcons of the EnumTopLevelWindowPayload to TRUE when the WorkerW
 * has the folder view as child.
 */
BOOL CALLBACK EnumWorkerWChildProc(HWND hWnd, LPARAM lParam) {
    if (IsWindowOfClass(hWnd, _T("SHELLDLL_DefView"))) {
        BOOL* foundDesktopIcons = reinterpret_cast<BOOL*>(lParam);
        *foundDesktopIcons = TRUE;
        return FALSE;
    }
    return TRUE;
}


/***
 * EnumWindowProc for iterating all top level windows.
 * Used to iterate all children of WorkerW windows. For more info @see EnumWorkerWChildProc
 */
BOOL CALLBACK EnumTopLevelWindowProc(HWND hwnd, LPARAM lParam) {
    EnumTopLevelWindowPayload* payload = reinterpret_cast<EnumTopLevelWindowPayload*>(lParam);

    /***
     * when the desktop icon folder view was found in the previous window, this window is
     * the desktop background
     */
    if (payload->foundDesktopIcons) {
        printf("Desktop Background Handle has been Found\n");
        payload->backgroundWorkerW = hwnd;
        return FALSE;
    }

    /***
     * enum children of all WorkerW windows
     */
    if (IsWindowOfClass(hwnd, _T("WorkerW"))) {
        EnumChildWindows(hwnd, EnumWorkerWChildProc, reinterpret_cast<LPARAM>(&payload->foundDesktopIcons));
    }

    return TRUE;
}


/***
 * Searches the Window Handle for the Desktop Background
 * @return the HWND to the desktop background or nullptr of none found
 */
HWND AWKYDO::Desktop::GetDesktopBackgroundHandle() {
    EnumTopLevelWindowPayload payload = { nullptr, FALSE };
    EnumWindows(EnumTopLevelWindowProc, reinterpret_cast<LPARAM>(&payload));

    return payload.backgroundWorkerW;
}

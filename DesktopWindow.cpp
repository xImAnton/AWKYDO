#include "DesktopWindow.h"


BOOL IsWindowOfClass(HWND hWnd, const char* className) {
    std::string windowClass;
    windowClass.resize(255);
    size_t charCount = RealGetWindowClass(hWnd, &*windowClass.begin(), windowClass.size());
    windowClass.resize(charCount);

    return windowClass == className;
}


struct EnumTopLevelWindowPayload {
    HWND backgroundWorkerW;
    BOOL foundDesktopIcons;
};


BOOL CALLBACK EnumTopLevelWindowProc(HWND hwnd, LPARAM lParam) {
    EnumTopLevelWindowPayload* payload = reinterpret_cast<EnumTopLevelWindowPayload*>(lParam);

    if (payload->foundDesktopIcons) {
        std::cout << "found" << std::endl;
        payload->backgroundWorkerW = hwnd;
        return FALSE;
    }

    if (IsWindowOfClass(hwnd, "SysHeader32")) {
        payload->foundDesktopIcons = TRUE;
    }

    return TRUE;
}

HWND AWKYDO::Desktop::GetDesktopBackgroundHandle() {
    HWND desktopWindow = GetDesktopWindow();

    EnumTopLevelWindowPayload payload = { nullptr, FALSE };
    EnumChildWindows(desktopWindow, EnumTopLevelWindowProc, reinterpret_cast<LPARAM>(&payload));

    return payload.backgroundWorkerW;
}

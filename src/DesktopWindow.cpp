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


BOOL CALLBACK EnumWorkerWChildProc(HWND hWnd, LPARAM lParam) {
    if (IsWindowOfClass(hWnd, "SHELLDLL_DefView")) {
        BOOL* foundDesktopIcons = reinterpret_cast<BOOL*>(lParam);
        *foundDesktopIcons = TRUE;
        return FALSE;
    }
    return TRUE;
}


BOOL CALLBACK EnumTopLevelWindowProc(HWND hwnd, LPARAM lParam) {
    EnumTopLevelWindowPayload* payload = reinterpret_cast<EnumTopLevelWindowPayload*>(lParam);

    if (payload->foundDesktopIcons) {
        std::cout << "found" << std::endl;
        payload->backgroundWorkerW = hwnd;
        return FALSE;
    }

    if (IsWindowOfClass(hwnd, "WorkerW")) {
        EnumChildWindows(hwnd, EnumWorkerWChildProc, reinterpret_cast<LPARAM>(&payload->foundDesktopIcons));
    }

    return TRUE;
}


HWND AWKYDO::Desktop::GetDesktopBackgroundHandle() {
    EnumTopLevelWindowPayload payload = { nullptr, FALSE };
    EnumWindows(EnumTopLevelWindowProc, reinterpret_cast<LPARAM>(&payload));

    return payload.backgroundWorkerW;
}

#include "DesktopWindow.h"


BOOL bFoundSysHeader32 = FALSE;


namespace Desktop {
	BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam) {
		std::wstring windowClass;
		windowClass.resize(255);
		unsigned int chars = ::RealGetWindowClass(hwnd, &*windowClass.begin(), windowClass.size());
		windowClass.resize(chars);

		if (bFoundSysHeader32) {
			HWND* hWorkerW = reinterpret_cast<HWND*>(lParam);
			*hWorkerW = hwnd;
			return FALSE;
		}

		if (windowClass == L"SysHeader32") {
			bFoundSysHeader32 = TRUE;
		}

		return TRUE;
	}

	HWND GetDesktopBackgroundHandle() {
		HWND desktopWindow = GetDesktopWindow();

		HWND folderView = 0;
		EnumChildWindows(desktopWindow, EnumChildProc, reinterpret_cast<LPARAM>(&folderView));

		return folderView;
	}
}
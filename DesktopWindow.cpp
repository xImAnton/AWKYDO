#include "DesktopWindow.h"


namespace Desktop {
	BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam) {
		std::wstring windowClass;
		windowClass.resize(255);
		unsigned int chars = ::RealGetWindowClass(hwnd, &*windowClass.begin(), windowClass.size());
		windowClass.resize(chars);

		std::string s(windowClass.begin(), windowClass.end());

		std::cout << s << std::endl;

		return TRUE;

		if (windowClass != L"SysListView32") {
			return TRUE;
		}

		HWND* folderView = reinterpret_cast<HWND*>(lParam);
		int num = 10202;
		*folderView = hwnd;

		std::cout << "Found" << std::endl;

		return FALSE;
	}

	HWND GetDesktopHandle() {
		HWND desktopWindow = GetDesktopWindow();

		HWND folderView = 0;
		EnumChildWindows(desktopWindow, EnumChildProc, reinterpret_cast<LPARAM>(&folderView));

		return folderView;
	}
}
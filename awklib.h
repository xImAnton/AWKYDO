#pragma once
#include <Windows.h>
#include <strsafe.h>

#define AWK_WM_ICON_NOTIFY (WM_APP + 1)
#define AWK_GUID_NOTIFICATION_ICON { 0x8F3C8D04, 0xE22B, 0x4683, {0xB4, 0xE6, 0x0F, 0x61, 0xD1, 0xF2, 0xE1, 0x19} }

namespace AWKYDO {
	namespace Notification {
		void GetNotificationIcon(HWND, NOTIFYICONDATA*);
		void RegisterNotificationIcon(HWND hWnd);
	}
	
	namespace Window {
		BOOL RegisterWindowClass(HINSTANCE hInst, WNDPROC wndProc);
		HWND CreateOverlayWindow(HINSTANCE hInst, HWND hDesktop);
		static LPCSTR sWindowClassName = (LPCSTR)L"AWKYDO_Overlay";
	}
}
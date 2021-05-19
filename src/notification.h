#pragma once
#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>

#define AWK_WM_ICON_NOTIFY (WM_APP + 1)
#define AWK_GUID_NOTIFICATION_ICON { 0x8F3C8D04, 0xE22B, 0x4683, { 0xB4, 0xE6, 0x0F, 0x61, 0xD1, 0xF2, 0xE1, 0x19 } }
#define AWK_UID_NOTIFY_MENU_EXIT 0x1000
#define AWK_UID_NOTIFY_MENU_OPENCFG 0x1001


namespace AWKYDO {
    namespace Notification {
        void GetNotificationIcon(HWND, NOTIFYICONDATA*);
    }
}
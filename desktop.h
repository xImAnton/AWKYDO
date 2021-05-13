#pragma once

#include <Windows.h>
#include <ShlObj.h>     // Shell API
#include <atlcomcli.h>  // CComPtr & Co.
#include <string> 
#include <iostream> 
#include <system_error>
#include <tchar.h>

CComPtr<IFolderView2> GetDesktopFolderView();

// Throw a std::system_error if the HRESULT indicates failure.
template< typename T >
void ThrowIfFailed(HRESULT hr, T&& msg) {
    if (FAILED(hr)) throw std::system_error{ hr, std::system_category(), std::forward<T>(msg) };
}

// RAII wrapper to initialize/uninitialize COM
struct CComInit {
    HRESULT hr = ::CoInitialize(nullptr);
    CComInit() { ThrowIfFailed(hr, "CoInitialize failed"); }
    ~CComInit() { ::CoUninitialize(); }
};

void FindDesktopFolderView(REFIID riid, void** ppv, std::string const& interfaceName);

// experimental
enum DesktopWindow {
    ProgMan,
    SHELLDLL_DefViewParent,
    SHELLDLL_DefView,
    SysListView32
};

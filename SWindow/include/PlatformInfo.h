#ifndef PLATFORMINFO_H_INCLUDED
#define PLATFORMINFO_H_INCLUDED

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

class PlatformInfo {
    #ifdef _WIN32

public:
    PlatformInfo() {}
    HINSTANCE hIns;
    HDC hDc;
    HGLRC hRc;
    HWND hwnd;
    HPALETTE hPalette = 0;
    const char* title;
    MSG msg;
    int close = 0;

    #endif // _WIN32
};

#endif // PLATFORMINFO_H_INCLUDED

#ifndef PLATFORMINFO_H_INCLUDED
#define PLATFORMINFO_H_INCLUDED

#ifdef _WIN32
#include <windows.h>
#else
#include <GL/glx.h>
#include <GL/gl.h>
#include <X11/X.h>
#include <X11/keysym.h>
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

    #else
public:

    PlatformInfo() {
        snglBuf[0] = GLX_RGBA;
        snglBuf[1] = GLX_DEPTH_SIZE;
        snglBuf[2] = 16;
        snglBuf[3] = None;

        dblBuf[0] = GLX_RGBA;
        dblBuf[1] = GLX_DEPTH_SIZE;
        dblBuf[2] = 16;
        dblBuf[3] = GLX_DOUBLEBUFFER;
        dblBuf[4] = None;

        close = 0;
    }

    int snglBuf[4];
    int dblBuf[5];

    int close;
    Display   *dpy;
    Window     win;
    GLboolean  doubleBuffer = GL_TRUE;
    Atom wmDeleteMessage;
    #endif // _WIN32
};

#endif // PLATFORMINFO_H_INCLUDED

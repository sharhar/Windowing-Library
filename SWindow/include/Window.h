#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#ifdef _WIN32
#include "Win32Window.h"
#endif

class Window
{
    public:
        Window(char* title, int width, int height);
        PlatformInfo* info;
        void swapBuffers();
        bool isCloseRequested();
        void destroy();
        void poll();
    private:
        int m_width;
        int m_height;
        char* m_title;
};

#endif // WINDOW_H

#pragma once

#ifndef X11WINDOW_H
#define X11WINDOW_H

#include "PlatformInfo.h"
#include <stdio.h>
#include <stdlib.h>
#include <GL/glx.h>
#include <GL/gl.h>
#include <X11/X.h>
#include <X11/keysym.h>

inline void platform_create(const char* title, int width, int height, PlatformInfo* info) {
  XVisualInfo         *vi;
  Colormap             cmap;
  XSetWindowAttributes swa;
  GLXContext           cx;
  int                  dummy;

  info->dpy = XOpenDisplay(NULL);
  if (info->dpy == NULL)
    printf("could not open display");

  if(!glXQueryExtension(info->dpy, &dummy, &dummy))
    printf("X server has no OpenGL GLX extension");

  vi = glXChooseVisual(info->dpy, DefaultScreen(info->dpy), info->dblBuf);
  if (vi == NULL)
  {
    vi = glXChooseVisual(info->dpy, DefaultScreen(info->dpy), info->snglBuf);
    if (vi == NULL) printf("no RGB visual with depth buffer");
    info->doubleBuffer = GL_FALSE;
  }
  //if(vi->class != TrueColor)
  //  fatalError("TrueColor visual required for this program");

  cx = glXCreateContext(info->dpy, vi, None, GL_TRUE);
  if (cx == NULL)
    printf("could not create rendering context");

  cmap = XCreateColormap(info->dpy, RootWindow(info->dpy, vi->screen), vi->visual, AllocNone);
  swa.colormap = cmap;
  swa.border_pixel = 0;
  swa.event_mask = KeyPressMask    | ExposureMask
                 | ButtonPressMask | StructureNotifyMask;
  info->win = XCreateWindow(info->dpy, RootWindow(info->dpy, vi->screen), 0, 0,
                      300, 300, 0, vi->depth, InputOutput, vi->visual,
                      CWBorderPixel | CWColormap | CWEventMask, &swa);
  XSetStandardProperties(info->dpy, info->win, "main", "main", None, (*""), 0, NULL);
  glXMakeCurrent(info->dpy, info->win, cx);
  XMapWindow(info->dpy, info->win);

  info->wmDeleteMessage = XInternAtom(info->dpy, "WM_DELETE_WINDOW", False);
  XSetWMProtocols(info->dpy, info->win, &info->wmDeleteMessage, 1);
}

inline void platform_poll(PlatformInfo* info) {
    XEvent               event;

    do
    {
      XNextEvent(info->dpy, &event);
      switch (event.type)
      {
        case KeyPress:
        {
          KeySym     keysym;
          //XKeyEvent *kevent;
          char       buffer[1];
          //kevent = (XKeyEvent *) &event;
          if ((XLookupString((XKeyEvent *)&event,buffer,1,&keysym,NULL) == 1) && (keysym == (KeySym)XK_Escape) )
            exit(0);
          break;
        }
        case ButtonPress:
          switch (event.xbutton.button)
          {

          }
          break;
        case ConfigureNotify:
          glViewport(0, 0, event.xconfigure.width,
                     event.xconfigure.height);
        case ClientMessage: {
            if (event.xclient.data.l[0] == (unsigned)info->wmDeleteMessage) {
                info->close = 1;
            }
            break;
        }
      }
    } while(XPending(info->dpy));
}

inline void platform_destroy(PlatformInfo* info) {
    exit(0);
}

inline void platform_swapBuffers(PlatformInfo* info) {
    if (info->doubleBuffer) {
        glXSwapBuffers(info->dpy, info->win);
    } else {
        glFlush();
    }
}

inline bool platform_isCloseRequested(PlatformInfo* info) {
    return info->close == 1;
}


#endif // WIN32WINDOW_H

#pragma once

#ifndef WIN32WINDOW_H
#define WIN32WINDOW_H

#include "PlatformInfo.h"
#include <windows.h>
#include <wingdi.h>
#include <GL/gl.h>
#include <stdio.h>

inline LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

inline void platform_create(const char* title, int width, int height, PlatformInfo* info) {
    int        n, pf;
	WNDCLASSEX 	wc;
	LOGPALETTE* lpPal;
	PIXELFORMATDESCRIPTOR pfd;

	info->hIns = GetModuleHandle(NULL);

    wc.cbSize           = sizeof(WNDCLASSEX);
	wc.style			= CS_OWNDC;
	wc.lpfnWndProc		= WndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= info->hIns;
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);;
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= title;
	wc.hIconSm          = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, "RegisterClassEx() failed", "Error", MB_OK);
		printf("Problem!\n");
		return;
	}

	int nStyle = WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE | WS_CAPTION | WS_MINIMIZEBOX;

	info->hwnd = CreateWindowEx(title, title, nStyle, 100, 100, width, height, NULL, NULL, info->hIns, NULL);
	if (info->hwnd == NULL) {
		MessageBox(NULL, "CreateWindowEx() failed", "Error", MB_OK);
		PostQuitMessage(0);
		printf("Problem!\n");
		return;
	}

	info->hDc = GetDC(info->hwnd);

	memset(&pfd, 0, sizeof(pfd));
    pfd.nSize        = sizeof(pfd);
    pfd.nVersion     = 1;
    pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType   = PFD_TYPE_RGBA;
    pfd.cColorBits   = 32;


    pf = ChoosePixelFormat(info->hDc, &pfd);
	if (pf == 0) {
	MessageBox(NULL, "ChoosePixelFormat() failed:  "
		   "Cannot find a suitable pixel format.", "Error", MB_OK);
		return;
    }

    if (SetPixelFormat(info->hDc, pf, &pfd) == FALSE) {
	MessageBox(NULL, "SetPixelFormat() failed:  "
		   "Cannot set format specified.", "Error", MB_OK);
		return;
    }

    DescribePixelFormat(info->hDc, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

    if (pfd.dwFlags & PFD_NEED_PALETTE ||
	pfd.iPixelType == PFD_TYPE_COLORINDEX) {

	n = 1 << pfd.cColorBits;
	if (n > 256) n = 256;

	lpPal = (LOGPALETTE*)malloc(sizeof(LOGPALETTE) +
				    sizeof(PALETTEENTRY) * n);
	memset(lpPal, 0, sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * n);
	lpPal->palVersion = 0x300;
	lpPal->palNumEntries = n;

	GetSystemPaletteEntries(info->hDc, 0, n, &lpPal->palPalEntry[0]);

	if (pfd.iPixelType == PFD_TYPE_RGBA) {
	    int redMask = (1 << pfd.cRedBits) - 1;
	    int greenMask = (1 << pfd.cGreenBits) - 1;
	    int blueMask = (1 << pfd.cBlueBits) - 1;
	    int i;

	    for (i = 0; i < n; ++i) {
		lpPal->palPalEntry[i].peRed =
		    (((i >> pfd.cRedShift)   & redMask)   * 255) / redMask;
		lpPal->palPalEntry[i].peGreen =
		    (((i >> pfd.cGreenShift) & greenMask) * 255) / greenMask;
		lpPal->palPalEntry[i].peBlue =
		    (((i >> pfd.cBlueShift)  & blueMask)  * 255) / blueMask;
		lpPal->palPalEntry[i].peFlags = 0;
	    }
	} else {
	    lpPal->palPalEntry[0].peRed = 0;
	    lpPal->palPalEntry[0].peGreen = 0;
	    lpPal->palPalEntry[0].peBlue = 0;
	    lpPal->palPalEntry[0].peFlags = PC_NOCOLLAPSE;
	    lpPal->palPalEntry[1].peRed = 255;
	    lpPal->palPalEntry[1].peGreen = 0;
	    lpPal->palPalEntry[1].peBlue = 0;
	    lpPal->palPalEntry[1].peFlags = PC_NOCOLLAPSE;
	    lpPal->palPalEntry[2].peRed = 0;
	    lpPal->palPalEntry[2].peGreen = 255;
	    lpPal->palPalEntry[2].peBlue = 0;
	    lpPal->palPalEntry[2].peFlags = PC_NOCOLLAPSE;
	    lpPal->palPalEntry[3].peRed = 0;
	    lpPal->palPalEntry[3].peGreen = 0;
	    lpPal->palPalEntry[3].peBlue = 255;
	    lpPal->palPalEntry[3].peFlags = PC_NOCOLLAPSE;
	}

	info->hPalette = CreatePalette(lpPal);
	if (info->hPalette) {
	    SelectPalette(info->hDc, info->hPalette, FALSE);
	    RealizePalette(info->hDc);
	}

	free(lpPal);
    }

	info->hRc = wglCreateContext(info->hDc);
	wglMakeCurrent(info->hDc, info->hRc);

	ShowWindow(info->hwnd, SW_SHOW);
	UpdateWindow(info->hwnd);
	SetForegroundWindow(info->hwnd);
	SetFocus(info->hwnd);

	ZeroMemory(&info->msg, sizeof(MSG));
	info->close = 0;
}

inline void platform_poll(PlatformInfo* info) {
    while(PeekMessage(&info->msg, info->hwnd, 0, 0, PM_NOREMOVE)) {
	    if (info->msg.message == WM_QUIT) {
            close = 1;
	    } else {
			TranslateMessage(&info->msg);
			DispatchMessage(&info->msg);
	    }
	}
}

inline void platform_destroy(PlatformInfo* info) {
    wglMakeCurrent(NULL, NULL);
    ReleaseDC(info->hwnd, info->hDc);
    wglDeleteContext(info->hRc);
	UnregisterClass(info->title, info->hIns);
	info->hIns = NULL;
}

inline void platform_swapBuffers(PlatformInfo* info) {
    SwapBuffers(info->hDc);
}

inline bool platform_isCloseRequested(PlatformInfo* info) {
    return info->close == 1;
}

inline LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message) {
		case WM_PAINT: {
			hdc = BeginPaint(hwnd, &ps);
			EndPaint(hwnd, &ps);
			break;
		}

		case WM_CLOSE: {
				PostQuitMessage(0);
				DestroyWindow(hwnd);
			break;
		}

		default: {
			return DefWindowProc(hwnd, message, wParam, lParam);
		}
	}

	return 0;
}


#endif // WIN32WINDOW_H

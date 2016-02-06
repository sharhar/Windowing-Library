#include "Window.h"

Window::Window(char* title, int width, int height) {
    info = new PlatformInfo();
    platform_create(title, width, height, info);
}

void Window::swapBuffers() {
    platform_swapBuffers(info);
}

void Window::poll() {
    platform_poll(info);
}

bool Window::isCloseRequested() {
    return platform_isCloseRequested(info);
}

void Window::destroy() {
    platform_destroy(info);
}

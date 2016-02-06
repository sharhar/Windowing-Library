#include "include/Window.h"
#include <stdio.h>

using namespace std;

int main()
{
    sw::Window window("Hello window", 640, 480);

    while(!window.isCloseRequested()) {
        window.poll();
        glClear(GL_COLOR_BUFFER_BIT);
        glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2i(0,  1);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex2i(-1, -1);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex2i(1, -1);
        glEnd();
        glFlush();
        window.swapBuffers();
    }

    window.destroy();

    return 0;
}

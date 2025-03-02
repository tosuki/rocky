#ifndef WM_H
#define WM_H

#include <X11/Xlib.h>

typedef struct RockyWM {
    Display* dpy;
    int primary_screen;
    Window root;

    Window focused_window;
} RockyWM;

RockyWM* create_rocky_wm();
void rocky_run(RockyWM* wm);

#endif

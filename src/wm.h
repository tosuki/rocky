#ifndef WM_H
#define WM_H

#include <X11/Xlib.h>

#include "window_node.h"

typedef struct RockyWM {
    Display* dpy;
    int primary_screen;
    Window root;

    Window focused_window;
    Windows* windows;
} RockyWM;

RockyWM* create_rocky_wm();
void wm_run(RockyWM* wm);
void rocky_exit(RockyWM* wm);

#endif

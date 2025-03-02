#include <X11/X.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <X11/Xlib.h>

#include "wm.h"

int wm_error_handler(Display* dpy, XErrorEvent* ev) {
    return 0;
}

Status wm_run(RockyWM* wm) {
    //send all events of all childs to us xD
    XSelectInput(wm->dpy, wm->root, SubstructureNotifyMask | SubstructureRedirectMask);

    XEvent xevent;

    //event loop
    for(;;) {
        XNextEvent(wm->dpy, &xevent);

        switch (xevent.type) {
            case ButtonPress:
                puts("A button got pressed");
                break;
            default:
                printf("Event of type %d dispatched\n", xevent.type);
                break;
        }

        XSync(wm->dpy, 0);
    }
    return 0;
}

RockyWM* create_rocky_wm() {
    RockyWM* wm = malloc(sizeof(RockyWM));

    if (wm == NULL) {
        puts("Failed to allocate memory to rocky wm");
        exit(1);
    }

    wm->dpy = XOpenDisplay(NULL);
    
    if (wm->dpy) {
        puts("Failed to open display");
        exit(1);
    }

    XSetErrorHandler(wm_error_handler);
    wm->root = XDefaultRootWindow(wm->dpy);
    wm->primary_screen = DefaultScreen(wm->dpy);

    wm->focused_window = wm->root;

    return wm;
}

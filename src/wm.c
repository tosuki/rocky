#include <X11/X.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <X11/Xlib.h>

#include "wm.h"
#include "mouse.h"

void panic(char* message) {
    puts(message);
    exit(1);
}

int wm_error_handler(Display* dpy, XErrorEvent* ev) {
    printf("An X11 error occured: Code %d\n", ev->error_code);
    return 0;
}

void wm_run(RockyWM* wm) {
    //send all events of all childs to us xD
    XSetErrorHandler(wm_error_handler);
    XSelectInput(wm->dpy, wm->root, SubstructureNotifyMask | SubstructureRedirectMask);
    if (!render_mouse_pointer(wm)) {
        panic("Failed to render the pointer");
    }

    XEvent xevent;

    //event loop
    for(;;) {
        XNextEvent(wm->dpy, &xevent);

        switch (xevent.type) {
            case ButtonPress:
                handle_button_press(wm, xevent.xbutton);
                break;
            default:
                printf("Event of type %d dispatched\n", xevent.type);
                break;
        }

        XSync(wm->dpy, 0);
    }
}

RockyWM* create_rocky_wm() {
    RockyWM* wm = malloc(sizeof(RockyWM));

    if (wm == NULL) {
        panic("Failed to allocate memory to rocky wm");
    }

    wm->dpy = XOpenDisplay(NULL);
    
    if (wm->dpy == NULL) {
        panic("Failed to open display");
    }

    wm->root = XDefaultRootWindow(wm->dpy);
    wm->primary_screen = DefaultScreen(wm->dpy);

    wm->focused_window = wm->root;

    return wm;
}

void rocky_exit(RockyWM *wm) {
    if (!XCloseDisplay(wm->dpy)) {
        panic("Failed to close the display, exitting");
    }
}

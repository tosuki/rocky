#include <X11/X.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <X11/Xlib.h>

#include "wm.h"
#include "mouse.h"
#include "window_node.h"
#include "window.h"
#include "keyboard.h"
#include "logger.h"

void panic(char* message) {
    puts(message);
    exit(1);
}

int wm_error_handler(Display* dpy, XErrorEvent* ev) {
    printf("Xorg error\nCode: %d\nMinor Code: %d\nResource Id: %li\nRequestCode: %d\n",
            ev->error_code,
            ev->minor_code,
            ev->resourceid,
            ev->request_code
            ); 
    return 0;
}

void wm_run(RockyWM* wm) {
    //send all events of all childs to us xD
    XSetErrorHandler(wm_error_handler);
    XSelectInput(wm->dpy, wm->root, SubstructureNotifyMask | SubstructureRedirectMask);
    if (!render_mouse_pointer(wm)) {
        panic("Failed to render the pointer");
    }

    grab_keyboard_keys(wm);
    XSync(wm->dpy, False);

    XEvent xevent;

    //event loop
    for(;;) {
        XNextEvent(wm->dpy, &xevent);

        switch (xevent.type) {
            case KeyPress:
                handle_keydown(wm, xevent.xkey);
                break;
            case MapRequest:
                logger_info("The window %li requested to be mapped", xevent.xmaprequest.window);
                render_window(wm, xevent.xmaprequest);
                break;

            case ButtonPress:
                handle_button_press(wm, xevent.xbutton);
                break;
        }

        XSync(wm->dpy, 0);
    }
}

//rocky graphical context
RockyGC* create_rocky_gc(RockyWM* wm) {
    RockyGC* gc = malloc(sizeof(RockyGC));

    if (gc == NULL) {
        panic("Failed to allocate memory to rocky gc");
    }

    gc->default_colormap = DefaultColormap(wm->dpy, wm->primary_screen);

    return gc;
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
    wm->windows = create_window_collection();

    wm->focused_window = wm->root;

    wm->gc = create_rocky_gc(wm);

    return wm;
}


void rocky_exit(RockyWM *wm) {
    if (!XCloseDisplay(wm->dpy)) {
        panic("Failed to close the display, exitting");
    }

    free_window_collection(wm->windows);
    free(wm->gc);
    free(wm);
}

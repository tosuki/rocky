#include <X11/X.h>
#include <X11/Xlib.h>
#include <stdio.h>

#include "window.h"
#include "wm.h"

Status frame_window(RockyWM *wm, Window window) {
    XWindowAttributes attributes;
    
    if (!XGetWindowAttributes(wm->dpy, window, &attributes)) {
        puts("Failed to get the attributes of the window");

        return 0;
    }

    Window frame_xid = XCreateSimpleWindow(
        wm->dpy,
        wm->root,
        attributes.x,
        attributes.y,
        attributes.width,
        attributes.height,
        WINDOW_BORDER_WIDTH,
        WINDOW_UNFOCUSED_BORDER_COLOR,
        WINDOW_BACKGROUND_COLOR
    );

    XReparentWindow(wm->dpy, window, frame_xid, 0, 0);
    XAddToSaveSet(wm->dpy, window);
    window_collection_add(wm->windows, frame_xid, window);
    XMapWindow(wm->dpy, frame_xid);
    XMapWindow(wm->dpy, window);
}

Status render_window(RockyWM *wm, XMapRequestEvent xevent) {
    frame_window(wm, xevent.window);
    return 1;
}
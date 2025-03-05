#include <X11/X.h>
#include <X11/Xlib.h>
#include <stdio.h>

#include "window.h"
#include "mouse.h"
#include "window_node.h"
#include "wm.h"

Status move_window_y(RockyWM* wm, Window window, int offset, XWindowAttributes* attributes) {
    if (!XMoveResizeWindow(wm->dpy, window, attributes->x, attributes->y + offset, attributes->width, attributes->height)) {
        printf("Failed to move the window %li to %d", window, attributes->y + offset);
        return 0;
    }
    return 1;
}

Status move_window_x(RockyWM* wm, Window window, int offset, XWindowAttributes* attributes) {
    if (!XMoveResizeWindow(wm->dpy, window, attributes->x + offset, attributes->y, attributes->width, attributes->height)) {
        printf("Failed to move the window %li to %d\n", window, attributes->x + offset);
        return 0;
    }
    return 1;
}

Status move_window(RockyWM* wm, Window window, int keycode) {
    WindowNode* node = window_collection_get(wm->windows, window);

    if (node == NULL) {
        printf("Attempt to move a non-client window");
        return 0;
    }

    XWindowAttributes attributes;

    if (!XGetWindowAttributes(wm->dpy, node->frame, &attributes)) {
        printf("Failed to get the attributes of the frame window %li\n", window);
        return 0;
    }

    switch (keycode) {
        case 111: //Up
            move_window_y(wm, node->frame, -25, &attributes);
            break;
        case 116://Down
            move_window_y(wm, node->frame, 25, &attributes);
            break;
        case 113://left
            move_window_x(wm, node->frame, -25, &attributes);
            break;
        case 114://right
            move_window_x(wm, node->frame, 25, &attributes);
            break;
    }

    return 1;
}

Status parse_color(RockyWM *wm, char* color_code, XColor* xcolor) {
    if (!XParseColor(wm->dpy, wm->gc->default_colormap,  color_code, xcolor)) {
        printf("Failed to allocate the color %s\n", color_code);
        return 0;
    };

    return XAllocColor(wm->dpy, wm->gc->default_colormap, xcolor);
}

Status set_window_border(RockyWM *wm, Window window, char* border_color, int shouldFlush) {
    WindowNode* node = window_collection_get(wm->windows, window);

    if (node == NULL) {
        printf("Attempt to change the border of a non-client window %li\n", window);
        return 0;
    }

    XColor border_xcolor;

    if (!parse_color(wm, border_color, &border_xcolor)) {
        printf("Failed to parse the color %s\n", border_color);
        return 0;
    }

    if (!XSetWindowBorder(wm->dpy, node->frame, border_xcolor.pixel)) {
        printf("Failed to set the bborder color of window %li to %s\n", window, border_color);
        return 0;
    }

    if (shouldFlush) {
        XFlush(wm->dpy);
    }

    return 1;
}


Status focus_window(RockyWM *wm, Window window) {
    if (wm->focused_window != wm->root && !set_window_border(wm, wm->focused_window, "#181818", False)) {
        return 0;
    }

    wm->focused_window = window;
    if (!set_window_border(wm, window, "#484848", True)) {
        return 0;
    };
    
    if (!XSetInputFocus(wm->dpy, window, wm->root, CurrentTime)) {
        printf("Failed to set the input focus to window %li\n", window);
        return 0;
    }

    return 1;
}

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

    return 1;
}

Status render_window(RockyWM *wm, XMapRequestEvent xevent) {
    if (!frame_window(wm, xevent.window)) {
        printf("Failed to render window %li\n", xevent.window);
    }

    grab_button(wm, Button1, xevent.window);

    return 1;
}

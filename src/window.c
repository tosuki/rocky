#include <X11/X.h>
#include <X11/Xlib.h>
#include <stdio.h>

#include "window.h"
#include "mouse.h"
#include "window_node.h"
#include "logger.h"
#include "wm.h"

Status unrender_window(RockyWM* wm, Window window) {
    WindowNode* node = window_collection_get(wm->windows, window);
    
    if (node == NULL) {
        logger_error("Attempt to kill a non-client window %li", window);
        return 0;
    }

    XRemoveFromSaveSet(wm->dpy, window);
    XUnmapWindow(wm->dpy, window);
    XDestroyWindow(wm->dpy, node->frame);
    window_collection_delete(wm->windows, window);

    return 1;
}

Status send_kill_event(RockyWM* wm, Window window) {
    //kill window gracefully
    Atom wm_delete_atom = XInternAtom(wm->dpy, "WM_DELETE_WINDOW", True);
    
    if (!wm_delete_atom) {
        return 0;
    }
    
    XEvent event = {0};

    event.xclient.type = ClientMessage;
    event.xclient.message_type = XInternAtom(wm->dpy, "WM_PROTOCOLS", True);
    event.xclient.format = 32;
    event.xclient.data.l[0] = wm_delete_atom;
    event.xclient.window = window;

    if (!XSendEvent(wm->dpy, window, False, NoEventMask, &event)) {
        logger_error("Failed to send a WM_DELETE_WINDOW atom to the window %li", window);
        return 0;
    }
    
    XFlush(wm->dpy);  
    return 1;
}

Status kill_window(RockyWM* wm, Window window) {
    if (wm->focused_window == wm->root) {
        logger_error("Attempt to kill the root window");
        return 0;
    }
    
    if (!unrender_window(wm, window)) {
        logger_error("Failed to unrender the window %li", window);
        return 0;
    }

    if (!send_kill_event(wm, window)) {
        logger_warn("Due to failure on killing window in a graceful way, kicking it out in an aggressive way");
        XKillClient(wm->dpy, window);
    }

    wm->focused_window = wm->root;

    return 1;
}

Status move_window_y(RockyWM* wm, Window window, int offset, XWindowAttributes* attributes) {
    if (!XMoveResizeWindow(wm->dpy, window, attributes->x, attributes->y + offset, attributes->width, attributes->height)) {
        logger_error("Failed to move the window %li to %d", window, attributes->y + offset);
        return 0;
    }
    return 1;
}

Status move_window_x(RockyWM* wm, Window window, int offset, XWindowAttributes* attributes) {
    if (!XMoveResizeWindow(wm->dpy, window, attributes->x + offset, attributes->y, attributes->width, attributes->height)) {
        logger_error("Failed to move the window %li to %d", window, attributes->x + offset);
        return 0;
    }
    return 1;
}

Status move_window(RockyWM* wm, Window window, int keycode) {
    if (wm->root == window) {
        return 0;
    }

    WindowNode* node = window_collection_get(wm->windows, window);

    if (node == NULL) {
        logger_error("Attempt to move a non-client window");
        return 0;
    }

    XWindowAttributes attributes;

    if (!XGetWindowAttributes(wm->dpy, node->frame, &attributes)) {
        logger_error("Failed to get the attributes of the frame window %li", window);
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
        logger_error("Failed to allocate the color %s", color_code);
        return 0;
    };

    return XAllocColor(wm->dpy, wm->gc->default_colormap, xcolor);
}

Status set_window_border(RockyWM *wm, Window window, char* border_color, int shouldFlush) {
    WindowNode* node = window_collection_get(wm->windows, window);

    if (node == NULL) {
        logger_error("Attempt to change the border of a non-client window %li\n", window);
        return 0;
    }

    XColor border_xcolor;

    if (!parse_color(wm, border_color, &border_xcolor)) {
        logger_error("Failed to parse the color %s", border_color);
        return 0;
    }

    if (!XSetWindowBorder(wm->dpy, node->frame, border_xcolor.pixel)) {
        logger_error("Failed to set the bborder color of window %li to %s", window, border_color);
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
   
    if (!XSetInputFocus(wm->dpy, window, RevertToPointerRoot, CurrentTime)) {
        logger_error("Failed to change the input focus to the window %li", window);
        return 0;
    }

    return 1;
}

Status focus_next(RockyWM* wm) {
    if (wm->windows->size < 2) {
        return 1;
    }

    if (wm->focused_window == wm->root) {
        return focus_window(wm, wm->windows->nodes->content);
    }

    WindowNode* node = window_collection_get(wm->windows, wm->focused_window);

    if (node == NULL) {
        logger_error("Failed to get the focused window node, received NULL");
        return 0;
    }

    if (node->next != NULL) {
        return focus_window(wm, node->next->content);
    }

    return focus_window(wm, wm->windows->nodes->content);
}

Status frame_window(RockyWM *wm, Window window) {
    XWindowAttributes attributes;
    
    if (!XGetWindowAttributes(wm->dpy, window, &attributes)) {
        logger_error("Failed to get the attributes of the window");

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
        logger_error("Failed to render window %li", xevent.window);
    }

    grab_button(wm, Button1, xevent.window);
    focus_window(wm, xevent.window);
    
    return 1;
}

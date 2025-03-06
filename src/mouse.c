#include "mouse.h"
#include "window.h"
#include "wm.h"
#include "logger.h"

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <stdio.h>

void grab_button(RockyWM* wm, unsigned int button, Window window) {
    XGrabButton(
        wm->dpy, 
        button,
        0,
        window,
        False,
        ButtonPressMask,
        GrabModeSync,
        GrabModeAsync,
        None,
        None
    );
}

Status render_mouse_pointer(RockyWM *wm) {
    Cursor pointer = XCreateFontCursor(wm->dpy, XC_left_ptr);

    if (pointer == None) {
        printf("Failed to create the cursor");
        return 1;
    }
    

    if (!XDefineCursor(wm->dpy, wm->root, pointer)) {
        return 0;
    }
 
    return 1;
}

void handle_button_release(RockyWM* wm, XButtonEvent xevent) {
    puts("Button released");
}

void handle_button_press(RockyWM *wm, XButtonEvent xevent) {
    logger_info("Button 1 pressed on window %li", xevent.window);
    logger_warn("Focused window: %li", wm->focused_window);

    if (wm->focused_window != xevent.window) {
        focus_window(wm, xevent.window);
    }

    XAllowEvents(wm->dpy, ReplayPointer, CurrentTime);
}

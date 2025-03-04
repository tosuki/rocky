#include "mouse.h"
#include "wm.h"

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
 
    grab_button(wm, Button1, wm->root);

    return 1;
}

void handle_button_release(RockyWM* wm, XButtonEvent xevent) {
    puts("Button released");
}

void handle_button_press(RockyWM *wm, XButtonEvent xevent) {
    printf("Button 1 pressed on window %li\n", xevent.window);
    XAllowEvents(wm->dpy, ReplayPointer, CurrentTime);
}

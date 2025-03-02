#include "mouse.h"
#include "wm.h"

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/cursorfont.h>

Status render_mouse_pointer(RockyWM *wm) {
    Cursor pointer = XCreateFontCursor(wm->dpy, XC_left_ptr);
    
    return XDefineCursor(wm->dpy, wm->root, pointer);
}

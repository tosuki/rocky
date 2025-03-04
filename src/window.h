#ifndef WINDOW_H
#define WINDOW_H

#define WINDOW_BORDER_WIDTH 2
#define WINDOW_BACKGROUND_COLOR 0x000000
#define WINDOW_UNFOCUSED_BORDER_COLOR 0xffffff
#define WINDOW_FOCUSED_BORDER_COLOR 0xffffff

#include "wm.h"
#include <X11/Xlib.h>

Status focus_window(RockyWM* wm, Window window);
Status render_window(RockyWM* wm, XMapRequestEvent xevent);

#endif

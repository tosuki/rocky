#ifndef WINDOW_H
#define WINDOW_H

#include "wm.h"
#include <X11/Xlib.h>

Status render_window(RockyWM* wm, XMapRequestEvent xevent);

#endif

#ifndef MOUSE_H
#define MOUSE_H
#include "wm.h"
#include <X11/Xlib.h>

Status render_mouse_pointer(RockyWM* wm);
void grab_button(RockyWM* wm, unsigned int button, Window window);

void handle_button_press(RockyWM* wm, XButtonEvent xevent);
void handle_button_release(RockyWM* wm, XButtonEvent xevent);

#endif

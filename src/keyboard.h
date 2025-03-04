#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <X11/Xlib.h>
#include "wm.h"

void grab_keyboard_keys(RockyWM* wm);
void handle_keydown(RockyWM* wm, XKeyPressedEvent xevent);

#endif

#include <stdio.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>

#include "keyboard.h"
#include "wm.h"

void grab_key(RockyWM* wm, KeySym key_sym, unsigned long mask) {
    KeyCode key_code = XKeysymToKeycode(wm->dpy, key_sym);

    XGrabKey(wm->dpy, key_code, mask, wm->root, KeyPressMask, GrabModeAsync, GrabModeAsync);
}

void handle_keydown(RockyWM* wm, XKeyPressedEvent xevent) {
    printf("Key %d pressed\n", xevent.keycode);
}

void grab_keyboard_keys(RockyWM* wm) {
    grab_key(wm, XK_Left, Mod1Mask);
    grab_key(wm, XK_Up, Mod1Mask);
    grab_key(wm, XK_Right, Mod1Mask);
    grab_key(wm, XK_Down, Mod1Mask);
}

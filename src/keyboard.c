#include <stdio.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>

#include "keyboard.h"
#include "window.h"
#include "wm.h"

void grab_key(RockyWM* wm, KeySym key_sym, unsigned long mask) {
    KeyCode key_code = XKeysymToKeycode(wm->dpy, key_sym);

    XGrabKey(wm->dpy, key_code, mask, wm->root, KeyPressMask, GrabModeAsync, GrabModeAsync);
}

void handle_modshift_keybinds(RockyWM* wm, XKeyPressedEvent* xevent) {
    switch (xevent->keycode) {
        case 26: //Alt + Shift + e -> exit
            puts("Attempt to exit");
            break;
        case 24://Alt + shift + q -> kill window
            kill_window(wm, wm->focused_window);
            break;
    }
}

void handle_mod_keybinds(RockyWM* wm, XKeyPressedEvent* xevent) {
    switch (xevent->keycode) {
        case 111://up
        case 114: //right
        case 116: //downn
        case 113://left
            move_window(wm, wm->focused_window, xevent->keycode);
            break;
    }
}

void handle_keydown(RockyWM* wm, XKeyPressedEvent xevent) {
    // printf("Key pressed %d\n", xevent.keycode);

    switch (xevent.state) {
        case Mod1Mask | ShiftMask:
            handle_modshift_keybinds(wm, &xevent);
            break;
        case Mod1Mask:
            handle_mod_keybinds(wm, &xevent);
            break;
    }
}

void grab_keyboard_keys(RockyWM* wm) {
    grab_key(wm, XK_e, Mod1Mask | ShiftMask);
    grab_key(wm, XK_Q, Mod1Mask | ShiftMask);
    grab_key(wm, XK_Left, Mod1Mask);
    grab_key(wm, XK_Up, Mod1Mask);
    grab_key(wm, XK_Right, Mod1Mask);
    grab_key(wm, XK_Down, Mod1Mask);
}

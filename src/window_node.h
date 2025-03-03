#ifndef WINDOW_NODE_H
#define WINDOW_NODE_H
#include <X11/Xlib.h>

typedef struct WindowNode {
    Window frame;
    Window content;

    WindowNode* next;
} WindowNode;

WindowNode* create_windownode(Window frame, Window content, WindowNode* next);
WindowNode* get_windownode(WindowNode* tail, Window content_xid);

#endif
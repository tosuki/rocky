#ifndef WINDOW_NODE_H
#define WINDOW_NODE_H
#include <X11/Xlib.h>

typedef struct WindowNode {
    Window frame;
    Window content;

    struct WindowNode* next;
} WindowNode;

typedef struct Windows {
    WindowNode *nodes;
    int size;
} Windows;

Windows* create_window_collection();
WindowNode* window_collection_get(Windows* collection, Window content_xid);
WindowNode* window_collection_add(Windows* collection, Window frame_xid, Window content_xid);
WindowNode* create_windownode(Window frame, Window content, WindowNode* next);
WindowNode* get_windownode(WindowNode* tail, Window content_xid);

#endif
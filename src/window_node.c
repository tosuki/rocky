#include <X11/Xlib.h>
#include <malloc.h>
#include <stdlib.h>

#include "window_node.h"

WindowNode* create_windownode(Window frame, Window content, WindowNode* next) {
    WindowNode* node = malloc(sizeof(WindowNode));

    if (node == NULL) {
        return node;
    }

    node->content = content;
    node->frame = frame;
    node->next = next;

    return node;
}

WindowNode* get_windownode(WindowNode* tail, Window content_xid) {
    return NULL;
}

#include <X11/Xlib.h>
#include <malloc.h>
#include <stdlib.h>

#include "window_node.h"

Windows* create_window_collection() {
    Windows* collection = malloc(sizeof(Windows));

    if (collection == NULL) {
        return NULL;
    }

    collection->nodes = NULL;
    collection->size = 0;

    return collection;
}

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
    if (tail == NULL) {
        return NULL;
    }

    if (tail->content == content_xid) {
        return tail;
    }

    return get_windownode(tail->next, content_xid);
}

WindowNode* window_collection_get(Windows* windows, Window content_xid) {
    return get_windownode(windows->nodes, content_xid);
}

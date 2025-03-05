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

void free_window_node(WindowNode* node) {
    if (node->next != NULL) {
        free_window_node(node->next);
    }

    free(node);
}

void free_window_collection(Windows *windows) {
    free_window_node(windows->nodes);
    free(windows);
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

WindowNode* window_collection_add(Windows* collection, Window frame_xid, Window content_xid) {
    if (collection->nodes == NULL) {
        collection->nodes = create_windownode(frame_xid, content_xid, NULL);
    } else {
        collection->nodes = create_windownode(frame_xid, content_xid, collection->nodes);
    }
    
    collection->size += 1;
    printf("New window added, xid: %li\n", collection->nodes->content);

    return collection->nodes;
}
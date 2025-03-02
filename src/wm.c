#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <X11/Xlib.h>
#include "wm.h"

RockyWM* create_rocky_wm() {
    RockyWM* wm = malloc(sizeof(RockyWM));

    if (wm == NULL) {
        puts("Failed to allocate memory to rocky wm");
        exit(1);
    }
    
    

    return wm;
}

#include "wm.h"

int main() {
    RockyWM* wm = create_rocky_wm();

    wm_run(wm);

    rocky_exit(wm);
    return 0;
}

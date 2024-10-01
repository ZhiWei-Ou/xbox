#include "xlog.h"
#include <stdio.h>

int main() {
    xlog_init(1, xlog_sink_console_st());

    xlog_print_all_sink(printf);

    XLOG_DEBUG("Hello World");
    XLOG_INFO("Hello World");

    XLOG_INFOF("I'm %s", "Ouzw");
    XLOG_ERRORF("I'm %s", "Ouzw");

    return 0;
}

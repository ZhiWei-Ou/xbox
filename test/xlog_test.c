#include "xlog.h"
#include <stdio.h>

int main() {
    xlog_init(2,
            xlog_sink_console_st(XLOG_LEVEL_INFO),
            xlog_sink_file_st(XLOG_LEVEL_TRACE,"xlog_test.log"));

    xlog_print_all_sink(printf);

    XLOG_DEBUG("Hello World");
    XLOG_INFO("Hello World");

    XLOG_TRACEF("I'm %s", "TRACEF");
    XLOG_DEBUGF("I'm %s", "DEBUGF");
    XLOG_INFOF("I'm %s", "INFOF");
    XLOG_WARNF("I'm %s", "WARNF");
    XLOG_ERRORF("I'm %s", "ERRORF");

    return 0;
}

#include "xlog.h"
#include "xtime.h"
#include <stdio.h>
#include <unistd.h>

void test_xlog_init() {
    XLOG_INFO("Hello Welcom xlog_test");
    XLOG_INFOF("Current Use Default Logger[%s]", "ConsoleSinker");
    xlog_print_all_sink(printf);
    XLOG_WARN("Next Step [call xlog_init] Will Use User Options Loggers");

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
}

/*
 * benchmark
 * 100000 entries
 *
 * - SingleThread
 *  1. ConsoleSinker
 *  2. FileSinker
 *  3. Console && File
 * - MultiThread
 */
#define BENCH_NUM 100000

void bench_1(xtimestamp *cost) {
    xtimestamp start = xtime_now(XTIME_Millisecond);

    for (int i = 0; i < BENCH_NUM; ++i) {
        XLOG_INFOF("#%d Hello World", i);
    }

    xtimestamp end = xtime_now(XTIME_Millisecond);

    *cost = end - start;
}

void bench_3(xtimestamp *cost) {

    xlog_init(2,
            xlog_sink_console_st(XLOG_LEVEL_INFO),
            xlog_sink_file_st(XLOG_LEVEL_TRACE,"xlog_test.log"));
    xlog_print_all_sink(printf);

    xtimestamp start = xtime_now(XTIME_Millisecond);
    for (int i = 0; i < BENCH_NUM; ++i) {
        XLOG_INFOF("#%d Hello World", i);
    }

    xtimestamp end = xtime_now(XTIME_Millisecond);

    *cost = end - start;
}

void test_udp_log() {

    xlog_init(2,
            xlog_sink_console_st(XLOG_LEVEL_INFO),
            xlog_sink_udp_st(XLOG_LEVEL_INFO, "127.0.0.1", 12223));
    xlog_print_all_sink(printf);

    for (int i = 0; i < 10; ++i) {
        XLOG_INFOF("#%d Hello", i);
        sleep(1);
    }
}

int main() {
    /*xtimestamp bench_1_cost, bench_3_cost;*/
    /**/
    /*bench_1(&bench_1_cost);*/
    /**/
    /*bench_3(&bench_3_cost);*/
    /**/
    /*printf("bench_1_cost: %ldms\n", bench_1_cost);*/
    /*printf("bench_3_cost: %ldms\n", bench_3_cost);*/

    test_udp_log();
    return 0;
}

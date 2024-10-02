/*
 * © 2024 [ZhiWei-Ou]. All rights reserved.
 * @file xtime.c
 * @author Ouzw
 * @mail ouzw.mail@gmail.com
 * @date 2024-10-01 23:54:40

 * @brief 
 */

#include "xtime.h"
#include "xdef.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

static xt_cstring xtime_format_table[] = {
    [XTIME_Layout] = "%d/%m %H:%M:%S%p '%y %z",
    [XTIME_ANSIC] = "%a %b _%-d %H:%M:%S %Y",
    [XTIME_UnixDate] = "%a %b _%-d %H:%M:%S %Z %Y",
    [XTIME_RubyDate] = "%a %b %d %H:%M:%S %z %Y",
    [XTIME_RFC822] = "%d %b %y %M:%S %Z",
    [XTIME_RFC822Z] = "%d %b %y %M:%S %z",
    [XTIME_RFC850] = "%A, %d-%b-%y %H:%M:%S %Z",
    [XTIME_RFC1123] = "%a, %d %b %Y %H:%M:%S %Z",
    [XTIME_RFC1123Z] = "%a, %d %b %Y %H:%M:%S %z",
    [XTIME_RFC3339] = "%Y-%m-%dT%H:%M:%SZxx:xx",
    [XTIME_RFC3339Nano] = "%Y-%m-%dT%H:%M:%S%Zxx:xx",
    [XTIME_Kitchen] = "%M:%S%p",
    [XTIME_Stamp] = "%b %_d %H:%M:%S",
    [XTIME_StampMilli] = "%b %_d %H:%M:%S",
    [XTIME_StampMicro] = "%b %_d %H:%M:%S",
    [XTIME_StampNano] = "%b %_d %H:%M:%S",
    [XTIME_DateTime] = "%Y-%m-%d %H:%M:%S",
    [XTIME_DateOnly] = "%Y-%m-%d",
    [XTIME_TimeOnly] = "%H:%M:%S",
};

xt_string xtime_now_legible(xtime_fmt fmt)
{
    xt_string out = (xt_string)xt_malloc(XTIME_LEGIBLE_MAX_LEN);
    if (!out) {
        return NULL;
    }

    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(out, XTIME_LEGIBLE_MAX_LEN, xtime_format_table[fmt], timeinfo);

    return out;
}

xt_string xtime_now_static_legible(xtime_fmt fmt, xt_string buf, xt_u8 buf_size)
{
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buf, buf_size, xtime_format_table[fmt], timeinfo);

    return buf;
}

xtimestamp xtime_now(xtime_unit unit)
{
    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts) == 0) {

        switch (unit) {
            case XTIME_Nanosecond:
                return ts.tv_sec * 1000000000 + ts.tv_nsec;
            case XTIME_Microsecond:
                return ts.tv_sec * 1000000 + ts.tv_nsec / 1000;
            case XTIME_Millisecond:
                return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
            case XTIME_Second:
                return ts.tv_sec;
            case XTIME_Minute:
                return ts.tv_sec / 60;
            case XTIME_Hour:
                return ts.tv_sec / 3600;
            default:
                return 0;
        }
    } else {
        return 0;
    }
}

xt_string xtime_timestamp_to_string(xtimestamp ts, xt_string buf, xt_u8 buf_size)
{
    snprintf(buf, buf_size, "%ld", ts);
    return buf;
}

xt_string xtime_timestamp_to_time(xtimestamp ts, xtime_fmt fmt, xt_string buf, xt_u8 buf_size)
{
    time_t now = time(NULL);
    time_t t = (time_t)ts;
    while (t > now) {
        t = t / 1000;
    }

    struct tm *local_time = localtime(&t);

    strftime(buf, buf_size, xtime_format_table[fmt], local_time);
    return buf;
}

xtimestamp xtime_sub(xtimestamp ago, xtimestamp now)
{
    return now - ago;
}

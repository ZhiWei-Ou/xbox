#include "xtime.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {

    /*XTIME_Layout, // "01/02 03:04:05PM '06 -0700" // The reference time, in numerical order.*/
    char *t = xtime_now_legible(XTIME_Layout);
    printf("XTIME_Layout:\n\t01/02 03:04:05PM '06 -0700\n\t%s\n", t);
    xt_free(t);

    /*XTIME_ANSIC, // "Mon Jan _2 15:04:05 2006"*/
    t = xtime_now_legible(XTIME_ANSIC);
    printf("XTIME_ANSIC:\n\tMon Jan _2 15:04:05 2006\n\t%s\n", t);
    xt_free(t);

    /*XTIME_UnixDate, // "Mon Jan _2 15:04:05 MST 2006"*/
    t = xtime_now_legible(XTIME_UnixDate);
    printf("XTIME_UnixDate:\n\tMon Jan _2 15:04:05 MST 2006\n\t%s\n", t);
    xt_free(t);

    /*XTIME_RubyDate, // "Mon Jan 02 15:04:05 -0700 2006"*/
    t = xtime_now_legible(XTIME_RubyDate);
    printf("XTIME_RubyDate:\n\tMon Jan 02 15:04:05 -0700 2006\n\t%s\n", t);
    xt_free(t);

    /*XTIME_RFC822, // "02 Jan 06 15:04 MST"*/
    t = xtime_now_legible(XTIME_RFC822);
    printf("XTIME_RFC822:\n\t02 Jan 06 15:04 MST\n\t%s\n", t);
    xt_free(t);

    /*XTIME_RFC822Z, // "02 Jan 06 15:04 -0700" // RFC822 with numeric zone*/
    t = xtime_now_legible(XTIME_RFC822Z);
    printf("XTIME_RFC822Z:\n\t02 Jan 06 15:04 -0700\n\t%s\n", t);
    xt_free(t);

    /*XTIME_RFC850, // "Monday, 02-Jan-06 15:04:05 MST"*/
    t = xtime_now_legible(XTIME_RFC850);
    printf("XTIME_RFC850:\n\tMonday, 02-Jan-06 15:04:05 MST\n\t%s\n", t);
    xt_free(t);

    /*XTIME_RFC1123, // "Mon, 02 Jan 2006 15:04:05 MST"*/
    t = xtime_now_legible(XTIME_RFC1123);
    printf("XTIME_RFC1123:\n\tMon, 02 Jan 2006 15:04:05 MST\n\t%s\n", t);
    xt_free(t);

    /*XTIME_RFC1123Z, // "Mon, 02 Jan 2006 15:04:05 -0700" // RFC1123 with numeric zone*/
    t = xtime_now_legible(XTIME_RFC1123Z);
    printf("XTIME_RFC1123Z:\n\tMon, 02 Jan 2006 15:04:05 -0700\n\t%s\n", t);
    xt_free(t);

    /*XTIME_RFC3339, // "2006-01-02T15:04:05Z07:00"*/
    t = xtime_now_legible(XTIME_RFC3339);
    printf("XTIME_RFC3339:\n\t2006-01-02T15:04:05Z07:00\n\t%s\n", t);
    xt_free(t);

    /*XTIME_RFC3339Nano, // "2006-01-02T15:04:05.999999999Z07:00"*/
    t = xtime_now_legible(XTIME_RFC3339Nano);
    printf("XTIME_RFC3339Nano:\n\t2006-01-02T15:04:05.999999999Z07:00\n\t%s\n", t);
    xt_free(t);

    /*XTIME_Kitchen, // "3:04PM"*/
    t = xtime_now_legible(XTIME_Kitchen);
    printf("XTIME_Kitchen:\n\t3:04PM\n\t%s\n", t);
    xt_free(t);

    /*/* Handy time stamps. */
    t = xtime_now_legible(XTIME_Stamp);
    printf("XTIME_Stamp:\n\tJan _2 15:04:05\n\t%s\n", t);
    xt_free(t);
    
    /*XTIME_Stamp, // "Jan _2 15:04:05"*/
    t = xtime_now_legible(XTIME_Stamp);
    printf("XTIME_Stamp:\n\tJan _2 15:04:05\n\t%s\n", t);
    xt_free(t);

    /*XTIME_StampMilli, // "Jan _2 15:04:05.000"*/
    t = xtime_now_legible(XTIME_StampMilli);
    printf("XTIME_StampMilli:\n\tJan _2 15:04:05.000\n\t%s\n", t);
    xt_free(t);

    /*XTIME_StampMicro, // "Jan _2 15:04:05.000000"*/
    t = xtime_now_legible(XTIME_StampMicro);
    printf("XTIME_StampMicro:\n\tJan _2 15:04:05.000000\n\t%s\n", t);
    xt_free(t);

    /*XTIME_StampNano, // "Jan _2 15:04:05.000000000"*/
    t = xtime_now_legible(XTIME_StampNano);
    printf("XTIME_StampNano:\n\tJan _2 15:04:05.000000000\n\t%s\n", t);
    xt_free(t);

    /*XTIME_DateTime, // "2006-01-02 15:04:05"*/
    t = xtime_now_legible(XTIME_DateTime);
    printf("XTIME_DateTime:\n\t2006-01-02 15:04:05\n\t%s\n", t);
    xt_free(t);

    /*XTIME_DateOnly, // "2006-01-02"*/
    t = xtime_now_legible(XTIME_DateOnly);
    printf("XTIME_DateOnly:\n\t2006-01-02\n\t%s\n", t);
    xt_free(t);

    /*XTIME_TimeOnly // "15:04:05"*/
    t = xtime_now_legible(XTIME_TimeOnly);
    printf("XTIME_TimeOnly:\n\t15:04:05\n\t%s\n", t);
    xt_free(t);


    /*XTIME_Nanosecond,*/
    /*XTIME_Microsecond, // 1000 * XTIME_Nanosecond,*/
    /*XTIME_Millisecond, // 1000 * XTIME_Microsecond,*/
    /*XTIME_Second, // 1000 * XTIME_Millisecond,*/
    /*XTIME_Minute, // 60 * XTIME_Second,*/
    /*XTIME_Hour // 60 * XTIME_Minute*/
    xtimestamp nanosec = xtime_now(XTIME_Nanosecond);
    xtimestamp microsec = xtime_now(XTIME_Microsecond);
    xtimestamp millisec = xtime_now(XTIME_Millisecond);
    xtimestamp sec = xtime_now(XTIME_Second);
    xtimestamp min = xtime_now(XTIME_Minute);
    xtimestamp hour = xtime_now(XTIME_Hour);
    printf("TimeStamp:\n\tNanosecond: %ld\n\tMicrosecond: %ld\n\tMillisecond: %ld\n\tSecond: %ld\n\tMinute: %ld\n\tHour: %ld\n", nanosec, microsec, millisec, sec, min, hour);

    xtimestamp ago = TIME_MILLISECOND();
    sleep(1);
    xtimestamp now = TIME_MILLISECOND();
    xtimestamp sub = xtime_sub(now, ago);

    printf("TimeStamp:\n\tFrom: %ld\n\tTo: %ld\n\tSub: %ld\n", ago, now, sub);

    char buf[128];
    // 2024-10-02 14:21:16
    xtime_timestamp_to_time(1727850076809606000, XTIME_DateTime, buf, sizeof(buf));
    printf("1727850076809606000 to format[datetime]: %s\n", buf);
    // Wed Oct _2 14:21:16 CST 2024
    xtime_timestamp_to_time(1727850076809606000, XTIME_UnixDate, buf, sizeof(buf));
    printf("1727850076809606000 to format[unixdate]: %s\n", buf);

    return 0;
}

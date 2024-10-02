/*
 * © 2024 [ZhiWei-Ou]. All rights reserved.
 * @file xtime.h
 * @author Ouzw
 * @mail ouzw.mail@gmail.com
 * @date 2024-10-01 23:54:32
 *
 * @brief 
 */
#ifndef __XTIME_H__
#define __XTIME_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "xdef.h"

#define DATE_TIME() XTIME_DateTime(XTIME_DateTime)

#define TIME_NANOSECOND() xtime_now(XTIME_Nanosecond)
#define TIME_MICROSECOND() xtime_now(XTIME_Microsecond)
#define TIME_MILLISECOND() xtime_now(XTIME_Millisecond)
#define TIME_SECOND() xtime_now(XTIME_Second)
#define TIME_MINUTE() xtime_now(XTIME_Minute)
#define TIME_HOUR() xtime_now(XTIME_Hour)

typedef enum {
    XTIME_Layout, // "01/02 03:04:05PM '06 -0700" // The reference time, in numerical order.
    XTIME_ANSIC, // "Mon Jan _2 15:04:05 2006"
    XTIME_UnixDate, // "Mon Jan _2 15:04:05 MST 2006"
    XTIME_RubyDate, // "Mon Jan 02 15:04:05 -0700 2006"
    XTIME_RFC822, // "02 Jan 06 15:04 MST"
    XTIME_RFC822Z, // "02 Jan 06 15:04 -0700" // RFC822 with numeric zone
    XTIME_RFC850, // "Monday, 02-Jan-06 15:04:05 MST"
    XTIME_RFC1123, // "Mon, 02 Jan 2006 15:04:05 MST"
    XTIME_RFC1123Z, // "Mon, 02 Jan 2006 15:04:05 -0700" // RFC1123 with numeric zone
    XTIME_RFC3339, // !!! Expected "2006-01-02T15:04:05Z07:00" But got "2006-01-02T15:04:05Zxx:xx"
    XTIME_RFC3339Nano, // !!! Expected "2006-01-02T15:04:05.999999999Z07:00" But got "2006-01-02T15:04:05Zxx:xx"
    XTIME_Kitchen, // "3:04PM"

    /* Handy time stamps. !!! current just support second*/
    XTIME_Stamp, // "Jan _2 15:04:05"
    XTIME_StampMilli, // !!! Expected "Jan _2 15:04:05.000" But got "Jan _2 15:04:05"
    XTIME_StampMicro, // !!! Expected "Jan _2 15:04:05.000000" But got "Jan _2 15:04:05"
    XTIME_StampNano, // !!! Expected "Jan _2 15:04:05.000000000" But got "Jan _2 15:04:05"
    XTIME_DateTime, // "2006-01-02 15:04:05"
    XTIME_DateOnly, // "2006-01-02"
    XTIME_TimeOnly // "15:04:05"
} xtime_fmt;

typedef enum {
    XTIME_Nanosecond,
    XTIME_Microsecond, // 1000 * XTIME_Nanosecond,
    XTIME_Millisecond, // 1000 * XTIME_Microsecond,
    XTIME_Second, // 1000 * XTIME_Millisecond,
    XTIME_Minute, // 60 * XTIME_Second,
    XTIME_Hour // 60 * XTIME_Minute
} xtime_unit;

typedef long xtimestamp;

#define XTIME_LEGIBLE_MAX_LEN 64
xt_string xtime_now_legible(xtime_fmt fmt);
xt_string xtime_now_static_legible(xtime_fmt fmt, xt_string buf, xt_u8 buf_size);
xtimestamp xtime_now(xtime_unit unit);
xt_string xtime_timestamp_to_string(xtimestamp ts, xt_string buf, xt_u8 buf_size);
xtimestamp xtime_sub(xtimestamp ago, xtimestamp now);
xt_string xtime_timestamp_to_time(xtimestamp time, xtime_fmt fmt, xt_string buf, xt_u8 buf_size);


#ifdef __cplusplus
}
#endif
#endif /* __XTIME_H__ */

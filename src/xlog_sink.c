#include "xdef.h"
#include "xlog.h"
#include <stdio.h>

/*typedef xt_bool (*expect_log)(xlog_sink self, xlog_level level);*/
/*typedef void (*log)(xlog_sink self, xt_cstring output_message);*/
/*typedef void (*flush)(xlog_sink self);*/
/*typedef void (*destroy)(xlog_sink self);*/

xt_bool xlog_sink_console_st_expect_log(xlog_sink self, xlog_level level)
{
    if (self->priv_level <= level) {
        return XTRUE;
    }

    return XFALSE;
}

void xlog_sink_console_st_log(xlog_sink self, xt_cstring output_message)
{
    printf("%s\n", output_message);
}

void xlog_sink_console_st_flush(xlog_sink self)
{
    (void) self;
}

void xlog_sink_console_st_destroy(xlog_sink self)
{

}

xlog_sink xlog_sink_console_st()
{
    return xlog_sink_create("ConsoleSingleThread",
                            XLOG_LEVEL_INFO,
                            xlog_sink_console_st_expect_log,
                            xlog_sink_console_st_log,
                            xlog_sink_console_st_flush,
                            xlog_sink_console_st_destroy);
}

/*xlog_sink xlog_sink_console_mt();*/
/*xlog_sink xlog_sink_file_st(xt_cstring path);*/
/*xlog_sink xlog_sink_file_mt(xt_cstring path);*/
/*xlog_sink xlog_sink_rotating_file_st(xt_cstring path, xt_u32 max_size, xt_u32 max_count);*/
/*xlog_sink xlog_sink_rotating_file_mt(xt_cstring path, xt_u32 max_size, xt_u32 max_count);*/
/*xlog_sink xlog_sink_remote_st(xt_cstring url);*/
/*xlog_sink xlog_sink_remote_mt(xt_cstring url);*/

/*
 * © 2024 [ZhiWei-Ou]. All rights reserved.
 * @file xlog.h
 * @author Ouzw
 * @mail ouzw.mail@gmail.com
 * @date 2024-10-01 11:55:34
 *
 * @brief 
 */
#ifndef __XLOG_H__
#define __XLOG_H__

#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif

#include "xdef.h"
#include <stdarg.h>

#define XLOG_TRACE(string) xlog_print((xlog_message) {XLOG_LEVEL_TRACE, __LINE__, __func__, XFALSE, (string)})
#define XLOG_DEBUG(string) xlog_print((xlog_message) {XLOG_LEVEL_DEBUG, __LINE__, __func__, XFALSE, (string)})
#define XLOG_INFO(string) xlog_print((xlog_message) {XLOG_LEVEL_INFO, __LINE__, __func__, XFALSE, (string)})
#define XLOG_WARN(string) xlog_print((xlog_message) {XLOG_LEVEL_WARN, __LINE__, __func__, XFALSE, (string)})
#define XLOG_ERROR(string) xlog_print((xlog_message) {XLOG_LEVEL_ERROR, __LINE__, __func__, XFALSE, (string)})
#define XLOG_FATAL(string) xlog_print((xlog_message) {XLOG_LEVEL_FATAL, __LINE__, __func__, XFALSE, (string)})

#define XLOG_TRACEF(fmt, ...) xlog_print((xlog_message) {XLOG_LEVEL_TRACE, __LINE__, __func__, XTRUE, (fmt)}, ##__VA_ARGS__)
#define XLOG_DEBUGF(fmt, ...) xlog_print((xlog_message) {XLOG_LEVEL_DEBUG, __LINE__, __func__, XTRUE, (fmt)}, ##__VA_ARGS__)
#define XLOG_INFOF(fmt, ...) xlog_print((xlog_message) {XLOG_LEVEL_INFO, __LINE__, __func__, XTRUE, (fmt)}, ##__VA_ARGS__)
#define XLOG_WARNF(fmt, ...) xlog_print((xlog_message) {XLOG_LEVEL_WARN, __LINE__, __func__, XTRUE, (fmt)}, ##__VA_ARGS__)
#define XLOG_ERRORF(fmt, ...) xlog_print((xlog_message) {XLOG_LEVEL_ERROR, __LINE__, __func__, XTRUE, (fmt)}, ##__VA_ARGS__)
#define XLOG_FATALF(fmt, ...) xlog_print((xlog_message) {XLOG_LEVEL_FATAL, __LINE__, __func__, XTRUE, (fmt)}, ##__VA_ARGS__)

#define XLOG_COLOR_TRACE "\033[90m"
#define XLOG_COLOR_DEBUG "\033[36m"
#define XLOG_COLOR_INFO "\033[32m"
#define XLOG_COLOR_WARN "\033[33m"
#define XLOG_COLOR_ERROR "\033[31m"
#define XLOG_COLOR_FATAL "\033[35m"

#define XLOG_COLOR_GRAY "\033[90m"
#define XLOG_COLOR_RESET "\033[0m"

#define XLOG_LOGGER_MAX_SINKS 10
#define XLOG_PROMPT_MAX_SIZE 128
#define XLOG_BUFFER_MAX_SIZE 1024

typedef enum {
    XLOG_LEVEL_TRACE = 0,
    XLOG_LEVEL_DEBUG,
    XLOG_LEVEL_INFO,
    XLOG_LEVEL_WARN,
    XLOG_LEVEL_ERROR,
    XLOG_LEVEL_FATAL,

    XLOG_LEVEL_MAX
} xlog_level;

typedef struct xlog_logger_t *xlog_logger;
typedef struct xlog_sink_t *xlog_sink; 

typedef xt_bool (*expect_log)(xlog_sink self, xlog_level level);
typedef void (*log)(xlog_sink self, xt_cstring output_message);
typedef void (*flush)(xlog_sink self);
typedef void (*destroy)(xlog_sink self);
struct xlog_sink_t{
    const char *priv_name;
    void *priv_ctx;
    xlog_level priv_level;

    expect_log priv_expect_log;
    log priv_log;
    flush priv_flush;
    destroy priv_destroy;
};
xlog_sink xlog_sink_create(const char *sink_name,
                            xlog_level lvl,
                            log log,
                            flush flush,
                            destroy destroy);
xlog_sink xlog_sink_custom_create(const char *sink_name,
                            xlog_level lvl,
                            expect_log expect,
                            log log,
                            flush flush,
                            destroy destroy,
                            void *ctx);
void xlog_sink_set_context(xlog_sink self, void *ctx);
void xlog_sink_set_level(xlog_sink self, xlog_level lvl);
void xlog_sink_destroy(xlog_sink self);
xlog_sink xlog_sink_get_by_name(xt_cstring name);

struct xlog_logger_t {
    xt_u8 priv_sink_count;
    xlog_sink priv_sink[XLOG_LOGGER_MAX_SINKS];
};

xlog_logger xlog_init(const int count/*sink_count*/, .../*sinks*/);

typedef struct {
    xlog_level level;
    int caller_line;
    const char *caller_func;
    xt_bool is_format;

    xt_cstring body;
} xlog_message;
void xlog_print(xlog_message message, ...);


typedef xt_s32 (*xlog_prompt)(xlog_logger self, xlog_message *message, xt_string buf, xt_s32 buf_size);
void xlog_set_prompt(xlog_prompt prompt);

/**
 * Define Sinker Type
 */
#define XLOG_SINK_CONSOLE_ST_NAME "ConsoleSingleThread"
xlog_sink xlog_sink_console_st(xlog_level level);
#define XLOG_SINK_CONSOLE_MT_NAME "ConsoleMultiThread"
xlog_sink xlog_sink_console_mt(xlog_level level);

#define XLOG_SINK_FILE_ST_NAME "FileSingleThread"
xlog_sink xlog_sink_file_st(xlog_level level, xt_cstring path);
#define XLOG_SINK_FILE_MT_NAME "FileMultiThread"
xlog_sink xlog_sink_file_mt(xlog_level level, xt_cstring path);

#define XLOG_SINK_ROTATING_FILE_ST_NAME "RotatingFileSingleThread"
xlog_sink xlog_sink_rotating_file_st(xlog_level level, xt_cstring path, xt_u32 max_size, xt_u32 max_count);
#define XLOG_SINK_ROTATING_FILE_MT_NAME "RotatingFileMultiThread"
xlog_sink xlog_sink_rotating_file_mt(xlog_level level, xt_cstring path, xt_u32 max_size, xt_u32 max_count);

#define XLOG_SINK_UDP_ST_NAME "UdpSingleThread"
xlog_sink xlog_sink_udp_st(xlog_level level, xt_cstring url);
#define XLOG_SINK_UDP_MT_NAME "UdpMultiThread"
xlog_sink xlog_sink_udp_mt(xlog_level level, xt_cstring url);

#define XLOG_SINK_TCP_ST_NAME "TcpSingleThread"
xlog_sink xlog_sink_tcp_st(xlog_level level, xt_cstring url);
#define XLOG_SINK_TCP_MT_NAME "TcpMultiThread"
xlog_sink xlog_sink_tcp_mt(xlog_level level, xt_cstring url);


/**
 * Debug Interface
 */
void xlog_print_all_sink(int (*print)(const char *fmt, ...));

#ifdef __cplusplus
}
#endif
#endif /* __XLOG_H__ */

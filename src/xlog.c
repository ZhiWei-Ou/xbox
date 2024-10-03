#include "xlog.h"
#include "xdef.h"
#include "xmutex.h"
#include "xtime.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static const xt_cstring xlog_level_string[] = {
    [XLOG_LEVEL_TRACE] = "TRACE",
    [XLOG_LEVEL_DEBUG] = "DEBUG",
    [XLOG_LEVEL_INFO] = "INFO",
    [XLOG_LEVEL_WARN] = "WARN",
    [XLOG_LEVEL_ERROR] = "ERROR",
    [XLOG_LEVEL_FATAL] = "FATAL",
};

// 02 Oct 24 01:24 CST INFO
static inline xt_s32 xlog_prompt_default(xlog_logger self, xlog_message *message, xt_string buf, xt_s32 buf_size)
{
    (void) self;
    xt_s32 len = 0;
    xt_string tm = NULL;
    tm = xtime_now_legible(XTIME_RFC822);

    switch (message->level) {
    case XLOG_LEVEL_TRACE:
        len = snprintf(buf,
                buf_size,
                "%s%s%s %s%s%s ",
                XLOG_COLOR_GRAY,tm, XLOG_COLOR_RESET,
                XLOG_COLOR_TRACE,xlog_level_string[XLOG_LEVEL_TRACE],XLOG_COLOR_RESET);
        break;
    case XLOG_LEVEL_DEBUG:
        len = snprintf(buf,
                buf_size,
                "%s%s%s %s%s%s ",
                XLOG_COLOR_GRAY,tm, XLOG_COLOR_RESET,
                XLOG_COLOR_DEBUG,xlog_level_string[XLOG_LEVEL_DEBUG],XLOG_COLOR_RESET);
        break;
    case XLOG_LEVEL_INFO:
        len = snprintf(buf,
                buf_size,
                "%s%s%s %s%s%s ",
                XLOG_COLOR_GRAY,tm, XLOG_COLOR_RESET,
                XLOG_COLOR_INFO,xlog_level_string[XLOG_LEVEL_INFO],XLOG_COLOR_RESET);
        break;
    case XLOG_LEVEL_WARN:
        len = snprintf(buf,
                buf_size,
                "%s%s%s %s%s%s ",
                XLOG_COLOR_GRAY,tm, XLOG_COLOR_RESET,
                XLOG_COLOR_WARN,xlog_level_string[XLOG_LEVEL_WARN],XLOG_COLOR_RESET);
        break;
    case XLOG_LEVEL_ERROR:
        len = snprintf(buf,
                buf_size,
                "%s%s%s %s%s%s ",
                XLOG_COLOR_GRAY,tm, XLOG_COLOR_RESET,
                XLOG_COLOR_ERROR,xlog_level_string[XLOG_LEVEL_ERROR],XLOG_COLOR_RESET);
        break;
    case XLOG_LEVEL_FATAL:
        len = snprintf(buf,
                buf_size,
                "%s%s%s %s%s%s ",
                XLOG_COLOR_GRAY,tm, XLOG_COLOR_RESET,
                XLOG_COLOR_FATAL,xlog_level_string[XLOG_LEVEL_FATAL],XLOG_COLOR_RESET);
        break;
    case XLOG_LEVEL_MAX:
    default:
        len = 0;
        memset(buf, 0, buf_size);
    }

    if (tm) xt_free(tm);
    return len;
}

static xmutex instance_mutex = NULL;
static xlog_prompt instance_prompt = xlog_prompt_default;
static xt_u8 instance_log_prompt_buffer[XLOG_PROMPT_MAX_SIZE] = {0};
static xt_u8 instance_log_buffer[XLOG_BUFFER_MAX_SIZE] = {0};

extern struct xlog_sink_t xlog_default_console_sinker;
#define XLOG_DEFAULT_LOGGER_INIT \
{ \
    1, \
    { \
        [0] = &xlog_default_console_sinker, \
    } \
}

struct xlog_logger_t instance_logger = XLOG_DEFAULT_LOGGER_INIT;

static inline xt_u8 *get_log_buffer(void)
{
    return instance_log_buffer;
}

static inline xt_u8 *get_log_prompt_buffer(void)
{
    return instance_log_prompt_buffer;
}

static inline xlog_logger get_instance(void)
{
    if (!instance_mutex) {
        instance_mutex = XMutexAutoInit();
    }

    return &instance_logger;
}

void xlog_set_prompt(xlog_prompt prompt)
{
    xlog_logger ins = get_instance();
    (void) ins;

    if (prompt == NULL) {
        return;
    }

    xmutex_lock(instance_mutex);
    instance_prompt = prompt;
    xmutex_unlock(instance_mutex);
}

xlog_logger xlog_init(const int count, ...)
{
    xlog_logger ins = get_instance();
    ins->priv_sink_count = 0;

    va_list sinks;
    va_start(sinks, count);
    for (int i = 0; i < count && i < XLOG_LOGGER_MAX_SINKS; ++i) {
        xlog_sink s = va_arg(sinks, xlog_sink);
        if (s) {
            ins->priv_sink[ ins->priv_sink_count++ ] = s;
        }
    }
    va_end(sinks);

    return &instance_logger;
}

void xlog_print(xlog_message message, ...)
{
    xlog_sink *sinks = get_instance()->priv_sink;
    xt_u8 sink_count = get_instance()->priv_sink_count;

    xt_u8* prompt = get_log_prompt_buffer();
    xt_u8* buffer = get_log_buffer();
    xt_string output = NULL;

    xmutex_lock(instance_mutex);
    {
        instance_prompt(NULL, &message, (xt_string)prompt, XLOG_PROMPT_MAX_SIZE);

        // output: prompt + body
        int pos = snprintf((xt_string)buffer, XLOG_BUFFER_MAX_SIZE, "%s", (xt_string)prompt);

        if (message.is_format) {
            va_list args;
            va_start(args, message);

            vsnprintf((xt_string)buffer + pos, XLOG_BUFFER_MAX_SIZE, message.body, args);

            va_end(args);
        }
        else {
            snprintf((xt_string)buffer + pos, XLOG_BUFFER_MAX_SIZE, "%s", message.body);
        }

        output = strdup((xt_string)buffer);
    }
    xmutex_unlock(instance_mutex);


    if (output) {
        for (int i = 0; i < sink_count; ++i) {
            xlog_sink s = sinks[i];
            if (s->priv_expect_log(s, message.level)) {
                s->priv_log(s, output);
            }
        }

        xt_free(output);
    }
}

xt_bool general_expect_log(xlog_sink self, xlog_level level)
{
    if (self->priv_level <= level) {
        return XTRUE;
    }

    return XFALSE;
}


xlog_sink xlog_sink_create(const char *sink_name,
                            xlog_level lvl,
                            log log,
                            flush flush,
                            destroy destroy)
{
    return xlog_sink_custom_create(sink_name, lvl, general_expect_log, log, flush, destroy, NULL);
}

xlog_sink xlog_sink_custom_create(const char *sink_name,
                            xlog_level lvl,
                            expect_log expect,
                            log log,
                            flush flush,
                            destroy destroy,
                            void *ctx)
{
    if (!sink_name || !log || !flush || !destroy /*|| !expect*/) {
        return NULL;
    }
    xlog_sink self = (xlog_sink) xt_malloc(sizeof(struct xlog_sink_t));
    if (!self) {
        return NULL;
    }

    self->priv_name = sink_name;
    self->priv_level = lvl;
    if (expect) self->priv_expect_log = expect;
    else self->priv_expect_log = general_expect_log;
    self->priv_log = log;
    self->priv_flush = flush;
    self->priv_destroy = destroy;

    self->priv_ctx = ctx;

    return self;
}

void xlog_sink_set_context(xlog_sink self, void *ctx)
{
    self->priv_ctx = ctx;
}
void xlog_sink_set_level(xlog_sink self, xlog_level lvl)
{
    self->priv_level = lvl;
}
void xlog_sink_destroy(xlog_sink self)
{
    if (self->priv_destroy) {
        self->priv_destroy(self);
    }
    xt_free(self);
    self = NULL;
}
xlog_sink xlog_sink_get_by_name(xt_cstring name)
{
    if (!name) {
        return NULL;
    }

    xlog_logger ins = get_instance();
    for (int i = 0; i < ins->priv_sink_count; ++i) {
        xlog_sink s = ins->priv_sink[i];
        if (s->priv_name && !strcmp((const char *)name, s->priv_name)) {
            return s;
        }
    }
    return NULL;
}


void xlog_print_all_sink(int (*print)(const char *fmt, ...))
{
    xlog_logger ins = get_instance();

    print("XLogger Sinks:\n");
    for (int i = 0; i < ins->priv_sink_count; ++i) {
        xlog_sink s = ins->priv_sink[i];
        print("\t#%d sink[%p]: %s\n", i, s, s->priv_name);
    }
}


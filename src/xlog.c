#include "xlog.h"
#include "xdef.h"
#include "xmutex.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static inline xt_s32 xlog_prompt_default(xlog_logger self, xlog_message *message, xt_string buf, xt_s32 buf_size)
{
    switch (message->level) {
    case XLOG_LEVEL_TRACE:
        return snprintf(buf, buf_size, XLOG_COLOR_TRACE"%s "XLOG_COLOR_RESET, "TRACE");
    case XLOG_LEVEL_DEBUG:
        return snprintf(buf, buf_size, XLOG_COLOR_DEBUG"%s "XLOG_COLOR_RESET, "DEBUG");
    case XLOG_LEVEL_INFO:
        return snprintf(buf, buf_size, XLOG_COLOR_INFO"%s "XLOG_COLOR_RESET, "INFO");
    case XLOG_LEVEL_WARN:
        return snprintf(buf, buf_size, XLOG_COLOR_WARN"%s "XLOG_COLOR_RESET, "WARN");
    case XLOG_LEVEL_ERROR:
        return snprintf(buf, buf_size, XLOG_COLOR_ERROR"%s "XLOG_COLOR_RESET, "ERROR");
    case XLOG_LEVEL_FATAL:
        return snprintf(buf, buf_size, XLOG_COLOR_FATAL"%s "XLOG_COLOR_RESET, "FATAL");

    case XLOG_LEVEL_MAX:
    default:
        return 0;
    }
}

static xmutex instance_mutex = NULL;
static xlog_prompt instance_prompt = xlog_prompt_default;
static xt_u8 instance_log_prompt_buffer[XLOG_PROMPT_MAX_SIZE] = {0};
static xt_u8 instance_log_buffer[XLOG_BUFFER_MAX_SIZE] = {0};
struct xlog_logger_t instance_logger = {0};

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
    va_list sinks;
    va_start(sinks, count);
    for (int i = 0; i < count && i < XLOG_LOGGER_MAX_SINKS; ++i) {
        xlog_sink s = va_arg(sinks, xlog_sink);
        if (s) {
            get_instance()->priv_sink[ get_instance()->priv_sink_count++ ] = s;
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


xlog_sink xlog_sink_create(const char *sink_name,
                            xlog_level lvl,
                            expect_log expect,
                            log log,
                            flush flush,
                            destroy destroy)
{
    xlog_sink self = (xlog_sink) xt_malloc(sizeof(struct xlog_sink_t));
    if (!self) {
        return NULL;
    }

    self->priv_name = sink_name;
    self->priv_level = lvl;
    self->priv_expect_log = expect;
    self->priv_log = log;
    self->priv_flush = flush;
    self->priv_destroy = destroy;

    self->priv_ctx = NULL;

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



void xlog_print_all_sink(int (*print)(const char *fmt, ...))
{
    xlog_logger ins = get_instance();

    for (int i = 0; i < ins->priv_sink_count; ++i) {
        xlog_sink s = ins->priv_sink[i];
        print("#%d sink[%p]: %s\n", i, s, s->priv_name);
    }
}
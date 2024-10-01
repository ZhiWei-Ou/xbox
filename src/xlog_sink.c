#include "xdef.h"
#include "xlog.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*typedef xt_bool (*expect_log)(xlog_sink self, xlog_level level);*/
/*typedef void (*log)(xlog_sink self, xt_cstring output_message);*/
/*typedef void (*flush)(xlog_sink self);*/
/*typedef void (*destroy)(xlog_sink self);*/

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

xlog_sink xlog_sink_console_st(xlog_level level)
{
    return xlog_sink_create(XLOG_SINK_CONSOLE_ST_NAME,
                            level,
                            xlog_sink_console_st_log,
                            xlog_sink_console_st_flush,
                            xlog_sink_console_st_destroy);
}



typedef struct {
    xt_string file;
    FILE *fp;
} xlog_sink_file_t;

void xlog_sink_file_st_log(xlog_sink self, xt_cstring output_message)
{
    xlog_sink_file_t *priv = self->priv_ctx;

    fprintf(priv->fp, "%s\n", output_message);
}

void xlog_sink_file_st_flush(xlog_sink self)
{
    xlog_sink_file_t *priv = self->priv_ctx;

    fflush(priv->fp);
}

void xlog_sink_file_st_destroy(xlog_sink self)
{
    xlog_sink_file_t *priv = self->priv_ctx;
    if (!priv) {
        return;
    }

    if (priv->fp) {
        fclose(priv->fp);
    }
    if (priv->file) {
        xt_free(priv->file);
    }
    xt_free(priv);

    self->priv_ctx = NULL;
}

xlog_sink xlog_sink_file_st(xlog_level level, xt_cstring path)
{
    if (path == NULL) {
        return NULL;
    }

    xlog_sink_file_t *ctx = (xlog_sink_file_t *) xt_malloc(sizeof(xlog_sink_file_t));
    if (!ctx) {
        return NULL;
    }

    ctx->fp = fopen(path, "w");
    if (!ctx->fp) {
        xt_free(ctx);
        return NULL;
    }
    ctx->file = strdup(path);

    return xlog_sink_custom_create(XLOG_SINK_FILE_ST_NAME,
                                    level,
                                    NULL,
                                    xlog_sink_file_st_log,
                                    xlog_sink_file_st_flush,
                                    xlog_sink_file_st_destroy,
                                    ctx);
}

/*xlog_sink xlog_sink_console_mt();*/
/*xlog_sink xlog_sink_file_st(xt_cstring path);*/
/*xlog_sink xlog_sink_file_mt(xt_cstring path);*/
/*xlog_sink xlog_sink_rotating_file_st(xt_cstring path, xt_u32 max_size, xt_u32 max_count);*/
/*xlog_sink xlog_sink_rotating_file_mt(xt_cstring path, xt_u32 max_size, xt_u32 max_count);*/
/*xlog_sink xlog_sink_remote_st(xt_cstring url);*/
/*xlog_sink xlog_sink_remote_mt(xt_cstring url);*/

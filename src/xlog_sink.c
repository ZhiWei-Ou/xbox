#include "xdef.h"
#include "xlog.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// socket
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

/*typedef xt_bool (*expect_log)(xlog_sink self, xlog_level level);*/
/*typedef void (*log)(xlog_sink self, xt_cstring output_message);*/
/*typedef void (*flush)(xlog_sink self);*/
/*typedef void (*destroy)(xlog_sink self);*/
extern xt_bool general_expect_log(xlog_sink self, xlog_level level);

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

struct xlog_sink_t xlog_default_console_sinker = {
    "ConsoleSinker",
    NULL,
    XLOG_LEVEL_INFO,
    general_expect_log,
    xlog_sink_console_st_log,
    xlog_sink_console_st_flush,
    xlog_sink_console_st_destroy
};
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

typedef struct {
    int priv_sockfd;
    struct sockaddr_in priv_addr;
} xlog_sink_udp_t;

void xlog_sink_udp_st_log(xlog_sink self, xt_cstring output_message)
{
    xlog_sink_udp_t *ctx = self->priv_ctx;

    sendto(ctx->priv_sockfd,
            output_message,
            strlen(output_message),
            0,
            (struct sockaddr *)&ctx->priv_addr, sizeof(ctx->priv_addr));
}

void xlog_sink_udp_st_flush(xlog_sink self)
{
    (void) self;
}

void xlog_sink_udp_st_destroy(xlog_sink self)
{
    xlog_sink_udp_t *ctx = self->priv_ctx;
    if (!ctx) {
        return;
    }

    close(ctx->priv_sockfd);
    xt_free(ctx);
    self->priv_ctx = NULL;
}

xlog_sink xlog_sink_udp_st(xlog_level level, xt_cstring host, xt_u16 port)
{
    xlog_sink_udp_t *ctx = (xlog_sink_udp_t *) xt_malloc(sizeof(xlog_sink_udp_t));
    if (!ctx) {
        return NULL;
    }
    
    if ((ctx->priv_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        xt_free(ctx);
        return NULL;
    }

    memset(&ctx->priv_addr, 0, sizeof(ctx->priv_addr));
    ctx->priv_addr.sin_family = AF_INET;
    ctx->priv_addr.sin_port = htons(port);
    ctx->priv_addr.sin_addr.s_addr = inet_addr(host);

    return xlog_sink_custom_create(XLOG_SINK_UDP_ST_NAME,
                                    level,
                                    NULL,
                                    xlog_sink_udp_st_log,
                                    xlog_sink_udp_st_flush,
                                    xlog_sink_udp_st_destroy,
                                    ctx);
}

typedef struct {
    int priv_sockfd;
    struct sockaddr_in priv_addr;
} xlog_sink_tcp_t;

void xlog_sink_tcp_st_log(xlog_sink self, xt_cstring output_message)
{
    xlog_sink_tcp_t *ctx = self->priv_ctx;

    send(ctx->priv_sockfd, output_message, strlen(output_message), MSG_NOSIGNAL);
}

void xlog_sink_tcp_st_flush(xlog_sink self)
{
    (void) self;
}

void xlog_sink_tcp_st_destroy(xlog_sink self)
{
    xlog_sink_tcp_t *ctx = self->priv_ctx;
    if (!ctx) {
        return;
    }

    close(ctx->priv_sockfd);
    xt_free(ctx);
    self->priv_ctx = NULL;
}

xlog_sink xlog_sink_tcp_st(xlog_level level, xt_cstring ip, xt_u16 port)
{
    xlog_sink_tcp_t *ctx = (xlog_sink_tcp_t *) xt_malloc(sizeof(xlog_sink_tcp_t));
    if (!ctx) {
        return NULL;
    }
    
    if ((ctx->priv_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("-- [Log] socket creation failed\n");
        xt_free(ctx);
        return NULL;
    }

    memset(&ctx->priv_addr, 0, sizeof(ctx->priv_addr));
    ctx->priv_addr.sin_family = AF_INET;
    ctx->priv_addr.sin_addr.s_addr = inet_addr(ip);
    ctx->priv_addr.sin_port = htons(port);

    if (connect(ctx->priv_sockfd, (struct sockaddr *)&ctx->priv_addr, sizeof(ctx->priv_addr)) < 0) {
        printf("\n-- [Log] Connection Failed \n");
        xt_free(ctx);
        return NULL;
    }

    return xlog_sink_custom_create(XLOG_SINK_TCP_ST_NAME,
                                    level,
                                    NULL,
                                    xlog_sink_tcp_st_log,
                                    xlog_sink_tcp_st_flush,
                                    xlog_sink_tcp_st_destroy,
                                    ctx);
}

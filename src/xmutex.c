#include "xmutex.h"
#include "xdef.h"

#include <stdlib.h>

struct xmutex_t {
    void *priv_mutex;

    lock priv_lock;
    unlock priv_unlock;
    destory priv_destory;
};

xmutex xmutex_init(create f0, lock f1, unlock f2, destory f3)
{
    xmutex self = (xmutex) xt_malloc(sizeof(struct xmutex_t));
    if (!self) {
        return NULL;
    }

    if (f0) {
        self->priv_mutex = f0();
    }
    self->priv_lock = f1;
    self->priv_unlock = f2;
    self->priv_destory = f3;

    return self;
}

void xmutex_fini(xmutex self)
{
    if (!self) {
        return;
    }

    if (self->priv_destory) {
        self->priv_destory(self->priv_mutex);
    }

    xt_free(self);
    self = NULL;
}

void xmutex_lock(xmutex self)
{
    // if (!self) {
    // return;
    // }

    if (self->priv_lock) {
        self->priv_lock(self->priv_mutex);
    }
}

void xmutex_unlock(xmutex self)
{
    if (self->priv_unlock) {
        self->priv_unlock(self->priv_mutex);
    }
}

xmutex get_null_mutex(void)
{
    static xmutex null_mutex = NULL;
    if (null_mutex) {
        return null_mutex;
    }

    null_mutex = xmutex_init(NULL, NULL, NULL, NULL);
    return null_mutex;
}

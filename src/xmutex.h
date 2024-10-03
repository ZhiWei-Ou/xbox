/*
 * © 2024 [ZhiWei-Ou]. All rights reserved.
 * @file xmutex.h
 * @author Ouzw
 * @mail ouzw.mail@gmail.com
 * @date 2024-10-01 19:14:57
 *
 * @brief 
 */
#ifndef __XMUTEX_H__
#define __XMUTEX_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "xdef.h"

#define XMutexAutoInit() xmutex_init(platform_mutex_create, platform_mutex_lock, platform_mutex_unlock, platform_mutex_destory)

typedef struct xmutex_t *xmutex;

// Platform Specific
typedef void *(*create)();
typedef void (*lock)(void *m);
typedef void (*unlock)(void *m);
typedef void (*destory)(void *m);

xmutex xmutex_init(create f0, lock f1, unlock f2, destory f3);
void xmutex_lock(xmutex self);
void xmutex_unlock(xmutex self);
void xmutex_fini(xmutex self);

xmutex get_null_mutex(void);

#if defined(__APPLE__) || defined(_POSIX_VERSION) || defined(__linux__)
#include <pthread.h>
#include <stdlib.h>
static inline void *platform_mutex_create()
{
    pthread_mutex_t *m = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(m, NULL);
    return (void *)m;
}
static inline void platform_mutex_lock(void *m)
{ pthread_mutex_lock((pthread_mutex_t *)m); }
static inline void platform_mutex_unlock(void *m)
{ pthread_mutex_unlock((pthread_mutex_t *)m); }
static inline void platform_mutex_destory(void *m)
{ pthread_mutex_destroy((pthread_mutex_t *)m); free(m); m = NULL; }
#endif

#ifdef __cplusplus
}
#endif
#endif /* __XMUTEX_H__ */

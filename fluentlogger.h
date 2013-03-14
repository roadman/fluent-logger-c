/*
 * fluent-logger-c : fluentlogger.h
 */

#ifndef __LIB_FLUENT_LOGGER_H__
#define __LIB_FLUENT_LOGGER_H__

#include <sys/types.h>

#define FLUENT_ERR  -1
#define FLUENT_OK   0

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char    *err;
    int      fd;
} fluent_context_t;


fluent_context_t    *fluent_connect(const char *ip, int port);
#if 0
fluent_context_t    *fluent_connect_unix(const char *path);
#endif
int                  fluent_post_json(fluent_context_t *, const char *, const char *);
void                 fluent_free(fluent_context_t *c);

#ifdef __cplusplus
}
#endif

#endif /* __LIB_FLUENT_LOGGER_H__ */

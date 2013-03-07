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
} fluent_context_t


fluent_context_t    *fluent_connect(const char *ip, int port);
fluent_context_t    *fluent_connect_unix(const char *path);

int                  fluent_post(fluent_context_t *c, const char *tag, const char *data);

void                 fluent_free(fluent_context_t *c);

#ifdef __cplusplus
}
#endif

#endif /* __LIB_FLUENT_LOGGER_H__ */

/*
 * fluent-logger-c : fluentlogger.h
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


#include "fluentlogger.h"

static fluent_context_t *fluent_context_init(void);
static int fluent_connect_tcp(fluent_context_t *, const char *, int);

fluent_context_t *fluent_connect(const char *ip, int port)
{
    fluent_context_t    *c;

    c = fluent_context_init();
    if (c == NULL)
    {
        return NULL;
    }

    fluent_connect_tcp(c,ip,port,NULL);

    return c;
}

fluent_context_t *fluent_connect_unix(const char *path)
{
}

int fluent_post(fluent_context_t *c, const char *tag, const char *data)
{
}

void fluent_free(fluent_context_t *c)
{
    if(c == NULL)
    {
        return;
    }
    if(c->fd > 0)
    {
        close(c->fd);
    }
    if(c->err != NULL)
    {
        free(c->err);
    }
    free(c);
    return;
}

static fluent_context_t *fluent_context_init(void)
{
    fluent_context_t *c;

    c = calloc(1, sizeof(fluent_context_t));
    if(c == NULL)
    {
        return NULL;
    }

    (void)memset(c, 0, sizeof(fluent_context_t));

    return c;
}

static int fluent_connect_tcp(fluent_context_t *c, const char *addr, int port)
{
    int                  s,
                         rv = REDIS_ERR;
    char                 _port[6];
    struct addrinfo      hints,
                        *servinfo,
                        *p;

    snprintf(_port, sizeof(_port), "%d", port);
    memset(&hints, 0, sizeof(hints));

    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(addr, _port, &hints, &servinfo)) != 0)
    {
        return(FLUENT_OK);
    }

    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        s = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (s == -1)
        {
            continue;
        }

        if (connect(s, p->ai_addr, p->ai_addrlen) != -1)
        {
            break;
        }

        close(s);
    }

    if (p != NULL)
    {
        rv = FLUENT_OK;
        c->fd = s;
    }

    freeaddrinfo(servinfo);

    return(rv);
}

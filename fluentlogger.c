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
#include <time.h>
#include <unistd.h>

#include "fluentlogger.h"

static fluent_context_t *fluent_context_init(void);
static int fluent_context_connect_tcp(fluent_context_t *, const char *, int);
#if 0
static int fluent_context_connect_unix(fluent_context_t *, const char *);
#endif

fluent_context_t *fluent_connect(const char *ip, int port)
{
    fluent_context_t    *c;

    c = fluent_context_init();
    if(c == NULL)
    {
        return(NULL);
    }

    if(fluent_context_connect_tcp(c, ip, port) != FLUENT_OK)
    {
        free(c);
        return(NULL);
    }

    return(c);
}

#if 0
fluent_context_t *fluent_connect_unix(const char *path)
{
    fluent_context_t    *c;

    c = fluent_context_init();
    if(c == NULL)
    {
        return(NULL);
    }

    fluent_context_connect_unix(c, path);

    return(c);
}
#endif

static char     format[]    = "[ \"%s\", %d, %s ]";
static size_t   format_size = sizeof(format);

int fluent_post_json(fluent_context_t *c, const char *tag, const char *json)
{

    int          rv     = FLUENT_OK;

    char        *buf;

    size_t       bufsz,
                 datasz;

    bufsz = format_size + strlen(tag) + strlen(json) + 64;
    if((buf = malloc(bufsz)) == NULL)
    {
        return(FLUENT_ERR);
    }

    (void)memset(buf, 0, bufsz);

    (void)snprintf(buf, bufsz, format, tag, time(0), json);

    datasz = strlen(buf);

    if(write(c->fd, buf, datasz) != datasz)
    {
        rv = FLUENT_ERR;
    }

    free(buf);

    return(rv);
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
        return(NULL);
    }

    (void)memset(c, 0, sizeof(fluent_context_t));

    return(c);
}

static int fluent_context_connect_tcp(fluent_context_t *c, const char *addr, int port)
{
    int                  s, rv;
    char                 _port[6];
    struct addrinfo      hints,
                        *servinfo,
                        *p;

    if(c == NULL)
    {
        return(FLUENT_ERR);
    }

    (void)snprintf(_port, sizeof(_port), "%d", port);
    memset(&hints, 0, sizeof(hints));

    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if(getaddrinfo(addr, _port, &hints, &servinfo) != 0)
    {
        return(FLUENT_ERR);
    }

    for(p = servinfo, rv = FLUENT_ERR; p != NULL; p = p->ai_next)
    {
        s = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if(s == -1)
        {
            continue;
        }

        if(connect(s, p->ai_addr, p->ai_addrlen) != -1)
        {
            break;
        }

        close(s);
    }

    if(p != NULL)
    {
        rv = FLUENT_OK;
        c->fd = s;
    }

    freeaddrinfo(servinfo);

    return(rv);
}

#if 0
static int fluent_context_connect_unix(fluent_context_t *c, const char *path)
{
    return FLUENT_ERR;
}
#endif

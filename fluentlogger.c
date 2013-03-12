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
#include <msgpack.h>

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

    fluent_context_connect_tcp(c, ip, port);

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

    bool         rv     = FLUENT_OK;

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

#if 0
int fluent_post(fluent_context_t *c, const char *tag, const msgpack_object *data)
{

    bool                 rv     = FLUENT_OK;

    msgpack_sbuffer     *buffer = msgpack_sbuffer_new();
    msgpack_packer      *pk     = msgpack_packer_new(buffer, msgpack_sbuffer_write);

    msgpack_pack_array(pk, 3);

    msgpack_pack_raw(pk, strlen(tag));
    msgpack_pack_raw_body(pk, tag, strlen(tag));
    msgpack_pack_raw(pk, sizeof(int64_t));
    msgpack_pack_int64(pk, time(0));
    msgpack_pack_raw(pk, data->size);
    msgpack_pack_raw_body(pk, data->data, data->size);

    msgpack_unpacked msg;
    msgpack_unpacked_init(&msg);
    bool success = msgpack_unpack_next(&msg, buffer->data, buffer->size, NULL);

    msgpack_object obj = msg.data;
    msgpack_object_print(stdout, obj);

    //char *aa = "[\"debug.test\", 0, { \"test\":\"log\"}]";
    //if(write(c->fd, aa, strlen(aa)) != buffer->size)
    if(write(c->fd, buffer->data, buffer->size) != buffer->size)
    {
        rv = FLUENT_ERR;
    }

    msgpack_sbuffer_free(buffer);
    msgpack_packer_free(pk);

    return(rv);
}
#endif

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
    int                  s,
                         rv = FLUENT_ERR;
    char                 _port[6];
    struct addrinfo      hints,
                        *servinfo,
                        *p;

    (void)snprintf(_port, sizeof(_port), "%d", port);
    memset(&hints, 0, sizeof(hints));

    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if((rv = getaddrinfo(addr, _port, &hints, &servinfo)) != 0)
    {
        return(FLUENT_OK);
    }

    for(p = servinfo; p != NULL; p = p->ai_next)
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

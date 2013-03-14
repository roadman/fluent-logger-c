
#include <stdio.h>

#include "fluentlogger.h"

int main()
{
    fluent_context_t *c = fluent_connect("127.0.0.1", 24224);
    if(c == NULL)
    {
        printf("connect error\n");
        exit(1);
    }

    if(fluent_post_json(c, "debug.test", "{\"Hello\":\"MessagePack\"}") != 0)
    {
        fluent_free(c);
        printf("post error\n");
        exit(1);
    }

    fluent_free(c);

    exit(0);
}

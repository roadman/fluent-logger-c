
#include <msgpack.h>

#include "fluentlogger.h"

int main()
{
    fluent_context_t *c = fluent_connect("127.0.0.1", 24224);

    fluent_post_json(c, "debug.test", "{\"Hello\":\"MessagePack\"}");

    fluent_free(c);
}

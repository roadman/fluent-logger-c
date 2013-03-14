# Fluent Logger for C

## Install

 $ cd fluent-logger-c
 $ make
 $ sudo make install

## Quickstart

    #include <stdio.h>
    #include "fluentlogger.h"
    
    int main()
    {
        fluent_context_t *c = fluent_connect("127.0.0.1", 24225);
        if(c == NULL)
        {
            printf("connect error\n");
            return(1);
        }
   
        if(fluent_post_json(c, "debug.test", "{\"Hello\":\"MessagePack\"}") != 0)
        {
            fluent_free(c);
            printf("post error\n");
            return(1);
        }
   
        fluent_free(c);
   
        return(0);
    }

## License
Apache License, Version 2.0


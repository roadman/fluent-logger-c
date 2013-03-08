CFLAGS= $(INCS) $(WARN) -O2 $G -fPIC -g  -std=gnu99
WARN= -pedantic -Wall
INCS= 

MYNAME= fluentlogger
MYLIB= $(MYNAME)
T= lib$(MYLIB).so
S= lib$(MYLIB).a
OBJS= $(MYLIB).o
LINKS=-lfluentlogger -L. -lmsgpack

all:	$T $S 

o:	$(MYLIB).o

so:	$T

a:	$S

$T:	$(OBJS)
	$(CC) -o $@ -shared $(OBJS)

$S:	$(OBJS)
	ar rv $@ $(OBJS)

clean:
	rm -f $(OBJS) $T $S 

test:
	gcc -g -Wall fluentlogger-test.c -o fluentlogger-test $(LINKS)

install:
	cp -p $T $S /usr/lib64/
   

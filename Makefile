CFLAGS=-std=c11 -g -static
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

9cc: $(OBJS)
	$(CC) -o 9cc $(OBJS) $(LDFLAGS)

$(OBJS):9cc.h

test: 9cc
	./test.sh

clean:
	rm -f 9cc *.o *~ tmp*

9ccdbg:  $(OBJS)
	$(CC) -Wall -g -o 9cc $(OBJS) $(LDFLAGS)

debug: 9ccdbg


.PHONY: test clean

CFLAGS = -Wall -D_GNU_SOURCE -std=c99 -O2 -ggdb
LIBS =
SRCS = pool.c main.c
OBJS = ${SRCS:.c=.o}

.PHONY: all clean

all: testpool

testpool: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

clean:
	@rm -f *.o *~ testpool

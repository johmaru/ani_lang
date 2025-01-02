CFLAGS=-std=c11 -g -static
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

ani_lang: $(OBJS)
	$(CC) -o ani_lang $(OBJS) $(LDFLAGS)

$(OBJS): ani_lang.h		

test: ani_lang
	./test.sh

clean:
	rm -f ani_lang *.o *~ tmp*

.PHONY: test clean		
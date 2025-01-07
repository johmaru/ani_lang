CFLAGS=-std=c11 -g -static -fno-stack-protector
LDFLAGS=-no-pie
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

# テスト用のオブジェクトファイル
TEST_OBJS=testfunc.o

ani_lang: $(OBJS)
	$(CC) -o ani_lang $(OBJS) $(LDFLAGS)

$(OBJS): ani_lang.h error_message.h

# test_func.oのビルドルール
test_func.o: testfunc.c
	$(CC) $(CFLAGS) -c testfunc.c

test: ani_lang testfunc.o
	$(CC) -c testfunc.c
	./test.sh

clean:
	rm -f ani_lang *.o *~ tmp*

.PHONY: test clean
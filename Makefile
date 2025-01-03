CC = gcc
CFLAGS = -Wall -fPIC
LDFLAGS = -shared -ldl

all: libhook.so test

libhook.so: hook.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

test: test.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f libhook.so test test.txt

run: all
	touch test.txt
	LD_PRELOAD=./libhook.so ./test

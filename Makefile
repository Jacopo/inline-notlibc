CC := gcc
CFLAGS := -Wall -Wextra -Wwrite-strings -Wpointer-arith -D_FORTIFY_SOURCE=2 -Wformat=2 -Werror=format-security -g3 -ggdb -gdwarf-4

CFLAGS += --std=gnu99 -fno-stack-protector -nostdlib

#CFLAGS += -m32

all: test
test: test.c *.h int/*.h int/*/*.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $< $(LDFLAGS)

.PHONY: clean all
clean:
	rm -f test

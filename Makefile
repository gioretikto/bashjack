CC = gcc
CFLAGS = -std=c99 -W -Wextra -Werror -Wall -pedantic  -Wstrict-prototypes -Wmissing-prototypes -Wshadow -Wconversion
SRCS = main.c black.c deck.c
OBJS = $(SRCS:.c=.o)
PREFIX ?= /usr/local
BINDIR ?= $(PREFIX)/bin
TARGET = bashjack

$(TARGET): $(OBJS)
	 $(CC) -o $@ $^ $(CFLAGS)
clean:
	rm -f $(OBJS)
install:
	install $(TARGET) $(BINDIR)
	
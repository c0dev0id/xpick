.POSIX:

PREFIX ?= /usr/local
PKG_CONFIG = pkg-config

CFLAGS = -Wall `$(PKG_CONFIG) --cflags x11`
LDFLAGS = `$(PKG_CONFIG) --libs x11`

all: xpick

xpick:
	$(CC) $(CFLAGS) -o ${@} xpick.c $(LDFLAGS)

clean:
	rm -f xpick

install: xpick
	install -m755 xpick $(DESTDIR)$(PREFIX)/bin/xpick

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/xpick

.PHONY: all clean install uninstall

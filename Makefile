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
	install -m644 xpick.1 $(DESTDIR)$(PREFIX)/man/man1/xpick.1

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/xpick
	rm -f $(DESTDIR)$(PREFIX)/man/man1/xpick.1

.PHONY: all clean install uninstall

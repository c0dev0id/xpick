.POSIX:

VERSION = 1.0
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
	install -m755 xpick $(PREFIX)/bin/xpick
	install -m644 xpick.1 $(PREFIX)/man/man1/xpick.1

uninstall:
	rm -f $(PREFIX)/bin/xpick
	rm -f $(PREFIX)/man/man1/xpick.1

release:
	rm -rf xpick-$(VERSION)*
	mkdir xpick-$(VERSION)
	cp Makefile xpick.c xpick.1 README xpick-$(VERSION)/
	tar czf xpick-$(VERSION).tar.gz xpick-$(VERSION)
	rm -rf xpick-$(VERSION)


.PHONY: all clean install uninstall

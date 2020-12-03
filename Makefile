CFLAGS=-g -Wall
INCS=-I/usr/X11R6/include
LIBS=-L/usr/X11R6/lib -lX11

all: clean xpick

xpick:
	$(CC) $(CFLAGS) $(INCS) $(LIBS) -o ${@} xpick.c

clean:
	rm -f xpick

install: xpick
	install -m755 xpick /usr/local/bin/xpick

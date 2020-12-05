/* Copyright (c) 2020 Stefan Hagen <sh+ports@codevoid.de>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

// original implementation in tmux colour.c
static int rgb_to_x256(uint8_t r, uint8_t g, uint8_t b)
{
    // Calculate the nearest 0-based color index at 16 .. 231
    #define v2ci(v) (v < 48 ? 0 : v < 115 ? 1 : (v - 35) / 40)
    int ir = v2ci(r), ig = v2ci(g), ib = v2ci(b);   // 0..5 each
    #define color_index() (36 * ir + 6 * ig + ib)  /* 0..215, lazy evaluation */

    // Calculate the nearest 0-based gray index at 232 .. 255
    int average = (r + g + b) / 3;
    int gray_index = average > 238 ? 23 : (average - 3) / 10;  // 0..23

    // Calculate the represented colors back from the index
    static const int i2cv[6] = {0, 0x5f, 0x87, 0xaf, 0xd7, 0xff};
    int cr = i2cv[ir], cg = i2cv[ig], cb = i2cv[ib];  // r/g/b, 0..255 each
    int gv = 8 + 10 * gray_index;  // same value for r/g/b, 0..255

    // Return the one which is nearer to the original input rgb value
    #define dist_square(A,B,C, a,b,c) ((A-a)*(A-a) + (B-b)*(B-b) + (C-c)*(C-c))
    int color_err = dist_square(cr, cg, cb, r, g, b);
    int gray_err  = dist_square(gv, gv, gv, r, g, b);
    return color_err <= gray_err ? 16 + color_index() : 232 + gray_index;
}

int main(int argc, char *argv[]) {

    if(argc <= 1) {
        printf("Usage: xpick [-rhx]\n");
        printf("    -r   RGB notation RR/GG/BB\n");
        printf("    -h   HEX notation #RRGGBB\n");
        printf("    -x   XTerm nearest terminal color\n");
        return 2;
    }

    Display *dpy = XOpenDisplay(NULL);
    Window root = RootWindow(dpy, DefaultScreen(dpy));

    Cursor cursor = XCreateFontCursor(dpy, XC_tcross);
    XColor fgc, bgc;
    fgc.red = -1; fgc.green = 0; fgc.blue = 0;
    bgc.red = -1; bgc.green = -1; bgc.blue = -1;

    XRecolorCursor(dpy, cursor, &fgc, &bgc);
    XGrabPointer(dpy, root, 0, ButtonReleaseMask, GrabModeAsync, GrabModeAsync,
            None, cursor, CurrentTime);

    XEvent event;
    XNextEvent(dpy, &event);
    XUngrabPointer(dpy, CurrentTime);
    XImage *ximage = XGetImage(dpy, root, event.xbutton.x_root, 
            event.xbutton.y_root, 1, 1, -1, ZPixmap);

    unsigned long p = XGetPixel(ximage, 0, 0);
    XDestroyImage(ximage);
    XColor result; result.pixel = p;
    XQueryColor(dpy, DefaultColormap(dpy, DefaultScreen(dpy)), &result);

    int opt;
    while((opt = getopt(argc, argv, ":hrx")) != -1){
        switch(opt){
            case 'h':
                printf("#%02x%02x%02x\n",
                        result.red/256, result.green/256, result.blue/256);
                break;
            case 'r':
                printf("%03d/%03d/%03d\n",
                        result.red/256, result.green/256, result.blue/256);
                break;
            case 'x':
                printf("%d\n", rgb_to_x256(result.red/256, result.green/256,
                        result.blue/256));
                break;
            case '?': //used for some unknown opts
                printf("Unknown option: %c\n", optopt);
                break;
        }
    }
    return 0;
}

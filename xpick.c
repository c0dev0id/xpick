#include <X11/Xlib.h> 
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <stdio.h>


int main() {

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
    printf("#%02.2x%02.2x%02.2x\n",
            result.red/256, result.green/256, result.blue/256);
    return 0;
}

/* $Id$ */

#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "video.h"
#include "proto.h"
#include "ui.h"                         /* include user interface */
#include "ace/OS.h"

ACE_RCSID(mpeg_client, util32, "$Id$")

/*
 * Return a pointer to a full color bit visual on the dpy
 */
Visual *
FindFullColorVisual (Display *dpy, int *depth)
{
  XVisualInfo vinfo;
  XVisualInfo *vinfo_ret;
  int numitems, maxdepth;

#ifdef __cplusplus
  vinfo.c_class = TrueColor;
#else
  vinfo.class = TrueColor;
#endif
  vinfo_ret = XGetVisualInfo(dpy, VisualClassMask, &vinfo, &numitems);
  
  if (numitems == 0) return NULL;

  maxdepth = 0;
  while(numitems > 0) {
    if (vinfo_ret[numitems-1].depth > maxdepth) {
      maxdepth = vinfo_ret[numitems-1 ].depth;
    }
    numitems--;
  }
  XFree(vinfo_ret);

  if (maxdepth < 24) return NULL;

  if (XMatchVisualInfo(dpy, DefaultScreen(dpy), maxdepth, 
		       TrueColor, &vinfo)) {
    *depth = maxdepth;
    return vinfo.visual;
  }
  
  return NULL;
}

Window
CreateFullColorWindow (Display *dpy, int x, int y, int w, int h)
{
    int depth;
    Visual *visual;
    XSetWindowAttributes xswa;
    Window temp;
    unsigned int mask;
    unsigned int valclass;
    int screen;

    screen = XDefaultScreen(dpy);
    valclass = InputOutput;	/* Could be InputOnly */
    visual = FindFullColorVisual (dpy, &depth);
    if (visual == NULL) {
	return 0;
    }
    mask = CWBackPixel | CWColormap | CWBorderPixel;
    xswa.colormap = XCreateColormap(dpy, XRootWindow(dpy, screen),
		    visual, AllocNone);
    xswa.background_pixel = BlackPixel(dpy, DefaultScreen(dpy));
    xswa.border_pixel = WhitePixel(dpy, DefaultScreen(dpy));

    temp = XCreateWindow(dpy, MonitorWindow(), x, y, w, h,
	1, depth, valclass, visual, mask, &xswa);

    UISetwin(temp);

    return temp;
}

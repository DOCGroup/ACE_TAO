/* $Id$ */

/*
 * Copyright (c) 1992 The Regents of the University of California.
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice and the following
 * two paragraphs appear in all copies of this software.
 *
 * IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF
 * CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 */

/* Copyright (c) 1995 Oregon Graduate Institute of Science and Technology
 * P.O.Box 91000-1000, Portland, OR 97291, USA;
 * 
 * Permission to use, copy, modify, distribute, and sell this software and its 
 * documentation for any purpose is hereby granted without fee, provided that 
 * the above copyright notice appear in all copies and that both that 
 * copyright notice and this permission notice appear in supporting 
 * documentation, and that the name of O.G.I. not be used in advertising or 
 * publicity pertaining to distribution of the software without specific, 
 * written prior permission.  O.G.I. makes no representations about the 
 * suitability of this software for any purpose.  It is provided "as is" 
 * without express or implied warranty.
 * 
 * O.G.I. DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING 
 * ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL 
 * O.G.I. BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY 
 * DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN 
 * AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF 
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author: Shanwei Cen
 *         Department of Computer Science and Engineering
 *         email: scen@cse.ogi.edu
 */

#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
/*
#include <sys/types.h>
#include <sys/socket.h>
*/
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Intrinsic.h>

#ifdef SH_MEM  /* this is already defined in video.h  */
#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/extensions/XShm.h>
#endif

#include "include/common.h"
#include "newproto.h"
#include "global.h"
#include "dither.h"
#include "video.h"

#include <math.h>
#include "proto.h"
#include "ui.h"

ACE_RCSID(mpeg_client, vp, "$Id$")

/* Declaration of global variable to hold dither info. */

extern int ditherType;

/* Structures used by the X server. */

extern Display *display;
extern Colormap cmap;

static int cmdSocket = -1;
static int win_width, win_height;
static Window window;
static GC gc;

static int usingShm = 0;
#ifdef SH_MEM
static XShmSegmentInfo shminfo;               /* Segment info.  */
#endif
static XImage *ximage = NULL;


/*
 *--------------------------------------------------------------
 *
 * InitColor --
 *
 *	Initialized lum, cr, and cb quantized range value arrays.
 *
 * Results: 
 *      None.
 *
 * Side effects:
 *      None.
 *
 *--------------------------------------------------------------
 */

void
InitColor()
{
  int i;

  for (i=0; i<LUM_RANGE; i++) {
    lum_values[i] = ((i * 256) / (LUM_RANGE)) + (256/(LUM_RANGE*2));
  }

  for (i=0; i<CR_RANGE; i++) {
    cr_values[i] = ((i * 256) / (CR_RANGE)) + (256/(CR_RANGE*2));
  }

  for (i=0; i<CB_RANGE; i++) {
    cb_values[i] = ((i * 256) / (CB_RANGE)) + (256/(CB_RANGE*2));
  }

}


/*
 *--------------------------------------------------------------
 *
 * ConvertColor --
 *
 *	Given a l, cr, cb tuple, converts it to r,g,b.
 *
 * Results:
 *	r,g,b values returned in pointers passed as parameters.
 *
 * Side effects:
 *      None.
 *
 *--------------------------------------------------------------
 */

static void
ConvertColor(unsigned char l, unsigned char cr, unsigned char cb,
	     unsigned char *r, unsigned char *g, unsigned char *b)
{
  double fl, fcr, fcb, fr, fg, fb;

  fl = (double) l;
  fcr =  ((double) cr) - 128.0;
  fcb =  ((double) cb) - 128.0;


  fr = fl + (1.40200 * fcb);
  fg = fl - (0.71414 * fcb) - (0.34414 * fcr);
  fb = fl + (1.77200 * fcr);

  if (fr < 0.0) fr = 0.0;
  else if (fr > 255.0) fr = 255.0;

  if (fg < 0.0) fg = 0.0;
  else if (fg > 255.0) fg = 255.0;

  if (fb < 0.0) fb = 0.0;
  else if (fb > 255.0) fb = 255.0;

  *r = (unsigned char) fr;
  *g = (unsigned char) fg;
  *b = (unsigned char) fb;

}

#ifdef SH_MEM

int gXErrorFlag = 0;

int HandleXError(Display * dpy, XErrorEvent * event)
{
  gXErrorFlag = 1;
  return 0;
}

void InstallXErrorHandler()
{
  int HandleXError(Display *,XErrorEvent *);

  XSetErrorHandler(HandleXError);
  XFlush(display);
}

void DeInstallXErrorHandler()
{
  XSetErrorHandler(NULL);
  XFlush(display);
}
#endif

/*
 *--------------------------------------------------------------
 *
 * MakeWindow --
 *
 *	Create X Window for image display
 *
 * Results:
 *	Read the code.
 *
 * Side effects:
 *      None.
 *
 *--------------------------------------------------------------
 */

#ifdef SH_MEM
int CompletionType = -1;
#endif

// int XShmGetEventBase (Display*);

static void 
MakeWindow(void) 
{
  
  XSizeHints hint;
  unsigned int fg, bg;
  char *hello = "MPEG Play";
  int screen;

  if (ditherType == NO_DITHER) return;
/*
  UICreate();
*/

#ifdef SH_MEM
  if (shmemFlag && display != NULL)
  {
    if (!XShmQueryExtension(display))
    {
      shmemFlag = 0;
      fprintf(stderr, "VP: Shared memory not supported, Reverting to normal Xlib.\n");
    }
    else
    {
      shmemFlag = 1;
      /*
      fprintf(stderr, "VP: try using shared mem, but STOPPED temporarily.\n");
      shmemFlag = 0;
      */
    }
  }
  else if (shmemFlag)
  {
    fprintf(stderr, "VP unexpected: display is NULL.\n");
    ACE_OS::exit (1);
  }
  if(shmemFlag)
    CompletionType = /*XShmGetEventBase(display) +*/ ShmCompletion;
#endif

  screen = DefaultScreen (display);
  
  /* Fill in hint structure */

  hint.x = 0;
  hint.y = 0;
  hint.width = 150;
  hint.height = 150;
  hint.flags = PPosition | PSize;
  
  /* Get some colors */
  
  bg = WhitePixel (display, screen);
  fg = BlackPixel (display, screen);
  
  /* Make the window */
  
  if (ditherType == FULL_COLOR_DITHER) {
    window = CreateFullColorWindow (display, hint.x, hint.y, hint.width, hint.height);
    if (window == 0) {
      fprintf (stderr, "-color option only valid on full color display\n");
      exit (-1);
    }
  } else if (ditherType == MONO_DITHER || ditherType == MONO_THRESHOLD) {
    window = XCreateSimpleWindow (display,
				  MonitorWindow(),
				  hint.x, hint.y,
				  hint.width, hint.height,
				  4, fg, bg);
    UISetwin(window);
  } else {
    XVisualInfo vinfo;
    
    if (!XMatchVisualInfo (display, screen, 8, PseudoColor, 
			   &vinfo)) {

      if (!XMatchVisualInfo(display, screen, 8, GrayScale, 
			    &vinfo)) {
	fprintf(stderr, "It seems you have a true-color/mono monitor,\n");
	fprintf(stderr, "please try 'vcr -dither color' or 'vcr -dither mono'\n");
	exit(-1);
      }
    }

    window = XCreateSimpleWindow (display,
				 MonitorWindow(),
				 hint.x, hint.y,
				 hint.width, hint.height,
				 4, fg, bg);
    UISetwin(window);
  }
  
  XSelectInput(display, window, StructureNotifyMask);

  /* Tell other applications about this window */
  
  XSetStandardProperties (display, window, hello, hello, None, NULL, 0, &hint);
  
  /* Map window. */

  XMapWindow(display, window);

  /* Wait for map. */
  while(1) {
    XEvent	xev;

    XNextEvent(display, &xev);
    if(xev.type == MapNotify && xev.xmap.event == window)
      break;
  }
  XSelectInput(display, window, NoEventMask);
}
  

/*
 *--------------------------------------------------------------
 *
 * InitDisplay --
 *
 *	Initialized display, sets up colormap, etc.
 *
 * Results:
 *      None.
 *
 * Side effects:
 *      None.
 *
 *--------------------------------------------------------------
 */


static char window_name[64];
#include <string.h>

void InitDisplay(char *name)
{

  int ncolors = LUM_RANGE*CB_RANGE*CR_RANGE;
  XColor xcolor;
  int i, lum_num, cr_num, cb_num;
  unsigned char r, g, b;
  Colormap dcmap;

  if (ditherType == NO_DITHER) return;

  strcpy(window_name, name);
  
  MakeWindow();

  gc = XCreateGC(display, window, 0, 0);

  dcmap = cmap = XDefaultColormap(display, DefaultScreen(display));

  xcolor.flags = DoRed | DoGreen | DoBlue;

  retry_alloc_colors:
  for (i=0; i<ncolors; i++) {

    lum_num = (i / (CR_RANGE*CB_RANGE))%LUM_RANGE;
    cr_num = (i / CB_RANGE)%CR_RANGE;
    cb_num = i % CB_RANGE;

    ConvertColor(lum_values[lum_num], cr_values[cr_num], cb_values[cb_num], &r, &g, &b);

    xcolor.red = r * 256;
    xcolor.green = g * 256;
    xcolor.blue = b * 256;

    if(XAllocColor(display, cmap, &xcolor) == 0 && cmap == dcmap) {
      int j;
      unsigned long tmp_pixel;
      XWindowAttributes xwa;

      Fprintf(stderr, "Using private colormap.\n");

      /* Free colors. */
      for(j = 0; j < i; j ++) {
	tmp_pixel = pixel[j];
        XFreeColors(display, cmap, &tmp_pixel, 1, 0);
      }

      XGetWindowAttributes(display, window, &xwa);
      cmap = XCreateColormap(display, window, xwa.visual, AllocNone);
      XSetWindowColormap(display, window, cmap);
      UISetColormap();

      goto retry_alloc_colors;
    }
    pixel[i] = xcolor.pixel;
  }

  /*
  Fprintf(stderr, "Pixel values allocated for colormap:");
  for (i = 0; i < ncolors; i++) {
    if (!(i % 20)) Fprintf(stderr, "\n");
    Fprintf(stderr, "%3d ", pixel[i]);
  }
  */

  ximage = NULL;
}


/*
 *--------------------------------------------------------------
 *
 * InitGrayDisplay --
 *
 *	Initialized display for gray scale dither.
 *
 * Results:
 *      None.
 *
 * Side effects:
 *      None.
 *
 *--------------------------------------------------------------
 */

#define NUM_COLORS 128

void InitGrayDisplay(char *name)
{
  int ncolors = NUM_COLORS;
  XColor xcolor;
  int i;
  Colormap dcmap;

  MakeWindow();

  gc = XCreateGC(display, window, 0, 0);

  dcmap = cmap = XDefaultColormap(display, DefaultScreen(display));

  xcolor.flags = DoRed | DoGreen | DoBlue;

  retry_alloc_grays:
  for (i=0; i<ncolors; i++) {

    xcolor.red = (i*2) * 256;
    xcolor.green = (i*2) * 256;
    xcolor.blue = (i*2) * 256;

    if(XAllocColor(display, cmap, &xcolor) == 0 && cmap == dcmap) {
      int j;
      unsigned long tmp_pixel;
      XWindowAttributes xwa;

      Fprintf(stderr, "Using private colormap.\n");

      /* Free colors. */
      for(j = 0; j < i; j ++) {
	tmp_pixel = pixel[j*2];
        XFreeColors(display, cmap, &tmp_pixel, 1, 0);
      }

      XGetWindowAttributes(display, window, &xwa);
      cmap = XCreateColormap(display, window, xwa.visual, AllocNone);
      XSetWindowColormap(display, window, cmap);
      UISetColormap();

      goto retry_alloc_grays;
    }
    pixel[(i*2)] = xcolor.pixel;
    pixel[(i*2)+1] = xcolor.pixel;
  }

  ximage = NULL;
}


/*
 *--------------------------------------------------------------
 *
 * InitMonoDisplay --
 *
 *	Initialized display for monochrome dither.
 *
 * Results:
 *      None.
 *
 * Side effects:
 *      None.
 *
 *--------------------------------------------------------------
 */

void InitMonoDisplay(char *name)
{
  XGCValues xgcv;

  MakeWindow();

  xgcv.background = BlackPixel(display, DefaultScreen(display));
  xgcv.foreground = WhitePixel(display, DefaultScreen(display));

  gc = XCreateGC(display, window, GCForeground | GCBackground, &xgcv);

  ximage = NULL;
}


/*
 *--------------------------------------------------------------
 *
 * InitColorDisplay --
 *
 *	Initialized display for full color output.
 *
 * Results:
 *      None.
 *
 * Side effects:
 *      None.
 *
 *--------------------------------------------------------------
 */

void InitColorDisplay(char *name)
{

  MakeWindow();

  gc = XCreateGC(display, window, 0, 0);
  ximage = NULL;
}


static void ClearWindow(void)
{
  int size;
  char * data;
  if ((ditherType == Twox2_DITHER) || (ditherType == FULL_COLOR_DITHER))
    size = win_width * win_height * 4;
  else
    size = win_width * win_height;
  data = (char *)ACE_OS::malloc(size);
  if (data != NULL) {
    memset(data, 0, size);
    ximage->data = data;
    XPutImage(display, window, gc, ximage, 0, 0, 0, 0,
	      ximage->width, ximage->height);
    XFlush(display);
    ximage->data = NULL;
    ACE_OS::free (data);
  }
}


void VPresizeWindow(int h, int w)
{
  int i;
  char dummy;
  Visual *fc_visual;
  int depth;
  static int ph = 0;
  static int pw = 0;
  
  if (ph == h && pw == w) {
    ClearWindow();
    return;
  }
  ph = h;
  pw = w;
  
  /* Excerpt from man page of XDestroyImage(3X11):
     Note that when the image is created using XCreateImage(),
     XGetImage(), or XSubImage(), the destroy procedure that the
     XDestroyImage function call frees both the image structure
     and the data pointed to by the image structure. */
  if (ximage != NULL) ximage->data = NULL;
  
  if (ditherType == NO_DITHER) return;

  win_width = ((w + 15)>>4)<<4;
  win_height = ((h + 15)>>4)<<4;
  
  i = (ditherType == Twox2_DITHER) ? 2 : 1;
  XResizeWindow(display, window, w*i, h*i);
  UIMinsize(w*i, h*i);
  
  /* ximage allocate/resize */

#ifdef SH_MEM
  if (shmemFlag && display != NULL)
  {
    InstallXErrorHandler();
  }
#endif
       
  if (ximage != NULL)
  {
#ifdef SH_MEM
    if (shmemFlag && usingShm == 1)
    {
      XDestroyImage(ximage);
      ximage = NULL;
    }
    else
#endif
    {
      XDestroyImage(ximage);
      ximage = NULL;
    }
  }
#ifdef SH_MEM
  if (shmemFlag)
  {
    /*
    Visual *fc_visual;
    int depth;
    Visual *FindFullColorVisual();
    */
    usingShm = 1;

    if (ditherType == Twox2_DITHER)
    {
      ximage = XShmCreateImage(display, None, 8, ZPixmap, NULL,
				    &(shminfo), win_width * 2, win_height * 2);
    }
    else if (ditherType == FULL_COLOR_DITHER)
    {
      fc_visual = FindFullColorVisual(display, &depth);
      ximage = XShmCreateImage(display, fc_visual, depth, ZPixmap,
				    NULL, &(shminfo), win_width, win_height);
    }
    else if (ditherType == MONO_DITHER || ditherType == MONO_THRESHOLD)
    {
      ximage = XShmCreateImage(display, None, 1, XYBitmap,
				    NULL, &(shminfo), win_width, win_height);
    }
    else
    {
      ximage = XShmCreateImage(display, None, 8, ZPixmap, NULL,
				    &(shminfo), win_width, win_height);
    }
    if (ximage == NULL)
    {
      fprintf(stderr, "Shared memory error, disabling.\n");
      goto shmemerror;
    }

    if (gXErrorFlag)
    {
      XDestroyImage(ximage);
      ximage = NULL;
      fprintf(stderr, "Shared memory error, disabling.\n");
      gXErrorFlag = 0;
      goto shmemerror;
    }
  }
  else
#endif
  {
shmemerror:

    usingShm = 0;
  
    if (ditherType == Twox2_DITHER)
    {
      ximage = XCreateImage(display, None, 8, ZPixmap, 0, &dummy,
			    win_width * 2,
			    win_height * 2, 8, 0);
    }
    else if (ditherType == FULL_COLOR_DITHER)
    {
      fc_visual = FindFullColorVisual(display, &depth);
      ximage = XCreateImage (display, fc_visual, depth, ZPixmap,
			     0, &dummy, win_width,
			     win_height, 32, 0);
    }
    else if (ditherType == MONO_DITHER || ditherType == MONO_THRESHOLD)
    {
      ximage = XCreateImage (display, None, 1, XYBitmap, 0, &dummy,
			     win_width,
			     win_height, 8, 0);
      ximage->byte_order = MSBFirst;
      ximage->bitmap_bit_order = MSBFirst;
    }
    else
    {
      ximage = XCreateImage(display, None, 8, ZPixmap, 0, &dummy,
			    win_width,
			    win_height, 8, 0);
    }
    if (ximage == NULL)
    {
     ACE_OS::perror ("VP error: fails to XCreateImage ximage");
      ACE_OS::exit (1);
    }
  }
#ifdef SH_MEM
  if (shmemFlag && display != NULL)
  {
    DeInstallXErrorHandler();
  }
#endif
  ximage->data = NULL;
  /*
  fprintf(stderr, "win_width %d, win_height %d, times %d\n",
	  win_width, win_height, win_width * win_height);
  */
  ClearWindow();
  /*
  fprintf(stderr, "VP window resized as width-%d, height-%d.\n", w, h);
  */
}

/* parameter 'position' is for VPcontinuous command only */
void VPcommand(int cmd, char * ptr)
{
  FrameBlock *buf = (FrameBlock *)ptr;
  /*
  fprintf(stderr, "VP get cmd %d.\n", cmd);
  */
  switch (cmd)
  {
  case CmdVPdisplayFrame:
    if (shared->cmd != CmdSTOP) {
      shared->currentGroup = buf->gop;
      shared->currentFrame = buf->frame;
      shared->currentDisplay = buf->display;
      ximage->data = (char *)buf->data;
      VPexpose();
    }
    VDreclaimMsg(buf);
    return;
  
  case CmdVPinitScreen:
    
    VPresizeWindow(shared->verticalSize, shared->horizontalSize);
    return;
    
  case CmdVPclearScreen:
    
    ClearWindow();
    /*
    fprintf(stderr, "VP screen cleaned for audio-only case.\n");
    */
    break;
    
  default:  /* all unknown cmds shall be processed by UI */
  
    break;
  }
}

void VPexpose(void)
{
  if (ximage == NULL) {
    return;
  }
  else if (ximage->data == NULL) {
    return;
  }
#ifdef SH_MEM
  if (shmemFlag && usingShm)
  {
    XShmPutImage(display, window, gc, ximage, 0, 0, 0, 0,
		 ximage->width, ximage->height, True);
    XFlush(display);
    while(1)
    {
      XEvent xev;
      
      XNextEvent(display, &xev);
      if(xev.type == CompletionType)
        break;
    }
  }
  else 
#endif
  {
    XPutImage(display, window, gc, ximage, 0, 0, 0, 0,
	      ximage->width, ximage->height);
    XFlush(display);
  }
}

static void DisplayInitialization(void)
{
/*
  printf("to initialize display.\n");
*/  
  switch (ditherType) {
    
  case HYBRID_DITHER:
    
    InitColor();
    InitDisplay(displayName);
    break;
    
    case HYBRID2_DITHER:
    InitColor();
    InitDisplay(displayName);
    break;
    
  case FS4_DITHER:
    InitColor();
    InitDisplay(displayName);
    break;
    
  case FS2_DITHER:
    InitColor();
    InitDisplay(displayName);
    break;
    
  case FS2FAST_DITHER:
    InitColor();
    InitDisplay(displayName);
    break;
    
  case Twox2_DITHER:
    InitColor();
    InitDisplay(displayName);
    break;

  case GRAY_DITHER:
    InitGrayDisplay(displayName);
    break;

  case FULL_COLOR_DITHER:
    InitColorDisplay(displayName);
    break;

  case NO_DITHER:
    shmemFlag = 0;
    break;

  case ORDERED_DITHER:
    InitColor();
    InitDisplay(displayName);
    break;

  case MONO_DITHER:
  case MONO_THRESHOLD:
    InitMonoDisplay(displayName);
    break;

  case ORDERED2_DITHER:
    InitColor();
    InitDisplay(displayName);
    break;

  case MBORDERED_DITHER:
    InitColor();
    InitDisplay(displayName);
    break;

  }
/*
  printf("finished init display.\n");
*/
}

void VPinitWindow(Widget shell, Window monitorWindow, int cmdSock)
{
  cmdSocket = cmdSock;
  lum_values = (int *) ACE_OS::malloc(LUM_RANGE*sizeof(int));
  cr_values = (int *) ACE_OS::malloc(CR_RANGE*sizeof(int));
  cb_values = (int *) ACE_OS::malloc(CB_RANGE*sizeof(int));

  DisplayInitialization();
  ACE_OS::memcpy (shared->pixel, pixel, 256);
  shared->pixelValid = 1;
  
  ACE_OS::free (lum_values);
  ACE_OS::free (cr_values);
  ACE_OS::free (cb_values);
#ifdef SH_MEM
  if (shmemFlag && display != NULL)
  {
    InstallXErrorHandler();
    
    shminfo.shmid = shared->VDbufId;
    shminfo.shmaddr = shared->VDbufAddr;
    shminfo.readOnly = False;
    XShmAttach(display, &(shminfo));
    XSync(display, False);
    
    if (gXErrorFlag)
    {
      Fprintf(stderr, "Shared memory error, disabling.\n");
      gXErrorFlag = 0;
      shmemFlag = 0;
    }
    DeInstallXErrorHandler();
  }
#endif
  
  remove_shmem_id(shared->VDbufId);
  VPresizeWindow(150, 200);
}

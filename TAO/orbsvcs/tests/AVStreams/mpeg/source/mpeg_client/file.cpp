/* $Id$ */

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
#include <unistd.h>
#include <stdlib.h>
#include "video.h"
#include <X11/Intrinsic.h>
#include <Xm/Xm.h>
#include <Xm/Text.h>
#include <Xm/FileSB.h>
#include <Xm/DialogS.h>
#if defined(sun) || defined(FreeBSD)
#include <stdlib.h>
#endif
#include "include/common.h"
#include "newproto.h"
#include "global.h"

ACE_RCSID(mpeg_client, file, "$Id$")

#ifdef XmFONTLIST_DEFAULT_TAG
#define XmStringTag XmFONTLIST_DEFAULT_TAG
#else
#define XmStringTag XmSTRING_DEFAULT_CHARSET
#endif
#define BUFSIZE	256

static Widget       fileshell;

void StartLocalServer(void)
{
  int res;
  
  static int startLocalServer = 1;
  
  if (startLocalServer) {
    startLocalServer = 0;
    if (access(VCR_UNIX_PORT, 0) == -1) { /* going to start a server */
      fprintf(stderr, "Trying to fork a server on local host...\n");
      res = system("vcrs -rt 2> /dev/null& exit");
      if (shared->config.verbose && res == -1) {
	perror("Error on fork server on local host");
      }
      usleep(1000000);
    }
  }
}

/* hostname == "" defaults to local host */
static void StartFile(char *hostname, char *filename)
{
  char *title = (char *)ACE_OS::malloc(strlen(hostname) +ACE_OS::strlen (filename) + 10);
  char buf[BUFSIZE];
  int len;

  if (*hostname == 0) StartLocalServer();

  if (*hostname == 0) {
    get_full_path(filename, buf, BUFSIZE);
  }
  else {
    strncpy(buf, filename, BUFSIZE);
    buf[BUFSIZE-1] = 0;
  }
  len =ACE_OS::strlen (buf);

  if (title == NULL) title = filename;
  else sprintf(title, "%s:%s", hostname, filename);

  if (!strcasecmp(".mpg", buf+len-4) || !strcasecmp(".mpeg", buf+len-5))
    StartProgram(title, hostname, buf, "", "");
  else if (!strcasecmp(".au", buf+len-3))
    StartProgram(title, "", "", hostname, buf);
  else
    fprintf(stderr, "Error: type of file %s not known (know only .mpeg .mpg, .au).\n",
	    filename);
  if (title != filename) ACE_OS::free (title);
}

static void dismissCB(Widget W, XtPointer closure, XtPointer call_data)
{

  XtUnmanageChild (fileshell);
}

static void playCB(Widget w, XtPointer closure, XtPointer call_data)
{
  char *filename = NULL;       /* String containing file name         */
  XmStringCharSet charset = (XmStringCharSet) XmSTRING_DEFAULT_CHARSET;
  XmFileSelectionBoxCallbackStruct *fcb =
         (XmFileSelectionBoxCallbackStruct *) call_data;
  
  XmStringGetLtoR(fcb->value, charset, &filename);

  if (filename == NULL)
  {
    fprintf(stderr, "Error: failed to get file name to play.\n");
    return;
  }
  StartFile("", filename);
  XmStringFree((XmString)filename);
}

Widget CreateFileWindow(Widget parent, int cmdSock, int * playflag, Widget frametext)
{
  Arg          args[10];    /*  arg list             */
  register     int n;       /*  arg count            */
  XmString okString, cancelString;

/*  Create the file selection box */
  okString = (XmString)XmStringCreateLtoR("Select", XmStringTag);
  cancelString = (XmString)XmStringCreateLtoR("Dismiss", XmStringTag);
  n = 0;
  XtSetArg(args[n], XmNtitle, "Virtual VCR File Selection"); n++;
  XtSetArg(args[n], XmNokLabelString, okString); n++;
  XtSetArg(args[n], XmNcancelLabelString, cancelString); n++;
  fileshell = XmCreateFileSelectionDialog(parent, "filebox", args, n);
  XmStringFree(okString);
  XmStringFree(cancelString);
  XtAddCallback (fileshell, XmNokCallback, (XtCallbackProc)playCB, NULL);
  XtAddCallback (fileshell, XmNcancelCallback, (XtCallbackProc)dismissCB, NULL);
  
  return fileshell;
}

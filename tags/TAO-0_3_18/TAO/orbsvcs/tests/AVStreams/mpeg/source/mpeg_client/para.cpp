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
#include <stdlib.h>
#include "video.h"
#include <X11/Intrinsic.h>
#include <Xm/DialogS.h>
#include <Xm/List.h>
#include <Xm/Frame.h>
#include <Xm/PushB.h>
#include <Xm/Form.h>
#include <Xm/Text.h>
#include <Xm/TextF.h>
#include <Xm/Label.h>
#include "include/common.h"
#include "newproto.h"
#include "global.h"

ACE_RCSID(mpeg_client, para, "$Id$")

#define NUM_LIST_ITEMS 25
#define PARAMETER_FILE "~/.vcr/vcrParameters"
#define BANNER "Virtual VCR Parameter File, version 2.0  ** Do not Edit **"
#ifdef XmFONTLIST_DEFAULT_TAG
#define XmStringTag XmFONTLIST_DEFAULT_TAG
#else
#define XmStringTag XmSTRING_DEFAULT_CHARSET
#endif
#define STRING_SIZE 64
#define ITEMS sizeof(shared->config)/4

static Widget parashell, wparalist, wparafield;

static int * config;
static float *fconfig;

static struct
{
  char * title;
  int float_tag;  /* 0 - integer, 1 - floating point */
}
para[] =
{
  {"Real time (audio on)(tag)", 0},
  {"Video max frames-per-second", 0},
  {"Audio max sampels-per-second", 0},
  {"FF frames-per-second", 0},
  {"Rewind frames-per-second", 0},
  {"Feedback delay (msec)", 0},
  {"Audio output mask", 0},
  {"Audio_para.encodeType", 0},
  {"Audio_para.channels", 0},
  {"Audio_para.samplesPerSecond", 0},
  {"Audio_para.bytesPerSamples", 0},
  {"Audio timer interval (millisec)", 0},
  {"Audio buffered intervals", 0},
  {"Frames per audio play", 0},
  {"Audio forward (samples)", 0},
  {"VS work-ahead (milliseconds)", 0},
  {"frame rate limit (fps, float)", 1},
  {"collect statistics(tag)", 0},
  {"collect video structure info(tag)", 0},
  {"Sync effective(tag)", 0},
  {"QoS effective(tag)", 0},
  {"Audio offset(samples)", 0},
  {"Filter parameter(1/R or nsamples)", 0},
  {"Max send pattern frames", 0},
  {"Reliable byte-stream audio (tag)", 0},
  {"Reliable byte-stream video (tag)", 0},
  {"Verbose message(tag)", 0},
  {"", 0} /* this empty string is needed for testing the size of para */
};

#define Fgets(s) {if (feof(fp) || fgets(s, STRING_SIZE, fp) == NULL) break;  s[strlen(s)-1] = 0; }

#define Fputs(s) {fputs(s, fp); fputc('\n', fp);}

static void InitBuf(void)
{
  FILE * fp;
  char buf[256];
  int i;

  config = (int *)&(shared->config);
  fconfig = (float *)&(shared->config);

  get_full_path(PARAMETER_FILE, buf, 256);
  fp = fopen(buf, "r");
  if (fp == NULL)
  {
    fprintf(stderr, "Warning: %s not found, use default parameter value\n",
	    PARAMETER_FILE);
  InitBuf_exit1:
    return;
  }
  if (fgets(buf, 100, fp) == NULL)
  {
    fclose(fp);
    goto InitBuf_exit1;
  }
  buf[strlen(buf)-1] = 0;
  if (ACE_OS::strcmp (buf, BANNER))
  {
    fprintf(stderr, "BANNER in %s not expected, use default parameter value\n",
	    PARAMETER_FILE);
    fclose(fp);
    goto InitBuf_exit1;
  }
  for(i = 0; i < ITEMS; i++)
  {
    Fgets(buf);
    if (para[i].float_tag)
      sscanf(buf, "%f", &fconfig[i]);
    else
      sscanf(buf, "%d", &config[i]);
  }
  fclose(fp);
}

static void SaveBuf(void)
{
  int i;
  FILE * fp;
  char buf[256];
  get_full_path(PARAMETER_FILE, buf, 256);
  fp = fopen(buf, "w");
  if (fp == NULL)
  {
    fprintf(stderr, "Fail to open %s for saving para", buf);
   ACE_OS::perror ("");
    return;
  }
  Fputs(BANNER);
  for (i = 0; i < ITEMS; i++)
    if (para[i].float_tag)
      fprintf(fp, "%f\n", fconfig[i]);
    else
      fprintf(fp, "%d\n", config[i]);
  fclose(fp);
}


static int curListPos(void)
{
  int * poses, count;
  
  if (XmListGetSelectedPos(wparalist, &poses, &count))
  {
    count = poses[0];
    XtFree((char*)poses);
    /*
    fprintf(stderr, "curListPos: current pos = %d\n", count);
    */
    return count - 1;
  }
  return -1;
}

static void modifyCB(Widget w, XtPointer closure, XtPointer call_data)
{
  int i, value;
  XmString item;
  char buf[100], * valptr;
  i = curListPos();
  if (i == -1) { beep(); return;}
  valptr = XmTextGetString(wparafield);
  if (para[i].float_tag)
    sscanf(valptr, "%f", &fconfig[i]);
  else
    sscanf(valptr, "%d", &config[i]);
  XtFree(valptr);
  if (para[i].float_tag)
    sprintf(buf, "%s: %f", para[i].title, fconfig[i]);
  else
    sprintf(buf, "%s: %d", para[i].title, config[i]);
  item = (XmString)XmStringCreateLtoR(buf, XmStringTag);
  XmListReplaceItemsPos(wparalist, &item, 1, i+1);
  XmListSelectPos(wparalist, i+1, 0);
  XmStringFree(item);
  SaveBuf();
  /*
  XtManageChild(parashell);
  */
  return;
}

static void dismissCB(Widget W, XtPointer closure, XtPointer call_data)
{
  
  XtUnrealizeWidget(parashell);
  /*
  XtUnmanageChild(parashell);
  */
}

Widget CreateParameterWindow(Widget parent)
{
  Arg   	args[20];
  int		n;
  Widget 	wform, frame1, frame2, wlabel,
                wmodify, wdismiss;
  XmFontList fontlist;
  XFontStruct * font;
  XmStringCharSet cset = XmSTRING_DEFAULT_CHARSET;

  InitBuf();

  n = 0;
  XtSetArg(args[n], XmNtitle, "Virtual VCR Parameter List"); n++;
  XtSetArg(args[n], XmNdeleteResponse, XmDO_NOTHING);n++;
  XtSetArg(args[n], XmNkeyboardFocusPolicy, XmPOINTER); n++;
  XtSetArg(args[n], XmNwidth, 350); n++;
  XtSetArg(args[n], XmNheight, 400); n++;
  XtSetArg(args[n], XmNminWidth, 250); n++;
  XtSetArg(args[n], XmNminHeight, 150); n++;
  
  parashell = XtAppCreateShell("Parameter List", "virtual_vcr",
			       topLevelShellWidgetClass, display, args, n);
  /*
  parashell = XmCreateDialogShell(parent, "virtual_vcr", args, n);
  */

  font = XLoadQueryFont (XtDisplay (parashell), "courB14");
  fontlist = XmStringCreateFontList (font, cset);

  n = 0;
  XtSetArg(args[n], XmNtitle, "Virtual Parameter List"); n++;
  wform  = XmCreateForm(parashell, "Parameters", args, n);
  XtManageChild(wform);
  /*
  parashell = wform  = XmCreateForm(parashell, "Parameters", args, n);
  */
  
  n = 0;
  wmodify = XmCreatePushButton (wform, "Update", args, n);
  XtManageChild(wmodify);
  XtAddCallback (wmodify, XmNactivateCallback, (XtCallbackProc)modifyCB, NULL);
  wdismiss = XmCreatePushButton (wform, "Dismiss", args, n);
  XtManageChild(wdismiss);
  XtAddCallback (wdismiss, XmNactivateCallback, (XtCallbackProc)dismissCB, NULL);

  n=0;
  XtSetArg (args[n], XmNwidth, 60); n++;
  XtSetArg (args[n], XmNheight, 28); n++;
  XtSetArg (args[n], XmNleftOffset, 20); n++;
  XtSetArg (args[n], XmNbottomOffset, 5); n++;
  XtSetArg (args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg (args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetValues (wmodify, args, n);

  n = 0;
  XtSetArg(args[n], XmNcolumns, 10); n++;
  XtSetArg (args[n], XmNleftOffset, 90); n++;
  XtSetArg (args[n], XmNbottomOffset, 5); n++;
  XtSetArg (args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg (args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  wparafield = XmCreateTextField(wform, "updata_text", args, n);
  XtManageChild(wparafield);
  
  n=0;
  XtSetArg (args[n], XmNwidth, 60); n++;
  XtSetArg (args[n], XmNheight, 28); n++;
  XtSetArg (args[n], XmNrightOffset, 20); n++;
  XtSetArg (args[n], XmNbottomOffset, 5); n++;
  XtSetArg (args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg (args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetValues (wdismiss, args, n);

  n = 0;
  XtSetArg (args[n], XmNtopOffset, 5); n++;
  XtSetArg (args[n], XmNleftOffset, 5); n++;
  XtSetArg (args[n], XmNbottomOffset, 45); n++;
  XtSetArg (args[n], XmNrightOffset, 5); n++;
  XtSetArg (args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg (args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg (args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg (args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNfontList,  fontlist); n++;
  XtSetArg(args[n], XmNvisibleItemCount,  NUM_LIST_ITEMS); n++;

  {
    int items;
    XmString * item;
    int i;
    /*
    fprintf(stderr, "Total parameter items: %d\n", items);
    */
    for (items = 0; para[items].title[0] != 0; items ++);
    if (items > ITEMS)
      items = ITEMS;
    else if (items < ITEMS)
    {
      fprintf(stderr, "Error in para.c: fewer titles than parameters, %d out of %d.\n",
	      items, ITEMS);
      ACE_OS::exit (1);
    }
    item = (XmString *)ACE_OS::malloc(sizeof(*item) * items);
    if (item == NULL)
    {
     ACE_OS::perror ("UI allocate parameter list item(XmString)");
      ACE_OS::exit (1);
    }
    for (i = 0; i < items; i ++)
    {
      char buf[100];
      if (para[i].float_tag)
        sprintf(buf, "%s: %f", para[i].title, fconfig[i]);
      else
        sprintf(buf, "%s: %d", para[i].title, config[i]);
      item[i] = (XmString)XmStringCreateLtoR(buf, XmStringTag);
    }
    XtSetArg(args[n], XmNitems, item); n++;
    XtSetArg(args[n], XmNitemCount, items); n++;
    XtSetArg(args[n], XmNselectedItems,  item); n++;
    XtSetArg(args[n], XmNselectedItemCount,  1); n++;
    wparalist = XmCreateScrolledList(wform, "ParameterList", args, n);
    XtManageChild(wparalist);
    for (i = 0; i < items; i++)
      XmStringFree(item[i]);
    ACE_OS::free (item);
  }
  XmFontListFree(fontlist);

  n=0;
  XtSetArg (args[n], XmNtopOffset, 5); n++;
  XtSetArg (args[n], XmNleftOffset, 5); n++;
  XtSetArg (args[n], XmNbottomOffset, 45); n++;
  XtSetArg (args[n], XmNrightOffset, 5); n++;
  XtSetArg (args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg (args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg (args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg (args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  frame2 = XmCreateFrame (wform, "frame", args, n);
  XtManageChild(frame2);

  n=0;
  XtSetArg (args[n], XmNtopOffset, 0); n++;
  XtSetArg (args[n], XmNleftOffset, 0); n++;
  XtSetArg (args[n], XmNbottomOffset, 0); n++;
  XtSetArg (args[n], XmNrightOffset, 0); n++;
  XtSetArg (args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg (args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg (args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg (args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  frame1 = XmCreateFrame (wform, "frame", args, n);
  XtManageChild(frame1);
  
  return parashell;
}

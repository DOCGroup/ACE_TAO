/* $Id$ */

/* Copyright (c) 1994 The Geometry Center; University of Minnesota
   1300 South Second Street;  Minneapolis, MN  55454, USA;

This file is free software; you can redistribute it and/or modify it only
under the the terms of the GNU GENERAL PUBLIC LICENSE which should be
included along with this file.  This software may be obtained via anonymous
ftp from geom.umn.edu; email: software@geom.umn.edu. */

/* Author: Daeron Meyer */

/* This software is modified on January 15, 1995 by:
 *
 *         Shanwei Cen
 *         Department of Computer Science and Engineering
 *         email: scen@cse.ogi.edu
 */

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <X11/IntrinsicP.h>
#include <Xm/DialogS.h>
#include "mpeg_mib/mibload.h"
#include "interface/fb.xbm"
#include "interface/play.xbm"
#include "interface/step.xbm"
#include "interface/stop.xbm"
#include "interface/loop.xbm"
#include "interface/ff.xbm"
#include "interface/MainWindow.mib"
#include "interface/Info.mib"
#include "video.h"

#include "include/common.h"
#include "newproto.h"
#include "global.h"
#include "ace/OS.h"

ACE_RCSID(mpeg_client, ui, "$Id$")

int cmdBusy = 0;

extern char infomessage[];

/*****************************************************************************/

/* Private Variables and Methods */

static int playtag = 0;
static int cmdSocket = -1;

/* Intrinsics Application Context */
static  XtAppContext		App;
static  Window			monitorwindow = (int)NULL;
static  mib_Widget		*mainload, *infoload;
static  Widget			shell = (int)NULL, infoshell = (int)NULL,
                                parashell = (int)NULL, progshell = (int)NULL,
                                fileshell = (int)NULL;
static  Widget			frametext= (int)NULL;
static  Widget			volumescale = (int)NULL, balancescale = (int)NULL,
                                speedscale = (int)NULL, positionscale = (int)NULL;
static  int			frameshown = 0;

#define MINX 500
#define MINY 360

static  Window displaywin       = (int)NULL;
static  int privcolormap	= 0;

static void exit_callback(Widget, XtPointer, XmAnyCallbackStruct *);
static void info_callback(Widget, XtPointer, XmAnyCallbackStruct *);
static void para_callback(Widget, XtPointer, XmAnyCallbackStruct *);
static void prog_callback(Widget, XtPointer, XmAnyCallbackStruct *);
static void file_callback(Widget, XtPointer, XmAnyCallbackStruct *);

static void loopchange_callback(Widget, XtPointer, XmAnyCallbackStruct *);
static void normal_callback(Widget, XtPointer, XmAnyCallbackStruct *);
static void fb_callback(Widget, XtPointer, XmAnyCallbackStruct *);
static void stop_callback(Widget, XtPointer, XmAnyCallbackStruct *);
static void ff_callback(Widget, XtPointer, XmAnyCallbackStruct *);
static void step_callback(Widget, XtPointer, XmAnyCallbackStruct *);
static void play_callback(Widget, XtPointer, XmAnyCallbackStruct *);

static void volume_callback(Widget, XtPointer, XmAnyCallbackStruct *);
static void balance_callback(Widget, XtPointer, XmAnyCallbackStruct *);
static void speed_callback(Widget, XtPointer, XmAnyCallbackStruct *);
static void position_callback(Widget, XtPointer, XmAnyCallbackStruct *);

static void cmdSocket_callback(Widget, XtPointer, XmAnyCallbackStruct *);

static void monitor_expose(Widget, XtPointer, XmDrawingAreaCallbackStruct *);

static void infoclose_callback(Widget, XtPointer, XmAnyCallbackStruct *);

static void CmdWrite(char * buf, int size)
{
  //  ACE_DEBUG ((LM_DEBUG,"(%P|%t)before writing cmd\n"));
  while (ACE_OS::write(cmdSocket, (buf), (size)) == -1)
  {
    if (errno == EINTR) continue;
    perror("UI write to cmdSocket");
    exit(1);
  }
  //  ACE_DEBUG ((LM_DEBUG,"(%P|%t)After writing cmd\n"));
}

/*****************************************************************************

  UICreate: Create/load in a motif interface.

 *****************************************************************************/

static void UICreate()
{

  Widget		toplevel;

  Widget 		mainwindow,
			temp;

  Pixel			fg, bg;
  Pixmap		button_pix;

  XmFontList fontlist;
  XFontStruct * font;

  Arg			args[20];
  int			n, depth;

  static String		fallbacks[] = {
   "*Foreground:  black",
  "*BorderWidth: 0",
  "*Background:  lavender",
  "*XmToggleButton.selectColor:         yellow",
  "*XmToggleButton.indicatorSize:       16",
  "*XmToggleButtonGadget.selectColor:   yellow",
  "*XmToggleButtonGadget.indicatorSize: 16",
  "*fontList: -adobe-helvetica-medium-r-normal--14-*-*-*-p-*-iso8859-1",
  "*XmText*fontList: -adobe-courier-medium-r-normal--12-*-*-*-m-70-iso8859-1",
  NULL
  };

/*****************************************************************************/

  n = 0;

  toplevel = XtAppInitialize(&App, "virtual_vcr", NULL, 0, &argc_share,
				argv_share, fallbacks, args, n);

  display = XtDisplay(toplevel);

/******************************************************************************/

  n = 0;
  XtSetArg(args[n], XmNtitle, "TAO Mpeg Player"); n++;
  XtSetArg(args[n], XmNdeleteResponse, XmDO_NOTHING);n++;
  XtSetArg(args[n], XmNkeyboardFocusPolicy, XmPOINTER); n++;
  XtSetArg(args[n], XmNwidth, MINX); n++;
  XtSetArg(args[n], XmNminWidth, MINX); n++;
  XtSetArg(args[n], XmNheight, MINY); n++;
  XtSetArg(args[n], XmNminHeight, MINY); n++;

  shell = XtAppCreateShell("TAO Mpeg Player", "virtual_vcr",
				topLevelShellWidgetClass, display, args, n);

/*****************************************************************************/

  n = 0;
  XtSetArg(args[n], XmNtitle, "TAO Mpeg Player"); n++;

  mainwindow = XmCreateForm(shell, "MainForm", args, n);
  XtManageChild(mainwindow);
  {
   XmStringCharSet cset = XmSTRING_DEFAULT_CHARSET;
   XmString titleString;

   font = XLoadQueryFont (XtDisplay (shell), "courB14");
   fontlist = XmStringCreateFontList (font, cset);


   titleString = XmStringCreateLtoR ("Balance", cset);
   n = 0;
   XtSetArg (args[n], XmNfontList, fontlist);	n++; 
   XtSetArg (args[n], XmNshowValue, True);	n++;
   XtSetArg (args[n], XmNtitleString, titleString); n++;
   XtSetArg (args[n], XmNorientation, XmHORIZONTAL); n++;
   XtSetArg (args[n], XmNmaximum, 100); n++;
   XtSetArg (args[n], XmNvalue, 50); n++;
   XtSetArg (args[n], XmNprocessingDirection, XmMAX_ON_RIGHT); n++;
   XtSetArg (args[n], XmNscaleHeight, 17);n++;
   XtSetArg (args[n], XmNscaleWidth, 100);n++;
   XtSetArg (args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
   XtSetArg (args[n], XmNbottomOffset, 4);n++;
   XtSetArg (args[n], XmNleftAttachment, XmATTACH_FORM); n++;
   XtSetArg (args[n], XmNleftOffset, 8);n++;
   balancescale = XmCreateScale(mainwindow, "scale", args, n); 
   XmStringFree (titleString);
   XtManageChild (balancescale);
   XtAddCallback (balancescale, XmNvalueChangedCallback, (XtCallbackProc)balance_callback, NULL);
   XtAddCallback (balancescale, XmNdragCallback,         (XtCallbackProc)balance_callback, NULL);


   titleString = XmStringCreateLtoR ("Volume", cset);
   n = 0;
   XtSetArg (args[n], XmNfontList, fontlist);	n++; 
   XtSetArg (args[n], XmNshowValue, True);	n++;
   XtSetArg (args[n], XmNtitleString, titleString); n++;
   XtSetArg (args[n], XmNorientation, XmHORIZONTAL); n++;
   XtSetArg (args[n], XmNmaximum, 100); n++;
   XtSetArg (args[n], XmNvalue, DEFAULT_volume); n++;
   XtSetArg (args[n], XmNprocessingDirection, XmMAX_ON_RIGHT); n++;
   XtSetArg (args[n], XmNscaleHeight, 17);n++;
   XtSetArg (args[n], XmNscaleWidth, 100);n++;
   XtSetArg (args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
   XtSetArg (args[n], XmNbottomOffset, 4);n++;
   XtSetArg (args[n], XmNleftAttachment, XmATTACH_FORM); n++;
   XtSetArg (args[n], XmNleftOffset, 128);n++;
   volumescale = XmCreateScale(mainwindow, "scale", args, n); 
   XmStringFree (titleString);
   XtManageChild (volumescale);
   XtAddCallback (volumescale, XmNvalueChangedCallback, (XtCallbackProc)volume_callback, NULL);
   XtAddCallback (volumescale, XmNdragCallback,         (XtCallbackProc)volume_callback, NULL);


   titleString = XmStringCreateLtoR ("Play Speed", cset);
   n = 0;
   XtSetArg (args[n], XmNfontList, fontlist);	n++; 
   XtSetArg (args[n], XmNshowValue, True);	n++;
   XtSetArg (args[n], XmNtitleString, titleString); n++;
   XtSetArg (args[n], XmNorientation, XmHORIZONTAL); n++;
   XtSetArg (args[n], XmNmaximum, 100); n++;
   XtSetArg (args[n], XmNvalue, 50); n++;
   XtSetArg (args[n], XmNprocessingDirection, XmMAX_ON_RIGHT); n++;
   XtSetArg (args[n], XmNscaleHeight, 17);n++;
   XtSetArg (args[n], XmNscaleWidth, 100);n++;
   XtSetArg (args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
   XtSetArg (args[n], XmNbottomOffset, 4);n++;
   XtSetArg (args[n], XmNleftAttachment, XmATTACH_FORM); n++;
   XtSetArg (args[n], XmNleftOffset, 248);n++;
   speedscale = XmCreateScale(mainwindow, "scale", args, n); 
   XmStringFree (titleString);
   XtManageChild (speedscale);
   XtAddCallback (speedscale, XmNvalueChangedCallback, (XtCallbackProc)speed_callback, NULL);
   XtAddCallback (speedscale, XmNdragCallback,         (XtCallbackProc)speed_callback, NULL);


   titleString = XmStringCreateLtoR ("Position", cset);
   n = 0;
   XtSetArg (args[n], XmNfontList, fontlist);	n++; 
   XtSetArg (args[n], XmNshowValue, True);	n++;
   XtSetArg (args[n], XmNtitleString, titleString); n++;
   XtSetArg (args[n], XmNorientation, XmHORIZONTAL); n++;
   XtSetArg (args[n], XmNmaximum, POSITION_RANGE); n++;
   XtSetArg (args[n], XmNprocessingDirection, XmMAX_ON_RIGHT); n++;
   XtSetArg (args[n], XmNscaleHeight, 17);n++;
   XtSetArg (args[n], XmNscaleWidth, 100);n++;
   XtSetArg (args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
   XtSetArg (args[n], XmNbottomOffset, 4);n++;
   XtSetArg (args[n], XmNleftAttachment, XmATTACH_FORM); n++;
   XtSetArg (args[n], XmNleftOffset, 368);n++;
   positionscale = XmCreateScale(mainwindow, "scale", args, n); 
   XmStringFree (titleString);
   XtManageChild (positionscale);
   XtAddCallback (positionscale, XmNvalueChangedCallback, (XtCallbackProc)position_callback, NULL);
   XtAddCallback (positionscale, XmNdragCallback,         (XtCallbackProc)position_callback, NULL);

  }

  mainload = mib_load_interface(mainwindow, MainMib, MI_FROMSTRING);
  XtSetMappedWhenManaged(shell, False);
  XtRealizeWidget(shell);

  frametext = mib_find_name(mainload, "TitleText")->me;
  XtVaSetValues(frametext,
		XmNfontList, fontlist,
		XmNcursorPositionVisible, False,
		XmNeditable, False,
		NULL);

  temp = mib_find_name(mainload, "Monitor")->me;
  XtAddCallback(temp, XmNexposeCallback,
			(XtCallbackProc) monitor_expose, (XtPointer) NULL);
  XtVaSetValues(temp,   XmNforeground,
			BlackPixel(display, DefaultScreen(display)),
			XmNbackground,
			BlackPixel(display, DefaultScreen(display)),
                        NULL);
  monitorwindow = XtWindow(temp);

  temp = mib_find_name(mainload, "Info")->me;
  XtAddCallback(temp, XmNactivateCallback, (XtCallbackProc) info_callback,
			(XtPointer) NULL);

  temp = mib_find_name(mainload, "Para")->me;
  XtAddCallback(temp, XmNactivateCallback, (XtCallbackProc) para_callback,
			(XtPointer) NULL);

  temp = mib_find_name(mainload, "Prog")->me;
  XtAddCallback(temp, XmNactivateCallback, (XtCallbackProc) prog_callback,
			(XtPointer) NULL);

  temp = mib_find_name(mainload, "File")->me;
  XtAddCallback(temp, XmNactivateCallback, (XtCallbackProc) file_callback,
			(XtPointer) NULL);

  temp = mib_find_name(mainload, "Exit")->me;
  XtAddCallback(temp, XmNactivateCallback, (XtCallbackProc) exit_callback,
			(XtPointer) NULL);
  
  temp = mib_find_name(mainload, "Loop")->me;
  XtVaGetValues(temp, XmNforeground, &fg,
                        XmNbackground, &bg, XmNdepth, &depth, NULL);
  button_pix = XCreatePixmapFromBitmapData(display, DefaultRootWindow(display),
                (char *)loop_bits, loop_width, loop_height, fg, bg, depth);
  /*
  {
    int i;
    for (i = 0; i < sizeof(loop_bits); i++)
      loop_bits[i] = ~loop_bits[i];
  }
  */
  XtVaSetValues(temp,
		XmNshadowThickness, 2,

                XmNwidth, 45,
		XmNheight, 28,
		
		XmNindicatorType, XmN_OF_MANY,
		
		XmNindicatorOn, False,
		
		XmNfillOnSelect, True,
		XmNlabelType, XmPIXMAP,
		XmNhighlightThickness, 0,
		XmNrubberPositioning, False,
                XmNlabelPixmap, button_pix,
		XmNselectPixmap, button_pix,
/*
  XmNselectPixmap, (XCreatePixmapFromBitmapData(display, DefaultRootWindow(display),
                               (char *)loop_bits, loop_width, loop_height, fg, bg, depth)),
*/
		NULL);
  XtAddCallback(temp, XmNvalueChangedCallback, (XtCallbackProc) loopchange_callback,
			(XtPointer) NULL);
/*
   XtAddCallback(temp, XmNarmCallback, (XtCallbackProc) looparm_callback,
			(XtPointer) NULL);
  XtAddCallback(temp, XmNdisarmCallback, (XtCallbackProc) loopdisarm_callback,
			(XtPointer) NULL);
*/
  temp = mib_find_name(mainload, "Normal")->me;
  XtAddCallback(temp, XmNactivateCallback, (XtCallbackProc) normal_callback,
			(XtPointer) NULL);

  temp = mib_find_name(mainload, "Rewind")->me;
  XtVaGetValues(temp, XmNforeground, &fg,
                        XmNbackground, &bg, XmNdepth, &depth, NULL);
  button_pix = XCreatePixmapFromBitmapData(display, DefaultRootWindow(display),
                (char *)fb_bits, fb_width, fb_height, fg, bg, depth);
  XtVaSetValues(temp, XmNlabelType, XmPIXMAP,
                        XmNlabelPixmap, button_pix, NULL);
  XtAddCallback(temp, XmNactivateCallback, (XtCallbackProc) fb_callback,
			(XtPointer) NULL);

  temp = mib_find_name(mainload, "Stop")->me;
  XtVaGetValues(temp, XmNforeground, &fg,
                        XmNbackground, &bg, XmNdepth, &depth, NULL);
  button_pix = XCreatePixmapFromBitmapData(display, DefaultRootWindow(display),
                (char *)stop_bits, stop_width, stop_height, fg, bg, depth);
  XtVaSetValues(temp, XmNlabelType, XmPIXMAP,
                        XmNlabelPixmap, button_pix, NULL);
  XtAddCallback(temp, XmNactivateCallback, (XtCallbackProc) stop_callback,
			(XtPointer) NULL);

  temp = mib_find_name(mainload, "FF")->me;
  XtVaGetValues(temp, XmNforeground, &fg,
                        XmNbackground, &bg, XmNdepth, &depth, NULL);
  button_pix = XCreatePixmapFromBitmapData(display, DefaultRootWindow(display),
                (char *)ff_bits, ff_width, ff_height, fg, bg, depth);
  XtVaSetValues(temp, XmNlabelType, XmPIXMAP,
                        XmNlabelPixmap, button_pix, NULL);
  XtAddCallback(temp, XmNactivateCallback, (XtCallbackProc) ff_callback,
			(XtPointer) NULL);

  temp = mib_find_name(mainload, "Step")->me;
  XtVaGetValues(temp, XmNforeground, &fg,
                        XmNbackground, &bg, XmNdepth, &depth, NULL);
  button_pix = XCreatePixmapFromBitmapData(display, DefaultRootWindow(display),
                 (char *)step_bits, step_width, step_height, fg, bg, depth);
  XtVaSetValues(temp, XmNlabelType, XmPIXMAP,
                        XmNlabelPixmap, button_pix, NULL);
  XtAddCallback(temp, XmNactivateCallback, (XtCallbackProc) step_callback,
			(XtPointer) NULL);

  temp = mib_find_name(mainload, "Play")->me;
  XtVaGetValues(temp, XmNforeground, &fg,
                        XmNbackground, &bg, XmNdepth, &depth, NULL);
  button_pix = XCreatePixmapFromBitmapData(display, DefaultRootWindow(display),
                (char *)play_bits, play_width, play_height, fg, bg, depth);
  XtVaSetValues(temp, XmNlabelType, XmPIXMAP,
                        XmNlabelPixmap, button_pix, NULL);
  XtAddCallback(temp, XmNactivateCallback, (XtCallbackProc) play_callback,
			(XtPointer) NULL);

/*****************************************************************************/

  n = 0;
  XtSetArg(args[n], XmNtitle, "Virtual VCR Infomation"); n++;
  XtSetArg(args[n], XmNdeleteResponse, XmDO_NOTHING);n++;
  XtSetArg(args[n], XmNkeyboardFocusPolicy, XmPOINTER); n++;
  XtSetArg(args[n], XmNwidth, MINX); n++;
  XtSetArg(args[n], XmNheight, MINY); n++;
  /*
  infoshell = XtAppCreateShell("Info", "virtual_vcr",
			       topLevelShellWidgetClass, display, args, n);
  */
  temp = XmCreateDialogShell(shell, "virtual_vcr", args, n);
  n = 0;
  XtSetArg(args[n], XmNtitle, "Virtual VCR Infomation"); n++;

  infoshell = XmCreateForm(temp, "virtual_vcr", args, n);
  infoload = mib_load_interface(infoshell, InfoMib,
                                MI_FROMSTRING);

  temp = mib_find_name(infoload, "Dismiss")->me;
  XtAddCallback(temp, XmNactivateCallback, (XtCallbackProc) infoclose_callback,
			(XtPointer) NULL);

  temp = mib_find_name(infoload, "TextBig")->me;
  XtVaSetValues(temp, XmNcursorPositionVisible, False,
			   XmNeditable, False, NULL);
  XmTextSetString(temp, infomessage);

  XmFontListFree (fontlist);

  parashell = CreateParameterWindow(shell);

  progshell = CreateProgramWindow(shell, cmdSocket, &playtag, frametext);
  
  fileshell = CreateFileWindow(shell, cmdSocket, &playtag, frametext);

}

/*****************************************************************************/

Window MonitorWindow()
{
  return monitorwindow;
}

/*****************************************************************************/

void UISetColormap()
{
  privcolormap = 1;
}

/*****************************************************************************/

void UISetwin(Window win)
{
  displaywin = win;
}

/*****************************************************************************/

void UISetFrame()
{
  char framestr[100];
    
  sprintf(framestr, "%d of %d", shared->currentDisplay, shared->totalFrames);
  XmTextFieldSetString(frametext, framestr);
}

/*****************************************************************************/

void UIMinsize(int x, int y)
{
  int minx;
  int miny;
  int winx;
  int winy;
  
  winx = x;
  winy = y;

  x += 30;
  y += 180;

  if (x < 450) x = MINX;
  if (y < 350) y = MINY;
  
  minx = x; miny = y;

  if (privcolormap)
  {
    XtVaSetValues(shell, XmNcolormap, cmap, NULL);
  }
  if (shell)
  {
    XtResizeWidget(shell, (Dimension)minx, (Dimension)miny, (Dimension)0);
    XtVaSetValues(shell,
		  XmNminWidth, MINX,
		  XmNminHeight, MINY,
                  XmNmaxWidth, minx,
		  XmNmaxHeight, miny,
		  NULL);

    XtMapWidget(shell); /* Map the shell now to avoid disappearing buttons! */
    if (displaywin)
    {
      x = winx; y = winy;
      winx = minx - 30; winy = miny - 180;
      XMoveWindow(display, displaywin, (winx - x)/2, (winy - y)/2+12);
    }
  }
}

/*****************************************************************************

  From here on we have private methods:

 *****************************************************************************/

static void exit_callback(Widget w, XtPointer data, XmAnyCallbackStruct *cbs)
{
  exit(0);
}


static void info_callback(Widget w, XtPointer data, XmAnyCallbackStruct *cbs)
{
  /*
  XtRealizeWidget(infoshell);
  */
  XtManageChild(infoshell);
}

static void infoclose_callback(Widget w, XtPointer data, XmAnyCallbackStruct *cbs)
{
  /*
  XtUnrealizeWidget(infoshell);
  */
  XtUnmanageChild(infoshell);
}

static void para_callback(Widget w, XtPointer data, XmAnyCallbackStruct *cbs)
{
  
  XtRealizeWidget(parashell);
  /*
  XtManageChild(parashell);
  */
}

static void prog_callback(Widget w, XtPointer data, XmAnyCallbackStruct *cbs)
{
  
  XtRealizeWidget(progshell);
  /*
  XtManageChild(progshell);
  */
}

static void file_callback(Widget w, XtPointer data, XmAnyCallbackStruct *cbs)
{
  /*
  XtRealizeWidget(fileshell);
  */
  XtManageChild (fileshell);
}

/*****************************************************************************/

static void loopchange_callback(Widget w, XtPointer data, XmAnyCallbackStruct *cbs)
{
  static int loop = 0;
  loop = !loop;
  if (playtag)
  {
    char cmd = loop ? CmdLOOPenable : CmdLOOPdisable;
    CmdWrite(&cmd, 1);
  }
  else
    beep();
}

static void normal_callback(Widget w, XtPointer data, XmAnyCallbackStruct *cbs)
/* set speed bar, suppose speed_callback should be called automatically */
{
  char cmd = CmdSPEED;
  int value = 50;
  CmdWrite(&cmd, 1);
  CmdWrite((char *)&value, 4);
  //  ACE_DEBUG ((LM_DEBUG,"(%P|%t) Reached in %s line %d\n",__FILE__,__LINE__));
  XmScaleSetValue(speedscale, 50);
}

static void fb_callback(Widget w, XtPointer data, XmAnyCallbackStruct *cbs)
{
  if (playtag && !cmdBusy)
  {
    char cmd = CmdFB;
    cmdBusy = 1;
    CmdWrite(&cmd, 1);
  }
  else
    beep();
}

static void stop_callback(Widget w, XtPointer data, XmAnyCallbackStruct *cbs)
{
  if (playtag && !cmdBusy)
  {
    char cmd = CmdSTOP;
    cmdBusy = 1;
    CmdWrite(&cmd, 1);
  }
  else
    beep();
}

static void ff_callback(Widget w, XtPointer data, XmAnyCallbackStruct *cbs)
{
  if (playtag && !cmdBusy)
  {
    char cmd = CmdFF;
    cmdBusy = 1;
    CmdWrite(&cmd, 1);
  }
  else
    beep();
}


static void step_callback(Widget w, XtPointer data, XmAnyCallbackStruct *cbs)
{
  if (playtag && !cmdBusy)
  {
    char cmd = CmdSTEP;
    cmdBusy = 1;
    CmdWrite(&cmd, 1);
  }
  else {
    /*
    beep();
    */
  }
}

static void play_callback(Widget w, XtPointer data, XmAnyCallbackStruct *cbs)
{
  ACE_DEBUG ((LM_DEBUG,"(%P)play_callback:cmdbusy= %d,playtag=%d\n",cmdBusy,playtag));
  if (playtag && !cmdBusy)
  {
    char cmd = CmdPLAY;
    cmdBusy = 1;
    CmdWrite(&cmd, 1);
  }
  else
    beep();
}

static void volume_callback(Widget w, XtPointer data, XmAnyCallbackStruct *cbs)
{
  char cmd;
  XmScaleCallbackStruct * call_value = (XmScaleCallbackStruct *) cbs;
  int value = call_value ->value;
  
  switch (call_value->reason)
  {
  case XmCR_VALUE_CHANGED:
  case XmCR_DRAG:
    cmd = CmdVOLUME;
    CmdWrite(&cmd, 1);
    CmdWrite((char *)&value, 4);
    break;
  default:
    fprintf(stderr, "Hit the default, incorrect reason sent for scale!!\n");
    break;
 }
}

static void speed_callback(Widget w, XtPointer data, XmAnyCallbackStruct *cbs)
{
  char cmd;
  XmScaleCallbackStruct * call_value = (XmScaleCallbackStruct *) cbs;
  int value = call_value ->value;
  
  switch (call_value->reason)
  {
  case XmCR_VALUE_CHANGED:
  case XmCR_DRAG:
    cmd = CmdSPEED;
    CmdWrite(&cmd, 1);
    CmdWrite((char *)&value, 4);
    break;
  default:
    fprintf(stderr, "Hit the default, incorrect reason sent for scale!!\n");
    break;
 }
}

static void balance_callback(Widget w, XtPointer data, XmAnyCallbackStruct *cbs)
{
  char cmd;
  XmScaleCallbackStruct * call_value = (XmScaleCallbackStruct *) cbs;
  int value = call_value ->value;
  
  switch (call_value->reason)
  {
  case XmCR_VALUE_CHANGED:
  case XmCR_DRAG:
    cmd = CmdBALANCE;
    CmdWrite(&cmd, 1);
    CmdWrite((char *)&value, 4);
    break;
  default:
    fprintf(stderr, "Hit the default, incorrect reason sent for scale!!\n");
    break;
 }
}

static void position_callback(Widget w, XtPointer data, XmAnyCallbackStruct *cbs)
{
  char cmd;
  XmScaleCallbackStruct * call_value = (XmScaleCallbackStruct *) cbs;
  int value = call_value ->value;
  
  switch (call_value->reason)
  {
  case XmCR_VALUE_CHANGED:
    if (playtag && !cmdBusy)
    {
      cmdBusy = 1;
      cmd = CmdPOSITIONrelease;
      CmdWrite(&cmd, 1);
      CmdWrite((char *)&value, 4);
    }
    else
    {
      //      ACE_DEBUG ((LM_DEBUG,"(%P|%t) Reached in %s line %d\n",__FILE__,__LINE__));
      XmScaleSetValue(positionscale, shared->locationPosition);
      /*
      fprintf(stderr, "UI: shared->cmd = %d\n", shared->cmd);
      if (shared->cmd != CmdPOSITION)
        beep();
      */
    }
      
    break;
  case XmCR_DRAG:
    if (playtag && !cmdBusy)
    {
      cmdBusy = 1;
      cmd = CmdPOSITION;
      CmdWrite(&cmd, 1);
      CmdWrite((char *)&value, 4);
    }
    break;
  default:
    fprintf(stderr, "Hit the default, incorrect reason sent for scale!!\n");
    break;
 }
}

static void cmdSocket_callback(Widget w, XtPointer data, XmAnyCallbackStruct *cbs)
{
  char cmd;
/*
   fprintf(stderr, "UI in CmdSocket_callback.\n");
*/
  while (read(cmdSocket, &cmd, 1) <= 0) 
  {
    if (errno == EINTR) return;
    perror("UI by callback read from CmdSocket");
    exit(1);
  }
  //  ACE_DEBUG ((LM_DEBUG,"(%P)cmdSocket_callback:cmd= %d",cmd));
  if (cmd == CmdDONE || cmd == CmdFAIL)
  {
    cmdBusy = 0;
    if (shared->cmd == CmdINIT)
    {
      if (cmd == CmdDONE)
      {
	char cmd = CmdSPEED;
	int value = 50;
        //        ACE_DEBUG ((LM_DEBUG,"(%P|%t) Reached in %s line %d\n",__FILE__,__LINE__));
	XmScaleSetValue(speedscale, 50);
        //        ACE_DEBUG ((LM_DEBUG,"(%P|%t) Reached in %s line %d\n",__FILE__,__LINE__));
	XmScaleSetValue(positionscale, 0);
	CmdWrite(&cmd, 1);
	CmdWrite((char *)&value, 4);
	/*
	fprintf(stderr, "UI: VCR program successfully initialized.\n");
	*/
	playtag = 1;
      }
      else
      {
	fprintf(stderr, "UI: VCR program initialization failed.\n");
	playtag = 0;
      }
    }
    else {
      /*
      fprintf(stderr, "UI: cmd-%d finished successfully.\n", shared->cmd);
      */
    }
    /*
    UISetFrame();
    */
  }
  else
  {
    static int prepos = 0;
    int curpos;
    char * ptr;

    if (cmd == CmdVPdisplayFrame) {
      read(cmdSocket, (char *)&ptr, sizeof(char *));
    }
    VPcommand((int)cmd, ptr);

    if (cmd == CmdVPaudioPosition)
    {
      curpos = (shared->nextSample * 100) / shared->totalSamples;
      if (shared->cmd == CmdPLAY &&
	  (curpos <= prepos - 1 || curpos >= prepos + 1))
      {
        //   ACE_DEBUG ((LM_DEBUG,"(%P|%t) Reached in %s line %d,positionscale = %d,curpos = %d\n",
        //          __FILE__,__LINE__,positionscale,curpos));
	XmScaleSetValue(positionscale, curpos);
	prepos = curpos;
      }
    }
    else if (cmd == CmdVPdisplayFrame && shared->totalFrames)
    {
      curpos = (shared->currentDisplay * 100) / shared->totalFrames;
      if ((shared->cmd == CmdSTEP || shared->cmd == CmdPLAY ||
	   shared->cmd == CmdFF || shared->cmd == CmdFB) &&
	  (curpos <= prepos - 1 || curpos >= prepos + 1))
      {
        //        ACE_DEBUG ((LM_DEBUG,"(%P|%t) Reached in %s line %d\n",__FILE__,__LINE__));
	XmScaleSetValue(positionscale, curpos);
	prepos = curpos;
      }
    }
  }
}

static void monitor_expose(Widget w, XtPointer data,
				XmDrawingAreaCallbackStruct *cbs)
{
  VPexpose();
}

#define BUFSIZE 256

void UIprocess(int cmdSock)
{
  char *title, *vh, *vf, *vb, *ah, *af, *ab;
  int i;
  cmdSocket = cmdSock;
  UICreate();
  
  VPinitWindow(shell, monitorwindow, cmdSock);
  XtAppAddInput(App, cmdSock, (XtPointer)XtInputReadMask,
		(XtInputCallbackProc)cmdSocket_callback, NULL);

  title = vh = vf = vb = ah = af = ab = NULL;

  for (i = 1; i < argc_share; i++) {
    if (strcmp(argv_share[i], "-v") == 0) {
      if (argc_share > i + 1) {
	i ++;
	vh = argv_share[i];
        cerr << "vh = " << vh << endl;
      }
      else break;
    }
    else if (strcmp(argv_share[i], "-a") == 0) {
      if (argc_share > i + 1) {
	i ++;
	ah = argv_share[i];
        cerr << "ah = " << ah << endl;
      }
      else break;
    }
    else if (strcmp(argv_share[i], "-p") == 0) {
      if (argc_share > i + 1) {
	i ++;
	title = argv_share[i];
      }
    }
  }

  if (title != NULL) { /* the init program is supplied by -l */
    cerr << "title is not null \n";
    FILE * fp;
    char buf[PATH_SIZE];
    char vh[PATH_SIZE];
    char vf[PATH_SIZE];
    char ah[PATH_SIZE];
    char af[PATH_SIZE];
    
    get_full_path(title, buf, 256);
    fp = fopen(buf, "r");
    if (fp != NULL) {
      if ((!feof(fp)) && fgets(buf, PATH_SIZE, fp) != NULL) {
	buf[strlen(buf)-1] = 0;
      }
      else buf[0] = 0;
      if ((!feof(fp)) && fgets(vh, PATH_SIZE, fp) != NULL) {
	vh[strlen(vh)-1] = 0;
      }
      else vh[0] = 0;
      if ((!feof(fp)) && fgets(vf, PATH_SIZE, fp) != NULL) {
	vf[strlen(vf)-1] = 0;
      }
      else vf[0] = 0;
      if ((!feof(fp)) && fgets(ah, PATH_SIZE, fp) != NULL) {
	ah[strlen(ah)-1] = 0;
      }
      else ah[0] = 0;
      if ((!feof(fp)) && fgets(af, PATH_SIZE, fp) != NULL) {
	af[strlen(af)-1] = 0;
      }
      else af[0] = 0;

      fclose(fp);
      Fprintf(stderr, "Init program: title %s, vh %s, vf %s, ah %s, af %s\n",
	      buf, vh, vf, ah, af);
      StartProgram(buf, vh, vf, ah, af);
    }
  }
  else if (vh != NULL || ah != NULL) {/* init program supplied by -v and -a */
    char *ptr;

    /* form the title */
    if (vh != NULL) {
      title = (char *)malloc(strlen(vh) + 1);
      if (title != NULL) {
	strcpy(title, vh);
      }
    }
    else {
      title = (char *)malloc(strlen(ah) + 1);
      if (title != NULL) {
	strcpy(title, ah);
      }
    }
    if (title == NULL) {
      title = "The program from the command line";
    }

    /* generate vh and vf */
    if (vh != NULL) {
      if ((ptr = strchr(vh, ':')) != NULL) {
	* ptr = 0;
	vf = ptr + 1;
      }
      else {
	vf = vh;
	vh = "";
	// vb = (char *)malloc(BUFSIZE);
// 	if (vb != NULL) {
// 	  get_full_path(vf, vb, BUFSIZE);
// 	  vf = vb;
//	}
      }
    }
    else vh = vf = "";

    /* generate ah and af */
    if (ah != NULL) {
      if ((ptr = strchr(ah, ':')) != NULL) {
	* ptr = 0;
	af = ptr + 1;
      }
      else {
	af = ah;
	ah = "";
// 	ab = (char *)malloc(BUFSIZE);
// 	if (ab != NULL) {
// 	  get_full_path(af, ab, BUFSIZE);
// 	  af = ab;
// 	}
      }
    }
    else ah = af = "";

    fprintf(stderr, "Init program: title %s, vh %s, vf %s, ah %s, af %s\n",
	    title, vh, vf, ah, af);
    StartProgram(title, vh, vf, ah, af);
    free(title);
    if (ab != NULL) free(ab);
    if (vb != NULL) free(vb);
  }
  
  XtAppMainLoop(App);
}


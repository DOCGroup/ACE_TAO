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
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include "video.h"
#include <X11/Intrinsic.h>
#include <Xm/List.h>
#include <Xm/Frame.h>
#include <Xm/PushB.h>
#include <Xm/Form.h>
#include <Xm/Text.h>
#include <Xm/TextF.h>
#include <Xm/DialogS.h>
#include "include/common.h"
#include "newproto.h"
#include "global.h"

ACE_RCSID(mpeg_client, prog, "$Id$")

#define NUM_LIST_ITEMS 20
#define STRING_SIZE 100
#define ITEMS 100
#define PROGRAM_FILE "~/.vcr/vcrPrograms"
#define BANNER "Virtual VCR Program DataBase. version 2.0 **DO NOT edit**"
#ifdef XmFONTLIST_DEFAULT_TAG
#define XmStringTag XmFONTLIST_DEFAULT_TAG
#else
#define XmStringTag XmSTRING_DEFAULT_CHARSET
#endif

static int cmdSocket;
static int *playtag;
static Widget titlewidget, progshell, progmodifyshell;
static Widget wproglist, wprogmodify;

static int items = 0;
static XmString * item;
static struct ProgramList
{
  char title[STRING_SIZE];
  char vh[STRING_SIZE];
  char vf[STRING_SIZE];
  char ah[STRING_SIZE];
  char af[STRING_SIZE];
} * prog;

static int progcmd, progid;  /* progcmd == 0 - insert, 1 - modify*/
static struct ProgramList deletedprog;

#define LINE1 "  Program Title: "
#define LINE2 "Video file host: "
#define LINE3 "Video file path: "
#define LINE4 "Audio file host: "
#define LINE5 "Audio file path: "
#define MarginHeight 10
#define MarginWidth  10
static int charHeight, charWidth;
static int titleSize;

#define Fgets(s) {if (feof(fp) || fgets(s, STRING_SIZE, fp) == NULL) break;  s[strlen(s)-1] = 0; }

#define Fputs(s) {fputs(s, fp); fputc('\n', fp);}

static void SaveBuf(void);

static void InitBuf(void)
{
  FILE * fp;
  char *fname;
  char buf[256];
  int copyTag = 0;

  deletedprog.title[0] = 0;
  
  item = (XmString *)malloc(sizeof(*item) * ITEMS);
  if (item == NULL)
  {
    perror("UI allocate proglist item(XmString)");
    exit(1);
  }
  prog = (struct ProgramList *) malloc(sizeof(*prog) * ITEMS);
  if (prog == NULL)
  {
    perror("UI allocate prog");
    exit(1);
  }
  fp = NULL;
  if (proglistName[0] != 0) { /* file name given in command line */
    get_full_path(proglistName, buf, 256);
    fname = proglistName;
    fp = fopen(buf, "r");
    if (fp == NULL) {
      fprintf(stderr,"PROG.C error opening %s:", proglistName);
      perror("");
    }
  }
  if (fp == NULL) {
    get_full_path(PROGRAM_FILE, buf, 256);
    fname = PROGRAM_FILE;
    fp = fopen(buf, "r");
  }
  if (fp == NULL) {
    {
      
    InitBuf_exit1:
      
      get_full_path(PROGRAM_FILE, buf, 256);
      if (access(buf, 0) != 0) { /* PROGRAM_FILE does not exist, create it */
	if (strchr(fname, '/') != NULL) { /* create the default directory */
	  char *ptr = strrchr(buf, '/');
	  *ptr = 0;
	  mkdir(buf, 0755);
	  *ptr = '/';
	}
	fname = PROGRAM_FILE;
	fp = fopen(buf, "w+");
      }
      else {  /* Exist, create a temp file in /tmp */
	char *ptr = tempnam("/tmp/", "vcr");
	fname = tmpnam(NULL);
	if (ptr != NULL) {
	  strncpy(fname, ptr, L_tmpnam);
	  free(ptr);
	}
	strcpy(buf, fname);
	fp = tmpfile();
      }
      if (fp != NULL) {
	if (strncmp(fname, "/tmp/", 5) != 0) {
	  fprintf(stderr,
		  "Warning: created file %s with default movie list.\n",
		  fname);
	}
	else fprintf(stderr, "Warning: using default movie list.\n");
	fprintf(fp, "%s\n", BANNER);
	fprintf(fp, "\
lemond: (va) Blazers Game 1 (128)\n\
lemond.cse.ogi.edu\n\
/projects/dsrg/L/multimedia3/c1.0.300.128.mpg\n\
lemond.cse.ogi.edu\n\
/projects/dsrg/L/multimedia3/c1.0.300.au\n\
lemond: (va) Blazers Game 1 (256)\n\
lemond.cse.ogi.edu\n\
/projects/dsrg/L/multimedia3/c1.0.300.256.mpg\n\
lemond.cse.ogi.edu\n\
/projects/dsrg/L/multimedia3/c1.0.300.au\n\
lemond: (va) Blazers Game 1 (320)\n\
lemond.cse.ogi.edu\n\
/projects/dsrg/L/multimedia3/c1.0.300.320.mpg\n\
lemond.cse.ogi.edu\n\
/projects/dsrg/L/multimedia3/c1.0.300.au\n\
lemond: (va) Blazers Game 2 (128)\n\
lemond.cse.ogi.edu\n\
/projects/dsrg/L/multimedia3/h1.0.300.128.mpg\n\
lemond.cse.ogi.edu\n\
/projects/dsrg/L/multimedia3/h1.0.300.au\n\
lemond: (va) Blazers Game 2 (256)\n\
lemond.cse.ogi.edu\n\
/projects/dsrg/L/multimedia3/h1.0.300.256.mpg\n\
lemond.cse.ogi.edu\n\
/projects/dsrg/L/multimedia3/h1.0.300.au\n\
lemond: (va) Blazers Game 2 (320)\n\
lemond.cse.ogi.edu\n\
/projects/dsrg/L/multimedia3/h1.0.300.320.mpg\n\
lemond.cse.ogi.edu\n\
/projects/dsrg/L/multimedia3/h1.0.300.au\n\
lemond: (va) Blazers Game 3 (128)\n\
lemond.cse.ogi.edu\n\
/projects/dsrg/L/multimedia3/s1.0.300.128.mpg\n\
lemond.cse.ogi.edu\n\
/projects/dsrg/L/multimedia3/s1.0.300.au\n\
lemond: (va) Blazers Game 3 (256)\n\
lemond.cse.ogi.edu\n\
/projects/dsrg/L/multimedia3/s1.0.300.256.mpg\n\
lemond.cse.ogi.edu\n\
/projects/dsrg/L/multimedia3/s1.0.300.au\n\
lemond: (vo) Skiing on Mt. Rainer\n\
lemond.cse.ogi.edu\n\
/projects/dsrg/L/multimedia3/skiRainer.mpeg\n\
\n\
\n\
lemond: (ao) Following the sense (Chinese song)\n\
\n\
\n\
lemond.cse.ogi.edu\n\
/projects/dsrg/L/multimedia3/sense.au\n\
");
	fseek(fp, (long)0, 0);
	fgets(buf, 100, fp);
	goto InitBuf_continue;
	/*
	fclose(fp);
	fp = fopen(buf, "r");
	if (fp != NULL) {
	  fgets(buf, 100, fp);
	  goto InitBuf_continue;
	}
	else {
	  fprintf(stderr, "Warning: failed to read newly created %s:", buf);
	  perror("");
	}
	*/
      }
      else {
	fprintf(stderr, "Warning: failed to create %s:", buf);
	perror("");
      }
      items = 0;
      return;
    }
  }
  if (fgets(buf, 100, fp) == NULL)
  {
    fclose(fp);
    goto InitBuf_exit1;
  }
  buf[strlen(buf)-1] = 0;
  if (strncmp(buf, BANNER, strlen(BANNER)))
  {
    fprintf(stderr, "BANNER in %s not expected\n", fname);
    fprintf(stderr, "    Expected: %s\n", BANNER);
    fprintf(stderr, "    Actual  : %s\n", buf);
    fclose(fp);
    goto InitBuf_exit1;
  }
  
 InitBuf_continue:
  
  items = 0;
  for(;;)
  {
    /*
    fprintf(stderr, "Reading next program.\n");
    */
    Fgets(prog[items].title);
    //    Fgets(prog[items].vh);
    Fgets(prog[items].vf);
    //    Fgets(prog[items].ah);
    Fgets(prog[items].af);
    item[items] = (XmString)XmStringCreateLtoR(prog[items].title, XmStringTag);
    if ((++items) >= ITEMS) break;
  }
  /*
  fprintf(stderr, "Total programs %d\n", items);
  */
  fclose(fp);
  if (copyTag) {
    SaveBuf();
  }
}

static void SaveBuf(void)
{
  int i;
  FILE * fp;
  char buf[256];
  get_full_path(PROGRAM_FILE, buf, 256);
  fp = fopen(buf, "w");
  if (fp == NULL)
  {
    fprintf(stderr, "Fail to open %s for saving programs", buf);
    perror("");
    return;
  }
  Fputs(BANNER);
  for (i = 0; i < items; i++)
  {
    Fputs(prog[i].title);
    //    Fputs(prog[i].vh);
    Fputs(prog[i].vf);
    //    Fputs(prog[i].ah);
    Fputs(prog[i].af);
  }
  fclose(fp);
}

static void CmdWrite(char * buf, int size)
{
  if (size == 0) return;
  while (write(cmdSocket, (buf), (size)) == -1)
  {
    if (errno == EINTR) continue;
    perror("UI write to cmdSocket");
    exit(1);
  }
}

void StartProgram(char * title, char * vh, char * vf, char * ah, char * af)
{
  extern int cmdBusy;
  char tmp = CmdINIT;
  int len;
  if (!cmdBusy) {
    char *str = title;
    len = strlen(str);
//     if (len > 0 && str[len-1] == 0x0d) str[len - 1] = 0;
//     str = vh;
    //    len = strlen(str);
    if (len > 0 && str[len-1] == 0x0d) str[len - 1] = 0;
    str = vf;
    //    len = strlen(str);
    //    if (len > 0 && str[len-1] == 0x0d) str[len - 1] = 0;
    //    str = ah;
    len = strlen(str);
    if (len > 0 && str[len-1] == 0x0d) str[len - 1] = 0;
    str = af;
    len = strlen(str);
    if (len > 0 && str[len-1] == 0x0d) str[len - 1] = 0;
    XmTextFieldSetString(titlewidget, title);
    cmdBusy = 1;
    CmdWrite(&tmp, 1);
//     len = strlen(vh);
//     CmdWrite((char *)&len, 4);
//     cerr << "StartProgram: len =" << len;
//     CmdWrite(vh, len);
//     cerr << " StartProgram: vh =" << vh << endl;
    len = strlen(vf);
    CmdWrite((char *)&len, 4);
    cerr << "StartProgram: len =" << len;
    CmdWrite(vf, len);
    cerr << " StartProgram: vf =" << vf << endl;
//     len = strlen(ah);
//     CmdWrite((char *)&len, 4);
//     cerr << "StartProgram: len =" << len;
//     CmdWrite(ah, len);
//     cerr << " StartProgram: ah =" << ah << endl;
    len = strlen(af);
    CmdWrite((char *)&len, 4);
    cerr << "StartProgram: len =" << len;
    CmdWrite(af, len);
    cerr << " StartProgram: af =" << af << endl;
  }
  else {
    beep();
  }
  cerr << "StartProgram  done\n";
}

static int curListPos(void)
{
  int * poses, count;
  
  if (XmListGetSelectedPos(wproglist, &poses, &count))
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


static void modifyprog(struct ProgramList *p)
{
  XmString item;
  
  if (items <0 || progid >= items || progid < 0)
  { beep();  return; }
  item = (XmString)XmStringCreateLtoR(p->title, XmStringTag);
  XmListReplaceItemsPos(wproglist, &item, 1, progid+1);
  XmListSelectPos(wproglist, progid+1, 0);
  XmStringFree(item);
  memcpy(&(prog[progid]), p, sizeof(*p));
  SaveBuf();
}

static void insertprog(struct ProgramList *p)
{
  XmString item;
  
  if (items <= 0 || items >= ITEMS) return;
  memcpy((char*)&(prog[items]), (char *)p, sizeof(*p));
  item = (XmString)XmStringCreateLtoR(p->title, XmStringTag);
  XmListAddItemUnselected(wproglist, item, items+1);
  XmStringFree(item);
  if (items == 0)
    XmListSelectPos(wproglist, 0, False);
  items ++;
  SaveBuf();
}

static void verifyCB(Widget w, XtPointer closure, XmTextVerifyCallbackStruct * cd);

static void deleteCB(Widget w, XtPointer closure, XtPointer call_data)
{
  int i, j;

  //  return;
  
  
  if (items <= 0) { beep(); return; }
  j = curListPos();
  if (j == -1) return;
  memcpy(&deletedprog, &prog[j], sizeof(deletedprog));
  for (i = j+1; i < items; i++)
    memcpy(&prog[i-1], &prog[i], sizeof(*prog));
  XmListDeletePos(wproglist, j+1);
  items --;
  SaveBuf();
  return;
}

static void undoCB(Widget w, XtPointer closure, XtPointer call_data)
{
  //  return;
  
  if (deletedprog.title[0] != 0)
  {
    insertprog(&deletedprog);
    deletedprog.title[0] = 0;
  }
  else
    beep();
  
}

static void setposition(Widget w, int x, int y);

static void insertCB (Widget w, XtPointer closure, XtPointer call_data)
{
  char buf[1000];

  //  return;

  //#if 0
  if (items >= ITEMS -1)
  {
    beep();
    return;
  }
  XtRemoveAllCallbacks(wprogmodify, XmNmodifyVerifyCallback);
  XtRemoveAllCallbacks(wprogmodify, XmNmotionVerifyCallback);
  progcmd = 0;
  sprintf(buf, "    *** append a program ***\n");
  strcat(buf, LINE1);
  strcat(buf, "\n");
  strcat(buf, LINE2);
  strcat(buf, "\n");
  strcat(buf, LINE3);
  strcat(buf, "\n");
  strcat(buf, LINE4);
  strcat(buf, "\n");
  strcat(buf, LINE5);
  strcat(buf, "\n\n");
  XmTextSetString(wprogmodify, buf);

  setposition(wprogmodify, titleSize, 1);
  XtAddCallback (wprogmodify, XmNmodifyVerifyCallback, (XtCallbackProc)verifyCB, NULL);
  XtAddCallback (wprogmodify, XmNmotionVerifyCallback, (XtCallbackProc)verifyCB, NULL);
  
  // uncommented
  //XtManageChild(progmodifyshell);
  
  XtRealizeWidget(progmodifyshell);
  //#endif
}

static void modifyCB(Widget w, XtPointer closure, XtPointer call_data)
{
  int i;
  char buf[1000];
  if (items == 0)
  {
    beep();
    return;
  }
  XtRemoveAllCallbacks(wprogmodify, XmNmodifyVerifyCallback);
  XtRemoveAllCallbacks(wprogmodify, XmNmotionVerifyCallback);
  progid = i = curListPos();
  progcmd = 1;
  sprintf(buf, "    *** modify program: %s ***\n", prog[i].title);
  strcat(buf, LINE1);
  strcat(buf, prog[i].title);
  strcat(buf, "\n");
  //  strcat(buf, LINE2);
  //  strcat(buf, prog[i].vh);
  //  strcat(buf, "\n");
  strcat(buf, LINE3);
  strcat(buf, prog[i].vf);
  strcat(buf, "\n");
  //  strcat(buf, LINE4);
  //  strcat(buf, prog[i].ah);
  //  strcat(buf, "\n");
  strcat(buf, LINE5);
  strcat(buf, prog[i].af);
  strcat(buf, "\n\n");
  XmTextSetString(wprogmodify, buf);

  setposition(wprogmodify, titleSize, 1);
  XtAddCallback (wprogmodify, XmNmodifyVerifyCallback, (XtCallbackProc)verifyCB, NULL);
  XtAddCallback (wprogmodify, XmNmotionVerifyCallback, (XtCallbackProc)verifyCB, NULL);
  /*
  XtManageChild(progmodifyshell);
  */
  XtRealizeWidget(progmodifyshell);
}

static void playCB(Widget w, XtPointer closure, XtPointer call_data)
{
  /*
  fprintf(stderr, "play");
  */
  if (items > 0)
  {
    int i = curListPos();
    if (i >= items || i < 0)
      return;
    cerr << "PlayCB\n";
    cerr << prog[i].title << " " << " " <<prog[i].vf << " " << " " << prog[i].af << endl;
    StartProgram(prog[i].title, prog[i].vh, prog[i].vf, prog[i].ah, prog[i].af);
  }
}

static void dismissCB(Widget W, XtPointer closure, XtPointer call_data)
{
  
  XtUnrealizeWidget(progshell);
  /*
  XtUnmanageChild(progshell);
  */
}

static void setposition(Widget w, int x, int y)
{
  Position x1, y1;
  x1 = charWidth * x + (MarginWidth+4);
  y1 = charHeight * y + (charHeight+MarginHeight+1);
  XmTextSetInsertionPosition(w, XmTextXYToPos(w, x1, y1));
}

static int getposition(Widget w, XmTextVerifyCallbackStruct * cd,
		       int * x, int *y)
{
  Position x1, y1;
  if (!XmTextPosToXY(w, cd->newInsert, &x1, &y1))
    return 0;
  *x = (x1-(MarginWidth+4))/charWidth;
  *y = (y1-(charHeight+MarginHeight+1))/charHeight;
  if ((*x)*charWidth != x1-(MarginWidth+4) || (*y)*charHeight != y1-(charHeight+MarginHeight+1))
  {
    /*
    fprintf(stderr, "error: TextCursorConversion failed. \007\n");
    fprintf(stderr, "x-%d, x1-%d, y-%d, y1-%d\n", *x, (int)x1, *y, (int)y1);
    */
    return 1;
  }
  return 1;
  
}

static void modifydismissCB(Widget w, XtPointer closure, XtPointer call_data)
{
  
  XtUnrealizeWidget(progmodifyshell);
  /*
  XtUnmanageChild(progmodifyshell);
  */
}

static void modifysaveCB(Widget w, XtPointer closure, XtPointer call_data)
{
  return;

  //#if 0
  if (progcmd == 1 && items <= progid)
  {
    beep();
  }
  else if (progcmd == 1 || progcmd == 0)
  {
    char * buf, * ptr, *ptr1;
    struct ProgramList p;
    buf = XmTextGetString(wprogmodify);
    ptr = buf;
    fprintf(stderr, "Buf: %s\n", buf);
    while (*ptr != '\n') ptr ++;
    ptr ++;
    
    ptr1 = ptr + titleSize;
    while (*ptr != '\n') ptr ++;
    (*ptr) = 0; ptr ++;
    strncpy(p.title, ptr1, STRING_SIZE);
    p.title[STRING_SIZE-1] = 0;

    ptr1 = ptr + titleSize;
    while (*ptr != '\n') ptr ++;
    (*ptr) = 0; ptr ++;
    strncpy(p.vh, ptr1, STRING_SIZE);
    p.vh[STRING_SIZE-1] = 0;
    
    ptr1 = ptr + titleSize;
    while (*ptr != '\n') ptr ++;
    (*ptr) = 0; ptr ++;
    strncpy(p.vf, ptr1, STRING_SIZE);
    p.vf[STRING_SIZE-1] = 0;
    
    ptr1 = ptr + titleSize;
    while (*ptr != '\n') ptr ++;
    (*ptr) = 0; ptr ++;
    strncpy(p.ah, ptr1, STRING_SIZE);
    p.ah[STRING_SIZE-1] = 0;
    
    ptr1 = ptr + titleSize;
    while (*ptr != '\n') ptr ++;
    (*ptr) = 0;
    strncpy(p.af, ptr1, STRING_SIZE);
    p.af[STRING_SIZE-1] = 0;
    /*
    fprintf(stderr, "*|%s||%s||%s||%s||%s||*\n", p.title, p.vh, p.vf, p.ah, p.af);
    */
    XtFree((char *)buf);
    if (progcmd == 1)
      modifyprog(&p);
    else
      insertprog(&p);
  }
  //#endif
}

static void verifyCB(Widget w, XtPointer closure, XmTextVerifyCallbackStruct * cd)
{
  int x = 0, y = 0;

  if (!getposition(w, cd, &x, &y))
  {
    fprintf(stderr, "error on conversion to x/y\n");
    return;
  }
/*  
  fprintf(stderr, "verifyCB: %d (x-%d, y-%d)\n",cd->newInsert, x, y);
*/
  if (x < titleSize || x > titleSize+STRING_SIZE || y < 1 || y > 5)
  {
    beep();
    cd->doit = False;
    return;
  }
  else
    return;
}

static void CreateModifyWindow(Widget parent)
{
  Arg   	args[20];
  int		n;
  Widget 	wform, wtext, wdone, wcancel;
  XmFontList fontlist;
  XFontStruct * font;
  XmStringCharSet cset = XmSTRING_DEFAULT_CHARSET;
  
  n = 0;
  XtSetArg(args[n], XmNtitle, "Virtual VCR Program List Edit"); n++;
  XtSetArg(args[n], XmNdeleteResponse, XmDO_NOTHING);n++;
  XtSetArg(args[n], XmNkeyboardFocusPolicy, XmPOINTER); n++;
  XtSetArg(args[n], XmNwidth, 700); n++;
  XtSetArg(args[n], XmNheight, 160); n++;
  XtSetArg(args[n], XmNallowShellResize, False); n++;
  
  progmodifyshell = XtAppCreateShell("Program List", "virtual_vcr",
                                     topLevelShellWidgetClass, display, args, n);
  
  // uncommented.
  //  progmodifyshell = XmCreateDialogShell(parent, "virtual_vcr", args, n);
  
  n = 0;

  /* Create form widget */
  n = 0;
  XtSetArg (args[n], XmNallowShellResize, False); n++;
  /*
  XtSetArg (args[n], XmNheight, 300); n++;
  XtSetArg (args[n], XmNwidth, 800); n++;
  */
  wform = XmCreateForm(progmodifyshell, "Programs", args, n);
  
  XtManageChild(wform);
  
  font = XLoadQueryFont (XtDisplay (progmodifyshell), "8x13");
  charHeight = 13;
  charWidth = 8;
  fontlist = XmStringCreateFontList (font, cset);

  n=0;
  XtSetArg (args[n], XmNwidth, 60); n++;
  XtSetArg (args[n], XmNheight, 28); n++;
  XtSetArg (args[n], XmNleftOffset, 20); n++;
  XtSetArg (args[n], XmNbottomOffset, 5); n++;
  XtSetArg (args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg (args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  wdone = XmCreatePushButton (wform, "Save", args, n);
  XtManageChild(wdone);
  XtAddCallback (wdone, XmNactivateCallback, (XtCallbackProc)modifysaveCB, NULL);
  
  n=0;
  XtSetArg (args[n], XmNwidth, 60); n++;
  XtSetArg (args[n], XmNheight, 28); n++;
  XtSetArg (args[n], XmNrightOffset, 20); n++;
  XtSetArg (args[n], XmNbottomOffset, 5); n++;
  XtSetArg (args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg (args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  wcancel = XmCreatePushButton (wform, "Dismiss", args, n);
  XtManageChild(wcancel);
  XtAddCallback (wcancel, XmNactivateCallback, (XtCallbackProc)modifydismissCB, NULL);

  titleSize = strlen(LINE1);
  
  n=0;
  XtSetArg (args[n], XmNleftOffset, 5); n++;
  XtSetArg (args[n], XmNtopOffset, 5); n++;
  XtSetArg (args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg (args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNcolumns, titleSize + STRING_SIZE+1); n++;
  XtSetArg(args[n], XmNrows, 7); n++;
/*
  XtSetArg(args[n], XmNeditable, True); n++;
*/
  XtSetArg(args[n], XmNeditable, False); n++;

  XtSetArg(args[n], XmNmarginHeight, MarginHeight); n++;
  XtSetArg(args[n], XmNmarginWidth, MarginWidth); n++;
  XtSetArg(args[n], XmNborderWidth, 0); n++;
  XtSetArg(args[n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
  /*
  XtSetArg(args[n], XmNvalue, new_string1); n++;
  */
  XtSetArg(args[n], XmNfontList, fontlist); n++;
  wtext = XmCreateText(wform, "Virtual VCR program list modify text", args, n);
  
  XtManageChild(wtext);
  
  XmFontListFree (fontlist);
  wprogmodify = wtext;
}

Widget CreateProgramWindow(Widget parent, int cmdSock, int * playflag, Widget frametext)
{
  Arg   	args[20];
  int		n;
  Widget 	wform, frame1, frame2, wlist,
                wdelete, wundo, winsert, wmodify, wplay, wdismiss;
  XmFontList fontlist;
  XFontStruct * font;
  XmStringCharSet cset = XmSTRING_DEFAULT_CHARSET;
  
  cmdSocket = cmdSock;
  playtag = playflag;
  titlewidget = frametext;

  InitBuf();
  
  n = 0;
  XtSetArg(args[n], XmNtitle, "Virtual VCR Program List"); n++;
  XtSetArg(args[n], XmNdeleteResponse, XmDO_NOTHING);n++;
  XtSetArg(args[n], XmNkeyboardFocusPolicy, XmPOINTER); n++;
  XtSetArg(args[n], XmNwidth, 396); n++;
  XtSetArg(args[n], XmNheight, 300); n++;
  XtSetArg(args[n], XmNminWidth, 396); n++;
  XtSetArg(args[n], XmNminHeight, 150); n++;
  
  progshell = XtAppCreateShell("Program List", "virtual_vcr",
			       topLevelShellWidgetClass, display, args, n);
  /*
  progshell = XmCreateDialogShell(parent, "virtual_vcr", args, n);
  */
  font = XLoadQueryFont (XtDisplay (progshell), "courB14");
  fontlist = XmStringCreateFontList (font, cset);
  
  n = 0;
  XtSetArg(args[n], XmNtitle, "Program List"); n++;
  wform  = XmCreateForm(progshell, "Programs", args, n);
  
  XtManageChild(wform);
  
  n = 0;
  wdelete = XmCreatePushButton (wform, "Delete", args, n);
  XtManageChild(wdelete);
  XtAddCallback (wdelete, XmNactivateCallback, (XtCallbackProc)deleteCB, NULL);
  wundo   = XmCreatePushButton (wform, "Undo", args, n);
  XtManageChild(wundo);
  XtAddCallback (wundo, XmNactivateCallback, (XtCallbackProc)undoCB, NULL);
  winsert = XmCreatePushButton (wform, "Insert", args, n);
  XtManageChild(winsert);
  XtAddCallback (winsert, XmNactivateCallback, (XtCallbackProc)insertCB, NULL);
  wmodify = XmCreatePushButton (wform, "Modify", args, n);
  XtManageChild(wmodify);
  XtAddCallback (wmodify, XmNactivateCallback, (XtCallbackProc)modifyCB, NULL);
  wplay   = XmCreatePushButton (wform, "Select", args, n);
  XtManageChild(wplay);
  XtAddCallback (wplay, XmNactivateCallback, (XtCallbackProc)playCB, NULL);
  wdismiss = XmCreatePushButton (wform, "Dismiss", args, n);
  XtManageChild(wdismiss);
  XtAddCallback (wdismiss, XmNactivateCallback, (XtCallbackProc)dismissCB, NULL);

  n=0;
  XtSetArg (args[n], XmNwidth, 60); n++;
  XtSetArg (args[n], XmNheight, 28); n++;
  XtSetArg (args[n], XmNleftOffset, 5); n++;
  XtSetArg (args[n], XmNbottomOffset, 5); n++;
  XtSetArg (args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg (args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetValues (wdelete, args, n);
  
  n=0;
  XtSetArg (args[n], XmNwidth, 60); n++;
  XtSetArg (args[n], XmNheight, 28); n++;
  XtSetArg (args[n], XmNleftOffset, 70); n++;
  XtSetArg (args[n], XmNbottomOffset, 5); n++;
  XtSetArg (args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg (args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetValues (wundo, args, n);
  
  n=0;
  XtSetArg (args[n], XmNwidth, 60); n++;
  XtSetArg (args[n], XmNheight, 28); n++;
  XtSetArg (args[n], XmNleftOffset, 135); n++;
  XtSetArg (args[n], XmNbottomOffset, 5); n++;
  XtSetArg (args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg (args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetValues (winsert, args, n);
  
  n=0;
  XtSetArg (args[n], XmNwidth, 60); n++;
  XtSetArg (args[n], XmNheight, 28); n++;
  XtSetArg (args[n], XmNleftOffset, 200); n++;
  XtSetArg (args[n], XmNbottomOffset, 5); n++;
  XtSetArg (args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg (args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetValues (wmodify, args, n);
  
  n=0;
  XtSetArg (args[n], XmNwidth, 60); n++;
  XtSetArg (args[n], XmNheight, 28); n++;
  XtSetArg (args[n], XmNleftOffset, 265); n++;
  XtSetArg (args[n], XmNbottomOffset, 5); n++;
  XtSetArg (args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg (args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetValues (wplay, args, n);
  
  n=0;
  XtSetArg (args[n], XmNwidth, 60); n++;
  XtSetArg (args[n], XmNheight, 28); n++;
  XtSetArg (args[n], XmNleftOffset, 330); n++;
  XtSetArg (args[n], XmNbottomOffset, 5); n++;
  XtSetArg (args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg (args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetValues (wdismiss, args, n);

  n = 0;
  XtSetArg (args[n], XmNtopOffset, 2); n++;
  XtSetArg (args[n], XmNleftOffset, 2); n++;
  XtSetArg (args[n], XmNbottomOffset, 38); n++;
  XtSetArg (args[n], XmNrightOffset, 2); n++;
  XtSetArg (args[n], XmNtopAttachment, XmATTACH_FORM); n++;
  XtSetArg (args[n], XmNleftAttachment, XmATTACH_FORM); n++;
  XtSetArg (args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
  XtSetArg (args[n], XmNrightAttachment, XmATTACH_FORM); n++;
  XtSetArg(args[n], XmNfontList,  fontlist); n++;
  XtSetArg(args[n], XmNvisibleItemCount,  NUM_LIST_ITEMS); n++;
  if (items > 0)
  {
    XtSetArg(args[n], XmNitems, item); n++;
    XtSetArg(args[n], XmNitemCount, items); n++;
    XtSetArg(args[n], XmNselectedItems,  item); n++;
    XtSetArg(args[n], XmNselectedItemCount,  1); n++;
  }
  wlist = XmCreateScrolledList(wform, "ProgramList", args, n);
  XtManageChild(wlist);
  XmFontListFree(fontlist);
  wproglist = wlist;
  for (n = 0; n < items; n++)
    XmStringFree(item[n]);
  free(item);
  
  n=0;
  XtSetArg (args[n], XmNtopOffset, 2); n++;
  XtSetArg (args[n], XmNleftOffset, 2); n++;
  XtSetArg (args[n], XmNbottomOffset, 38); n++;
  XtSetArg (args[n], XmNrightOffset, 2); n++;
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
  
  CreateModifyWindow(parent);

  return progshell;
}

/* $Id$ */
/* Copyright (c) 1994 The Geometry Center; University of Minnesota
   1300 South Second Street;  Minneapolis, MN  55454, USA;

This file is free software; you can redistribute it and/or modify it only
under the the terms of the GNU GENERAL PUBLIC LICENSE which should be
included along with thisw file.  This software may be obtained via anonymous
ftp from geom.umn.edu; email: software@geom.umn.edu. */


/* Author: Daeron Meyer */

#include "mibload.h"
#include "mibwidgets.h"

extern Display		*dpy;
extern GC		 mib_gc;

/* Code for Button */
/*****************************************************************************/

mib_Widget *mib_create_Button(mib_Widget *parent, char *name, char *label,
		int posx, int posy, int width, int height, int mib_fill)
{
  mib_Widget *temp;
  mib_Button *myres;
  unsigned char *label_text;
  Arg     args[20];
  int     n;

  /* create the new widget and add it to the tree */

  temp = mib_new_mib_Widget();
  if (mib_fill == WDEFAULT)
    mib_add_backward(temp, parent);
  else
    mib_add_mib_Widget(temp, parent);
  myres = (mib_Button *)malloc(sizeof(mib_Button));

  /* initialize public resources */

  if (mib_fill == WDEFAULT)
  {
    temp->name = (char *)malloc(strlen(name)+1);
    strcpy(temp->name,name);
  }
  temp->mib_class = (char *)malloc(7);
  sprintf(temp->mib_class,"Button");
  temp->mib_class_num = MIB_BUTTON;
  temp->width = width;
  temp->height = height;
  temp->topOffset = posy;
  temp->leftOffset = posx;
  temp->bottomOffset = 0;
  temp->rightOffset = 0;
  temp->topAttachment = 1;
  temp->leftAttachment = 1;
  temp->bottomAttachment = 0;
  temp->rightAttachment = 0;

  temp->mib_allowresize = 1;

  /* initialize private resources */

  temp->myres = (void *)myres;

  if (mib_fill == WDEFAULT)
  {
    myres->label = (char *)malloc(strlen(label)+1);
    strcpy(myres->label,label);
  }

  /* create Xt widget */

  n = 0;

  if (mib_fill == WDEFAULT)
  {
    label_text = XmStringCreateLtoR(label, XmSTRING_DEFAULT_CHARSET);

    XtSetArg (args[n], XmNlabelString, label_text); n++;
    XtSetArg (args[n], XmNleftAttachment, XmATTACH_FORM); n++;
    XtSetArg (args[n], XmNleftOffset, posx);n++;
    XtSetArg (args[n], XmNtopAttachment, XmATTACH_FORM); n++;
    XtSetArg (args[n], XmNtopOffset, posy);n++;
    XtSetArg (args[n], XmNwidth, width); n++;
    XtSetArg (args[n], XmNheight, height); n++;
  }

  XtSetArg (args[n], XmNrubberPositioning, False); n++;
  XtSetArg (args[n], XmNhighlightThickness, 0); n++;

  temp->me = XtCreateManagedWidget(name, xmPushButtonWidgetClass,
                temp->parent->me, args, n);

  if (mib_fill == WDEFAULT)
  {
    XmStringFree(label_text);
  }

  if (mib_fill == WEDIT || mib_fill == WDEFAULT)
  {
    mib_apply_eventhandlers(temp->me, temp);
  }

  return temp;
}

void mib_delete_Button(mib_Widget *thisw)
{
  mib_Button *temp = (mib_Button *)thisw->myres;

  free(temp->label);
  free(temp);
}

void mib_save_Button(mib_Widget *thisw, FILE *fout)
{
  mib_Button *temp = (mib_Button *)thisw->myres;

  fprintf(fout,"label: \\\"%s\\\"\\n\\\n", temp->label);
}

void mib_code_gen_Button(mib_Widget *thisw, FILE *fout)
{
}

int mib_load_Button(mib_Widget *thisw, mib_Buffer *fin)
{
  mib_Button	*myres;
  unsigned char *label_text;
  char          res[MI_MAXSTRLEN];
  char          val[MI_MAXSTRLEN];
  Arg		args[20];
  int		n, got_line, vallen;

  myres = (mib_Button *)thisw->myres;

  got_line = mib_read_line(fin, res, val);
  if (!got_line)
    return 0;

  if (!strcmp(res,"label"))
  {
    vallen = strlen(val);
    if (vallen < 2)
      return 0;
    val[vallen-1] = '\0';
    myres->label = (char *)malloc(vallen-1);
    sprintf(myres->label,"%s",&(val[1]));

    label_text = XmStringCreateLtoR(myres->label, XmSTRING_DEFAULT_CHARSET);

    n = 0;
    XtSetArg (args[n], XmNlabelString, label_text); n++;
    XtSetValues(thisw->me, args, n);

    XmStringFree(label_text);

  }
  else
    return 0;

  got_line = mib_read_line(fin, res, val);
  if (!got_line)
    return 0;

  if (strcmp(res,"EndWidget"))
    return 0;

  return 1;
}

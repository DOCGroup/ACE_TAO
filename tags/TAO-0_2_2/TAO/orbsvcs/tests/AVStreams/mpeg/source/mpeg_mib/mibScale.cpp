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

ACE_RCSID(mpeg_mib, mibScale, "$Id$")

extern Display	*dpy;
extern GC	 mib_gc;
static int       scaleflag;

/* Code for Scale */
/*****************************************************************************/

mib_Widget *mib_create_Scale(mib_Widget *parent, char *name, char *orient,
		int posx, int posy, int width, int height, int mib_fill)
{
  mib_Widget	*temp;
  mib_Scale	*myres;
  Arg		 args[20];
  int		 n;


  scaleflag = 0;
  /* create the new widget and add it to the tree */

  temp = mib_new_mib_Widget();
  if (mib_fill == WDEFAULT)
    mib_add_backward(temp, parent);
  else
    mib_add_mib_Widget(temp, parent);

  myres = (mib_Scale *)ACE_OS::malloc(sizeof(mib_Scale));

  /* initialize public resources */

  if (mib_fill == WDEFAULT)
  {
    temp->name = (char *)ACE_OS::malloc(strlen(name)+1);
    strcpy(temp->name,name);
  }
  temp->mib_class = (char *)ACE_OS::malloc(10);
  sprintf(temp->mib_class,"Scale");
  temp->mib_class_num = MIB_SCALE;
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
  myres->orientation = 0;

  /* create Xt widget */

  n = 0;

  if (mib_fill == WDEFAULT)
  {
    XtSetArg (args[n], XmNleftAttachment, XmATTACH_FORM); n++;
    XtSetArg (args[n], XmNleftOffset, posx);n++;
    XtSetArg (args[n], XmNtopAttachment, XmATTACH_FORM); n++;
    XtSetArg (args[n], XmNtopOffset, posy);n++;
    XtSetArg (args[n], XmNwidth, width); n++;
    XtSetArg (args[n], XmNheight, height); n++;

  }

  XtSetArg (args[n], XmNrubberPositioning, False); n++;


  if (mib_fill == WDEFAULT)
    if (!strcmp("VertScale",orient))
    {
      XtSetArg (args[n], XmNorientation, XmVERTICAL); n++;
    }
    else
    if (!strcmp("HorzScale",orient))
    {
      XtSetArg (args[n], XmNorientation, XmHORIZONTAL); n++;
      myres->orientation = 1;
    }

  if (mib_fill == WEDIT || mib_fill == WDEFAULT)
  {
    XtSetArg (args[n], XmNshowArrows, False); n++;
    XtSetArg (args[n], XmNsliderSize, 30); n++;
  }

  XtSetArg (args[n], XmNrubberPositioning, False); n++;
  XtSetArg (args[n], XmNhighlightThickness, 0); n++;

  if (mib_fill == WEDIT || mib_fill == WDEFAULT)
    temp->me = XtCreateManagedWidget(name, xmScrollBarWidgetClass,
                temp->parent->me, args, n);
  else
    temp->me = XtCreateManagedWidget(name, xmScaleWidgetClass,
                temp->parent->me, args, n);

  if (mib_fill == WEMPTY)
    scaleflag = 1;

  if (mib_fill == WEDIT || mib_fill == WDEFAULT)
  {
    mib_apply_eventhandlers(temp->me, temp);
  }

  return temp;
}

void mib_delete_Scale(mib_Widget *thisw)
{
  mib_Scale *temp = (mib_Scale *)thisw->myres;

  ACE_OS::free(temp);
}

void mib_save_Scale(mib_Widget *thisw, FILE *fout)
{
  mib_Scale *temp = (mib_Scale *)thisw->myres;

  fprintf(fout,"orientation: %d\\n\\\n", temp->orientation);
}

int mib_load_Scale(mib_Widget *thisw, mib_Buffer *fin)
{
  mib_Scale *myres;
  unsigned char *label_text;
  char          res[MI_MAXSTRLEN];
  char          val[MI_MAXSTRLEN];
  Arg           args[5];
  int		n;
  Dimension	myht,mywd;


  myres = (mib_Scale *)thisw->myres;

  if (!mib_read_line(fin, res, val))
    return 0;

  if (!strcmp(res,"orientation"))
  {
    sscanf(val,"%d",&(myres->orientation));

    n = 0;
    if (scaleflag)
    {
      if (!myres->orientation)
      {
        XtSetArg (args[n], XmNscaleWidth, thisw->width); n++;
      }
      else
      {
        XtSetArg (args[n], XmNscaleWidth, thisw->height); n++;
      }

      XtSetValues(thisw->me, args, n);

    }

    n = 0;
    switch (myres->orientation) {
        case 0:
        XtSetArg (args[n], XmNorientation, XmVERTICAL); n++;
        break;
        case 1:
        XtSetArg (args[n], XmNorientation, XmHORIZONTAL); n++;
        break;
        default:
        break;
    }
    XtSetValues(thisw->me, args, n);

  }
  else
    return 0;

  if (!mib_read_line(fin, res, val))
    return 0;

  if (strcmp(res,"EndWidget"))
    return 0;

  return 1;
}

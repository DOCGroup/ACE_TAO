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

ACE_RCSID(mpeg_mib, mibRadioBox, "$Id$")

extern Display	*dpy;
extern GC	 mib_gc;

/* Code for RadioBox */
/*****************************************************************************/

mib_Widget *mib_create_RadioBox(mib_Widget *parent, char *name, char *label,
		int posx, int posy, int width, int height, int mib_fill)
{
  mib_Widget    *temp;
  mib_RadioBox  *myres;
  Widget	wtemp;
  unsigned char *label_text;
  char		label_temp[50];
  Arg		args[20];
  int		n;

  /* create the new widget and add it to the tree */

  temp = mib_new_mib_Widget();
  if (mib_fill == WDEFAULT)
    mib_add_backward(temp, parent);
  else
    mib_add_mib_Widget(temp, parent);

  myres = (mib_RadioBox *)ACE_OS::malloc(sizeof(mib_RadioBox));

  /* initialize public resources */

  if (mib_fill == WDEFAULT)
  {
    temp->name = (char *)ACE_OS::malloc(strlen(name)+1);
    strcpy(temp->name,name);
  }
  temp->mib_class = (char *)ACE_OS::malloc(9);
  sprintf(temp->mib_class,"RadioBox");
  temp->mib_class_num = MIB_RADIOBOX;

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

  temp->mib_allowresize = 0;

  /* initialize private resources */

  temp->myres = (void *)myres;

  myres->numlabel = 0;

  if (mib_fill == WDEFAULT)
  {
    myres->numlabel = 2;
    myres->labels = (char **)ACE_OS::malloc(10);
    myres->buttons = (Widget *)ACE_OS::malloc(sizeof(Widget) * myres->numlabel);
    sprintf(label_temp, "Radio 1");
    myres->labels[0] = (char *)ACE_OS::malloc(strlen(label_temp)+1);
    strcpy(myres->labels[0],label_temp);
    sprintf(label_temp, "Radio 2");
    myres->labels[1] = (char *)ACE_OS::malloc(strlen(label_temp)+1);
    strcpy(myres->labels[1],label_temp);
  }


  /* create Xt widget */

  n = 0;

  XtSetArg (args[n], XmNrubberPositioning, False); n++;

  if (mib_fill == WDEFAULT)
  {
    XtSetArg (args[n], XmNleftAttachment, XmATTACH_FORM); n++;
    XtSetArg (args[n], XmNleftOffset, posx);n++;
    XtSetArg (args[n], XmNtopAttachment, XmATTACH_FORM); n++;
    XtSetArg (args[n], XmNtopOffset, posy);n++;
    XtSetArg (args[n], XmNwidth, width); n++;
    XtSetArg (args[n], XmNheight, height); n++;
  }

  XtSetArg (args[n], XmNhighlightThickness, 0); n++;

  temp->me = XmCreateRadioBox(temp->parent->me, "Radio", args, n);
  wtemp = temp->me;


  if (mib_fill == WDEFAULT)
  {
    n = 0;
    XtSetArg (args[n], XmNhighlightThickness, 0); n++;

    XtCreateManagedWidget("Radio 1", xmToggleButtonGadgetClass,
		wtemp, args, n);

    n = 0;
    XtSetArg (args[n], XmNhighlightThickness, 0); n++;
    XtCreateManagedWidget("Radio 2", xmToggleButtonGadgetClass,
		wtemp, args, n);
  }


  XtManageChild(wtemp);

  if (mib_fill == WEDIT || mib_fill == WDEFAULT)
  {
    mib_apply_eventhandlers(temp->me, temp);
  }

  return temp;
}

void mib_delete_RadioBox(mib_Widget *thisw)
{
  mib_RadioBox *temp = (mib_RadioBox *)thisw->myres;
  int		count;

  for (count=0; count < temp->numlabel; count++)
    ACE_OS::free(temp->labels[count]);

  ACE_OS::free(temp);
}

void mib_save_RadioBox(mib_Widget *thisw, FILE *fout)
{
  mib_RadioBox *temp = (mib_RadioBox *)thisw->myres;
  int count;

  fprintf(fout,"numlabel: %d\\n\\\n", temp->numlabel);

  for (count=0; count < temp->numlabel; count++)
  {
    fprintf(fout,"label: \\\"%s\\\"\\n\\\n", temp->labels[count]);
  }
}

int mib_load_RadioBox(mib_Widget *thisw, mib_Buffer *fin)
{
  char		res[MI_MAXSTRLEN];
  char		val[MI_MAXSTRLEN];
  int		count, vallen, n;
  Arg		args[5];
  mib_RadioBox *myres;

  myres = (mib_RadioBox *)thisw->myres;

  if (!mib_read_line(fin, res, val))
    return 0;

  if (!strcmp(res,"numlabel"))
  {
    myres->numlabel = 0;
    sscanf(val,"%d",&(myres->numlabel));
    if (!(myres->numlabel))
      return 0;

    myres->labels = (char **)ACE_OS::malloc((myres->numlabel+1)*4);
    myres->buttons = (Widget *)ACE_OS::malloc(sizeof(Widget)*(myres->numlabel));
    for (count=0; count < myres->numlabel; count++)
    {
      if (!mib_read_line(fin, res, val))
	return 0;
      vallen = strlen(val);
      if (vallen < 2)
	return 0;
      val[vallen-1] = '\0';
      myres->labels[count] = (char *)ACE_OS::malloc(vallen-1);
      sprintf(myres->labels[count],"%s",&(val[1]));

      n = 0;
      XtSetArg (args[n], XmNhighlightThickness, 0); n++;

      myres->buttons[count] = XtCreateManagedWidget(myres->labels[count],
		xmToggleButtonGadgetClass, thisw->me, args, n);

    }
  }
  else
    return 0;

  if (!mib_read_line(fin, res, val))
    return 0;
  if (strcmp(res,"EndWidget"))
    return 0;

  return 1;
}

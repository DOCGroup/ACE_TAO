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

ACE_RCSID(mpeg_mib, mibMenu, "$Id$")

extern Display	*dpy;
extern GC	mib_gc;
static int	delhandler; /* delay adding event handler until menu
					is actually created */

/* Code for Menu */
/*****************************************************************************/

mib_Widget *mib_create_Menu(mib_Widget *parent, char *name, char *label,
		int posx, int posy, int width, int height, int mib_fill)
{
  mib_Widget	*temp;
  mib_Menu	*myres;
  Widget	 wtemp;
  char		 label_temp[50];
  Arg		 args[20];
  int		 n;

  /* create the new widget and add it to the tree */

  temp = mib_new_mib_Widget();
  if (mib_fill == WDEFAULT)
    mib_add_backward(temp, parent);
  else
    mib_add_mib_Widget(temp, parent);

  myres = (mib_Menu *)ACE_OS::malloc(sizeof(mib_Menu));

  /* initialize public resources */

  if (mib_fill == WDEFAULT)
  {
    temp->name = (char *)ACE_OS::malloc(strlen(name)+1);
    strcpy(temp->name,name);
  }
  temp->mib_class = (char *)ACE_OS::malloc(8);
  sprintf(temp->mib_class,"Menu");
  temp->mib_class_num = MIB_MENU;
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
  myres->numitems = 0;

  /* create Xt widget */

  n = 0;

  if (mib_fill == WDEFAULT)
  {
    XtSetArg (args[n], XmNleftAttachment, XmATTACH_FORM); n++;
    XtSetArg (args[n], XmNleftOffset, posx);n++;
    XtSetArg (args[n], XmNtopAttachment, XmATTACH_FORM); n++;
    XtSetArg (args[n], XmNtopOffset, posy);n++;
  }
  XtSetArg (args[n], XmNrubberPositioning, False); n++;

  temp->me = XtCreateManagedWidget(name, xmRowColumnWidgetClass,
                temp->parent->me, args, n);

  n = 0;

  delhandler = 1;
  if (mib_fill == WDEFAULT)
  {
    delhandler = 0;
    myres->numitems = 3;
    myres->my_menu = (MenuItem *)ACE_OS::malloc(sizeof(MenuItem) * (myres->numitems+1));
    myres->items = (Widget *)ACE_OS::malloc(sizeof(Widget) * myres->numitems);
    sprintf(label_temp, "Item 1");
    myres->my_menu[0].label = (char *)ACE_OS::malloc(strlen(label_temp)+1);
    strcpy(myres->my_menu[0].label,label_temp);
    myres->my_menu[0].wclass = &xmPushButtonGadgetClass;
    myres->my_menu[0].mnemonic = '1';
    myres->my_menu[0].accelerator = NULL;
    myres->my_menu[0].accel_text = NULL;
    myres->my_menu[0].accel_text = NULL;
    myres->my_menu[0].callback = NULL;
    myres->my_menu[0].callback_data = NULL;
    myres->my_menu[0].subitems = (struct _menu_item *)1;

    sprintf(label_temp, "Item 2");
    myres->my_menu[1].label = (char *)ACE_OS::malloc(strlen(label_temp)+1);
    strcpy(myres->my_menu[1].label,label_temp);
    myres->my_menu[1].wclass = &xmPushButtonGadgetClass;
    myres->my_menu[1].mnemonic = '2';
    myres->my_menu[1].accelerator = NULL;
    myres->my_menu[1].accel_text = NULL;
    myres->my_menu[1].accel_text = NULL;
    myres->my_menu[1].callback = NULL;
    myres->my_menu[1].callback_data = NULL;
    myres->my_menu[1].subitems = (struct _menu_item *)1;

    sprintf(label_temp, "Item 3");
    myres->my_menu[2].label = (char *)ACE_OS::malloc(strlen(label_temp)+1);
    strcpy(myres->my_menu[2].label,label_temp);
    myres->my_menu[2].wclass = &xmPushButtonGadgetClass;
    myres->my_menu[2].mnemonic = '2';
    myres->my_menu[2].accelerator = NULL;
    myres->my_menu[2].accel_text = NULL;
    myres->my_menu[2].accel_text = NULL;
    myres->my_menu[2].callback = NULL;
    myres->my_menu[2].callback_data = NULL;
    myres->my_menu[2].subitems = (struct _menu_item *)1;

    myres->my_menu[3].label = NULL;

    wtemp = BuildMenu(temp->me, XmMENU_OPTION, NULL, 'M', myres->my_menu);
    XtManageChild(wtemp);
  }

  if (mib_fill == WEDIT || mib_fill == WDEFAULT)
  {

    /* default menu */

    if (!delhandler)
    {
      mib_apply_eventhandlers(wtemp, temp);
      mib_apply_eventhandlers(temp->me, temp);
    }
  }

  if (mib_fill == WEMPTY)
    delhandler = 0;

  return temp;
}

void mib_delete_Menu(mib_Widget *thisw)
{
  mib_Menu *temp = (mib_Menu *)thisw->myres;

  ACE_OS::free(temp);
}

void mib_save_Menu(mib_Widget *thisw, FILE *fout)
{
  mib_Menu *temp = (mib_Menu *)thisw->myres;
  int count;

  fprintf(fout, "numitems: %d\\n\\\n", temp->numitems);

  for (count=0; count < temp->numitems; count++)
  {
    fprintf(fout, "item: \\\"%s\\\"\\n\\\n", temp->my_menu[count].label);
  }

}

int mib_load_Menu(mib_Widget *thisw, mib_Buffer *fin)
{
  char          res[MI_MAXSTRLEN];
  char          val[MI_MAXSTRLEN];
  int		count, vallen;
  Widget	wtemp;
  mib_Menu     *myres = (mib_Menu *)thisw->myres;
  

  if (!mib_read_line(fin, res, val))
    return 0;

  if (!strcmp(res, "numitems"))
  {
    myres->numitems = 0;
    sscanf(val, "%d", &(myres->numitems));
    if (!(myres->numitems))
      return 0;

    myres->my_menu = (MenuItem *)ACE_OS::malloc(sizeof(MenuItem) * (myres->numitems+1));
    myres->items = (Widget *)ACE_OS::malloc(sizeof(Widget) * myres->numitems);
    for (count = 0; count < myres->numitems; count++)
    {
      if (!mib_read_line(fin, res, val))
	return 0;
      vallen = strlen(val);
      if (vallen < 2)
	return 0;
      val[vallen-1] = '\0';
      myres->my_menu[count].label = (char *)ACE_OS::malloc(vallen-1);
      sprintf(myres->my_menu[count].label, "%s", &(val[1]));
      myres->my_menu[count].wclass = &xmPushButtonGadgetClass;
      myres->my_menu[count].mnemonic = NULL;
      myres->my_menu[count].accelerator = NULL;
      myres->my_menu[count].accel_text = NULL;
      myres->my_menu[count].accel_text = NULL;
      myres->my_menu[count].callback = NULL;
      myres->my_menu[count].callback_data = NULL;
      myres->my_menu[count].subitems = (struct _menu_item *)1;
    }
    myres->my_menu[myres->numitems].label = NULL;

    wtemp = BuildMenu(thisw->me, XmMENU_OPTION, NULL, NULL, myres->my_menu);
    XtManageChild(wtemp);

    if (delhandler)
    {
      mib_apply_eventhandlers(wtemp, thisw);
      mib_apply_eventhandlers(thisw->me, thisw);
    }
    else
      thisw->me = wtemp;

    for (count = 0; count < myres->numitems; count++)
    {
      /* thisw hack extracts the widget for each item in the menu
	 for possible use by the programmer */
      myres->items[count] = (Widget)(myres->my_menu[count].subitems);
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

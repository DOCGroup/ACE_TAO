/* $Id$ */

/* Copyright (c) 1994 The Geometry Center; University of Minnesota
   1300 South Second Street;  Minneapolis, MN  55454, USA;

This file is free software; you can redistribute it and/or modify it only
under the the terms of the GNU GENERAL PUBLIC LICENSE which should be
included along with thisw file.  This software may be obtained via anonymous
ftp from geom.umn.edu; email: software@geom.umn.edu. */

/* Author: Daeron Meyer */

#include <stdlib.h>
#include "mibload.h"
#include "mibwidgets.h"

ACE_RCSID(mpeg_mib, mibload, "$Id$")

mib_Widget	*mib_root_Widget;
Display		*dpy;
GC		 mib_gc;
static struct _mib_event_handle_funcs {
#ifdef __cplusplus
  void		(*mib_pick_mib_Widget)( Widget, XtPointer,
			XEvent *, Boolean * );
  void		(*mib_move_mib_Widget)( Widget, XtPointer,
			XEvent *, Boolean *);
  void		(*mib_unpick_mib_Widget)( Widget, XtPointer,
			XEvent *, Boolean * );
#else
  void		(*mib_pick_mib_Widget)(/* Widget, XtPointer,
			XButtonPressedEvent *, Boolean * */);
  void		(*mib_move_mib_Widget)(/* Widget, XtPointer,
			XPointerMovedEvent *, Boolean * */);
  void		(*mib_unpick_mib_Widget)(/* Widget, XtPointer,
			XButtonReleasedEvent *, Boolean * */);
#endif
} mib_events;

mib_widget_funcs mwfuncs[] =
{
  { NULL, NULL, NULL, NULL, NULL },
  { "TextBox", mib_create_TextBox, mib_delete_TextBox,
	mib_save_TextBox, mib_load_TextBox},
  { "Button", mib_create_Button, mib_delete_Button,
	mib_save_Button, mib_load_Button},
  { "Toggle", mib_create_Toggle, mib_delete_Toggle,
	mib_save_Toggle, mib_load_Toggle},
  { "RadioBox", mib_create_RadioBox, mib_delete_RadioBox,
	mib_save_RadioBox, mib_load_RadioBox},
  { "DrawingArea", mib_create_DrawingArea, mib_delete_DrawingArea,
	mib_save_DrawingArea, mib_load_DrawingArea},
  { "Label", mib_create_Label, mib_delete_Label,
	mib_save_Label, mib_load_Label},
  { "Frame", mib_create_Frame, mib_delete_Frame,
	mib_save_Frame, mib_load_Frame},
  { "ScrollBar", mib_create_ScrollBar, mib_delete_ScrollBar,
	mib_save_ScrollBar, mib_load_ScrollBar},
  { "TextBig", mib_create_TextBig, mib_delete_TextBig,
	mib_save_TextBig, mib_load_TextBig},
  { "List", mib_create_List, mib_delete_List,
	mib_save_List, mib_load_List},
  { "Scale", mib_create_Scale, mib_delete_Scale,
	mib_save_Scale, mib_load_Scale},
  { "Menu", mib_create_Menu, mib_delete_Menu,
	mib_save_Menu, mib_load_Menu},
  { NULL, NULL, NULL, NULL, NULL },
};

/*****************************************************************************/

void mib_add_mib_Widget(mib_Widget *thisw, mib_Widget *parent)
{
  mib_Widget *tmp;

  if (parent->child == NULL)
  {
    parent->child = thisw;
    thisw->prev = parent;
    thisw->parent = parent;
    thisw->sibling = NULL;
    thisw->child = NULL;
  }
  else
    {
      tmp = parent->child;
      while (tmp->sibling != NULL)
        tmp = tmp->sibling;
      tmp->sibling = thisw;
      thisw->prev = tmp;
      thisw->parent = parent;
      thisw->sibling = NULL;
      thisw->child = NULL;
    }
}

/*****************************************************************************/

void mib_add_backward(mib_Widget *thisw, mib_Widget *parent)
{
  mib_Widget *tmp;

  if (parent->child == NULL)
  {
    parent->child = thisw;
    thisw->prev = parent;
    thisw->parent = parent;
    thisw->sibling = NULL;
    thisw->child = NULL;
  }
  else
    {
      tmp = parent->child;
      parent->child = thisw;
      thisw->prev = parent;
      thisw->parent = parent;
      thisw->sibling = tmp;
      thisw->child = NULL;
      tmp->prev = thisw;
    }
}

/*****************************************************************************/

void mib_remove_mib_Widget(mib_Widget *thisw)
{
  int		 count;
  mib_Widget	*pnt;

  XtVaSetValues(mib_root_Widget->me, XmNresizePolicy, XmRESIZE_NONE, NULL);
  XtDestroyWidget(thisw->me);

  while (thisw->child != NULL)
    mib_remove_mib_Widget(thisw->child);

  if (thisw->parent == thisw)
  {
    mib_clear_myres(thisw);
    return;
  }

  if (thisw->prev == thisw->parent)
  {
    thisw->parent->child = thisw->sibling;
    if (thisw->sibling != NULL)
      thisw->sibling->prev = thisw->parent;
  }
  else
    {
      thisw->prev->sibling = thisw->sibling;
      if (thisw->sibling != NULL)
	thisw->sibling->prev = thisw->prev;
    }

  mib_clear_myres(thisw);
}

/*****************************************************************************/

void mib_clear_myres(mib_Widget *thisw)
{
  ACE_OS::free(thisw->mib_class);
  ACE_OS::free(thisw->name);

  if ((thisw->mib_class_num < 1) || (thisw->mib_class_num > MI_NUMCLASSES))
    return;

  mwfuncs[thisw->mib_class_num].mib_delete(thisw);
  ACE_OS::free(thisw);
}

/*****************************************************************************/

mib_Widget *mib_new_mib_Widget()
{
  mib_Widget *thisw;
  thisw = (mib_Widget *)ACE_OS::malloc(sizeof(mib_Widget));
  thisw->me = NULL;
  thisw->mib_class_num = MIB_NULL;
  thisw->mib_selected = 0;
  thisw->mib_resizing = 0;
  thisw->myres = NULL;
  thisw->parent = NULL;
  thisw->sibling = NULL;
  thisw->prev = NULL;
  thisw->child = NULL;
  thisw->width = 0;
  thisw->height = 0;
  thisw->topAttachment = 0;
  thisw->bottomAttachment = 0;
  thisw->leftAttachment = 0;
  thisw->rightAttachment = 0;
  thisw->topOffset = 0;
  thisw->bottomOffset = 0;
  thisw->leftOffset = 0;
  thisw->rightOffset = 0;

  return thisw;
}

/*****************************************************************************/

mib_Widget *mib_find_name(mib_Widget *temp, char *name)
{
  mib_Widget *child = temp->child;
  mib_Widget *ret = NULL;

  if (!strcmp(temp->name, name))
    return temp;

  if (child != NULL)
    if (ret = mib_find_name(child, name))
	return ret;

  child = temp->sibling;
  if (child != NULL)
    if (ret = mib_find_name(child, name))
	return ret;

  return NULL;
}

/*****************************************************************************/

Widget
BuildMenu(Widget parent, int menu_type, char *menu_title, char menu_mnemonic,
		 MenuItem *items)
{
    Widget menu, cascade, widget;
    int i;
    XmString str;

    if (menu_type == XmMENU_PULLDOWN || menu_type == XmMENU_OPTION)
        menu = XmCreatePulldownMenu(parent, "_pulldown", NULL, 0);
    else if (menu_type == XmMENU_POPUP)
        menu = XmCreatePopupMenu(parent, "_popup", NULL, 0);
    else {
        XtWarning("Invalid menu type passed to BuildMenu()");
        return NULL;
    }

    /* Pulldown menus require a cascade button to be made */
    if (menu_type == XmMENU_PULLDOWN) {
        str = XmStringCreateSimple(menu_title);
        cascade = XtVaCreateManagedWidget(menu_title,
            xmCascadeButtonGadgetClass, parent,
            XmNsubMenuId,   menu,
            XmNlabelString, str,
            XmNmnemonic,    menu_mnemonic,
            NULL);
        XmStringFree(str);
    } else if (menu_type == XmMENU_OPTION) {
        /* Option menus are a special case, but not hard to handle */
        Arg args[2];
        str = XmStringCreateSimple(menu_title);
        XtSetArg(args[0], XmNsubMenuId, menu);
        XtSetArg(args[1], XmNlabelString, str);
        /* This really isn't a cascade, but thisw is the widget handle
         * we're going to return at the end of the function.
         */
        cascade = XmCreateOptionMenu(parent, menu_title, args, 2);
        XmStringFree(str);
    }

    /* Now add the menu items */
    for (i = 0; items[i].label != NULL; i++) {
        /* If subitems exist, create the pull-right menu by calling thisw
         * function recursively.  Since the function returns a cascade
         * button, the widget returned is used..
         */
        if (items[i].subitems)
            if (menu_type == XmMENU_OPTION) {
            widget = XtVaCreateManagedWidget(items[i].label,
                *items[i].wclass, menu, NULL);
		items[i].subitems = (struct _menu_item *) widget;
	     /* daeron mod (tm) :-) ... we now use thisw to pass back each
		widget we create to the mibMenu functions so that it can
		be stored as part of the mibMenu structure */

             /* XtWarning("You can't have submenus from option menu items.");
                continue;*/
            } else
                widget = BuildMenu(menu, XmMENU_PULLDOWN,
                    items[i].label, items[i].mnemonic, items[i].subitems);
        else
	  {
            widget = XtVaCreateManagedWidget(items[i].label,
                *items[i].wclass, menu,
                NULL);
	    /* ditto here from above ... - Daeron mod (tm) */
	    items[i].subitems = (struct _menu_item *) widget;
	  }

        /* Whether the item is a real item or a cascade button with a
         * menu, it can still have a mnemonic.
         */
        if (items[i].mnemonic)
            XtVaSetValues(widget, XmNmnemonic, items[i].mnemonic, NULL);

        /* any item can have an accelerator, except cascade menus. But,
         * we don't worry about that; we know better in our declarations.
         */
        if (items[i].accelerator) {
            str = XmStringCreateSimple(items[i].accel_text);
            XtVaSetValues(widget,
                XmNaccelerator, items[i].accelerator,
                XmNacceleratorText, str,
                NULL);
            XmStringFree(str);
        }

        if (items[i].callback)
            XtAddCallback(widget,
                (items[i].wclass == &xmToggleButtonWidgetClass ||
                 items[i].wclass == &xmToggleButtonGadgetClass)?
                    XmNvalueChangedCallback : /* ToggleButton class */
                    XmNactivateCallback,      /* PushButton class */
                items[i].callback, items[i].callback_data);
    }

    /* for popup menus, just return the menu; pulldown menus, return
     * the cascade button; option menus, return the thing returned
     * from XmCreateOptionMenu().  This isn't a menu, or a cascade button!
     */
    return menu_type == XmMENU_POPUP? menu : cascade;
}

/*****************************************************************************/

mib_Widget *mib_load_interface(Widget parent, char *from, int file_type)
{
  mib_Buffer  thiswfile;
  mib_Widget *thisw;
  FILE	     *infile;
  char	     *instring;
  char	      ch;

  thiswfile.buf_type = file_type;

  dpy = XtDisplay(parent);

  if ((file_type == MI_FROMFILE) || (file_type == MI_EDITFROMFILE))
  {
    if (!(infile = fopen(from,"r")))
      return NULL;

    ch = '\0';
    while ((ch != '\n')&&(!feof(infile)))  /* throw away first line */
      ch = (char)fgetc(infile);

    thiswfile.buffer = (void *)infile;
    thiswfile.point = 0;

    if (!mib_load_Root(parent, &thisw, &thiswfile))
    {
      /* delete thisw */
      return NULL;
    }
    else
    {
      fclose(infile);
      return thisw;
    }
  }
  else
  if ((file_type == MI_FROMSTRING) || (file_type == MI_EDITFROMSTRING))
  {
    instring = from;
    if (instring == NULL)
      return NULL;

    thiswfile.buffer = (void *)instring;
    thiswfile.buflen =ACE_OS::strlen (instring);
    thiswfile.point = 0;

    if (!mib_load_Root(parent, &thisw, &thiswfile))
    {
      /* delete thisw */
      return NULL;
    }
    else
      return thisw;
  }
  else
    return NULL;

}

/*****************************************************************************/

int mib_load_mib_class(mib_Widget **thisw, mib_Widget *parent, char *name,
		char *iname, mib_Buffer *fin )
{
  int namelen, editstate, count, set;

  if ((fin->buf_type == MI_EDITFROMFILE) ||
	(fin->buf_type == MI_EDITFROMSTRING))
    editstate = WEDIT;
  else
    editstate = WEMPTY;

  namelen =ACE_OS::strlen (name);
  if (namelen < 2)
    return 0;

  name[namelen-1] = '\0';
  name[0] = ' ';

  count = 1;

  while (mwfuncs[count].name)
  {
    if (!strcmp(&(name[1]), mwfuncs[count].name))
    {
      *thisw = mwfuncs[count].mib_create(parent, iname, NULL, 0, 0, 0, 0,
		editstate);
      return 1;
    }
    count++;
  }

  (*thisw)->parent = (*thisw);
  return 1;
}

/*****************************************************************************/

mib_Widget *mib_load_public(mib_Widget *root, mib_Widget *thisw, mib_Buffer *fin)
{
  int	got_line, done;
  char	res[MI_MAXSTRLEN];
  char	val[MI_MAXSTRLEN];
  char	valcp[MI_MAXSTRLEN];
  Arg	args[20];
  int	mynum, n;
 
  got_line = 1;
  done = 0;

  /* thisw loop reads basic info about Widget */
  while (got_line && (!done))
  {
    got_line = mib_read_line(fin, res, val);
    if (!strcmp(res,"Ref"))
      sscanf(val, "%d", &mynum);
    else
    if (!strcmp(res,"Widget"))
    {
      strcpy(valcp,val);
      done = 1;
    }
  }

  done = 0;

  while (got_line && (!done))
  {
    got_line = mib_read_line(fin, res, val);
    if (!strcmp(res, "Children"))
      n = 0;
    else
    if (!strcmp(res, "Parent")) /* don't support complete widget tree yet */
      n = 0;
    else
    if (!strcmp(res,"Public-"))
      n = 0;
    else
    if (!strcmp(res,"Name"))
    {
      val[strlen(val)-1] = '\0';
      mib_load_mib_class(&thisw, root, valcp, &(val[1]), fin);
      thisw->name = (char *)ACE_OS::malloc(strlen(val));
      sprintf(thisw->name,"%s",&(val[1]));
      thisw->mib_mynum = mynum;
      done = 1;
    }
    else
       return 0;
  }

  if (!got_line)
    return NULL;

  done = 0;

  /* second loop reads public info */
  while (got_line && (!done))
  {
    got_line = mib_read_line(fin, res, val);
    if (!strcmp(res,"Xmwidth"))
      sscanf(val,"%d",&(thisw->width));
    else
    if (!strcmp(res,"Xmheight"))
      sscanf(val,"%d",&(thisw->height));
    else
    if (!strcmp(res,"XmtopAttachment"))
      sscanf(val,"%d",&(thisw->topAttachment));
    else
    if (!strcmp(res,"XmbottomAttachment"))
      sscanf(val,"%d",&(thisw->bottomAttachment));
    else
    if (!strcmp(res,"XmleftAttachment"))
      sscanf(val,"%d",&(thisw->leftAttachment));
    else
    if (!strcmp(res,"XmrightAttachment"))
      sscanf(val,"%d",&(thisw->rightAttachment));
    else
    if (!strcmp(res,"XmtopOffset"))
      sscanf(val,"%d",&(thisw->topOffset));
    else
    if (!strcmp(res,"XmbottomOffset"))
      sscanf(val,"%d",&(thisw->bottomOffset));
    else
    if (!strcmp(res,"XmleftOffset"))
      sscanf(val,"%d",&(thisw->leftOffset));
    else
    if (!strcmp(res,"XmrightOffset"))
      sscanf(val,"%d",&(thisw->rightOffset));
    else
    if (!strcmp(res,"Private-"))
      done = 1;
  }

  n = 0;
  if ((fin->buf_type == MI_EDITFROMFILE) ||
	(fin->buf_type == MI_EDITFROMSTRING))
  {
    XtSetArg (args[n], XmNleftAttachment, XmATTACH_FORM); n++;
    XtSetArg (args[n], XmNleftOffset, thisw->leftOffset); n++;
    XtSetArg (args[n], XmNtopAttachment, XmATTACH_FORM); n++;
    XtSetArg (args[n], XmNtopOffset, thisw->topOffset); n++;
    if (thisw == root)
    {
      XtSetArg (args[n], XmNrightAttachment, XmATTACH_FORM); n++;
      XtSetArg (args[n], XmNrightOffset, thisw->rightOffset); n++;
      XtSetArg (args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
      XtSetArg (args[n], XmNbottomOffset, thisw->bottomOffset); n++;
    }
  }
  else
  {
    if (thisw->leftAttachment)
    {
      XtSetArg (args[n], XmNleftAttachment, XmATTACH_FORM); n++;
      XtSetArg (args[n], XmNleftOffset, thisw->leftOffset);n++;
    }
    if (thisw->topAttachment)
    {
      XtSetArg (args[n], XmNtopAttachment, XmATTACH_FORM); n++;
      XtSetArg (args[n], XmNtopOffset, thisw->topOffset);n++;
    }
    if (thisw->bottomAttachment)
    {
      XtSetArg (args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
      XtSetArg (args[n], XmNbottomOffset, thisw->bottomOffset);n++;
    }
    if (thisw->rightAttachment)
    {
      XtSetArg (args[n], XmNrightAttachment, XmATTACH_FORM); n++;
      XtSetArg (args[n], XmNrightOffset, thisw->rightOffset);n++;
    }
  }

  XtSetArg (args[n], XmNwidth, thisw->width); n++;
  XtSetArg (args[n], XmNheight, thisw->height); n++;

  XtSetValues(thisw->me, args, n);

  return thisw;
}

/*****************************************************************************/

int mib_load_private(mib_Widget *thisw, mib_Buffer *fin)
{

  if (thisw->mib_class_num == MIB_NULL)
    return 1;

  if ((thisw->mib_class_num < 1) || (thisw->mib_class_num > MI_NUMCLASSES))
    return 0;

  mwfuncs[thisw->mib_class_num].mib_load(thisw, fin);

  return 1;
}

/*****************************************************************************/

int mib_load_Root(Widget parent, mib_Widget **thisw, mib_Buffer *fin)
{

  char		res[MI_MAXSTRLEN];
  char		val[MI_MAXSTRLEN];
  char		name[MI_MAXSTRLEN];
  int		num_widgets, count, n, got_line;
  Arg		args[20];
  XGCValues     gcvals;
  XtGCMask      val_mask;
  mib_Widget   *temp;

  got_line = mib_read_line(fin, res, val);
  if (!strcmp(res,"TotalWidgets"))
    sscanf(val, "%d", &num_widgets);
  else
    return 0;

  (*thisw) = mib_new_mib_Widget();
  (*thisw)->mib_class = (char*)ACE_OS::malloc(9);
  sprintf((*thisw)->mib_class,"RootForm");

/*  (*thisw)->me = XmCreateForm(parent, "MainForm", args, 0); */
  
  (*thisw)->me = parent;

  if (!mib_load_public((*thisw), (*thisw), fin))
    return 0;

  /* we don't expect any private resources for the root widget */

  got_line = mib_read_line(fin, res, val);
  if (ACE_OS::strcmp (res,"EndWidget."))
    return 0;

/*   XtManageChild((*thisw)->me);
*/
  XtVaSetValues((*thisw)->me, XmNresizePolicy, XmRESIZE_NONE, NULL);

  count = num_widgets - 1;
  while (count > 0)
  {

    if (!(temp = mib_load_public((*thisw), temp, fin)))
    {
      /* delete temp */
      return 0;
    }

    if (!mib_load_private(temp,fin))
    {
      /* delete temp */
      return 0;
    }
    count--;

  }

  mib_reset_size((*thisw));

  XtVaSetValues((*thisw)->me, XmNresizePolicy, XmRESIZE_ANY, NULL);

  val_mask = (long)0;
  mib_gc = XtGetGC((*thisw)->me, val_mask, &gcvals);

  return 1;
}

/*****************************************************************************/

int mib_read_line(mib_Buffer *bufin, char *res, char *val)
{
  FILE *fin;
  char *strin;
  char  ch;
  int   count, mark;
  char  inbuf[MI_MAXSTRLEN];

  if ((bufin->buf_type == MI_FROMFILE) || (bufin->buf_type == MI_EDITFROMFILE))
  {
    fin = (FILE *)bufin->buffer;
    ch = '\0';
    count = 0;
    mark = 0;
    while ((ch != '\n')&&(!feof(fin))&&(count<MI_MAXSTRLEN))
    {
      ch = (char)fgetc(fin);
      if ((mark == 0) && (ch == ':'))
	mark = count;
      if ((ch != '\\')&&(ch != '\n'))
      {
	inbuf[count] = ch;
	count++;
      }
    }
    if (feof(fin))
      return 0;
    inbuf[count] = '\0';
    if (count > 0)
    {
      if (inbuf[count-1] == 'n')
        inbuf[count-1] = '\0';
    }
    else
      return 0;

  }
  else
  if ((bufin->buf_type == MI_FROMSTRING) ||
	(bufin->buf_type == MI_EDITFROMSTRING))
  {
    strin = (char *)bufin->buffer;
    count = bufin->point;
    mark = 0;

    if (count >= bufin->buflen)
      return 0;

    while ((strin[count] != '\n') && (count < bufin->buflen))
    {
      if ((mark == 0) && (strin[count] == ':'))
	mark = count;
      count++;
    }

    strin[count] = '\0';
    if (count >= bufin->buflen)
      return 0;
    sprintf(inbuf,"%s",&(strin[bufin->point]));
    strin[count] = '\n';
    if (mark != 0)
      mark -= bufin->point;
    bufin->point = count+1;
  }
  else
    return 0;

  if (mark == 0)
  {
    sprintf(res,"%s",inbuf);
    sprintf(val,"\0");
  }
  else
  {
    inbuf[mark] = '\0';
    sprintf(res,"%s",inbuf);
    inbuf[mark] = ' ';
    if ((int)(strlen(inbuf)-mark) > 1)
      sprintf(val,"%s",&(inbuf[mark+2]));
    else
      sprintf(val,"\0");
  }

  return 1;
}

/*****************************************************************************/

void mib_reset_size(mib_Widget *temp)
{
  Arg	args[5];
  int	n;

  mib_Widget *child = temp->child;

  if (temp->mib_class_num != MIB_NULL)
  {
    n = 0;
    XtSetArg (args[n], XmNwidth, temp->width); n++;
    XtSetArg (args[n], XmNheight, temp->height); n++;

    XtSetValues(temp->me, args, n);
  }

  if (child != NULL)
    mib_reset_size(child);

  child = temp->sibling;
  if (child != NULL)
    mib_reset_size(child);
}

/*****************************************************************************/

#ifdef __cplusplus
void mib_set_eventhandlers(void a(Widget, XtPointer,
			XEvent *, Boolean * ),
                           void b( Widget, XtPointer,
			XEvent *, Boolean* ),
                           void c(Widget, XtPointer,
			XEvent *, Boolean *)
                           )
#else
void mib_set_eventhandlers(void * a, void * b, void * c)
#endif
{
  mib_events.mib_pick_mib_Widget =  a;
  mib_events.mib_move_mib_Widget =  b;
  mib_events.mib_unpick_mib_Widget = c;
}

/*****************************************************************************/

void mib_apply_eventhandlers(Widget thisw, mib_Widget *actual)
{
  XtAddEventHandler(thisw, ButtonPressMask, FALSE,
	mib_events.mib_pick_mib_Widget, (XtPointer)actual);
  XtAddEventHandler(thisw, Button3MotionMask, FALSE,
	mib_events.mib_move_mib_Widget, (XtPointer)actual);
  XtAddEventHandler(thisw, ButtonReleaseMask, FALSE,
	mib_events.mib_unpick_mib_Widget, (XtPointer)actual);

}

/*****************************************************************************/

/* $Id$ */
/* Copyright (c) 1994 The Geometry Center; University of Minnesota
   1300 South Second Street;  Minneapolis, MN  55454, USA;

This file is free software; you can redistribute it and/or modify it only
under the the terms of the GNU GENERAL PUBLIC LICENSE which should be
included along with this file.  This software may be obtained via anonymous
ftp from geom.umn.edu; email: software@geom.umn.edu. */

/* Author: Daeron Meyer */

#include <stdio.h>
#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <Xm/Xm.h>
#include <Xm/Text.h>
#include <Xm/TextF.h>
#include <Xm/RowColumn.h>
#include <Xm/MainW.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>
#include <Xm/PushBG.h>
#include <Xm/ToggleB.h>
#include <Xm/ToggleBG.h>
#include <Xm/DrawingA.h>
#include <Xm/CascadeBG.h>
#include <Xm/SeparatoG.h>
#include <Xm/Frame.h>
#include <Xm/BulletinB.h>
#include <Xm/Scale.h>
#include <Xm/ScrollBar.h>
#include <Xm/Form.h>
#include <Xm/List.h>
#include <Xm/FileSB.h>
#include <Xm/AtomMgr.h>
#include <Xm/Protocols.h>
#include <Xm/MwmUtil.h>


/* Structure of mib_Widget */
/*****************************************************************************/

typedef struct _mib_Widget {
  char          *mib_class;     /* name of mib class (Button, TextBox, etc..)*/
  int            mib_class_num; /* class number for ez reference :) */
  int            mib_mynum;     /* numbering for storage format */
  Widget         me;            /* Xt widget */
  int            mib_selected;  /* is selected */
  int            mib_resizing;  /* is resizing 1, or being moved 0 */
  int            mib_resizetype;/* 1  = upper left, 2 = upper, 3 = ... */
  int            mib_allowresize; /* can this widget be resized ? */

  int            clkx, clky;    /* where user clicked */

  void          *myres;         /* my resources (different for each widget) */

  char          *name;          /* reference name of this widget */
  int            width, height; /* width and height */

  int   topAttachment, bottomAttachment, leftAttachment, rightAttachment;
                                /* attachments 1=attached 0=not-attached */

  int   topOffset, bottomOffset, leftOffset, rightOffset;
                                /* offsets if attached */

  struct _mib_Widget *parent;   /* pointer to parent */
  struct _mib_Widget *sibling;  /* remaining linked list of sibling widgets */
  struct _mib_Widget *prev;     /* previous sibling or parent */
  struct _mib_Widget *child;    /* linked list of children widgets */
} mib_Widget;

/* mib_Buffer structure */
/*****************************************************************************/

typedef struct _mib_Buffer {
  void  *buffer;        /* pointer to either a file or a char string */
  int    buf_type;      /* type of buffer (defined above) */
  int    point;         /* pointer for string */
  int    buflen;        /* length of string buffer */
} mib_Buffer;

/* mib_Widget functions */
/*****************************************************************************/

void mib_add_mib_Widget(mib_Widget *, mib_Widget *);
void mib_add_backward_Widget(mib_Widget *, mib_Widget *);
void mib_remove_mib_Widget(mib_Widget *);
void mib_clear_myres(mib_Widget *);
mib_Widget *mib_new_mib_Widget();
mib_Widget *mib_find_name(mib_Widget *, char *);
mib_Widget *mib_load_interface(Widget, char *, int);
int mib_count_all(mib_Widget *, int);
int mib_load_Root(Widget, mib_Widget **, mib_Buffer *);
mib_Widget *mib_load_public(mib_Widget *, mib_Widget *, mib_Buffer *);
int mib_load_mib_class(mib_Widget **, mib_Widget *, char *, char *,
                mib_Buffer *);
int mib_load_private(mib_Widget *, mib_Buffer *);
void mib_reset_size(mib_Widget *);
int mib_read_line(mib_Buffer *, char *, char *);
void mib_set_eventhandlers(void *, void *, void *);
void mib_apply_eventhandlers(Widget, mib_Widget *);

/* supporting functions and structures */
/*****************************************************************************/

typedef struct _menu_item {
  char        *label;
  WidgetClass *class;
  char         mnemonic;
  char        *accelerator;
  char        *accel_text;
  void        (*callback)();
  XtPointer   callback_data;
  struct _menu_item *subitems;
} MenuItem;

Widget BuildMenu(Widget, int, char *, char, MenuItem *);

/* mib class numbers */

#define MIB_NULL                0
#define MIB_TEXTBOX             1
#define MIB_BUTTON              2
#define MIB_TOGGLE              3
#define MIB_RADIOBOX            4
#define MIB_DRAWINGAREA         5
#define MIB_LABEL               6
#define MIB_FRAME               7
#define MIB_SCROLLBAR           8
#define MIB_TEXTBIG             9
#define MIB_LIST                10
#define MIB_SCALE               11
#define MIB_MENU                12

/* number of classes */
#define MI_NUMCLASSES           12

/* for specifying creation of a widget with
   default private values, no values at all (empty),
   or no values and editable */

#define WDEFAULT                1
#define WEMPTY                  2
#define WEDIT                   3

/* for specifing whether we are loading an
   interface from a file or from a string and whether it
   is editable :) */

#define MI_FROMFILE             1
#define MI_EDITFROMFILE         2
#define MI_FROMSTRING           3
#define MI_EDITFROMSTRING       4

#define MI_MAXSTRLEN            200   /* maximum string length */

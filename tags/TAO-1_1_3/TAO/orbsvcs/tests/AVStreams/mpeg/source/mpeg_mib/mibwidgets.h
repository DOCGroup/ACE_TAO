/* $Id$ */
/* Copyright (c) 1994 The Geometry Center; University of Minnesota
   1300 South Second Street;  Minneapolis, MN  55454, USA;

This file is free software; you can redistribute it and/or modify it only
under the the terms of the GNU GENERAL PUBLIC LICENSE which should be
included along with this file.  This software may be obtained via anonymous
ftp from geom.umn.edu; email: software@geom.umn.edu. */

/* Author: Daeron Meyer */

/* Structure of mib_TextBox */
/*****************************************************************************/

typedef struct _mib_TextBox {
  char  *init_contents;         /* initial text contents */

} mib_TextBox;

/* mib_TextBox functions */
/*****************************************************************************/
mib_Widget *mib_create_TextBox(mib_Widget *, char *, char *, int, int, int,
                int, int);
void mib_delete_TextBox(mib_Widget *);
void mib_save_TextBox(mib_Widget *, FILE *);
int mib_load_TextBox(mib_Widget *, mib_Buffer *);


/* Structure of mib_Button */
/*****************************************************************************/

typedef struct _mib_Button {
  char *label;                  /* label on button */

} mib_Button;

/* mib_Button functions */
/*****************************************************************************/
mib_Widget *mib_create_Button(mib_Widget *, char *, char *, int, int, int,
                int, int);
void mib_delete_Button(mib_Widget *);
void mib_save_Button(mib_Widget *, FILE *);
int mib_load_Button(mib_Widget *, mib_Buffer *);


/* Structure of mib_Toggle */
/*****************************************************************************/

typedef struct _mib_Toggle {
  char *label;                  /* label on toggle */
  int   isize;

} mib_Toggle;

/* mib_Toggle functions */
/*****************************************************************************/
mib_Widget *mib_create_Toggle(mib_Widget *, char *, char *, int, int, int,
                int, int);
void mib_delete_Toggle(mib_Widget *);
void mib_save_Toggle(mib_Widget *, FILE *);
int mib_load_Toggle(mib_Widget *, mib_Buffer *);


/* Structure of mib_RadioBox */
/*****************************************************************************/
typedef struct _mib_RadioBox {
  char **labels;                /* array of labels */
  int    numlabel;              /* number of labels */
  Widget *buttons;              /* pointers to each button */

} mib_RadioBox;

/* mib_RadioBox functions */

/*****************************************************************************/
mib_Widget *mib_create_RadioBox(mib_Widget *, char *, char*, int, int, int,
                                        int, int);
void mib_delete_RadioBox(mib_Widget *);
void mib_save_RadioBox(mib_Widget *, FILE *);
int mib_load_RadioBox(mib_Widget *, mib_Buffer *);


/* Structure of mib_DrawingArea */
/*****************************************************************************/
typedef struct _mib_DrawingArea {
  int   nothing;        /* couldn't think of anything yet */

} mib_DrawingArea;

/* mib_DrawingArea functions */
/*****************************************************************************/
mib_Widget *mib_create_DrawingArea(mib_Widget *, char *, char *, int, int, int,
                int, int);
void mib_delete_DrawingArea(mib_Widget *);
void mib_save_DrawingArea(mib_Widget *, FILE *);
int mib_load_DrawingArea(mib_Widget *, mib_Buffer *);


/* Structure of mib_Label */
/*****************************************************************************/
typedef struct _mib_Label {
  char *label;          /* text in label */

} mib_Label;

/* mib_Label functions */
/*****************************************************************************/
mib_Widget *mib_create_Label(mib_Widget *, char *, char *, int, int, int,
                int, int);
void mib_delete_Label(mib_Widget *);
void mib_save_Label(mib_Widget *, FILE *);
int mib_load_Label(mib_Widget *, mib_Buffer *);


/* Structure of mib_Frame */
/*****************************************************************************/
typedef struct _mib_Frame {
  int   shadowtype;     /* 0 = in, 1 = out, 2 = etched_in, 3 = etched_out  */

} mib_Frame;

/* mib_Frame functions */
/*****************************************************************************/
mib_Widget *mib_create_Frame(mib_Widget *, char *, char *, int, int, int,
                int, int);
void mib_delete_Frame(mib_Widget *);
void mib_save_Frame(mib_Widget *, FILE *);
int mib_load_Frame(mib_Widget *, mib_Buffer *);


/* Structure of mib_ScrollBar */
/*****************************************************************************/
typedef struct _mib_ScrollBar {
  int   orientation;

} mib_ScrollBar;

/* mib_ScrollBar functions */
/*****************************************************************************/
mib_Widget *mib_create_ScrollBar(mib_Widget *, char *, char *, int, int, int,
                int, int);
void mib_delete_ScrollBar(mib_Widget *);
void mib_save_ScrollBar(mib_Widget *, FILE *);
int mib_load_ScrollBar(mib_Widget *, mib_Buffer *);


/* Structure of mib_TextBig */
/*****************************************************************************/
typedef struct _mib_TextBig {
  int   nothing;        /* couldn't think of anything yet */

} mib_TextBig;

/* mib_TextBig functions */
/*****************************************************************************/
mib_Widget *mib_create_TextBig(mib_Widget *, char *, char *, int, int, int, int, int);
void mib_delete_TextBig(mib_Widget *);
void mib_save_TextBig(mib_Widget *, FILE *);
int mib_load_TextBig(mib_Widget *, mib_Buffer *);


/* Structure of mib_List */
/*****************************************************************************/
typedef struct _mib_List {
  Widget real_list;

} mib_List;

/* mib_List functions */
/*****************************************************************************/
mib_Widget *mib_create_List(mib_Widget *, char *, char *, int, int, int,
                                int, int);
void mib_delete_List(mib_Widget *);
void mib_save_List(mib_Widget *, FILE *);
int mib_load_List(mib_Widget *, mib_Buffer *);

/* Structure of mib_Scale */
/*****************************************************************************/
typedef struct _mib_Scale {
  int   orientation;

} mib_Scale;

/* mib_Scale functions */
/*****************************************************************************/
mib_Widget *mib_create_Scale(mib_Widget *, char *, char *, int, int, int,
                int, int);
void mib_delete_Scale(mib_Widget *);
void mib_save_Scale(mib_Widget *, FILE *);
int mib_load_Scale(mib_Widget *, mib_Buffer *);

/* Structure of mib_Menu */
/*****************************************************************************/
typedef struct _mib_Menu {
  int       numitems;           /* number of items in menu*/
  MenuItem *my_menu;            /* array of menu items */
  Widget   *items;              /* pointers to each item */

} mib_Menu;

/* mib_Menu functions */
/*****************************************************************************/
mib_Widget *mib_create_Menu(mib_Widget *, char *, char *, int, int, int,
                int, int);
void mib_delete_Menu(mib_Widget *);
void mib_save_Menu(mib_Widget *, FILE *);
int mib_load_Menu(mib_Widget *, mib_Buffer *);

/*****************************************************************************/

typedef struct _mib_widget_funcs {
  char          *name;
#ifdef __cplusplus
  mib_Widget    *(*mib_create)(mib_Widget *parent, char *name, char *label,
                int posx, int posy, int width, int height, int mib_fill);
  void           (*mib_delete)(mib_Widget *thisw);
  void           (*mib_save)(mib_Widget *thisw, FILE *fout);
  int            (*mib_load)(mib_Widget *thisw, mib_Buffer *fin);
#else
  mib_Widget    *(*mib_create)();
  void           (*mib_delete)();
  void           (*mib_save)();
  int            (*mib_load)();
#endif
} mib_widget_funcs;

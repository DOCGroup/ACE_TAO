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
#include <sys/types.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Intrinsic.h>

#include "include/common.h"

ACE_RCSID(mpeg_client, global, "$Id$")

/* Declaration of global variable to hold dither info. */

int ditherType;

/* Global file pointer to incoming data. */
FILE *input;

/* End of File flag. */
int EOF_flag = 0;

/* Loop flag. */
int loopFlag = 0;

/* Shared memory flag. */
int shmemFlag = 0;

/* Quiet flag. */
int quietFlag = 0;

/* Display image on screen? */
int noDisplayFlag = 0;

/* real time flag */
int realTimeFlag = 0;

/* shared version of command line argument */
int argc_share;
char **argv_share;

/* for command line display name */
char displayName[64];

/* command line program list name */
char proglistName[PATH_SIZE] = "";

/* shared data structure */
SharedData * shared = NULL;

/* Range values for lum, cr, cb. */
int LUM_RANGE;
int CR_RANGE;
int CB_RANGE;

/* Array that remaps color numbers to actual pixel values used by X server. */

unsigned char pixel[256];

/* Arrays holding quantized value ranged for lum, cr, and cb. */

int *lum_values = NULL;
int *cr_values = NULL;
int *cb_values = NULL;

/* Structures used by the X server. */

Display *display;
Colormap cmap;



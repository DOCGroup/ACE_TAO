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
/* Declaration of global variable to hold dither info. */

extern int ditherType;

/* Global file pointer to incoming data. */
extern FILE *input;

/* End of File flag. */
extern int EOF_flag;

/* Loop flag. */
extern int loopFlag;

/* Shared memory flag. */
extern int shmemFlag;

/* Quiet flag. */
extern int quietFlag;

/* Display image on screen? */
extern int noDisplayFlag;

/* real time flag */
extern int realTimeFlag;

/* shared version of command line argument */
extern int argc_share;
extern char **argv_share;

/* for command line display name */
extern char displayName[];

/* command line program list name */
extern char proglistName[PATH_SIZE];

/* shared data structure */
extern SharedData * shared;

/* Structures used by the X server. */

extern Display *display;
extern Colormap cmap;

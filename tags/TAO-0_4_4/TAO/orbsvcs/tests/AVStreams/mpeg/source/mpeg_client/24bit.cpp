/* $Id$ */

/*
 * Copyright (c) 1992 The Regents of the University of California.
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice and the following
 * two paragraphs appear in all copies of this software.
 * 
 * IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF
 * CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 */

#include <stdlib.h>
#include <string.h>
#include "video.h"
#include "dither.h"
#include "proto.h"
#include "ace/OS.h"

ACE_RCSID(mpeg_client, 24bit, "$Id$")

/*
 * We'll define the "ConvertColor" macro here to do fixed point arithmetic
 * that'll convert from YCrCb to RGB using:
 *	R = L + 1.40200*Cr;
 *	G = L - 0.34414*Cb - 0.71414*Cr
 *	B = L + 1.77200*Cb;
 *
 * We'll use fixed point by adding two extra bits after the decimal.
 */

#define BITS	8
#define ONE     ((int) 1)
#define CONST_SCALE	(ONE << BITS)
#define ROUND_FACTOR	(ONE << (BITS-1))

/* Macro to convert integer to fixed. */
#define UP(x)	(((int)(x)) << BITS)

/* Macro to convert fixed to integer (with rounding). */
#define DOWN(x)	(((x) + ROUND_FACTOR) >> BITS)

/* Macro to convert a float to a fixed */
#define FIX(x)  ((int) ((x)*CONST_SCALE + 0.5))

#define CLAMP(ll,x,ul)	( ((x)<(ll)) ?(ll):( ((x)>(ul)) ?(ul):(x)))

static int *Cb_r_tab, *Cr_g_tab, *Cb_g_tab, *Cr_b_tab;

/*
 *--------------------------------------------------------------
 *
 * InitColorDither --
 *
 *	To get rid of the multiply and other conversions in color
 *	dither, we use a lookup table.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The lookup tables are initialized.
 *
 *--------------------------------------------------------------
 */

void
InitColorDither()
{
    int CR, CB, i;

    Cr_b_tab = (int *)ACE_OS::malloc(256*sizeof(int));
    Cr_g_tab = (int *)ACE_OS::malloc(256*sizeof(int));
    Cb_g_tab = (int *)ACE_OS::malloc(256*sizeof(int));
    Cb_r_tab = (int *)ACE_OS::malloc(256*sizeof(int));

    for (i=0; i<256; i++) {
	CB = CR = i;

	CB -= 128; CR -= 128;

	Cb_r_tab[i] = FIX(1.40200) * CB;
	Cr_g_tab[i] = -FIX(0.34414) * CR;
	Cb_g_tab[i] = -FIX(0.71414) * CB;   
	Cr_b_tab[i] = FIX(1.77200) * CR;
    }
}


/*
 *--------------------------------------------------------------
 *
 * ColorDitherImage --
 *
 *	Converts image into 24 bit color.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	None.
 *
 *--------------------------------------------------------------
 */

void
ColorDitherImage(unsigned char *lum, unsigned char *cr,
		 unsigned char *cb, unsigned char *out,
		 int rows, int cols)
{
    int L, CR, CB;
    unsigned int *row1, *row2;
    unsigned char *lum2;
    int x, y;
    unsigned int r, b, g;
    int cb_r;
    int cr_g;
    int cb_g;
    int cr_b;

    row1 = (unsigned int *)out;
    row2 = row1 + cols;
    lum2 = lum + cols;
    for (y=0; y<rows; y+=2) {
	for (x=0; x<cols; x+=2) {
	    int R, G, B;

	    CR = *cr++;
	    CB = *cb++;
	    cb_r = Cb_r_tab[CB];
	    cr_g = Cr_g_tab[CR];
	    cb_g = Cb_g_tab[CB];
	    cr_b = Cr_b_tab[CR];

	    L = *lum++;
	    L = UP(L);
	    R = L + cb_r;
	    G = L + cr_g + cb_g;
	    B = L + cr_b;
#ifndef LITTLE_ENDIAN
	    r = CLAMP(0,R,UP(255)) >> BITS;
	    g = CLAMP(0,G,UP(255)) & 0xff00;
	    b = (CLAMP(0,B,UP(255)) & 0xff00) << BITS;
#else
	    b = CLAMP(0,B,UP(255)) >> BITS;
	    g = CLAMP(0,G,UP(255)) & 0xff00;
	    r = (CLAMP(0,R,UP(255)) & 0xff00) << BITS;
#endif
	    *row1++ = r | g | b;

	    L = *lum++;
	    L = UP(L);
	    R = L + cb_r;
	    G = L + cr_g + cb_g;
	    B = L + cr_b;
#ifndef LITTLE_ENDIAN
	    r = CLAMP(0,R,UP(255)) >> BITS;
	    g = CLAMP(0,G,UP(255)) & 0xff00;
	    b = (CLAMP(0,B,UP(255)) & 0xff00) << BITS;
#else
	    b = CLAMP(0,B,UP(255)) >> BITS;
	    g = CLAMP(0,G,UP(255)) & 0xff00;
	    r = (CLAMP(0,R,UP(255)) & 0xff00) << BITS;
#endif
	    *row1++ = r | g | b;

	    /*
	     * Now, do second row.
	     */
	    L = *lum2++;
	    L = UP(L);
	    R = L + cb_r;
	    G = L + cr_g + cb_g;
	    B = L + cr_b;
#ifndef LITTLE_ENDIAN
	    r = CLAMP(0,R,UP(255)) >> BITS;
	    g = CLAMP(0,G,UP(255)) & 0xff00;
	    b = (CLAMP(0,B,UP(255)) & 0xff00) << BITS;
#else
	    b = CLAMP(0,B,UP(255)) >> BITS;
	    g = CLAMP(0,G,UP(255)) & 0xff00;
	    r = (CLAMP(0,R,UP(255)) & 0xff00) << BITS;
#endif
	    *row2++ = r | g | b;

	    L = *lum2++;
	    L = UP(L);
	    R = L + cb_r;
	    G = L + cr_g + cb_g;
	    B = L + cr_b;
#ifndef LITTLE_ENDIAN
	    r = CLAMP(0,R,UP(255)) >> BITS;
	    g = CLAMP(0,G,UP(255)) & 0xff00;
	    b = (CLAMP(0,B,UP(255)) & 0xff00) << BITS;
#else
	    b = CLAMP(0,B,UP(255)) >> BITS;
	    g = CLAMP(0,G,UP(255)) & 0xff00;
	    r = (CLAMP(0,R,UP(255)) & 0xff00) << BITS;
#endif
	    *row2++ = r | g | b;
	}
	lum += cols;
	lum2 += cols;
	row1 += cols;
	row2 += cols;
    }
}

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
#include "proto.h"
#include "dither.h"
#include "ace/OS.h"

ACE_RCSID(mpeg_client, fs2fast, "$Id$")

/* Arrays containing error values for floyd-steinberg dithering. */

static int deltay[256];
static int deltau[256];
static int deltav[256];
static int deltay2[256];
static int deltau2[256];
static int deltav2[256];

/* Definitions governing number of bits used for luminance, cr, and cb. */

#define L_BITS 3
#define CR_BITS 2
#define CB_BITS 2

/* Masks for proper quantization of lum, cr, and cb values. */

#define L_MASK 0xe0
#define CR_MASK 0xc0
#define CB_MASK 0xc0



/*
 *--------------------------------------------------------------
 *
 * InitFS2FastDither --
 *
 *	Initializes structures and arrays neeeded for fast implementation
 *      of two error F-S dithering.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *      None.
 *
 *--------------------------------------------------------------
 */

void InitFS2FastDither()
{
  int i;
  int lum_num, cr_num, cb_num;

  for (i=0; i<256; i++) {
    lum_num = (i >> (8-L_BITS));
    cr_num = (i >> (8-CR_BITS));
    cb_num = (i >> (8-CB_BITS));

    /* These arrays contain the error values propogated for each pixel value 
       for each channel. 
    */

    deltay[i] = (i - ((int) lum_values[lum_num])) / 2;
    deltau[i] = (i-((int) cr_values[cr_num])) / 2;
    deltav[i] = (i-((int) cb_values[cb_num])) / 2;
    deltay2[i] = (i - ((int) lum_values[lum_num])) - deltay[i];
    deltau2[i] = (i - ((int) cr_values[cr_num])) - deltau[i];
    deltav2[i] = (i - ((int) cb_values[cb_num])) - deltav[i];

  }

}

/*
 *--------------------------------------------------------------
 *
 * DitherImage --
 *
 *	Dithers an image using floyd-steinberg.
 *	Assumptions made:
 *	  1) The color space is allocated y:cr:cb = 8:4:4
 *	  2) The spatial resolution of y:cr:cb is 4:1:1
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
FS2FastDitherImage (
    unsigned char *lum,
    unsigned char *cr,
    unsigned char *cb,
    unsigned char *out,
    int w, int h)
{
    int i, j, idx, idx2;
    int y, u, v;
    int dy, du, dv;
    int code;
    static int *yerr1;
    static int *yerr2;
    static int *uerr1;
    static int *uerr2;
    static int *verr1;
    static int *verr2;
    int *ye1, *ue1, *ve1;
    int *ye2, *ue2, *ve2;
    unsigned char *o, *l, *r, *b;
    static int first = 1;

    /* If first time called, allocate error arrays. */

    if (first) {
      first = 0;
      yerr1 = (int *) ACE_OS::malloc((w+5)*sizeof(int));
      yerr2 = (int *) ACE_OS::malloc((w+5)*sizeof(int));
      uerr1 = (int *) ACE_OS::malloc((w+5)*sizeof(int));
      uerr2 = (int *) ACE_OS::malloc((w+5)*sizeof(int));
      verr1 = (int *) ACE_OS::malloc((w+5)*sizeof(int));
      verr2 = (int *) ACE_OS::malloc((w+5)*sizeof(int));
    }

    /*
     * Init error arrays and variables.
     */
    memset ((char *)yerr1, 0, (w+5)*sizeof(int));
    memset ((char *)yerr2, 0, (w+5)*sizeof(int));
    memset ((char *)uerr1, 0, (w+5)*sizeof(int));
    memset ((char *)uerr2, 0, (w+5)*sizeof(int));
    memset ((char *)verr1, 0, (w+5)*sizeof(int));
    memset ((char *)verr2, 0, (w+5)*sizeof(int));
    du = dv = dy = 0;

    for (j=0; j<h; j+=2) {
	ye1 = yerr1;
	ue1 = uerr1;
	ve1 = verr1;
	ye2 = yerr2;
	ue2 = uerr2;
	ve2 = verr2;
	idx = j*w;
	idx2 = idx/4;
	o = out+idx;
	l = lum+idx;
	r = cr+idx2;
	b = cb+idx2;
	/* Do the top row in forward order. */
	for (i=0; i<w; i+=2) {
	    /* Do left side of this pair... */
	    y = *l++ + dy + *ye1++;
	    u = *r + du + *ue1++;
	    v = *b + dv + *ve1++;

	    if (y < 0) y = 0;
	    else if (y > 255) y = 255;
	    if (u < 0) u = 0;
	    else if (u > 255) u = 255;
	    if (v < 0) v = 0;
	    else if (v > 255) v = 255;

	    /*
	     * Construct a code using:
	     *	high order 3 bits of y, 
	     *	high order 2 bits of u, 
	     *	high order 2 bits of v
	     */
	    code = (((y & L_MASK) | ((u & CR_MASK) >> L_BITS) | (v >> (L_BITS+CR_BITS))) 
		    >> (8-(L_BITS+CR_BITS+CB_BITS)));
	    *o++ = pixel[code];
	    *ye2++ = deltay[y];
	    *ue2++ = deltau[u];
	    *ve2++ = deltav[v];
	    dy = deltay2[y];
	    du = deltau2[u];
	    dv = deltav2[v];

	    /* Do right side of this pair... */
	    y = *l++ + dy + *ye1++;
	    u = *r++ + du + *ue1++;
	    v = *b++ + dv + *ve1++;

	    if (y < 0) y = 0;
	    else if (y > 255) y = 255;
	    if (u < 0) u = 0;
	    else if (u > 255) u = 255;
	    if (v < 0) v = 0;
	    else if (v > 255) v = 255;

	    code = (((y & L_MASK) | ((u & CR_MASK) >> L_BITS) | (v >> (L_BITS+CR_BITS))) 
		    >> (8-(L_BITS+CR_BITS+CB_BITS)));
	    *o++ = pixel[code];
	    *ye2++ = deltay[y];
	    *ue2++ = deltau[u];
	    *ve2++ = deltav[v];
	    dy = deltay2[y];
	    du = deltau2[u];
	    dv = deltav2[v];

	}
	
	ye1 = yerr1+w-1;
	ue1 = uerr1+w-1;
	ve1 = verr1+w-1;
	ye2 = yerr2+w-1;
	ue2 = uerr2+w-1;
	ve2 = verr2+w-1;
	l += w-1;
	o += w-1;
	r--;
	b--;
	dy = du = dv = 0;

	/* Do bottom part of row, in right to left order. */
	for (i=w-1; i>0; i-=2) {
	    /* Do right side of this pair... */
	    y = *l-- + dy + *ye2--;
	    u = *r + du + *ue2--;
	    v = *b + dv + *ve2--;

     	    if (y < 0) y = 0;
	    else if (y > 255) y = 255;
	    if (u < 0) u = 0;
	    else if (u > 255) u = 255;
	    if (v < 0) v = 0;
	    else if (v > 255) v = 255;

	    /*
	     * Construct a code using:
	     *	high order 3 bits of y, 
	     *	high order 2 bits of u, 
	     *	high order 2 bits of v
	     */
	    code = (((y & L_MASK) | ((u & CR_MASK) >> L_BITS) | (v >> (L_BITS+CR_BITS))) 
		    >> (8-(L_BITS+CR_BITS+CB_BITS)));
	    *o-- = pixel[code];
	    *ye1-- = deltay[y];
	    *ue1-- = deltau[u];
	    *ve1-- = deltav[v];
	    dy = deltay2[y];
	    du = deltau2[u];
	    dv = deltav2[v];

	    /* Do left side of this pair... */
	    y = *l-- + dy + *ye2--;
	    u = *r-- + du + *ue2--;
	    v = *b-- + dv + *ve2--;

	    if (y < 0) y = 0;
	    else if (y > 255) y = 255;
	    if (u < 0) u = 0;
	    else if (u > 255) u = 255;
	    if (v < 0) v = 0;
	    else if (v > 255) v = 255;

	    code = (((y & L_MASK) | ((u & CR_MASK) >> L_BITS) | (v >> (L_BITS+CR_BITS))) 
		    >> (8-(L_BITS+CR_BITS+CB_BITS)));
	    *o-- = pixel[code];
	    *ye1-- = deltay[y];
	    *ue1-- = deltau[u];
	    *ve1-- = deltav[v];
	    dy = deltay2[y];
	    du = deltau2[u];
	    dv = deltav2[v];

	}
    }
}






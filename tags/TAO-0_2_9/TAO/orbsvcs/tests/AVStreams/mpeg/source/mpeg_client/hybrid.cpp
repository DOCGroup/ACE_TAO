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

/* This file contains C code to implement an ordered dither. */

#include <string.h>
#include <stdlib.h>
#include "video.h"
#include "proto.h"
#include "dither.h"
#include "ace/OS.h"

ACE_RCSID(mpeg_client, hybrid, "$Id$")

#define DITH_SIZE 16


/* Structures used to implement hybrid ordered dither/floyd-steinberg
   dither algorithm.
*/

static unsigned char *l_darrays[DITH_SIZE];
static unsigned char cr_fsarray[256][4];
static unsigned char cb_fsarray[256][4];


/*
 *--------------------------------------------------------------
 *
 *  InitHybridDither--
 *
 *	Structures intialized for hybrid dithering. Ordered dither
 *      patterns set for luminance channel, f-s errors precomputed
 *      for chrominance channels.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *      None.
 *
 *--------------------------------------------------------------
 */

void
InitHybridDither()
{
  int i, j, k, err_range, threshval;
  unsigned char *lmark;

  for (i=0; i<DITH_SIZE; i++) {
    lmark = l_darrays[i] = (unsigned char *) malloc(256);

    for (j=0; j<lum_values[0]; j++) {
      *lmark++ = 0;
    }

    for (j=0; j<(LUM_RANGE-1); j++) {
      err_range = lum_values[j+1] - lum_values[j];
      threshval = ((i * err_range) / DITH_SIZE)+lum_values[j];

      for (k=lum_values[j]; k<lum_values[j+1]; k++) {
	if (k > threshval) *lmark++ = ((j+1) * (CR_RANGE * CB_RANGE));
	else *lmark++ = (j * (CR_RANGE * CB_RANGE));
      }
    }

    for (j=lum_values[LUM_RANGE-1]; j<256; j++) {
      *lmark++ = (LUM_RANGE-1)*(CR_RANGE * CB_RANGE);
    }

  }
  {
    int cr1, cr2, cr3, cr4, err1, err2;
    int cb1, cb2, cb3, cb4, val, nval;

    for (i=0; i<256; i++) {

      val = i;

      cr1 = (val * CR_RANGE) / 256;
      err1 = (val - cr_values[cr1])/2;
      err2 = (val - cr_values[cr1]) - err1;

      nval = val+err1;
      if (nval > 255) nval = 255;
      else if (nval < 0) nval = 0;
      cr2 = (nval * CR_RANGE) / 256;
      err1 = (nval - cr_values[cr2])/2;

      nval = val+err2;
      if (nval > 255) nval = 255;
      else if (nval < 0) nval = 0;
      cr3 = (nval * CR_RANGE) / 256;
      err2 = (nval - cr_values[cr3])/2;

      nval = val+err1+err2;
      if (nval > 255) nval = 255;
      else if (nval < 0) nval = 0;
      cr4 = (nval * CR_RANGE) / 256;

      cr_fsarray[i][0] = cr1*CB_RANGE;
      cr_fsarray[i][1] = cr2*CB_RANGE;
      cr_fsarray[i][2] = cr3*CB_RANGE;
      cr_fsarray[i][3] = cr4*CB_RANGE;
    }
    
    for (i=0; i<256; i++) {

      val = i;

      cb1 = (val * CB_RANGE) / 256;
      err1 = (val - cb_values[cb1])/2;
      err2 = (val - cb_values[cb1]) - err1;

      nval = val+err1;
      if (nval > 255) nval = 255;
      else if (nval < 0) nval = 0;
      cb2 = (nval * CB_RANGE) / 256;
      err1 = (nval - cb_values[cb2])/2;

      nval = val+err2;
      if (nval > 255) nval = 255;
      else if (nval < 0) nval = 0;
      cb3 = (nval * CB_RANGE) / 256;
      err2 = (nval - cb_values[cb3])/2;

      nval = val+err1+err2;
      if (nval > 255) nval = 255;
      else if (nval < 0) nval = 0;
      cb4 = (nval * CB_RANGE) / 256;

      cb_fsarray[i][0] = cb1;
      cb_fsarray[i][1] = cb2;
      cb_fsarray[i][2] = cb3;
      cb_fsarray[i][3] = cb4;
    }
  }
}

/*
 *--------------------------------------------------------------
 *
 * HybridDitherImage --
 *
 *	Dithers an image using an ordered dither.
 *	Assumptions made:
 *	  1) The color space is allocated y:cr:cb = 8:4:4
 *	  2) The spatial resolution of y:cr:cb is 4:1:1
 *      The luminance channel is dithered based on the standard
 *      ordered dither pattern for a 4x4 area. The Chrominance 
 *      channels are dithered based on precomputed f-s errors.
 *      Two errors are propogated per pixel. Errors are NOT propogated
 *      beyond a 2x2 pixel area. 
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
HybridDitherImage (unsigned char *lum, unsigned char *cr,
		   unsigned char *cb, unsigned char *out,
		   int w, int h)
{
  unsigned char *l, *r, *b, *o1, *o2;
  unsigned char *l2;
  int i, j;

  l = lum;
  l2 = lum+w;
  r = cr;
  b = cb;
  o1 = out;
  o2 = out+w;

  for (i=0; i<h; i+=4) {

    for (j=0; j<w; j+=4) {

      *o1++ = pixel[(l_darrays[0][*l++] | cr_fsarray[*r][0] | cb_fsarray[*b][0])];
      *o1++ = pixel[(l_darrays[8][*l++] | cr_fsarray[*r][1] | cb_fsarray[*b][1])];
      *o2++ = pixel[(l_darrays[12][*l2++] | cr_fsarray[*r][2] | cb_fsarray[*b][2])];
      *o2++ = pixel[(l_darrays[4][*l2++] | cr_fsarray[*r++][3] | cb_fsarray[*b++][3])];

      *o1++ = pixel[(l_darrays[2][*l++] | cr_fsarray[*r][0] | cb_fsarray[*b][0])];
      *o1++ = pixel[(l_darrays[10][*l++] | cr_fsarray[*r][1] | cb_fsarray[*b][1])];
      *o2++ = pixel[(l_darrays[14][*l2++] | cr_fsarray[*r][2] | cb_fsarray[*b][2])];
      *o2++ = pixel[(l_darrays[6][*l2++] | cr_fsarray[*r++][3] | cb_fsarray[*b++][3])];
    }

    l += w; l2 += w;
    o1 += w; o2 += w;

    for (j=0; j<w; j+=4) {

      *o1++ = pixel[(l_darrays[3][*l++] | cr_fsarray[*r][1] | cb_fsarray[*b][1])];
      *o1++ = pixel[(l_darrays[11][*l++] | cr_fsarray[*r][0] | cb_fsarray[*b][0])];
      *o2++ = pixel[(l_darrays[15][*l2++] | cr_fsarray[*r][3] | cb_fsarray[*b][3])];
      *o2++ = pixel[(l_darrays[7][*l2++] | cr_fsarray[*r++][2] | cb_fsarray[*b++][2])];

      *o1++ = pixel[(l_darrays[1][*l++] | cr_fsarray[*r][1] | cb_fsarray[*b][1])];
      *o1++ = pixel[(l_darrays[9][*l++] | cr_fsarray[*r][0] | cb_fsarray[*b][0])];
      *o2++ = pixel[(l_darrays[13][*l2++] | cr_fsarray[*r][3] | cb_fsarray[*b][3])];
      *o2++ = pixel[(l_darrays[5][*l2++] | cr_fsarray[*r++][2] | cb_fsarray[*b++][2])];
    }

    l += w; l2 += w;
    o1 += w; o2 += w;
  }
}


  

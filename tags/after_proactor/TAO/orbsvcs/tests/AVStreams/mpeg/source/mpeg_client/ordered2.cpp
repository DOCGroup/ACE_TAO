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

#include <stdlib.h>
#include "video.h"
#include "proto.h"
#include "dither.h"
#include "ace/OS.h"

ACE_RCSID(mpeg_client, ordered2, "$Id$")

#define DITH_SIZE 16


/* Structures used to implement hybrid ordered dither/floyd-steinberg
   dither algorithm.
*/

static unsigned char ***ditherPtr[DITH_SIZE];


/*
 *--------------------------------------------------------------
 *
 *  InitOrderedDither--
 *
 *	Structures intialized for ordered dithering. 
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
InitOrdered2Dither()
{
  unsigned char ****pos_2_cb;
  unsigned char ***cb_2_cr;
  unsigned char **cr_2_l;
  int cb_val, cb_rval, cr_val, cr_rval, l_val, l_rval;
  int i, j, pos;
  int err_range, threshval;

  pos_2_cb = (unsigned char ****) malloc (DITH_SIZE*sizeof(unsigned char ***));
  cb_2_cr = (unsigned char ***) malloc(CB_RANGE*sizeof(unsigned char **));
  cr_2_l = (unsigned char **) malloc(CR_RANGE*sizeof(unsigned char *));

  for (pos=0; pos<DITH_SIZE; pos++) {
    
    pos_2_cb[pos] = (unsigned char ***) malloc(256*(sizeof(unsigned char **)));

    for (j=0; j<CB_RANGE; j++) {
      cb_2_cr[j] = (unsigned char **) malloc(256*(sizeof(unsigned char *)));
    }

    for (cb_val=0; cb_val<cb_values[0]; cb_val++) {
      (pos_2_cb[pos])[cb_val] = cb_2_cr[0];
    }

    for (cb_rval=0; cb_rval<(CB_RANGE-1); cb_rval++) {
      err_range = cb_values[cb_rval+1] - cb_values[cb_rval];
      threshval = ((pos*err_range)/DITH_SIZE)+cb_values[cb_rval];

      for (cb_val=cb_values[cb_rval]; cb_val<cb_values[cb_rval+1]; cb_val++) {
	if (cb_val>threshval) (pos_2_cb[pos])[cb_val] = cb_2_cr[cb_rval+1];
	else (pos_2_cb[pos])[cb_val] = cb_2_cr[cb_rval];
      }
    }

    for (cb_val=cb_values[CB_RANGE-1]; cb_val<256; cb_val++) {
      (pos_2_cb[pos])[cb_val] = cb_2_cr[CB_RANGE-1];
    }

    for (cb_rval=0; cb_rval<CB_RANGE; cb_rval++) {
      
      for (j=0; j<CR_RANGE; j++) {
	cr_2_l[j] = (unsigned char *) malloc(256*(sizeof(unsigned char)));
      }

      for (cr_val=0; cr_val < cr_values[0]; cr_val++) {
	(cb_2_cr[cb_rval])[cr_val] = cr_2_l[0];
      }

      for (cr_rval=0; cr_rval<(CR_RANGE-1); cr_rval++) {
	err_range = cr_values[cr_rval+1] - cr_values[cr_rval];
	threshval = ((pos*err_range)/DITH_SIZE)+cr_values[cr_rval];
	
	for (cr_val=cr_values[cr_rval]; cr_val<cr_values[cr_rval+1]; cr_val++) {
	  if (cr_val>threshval) (cb_2_cr[cb_rval])[cr_val] = cr_2_l[cr_rval+1];
	  else (cb_2_cr[cb_rval])[cr_val] = cr_2_l[cr_rval];
	}
      }
      
      for (cr_val=cr_values[CR_RANGE-1]; cr_val<256; cr_val++) {
	(cb_2_cr[cb_rval])[cr_val] = cr_2_l[CR_RANGE-1];
      }
      
      for (cr_rval=0; cr_rval<CR_RANGE; cr_rval++) {
	
	for (l_val = 0; l_val < lum_values[0]; l_val++) {
	  (cr_2_l[cr_rval])[l_val] = pixel[cb_rval+(cr_rval*CB_RANGE)+
					    (0*CR_RANGE*CB_RANGE)];
	}

	for (l_rval=0; l_rval<(LUM_RANGE-1); l_rval++) {
	  err_range = lum_values[l_rval+1] - lum_values[l_rval];
	  threshval = ((pos*err_range) /DITH_SIZE) + lum_values[l_rval];

	  for (l_val = lum_values[l_rval]; l_val < lum_values[l_rval+1]; l_val++) {
	    if (l_val>threshval) (cr_2_l[cr_rval])[l_val] = 
	      pixel[cb_rval+(cr_rval*CB_RANGE)+((l_rval+1)*CR_RANGE*CB_RANGE)];
	    else (cr_2_l[cr_rval])[l_val] =
	      pixel[cb_rval+(cr_rval*CB_RANGE)+(l_rval*CR_RANGE*CB_RANGE)];
	  }
	}

	for (l_val = lum_values[LUM_RANGE-1]; l_val < 256; l_val++) {
	  (cr_2_l[cr_rval])[l_val] = 
	    pixel[cb_rval+(cr_rval*CB_RANGE)+((LUM_RANGE-1)*CR_RANGE*CB_RANGE)];
	}
      }
    }
  }

  for (i=0; i<DITH_SIZE; i++) {
    ditherPtr[i] = pos_2_cb[i];
  }
}

/*
 *--------------------------------------------------------------
 *
 * Ordered2DitherImage --
 *
 *	Dithers an image using an ordered dither.
 *	Assumptions made:
 *	  1) The color space is allocated y:cr:cb = 8:4:4
 *	  2) The spatial resolution of y:cr:cb is 4:1:1
 *      The channels are dithered based on the standard
 *      ordered dither pattern for a 4x4 area. 
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
Ordered2DitherImage (unsigned char *lum, unsigned char *cr,
		     unsigned char *cb, unsigned char *out,
		     int h, int w)
{
  unsigned char *l, *r, *b, *o1, *o2;
  unsigned char *l2;
  unsigned char L, R, B;
  int i, j;
  unsigned char ***dp0 = ditherPtr[0];
  unsigned char ***dp2 = ditherPtr[2];
  unsigned char ***dp4 = ditherPtr[4];
  unsigned char ***dp6 = ditherPtr[6];
  unsigned char ***dp8 = ditherPtr[8];
  unsigned char ***dp10 = ditherPtr[10];
  unsigned char ***dp12 = ditherPtr[12];
  unsigned char ***dp14 = ditherPtr[14];
  unsigned char ***dp1 = ditherPtr[1];
  unsigned char ***dp3 = ditherPtr[3];
  unsigned char ***dp5 = ditherPtr[5];
  unsigned char ***dp7 = ditherPtr[7];
  unsigned char ***dp9 = ditherPtr[9];
  unsigned char ***dp11 = ditherPtr[11];
  unsigned char ***dp13 = ditherPtr[13];
  unsigned char ***dp15 = ditherPtr[15];

  l = lum;
  l2 = lum+w;
  r = cr;
  b = cb;
  o1 = out;
  o2 = out+w;

  for (i=0; i<h; i+=4) {

    for (j=0; j<w; j+=8) {

      R = r[0]; B = b[0];

      L = l[0];
      o1[0] = ((dp0[B])[R])[L];
      L = l[1];
      o1[1] = ((dp8[B])[R])[L];
      L = l2[0];
      o2[0] = ((dp12[B])[R])[L];
      L = l2[1];
      o2[1] = ((dp4[B])[R])[L];

      R = r[1]; B = b[1];

      L = l[2];
      o1[2] = ((dp2[B])[R])[L];
      L = l[3];
      o1[3] = ((dp10[B])[R])[L];
      L = l2[2];
      o2[2] = ((dp14[B])[R])[L];
      L = l2[3];
      o2[3] = ((dp6[B])[R])[L];

      R = r[2]; B = b[2];

      L = l[4];
      o1[4] = ((dp0[B])[R])[L];
      L = l[5];
      o1[5] = ((dp8[B])[R])[L];
      L = l2[4];
      o2[4] = ((dp12[B])[R])[L];
      L = l2[5];
      o2[5] = ((dp4[B])[R])[L];

      R = r[3]; B = b[3];

      L = l[6];
      o1[6] = ((dp2[B])[R])[L];
      L = l[7];
      o1[7] = ((dp10[B])[R])[L];
      L = l2[6];
      o2[6] = ((dp14[B])[R])[L];
      L = l2[7];
      o2[7] = ((dp6[B])[R])[L];

      l += 8;
      l2 += 8;
      r += 4;
      b += 4;
      o1 += 8;
      o2 += 8;
    }

    l += w; l2 += w;
    o1 += w; o2 += w;

    for (j=0; j<w; j+=8) {

      R = r[0]; B = b[0];

      L = l[0];
      o1[0] = ((dp3[B])[R])[L];
      L = l[1];
      o1[1] = ((dp11[B])[R])[L];
      L = l2[0];
      o2[0] = ((dp15[B])[R])[L];
      L = l2[1];
      o2[1] = ((dp7[B])[R])[L];

      R = r[1]; B = b[1];

      L = l[2];
      o1[2] = ((dp1[B])[R])[L];
      L = l[3];
      o1[3] = ((dp9[B])[R])[L];
      L = l2[2];
      o2[2] = ((dp13[B])[R])[L];
      L = l2[3];
      o2[3] = ((dp5[B])[R])[L];

      R = r[2]; B = b[2];

      L = l[4];
      o1[4] = ((dp3[B])[R])[L];
      L = l[5];
      o1[5] = ((dp11[B])[R])[L];
      L = l2[4];
      o2[4] = ((dp15[B])[R])[L];
      L = l2[5];
      o2[5] = ((dp7[B])[R])[L];

      R = r[3]; B = b[3];

      L = l[6];
      o1[6] = ((dp1[B])[R])[L];
      L = l[7];
      o1[7] = ((dp9[B])[R])[L];
      L = l2[6];
      o2[6] = ((dp13[B])[R])[L];
      L = l2[7];
      o2[7] = ((dp5[B])[R])[L];

      l += 8;
      l2 += 8;
      r += 4;
      b += 4;
      o1 += 8;
      o2 += 8;
    }

    l += w; l2 += w;
    o1 += w; o2 += w;
  }
}


  

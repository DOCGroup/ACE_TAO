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

#include <string.h>
#include <stdlib.h>
#include "video.h"
#include "dither.h"
#include "proto.h"
#include "ace/OS.h"

ACE_RCSID(mpeg_client, 2x2, "$Id$")

#define RAND_ERR_RANGE 7
#define RAND_ERR_SUBVAL 3

/* Array containing actual pixel values for each possible 2x2 dither pattern. */

static unsigned char *dith_a;

/* Arrays mapping lum, cr, and cb values to portions of dither pattern code. 
   The addtion of one value from each array yields a valid dither pattern
   code.
*/

static int lval_a[256+RAND_ERR_RANGE-1];
static int rval_a[256+RAND_ERR_RANGE-1];
static int bval_a[256+RAND_ERR_RANGE-1];

/* Range of possible dither patterns in each channel. */

#define L_DITH_RANGE (((LUM_RANGE-1)*4)+1)
#define CR_DITH_RANGE (((CR_RANGE-1)*4)+1)
#define CB_DITH_RANGE (((CB_RANGE-1)*4)+1)

/* Arrays of random error terms added to break up contours. */

static int *randval_a;
static int **randptr_a;


/*
 *--------------------------------------------------------------
 *
 *  Init2x2Dither--
 *
 *	Initializes structures used for 2x2 dithering.
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
Init2x2Dither()
{  
  unsigned char *dith_ca;
  int numcodes;
  int l_range, cr_range, cb_range;
  int p1, p2, p3, p4;
  int l_dith, cr_dith, cb_dith;
  int big_part, small_part;
  int i, j;

  l_range = L_DITH_RANGE;
  cr_range = CR_DITH_RANGE;
  cb_range = CB_DITH_RANGE;

  numcodes =  l_range * cr_range * cb_range;

  dith_a = (unsigned char *) ACE_OS::malloc(numcodes*4);

  dith_ca =  dith_a;

  for (i=0; i<numcodes; i++) {
    l_dith = i  % l_range;

    big_part = l_dith / 4;
    small_part = l_dith % 4;

    p1 = big_part + ((small_part > 0) ? 1 : 0);
    p2 = big_part + ((small_part > 2) ? 1 : 0);
    p3 = big_part;
    p4 = big_part + ((small_part > 1) ? 1 : 0);

    p1 *= CR_RANGE * CB_RANGE;
    p2 *= CR_RANGE * CB_RANGE;
    p3 *= CR_RANGE * CB_RANGE;
    p4 *= CR_RANGE * CB_RANGE;

    cr_dith = (i/l_range) % cr_range;

    big_part = cr_dith / 4;
    small_part = cr_dith % 4;

    p1 += (big_part + ((small_part > 0) ? 1 : 0))*CB_RANGE;
    p2 += (big_part + ((small_part > 2) ? 1 : 0))*CB_RANGE;
    p3 += (big_part)*CB_RANGE;
    p4 += (big_part + ((small_part > 1) ? 1 : 0))*CB_RANGE;

    cb_dith = (i/(cr_range*l_range)) % cb_range;

    big_part = cb_dith / 4;
    small_part = cb_dith % 4;

    p1 += (big_part + ((small_part > 0) ? 1 : 0));
    p2 += (big_part + ((small_part > 2) ? 1 : 0));
    p3 += (big_part);
    p4 += (big_part + ((small_part > 1) ? 1 : 0));

    *dith_ca++ = p1;
    *dith_ca++ = p2;
    *dith_ca++ = p3;
    *dith_ca++ = p4;
  }

  for (i=RAND_ERR_SUBVAL; i<256+RAND_ERR_SUBVAL; i++) {
    j = i-RAND_ERR_SUBVAL;
    lval_a[i] = (j * L_DITH_RANGE)/256;
    rval_a[i] = (j * CR_DITH_RANGE)/256;
    bval_a[i] = (j * CB_DITH_RANGE)/256;

    bval_a[i] *= CR_DITH_RANGE * L_DITH_RANGE * 4;
    rval_a[i] *= L_DITH_RANGE * 4;
    lval_a[i] *= 4;
  }

  for (i=0; i<RAND_ERR_SUBVAL; i++) {
    lval_a[i] = lval_a[RAND_ERR_SUBVAL];
    rval_a[i] = rval_a[RAND_ERR_SUBVAL];
    bval_a[i] = bval_a[RAND_ERR_SUBVAL];
  }

   for(i=256+RAND_ERR_SUBVAL; i<256+RAND_ERR_RANGE-1; i++) {
     lval_a[i] = lval_a[255+RAND_ERR_SUBVAL];
     rval_a[i] = rval_a[255+RAND_ERR_SUBVAL];
     bval_a[i] = bval_a[255+RAND_ERR_SUBVAL];
   }
}


/*
 *--------------------------------------------------------------
 *
 * RandInit --
 *
 *	Initializes the random values used for 2x2 dithering.
 *
 * Results:
 *	randval_a filled with random values.
 *      randptr_a filled with random pointers to random value arrays.
 *
 * Side effects:
 *      None.
 *
 *--------------------------------------------------------------
 */

void RandInit(int h, int w)
{
  int i;

  randval_a = (int *) ACE_OS::malloc(w*5*sizeof(int));
  randptr_a = (int **) ACE_OS::malloc(h*sizeof(int *));

#ifdef NO_LRAND48
  for (i=0; i<w*5; i++) {
    long int random();

    randval_a[i] = random() % RAND_ERR_RANGE;
  }

  for (i=0; i<h; i++) {
    long int random();

    randptr_a[i] = randval_a + (random() % (w*2));
  }
#else /* NO_LRAND48 */

  for (i=0; i<w*5; i++) {
    
    randval_a[i] = lrand48() % RAND_ERR_RANGE;
  }
  
  for (i=0; i<h; i++) {

    randptr_a[i] = randval_a + (lrand48() % (w*2));
  }
#endif
  
}


/*
 *--------------------------------------------------------------
 *
 *  PostInit2x2Dither--
 *
 *	Remaps color numbers in dither patterns to actual pixel
 *      values allocated by the X server.
 *
 * Results:
 *      None.
 *
 * Side effects:
 *      None.
 *
 *--------------------------------------------------------------
 */

void
PostInit2x2Dither() 
{
  unsigned char *dith_ca;
  int i;

  dith_ca = dith_a;

  for (i=0; i < (L_DITH_RANGE * CR_DITH_RANGE * CB_DITH_RANGE); i++) {
    
    *dith_ca = pixel[*dith_ca];
    dith_ca++;
    *dith_ca = pixel[*dith_ca];
    dith_ca++;
    *dith_ca = pixel[*dith_ca];
    dith_ca++;
    *dith_ca = pixel[*dith_ca];
    dith_ca++;
  }
}


/*
 *--------------------------------------------------------------
 *
 * Twox2DitherImage --
 *
 *	Dithers lum, cr, and cb channels togethor using predefined
 *      and computed 2x2 dither patterns. Each possible combination of
 *      lum, cr, and cb values combines to point to a particular dither
 *      pattern (2x2) which is used to represent the pixel. This assumes
 *      That the display plane is 4 times larger than the lumianance 
 *      plane. 
 *
 * Results:
 *      None.
 *
 * Side effects:
 *      None.
 *
 *--------------------------------------------------------------
 */

void 
Twox2DitherImage(unsigned char *lum, unsigned char *cr,
		 unsigned char *cb, unsigned char *out,
		 int h, int w)
{
  int i, j;
  unsigned short *o1, *o2, *o3, *o4;
  unsigned char *l1, *l2, *base;
  unsigned char B, R;
  unsigned short *dith_ca;
  int big_adv = 3*w;
  int b_val, r_val, l_val;
  int *randvalptr;
  int randval;
  static int first = 1;

  if (first) {
    RandInit(h, w);
    first = 0;
  }

  o1 = (unsigned short *)out;
  o2 = (unsigned short *)(out+(2*w));
  o3 = (unsigned short *)(out+(4*w));
  o4 = (unsigned short *)(out+(6*w));
  l1 = lum;
  l2 = lum+w;

  for (i=0; i<h; i+=2) {
    for(j=0; j<w; j+= 4) {
 
      B = cb[0];
      b_val = bval_a[B];
      R = cr[0];
      r_val = rval_a[R];
      base = dith_a + b_val + r_val;

      l_val = lval_a[l1[0]];
      dith_ca = (unsigned short *)(base + l_val);
      o1[0] = dith_ca[0];
      o2[0] = dith_ca[1];

      l_val = lval_a[l1[1]];
      dith_ca = (unsigned short *)(base + l_val);
      o1[1] = dith_ca[0];
      o2[1] = dith_ca[1];

      l_val = lval_a[l2[0]];
      dith_ca = (unsigned short *)(base + l_val);
      o3[0] = dith_ca[0];
      o4[0] = dith_ca[1];

      l_val = lval_a[l2[1]];
      dith_ca = (unsigned short *)(base + l_val);
      o3[1] = dith_ca[0];
      o4[1] = dith_ca[1];

      B = cb[1];
      b_val = bval_a[B];
      R = cr[1];
      r_val = rval_a[R];
      base = dith_a + b_val + r_val;

      l_val = lval_a[l1[2]];
      dith_ca = (unsigned short *)(base + l_val);
      o1[2] = dith_ca[0];
      o2[2] = dith_ca[1];

      l_val = lval_a[l1[3]];
      dith_ca = (unsigned short *)(base + l_val);
      o1[3] = dith_ca[0];
      o2[3] = dith_ca[1];

      l_val = lval_a[l2[2]];
      dith_ca = (unsigned short *)(base + l_val);
      o3[2] = dith_ca[0];
      o4[2] = dith_ca[1];

      l_val = lval_a[l2[3]];
      dith_ca = (unsigned short *)(base + l_val);
      o3[3] = dith_ca[0];
      o4[3] = dith_ca[1];

      o1 += 4;
      o2 += 4;
      o3 += 4;
      o4 += 4;
      l1 += 4;
      l2 += 4;
      cb += 2;
      cr += 2;
    }    

    l1 += w;
    l2 += w;
    o1 += big_adv;
    o2 += big_adv;
    o3 += big_adv;
    o4 += big_adv;
  }
}

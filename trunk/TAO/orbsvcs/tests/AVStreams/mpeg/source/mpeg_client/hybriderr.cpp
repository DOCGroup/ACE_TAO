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

/* This file contains C code to implement an ordered dither in the 
   luminance channel and F-S error diffusion on chrominance.
*/

#include <stdlib.h>
#include <string.h>
#include "video.h"
#include "proto.h"
#include "dither.h"
#include "ace/OS.h"

ACE_RCSID(mpeg_client, hybriderr, "$Id$")

#define DITH_SIZE 16

/* Structures used for hybrid dither with errors propogated. */

static unsigned char *l_darrays[DITH_SIZE];
static unsigned char *l_darrays0, *l_darrays1, *l_darrays2, *l_darrays3;
static unsigned char *l_darrays4, *l_darrays5, *l_darrays6, *l_darrays7;
static unsigned char *l_darrays8, *l_darrays9, *l_darrays10, *l_darrays11;
static unsigned char *l_darrays12, *l_darrays13, *l_darrays14, *l_darrays15;
static unsigned char cr_fsarray[256*256][4];
static unsigned char cb_fsarray[256*256][4];
static unsigned short  c_fserr[256*256][2];


/*
 *--------------------------------------------------------------
 *
 *  InitHybridErrorDither--
 *
 *	Initializes structures used for hybrid dither algorithm
 *      with errors propogated on Cr and Cb. 
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
InitHybridErrorDither()
{
  int i, j, k, err_range, threshval;
  unsigned char *lmark;


  for (i=0; i<DITH_SIZE; i++) {
    lmark = l_darrays[i] = (unsigned char *) ACE_OS::malloc(256);

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

    for (j=lum_values[LUM_RANGE-1]; j <256; j++) {
      *lmark++ = (LUM_RANGE-1)*(CR_RANGE * CB_RANGE);
    }
  }
  l_darrays0 = l_darrays[0]; l_darrays8 = l_darrays[8];
  l_darrays1 = l_darrays[1]; l_darrays9 = l_darrays[9];
  l_darrays2 = l_darrays[2]; l_darrays10 = l_darrays[10];
  l_darrays3 = l_darrays[3]; l_darrays11 = l_darrays[11];
  l_darrays4 = l_darrays[4]; l_darrays12 = l_darrays[12];
  l_darrays5 = l_darrays[5]; l_darrays13 = l_darrays[13];
  l_darrays6 = l_darrays[6]; l_darrays14 = l_darrays[14];
  l_darrays7 = l_darrays[7]; l_darrays15 = l_darrays[15];
  {
    int cr1, cr2, cr3, cr4, err1, err2;
    int cb1, cb2, cb3, cb4, val, nval;
    int outerr1, outerr2, outerr3, outerr4;
    int inerr1, inerr2, inerr3, inerr4;
    unsigned short oe1, oe2, oe3, oe4;

    for (j=0; j<65536; j+= 256) {

      inerr1 = (((j & 0xc000) >> 14)*8) - 12;
      inerr2 = (((j & 0x3000) >> 12)*8) - 12;
      inerr3 = (((j & 0x0c00) >> 10)*8) - 12;
      inerr4 = (((j & 0x0300) >> 8) *8) - 12;

      for (i=0; i<256; i++) {
	val = i;

	nval = val+inerr1+inerr3;
	if (nval < 0) nval = 0; else if (nval > 255) nval = 255;
	cr1 = ((nval) * CR_RANGE) / 256;
	err1 = ((nval) - cr_values[cr1])/2;
	err2 = ((nval) - cr_values[cr1]) - err1;

	nval = val+err1+inerr2;
	if (nval < 0) nval = 0; else if (nval > 255) nval = 255;
	cr2 = ((nval) * CR_RANGE) / 256;
	err1 = ((nval) - cr_values[cr2])/2;
	outerr3 = ((nval) - cr_values[cr2])-err1;

	nval = val+err2+inerr4;
	if (nval < 0) nval = 0; else if (nval > 255) nval = 255;
	cr3 = ((nval) * CR_RANGE) / 256;
	err2 = ((nval) - cr_values[cr3])/2;
	outerr1 = ((nval) - cr_values[cr3]) - err2;

	nval = val+err1+err2;
	if (nval < 0) nval = 0; else if (nval > 255) nval = 255;
	cr4 = ((nval) * CR_RANGE) / 256;
	outerr2 = ((nval) - cr_values[cr4])/2;
	outerr4 = ((nval) - cr_values[cr4])-outerr2;

	cr_fsarray[i+j][0] = cr1*CB_RANGE;
	cr_fsarray[i+j][1] = cr2*CB_RANGE;
	cr_fsarray[i+j][2] = cr3*CB_RANGE;
	cr_fsarray[i+j][3] = cr4*CB_RANGE;

	if (outerr1 < -16) outerr1++;
	else if (outerr1 > 15) outerr1--;
	if (outerr2 < -16) outerr2++;
	else if (outerr2 > 15) outerr2--;
	if (outerr3 < -16) outerr3++;
	else if (outerr3 > 15) outerr3--;
	if (outerr4 < -16) outerr4++;
	else if (outerr4 > 15) outerr4--;

	oe1 = (outerr1 + 16) / 8;
	oe2 = (outerr2 + 16) / 8;
	oe3 = (outerr3 + 16) / 8;
	oe4 = (outerr4 + 16) / 8;

/* This is a debugging check and should be removed if not needed. */
	if ((oe1 > 3) || (oe2 > 3) || (oe3 > 3) || (oe4 > 3))
	  fprintf(stderr, "OE error!!!!\n");


	c_fserr[i+j][0] = ((oe1 << 14) | (oe2 << 12));

	c_fserr[i+j][1] = ((oe3 << 10) | (oe4 << 8));
      }

      for (i=0; i<256; i++) {
	val = i;
	nval = val+inerr1+inerr3;
  	if (nval < 0) nval = 0; else if (nval > 255) nval = 255;     
	cb1 = ((nval) * CB_RANGE) / 256;
	err1 = ((nval) - cb_values[cb1])/2;
	err2 = ((nval) - cb_values[cb1]) - err1;

	nval = val+err1+inerr2;
  	if (nval < 0) nval = 0; else if (nval > 255) nval = 255;     
	cb2 = ((nval) * CB_RANGE) / 256;
	err1 = ((nval) - cb_values[cb2])/2;

	nval = val+err2+inerr4;
  	if (nval < 0) nval = 0; else if (nval > 255) nval = 255;     
	cb3 = ((nval) * CB_RANGE) / 256;
	err2 = ((nval) - cb_values[cb3])/2;

	nval = val+err1+err2;
  	if (nval < 0) nval = 0; else if (nval > 255) nval = 255;     
	cb4 = ((nval) * CB_RANGE) / 256;

	cb_fsarray[i+j][0] = cb1;
	cb_fsarray[i+j][1] = cb2;
	cb_fsarray[i+j][2] = cb3;
	cb_fsarray[i+j][3] = cb4;
      }
    }
  }
}

/*
 *--------------------------------------------------------------
 *
 * HybridErrorDitherImage --
 *
 *	Dithers an image using a hybrid ordered/floyd-steinberg dither.
 *	Assumptions made:
 *	  1) The color space is allocated y:cr:cb = 8:4:4
 *	  2) The spatial resolution of y:cr:cb is 4:1:1
 *      This dither is almost exactly like the dither implemented in the
 *      file odith.c (i.e. hybrid dithering) except a quantized amount of
 *      error is propogated between 2x2 pixel areas in Cr and Cb.
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
HybridErrorDitherImage (unsigned char *lum, unsigned char *cr,
			unsigned char *cb, unsigned char *out,
			int w, int h)
{
  unsigned char *l, *r, *b, *o1, *o2;
  unsigned char *l2;
  static int *cr_row_errs;
  static int *cb_row_errs;
  int *cr_r_err;
  int *cb_r_err;
  int cr_c_err;
  int cb_c_err;
  unsigned char *cr_fsptr;
  unsigned char *cb_fsptr;
  static int first = 1;
  int cr_code, cb_code;

  int i, j;
  int row_advance, row_advance2;
  int half_row_advance, half_row_advance2;

  /* If first time called, allocate error arrays. */

  if (first) {
    cr_row_errs = (int *) ACE_OS::malloc((w+5)*sizeof(int));
    cb_row_errs = (int *) ACE_OS::malloc((w+5)*sizeof(int));
    first = 0;
  }

  row_advance = (w << 1) - 1;
  row_advance2 = row_advance+2;
  half_row_advance = (w>>1)-1;
  half_row_advance2 = half_row_advance+2;

  l = lum;
  l2 = lum+w;
  r = cr;
  b = cb;
  o1 = out;
  o2 = out+w;

  memset( (char *) cr_row_errs, 0, (w+5)*sizeof(int));
  cr_r_err = cr_row_errs;
  cr_c_err = 0;
  memset( (char *) cb_row_errs, 0, (w+5)*sizeof(int));
  cb_r_err = cb_row_errs;
  cb_c_err = 0;

  for (i=0; i<h; i+=4) {

    for (j=w; j>0; j-=4) {

      cr_code = (*cr_r_err | cr_c_err | *r++);
      cb_code = (*cb_r_err | cb_c_err | *b++);

      cr_fsptr = cr_fsarray[cr_code];
      cb_fsptr = cb_fsarray[cb_code];

      *o1++ = pixel[(l_darrays0[*l++] | *cr_fsptr++ | *cb_fsptr++)];
      *o1++ = pixel[(l_darrays8[*l++] | *cr_fsptr++ | *cb_fsptr++)];
      *o2++ = pixel[(l_darrays12[*l2++] | *cr_fsptr++ | *cb_fsptr++)];
      *o2++ = pixel[(l_darrays4[*l2++] | *cr_fsptr++ | *cb_fsptr++)];

      cr_c_err = c_fserr[cr_code][1];
      cb_c_err = c_fserr[cb_code][1];
      *cr_r_err++ = c_fserr[cr_code][0];
      *cb_r_err++ = c_fserr[cb_code][0];
      cr_code = (*cr_r_err | cr_c_err | *r++);
      cb_code = (*cb_r_err | cb_c_err | *b++);

      cr_fsptr = cr_fsarray[cr_code];
      cb_fsptr = cb_fsarray[cb_code];

      *o1++ = pixel[(l_darrays2[*l++] | *cr_fsptr++ | *cb_fsptr++)];
      *o1++ = pixel[(l_darrays10[*l++] | *cr_fsptr++ | *cb_fsptr++)];
      *o2++ = pixel[(l_darrays14[*l2++] | *cr_fsptr++ | *cb_fsptr++)];
      *o2++ = pixel[(l_darrays6[*l2++] | *cr_fsptr++ | *cb_fsptr++)];

      cr_c_err = c_fserr[cr_code][1];
      cb_c_err = c_fserr[cb_code][1];
      *cr_r_err++ = c_fserr[cr_code][0];
      *cb_r_err++ = c_fserr[cb_code][0];
    }

    l += row_advance; l2 += row_advance;
    o1 += row_advance; o2 += row_advance;
    cr_c_err = 0;
    cb_c_err = 0;
    cr_r_err--; cb_r_err--;
    r += half_row_advance; b += half_row_advance;

    for (j=w; j>0; j-=4) {

      cr_code = (*cr_r_err | cr_c_err | *r--);
      cb_code = (*cb_r_err | cb_c_err | *b--);
      cr_fsptr = cr_fsarray[cr_code];
      cb_fsptr = cb_fsarray[cb_code];

      *o1-- = pixel[(l_darrays9[*l--] | *cr_fsptr++ | *cb_fsptr++)];
      *o1-- = pixel[(l_darrays1[*l--] | *cr_fsptr++ | *cb_fsptr++)];
      *o2-- = pixel[(l_darrays5[*l2--] | *cr_fsptr++ | *cb_fsptr++)];
      *o2-- = pixel[(l_darrays13[*l2--] | *cr_fsptr++ | *cb_fsptr++)];

      cr_c_err = c_fserr[cr_code][1];
      cb_c_err = c_fserr[cb_code][1];
      *cr_r_err-- = c_fserr[cr_code][0];
      *cb_r_err-- = c_fserr[cb_code][0];
      cr_code = (*cr_r_err | cr_c_err | *r--);
      cb_code = (*cb_r_err | cb_c_err | *b--);
      cr_fsptr = cr_fsarray[cr_code];
      cb_fsptr = cb_fsarray[cb_code];

      *o1-- = pixel[(l_darrays11[*l--] | *cr_fsptr++ | *cb_fsptr++)];
      *o1-- = pixel[(l_darrays3[*l--] | *cr_fsptr++ | *cb_fsptr++)];
      *o2-- = pixel[(l_darrays7[*l2--] | *cr_fsptr++ | *cb_fsptr++)];
      *o2-- = pixel[(l_darrays15[*l2--] | *cr_fsptr++ | *cb_fsptr++)];

      cr_c_err = c_fserr[cr_code][1];
      cb_c_err = c_fserr[cb_code][1];
      *cr_r_err-- = c_fserr[cr_code][0];
      *cb_r_err-- = c_fserr[cb_code][0];

    }

    l += row_advance2; l2 += row_advance2;
    o1 += row_advance2; o2 += row_advance2;
    cr_c_err = 0; cb_c_err = 0;
    cr_r_err++; cb_r_err++;
    r += half_row_advance2; b += half_row_advance2;
  }
}


  

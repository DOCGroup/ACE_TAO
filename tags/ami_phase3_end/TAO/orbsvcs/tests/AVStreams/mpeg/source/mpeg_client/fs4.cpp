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

/* This file contains C code to do YCrCb -> colormap space. */

#include <stdlib.h>
#include <string.h>
#include "fs4.h"
#include "video.h"
#include "proto.h"
#include "dither.h"

ACE_RCSID(mpeg_client, fs4, "$Id$")

/* Structures containing precomputed error terms. */

static FS4Dither lum_index[256];
static FS4Dither cr_index[256];
static FS4Dither cb_index[256];


/*
 *--------------------------------------------------------------
 *
 * InitFS4Dither --
 *
 *	Initializes structures used for f-s dithering. Precomputes
 *      error terms.
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
InitFS4Dither()
{
  int i;

  for (i=0; i<256; i++) {
    lum_index[i].value = (i * LUM_RANGE) / 256;

    lum_index[i].e1 = (7 * (i-lum_values[lum_index[i].value])) / 16;
    lum_index[i].e2 = (i-lum_values[lum_index[i].value])/16;
    lum_index[i].e3 = (5 *  (i - lum_values[lum_index[i].value])) / 16;
    lum_index[i].e4 = (i-lum_values[lum_index[i].value]) - lum_index[i].e1 -
      lum_index[i].e2 - lum_index[i].e3;

    lum_index[i].value *= LUM_BASE;

    cr_index[i].value = (i * CR_RANGE) / 256; 

    cr_index[i].e1 = (7 * (i-cr_values[cr_index[i].value])) / 16;
    cr_index[i].e2 = (i-cr_values[cr_index[i].value])/16;
    cr_index[i].e3 = (5 *  (i - cr_values[cr_index[i].value])) / 16;
    cr_index[i].e4 = (i-cr_values[cr_index[i].value]) - cr_index[i].e1 -
      cr_index[i].e2 - cr_index[i].e3;
    cr_index[i].value *= CR_BASE;

    cb_index[i].value = (i * CB_RANGE) / 256; 

    cb_index[i].e1 = (7 * (i-cb_values[cb_index[i].value])) / 16;
    cb_index[i].e2 = (i-cb_values[cb_index[i].value])/16;
    cb_index[i].e3 = (5 *  (i - cb_values[cb_index[i].value])) / 16;
    cb_index[i].e4 = (i-cb_values[cb_index[i].value]) - cb_index[i].e1 -
      cb_index[i].e2 - cb_index[i].e3;
    cb_index[i].value *= CB_BASE;

  }

}


/*
 *--------------------------------------------------------------
 *
 * DitherImage --
 *
 *	Converts lum, cr, cb image planes into fixed colormap
 *      space. Uses Floyd-Steinberg dithering in serpentine
 *      pattern with standard 4 errors propogated.
 *
 * Results:
 *	The display plane is replaced by 8-bit colormap space
 *      image.
 *
 * Side effects:
 *      Hopefully, none.
 *
 *--------------------------------------------------------------
 */

void 
FS4DitherImage(unsigned char *lum, unsigned char *cr,
	       unsigned char *cb, unsigned char *disp,
	       int rows, int cols)
{
  static char *cur_row_error, *next_row_error;
  static int first = 1;
  char  *cur_row_err_mark, *next_row_err_mark;
  char *temp;
  int i, j, pixsum, c_cols;
  unsigned char *cur_row, *channel, *dest_row;
  FS4Dither *chan_index;

  if (first) {
    cur_row_error = (char *) ACE_OS::malloc(cols+2);
    next_row_error = (char *) ACE_OS::malloc(cols+2);
    first = 0;
  }

  memset(cur_row_error, 0, cols+2);
  memset(next_row_error, 0, cols+2);

  for(i=0; i<rows; i+=2) {
     cur_row = lum + (i*cols);
     dest_row = disp + (i*cols);

     cur_row_err_mark = cur_row_error + 1;
     next_row_err_mark = next_row_error + 1;

     for (j=0; j<cols; j++) {

       pixsum = *cur_row + *cur_row_err_mark;
       if (pixsum < 0) pixsum = 0;
       else if (pixsum > 255) pixsum = 255;

       *dest_row = lum_index[pixsum].value;
       *(cur_row_err_mark+1) += lum_index[pixsum].e1; 
       *(next_row_err_mark+1) += lum_index[pixsum].e2;
       *next_row_err_mark += lum_index[pixsum].e3; 
       *(next_row_err_mark-1) += lum_index[pixsum].e4;

       cur_row++;
       dest_row++;
       cur_row_err_mark++;
       next_row_err_mark++;
     }

     temp = cur_row_error;
     cur_row_error = next_row_error;
     next_row_error = temp;

     memset(next_row_error, 0, cols+2); 

     cur_row += cols-1;
     dest_row += cols-1;
     cur_row_err_mark = cur_row_error + cols;
     next_row_err_mark = next_row_error + cols;

     for (j=0; j<cols; j++) {

       pixsum = *cur_row + *cur_row_err_mark;
       if (pixsum < 0) pixsum = 0;
       else if (pixsum > 255) pixsum = 255;

       *dest_row = lum_index[pixsum].value;
       *(cur_row_err_mark-1) += lum_index[pixsum].e1; 
       *(next_row_err_mark-1) += lum_index[pixsum].e2;
       *next_row_err_mark += lum_index[pixsum].e3; 
       *(next_row_err_mark+1) += lum_index[pixsum].e4;

       cur_row--;
       dest_row--;
       cur_row_err_mark--;
       next_row_err_mark--;
     }

     temp = cur_row_error;
     cur_row_error = next_row_error;
     next_row_error = temp;

     memset(next_row_error, 0, cols+2); 
   }

  memset(cur_row_error, 0, cols+2); 

  c_cols = cols >> 1;

  channel = cr;
  chan_index = cr_index;

 repeat:

  for (i=0; i < rows; i+=2) {
    cur_row = channel + ((i>>1)*c_cols);
    dest_row = disp + (i*cols);

    cur_row_err_mark = cur_row_error+1;
    next_row_err_mark = next_row_error+1;
    
    for (j=0; j<cols; j++) {
      int p_val;

      p_val = *cur_row;

      pixsum = *cur_row_err_mark + p_val;

      if (pixsum < 0) pixsum = 0;
      else if (pixsum > 255) pixsum = 255;

      *dest_row += chan_index[pixsum].value;

      *(cur_row_err_mark+1) += chan_index[pixsum].e1; 
      *(next_row_err_mark+1) += chan_index[pixsum].e2;
      *next_row_err_mark += chan_index[pixsum].e3; 
      *(next_row_err_mark-1) += chan_index[pixsum].e4;


      if (j&1) cur_row++;
      dest_row++;
      cur_row_err_mark++;
      next_row_err_mark++;
    }
      
    temp = cur_row_error;
    cur_row_error = next_row_error;
    next_row_error = temp;

    memset(next_row_error, 0, cols+2);

    cur_row += c_cols-1;
    dest_row += cols-1;
    cur_row_err_mark = cur_row_error+cols;
    next_row_err_mark = next_row_error+cols;

    for (j=0; j<cols; j++) {
      int p_val;

      p_val = *cur_row;

      pixsum = *cur_row_err_mark + p_val;

      if (pixsum < 0) pixsum = 0;
      else if (pixsum > 255) pixsum = 255;

      *dest_row += chan_index[pixsum].value;

      *(cur_row_err_mark-1) += chan_index[pixsum].e1; 
      *(next_row_err_mark-1) += chan_index[pixsum].e2;
      *next_row_err_mark += chan_index[pixsum].e3; 
      *(next_row_err_mark+1) += chan_index[pixsum].e4;

      if (j&1) cur_row--;
      dest_row--;
      cur_row_err_mark--;
      next_row_err_mark--;
    }

    temp = cur_row_error;
    cur_row_error = next_row_error;
    next_row_error = temp;

    memset(next_row_error, 0, cols+2);
  }

  if (channel == cr) {
    channel = cb;
    chan_index = cb_index;
    memset(cur_row_error, 0, cols+2);

    goto repeat;
  }

  dest_row = disp;


  for (i=0; i<rows; i++) {
    for (j=0; j<cols; j++) {
      *dest_row =  pixel[*dest_row];
      dest_row++;
    }
  }
}















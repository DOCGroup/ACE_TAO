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
#include "fs2.h"
#include "proto.h"

ACE_RCSID(mpeg_client, fs2, "$Id$")

/* Structures for precomputed error propogation values. */

static FS2DithVal lum_index[256];
static FS2DithVal cr_index[256];
static FS2DithVal cb_index[256];



/*
 *--------------------------------------------------------------
 *
 * InitFS2Dither --
 *
 *	Initializes structures for precomputed 2 error f-s dithering.
 *      The value field of the structure contains the pixel component 
 *      of the particular channel in question. Thus the addition of
 *      the value field of a structure in the luminance index, a 
 *      structure in the Cr index, and a structure in the Cb index will
 *      yeild a color number. This color number can then be transformed
 *      into a pixel value to be displayed. Each channel can then be
 *      processed (i.e. dithered) separately, with the results being
 *      added up and remapped to yield a final pixel value.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *      None.
 *
 *--------------------------------------------------------------
 */

void InitFS2Dither()
{
  int i;

  /* For each possible pixel value, precompute propogated error and
     store in array.
  */
  
  for (i=0; i<256; i++) {
    lum_index[i].value = (i * LUM_RANGE) / 256;

    lum_index[i].e1 = (i-lum_values[lum_index[i].value]) / 2;
    lum_index[i].e3 = (i - lum_values[lum_index[i].value]) - lum_index[i].e1;

    lum_index[i].value *= LUM_BASE;

    cr_index[i].value = (i * CR_RANGE) / 256; 

    cr_index[i].e1 = (i - cr_values[cr_index[i].value]) / 2;
    cr_index[i].e3 = (i - cr_values[cr_index[i].value]) - cr_index[i].e1 ;

    cr_index[i].value *= CR_BASE;

    cb_index[i].value = (i * CB_RANGE) / 256; 

    cb_index[i].e1 = (i - cb_values[cb_index[i].value]) / 2;
    cb_index[i].e3 = (i - cb_values[cb_index[i].value]) - cb_index[i].e1;

    cb_index[i].value *= CB_BASE;

  }

}


/*
 *--------------------------------------------------------------
 *
 * DitherImage --
 *
 *	Converts lum, cr, cb image planes into fixed colormap
 *      space.
 *
 * Results:
 *	the display plane is replaced by 8-bit colormap space
 *      image.
 *
 * Side effects:
 *      Hopefully, none.
 *
 *--------------------------------------------------------------
 */

void FS2DitherImage(unsigned char *lum, unsigned char *cr,
		    unsigned char *cb, unsigned char *disp,
		    int rows, int cols)
{
  static char *cur_row_error, *next_row_error;
  static int first = 1;
  char  *cur_row_err_mark, *next_row_err_mark;
  char *temp;
  int i, j, pixsum, c_cols;
  unsigned char *cur_row, *channel, *dest_row;
  FS2DithVal *chan_index;

  /* Allocate error arrays. */

  if (first) {
    cur_row_error = (char *) ACE_OS::malloc(cols+2);
    next_row_error = (char *) ACE_OS::malloc(cols+2);
    first = 0;
  }

  /* Initialize error arrays. */

  memset(cur_row_error, 0, cols+2);
  memset(next_row_error, 0, cols+2);

  /* Use luminance values first. */

  /* For each two rows, do... */

  for(i=0; i<rows; i+=2) {

    /* Establish pointer to current source and destination rows. */
    cur_row = lum + (i*cols);
    dest_row = disp + (i*cols);

    /* Establish pointers to error arrays. */
    cur_row_err_mark = cur_row_error + 1;
    next_row_err_mark = next_row_error + 1;

    
    /* For each column within first row do... */

    for (j=0; j<cols; j++) {

      /* Calculate pixel value with error. */

      pixsum = *cur_row + *cur_row_err_mark;

      /* Bounds check. */
      if (pixsum < 0) pixsum = 0;
      else if (pixsum > 255) pixsum = 255;

      /* Establish dest value, propogate errors. */

      *dest_row = lum_index[pixsum].value;
      *(cur_row_err_mark+1) += lum_index[pixsum].e1; 
      *next_row_err_mark += lum_index[pixsum].e3; 

      /* Advance pointers. */

      cur_row++;
      dest_row++;
      cur_row_err_mark++;
      next_row_err_mark++;
    }

    /* Switch error arrays, so next row errors are now current row errors, and
       vice versa. 
    */

    temp = cur_row_error;
    cur_row_error = next_row_error;
    next_row_error = temp;

    /* Reset next row errors. */

    memset(next_row_error, 0, cols+2); 

    /* Establish pointers for second row. This one will be processed right to
       left to establish serpantine motion.
    */

    cur_row += cols-1;
    dest_row += cols-1;
    cur_row_err_mark = cur_row_error + cols;
    next_row_err_mark = next_row_error + cols;

    /* Process each column... */

    for (j=0; j<cols; j++) {

      pixsum = *cur_row + *cur_row_err_mark;
      if (pixsum < 0) pixsum = 0;
      else if (pixsum > 255) pixsum = 255;
      
      *dest_row = lum_index[pixsum].value;
      *(cur_row_err_mark-1) += lum_index[pixsum].e1; 
      *next_row_err_mark += lum_index[pixsum].e3; 
      
      cur_row--;
      dest_row--;
      cur_row_err_mark--;
      next_row_err_mark--;
    }

    /* Switch error arrays. */

    temp = cur_row_error;
    cur_row_error = next_row_error;
    next_row_error = temp;

    /* Reset next row errors. */

    memset(next_row_error, 0, cols+2); 
  }

  /* Reset error arrays. */

  memset(cur_row_error, 0, cols+2); 
  
  /* Establish column length divided by two. */

  c_cols = cols >> 1;

  /* Set channel to Cr. Use Cr index. */

  channel = cr;
  chan_index = cr_index;
  
 repeat:

  /* Process each row of chrominance data... */

  for (i=0; i < rows; i+=2) {

    /* Establish pointers. */

    cur_row = channel + ((i>>1)*c_cols);
    dest_row = disp + (i*cols);

    cur_row_err_mark = cur_row_error+1;
    next_row_err_mark = next_row_error+1;

    /* For each column in row... */

    for (j=0; j<cols; j++) {
      int p_val;

      /* Get pixel value as twos bit complement. */

      p_val = *cur_row;

      /* Add error term. */

      pixsum = *cur_row_err_mark + p_val;

      /* Bounds check. */

      if (pixsum < 0) pixsum = 0;
      else if (pixsum > 255) pixsum = 255;

      /* Increment dest value. */

      *dest_row += chan_index[pixsum].value;

      /* Propogate error values. */

      *(cur_row_err_mark+1) += chan_index[pixsum].e1; 
      *next_row_err_mark += chan_index[pixsum].e3; 


      /* If count is odd, advance source pointer (Cr and Cb channels are 2:1 
	 subsampled.
      */

      if (j&1) cur_row++;

      /* Advance destination and error pointers. */

      dest_row++;
      cur_row_err_mark++;
      next_row_err_mark++;
    }

    /* Switch error arrays. */

    temp = cur_row_error;
    cur_row_error = next_row_error;
    next_row_error = temp;

    /* Reset next row errors. */

    memset(next_row_error, 0, cols+2);

    /* Re-establish pointers. */

    cur_row += c_cols-1;
    dest_row += cols-1;
    cur_row_err_mark = cur_row_error+cols;
    next_row_err_mark = next_row_error+cols;

    /* Process second row right to left. */

    for (j=0; j<cols; j++) {
      int p_val;

      /* Get source value as twos bit complement. */

      p_val = *cur_row;

      /* Add error. */

      pixsum = *cur_row_err_mark + p_val;

      /* Bounds check. */

      if (pixsum < 0) pixsum = 0;
      else if (pixsum > 255) pixsum = 255;

      /* Increment dest value. */

      *dest_row += chan_index[pixsum].value;

      /* Propogate errors. */

      *(cur_row_err_mark-1) += chan_index[pixsum].e1; 
      *next_row_err_mark += chan_index[pixsum].e3; 

      /* If column counters is odd, decrement source pointer. */

      if (j&1) cur_row--;

      /* Decrement dest and error pointers. */

      dest_row--;
      cur_row_err_mark--;
      next_row_err_mark--;
    }

    /* Switch error arrays. */

    temp = cur_row_error;
    cur_row_error = next_row_error;
    next_row_error = temp;

    /* Reinitialize next row errors. */

    memset(next_row_error, 0, cols+2);
  }

  /* If Cr channel completed, set channel to Cb and Cb index and repeat. */

  if (channel == cr) {
    channel = cb;
    chan_index = cb_index;
    memset(cur_row_error, 0, cols+2);

    goto repeat;
  }

  /* Establish pointer to start of display frame. */

  dest_row = disp;

  /* Transform all display values to pixel values. */

  for (i=0; i<rows; i++) {
    for (j=0; j<cols; j++) {
      *dest_row =  pixel[*dest_row];
      dest_row++;
    }
  }
}









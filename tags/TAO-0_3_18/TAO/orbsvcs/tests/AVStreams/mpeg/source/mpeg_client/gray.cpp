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
#include "proto.h"
#include "dither.h"
#include "ace/OS.h"

ACE_RCSID(mpeg_client, gray, "$Id$")


/*
 *--------------------------------------------------------------
 *
 * GrayDitherImage --
 *
 *	Dithers image into 128 gray scales. Simply maps luminance
 *      value into 1 of 128 gray scale colors (divide by two, essentially).
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
GrayDitherImage(unsigned char *lum, unsigned char *cr,
		unsigned char *cb, unsigned char *out,
		int h, int w)
{

  int i, max = w*h/16;

  for (i=0; i<max; i++) {
    out[0] = pixel[lum[0]];
    out[1] = pixel[lum[1]];
    out[2] = pixel[lum[2]];
    out[3] = pixel[lum[3]];
    out[4] = pixel[lum[4]];
    out[5] = pixel[lum[5]];
    out[6] = pixel[lum[6]];
    out[7] = pixel[lum[7]];
    out[8] = pixel[lum[8]];
    out[9] = pixel[lum[9]];
    out[10] = pixel[lum[10]];
    out[11] = pixel[lum[11]];
    out[12] = pixel[lum[12]];
    out[13] = pixel[lum[13]];
    out[14] = pixel[lum[14]];
    out[15] = pixel[lum[15]];
    out += 16;
    lum += 16;
  }
}







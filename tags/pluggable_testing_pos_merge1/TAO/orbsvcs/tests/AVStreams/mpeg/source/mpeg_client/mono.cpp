/* $Id$ */

/*
 * Author:	Yoichiro Ueno (ueno@cs.titech.ac.jp)
 *
 * Copyright (C) 1991, 1992, Yoichiro Ueno.
 *
 * Permission to use, copy, modify, and distribute this software and
 * its documentation for any purpose is hereby granted by the Author without
 * fee, provided that the above copyright notice appear in all copies and
 * that both the copyright notice and this permission notice appear in
 * supporting documentation, and that the name of the Author not be used
 * in advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission.  The Author makes no
 * representations about the suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 *
 * THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 */

#include <string.h>
#include <stdlib.h>
#include "video.h"
#include "proto.h"
#include "dither.h"
#include "ace/OS.h"

ACE_RCSID(mpeg_client, mono, "$Id$")

/*
 *--------------------------------------------------------------
 *
 * MonoDitherImage --
 *
 *	Dithers image into monochrome.
 *	Dither algorithm is based on dither.c in xli.1.11.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	None.
 *
 *--------------------------------------------------------------
 */
#define MaxGrey		65280
#define Threshold	(MaxGrey/2)
#define MinGrey		0

#if ultrix && mips
#	define SEED_BIT 0x01
#	define OPP_SEED_BIT 0x80
#	define SHIFT_SEED_BIT(b) (b <<= 1)
#	define OPP_SHIFT_SEED_BIT(b) (b >>= 1)
#else
#	define SEED_BIT 0x80
#	define OPP_SEED_BIT 0x01
#	define SHIFT_SEED_BIT(b) (b >>= 1)
#	define OPP_SHIFT_SEED_BIT(b) (b <<= 1)
#endif

static int	*curr = NULL;
static int	*next = NULL;

#if 0
void
MonoDitherImage(register unsigned char *lum, unsigned char *cr,
		unsigned char *cb, unsigned char *out,
		int h, int w)
{
  int bit_r2l;
  register unsigned int bit;
  register unsigned int data;
  int i;
  register int j;
  int *swap;
  register int out_err;
  register int next1;
  register int next2;

  if(curr == NULL) {
    curr = (int *)ACE_OS::malloc(sizeof(int) * (w + 2));
    curr += 1;
  }
  if(next == NULL) {
    next = (int *)ACE_OS::malloc(sizeof(int) * (w + 2));
    next += 1;
  }

  bzero ((char *)curr, w * sizeof(*curr));

  bit_r2l = SEED_BIT << (w - 1 & 7);
  for(i = 0; i < h; i ++) {
    if(i & 0x01) {				/* Right to Left */
      bit = bit_r2l;
      data = 0;
      out_err = curr[w-1];
      next1 = 0;
      next2 = 0;
      for (j=(w-1); j>=0; j--)
      {
	out_err = (out_err >> 4) + (lum[j] << 8);
	if(out_err > Threshold) {
	  data |= bit;
	  out_err -= MaxGrey;
	}
	else
	  out_err -= MinGrey;

	next[j+1] = next1 +     (out_err * 3);
	next1     = next2 +     (out_err * 5);
	next2     =             (out_err * 1);
	out_err   = curr[j-1] + (out_err * 7);

	OPP_SHIFT_SEED_BIT(bit);
#if ultrix && mips
	if(bit == 0)
#else
	if(bit > 0x80)
#endif
	{
	  out[j >> 3] = data;
	  bit = OPP_SEED_BIT;
	  data = 0;
	}
      }
      next[0] = next1;
    }
    else {					/* Left to Right */
      bit = SEED_BIT;
      data = 0;
      out_err = curr[0];
      next1 = 0;
      next2 = 0;
      for (j=0; j<w; j++)
      {
	out_err = (out_err >> 4) + (lum[j] << 8);
	if(out_err > Threshold) {
	  data |= bit;
	  out_err = out_err - MaxGrey;
	}
	else
	  out_err = out_err - MinGrey;

	next[j-1] = next1 +     (out_err * 3);
	next1     = next2 +     (out_err * 5);
	next2     =             (out_err * 1);
	out_err   = curr[j+1] + (out_err * 7);

	SHIFT_SEED_BIT(bit);
#if ultrix && mips
	if(bit > 0x80)
#else
	if(bit == 0)
#endif
	{
	  out[j >> 3] = data;
	  bit = SEED_BIT;
	  data = 0;
	}
      }
      next[w-1] = next1;
    }
    
    lum += w;
    out += w >> 3;
    swap = curr;
    curr = next;
    next = swap;
  }
}
#endif    

void
MonoDitherImage(register unsigned char *lum, unsigned char *cr,
		unsigned char *cb, unsigned char *out,
		int h, int w)
{
  register unsigned int bit;
  register unsigned int data;
  register int j;
  int i;
  static unsigned char d[4][4] = {{0, 8, 2, 10},
			  {12, 4, 14, 6},
			  {3, 11, 1, 9},
			  {15, 7, 13, 5}};
  
  for(i = 0; i < h; i ++) {
    bit = SEED_BIT;
    data = 0;
    for (j=0; j<w; j++) {
      if((lum[j] >> 4) > d[i % 4][j % 4]) {
	data |= bit;
      }
      SHIFT_SEED_BIT(bit);
#if ultrix && mips
      if(bit > 0x80)
#else
      if(bit == 0)
#endif
      {
	out[j >> 3] = data;
	bit = SEED_BIT;
	data = 0;
      }
    }
    lum += w;
    out += (w + 7) >> 3;
  }
}


/*
 *--------------------------------------------------------------
 *
 * MonoThresholdImage --
 *
 *	convert image into monochrome with threshold.
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
MonoThresholdImage(unsigned char *lum, unsigned char *cr,
		   unsigned char *cb, unsigned char *out,
		   int h, int w)
{
  unsigned char	bit;
  unsigned char	data;

  bit = SEED_BIT;
  data = 0;
  for (w*=h; w>0; w--) {
    if(*lum++>128)
      data |= bit;

    SHIFT_SEED_BIT(bit);
    if(bit == 0) {
      *out ++ = data;
      bit = SEED_BIT;
      data = 0;
    }
  }
}

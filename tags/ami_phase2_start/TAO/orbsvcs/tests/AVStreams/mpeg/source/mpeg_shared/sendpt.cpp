/* $Id$ */

/* Copyright (c) 1995 Oregon Graduate Institute of Science and Technology
 * P.O.Box 91000-1000, Portland, OR 97291, USA;
 * 
 * Permission to use, copy, modify, distribute, and sell this software and its 
 * documentation for any purpose is hereby granted without fee, provided that 
 * the above copyright notice appear in all copies and that both that 
 * copyright notice and this permission notice appear in supporting 
 * documentation, and that the name of O.G.I. not be used in advertising or 
 * publicity pertaining to distribution of the software without specific, 
 * written prior permission.  O.G.I. makes no representations about the 
 * suitability of this software for any purpose.  It is provided "as is" 
 * without express or implied warranty.
 * 
 * O.G.I. DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING 
 * ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL 
 * O.G.I. BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY 
 * DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN 
 * AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF 
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author: Shanwei Cen
 *         Department of Computer Science and Engineering
 *         email: scen@cse.ogi.edu
 */
#include <string.h>
#include "sendpt.h"

ACE_RCSID(mpeg_shared, sendpt, "$Id$")

/* parameters:
   pat -- pattern
   buf -- buffer for holding sendpattern computed;
   len -- length of pattern and send pattern (number of frames);
   f   -- number of frames to send (represented by result send pattern);
*/
void ComputeSendPattern(char * pat, char * buf, int len, int f)
{
  memset(buf, 0, len);
  if (f >= len)
  {
    memset(buf, 1, len);
  }
  else if (f <= 1)
  {
    buf[0] = 1;
  }
  else
  {
    int i, is = 0, ps = 0, bs = 0;
    
    memset(buf, 1, len);
    for (i = 0; i < len; i++)
    {
      if (pat[i] == 'I')
        is ++;
      else if (pat[i] == 'P')
        ps ++;
      else  /* B frame */
        bs ++;
    }
    f = len - f;	/* number of frames to drop */
    if (f < bs)  /* drops only B frames */
    {
      double scale = (double)bs / (double)f;
      int ptr = 0, ptr1 = 0;
      for (i = 0; i < len; i ++)
      {
	if (pat[i] == 'B')
	{
	  if ( ptr == (int) ((double)ptr1 * scale + 0.5))
	  {
	    buf[i] = 0;
	    ptr1 ++;
	  }
	  ptr ++;
	}
      }
      goto endOfCompute;
    }
    
    /* drop all B frames */
    for (i = 0; i < len; i ++)
      if (pat[i] == 'B')
        buf[i] = 0;

    if (f == bs)
      goto endOfCompute;
    
    if (f > bs && f < bs + ps)  /* drop also some P frames */
    {
      int ptr = len - 1;
      for (i = 0; i < is; i ++)
      {
	int j;
	for (j = 0; j < (f - bs) / is; j ++)
	{
	  while (pat[ptr] != 'P') ptr --;
	  buf[ptr] = 0;
	  ptr --;
	}
	while (pat[ptr] != 'I') ptr --;
      }
      ptr = len - 1;
      for (i = 0; i < (f - bs) % is; i ++)
      {
	while (pat[ptr] != 'P' || buf[ptr] == 0) ptr --;
	buf[ptr] = 0;
	while (pat[ptr] != 'I') ptr --;
      }
      goto endOfCompute;
    }
    
    /* drop all P frames */
    for (i = 0; i < len; i ++)
      if (pat[i] == 'P')
        buf[i] = 0;

    if (f > bs + ps)  /* needs to drop some I frames */
    {
      double scale = (double)is / (double)(f - bs - ps);
      int ptr = 0, ptr1 = 0;
      for (i = 0; i < len; i ++)
      {
	if (pat[i] == 'I')
	{
	  if ( ptr == (int) ((double)ptr1 * scale + 0.5))
	  {
	    buf[i] = 0;
	    ptr1 ++;
	  }
	  ptr ++;
	}
      }
      goto endOfCompute;
    }
  }
 endOfCompute:;
}

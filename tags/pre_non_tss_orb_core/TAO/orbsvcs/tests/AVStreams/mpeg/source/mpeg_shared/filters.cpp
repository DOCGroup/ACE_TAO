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
#include <stdio.h>
#include <limits.h>   /* INT_MIN, INT_MAX are defined here */
#include <errno.h>
#include <stdlib.h>
#include <string.h>
/*
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
*/
#include "filters.h"
#include "ace/OS.h"

ACE_RCSID(mpeg_shared, filters, "$Id$")

#define BUFSIZE 256
#define maxval(a,b) ((a) > (b) ? (a) : (b))
#define minval(a,b) ((a) < (b) ? (a) : (b))

static float FLOAT_MIN = (float)INT_MIN;
static float FLOAT_MAX = (float)INT_MAX;

AverageFilter * NewAverageFilter(int nsamples)
{
  AverageFilter * f;
  f = (AverageFilter *) ACE_OS::malloc(sizeof(*f));
  if (f == NULL)
    return NULL;
  f->type = FILTER_AVERAGE;
  f->nsamples = nsamples;
  f->buf = (double *)ACE_OS::malloc(nsamples * sizeof(double));
  if (f->buf == NULL) {
   ACE_OS::free(f);
    return NULL;
  }
  f->count = 0;
  f->ptr = 0;
  f->sum = 0;
  return f;
}

AverageFilter * ResetAverageFilter(AverageFilter *f, int nsamples)
{
  if (f->nsamples != nsamples) {
   ACE_OS::free(f->buf);
    f->nsamples = nsamples;
    f->buf = (double *)ACE_OS::malloc(nsamples * sizeof(double));
    if (f->buf == NULL) {
     ACE_OS::free(f);
      return NULL;
    }
  }
  f->count = 0;
  f->ptr = 0;
  f->sum = 0;
  return f;
}

void FreeAverageFilter(AverageFilter * f)
{
 ACE_OS::free(f->buf);
 ACE_OS::free(f);
}

double DoAverageFilter(AverageFilter *f, double value)
{
  int offset;
  double oldval = f->buf[f->ptr];
  f->buf[f->ptr] = value;
  f->ptr = (f->ptr + 1) % f->nsamples;
  f->sum += value;
  if (f->count >= f->nsamples) {
    f->sum -= oldval;
  }
  else {
    f->count ++;
  }
  return (double)(f->sum) / (double)f->count;
}


LowPassFilter * NewLowPassFilter(double Rvalue)
{
  LowPassFilter * f;
  f = (LowPassFilter *)ACE_OS::malloc(sizeof(*f));
  if (f == NULL) {
    return NULL;
  }
  if (Rvalue > 1.0) {
    Rvalue = 1.0;
  }
  else if (Rvalue < 0.0) {
    Rvalue = 0.0;
  }
  f->type = FILTER_LOWPASS;
  f->count = 0;
  f->maxcount = (int)(1.0 / maxval(Rvalue, 0.0001));
  f->R = Rvalue;
  f->result = FLOAT_MIN;
  return f;
}

LowPassFilter * ResetLowPassFilter(LowPassFilter * f, double Rvalue)
{
  if (Rvalue > 1.0) {
    Rvalue = 1.0;
  }
  else if (Rvalue < 0.0) {
    Rvalue = 0.0;
  }
  f->count = 0;
  f->maxcount = (int)(1.0 / maxval(Rvalue, 0.0001));
  f->R = Rvalue;
  f->result = FLOAT_MIN;
  return f;
}

void FreeLowPassFilter(LowPassFilter * f)
{
 ACE_OS::free(f);
}

double DoLowPassFilter(LowPassFilter * f, double value)
{
  if (f->count <= f->maxcount) {
    if (!f->count) {
      f->count ++;
      return (f->result = value);
    }
    f->count ++;
    return (f->result = (1.0 - (1.0/(double)f->count)) * f->result +
			 (1.0/(double)f->count) * value);
  }
  return (f->result = (1.0 - f->R) * f->result + f->R * value);
}

MedianFilter * NewMedianFilter(int nsamples)
{
  int minv = 0, maxv = 1;
  MedianFilter * f;
  f = (MedianFilter *) ACE_OS::malloc(sizeof(*f));
  if (f == NULL)
    return NULL;
  f->type = FILTER_MEDIAN;
  f->nsamples = nsamples;
  f->max = maxval(maxv, minv);
  f->min = minval(maxv, minv);
  f->statsize = f->max - f->min + 1;
  f->buf = (int *)ACE_OS::malloc(nsamples * sizeof(int));
  if (f->buf == NULL) {
   ACE_OS::free(f);
    return NULL;
  }
  f->stat = (int *)ACE_OS::malloc(f->statsize * sizeof(int));
  if (f->stat == NULL) {
   ACE_OS::free(f->buf);
   ACE_OS::free(f);
    return NULL;
  }
  f->count = 0;
  f->ptr = 0;
  f->med = 0;
  f->below = 0;
  memset(f->stat, 0, f->statsize * sizeof(int));
  return f;
}

MedianFilter * ResetMedianFilter(MedianFilter * f, int nsamples)
{
  if (f->nsamples != nsamples) {
   ACE_OS::free(f->buf);
    f->nsamples = nsamples;
    f->buf = (int *)ACE_OS::malloc(nsamples * sizeof(int));
    if (f->buf == NULL) {
     ACE_OS::free(f);
      return NULL;
    }
  }
  f->count = 0;
  f->ptr = 0;
  f->med = 0;
  f->below = 0;
  memset(f->stat, 0, f->statsize * sizeof(int));
  return f;
}

void FreeMedianFilter(MedianFilter * f)
{
 ACE_OS::free(f->buf);
 ACE_OS::free(f->stat);
 ACE_OS::free(f);
}

double DoMedianFilter(MedianFilter *f, double pvalue)
{
  int value = (int)pvalue;
  int offset;
  int oldval = f->buf[f->ptr];
  if (value > f->max) {
    int * oldstat = f->stat;
    int oldsize = f->statsize;
    f->max = value + 10;
    f->statsize = f->max - f->min + 1;
    f->stat = (int *)ACE_OS::malloc(f->statsize * sizeof(int));
    if (f->stat == NULL) {
      fprintf(stderr, "MedianFilter Failed to extend up stat to % items",
	      f->statsize);
     ACE_OS::perror ("");
      ACE_OS::exit (1);
    }
    memset((char *)(f->stat) + sizeof(int) * oldsize, 0,
	   sizeof(int) * (f->statsize - oldsize));
    ACE_OS::memcpy (f->stat, oldstat, oldsize * sizeof(int));
  }
  else if (value < f->min) {
    int * oldstat = f->stat;
    int oldsize = f->statsize;
    f->min = value - 10;
    f->statsize = f->max - f->min + 1;
    f->stat = (int *)ACE_OS::malloc(f->statsize * sizeof(int));
    if (f->stat == NULL) {
      fprintf(stderr, "MedianFilter Failed to extend down stat to % items",
	      f->statsize);
     ACE_OS::perror ("");
      ACE_OS::exit (1);
    }
    memset(f->stat, 0, sizeof(int) * (f->statsize - oldsize));
    ACE_OS::memcpy ((char *)(f->stat) + sizeof(int) * (f->statsize - oldsize),
	   oldstat, sizeof(int) * oldsize);
    f->med += f->statsize - oldsize;
  }
  f->buf[f->ptr] = value;
  f->ptr = (f->ptr + 1) % f->nsamples;
  offset = value - f->min;
  if (offset < 0) {
    offset = 0;
  }
  else if (offset >= f->statsize) {
    offset = f->statsize - 1;
  }
  f->stat[offset] ++;
  if (offset < f->med) {
    f->below ++;
  }
  if (f->count >= f->nsamples) {
    offset = oldval - f->min;
    if (offset < 0) {
      offset = 0;
    }
    else if (offset >= f->statsize) {
      offset = f->statsize - 1;
    }
    f->stat[offset] --;
    if (offset < f->med) {
      f->below --;
    }
  }
  else {
    f->count ++;
  }
  oldval = f->count >> 1;
  if (f->below > oldval) {
    offset = f->med - 1;
    while (offset >= 0 && (f->below - f->stat[offset]) > oldval) {
      f->below -= f->stat[offset];
      offset --;
    }
    f->med = offset + 1;
  }
  else {
    offset = f->med;
    while (offset < f->statsize && (f->below + f->stat[offset]) <= oldval) {
      f->below += f->stat[offset];
      offset ++;
    }
    f->med = offset;
  }
  return (double)(f->med + f->min);
}

Filter * NewFilter(int type, int parameter)
{
  switch (type) {
  case FILTER_LOWPASS:
    return (Filter *)NewLowPassFilter(1.0/(double)parameter);
  case FILTER_MEDIAN:
    return (Filter *)NewMedianFilter(parameter);
  case FILTER_AVERAGE:
    return (Filter *)NewAverageFilter(parameter);
  default:
    break;
  }
  return NULL;
}

Filter * ResetFilter(Filter * f, int parameter)
{
  switch (f->type) {
  case FILTER_LOWPASS:
    return (Filter *)ResetLowPassFilter((LowPassFilter *)f, 1.0/(double)parameter);
  case FILTER_MEDIAN:
    return (Filter *)ResetMedianFilter((MedianFilter *)f, parameter);
  case FILTER_AVERAGE:
    return (Filter *)ResetAverageFilter((AverageFilter *)f, parameter);
  default:
    break;
  }
  return NULL;
}

void FreeFilter(Filter * f)
{
  switch (f->type) {
  case FILTER_LOWPASS:
     FreeLowPassFilter((LowPassFilter *)f);
     break;
  case FILTER_MEDIAN:
     FreeMedianFilter((MedianFilter *)f);
     break;
  case FILTER_AVERAGE:
     FreeAverageFilter((AverageFilter *)f);
     break;
  default:
     break;
  }
}

double DoFilter(Filter *f, double value)
{
  switch (f->type) {
  case FILTER_LOWPASS:
     return DoLowPassFilter((LowPassFilter *)f, value);
     break;
  case FILTER_MEDIAN:
     return DoMedianFilter((MedianFilter *)f, value);
     break;
  case FILTER_AVERAGE:
     return DoAverageFilter((AverageFilter *)f, value);
     break;
  default:
     break;
  }
  return FLOAT_MIN;
}

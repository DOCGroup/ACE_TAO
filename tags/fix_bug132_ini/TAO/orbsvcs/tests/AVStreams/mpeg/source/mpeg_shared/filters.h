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

#if !defined (AV_FILTERS_H)
#define AV_FILTERS_H

#define FILTER_LOWPASS  0
#define FILTER_MEDIAN   1
#define FILTER_AVERAGE  2

typedef struct MedianFilter {
  int type;
  int * buf;
  int nsamples;
  int count;
  int ptr;
  int * stat;
  int statsize;
  int max, min;
  int med; /* median offset to stat */
  int below; /* total points below the med entry */
} MedianFilter;

typedef struct LowPassFilter {
  int type;
  int count;
  int maxcount;
  double R;
  double result;
} LowPassFilter;

typedef struct AverageFilter {
  int type;
  double * buf;
  int nsamples;
  int ptr;
  double sum;
  int count;
} AverageFilter;

typedef union Filter {
  int type;
  MedianFilter m;
  AverageFilter a;
  LowPassFilter l;
} Filter;

Filter * NewFilter(int type, int parameter);
Filter * ResetFilter(Filter * f, int parameter);
void FreeFilter(Filter * f);
double DoFilter(Filter *f, double value);

MedianFilter * NewMedianFilter(int nsamples);
MedianFilter * ResetMedianFilter(MedianFilter * f, int nsamples);
void FreeMedianFilter(MedianFilter * f);
double DoMedianFilter(MedianFilter *f, double value);

AverageFilter * NewAverageFilter(int nsamples);
AverageFilter * ResetAverageFilter(AverageFilter * f, int nsamples);
void FreeAverageFilter(AverageFilter * f);
double DoAverageFilter(AverageFilter *f, double value);

LowPassFilter * NewLowPassFilter(double R);
LowPassFilter * ResetLowPassFilter(LowPassFilter * f, double R);
void FreeLowPassFilter(LowPassFilter * f);
double DoLowPassFilter(LowPassFilter *f, double value);

#endif /* AV_FILTERS_H */

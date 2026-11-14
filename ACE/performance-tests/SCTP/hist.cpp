/* Replacement for CSIM histogram created by Alex Carobus 7/20/98
   #include <std/disclaimer.h>
   */
/*
//
// Update history:
//
// [99.10.18] jcohen
//   version 1.3
//   added optional fields to header
//   to use: add_field("key","value",histogram);
//       or: add_field_n("key",45,histogram);
//
// [99.01.08] jcohen
//   version 1.2
//   histogram includes skew and kurtosis
//   call enable_skew(h) to enable skew and kurtosis on histogram h
//
// [98.07.29] jcohen
//   added add_histogram function
//   added version 1.1 display, added num_bins display
//   added read_hist
//
*/

#include <list>
#include <string>
#include <utility>

#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_math.h"
#include "ace/OS_NS_math.h"
#include "hist.h"
namespace ACE_SCTP
{
static std::list<hist_t> histograms;

hist_t *histogram (std::string name,
                   unsigned int num_bins,
                   double first,
                   double last)
{
  hist_t hist;
  hist.name = std::move (name);
  hist.num_bins = num_bins;
  hist.first = first;
  hist.last = last;
  hist.hs.resize (num_bins + 2);
  histograms.emplace_back (std::move (hist));
  return &histograms.back ();
}

void set_outer (unsigned int max_num_outer, hist_t *hist)
{
  hist->outer.resize (max_num_outer);
  hist->max_num_outer = max_num_outer;
}

void enable_skew (hist_t *hist)
{
  hist->skew = 1;
}

void add_field (std::string key, std::string value, hist_t *hist)
{
  hist->optional_headers.emplace_back (std::move (key), std::move (value));
}

void add_field_n (std::string key, int value, hist_t *hist)
{
  add_field (std::move (key), std::to_string (value), hist);
}

void record (double point, hist_t *hist)
{
  unsigned int bin;
  double incsum;
  if (point < hist->min) hist->min = point;
  if (point > hist->max) hist->max = point;
  if (point < hist->first) {
    bin = 0;
    if (hist->max_num_outer > hist->num_outer)
      hist->outer[hist->num_outer++] = point;
  }
  else if (point >= hist->last) {
    bin = hist->num_bins+1;
    if (hist->max_num_outer > hist->num_outer)
      hist->outer[hist->num_outer++] = point;
  }
  else
    bin = (unsigned int)(((point - hist->first)*(hist->num_bins))/(hist->last - hist->first)+1);

  hist->hs[bin]++;
  ++hist->num_points;
  hist->sum+=point;
  incsum = point*point;
  hist->sum2 += incsum;
  if (hist->skew) {
    incsum *= point;
    hist->sum3 += incsum;
    incsum *= point;
    hist->sum4 += incsum;
  }

/*  printf("point : %g, bin %u, points in bin %u\n", point, bin, hist->hs[bin]); */
}

void report_to (std::FILE *strm, hist_t const *hist)
{
  unsigned int sofar, i;
  double p1, p2, d, mean;
  double variance;

  if (hist->num_points == 0) {
    ACE_OS::fprintf(strm, "\n\n\t\t\t\tHistogram %s is empty\n\n", hist->name.c_str ());
    return;
  }
  ACE_OS::fprintf(strm, "\n\n\t\t\t\tHistogram %s\n", hist->name.c_str ());
  if (hist->skew) ACE_OS::fprintf(strm, "version: %s\n", HIST_VERSION);
  else ACE_OS::fprintf(strm, "version: 1.1\n");
  ACE_OS::fprintf(strm, "minimum: %g\n", hist->min);
  ACE_OS::fprintf(strm, "maximum: %g\n", hist->max);
  ACE_OS::fprintf(strm, "mean: %g\n", mean = hist->sum/hist->num_points);
  variance = (hist->sum2 -
          2*hist->sum*mean +
          hist->num_points*mean*mean) / (hist->num_points-1);
  ACE_OS::fprintf(strm, "variance: %g\n", variance);
  if (hist->skew)
  {
    ACE_OS::fprintf(strm, "skew: %g\n", ((hist->sum3 -
                    3*hist->sum2*mean +
                    3*hist->sum*mean*mean -
                    hist->num_points*mean*mean*mean) /
                    pow(sqrt(variance),3) / hist->num_points));

    ACE_OS::fprintf(strm, "kurtosis: %g\n", ((hist->sum4 -
                    4*hist->sum3*mean +
                    6*hist->sum2*mean*mean -
                    4*hist->sum*mean*mean*mean +
                    hist->num_points*mean*mean*mean*mean) /
                    pow(sqrt(variance),4)) / hist->num_points - 3);
  }
  ACE_OS::fprintf(strm, "num_points: %u\n", hist->num_points);
  ACE_OS::fprintf (strm, "num_bins: %u %g %g\n", hist->num_bins, hist->first, hist->last);
  for (std::pair<std::string, std::string> const &field : hist->optional_headers)
    ACE_OS::fprintf (strm, "%s: %s\n", field.first.c_str (), field.second.c_str ());
  ACE_OS::fprintf(strm, "\n");
  sofar = hist->hs[0];
  ACE_OS::fprintf(strm, "\t      Low     -          High     Count  Fraction  Cumulative\n");
  ACE_OS::fprintf(strm, "\t     below    -  %12.3f  :  %5u    %0.3f    %0.3f\n", hist->first,
                  hist->hs[0], (double)hist->hs[0]/hist->num_points,
                  (double)histfloor(1000.0*sofar/hist->num_points)/1000.0);
  p2 = hist->first;
  d = (hist->last - hist->first)/hist->num_bins;
  for (i = 1; i <= hist->num_bins; ++i) {
    p1 = p2;
    p2 = i*d+hist->first;
    sofar += hist->hs[i];
    ACE_OS::fprintf(strm, "\t%12.3f  -  %12.3f  :  %5u    %0.3f    %0.3f\n", p1, p2, hist->hs[i],
                    (double)hist->hs[i]/hist->num_points,
                    (double)histfloor(1000.0*sofar/hist->num_points)/1000.0);
  }

  sofar += hist->hs[hist->num_bins+1];
  ACE_OS::fprintf(strm, "\t%12.3f  -       above    :  %5u    %0.3f    %0.3f\n\n",
                  hist->last, hist->hs[hist->num_bins+1],
                  (double)hist->hs[hist->num_bins+1]/hist->num_points,
                  (double)histfloor(1000.0*sofar/hist->num_points)/1000.0);

  if (hist->num_outer)
  {
    ACE_OS::fprintf(strm, "outliers:\n");
    for (i = 0; i < hist->num_outer; ++i)
      ACE_OS::fprintf (strm, "\t%12.3f\n", hist->outer[i]);
  }
  ACE_OS::fprintf(strm, "\n\n");
}

void report ()
{
  for (hist_t const &hist : histograms)
    report_to (stdout, &hist);
}

double get_mean (hist_t const *hist)
{
  return(hist->sum/hist->num_points);
}

double get_min (hist_t const *hist)
{
  return(hist->min);
}

double get_max (hist_t const *hist)
{
  return(hist->max);
}

double get_variance (hist_t const *hist)
{
  double mean = hist->sum/hist->num_points;
  return((hist->sum2 - 2*hist->sum*mean + hist->num_points*mean*mean)/
    (hist->num_points-1));
}

double get_num (hist_t const *hist)
{
  return(hist->num_points);
}

void set_mean (hist_t *hist, double mean)
{
  hist->sum = mean*hist->num_points;
}

void set_min (hist_t *hist, double min)
{
 hist->min = min;
}

void set_max (hist_t *hist, double max)
{
  hist->max = max;
}

void set_variance (hist_t *hist, double variance)
{
  /* do this after seting mean */
  double mean = hist->sum/hist->num_points;
  hist->sum2 = variance *(hist->num_points-1) + 2*hist->sum*mean - hist->num_points*mean*mean;
}

void add_histogram (hist_t *dest, hist_t const *source)
{
  unsigned int i,j;
  double d,midpoint;

   d = (source->last - source->first)/source->num_bins;

   for (i = 1; i <= source->num_bins; ++i) {
      midpoint = source->first + ((i - 0.5) * d);
      for (j = 0; j < source->hs[i]; ++j) {
         record(midpoint,dest);
      }
   }

   for (i = 0; i < source->num_outer; ++i)
      record(source->outer[i],dest);

   set_min(dest, get_min(source));
   set_max(dest, get_max(source));
   set_mean(dest, get_mean(source));
   set_variance(dest, get_variance(source));
}

double histfloor (double x)
{
#ifdef WIN32
  return ACE_OS::floor (x);
#else
  return static_cast<double> (static_cast<long long> (x));
#endif
}
}

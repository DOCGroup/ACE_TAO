#ifndef ACE_PERFORMANCE_TESTS_SCTP_HIST_H
#define ACE_PERFORMANCE_TESTS_SCTP_HIST_H

#include <cfloat>
#include <cstdio>
#include <string>
#include <utility>
#include <vector>

namespace ACE_SCTP
{
  inline constexpr char HIST_VERSION[] = "1.3";

  struct hist_t
  {
    std::string name;
    double max {DBL_MIN};
    double min {DBL_MAX};
    double sum {};
    double sum2 {};
    double sum3 {};
    double sum4 {};
    double first {};
    double last {};
    unsigned int num_points {};
    unsigned int num_bins {};
    std::vector<unsigned int> hs;
    unsigned int num_outer {};
    unsigned int max_num_outer {};
    std::vector<double> outer;
    int skew {};
    std::vector<std::pair<std::string, std::string>> optional_headers;
  };

  // The histogram registry in hist.cpp owns these; pointers remain valid
  // until the process exits.
  using HIST = hist_t *;

  hist_t *histogram (std::string name,
                     unsigned int num_bins,
                     double first,
                     double last);

  void set_outer (unsigned int max_num_outer, hist_t *hist);

  void enable_skew (hist_t *hist);

  void add_field (std::string key, std::string value, hist_t *hist);

  void add_field_n (std::string key, int value, hist_t *hist);

  void record (double point, hist_t *hist);

  void report_to (std::FILE *strm, hist_t const *hist);

  void report ();

  double get_mean (hist_t const *hist);

  double get_min (hist_t const *hist);

  double get_max (hist_t const *hist);

  double get_variance (hist_t const *hist);

  double get_num (hist_t const *hist);

  void set_mean (hist_t *hist, double mean);

  void set_min (hist_t *hist, double min);

  void set_max (hist_t *hist, double max);

  void set_variance (hist_t *hist, double variance);

  void add_histogram (hist_t *dest, hist_t const *source);

  double histfloor (double x);
}

#endif // ACE_PERFORMANCE_TESTS_SCTP_HIST_H

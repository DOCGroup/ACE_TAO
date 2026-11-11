#include <stdio.h>
#include <string>
#include <utility>
#include <vector>

#define HIST_VERSION  "1.3"

namespace ACE_SCTP
{
  typedef struct hist_s {
    std::string name;
    double max, min, sum, sum2, sum3, sum4;
    double first, last;
    unsigned int num_points, num_bins;
    std::vector<unsigned int> hs;
    unsigned int num_outer, max_num_outer;
    std::vector<double> outer;
    int skew;
    std::vector<std::pair<std::string, std::string>> optional_headers;
  } hist_t;

  // The histogram registry in hist.cpp owns these; pointers remain valid
  // until the process exits.
  using HIST = hist_t *;

  hist_t *histogram(std::string name, unsigned int num_bins, double first,
                    double last);

  void set_outer(unsigned int max_num_outer, hist_t *hist);

  void enable_skew(hist_t *hist);

  void add_field(std::string key, std::string value, hist_t *hist);

  void add_field_n(std::string key, int value, hist_t *hist);

  void record(double point, hist_t *hist);

  void report_to(FILE *strm, hist_t *hist);

  void report();

  void stats_init();

  double get_mean(HIST hist);

  double get_min(HIST hist);

  double get_max(HIST hist);

  double get_variance(HIST hist);

  double get_num(HIST hist);

  void set_mean(HIST hist, double mean);

  void set_min(HIST hist, double min);

  void set_max(HIST hist, double max);

  void set_variance(HIST hist, double variance);

  void add_histogram(HIST dest, HIST source);

  double histfloor(double x);
}

// $Id$

#include <stdio.h>

#define HIST_VERSION  "1.3"


struct optheader {
  char *key;
  char *value;
  struct optheader *next;
};

typedef struct hist_s {
  char *name;
  double max, min, sum, sum2, sum3, sum4;
  double first, last;
  unsigned int num_points, num_bins;
  unsigned int *hs;
  unsigned int num_outer, max_num_outer;
  double *outer;
  struct hist_s *next;
  int skew;
  struct optheader *firstoptheader;
} hist_t;

#define HIST hist_t *

hist_t *histogram(char *name, unsigned int num_bins, double first,
                  double last);

void set_outer(unsigned int max_num_outer, hist_t *hist);

void enable_skew(hist_t *hist);

void add_field(char *key, char *value, hist_t *hist);

void add_field_n(char *key, int value, hist_t *hist);

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

// $Id$

#include "ECT_Driver.h"

#if !defined (__ACE_INLINE__)
#include "ECT_Driver.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(EC_Throughput, ECT_Driver, "$Id$")

ECT_Driver::~ECT_Driver (void)
{
}

void
ECT_Driver::dump_latency_results (const char* d)
{
  this->end_to_end_.dump_results (d, "end to end");
  this->supplier_to_ec_.dump_results (d, "supplier to ec");
  this->inside_ec_.dump_results (d, "inside ec");
  this->ec_to_consumer_.dump_results (d, "ec to consumer");
}

// ****************************************************************

void
ECT_Driver::Latency_Stats::dump_results (const char *description,
                                         const char *desc2)
{
  if (this->n_ <= 1)
    return;

  ACE_hrtime_t avg = this->sum_ / this->n_;
  ACE_hrtime_t dev =
    this->sum2_ / (this->n_ - 1)
    - (this->sum_ / this->n_) * (this->sum_ / (this->n_ - 1));

  double min_usec = this->min_ / 1000.0;
  double max_usec = this->max_ / 1000.0;
  double avg_usec = avg / 1000.0;
  double dev_usec = dev / 1000.0;
  ACE_DEBUG ((LM_DEBUG,
              "%s (%s): %.2f/%.2f/%.2f/%.2f (min/avg/max/dev2) [usecs]\n",
              description, desc2,
              min_usec, avg_usec, max_usec, dev_usec));
}


// $Id$

#include "ECT_Driver.h"
#include "ace/High_Res_Timer.h"

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
#if 0
  this->supplier_to_ec_.dump_results (d, "supplier to ec");
  this->inside_ec_.dump_results (d, "inside ec");
  this->ec_to_consumer_.dump_results (d, "ec to consumer");
#endif /* 0 */
}

// ****************************************************************

void
ECT_Driver::Latency_Stats::dump_results (const char *test_name,
                                         const char *sub_test)
{
  if (this->n_ <= 1)
    return;

  ACE_hrtime_t avg = this->sum_ / this->n_;
  ACE_hrtime_t dev =
    this->sum2_ / (this->n_ - 1)
    - (this->sum_ / this->n_) * (this->sum_ / (this->n_ - 1));

  double min_usec = ACE_CU64_TO_CU32 (this->min_) / 1000.0;
  double max_usec = ACE_CU64_TO_CU32 (this->max_) / 1000.0;
  double avg_usec = ACE_CU64_TO_CU32 (avg) / 1000.0;
  double dev_usec = ACE_CU64_TO_CU32 (dev) / 1000.0;
  ACE_DEBUG ((LM_DEBUG,
              "%s/%s: %.2f/%.2f/%.2f/%.2f (min/avg/max/dev2) [usecs]\n",
              test_name, sub_test,
              min_usec, avg_usec, max_usec, dev_usec));
}

void
ECT_Driver::Latency_Stats::accumulate (const Latency_Stats& rhs)
{
  if (rhs.n_ == 0)
    return;

  if (this->n_ == 0)
    {
      *this = rhs;
      return;
    }

  if (this->min_ > rhs.min_)
    this->min_ = rhs.min_;
  if (this->max_ < rhs.max_)
    this->max_ = rhs.max_;

  this->sum_ += rhs.sum_;
  this->sum2_ += rhs.sum2_;
  this->n_ += rhs.n_;
}

// ****************************************************************

ECT_Driver::Throughput_Stats::Throughput_Stats (void)
  :  n_ (0),
     done_ (0)
{
}

void
ECT_Driver::Throughput_Stats::accumulate (const Throughput_Stats& rhs)
{
  if (!this->done_)
    {
      this->done_ = 1;
      this->start_ = rhs.start_;
      this->stop_ = rhs.stop_;
      this->n_ = rhs.n_;
      return;
    }

  if (this->start_ > rhs.start_)
    this->start_ = rhs.start_;

  if (this->stop_ < rhs.stop_)
    this->stop_ = rhs.stop_;

  this->n_ += rhs.n_;
}

void
ECT_Driver::Throughput_Stats::dump_results (const char *test_name,
                                            const char *subtest)
{
  if (this->n_ == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s/%s: no events recorded\n",
                  test_name, subtest));
      return;
    }

  if (this->done_ == 0)
    {
      this->stop ();
      ACE_DEBUG ((LM_DEBUG,
                  "%s/%s: incomplete data,"
                  " potentially skewed results\n",
                  test_name, subtest));
    }

  ACE_Time_Value tv;
  ACE_High_Res_Timer::hrtime_to_tv (tv, this->stop_ - this->start_);

  double f = 1.0/(tv.sec () + tv.usec () / 1000000.0);
  double events_per_second = this->n_ * f;

  ACE_DEBUG ((LM_DEBUG,
              "%s/%s: "
              "%d / %d.%08.8d = %.3f events/second\n",
              test_name, subtest,
              this->n_,
              tv.sec (), tv.usec (),
              events_per_second));
}

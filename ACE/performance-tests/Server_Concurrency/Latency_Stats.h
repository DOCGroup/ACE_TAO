#ifndef ACE_PERFORMANCE_TESTS_SERVER_CONCURRENCY_LATENCY_STATS_H
#define ACE_PERFORMANCE_TESTS_SERVER_CONCURRENCY_LATENCY_STATS_H

#include "ace/High_Res_Timer.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_Thread.h"
#include "ace/Sched_Params.h"

#include <stddef.h>

class Latency_Stats
{
public:
  Latency_Stats () = default;

  void dump_results (ACE_TCHAR const *test_name,
                     ACE_TCHAR const *sub_test) const;

  void sample (ACE_hrtime_t const sample);

  /// Merge another latency sample set into this one.
  void accumulate (Latency_Stats const &stats);

private:
  size_t n_ {};
  ACE_hrtime_t sum_ {};
  ACE_hrtime_t sum2_ {};
  ACE_hrtime_t min_ {};
  ACE_hrtime_t max_ {};
};

inline void
Latency_Stats::sample (ACE_hrtime_t const sample)
{
  this->sum_  += sample;
  this->sum2_ += sample * sample;
  if (this->n_ == 0)
    {
      this->min_ = sample;
      this->max_ = sample;
    }
  if (this->min_ > sample)
    this->min_ = sample;
  if (this->max_ < sample)
    this->max_ = sample;
  ++this->n_;
}

inline void
Latency_Stats::dump_results (ACE_TCHAR const *test_name,
                             ACE_TCHAR const *sub_test) const
{
  if (this->n_ < 1)
    return;

  ACE_hrtime_t const avg {this->sum_ / this->n_};
  ACE_hrtime_t const dev {this->sum2_ / this->n_ - avg * avg};
  ACE_High_Res_Timer::global_scale_factor_type const gsf {
    ACE_High_Res_Timer::global_scale_factor ()};

  ACE_UINT64 const min_usec {this->min_ / gsf};
  ACE_UINT64 const max_usec {this->max_ / gsf};
  ACE_UINT64 const avg_usec {avg / gsf};
  ACE_UINT64 const dev_usec {dev / (gsf * gsf)};
  ACE_DEBUG ((LM_DEBUG,
              "%s/%s: %.2f/%.2f/%.2f/%.2f (min/avg/max/var^2) [usecs]\n",
              test_name, sub_test,
              static_cast<double> (min_usec),
              static_cast<double> (avg_usec),
              static_cast<double> (max_usec),
              static_cast<double> (dev_usec)));
}

inline void
Latency_Stats::accumulate (Latency_Stats const &rhs)
{
  if (rhs.n_ == 0)
    return;

  if (this->n_ == 0)
    {
      this->n_    = rhs.n_;
      this->min_  = rhs.min_;
      this->max_  = rhs.max_;
      this->sum_  = rhs.sum_;
      this->sum2_ = rhs.sum2_;
      return;
    }

  if (this->min_ > rhs.min_)
    this->min_ = rhs.min_;
  if (this->max_ < rhs.max_)
    this->max_ = rhs.max_;

  this->sum_  += rhs.sum_;
  this->sum2_ += rhs.sum2_;
  this->n_    += rhs.n_;
}

class Throughput_Stats
{
public:
  Throughput_Stats () = default;

  void dump_results (ACE_TCHAR const *test_name,
                     ACE_TCHAR const *sub_test) const;

  /// Record one received event.
  void sample ();

  /// Merge another event count and its time range into this one.
  void accumulate (Throughput_Stats const &stats);

private:
  size_t n_ {};
  ACE_hrtime_t start_ {};
  ACE_hrtime_t stop_ {};
};

inline void
Throughput_Stats::accumulate (Throughput_Stats const &rhs)
{
  if (rhs.n_ == 0)
    return;

  if (this->n_ == 0)
    {
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

inline void
Throughput_Stats::dump_results (ACE_TCHAR const *test_name,
                                ACE_TCHAR const *subtest) const
{
  if (this->n_ == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s/%s: no events recorded\n",
                  test_name, subtest));
      return;
    }

  ACE_Time_Value tv {};
  ACE_High_Res_Timer::hrtime_to_tv (tv, this->stop_ - this->start_);

  double const elapsed_seconds {
    static_cast<double> (tv.sec ())
      + static_cast<double> (tv.usec ()) / 1000000.0};
  if (elapsed_seconds <= 0.0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s/%s: elapsed time is too short to measure\n",
                  test_name, subtest));
      return;
    }
  double const events_per_second {
    static_cast<double> (this->n_) / elapsed_seconds};

  ACE_DEBUG ((LM_DEBUG,
              "%s/%s: %B / %.6f = %.3f events/second\n",
              test_name, subtest,
              this->n_,
              elapsed_seconds,
              events_per_second));
}

inline void
Throughput_Stats::sample ()
{
  if (this->n_ == 0)
    {
      this->start_ = ACE_OS::gethrtime ();
    }
  ++this->n_;
  this->stop_ = ACE_OS::gethrtime ();
}

inline void
move_to_rt_class ()
{
  // Enable FIFO scheduling
  int const priority_min {ACE_Sched_Params::priority_min (ACE_SCHED_FIFO)};
  int const priority_max {ACE_Sched_Params::priority_max (ACE_SCHED_FIFO)};
  int const priority {(priority_min + priority_max) / 2};

  int result {ACE_OS::sched_params (ACE_Sched_Params (ACE_SCHED_FIFO,
                                                       priority,
                                                       ACE_SCOPE_PROCESS))};
  if (result == 0)
    {
      result = ACE_OS::thr_setprio (priority);
    }

  if (result != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Cannot move program to realtime class.\n"));
    }
}

#endif /* ACE_PERFORMANCE_TESTS_SERVER_CONCURRENCY_LATENCY_STATS_H */

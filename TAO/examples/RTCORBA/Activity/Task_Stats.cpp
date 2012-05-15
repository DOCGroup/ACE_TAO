//$Id$
#include "Task_Stats.h"
#include "ace/Log_Msg.h"

#if !defined (__ACE_INLINE__)
#include "Task_Stats.inl"
#endif /* __ACE_INLINE__ */

Base_Time::Base_Time (void)
{
  base_time_ = ACE_OS::gethrtime ();
}

Task_Stats::Task_Stats (size_t max_samples)
  : base_time_(0),
    end_time_ (0),
    max_samples_ (max_samples),
    samples_count_ (0),
    time_inv_ (0),
    time_exec_ (0),
    exec_time_min_ (0),
    exec_time_min_at_ (0),
    exec_time_max_ (0),
    exec_time_max_at_(0),
    sum_ (0),
    sum2_ (0)
{
}

Task_Stats::~Task_Stats (void)
{
  delete[] this->time_inv_;
  delete[] this->time_exec_;
}

int
Task_Stats::init (void)
{
  ACE_NEW_RETURN (this->time_inv_, ACE_UINT64[this->max_samples_], -1);
  ACE_NEW_RETURN (this->time_exec_, ACE_UINT64[this->max_samples_], -1);
  return 0;
}

void
Task_Stats::base_time (ACE_hrtime_t time)
{
  base_time_ = time;
}

void
Task_Stats::end_time (ACE_hrtime_t time)
{
  end_time_ = time;
}

void
Task_Stats::dump_samples (
  const ACE_TCHAR *file_name,
  const ACE_TCHAR *msg,
  ACE_High_Res_Timer::global_scale_factor_type scale_factor)
{
  FILE* output_file = ACE_OS::fopen (file_name, "w");

  // first dump what the caller has to say.
  ACE_OS::fprintf (output_file, "%s\n", ACE_TEXT_ALWAYS_CHAR (msg));

  // next, compose and dump what we want to say.

  // calc throughput.

  ACE_TCHAR out_msg[BUFSIZ];

  ACE_hrtime_t elapsed_microseconds = (end_time_ - base_time_) / scale_factor;
  double elapsed_seconds =
    ACE_CU64_TO_CU32(elapsed_microseconds) / 1000000.0;
  double throughput =
    double(samples_count_) / elapsed_seconds;

  ACE_OS::sprintf (out_msg, ACE_TEXT("#Throughtput: %f\n"), throughput);
  ACE_OS::fprintf (output_file, ACE_TEXT("%s\n"),out_msg);

  // dump latency stats.
  this->dump_latency_stats (out_msg, scale_factor);
  ACE_OS::fprintf (output_file, "%s\n", ACE_TEXT_ALWAYS_CHAR (out_msg));
  ACE_OS::fprintf (output_file, "#Invocation time \t Execution time\n");

  // dump the samples recorded.
  for (size_t i = 0; i != this->samples_count_; ++i)
    {
      ACE_UINT64 x = this->time_inv_[i] / scale_factor;
      ACE_UINT32 val_1 = ACE_CU64_TO_CU32 (x);

      ACE_UINT64 y = this->time_exec_[i] / scale_factor;
      ACE_UINT32 val_2 = ACE_CU64_TO_CU32 (y);

      ACE_OS::fprintf (output_file, "%u \t %u\n",val_1, val_2);
    }

  ACE_OS::fclose (output_file);
}

void
Task_Stats::dump_latency_stats (
  ACE_TCHAR *out_msg,
  ACE_High_Res_Timer::global_scale_factor_type sf)
{
  if (this->samples_count_ == 0u)
    {
      ACE_OS::sprintf (out_msg,
                       ACE_TEXT ("# no data collected\n"));
      return;
    }

  ACE_UINT64 avg = this->sum_ / this->samples_count_;
  ACE_UINT64 dev =
  this->sum2_ / this->samples_count_ - avg * avg;

  ACE_UINT64 l_min_ = this->exec_time_min_ / sf;
  ACE_UINT32 l_min = ACE_CU64_TO_CU32 (l_min_);

  ACE_UINT64 l_max_ = this->exec_time_max_ / sf;
  ACE_UINT32 l_max = ACE_CU64_TO_CU32 (l_max_);

  double l_avg = ACE_UINT64_DBLCAST_ADAPTER(avg) / sf;
  double l_dev = ACE_UINT64_DBLCAST_ADAPTER(dev) / (sf * sf);

  ACE_UINT64 tmin_ = this->time_inv_[0] / sf;
  ACE_UINT32 tmin = ACE_CU64_TO_CU32 (tmin_);

  ACE_UINT64 tmax_ = this->time_inv_[samples_count_-1] / sf;
  ACE_UINT32 tmax = ACE_CU64_TO_CU32 (tmax_);

  ACE_OS::sprintf(out_msg,
                ACE_TEXT ("#latency   : %u[%d]/%.2f/%u[%d]/%.2f (min/avg/max/var^2)\n #first invocation time = %u, last invocation time = %u\n"),
                l_min, this->exec_time_min_at_,
                l_avg,
                l_max, this->exec_time_max_at_,
                l_dev,
                tmin,tmax);
}

#if defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)
template ACE_Singleton<Base_Time, TAO_SYNCH_MUTEX> *ACE_Singleton<Base_Time, TAO_SYNCH_MUTEX>::singleton_;
#endif /* ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION */

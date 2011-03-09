//$Id$

#include "Task_Stats.h"
#include "ace/ACE.h"
#include "ace/Log_Msg.h"

#if !defined (__ACE_INLINE__)
#include "Task_Stats.inl"
#endif /* __ACE_INLINE__ */

ACE_UINT32 Task_Stats::gsf_ = ACE_High_Res_Timer::global_scale_factor ();

Base_Time::Base_Time (void)
{
  base_time_ = ACE_OS::gethrtime ();
}

Task_Stats::Task_Stats (void)
  : base_time_(0),
    end_time_ (0),
    max_samples_ (0),
    samples_count_ (0),
    time_inv_ (0),
    time_exec_ (0),
    mean_ (0),
    var_2_ (0)
{
}

Task_Stats::~Task_Stats (void)
{
  delete[] this->time_inv_;
  delete[] this->time_exec_;
}

int
Task_Stats::init (size_t max_samples)
{
  this->max_samples_ = max_samples;

  ACE_NEW_RETURN (this->time_inv_, ACE_UINT64[this->max_samples_], -1);
  ACE_NEW_RETURN (this->time_exec_, ACE_UINT64[this->max_samples_], -1);
  return 0;
}

void
Task_Stats::base_time (ACE_UINT64 time)
{
  base_time_ = time;
}

void
Task_Stats::end_time (ACE_UINT64 time)
{
  end_time_ = time;
}

void
Task_Stats::dump_samples (const ACE_TCHAR *file_name, const ACE_TCHAR *msg, int dump_samples)
{
  FILE* output_file = ACE_OS::fopen (file_name, "w");

  // first dump what the caller has to say.
  ACE_OS::fprintf (output_file, "%s\n",msg);

  // next, compose and dump what we want to say.

  // calc throughput.
  double seconds = this->diff_sec (base_time_, end_time_);

  char out_msg[BUFSIZ];

  if (ACE::is_equal (seconds, 0.0) || samples_count_ == 0)
  {
        ACE_OS::sprintf (out_msg,
                   "# No samples recorded\n");
        ACE_OS::fprintf (output_file, "%s",out_msg);
        ACE_OS::fclose (output_file);

        return;
  }

  double t_avg = samples_count_ / seconds;

  ACE_OS::sprintf (out_msg,
                   "# Throughput: %.2f (events/second) ["
                   ACE_SIZE_T_FORMAT_SPECIFIER_ASCII
                   " samples in %.2f seconds]\n",
                   t_avg, samples_count_, seconds);
  ACE_OS::fprintf (output_file, "%s",out_msg);

  // Calc the mean.
  size_t i = 0;

  for (i = 0; i != this->samples_count_; ++i)
    {
      ACE_UINT32 val_2 = Task_Stats::diff_usec (time_inv_[i], time_exec_[i]);

      // Write the normalized value.
      // we will need this to calculate the var^2
      this->time_exec_[i] = val_2;

      this->mean_ += val_2;
    }

  // calculate the mean.
  this->mean_ /= this->samples_count_;

  // Calculate the var^2

  for (i = 0; i != this->samples_count_; ++i)
    {
      ACE_UINT64 diff = this->time_exec_[i] - this->mean_;

      ACE_UINT64 diff_sq =
#if defined ACE_LACKS_LONGLONG_T
        diff * ACE_U64_TO_U32(diff);
#else  /* ! ACE_LACKS_LONGLONG_T */
      diff * diff;
#endif /* ! ACE_LACKS_LONGLONG_T */

      this->var_2_ += diff_sq;
    }

  this->var_2_ /= this->samples_count_;

  ACE_OS::fprintf (output_file, "## Latency: Avg = %u, Var^2 = %u\n"
                   , ACE_CU64_TO_CU32 (this->mean_)
                   , ACE_CU64_TO_CU32 (this->var_2_));

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, " Latency: Avg = %u, Var^2 = %u\n"
                , ACE_CU64_TO_CU32 (this->mean_)
                , ACE_CU64_TO_CU32 (this->var_2_)));

  // if we are asked to, dump the samples recorded.
  if (dump_samples)
    {
      ACE_OS::fprintf (output_file, "#Invocation time \t Execution time\n");

      for (i = 0; i != this->samples_count_; ++i)
        {
          ACE_UINT32 val_1 = Task_Stats::diff_usec (base_time_, time_inv_[i]);

          ACE_OS::fprintf (output_file, "%u \t %u\n",val_1,
                           ACE_CU64_TO_CU32 (time_exec_[i]));
        }
    }

  ACE_OS::fclose (output_file);
}

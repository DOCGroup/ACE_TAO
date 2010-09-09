//$Id$
#include "Task_Stats.h"
#include "ace/OS.h"
#include "ace/Log_Msg.h"

#if !defined (__ACE_INLINE__)
#include "Task_Stats.inl"
#endif /* __ACE_INLINE__ */

Base_Time::Base_Time (void)
{
  base_time_ = ACE_OS::gethrtime ();
}

Task_Stats::Task_Stats (void)
  : base_time_(0),
    end_time_ (0),
    max_samples_ (0),
    samples_count_ (0),
    thr_run_time_ (0),
    thr_count_ (0),
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
  delete[] this->thr_run_time_;
  delete[] this->thr_count_;
}

int
Task_Stats::init (size_t max_samples)
{
  max_samples_ = max_samples;
  ACE_NEW_RETURN (this->thr_run_time_, ACE_UINT32[this->max_samples_], -1);
  ACE_NEW_RETURN (this->thr_count_, int[this->max_samples_], -1);
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
Task_Stats::dump_samples (const ACE_TCHAR *file_name, const ACE_TCHAR *msg,
                          ACE_UINT32)
{
  FILE* output_file = ACE_OS::fopen (file_name, "w");

  if (output_file == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "%s cannot be opened\n",
                  file_name));
    }

  // first dump what the caller has to say.
  ACE_OS::fprintf (output_file, "%s\n",msg);

  // next, compose and dump what we want to say.
  ACE_UINT32 val_1;
  int val_2;
  ACE_UINT64 x;

  // dump the samples recorded.
  for (size_t i = 0; i < this->samples_count_; ++i)
    {
      x = this->thr_run_time_[i];
      val_1 = ACE_CU64_TO_CU32 (x);
      val_2  = this->thr_count_[i];
      ACE_OS::fprintf (output_file, "%u \t %d\n",val_1,val_2);
    }

  ACE_OS::fclose (output_file);

  ACE_DEBUG ((LM_DEBUG,
              "Samples are ready to be viewed\n"));
}


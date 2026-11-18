#include "Task_Stats.h"
#include "ace/ACE.h"
#include "ace/Log_Msg.h"

#include <new>
#include <utility>

using File_Handle = std::unique_ptr<FILE, int (*) (FILE *)>;

#if !defined (__ACE_INLINE__)
#include "Task_Stats.inl"
#endif /* __ACE_INLINE__ */

ACE_High_Res_Timer::global_scale_factor_type Task_Stats::gsf_ =
  ACE_High_Res_Timer::global_scale_factor ();

Base_Time::Base_Time ()
{
  base_time_ = ACE_OS::gethrtime ();
}

Task_Stats::Task_Stats () = default;

Task_Stats::~Task_Stats () = default;

int
Task_Stats::init (size_t max_samples)
{
  std::unique_ptr<ACE_UINT64[]> time_inv (new (std::nothrow) ACE_UINT64[max_samples]);
  if (!time_inv)
  {
    return -1;
  }

  std::unique_ptr<ACE_UINT64[]> time_exec (new (std::nothrow) ACE_UINT64[max_samples]);
  if (!time_exec)
  {
    return -1;
  }

  this->time_inv_ = std::move (time_inv);
  this->time_exec_ = std::move (time_exec);
  this->max_samples_ = max_samples;
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
Task_Stats::dump_samples (ACE_TCHAR const *file_name, ACE_TCHAR const *msg, bool dump_samples)
{
  File_Handle output_file (ACE_OS::fopen (file_name, ACE_TEXT ("w")), ACE_OS::fclose);
  if (!output_file)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Task_Stats::dump_samples: unable to open output file\n")));
      return;
    }

  // first dump what the caller has to say.
  ACE_OS::fprintf (output_file.get (), "%s\n", ACE_TEXT_ALWAYS_CHAR (msg));

  // next, compose and dump what we want to say.

  // calc throughput.
  double const seconds = this->diff_sec (base_time_, end_time_);

  char out_msg[BUFSIZ];

  if (ACE::is_equal (seconds, 0.0) || samples_count_ == 0)
    {
      ACE_OS::sprintf (out_msg, "# No samples recorded\n");
      ACE_OS::fprintf (output_file.get (), "%s", out_msg);
      return;
    }

  double const t_avg = samples_count_ / seconds;

  ACE_OS::sprintf (out_msg,
                   "# Throughput: %.2f (events/second) ["
                   ACE_SIZE_T_FORMAT_SPECIFIER_ASCII
                   " samples in %.2f seconds]\n",
                   t_avg, samples_count_, seconds);
  ACE_OS::fprintf (output_file.get (), "%s", out_msg);

  // Calc the mean.
  for (size_t i {}; i != this->samples_count_; ++i)
    {
      ACE_UINT32 const val_2 = Task_Stats::diff_usec (time_inv_[i], time_exec_[i]);

      // Write the normalized value.
      // we will need this to calculate the var^2
      this->time_exec_[i] = val_2;

      this->mean_ += val_2;
    }

  // calculate the mean.
  this->mean_ /= this->samples_count_;

  // Calculate the var^2

  for (size_t i {}; i != this->samples_count_; ++i)
    {
      ACE_UINT64 const diff = this->time_exec_[i] - this->mean_;
      ACE_UINT64 const diff_sq = diff * diff;

      this->var_2_ += diff_sq;
    }

  this->var_2_ /= this->samples_count_;

  ACE_OS::fprintf (output_file.get (),
                   "## Latency: Avg = %u, Var^2 = %u\n",
                   static_cast<unsigned int> (ACE_CU64_TO_CU32 (this->mean_)),
                   static_cast<unsigned int> (ACE_CU64_TO_CU32 (this->var_2_)));

  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  " Latency: Avg = %u, Var^2 = %u\n",
                  static_cast<unsigned int> (ACE_CU64_TO_CU32 (this->mean_)),
                  static_cast<unsigned int> (ACE_CU64_TO_CU32 (this->var_2_))));
    }

  // if we are asked to, dump the samples recorded.
  if (dump_samples)
    {
      ACE_OS::fprintf (output_file.get (), "#Invocation time \t Execution time\n");

      for (size_t i {}; i != this->samples_count_; ++i)
        {
          ACE_UINT32 const val_1 = Task_Stats::diff_usec (base_time_, time_inv_[i]);

          ACE_OS::fprintf (output_file.get (), "%u \t %u\n", val_1,
                           ACE_CU64_TO_CU32 (time_exec_[i]));
        }
    }
}

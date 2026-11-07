#include "Task_Stats.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdio.h"
#include <memory>
#include <new>
#include <stdexcept>

#if !defined (__ACE_INLINE__)
#include "Task_Stats.inl"
#endif /* __ACE_INLINE__ */

int
Task_Stats::init (std::size_t max_samples)
{
  std::vector<Sample> samples;
  try
    {
      samples.resize (max_samples);
    }
  catch (std::bad_alloc const &)
    {
      return -1;
    }
  catch (std::length_error const &)
    {
      return -1;
    }

  this->samples_.swap (samples);
  this->samples_count_ = 0;
  return 0;
}

void
Task_Stats::dump_samples (ACE_TCHAR const *file_name, ACE_TCHAR const *msg)
{
  std::unique_ptr<FILE, int (*) (FILE *)> output_file (
    ACE_OS::fopen (file_name, ACE_TEXT ("w")), ACE_OS::fclose);

  if (!output_file)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%s cannot be opened\n"),
                  file_name));
      return;
    }

  // first dump what the caller has to say.
  ACE_OS::fprintf (output_file.get (), "%s\n", ACE_TEXT_ALWAYS_CHAR (msg));

  for (std::size_t i = 0; i < this->samples_count_; ++i)
    {
      ACE_OS::fprintf (output_file.get (),
                       "%u \t %d\n",
                       this->samples_[i].run_time,
                       this->samples_[i].count);
    }

  ACE_DEBUG ((LM_DEBUG,
              "Samples are ready to be viewed\n"));
}


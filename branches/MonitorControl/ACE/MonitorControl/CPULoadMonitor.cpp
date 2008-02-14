// $Id$

#include "MonitorControl/CPULoadMonitor.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    CPULoadMonitor<true>::CPULoadMonitor (void)
      : MonitorPoint<true> ("CPULoad",
                            MonitorControl_Types::IT_NUMBER)
      , user_ (0)
      , nice_ (0)
      , kernel_ (0)
      , idle_ (0)
      , prev_idle_ (0)
      , prev_total_ (0)
#if defined (ACE_WIN32)
      , query_ (0)
      , counter_ (0)
#elif defined (linux)
      , file_ptr_ (0)
#elif defined (sun)
      , kstats_ (0)
      , kstat_ (0)
      , kstat_id_ (0)
#endif
    {
#if defined (ACE_WIN32)
      this->status_ = PdhOpenQuery (0, 0, &this->query_);

      if (ERROR_SUCCESS != this->status_)
        {
          ACE_ERROR ((LM_DEBUG, "PdhOpenQuery failed\n"));
        }

      this->status_ =
        PdhAddCounter (this->query_,
                       "\\Processor(_Total)\\% Processor Time",
                       0,
                       &this->counter_);

      if (ERROR_SUCCESS != this->status_)
        {
          ACE_ERROR ((LM_DEBUG, "PdhAddCounter failed\n"));
        }
#elif defined (linux)
      /// All data in this file are stored as running 'jiffy' totals, so we
      /// get values here in the constructor to subtract for the difference
      /// in subsequent calls.
      this->access_proc_stat (&this->prev_idle_);

      this->prev_total_ =
        this->user_ + this->nice_ + this->kernel_ + this->prev_idle_;
#endif
    }

    void
    CPULoadMonitor<true>::update (void)
    {
#if defined (ACE_WIN32)
      PdhCollectQueryData (this->query_);
      PdhGetFormattedCounterValue (this->counter_,
                                   PDH_FMT_DOUBLE,
                                   0,
                                   &this->value_);

      /// Stores value and timestamp with thread-safety.
      this->receive (this->value_.doubleValue);
#elif defined (linux)
      this->access_proc_stat (&this->idle_);    

      double delta_idle = this->idle_ - this->prev_idle_;
      double total =
        this->user_ + this->nice_ + this->kernel_ + this->idle_;
      double delta_total = total - this->prev_total_;
      
      if (delta_total == 0.0)
        {
          /// The system hasn't updated /proc/stat since the last call
          /// to update(), we must avoid dividing by 0.
          return;
        }

      double percent_cpu_load = 100.0 - (delta_idle / delta_total * 100.0);
      this->receive (percent_cpu_load);

      this->prev_idle_ = this->idle_;
      this->prev_total_ = total;
#endif
    }

#if defined (linux)
    void
    CPULoadMonitor<true>::access_proc_stat (unsigned long *which_idle)
    {
      this->file_ptr_ = ACE_OS::fopen ("/proc/stat", "r");
      
      if (this->file_ptr_ == 0)
        {
          ACE_ERROR ((LM_ERROR, "Opening file /proc/stat failed\n"));
          return;
        }
        
      char *item = 0;
      char *arg = 0;

      while ((ACE_OS::fgets (buf_, sizeof (buf_), file_ptr_)) != 0)
        {
          item = ACE_OS::strtok (this->buf_, " \t\n");
          arg = ACE_OS::strtok (0, "\n");
          
          if (item == 0 || arg == 0)
            {
              continue;
            }
          
          if (ACE_OS::strcmp (item, "cpu") == 0)
            {  
              sscanf (arg,
                      "%lu %lu %lu %lu",
                      &this->user_,
                      &this->nice_,
                      &this->kernel_,
                      which_idle);
              break;
            }
        }

      ACE_OS::fclose (this->file_ptr_);    
    }
#endif
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL


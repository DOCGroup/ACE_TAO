// $Id$

#include "MonitorControl/CPULoadMonitor.h"

#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

#if defined (ACE_HAS_KSTAT)
#include <sys/sysinfo.h>
#endif

#if defined (linux)
#include "ace/OS_NS_stdio.h"
#endif

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    const char* CPULoadMonitor::default_name_ =
      "OS/Processor/CPULoad";

    CPULoadMonitor::CPULoadMonitor (const char* name)
      : Monitor_Base (name)
#if defined (ACE_WIN32)
      , WindowsMonitor (ACE_TEXT("\\Processor(_Total)\\% Processor Time"))
#endif
#if defined (linux) || defined (ACE_HAS_KSTAT)
      , user_ (0)
      , wait_ (0)
      , kernel_ (0)
      , idle_ (0)
      , prev_idle_ (0)
      , prev_total_ (0.0)
#endif
#if defined (linux)
      , file_ptr_ (0)
#elif defined (ACE_HAS_KSTAT)
      , kstats_ (0)
      , kstat_ (0)
      , kstat_id_ (0)
#endif
    {
#if defined (linux)
      /// All data in this file are stored as running 'jiffy' totals, so we
      /// get values here in the constructor to subtract for the difference
      /// in subsequent calls.
      this->access_proc_stat (&this->prev_idle_);

      this->prev_total_ =
        this->user_ + this->wait_ + this->kernel_ + this->prev_idle_;
#elif defined (ACE_HAS_KSTAT)
      /// Stored similarly to Linux, in a system file.
      this->access_kstats (&this->prev_idle_);

      this->prev_total_ =
        this->user_ + this->wait_ + this->kernel_ + this->prev_idle_;
#endif
    }

    void
    CPULoadMonitor::update (void)
    {
#if defined (ACE_WIN32)
      this->win_update ();

      /// Stores value and timestamp with thread-safety.
      this->receive (this->value_.doubleValue);
#elif defined (linux)
      this->access_proc_stat (&this->idle_);
#elif defined (ACE_HAS_KSTAT)
      this->access_kstats (&this->idle_);
#endif

#if defined (linux) || defined (ACE_HAS_KSTAT)
      double delta_idle = this->idle_ - this->prev_idle_;
      double total =
        this->user_ + this->wait_ + this->kernel_ + this->idle_;
      double delta_total = total - this->prev_total_;

      if (delta_total == 0.0)
        {
          /// The system hasn't updated /proc/stat since the last call
          /// to update(), we must avoid dividing by 0.
          return;
        }

      double percent_cpu_load = 100.0 - (delta_idle / delta_total * 100.0);

      /// Stores value and timestamp with thread-safety.
      this->receive (percent_cpu_load);

      this->prev_idle_ = this->idle_;
      this->prev_total_ = total;
#endif
    }

    const char*
    CPULoadMonitor::default_name (void)
    {
      return CPULoadMonitor::default_name_;
    }

#if defined (linux)
    void
    CPULoadMonitor::access_proc_stat (unsigned long *which_idle)
    {
      this->file_ptr_ = ACE_OS::fopen (ACE_TEXT ("/proc/stat"),
                                       ACE_TEXT ("r"));

      if (this->file_ptr_ == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("CPU load - opening /proc/stat failed\n")));
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
                      &this->wait_,
                      &this->kernel_,
                      which_idle);
              break;
            }
        }

      ACE_OS::fclose (this->file_ptr_);
    }
#endif

#if defined (ACE_HAS_KSTAT)
    void
    CPULoadMonitor::access_kstats (unsigned long *which_idle)
    {
      this->kstats_ = kstat_open ();

      if (this->kstats_ == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("opening kstats file failed\n")));
          return;
        }

      this->kstat_id_ = this->kstats_->kc_chain_id;

      while (true)
        {
          this->kernel_ = 0UL;
          this->wait_ = 0UL;
          this->user_ = 0UL;
          (*which_idle) = 0UL;

          /// Unlike Linux's "/proc/stat", there is no entry for total CPU
          /// stats, so we have to sum them manually.
          for (this->kstat_ = this->kstats_->kc_chain;
               this->kstat_ != 0;
               this->kstat_ = this->kstat_->ks_next)
            {
              int result = ACE_OS::strncmp (this->kstat_->ks_name,
                                            "cpu_stat",
                                            ACE_OS::strlen ("cpu_stat"));

              if (result == 0)
                {
                  /// Because the kstat chain can change dynamically,
                  /// watch the chain ID and restart the walk if the ID
                  /// differs from what we saw during the walk. The restart
                  /// is done by breaking from the cycle with kstat_ not 0.

                  kid_t kstat_id = kstat_read (this->kstats_, this->kstat_, 0);

                  if (kstat_id != this->kstat_id_)
                    {
                      break;
                    }

                  cpu_stat_t &kstat_cpu =
                    *((cpu_stat_t *) this->kstat_->ks_data);

                  this->kernel_ += kstat_cpu.cpu_sysinfo.cpu[CPU_KERNEL];
                  this->wait_ += kstat_cpu.cpu_sysinfo.cpu[CPU_WAIT];
                  this->user_ += kstat_cpu.cpu_sysinfo.cpu[CPU_USER];
                  (*which_idle) += kstat_cpu.cpu_sysinfo.cpu[CPU_IDLE];
                }
            }

          if (this->kstat_ != 0)
            {
              /// The ID changed underneath us, so get the new one and
              /// start again.
              this->kstat_id_ = kstat_chain_update (this->kstats_);

              if (! this->kstat_id_ > 0)
                {
                  ACE_ERROR ((LM_ERROR,
                              ACE_TEXT ("kstat chain update ")
                              ACE_TEXT ("returned null id\n")));
                  return;
                }
            }
          else
            {
              /// Clean run, exit the WHILE loop.
              break;
            }
        }

      int status = kstat_close (this->kstats_);

      if (status != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("closing kstats file failed\n")));
        }
    }
#endif
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */


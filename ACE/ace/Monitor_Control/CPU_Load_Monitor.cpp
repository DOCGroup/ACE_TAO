#include "ace/Monitor_Control/CPU_Load_Monitor.h"

#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

#if defined (ACE_LINUX)
#include "ace/OS_NS_stdio.h"
#endif

#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace Monitor_Control
  {
    const char* CPU_Load_Monitor::default_name_ =
      "OS/Processor/CPULoad";

    CPU_Load_Monitor::CPU_Load_Monitor (const char* name)
      : Monitor_Base (name, Monitor_Control_Types::MC_NUMBER)
#if defined (ACE_HAS_WIN32_PDH)
      , Windows_Monitor (ACE_TEXT("\\Processor(_Total)\\% Processor Time"))
#endif
#if defined (ACE_LINUX)
      , user_ (0)
      , wait_ (0)
      , kernel_ (0)
      , idle_ (0)
      , prev_idle_ (0)
      , prev_total_ (0.0)
#endif
#if defined (ACE_LINUX)
      , file_ptr_ (0)
#endif
    {
      this->init ();
    }

    void
    CPU_Load_Monitor::update ()
    {
#if defined (ACE_HAS_WIN32_PDH)
      this->update_i ();
      this->receive (this->value_);
#elif defined (ACE_LINUX)
      this->access_proc_stat (&this->idle_);
#elif defined (ACE_HAS_KSTAT)
      this->access_kstats (&this->idle_);
#endif

#if defined (ACE_LINUX)
      double delta_idle = this->idle_ - this->prev_idle_;
      double total =
        this->user_ + this->wait_ + this->kernel_ + this->idle_;
      double delta_total = total - this->prev_total_;

      if (ACE::is_equal (delta_total, 0.0))
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
    CPU_Load_Monitor::default_name ()
    {
      return CPU_Load_Monitor::default_name_;
    }

    void
    CPU_Load_Monitor::clear_i ()
    {
#if defined (ACE_HAS_WIN32_PDH)
      this->clear_impl ();
#endif

      this->init ();
      this->Monitor_Base::clear_i ();
    }

    void
    CPU_Load_Monitor::init ()
    {
#if defined (ACE_LINUX)
      /// All data in this file are stored as running 'jiffy' totals, so we
      /// get values here in the constructor to subtract for the difference
      /// in subsequent calls.
      this->access_proc_stat (&this->prev_idle_);

      this->prev_total_ =
        this->user_ + this->wait_ + this->kernel_ + this->prev_idle_;
#endif
    }

#if defined (ACE_LINUX)
    void
    CPU_Load_Monitor::access_proc_stat (unsigned long *which_idle)
    {
      this->file_ptr_ = ACE_OS::fopen (ACE_TEXT ("/proc/stat"),
                                       ACE_TEXT ("r"));

      if (this->file_ptr_ == 0)
        {
          ACELIB_ERROR ((LM_ERROR,
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
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */

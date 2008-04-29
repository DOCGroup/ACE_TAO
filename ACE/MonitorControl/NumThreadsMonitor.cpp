// $Id$

#include "MonitorControl/NumThreadsMonitor.h"

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
    const char* NumThreadsMonitor::default_name_ =
      "OS/System/NumThreads";

    NumThreadsMonitor::NumThreadsMonitor (const char* name)
      : Monitor_Base (name)
#if defined (ACE_WIN32)
      , WindowsMonitor (ACE_TEXT ("\\System\\Threads"))
#elif defined (linux)
      , file_ptr_ (0)
      , nthreads_ (0UL)
#endif
    {
    }

    void
    NumThreadsMonitor::update (void)
    {
#if defined (ACE_WIN32)
      this->win_update ();

      /// Stores value and timestamp with thread-safety.
      this->receive (this->value_.doubleValue);
#elif defined (linux)
      this->file_ptr_ = ACE_OS::fopen ("/proc/self/status", "r");

      if (this->file_ptr_ == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("CPU load - opening /proc/self/status failed\n")));
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

          if (ACE_OS::strcmp (item, "Threads:") == 0)
            {
              sscanf (arg, "%lu", &this->nthreads_);
              break;
            }
        }

      /// Stores value and timestamp with thread-safety.
      this->receive (static_cast<double> (this->nthreads_));

      ACE_OS::fclose (this->file_ptr_);
#elif defined (ACE_HAS_KSTAT)
#endif
    }

    const char*
    NumThreadsMonitor::default_name (void)
    {
      return NumThreadsMonitor::default_name_;
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */


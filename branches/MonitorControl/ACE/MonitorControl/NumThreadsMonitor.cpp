// $Id$

#if defined (ACE_HAS_KSTAT)
#include <sys/sysinfo.h>
#endif

#include "MonitorControl/NumThreadsMonitor.h"

#if defined (ACE_ENABLE_MONITORS)

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    NumThreadsMonitor::NumThreadsMonitor (void)
      : Monitor_Base ("OS/System/NumThreads")
#if defined (ACE_WIN32)
      , WindowsMonitor ("\\System\\Threads")
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
                      "CPU load - opening /proc/self/status failed\n"));
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
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_ENABLE_MONITORS */


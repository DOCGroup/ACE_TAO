// $Id$

#if defined (ACE_HAS_KSTAT)
#include <sys/sysinfo.h>
#endif

#include "MonitorControl/MemoryUsageMonitor.h"

#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    const char* MemoryUsageMonitor::default_name_ =
      "OS/Memory/TotalUsage";

    MemoryUsageMonitor::MemoryUsageMonitor (const char* name)
      : Monitor_Base (name)
#if defined (ACE_WIN32)
      , WindowsMonitor ("\\Memory\\% Committed Bytes In Use")
#elif defined (ACE_HAS_KSTAT)
#endif
    {
    }

    void
    MemoryUsageMonitor::update (void)
    {
#if defined (ACE_WIN32)
      this->win_update ();

      /// Stores value and timestamp with thread-safety.
      this->receive (this->value_.doubleValue);
#elif defined (linux)
      if (sysinfo (&this->sysinfo_) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "Memory usage - sysinfo() failed\n"));
          return;
        }

      double used_ram = this->sysinfo_.totalram - this->sysinfo_.freeram;
      double percent_mem_usage = used_ram / this->sysinfo_.totalram * 100.0;

      this->receive (percent_mem_usage);
#elif defined (ACE_HAS_KSTAT)
      unsigned long page_size = sysconf (_SC_PAGE_SIZE);
      unsigned long total = sysconf (_SC_PHYS_PAGES) * page_size;
      unsigned long free = sysconf (_SC_AVPHYS_PAGES) * page_size;

      double used = total - free;
      double percent_mem_usage = used / total * 100.0;

      this->receive (percent_mem_usage);
#endif
    }

    const char*
    MemoryUsageMonitor::default_name (void)
    {
      return MemoryUsageMonitor::default_name_;
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */


// $Id$

#if defined (ACE_HAS_KSTAT)
#include <sys/sysinfo.h>
#endif

#include "MonitorControl/NumThreadsMonitor.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    NumThreadsMonitor<true>::NumThreadsMonitor (void)
      : MonitorPoint<true> ("NumThreads")
#if defined (linux) || defined (ACE_HAS_KSTAT)
      , user_ (0)
      , wait_ (0)
      , kernel_ (0)
      , idle_ (0)
      , prev_idle_ (0)
      , prev_total_ (0.0)
#endif
#if defined (ACE_WIN32)
      , query_ (0)
      , counter_ (0)
#elif defined (linux)
      , file_ptr_ (0)
#elif defined (ACE_HAS_KSTAT)
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
                       "\\System\\Threads",
                       0,
                       &this->counter_);

      if (ERROR_SUCCESS != this->status_)
        {
          ACE_ERROR ((LM_DEBUG, "PdhAddCounter failed\n"));
        }
#elif defined (linux)
#elif defined (ACE_HAS_KSTAT)
#endif
    }
  
    void
    NumThreadsMonitor<true>::update (void)
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
#elif defined (ACE_HAS_KSTAT)
#endif
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL


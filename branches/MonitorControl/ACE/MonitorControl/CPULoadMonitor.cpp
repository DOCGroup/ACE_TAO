// $Id$

#include "ace/OS_NS_sys_time.h"

#include "MonitorControl/CPULoadMonitor.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    CPULoadMonitor<true>::CPULoadMonitor (void)
#if defined (ACE_WIN32)
      : query_ (0),
        counter_ (0)
#endif
    {
#if defined (ACE_WIN32)
      this->status_ = PdhOpenQuery (0, 0, &this->query_);
      
      if (ERROR_SUCCESS != this->status_)
        {
          ACE_DEBUG ((LM_DEBUG, "PdhOpenQuery failed\n"));
        }
        
      this->status_ =
        PdhAddCounter (this->query_,
                       "\\Processor(_Total)\\% Processor Time",
                       0,
                       &this->counter_);
      
      if (ERROR_SUCCESS != this->status_)
        {
          ACE_DEBUG ((LM_DEBUG, "PdhAddCounter failed\n"));
        }
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
#endif
    }
    
    void
    CPULoadMonitor<true>::receive (const double data)
    {
      ACE_WRITE_GUARD (ACE_SYNCH_MUTEX, guard, this->mutex_);
      this->data_.timestamp_ = ACE_OS::gettimeofday ();
      this->data_.value_ = data;
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL


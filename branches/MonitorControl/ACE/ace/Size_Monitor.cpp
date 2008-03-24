// $Id$

#include "ace/Size_Monitor.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/Guard_T.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    Size_Monitor::Size_Monitor (void)
      : Monitor_Base ("Size_Monitor")
    {
    }
        
    Size_Monitor::Size_Monitor (const char* name)
      : Monitor_Base (name)
    {
    }
    
    Size_Monitor::~Size_Monitor (void)
    {
    }
    
    void
    Size_Monitor::update (void)
    {
      // No platform-specific or periodic code is needed, receive() can be
      // called directly whenever the size changes.
    }
    
    void
    Size_Monitor::receive (double /* value */)
    {
      // No reason to call this overload on a size monitor.
    }
    
    void
    Size_Monitor::receive (size_t value)
    {
      ACE_WRITE_GUARD (ACE_SYNCH_MUTEX, guard, this->mutex_);
      this->data_.timestamp_ = ACE_OS::gettimeofday ();
      this->data_.value_ = static_cast<double> (value);
    }
    
    void
    Size_Monitor::clear (void)
    {
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

// $Id$

#include "ace/Message_Queue_Monitor.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/Guard_T.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    Message_Queue_Monitor::Message_Queue_Monitor (void)
    {}
        
    Message_Queue_Monitor::Message_Queue_Monitor (const char* name)
      : Monitor_Base (name)
    {}
    
    Message_Queue_Monitor::~Message_Queue_Monitor (void)
    {}
    
    void
    Message_Queue_Monitor::update (void)
    {
    }
    
    void
    Message_Queue_Monitor::group_parent (const Monitor_Base* /* parent */)
    {
    }
    
    void
    Message_Queue_Monitor::add_group_child (const Monitor_Base* /* child */)
    {
    }
    
    void
    Message_Queue_Monitor::constraint (
      const ETCL_Constraint* /* constraint */)
    {
    }
    
    void
    Message_Queue_Monitor::receive (const double value)
    {
      ACE_WRITE_GUARD (ACE_SYNCH_MUTEX, guard, this->mutex_);
      this->data_.timestamp_ = ACE_OS::gettimeofday ();
      this->data_.value_ = value;
    }
    
    void
    Message_Queue_Monitor::receive (const size_t value)
    {
      ACE_WRITE_GUARD (ACE_SYNCH_MUTEX, guard, this->mutex_);
      this->data_.timestamp_ = ACE_OS::gettimeofday ();
      this->data_.value_ = static_cast<double> (value);
    }
    
    void
    Message_Queue_Monitor::clear (void)
    {
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

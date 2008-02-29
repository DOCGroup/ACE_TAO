// $Id$

#include "ace/OS_NS_sys_time.h"
#include "ace/Guard_T.h"

#include "MonitorControl/MonitorPoint.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    MonitorPoint<true>::MonitorPoint (const char* name)
      : Monitor_Base (name)
    {}
    
    void
    MonitorPoint<true>::constraint (
      const ETCL_Constraint* /* constraint */)
    {
      // TODO
      // call build_tree() on the interpreter, to get that part done
      // out of the critical path and possibly detect a bad constraint
    }
    
    void
    MonitorPoint<true>::receive (double data)
    {
      ACE_GUARD (ACE_SYNCH_MUTEX, guard, this->mutex_);
      this->data_.timestamp_ = ACE_OS::gettimeofday ();
      this->data_.value_ = data;
    }
    
    void
    MonitorPoint<true>::receive (size_t /* data */)
    {
    }
    
    void
    MonitorPoint<true>::clear (void)
    {
      // TODO
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL


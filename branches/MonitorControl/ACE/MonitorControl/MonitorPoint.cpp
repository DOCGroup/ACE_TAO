// $Id$

#include "ace/OS_NS_sys_time.h"
#include "ace/Guard_T.h"
#include "ace/Control_Action.h"

#include "MonitorControl/MonitorPoint.h"
#include "MonitorControl/Constraint_Interpreter.h"
#include "MonitorControl/Constraint_Visitor.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    MonitorPoint<true>::MonitorPoint (const char* name)
      : Monitor_Base (name)
    {}

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
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL


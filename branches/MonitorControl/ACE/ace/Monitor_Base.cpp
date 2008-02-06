// $Id$

#include "ace/Monitor_Base.h"
#include "ace/Guard_T.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    Monitor_Base::InvalidOperation::InvalidOperation (void)
    {}
    
    Monitor_Base::InvalidOperation::~InvalidOperation (void)
    {}
    
    Monitor_Base::Monitor_Base (void)
    {}
    
    Monitor_Base::Monitor_Base (const char*  name)
      : MC_Generic (name)
    {}
    
    Monitor_Base::~Monitor_Base (void)
    {}
    
    MonitorControl_Types::Data
    Monitor_Base::retrieve (void)
    {
      ACE_READ_GUARD_RETURN (ACE_SYNCH_MUTEX,
                             guard,
                             this->mutex_,
                             this->data_);
      return this->data_;
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL


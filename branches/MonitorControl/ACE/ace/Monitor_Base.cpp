// $Id$

#include "ace/Monitor_Base.h"

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
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL


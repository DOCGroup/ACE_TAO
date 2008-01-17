// $Id$

#include "MonitorControl/Datatypes.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    Generic::Generic (void)
    {}
  
    Generic::Generic (const char* name)
      : name_ (name)
    {}
    
    Generic::~Generic (void)
    {}
    
    const char*
    Generic::name (void) const
    {
      return this->name_.fast_rep ();
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL


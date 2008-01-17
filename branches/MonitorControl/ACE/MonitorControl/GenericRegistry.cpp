// $Id$

#include "MonitorControl/GenericRegistry.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    GenericRegistry::MapError::MapError (ErrorReason why)
      : why_ (why)
    {}
    
    GenericRegistry::MapError::~MapError (void)
    {}
    
    GenericRegistry::GenericRegistry (void)
    {}
    
    GenericRegistry::~GenericRegistry (void)
    {}
    
    bool
    GenericRegistry::add (Generic* /* type */)
    {
      // TODO
      return true;
    }
    
    bool
    GenericRegistry::remove (const ACE_CString& /* name */)
    {
      // TODO
      return true;
    }
    
    const Datatypes::NameList&
    GenericRegistry::names (void) const
    {
      // TODO
      return this->name_cache_;
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL


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
    
    GenericRegistry::GenericRegistry (void)
    {}
    
    GenericRegistry::~GenericRegistry (void)
    {}
    
    bool
    GenericRegistry::add (Generic* type)
    {
      ACE_WRITE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, this->mutex_, false);
      
      if (type == 0)
        {
          throw MapError (MapError::MAP_ERROR_INVALID_VALUE);
        }

      int status = this->map_.bind (type->name (), type);
      
      if (status == -1)
        {
          throw MapError (MapError::MAP_ERROR_BIND_FAILURE);
        }
      else if (status == 0)
        {
          // Invalidate the name list cache.
          this->name_cache_.clear ();
        }
        
      return (status == 0);
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


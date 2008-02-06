// $Id$

#include "ace/MC_Generic_Registry.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    MC_Generic_Registry::MapError::MapError (ErrorReason why)
      : why_ (why)
    {}
    
    MC_Generic_Registry::MC_Generic_Registry (void)
    {}
    
    MC_Generic_Registry::~MC_Generic_Registry (void)
    {}
    
    bool
    MC_Generic_Registry::add (MC_Generic* type)
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
    
    const MonitorControl_Types::NameList&
    MC_Generic_Registry::names (void) const
    {
      // TODO
      return this->name_cache_;
    }
    
    MC_Generic*
    MC_Generic_Registry::getobj (const ACE_CString& name) const
    {
      ACE_READ_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, this->mutex_, 0);
      Map::data_type type = 0;
      this->map_.find (name, type);
      return type;
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL


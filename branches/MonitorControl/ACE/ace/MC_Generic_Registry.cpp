// $Id$

#include "ace/MC_Generic_Registry.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    MC_Generic_Registry::MC_Generic_Registry (void)
    {}

    MC_Generic_Registry::~MC_Generic_Registry (void)
    {}

    bool
    MC_Generic_Registry::add (MC_Generic* type)
    {
      if (type == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "registry add: null type\n"),
                            false);
        }

      ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, this->mutex_, false);

      int const status = this->map_.bind (type->name (), type);

      if (status == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "registry add: map bind failed\n"),
                            false);
        }
        
      return (status == 0);
    }
    
    MonitorControl_Types::NameList
    MC_Generic_Registry::names (void)
    {
      ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, this->mutex_, 0);

      MonitorControl_Types::NameList name_holder_;
      
      for (Map::CONST_ITERATOR i (this->map_); !i.done (); i.advance ())
        {
          name_holder_.push_back (i->key ());
        }
        
      return name_holder_;
    }

    MC_Generic*
    MC_Generic_Registry::getobj (const ACE_CString& name) const
    {
      ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, this->mutex_, 0);
      Map::data_type type = 0;
      this->map_.find (name, type);
      return type;
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL


// $Id$

#include "ace/Monitor_Point_Registry.h"

#if defined (ACE_ENABLE_MONITORS)

#include "ace/Monitor_Base.h"
#include "ace/Singleton.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    Monitor_Point_Registry*
    Monitor_Point_Registry::instance (void)
    {
      return
        ACE_Singleton<Monitor_Point_Registry, ACE_SYNCH_MUTEX>::instance ();
    }
    
    Monitor_Point_Registry::Monitor_Point_Registry (void)
      : constraint_id_ (0)
    {
    }

    bool
    Monitor_Point_Registry::add (Monitor_Base* type)
    {
      if (type == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "registry add: null type\n"),
                            false);
        }
        
      int status = 0;

      {
        ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, this->mutex_, false);

        status = this->map_.bind (type->name (), type);
      }

      if (status == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "registry add: map bind failed\n"),
                            false);
        }
        
      return (status == 0);
    }
    
    bool
    Monitor_Point_Registry::remove (const char* name)
    {
      if (name == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "registry remove: null name\n"),
                            false);
        }
        
      int status = 0;

      {
        ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, this->mutex_, false);

        ACE_CString name_str (name, 0, false);
        status = this->map_.unbind (name_str);
      }

      if (status == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "registry add: map unbind failed\n"),
                            false);
        }
        
      return (status == 0);
    }
    
    MonitorControl_Types::NameList
    Monitor_Point_Registry::names (void)
    {
      MonitorControl_Types::NameList name_holder_;
      
      {
        ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, this->mutex_, 0);

        for (Map::CONST_ITERATOR i (this->map_); !i.done (); i.advance ())
          {
            name_holder_.push_back (i->key ());
          }
      }
        
      return name_holder_;
    }

    Monitor_Base*
    Monitor_Point_Registry::get (const ACE_CString& name) const
    {
      Map::data_type type = 0;
      
      {
        ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, this->mutex_, 0);
        
        this->map_.find (name, type);
      }
      
      return type;
    }
    
    long
    Monitor_Point_Registry::constraint_id (void)
    {
      long retval = 0;
      
      {
        ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, this->mutex_, -1);
        
        retval = this->constraint_id_++;
      }
      
      return retval;      
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_ENABLE_MONITORS */


// $Id$
#include "orbsvcs/Notify/MonitorControl/Generic_Registry.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Generic_Registry::~TAO_Generic_Registry (void)
{
  ACE_WRITE_GUARD (ACE_SYNCH_RW_MUTEX, guard, this->mutex_);
  Map::iterator itr (this->map_);
  Map::value_type* entry = 0;
  while (itr.next (entry))
    {
      delete entry->item ();
      itr.advance ();
    }
}

bool
TAO_Generic_Registry::add(TAO_NS_Generic* type)
{
  ACE_WRITE_GUARD_RETURN (ACE_SYNCH_RW_MUTEX, guard, this->mutex_, false);
  if (type == 0)
    throw Map_Error (Map_Error::MAP_ERROR_INVALID_VALUE);

  int status = this->map_.bind(type->name(), type);
  if (status == -1)
    {
      throw Map_Error (Map_Error::MAP_ERROR_BIND_FAILURE);
    }
  else if (status == 0)
    {
      // Invalidate the name list cache
      this->name_cache_.length (0);
    }
  return (status == 0);
}

bool
TAO_Generic_Registry::remove(const ACE_CString& name)
{
  ACE_WRITE_GUARD_RETURN (ACE_SYNCH_RW_MUTEX, guard, this->mutex_, false);
  Map::data_type type = 0;
  int status = this->map_.unbind(name, type);
  if (status != 0)
    return false;

  // Invalidate the name list cache
  this->name_cache_.length (0);
  delete type;
  return true;
}

const TAO_Generic_Registry::NameList&
TAO_Generic_Registry::names (void)
{
  if (this->name_cache_.length () == 0)
    {
      ACE_WRITE_GUARD_RETURN (ACE_SYNCH_RW_MUTEX, guard,
                              this->mutex_, this->name_cache_);

      if (this->name_cache_.length () == 0)
        {
          CORBA::ULong length = 0;
          Map::iterator itr (this->map_);
          Map::value_type* entry = 0;
          while (itr.next (entry))
            {
              this->name_cache_.length (length + 1);
              this->name_cache_[length++] =
                        CORBA::string_dup (entry->key ().c_str ());
              itr.advance ();
            }
        }
    }

  return this->name_cache_;
}

TAO_NS_Generic*
TAO_Generic_Registry::getobj(const ACE_CString& name) const
{
  ACE_READ_GUARD_RETURN (ACE_SYNCH_RW_MUTEX, guard, this->mutex_, 0);
  Map::data_type type = 0;
  this->map_.find(name, type);
  return type;
}

TAO_END_VERSIONED_NAMESPACE_DECL


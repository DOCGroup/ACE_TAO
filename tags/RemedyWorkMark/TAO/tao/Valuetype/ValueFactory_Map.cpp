// -*- C++ -*-
// $Id$

#include "tao/Valuetype/ValueFactory_Map.h"
#include "tao/Valuetype/ValueFactory.h"
#include "tao/CORBA_String.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_ValueFactory_Map::TAO_ValueFactory_Map (void)
  : map_ (TAO_DEFAULT_VALUE_FACTORY_TABLE_SIZE)
{
}

TAO_ValueFactory_Map::~TAO_ValueFactory_Map (void)
{
  // Initialize an iterator.  We need to go thru each entry and free
  // up storage allocated to hold the external ids and invoke
  // _remove_ref () on the internal ids.
  FACTORY_MAP_MANAGER::ITERATOR iterator (this->map_);

  for (FACTORY_MAP_MANAGER::ENTRY *entry = 0;
       iterator.next (entry) != 0;
       iterator.advance ())
    {
      // We had allocated memory and stored the string. So we free the
      // memory.
      CORBA::string_free ((char *) entry->ext_id_);
      entry->ext_id_ = 0;
      entry->int_id_->_remove_ref ();
      entry->int_id_ = 0;
    }
}

int
TAO_ValueFactory_Map::rebind (const char *repo_id,
                              CORBA::ValueFactory &factory)
{
  ACE_GUARD_RETURN(TAO_SYNCH_MUTEX, guard, this->mutex_, -1);

  const char *prev_repo_id = 0;
  CORBA::ValueFactory prev_factory = 0;
  int const ret = this->map_.rebind (CORBA::string_dup (repo_id),
                                     factory,
                                     prev_repo_id,
                                     prev_factory);

  if (ret > -1)   // ok, no error
    {
      factory->_add_ref ();    // The map owns one reference.

      if (ret == 1)    // there was a previous factory
        {
          factory = prev_factory;
          CORBA::string_free (const_cast<char*> (prev_repo_id));
        }
    }

  return ret;
}

int
TAO_ValueFactory_Map::unbind (const char *repo_id,
                              CORBA::ValueFactory &factory)
{
  ACE_GUARD_RETURN(TAO_SYNCH_MUTEX, guard, this->mutex_, -1);

  FACTORY_MAP_MANAGER::ENTRY *prev_entry = 0;
  int ret = this->map_.find (repo_id, prev_entry);

  if (ret == 0)    // there was a matching factory
    {
      // set factory to the previous factory,
      factory = prev_entry->int_id_;
      char *temp = const_cast<char *> (prev_entry->ext_id_);
      ret = this->map_.unbind (prev_entry);

      if (ret == 0)
        {
          CORBA::string_free (temp);
        }
    }

  return ret;
}

// %! perhaps inline
int
TAO_ValueFactory_Map::find (const char *repo_id,
                            CORBA::ValueFactory &factory)
{
  ACE_GUARD_RETURN(TAO_SYNCH_MUTEX, guard, this->mutex_, -1);

  int const ret = this->map_.find (repo_id, factory);
  if (ret > -1)
    {
      factory->_add_ref ();    // The caller gets one reference as gift.
    }

  return ret;
}

TAO_END_VERSIONED_NAMESPACE_DECL


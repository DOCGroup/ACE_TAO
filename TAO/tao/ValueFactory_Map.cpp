// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     ValueFactory_Map.cpp
//
// = AUTHOR
//     Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
//
// ============================================================================

#include "tao/ValueFactory_Map.h"
#include "tao/ValueFactory.h"
#include "tao/ORB.h"

#ifdef TAO_HAS_VALUETYPE

#if !defined (__ACE_INLINE__)
# include "tao/ValueFactory_Map.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, ValueFactory_Map, "$Id$")


TAO_ValueFactory_Map::TAO_ValueFactory_Map ()
  : map_ ()   // use default size and allocator
{
}

TAO_ValueFactory_Map::~TAO_ValueFactory_Map ()
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

// %! Thread issues

int
TAO_ValueFactory_Map::rebind (const char *repo_id,
                              CORBA_ValueFactory_ptr &factory)
{
//  ACE_READ_GUARD_RETURN (ACE_SYNCH_RW_MUTEX, guard, map_->mutex(),-1);
//   --- but must be recursive
  const char *prev_repo_id;
  CORBA_ValueFactory_ptr prev_factory;
  int ret = this->map_.rebind (CORBA::string_dup (repo_id),
                                 factory,
                                 prev_repo_id, prev_factory);
  if (ret > -1)   // ok, no error
    {
      factory->_add_ref ();    // The map owns one reference.
      if (ret == 1)    // there was a previous factory
        {
          factory = prev_factory;
          CORBA::string_free (ACE_const_cast(char*,prev_repo_id));
        }
    }
  return ret;
}

int
TAO_ValueFactory_Map::unbind (const char *repo_id,
                              CORBA_ValueFactory_ptr &factory)
{
  // ACE_Hash_Map_Entry<const char *, CORBA_ValueFactory_ptr> *prev_entry;
  FACTORY_MAP_MANAGER::ENTRY *prev_entry;
  int ret = this->map_.find (repo_id, prev_entry);
  if (ret == 0)    // there was a matching factory
    {
      // set factory to the previous factory,
      factory = prev_entry->int_id_;
      char *temp = ACE_const_cast (char *, prev_entry->ext_id_);
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
                            CORBA_ValueFactory_ptr &factory)
{
  int ret = this->map_.find (repo_id, factory);
  // %! this must be guarded to be atomic  !!!!!!!!!!!!!!!!!!
  if (ret > -1)
    {
      factory->_add_ref ();    // The caller gets one reference as gift.
    }
  return ret;
}




#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Hash_Map_Iterator_Base_Ex<const char *, CORBA_ValueFactory_ptr, ACE_Hash<const char *>, ACE_Equal_To<const char *>, ACE_SYNCH_RW_MUTEX>;
template class ACE_Hash_Map_Manager_Ex<const char *, CORBA_ValueFactory_ptr, ACE_Hash<const char *>, ACE_Equal_To<const char *>, ACE_SYNCH_RW_MUTEX>;
template class ACE_Hash_Map_Entry<const char *, CORBA_ValueFactory_ptr>;
template class ACE_Singleton<TAO_ValueFactory_Map, ACE_SYNCH_MUTEX>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<const char *, CORBA_ValueFactory_ptr, ACE_Hash<const char *>, ACE_Equal_To<const char *>, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Hash_Map_Manager_Ex<const char *, CORBA_ValueFactory_ptr, ACE_Hash<const char *>, ACE_Equal_To<const char *>, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Hash_Map_Entry<const char *, CORBA_ValueFactory_ptr>
#pragma instantiate ACE_Singleton<TAO_ValueFactory_Map, ACE_SYNCH_MUTEX>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


#endif /* TAO_HAS_VALUETYPE */

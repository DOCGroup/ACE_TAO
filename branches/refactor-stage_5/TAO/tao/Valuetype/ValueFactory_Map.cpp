#include "ValueFactory_Map.h"
#include "ValueFactory.h"
#include "tao/CORBA_String.h"

#if !defined (__ACE_INLINE__)
# include "ValueFactory_Map.inl"
#endif /* ! __ACE_INLINE__ */


ACE_RCSID (tao,
           ValueFactory_Map,
           "$Id$")


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

// %! Thread issues

int
TAO_ValueFactory_Map::rebind (const char *repo_id,
                              CORBA::ValueFactory &factory)
{
//  ACE_READ_GUARD_RETURN (TAO_SYNCH_RW_MUTEX, guard, map_->mutex(),-1);
//   --- but must be recursive
  const char *prev_repo_id;
  CORBA::ValueFactory prev_factory;
  int ret = 0;
  ret = this->map_.rebind (CORBA::string_dup (repo_id),
                           factory,
                           prev_repo_id,
                           prev_factory);

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
                              CORBA::ValueFactory &factory)
{
  FACTORY_MAP_MANAGER::ENTRY *prev_entry;
  int ret = 0;
  ret = this->map_.find (repo_id,
                         prev_entry);
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
                            CORBA::ValueFactory &factory)
{
  int ret = 0;
  ret = this->map_.find (repo_id,
                         factory);
  // %! this must be guarded to be atomic  !!!!!!!!!!!!!!!!!!
  if (ret > -1)
    {
      factory->_add_ref ();    // The caller gets one reference as gift.
    }

  return ret;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Hash_Map_Iterator_Base_Ex<const char *, CORBA::ValueFactoryBase *, ACE_Hash<const char *>, ACE_Equal_To<const char *>, TAO_SYNCH_RW_MUTEX>;
template class ACE_Hash_Map_Iterator_Ex<const char *, CORBA::ValueFactoryBase *, ACE_Hash<const char *>, ACE_Equal_To<const char *>, TAO_SYNCH_RW_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<const char *, CORBA::ValueFactoryBase *, ACE_Hash<const char *>, ACE_Equal_To<const char *>, TAO_SYNCH_RW_MUTEX>;
template class ACE_Hash_Map_Manager_Ex<const char *, CORBA::ValueFactoryBase *, ACE_Hash<const char *>, ACE_Equal_To<const char *>, TAO_SYNCH_RW_MUTEX>;
template class ACE_Hash_Map_Entry<const char *, CORBA::ValueFactoryBase *>;
template class TAO_Singleton<TAO_ValueFactory_Map, TAO_SYNCH_MUTEX>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<const char *, CORBA::ValueFactoryBase *, ACE_Hash<const char *>, ACE_Equal_To<const char *>, TAO_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<const char *, CORBA::ValueFactoryBase *, ACE_Hash<const char *>, ACE_Equal_To<const char *>, TAO_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<const char *, CORBA::ValueFactoryBase *, ACE_Hash<const char *>, ACE_Equal_To<const char *>, TAO_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Hash_Map_Manager_Ex<const char *, CORBA::ValueFactoryBase *, ACE_Hash<const char *>, ACE_Equal_To<const char *>, TAO_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Hash_Map_Entry<const char *, CORBA::ValueFactoryBase *>
#pragma instantiate TAO_Singleton<TAO_ValueFactory_Map, TAO_SYNCH_MUTEX>

#elif defined (__GNUC__) && (defined (_AIX) || defined (__hpux) || defined (VXWORKS))
template TAO_Singleton<TAO_ValueFactory_Map, TAO_SYNCH_MUTEX> * TAO_Singleton<TAO_ValueFactory_Map, TAO_SYNCH_MUTEX>::singleton_;

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

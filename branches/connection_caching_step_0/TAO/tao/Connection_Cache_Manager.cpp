#include "tao/Connection_Cache_Manager.h"



#if !defined (__ACE_INLINE__)
# include "tao/Connection_Cache_Manager.i"
#endif /* __ACE_INLINE__ */


ACE_RCSID(tao, Connection_Cache_Hash_Manager, "$Id$")


TAO_Connection_Cache_Manager::~TAO_Connection_Cache_Hash_Manager (void)
{
  // Close the HASH MAP and release resources
  HASH_MAP.close ();
}


int
TAO_Connection_Cache_Manager::bind (const TAO_Cache_ExtId &ext_id,
                                         const TAO_Cache_IntId &int_id)
{
  return HASH_MAP.bind (ext_id,
                        int_id);
}


int
TAO_Connection_Cache_Manager::find (const TAO_Cache_ExtId &key,
                                         TAO_Cache_IntId &value)
{
  return HASH_MAP.find (key,
                        value);
}

int
TAO_Connection_Cache_Manager::rebind (const TAO_Cache_ExtId &key,
                                           const TAO_Cache_IntId &value)
{
  return HASH_MAP.rebind (key,
                          value);
}

int
TAO_Connection_Cache_Manager::trybind (const TAO_Cache_ExtId &key,
                                            TAO_Cache_IntId &value)
{
  return HASH_MAP.trybind (key, value);
}

int
TAO_Connection_Cache_Manager::unbind (const TAO_Cache_ExtId &key)
{
  return HASH_MAP.unbind (key);
}

int
TAO_Connection_Cache_Manager::unbind (const TAO_Cache_ExtId &key,
                                           TAO_Cache_IntId &value)
{
  return HASH_MAP.unbind (key,
                          value);
}


int
TAO_Connection_Cache_Manager::purge (void)
{
}

size_t
TAO_Connection_Cache_Manager::current_size (void) const
{
  return HASH_MAP.current_size ();
}

size_t
TAO_Connection_Cache_Manager::total_size (void) const
{
  return HASH_MAP.total_size ();
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

  // Instantiations for the Hash Map
template class ACE_Equal_To<TAO_Cache_ExtId>;
template class ACE_Hash<TAO_Cache_ExtId>;
template class ACE_Hash_Map_Entry<TAO_Cache_ExtId, TAO_Cache_IntId>;
template class ACE_Hash_Map_Manager<TAO_Cache_ExtId, TAO_Cache_IntId, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<TAO_Cache_ExtId, TAO_Cache_IntId, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<TAO_Cache_ExtId, TAO_Cache_IntId, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<TAO_Cache_ExtId, TAO_Cache_IntId, ACE_Hash<TAO_Cache_ExtId>, ACE_Equal_To<TAO_Cache_ExtId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<TAO_Cache_ExtId, TAO_Cache_IntId, ACE_Hash<TAO_Cache_ExtId>, ACE_Equal_To<TAO_Cache_ExtId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<TAO_Cache_ExtId, TAO_Cache_IntId, ACE_Hash<TAO_Cache_ExtId>, ACE_Equal_To<TAO_Cache_ExtId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<TAO_Cache_ExtId, TAO_Cache_IntId, ACE_Hash<TAO_Cache_ExtId>, ACE_Equal_To<TAO_Cache_ExtId>, ACE_Null_Mutex>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

  // Instantiations for the Hash Map
#pragma instantiate ACE_Equal_To<TAO_Cache_ExtId>
#pragma instantiate ACE_Hash<TAO_Cache_ExtId>
#pragma instantiate ACE_Hash_Map_Entry<TAO_Cache_ExtId, TAO_Cache_IntId>
#pragma instantiate ACE_Hash_Map_Manager<TAO_Cache_ExtId, TAO_Cache_IntId, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<TAO_Cache_ExtId, TAO_Cache_IntId, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<TAO_Cache_ExtId, TAO_Cache_IntId, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<TAO_Cache_ExtId, TAO_Cache_IntId, ACE_Hash<TAO_Cache_ExtId>, ACE_Equal_To<TAO_Cache_ExtId>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<TAO_Cache_ExtId, TAO_Cache_IntId, ACE_Hash<TAO_Cache_ExtId>, ACE_Equal_To<TAO_Cache_ExtId>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<TAO_Cache_ExtId, TAO_Cache_IntId, ACE_Hash<TAO_Cache_ExtId>, ACE_Equal_To<TAO_Cache_ExtId>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<TAO_Cache_ExtId, TAO_Cache_IntId, ACE_Hash<TAO_Cache_ExtId>, ACE_Equal_To<TAO_Cache_ExtId>, ACE_Null_Mutex>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

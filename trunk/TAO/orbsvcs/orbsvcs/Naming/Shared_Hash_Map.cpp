// $Id$

#ifndef ACE_SHARED_HASH_MAP_T_C
#define ACE_SHARED_HASH_MAP_T_C

#include "Shared_Hash_Map.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

template <class EXT_ID, class INT_ID>
ACE_Shared_Hash_Map<EXT_ID, INT_ID>::ACE_Shared_Hash_Map (ACE_Allocator *alloc)
  : ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_Null_Mutex> (alloc)
{
}

template <class EXT_ID, class INT_ID>
ACE_Shared_Hash_Map<EXT_ID, INT_ID>::ACE_Shared_Hash_Map (size_t size,
                                                          ACE_Allocator *alloc)
  : ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_Null_Mutex> (size, alloc)
{
}

template <class EXT_ID, class INT_ID> int
ACE_Shared_Hash_Map<EXT_ID, INT_ID>::close (ACE_Allocator *alloc)
{
  this->allocator_ = alloc;
  return this->close_i ();
}

template <class EXT_ID, class INT_ID> int
ACE_Shared_Hash_Map<EXT_ID, INT_ID>::bind (const EXT_ID &ext_id,
                                           const INT_ID &int_id,
                                           ACE_Allocator *alloc)
{
  this->allocator_ = alloc;
  return this->bind_i (ext_id, int_id);
}

template <class EXT_ID, class INT_ID> int
ACE_Shared_Hash_Map<EXT_ID, INT_ID>::unbind (const EXT_ID &ext_id,
                                             INT_ID &int_id,
                                             ACE_Allocator *alloc)
{
  this->allocator_ = alloc;
  return this->unbind_i (ext_id, int_id);
}

template <class EXT_ID, class INT_ID> int
ACE_Shared_Hash_Map<EXT_ID, INT_ID>::rebind (const EXT_ID &ext_id,
                                             const INT_ID &int_id,
                                             EXT_ID &old_ext_id,
                                             INT_ID &old_int_id,
                                             ACE_Allocator *alloc)
{
  this->allocator_ = alloc;
  return this->rebind_i (ext_id, int_id, old_ext_id, old_int_id);
}

template <class EXT_ID, class INT_ID> int
ACE_Shared_Hash_Map<EXT_ID, INT_ID>::find (const EXT_ID &ext_id,
                                           INT_ID &int_id,
                                           ACE_Allocator *alloc)
{
  this->allocator_ = alloc;
  return this->find_i (ext_id, int_id);
}

#endif /* ACE_SHARED_HASH_MAP_T_C */

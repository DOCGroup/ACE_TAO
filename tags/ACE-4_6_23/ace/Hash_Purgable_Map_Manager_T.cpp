// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Hash_Purgable_Map_Manager_T.cpp
//
// = AUTHOR
//    Kirthika Parameswaran  <kirthika@cs.wustl.edu>
//
// ============================================================================

#ifndef ACE_HASH_PURGABLE_MAP_MANAGER_T_CPP
#define ACE_HASH_PURGABLE_MAP_MANAGER_T_CPP

#include "ace/Hash_Purgable_Map_Manager_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if !defined (__ACE_INLINE__)
# include "ace/Hash_Purgable_Map_Manager_T.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Hash_Purgable_Map_Manager_T, "$Id$")

// Initialisation.
template <class EXT_ID, class INT_ID> 
ACE_Hash_Purgable_Map_Entry<EXT_ID, INT_ID>::ACE_Hash_Purgable_Map_Entry (ACE_Hash_Map_Entry<EXT_ID, INT_ID> *next,
                                                                          ACE_Hash_Map_Entry<EXT_ID, INT_ID> *prev,
                                                                          int purge_tag)
  : ACE_Hash_Map_Entry<EXT_ID, INT_ID> (next, 
                                        prev),
    purge_tag_ (purge_tag)
{
}

template <class EXT_ID, class INT_ID>
ACE_Hash_Purgable_Map_Entry<EXT_ID, INT_ID>::ACE_Hash_Purgable_Map_Entry (const EXT_ID &ext_id,
                                                                          const INT_ID &int_id,
                                                                          ACE_Hash_Map_Entry<EXT_ID, INT_ID> *next,
                                                                          ACE_Hash_Map_Entry<EXT_ID, INT_ID> *prev,
                                                                          int purge_tag)
  : ACE_Hash_Map_Entry<EXT_ID, INT_ID> (ext_id,
                                        int_id,
                                        next,
                                        prev),
    purge_tag_ (purge_tag)
{
}
#if ! defined (ACE_HAS_BROKEN_NOOP_DTORS)
template <class EXT_ID, class INT_ID> 
ACE_Hash_Purgable_Map_Entry<EXT_ID, INT_ID>::~ACE_Hash_Purgable_Map_Entry (void)
{
}
#endif /* ! defined (ACE_HAS_BROKEN_NOOP_DTORS) */

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> ACE_Hash_Map_Entry<EXT_ID, INT_ID>*
ACE_Hash_Purgable_Map_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>:: create_entry (ACE_Hash_Map_Entry<EXT_ID, INT_ID> *next,
                                                                                                   ACE_Hash_Map_Entry<EXT_ID, INT_ID> *prev)
{
  ACE_Hash_Purgable_Map_Entry<EXT_ID, INT_ID> *new_entry;
  void *ptr;
  
  // Memory is  allocated for the entry.
  ACE_ALLOCATOR_RETURN (ptr,
                        this->allocator_->malloc (sizeof (ACE_Hash_Purgable_Map_Entry<EXT_ID, INT_ID>)),
                        0);
  
  // A entry is created with its purge_tag value set to the current
  // timer value.
  new_entry = new (ptr) ACE_Hash_Purgable_Map_Entry<EXT_ID, INT_ID> (next,
                                                                     prev,
                                                                     this->timer_);
                                                           
  // The timer ticks on...;)
  ++this->timer_;
  
  return new_entry;
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> ACE_Hash_Map_Entry<EXT_ID, INT_ID>*
ACE_Hash_Purgable_Map_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>:: create_entry (const EXT_ID &ext_id,
                                                                                                   const INT_ID &int_id,
                                                                                                   ACE_Hash_Map_Entry<EXT_ID, INT_ID> *next,
                                                                                                   ACE_Hash_Map_Entry<EXT_ID, INT_ID> *prev)
{
  ACE_Hash_Purgable_Map_Entry<EXT_ID, INT_ID> *new_entry;
  void *ptr;

  // Memory is  allocated for the entry.
  ACE_ALLOCATOR_RETURN (ptr,
                        this->allocator_->malloc (sizeof (ACE_Hash_Purgable_Map_Entry<EXT_ID, INT_ID>)),
                        0);

  // A entry is created with its purge_tag value set to the current
  // timer value.
  new_entry = new (ptr) ACE_Hash_Purgable_Map_Entry<EXT_ID, INT_ID> (ext_id,
                                                                     int_id,
                                                                     next,
                                                                     prev,
                                                                     this->timer_);
  // The timer moves on.
  ++this->timer_;
  
  return new_entry;
}

// This method finds the appropriate entry and also sets its purge_tag
// to the current timer value. 
template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> int
ACE_Hash_Purgable_Map_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::shared_find (const EXT_ID &ext_id,
                                                                                                 ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&entry,
                                                                                                 u_long &loc)
{
  // The entry is filled in depending upon the ext_id searched in the map.
  int result = ACE_Hash_Map_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::shared_find (ext_id,
                                                                                                       entry,
                                                                                                       loc);
  if (result == -1)
    return -1;
 
  // On a successful find ..
  if (result == 0)
    {
      PURGABLE_ENTRY *purgable_entry = ACE_dynamic_cast (PURGABLE_ENTRY *&,
                                                         entry);
      
       // Set the purge_tag to the current timer value showing the
       // recent use of the entry. The timer is then incremented.
       purgable_entry->set_purge_tag (this->timer_);
       ++this->timer_;
     }

  return result;
}

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK> int 
ACE_Hash_Purgable_Map_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>::purge (void)
{
  // The minimum value has to be large enough to be able to fall
  // thru the loop to find lesser valued entries.
  unsigned int min= ULONG_MAX;
  
  ENTRY *entry = 0;
  PURGABLE_ENTRY *purgable_entry = 0;
  PURGABLE_ENTRY *purgable_entry_to_remove = 0;
  
  // The iterator moves thru the map searching for the entry with the
  // lowest purge_tag. Such an entry is stored and later removing by
  // unbinding it from the map.
  for (ITERATOR iter (*this);
       iter.next (entry) !=0;
       iter.advance ())
    {
      purgable_entry = ACE_dynamic_cast (PURGABLE_ENTRY *,
                                         entry);

      // Ah! an entry with a lower purge_tag...
      if (min > purgable_entry->get_purge_tag ())
        {
          min = purgable_entry->get_purge_tag ();
          purgable_entry_to_remove = purgable_entry;
        }
    }
  
  // The entry is removed from the map only if it is non-NULL.
  if (purgable_entry_to_remove != 0) 
    {
      return this->unbind_i (purgable_entry_to_remove);
    }
  else
    {
      return 0;
    }
}

#endif /* ACE_HASH_PURGABLE_MAP_MANAGER_T_CPP */

/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Hash_Purgable_Map_Manager_T.h
//
// = AUTHOR
//    Kirthika Parameswaran  <kirthika@cs.wustl.edu>
//
// ============================================================================

#ifndef ACE_HASH_PURGABLE_MAP_MANAGER_T_H
#define ACE_HASH_PURGABLE_MAP_MANAGER_T_H

#include "ace/Hash_Map_Manager_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

template <class EXT_ID, class INT_ID>
class ACE_Hash_Purgable_Map_Entry : public ACE_Hash_Map_Entry<EXT_ID, INT_ID>
{
  // = TITLE
  //     Define an purgable entry in the hash table. A purge_tag field
  //     helps in keeping track and deciding whether it has been used
  //     recently.
public:

  // = Initialization and termination methods.
  ACE_Hash_Purgable_Map_Entry (const EXT_ID &ext_id,
                               const INT_ID &int_id,
                               ACE_Hash_Map_Entry<EXT_ID, INT_ID> *next = 0,
                               ACE_Hash_Map_Entry<EXT_ID, INT_ID> *prev = 0,
                               int purge_tag = 0);
  // Constructor.
  
  ACE_Hash_Purgable_Map_Entry (ACE_Hash_Map_Entry<EXT_ID, INT_ID> *next,
                               ACE_Hash_Map_Entry<EXT_ID, INT_ID> *prev,
                               int purge_tag = 0);
  // Constructor.

#if ! defined (ACE_HAS_BROKEN_NOOP_DTORS)
  ~ACE_Hash_Purgable_Map_Entry (void);
  // Destructor.
#endif /* ! defined (ACE_HAS_BROKEN_NOOP_DTORS) */ 

  void set_purge_tag (u_long value);
  // Used to change the purge_tag value.

  u_long get_purge_tag (void);
  // Obtain the purge_tag value.
protected:
  u_long purge_tag_;
  // This is the purge_value which will decide the entry to be purged.  
};

template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK>
class ACE_Hash_Purgable_Map_Manager_Ex : public ACE_Hash_Map_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>
{
  // = TITLE
  //     Define a purgable map abstraction that efficiently associates
  //     <EXT_ID>s with <INT_ID>s.
  //
  // = DESCRIPTION
  //
  //     This implementation of a map uses a purgable hash table.  Key
  //     hashing is achieved through the HASH_KEY object and key
  //     comparison is achieved through the COMPARE_KEYS object.
  //
  //     This class provides the feature of purging the entries on
  //     need based on the purging algorithm followed. By default the
  //     Least Recently Used algorithm is applied for purging "k"
  //     entires from the map.
  //     
  //     The Least Recently used algorithm is used to decide on the
  //     entry to purge. For this a timesatmp is used per entry with a
  //     virtual timer providing the timestamp-value. On a lookup or
  //     use of an entry, its timestamp value is updated proving that
  //     the entry has been used recently. When the map gets full and
  //     some entries need to be purged, the "K" entries with minimum
  //     timestamp are removed.
public:
  
  typedef ACE_Hash_Purgable_Map_Entry<EXT_ID, INT_ID> PURGABLE_ENTRY;
  // The purgable_entry has an extra purge_tag field which will act as
  // the timestamp in deciding the entry to be purged from the map.

  // = Initialization and termination methods.

  ACE_Hash_Purgable_Map_Manager_Ex (ACE_Allocator *alloc = 0);
  // Initialize a <Hash_Purgable_Map_Manager_Ex> with default size.

  ACE_Hash_Purgable_Map_Manager_Ex (size_t size,
                                    ACE_Allocator *alloc = 0);
  // Initialize a <Hash_Purgable_Map_Manager_Ex> with size <length>.
  
  ~ACE_Hash_Purgable_Map_Manager_Ex (void);
  // Destruct a <Hash_Purgable_Map_Manager_Ex> with size <length>.
  
  int purge (int num);
  // The <num> of entries specified are removed form the map based on
  // the purging algorithm used. By default Least Recently Used
  // algorithm is applied. Returns 0 on success and -1 on
  // failure. This method is called with locks held.
  
protected:
  int purge_i (int num);
  // The <num> of entries specified are removed form the map based on the
  // purging algorithm used. By default Least Recently Used algorithm is
  // applied. Returns 0 on success and -1 on failure. This method is
  // called w.o. locks being held.

  int shared_find (const EXT_ID &ext_id,
                   ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&entry,
                   u_long &loc);
  // Returns the <ACE_Hash_Map_Entry> that corresponds to
  // <ext_id>. This method interface has been inherited form the base
  // class,i.e. Hash_Map_Manager_Ex.

  ACE_Hash_Map_Entry<EXT_ID, INT_ID> *create_entry (ACE_Hash_Map_Entry<EXT_ID, INT_ID> *next = 0,
                                                    ACE_Hash_Map_Entry<EXT_ID, INT_ID> *prev = 0);
  // Creates an entry. This method interface has been inherited form
  // the base class, i.e. Hash_Map_Manager_Ex.

  ACE_Hash_Map_Entry<EXT_ID, INT_ID> *create_entry (const EXT_ID &ext_id,
                                                    const INT_ID &int_id,
                                                    ACE_Hash_Map_Entry<EXT_ID, INT_ID> *next = 0,
                                                    ACE_Hash_Map_Entry<EXT_ID, INT_ID> *prev = 0);
  // Creates an entry.This method interface has been inherited form
  // the base class, i.e. Hash_Map_Manager_Ex.

  int purge (void);
  // Removes the minimum element from the map. Selection Sort is used
  // to find and remove the entry. This method purges a single entry.
 
  u_long timer_;
  // The virtual clock which provides the purge_value for the entries.
};
#if defined (__ACE_INLINE__)
#include "ace/Hash_Purgable_Map_Manager_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Hash_Purgable_Map_Manager_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation "ace/Hash_Purgable_Map_Manager_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_HASH_PURGABLE_MAP_MANAGER_T_H */




  


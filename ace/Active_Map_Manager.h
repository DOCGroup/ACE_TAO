/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Active_Map_Manager.h
//
// = AUTHOR
//    Irfan Pyarali
//
// ============================================================================

#ifndef ACE_ACTIVE_MAP_MANAGER_H
#define ACE_ACTIVE_MAP_MANAGER_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Export ACE_Active_Map_Manager_Key
{
  // = TITLE
  //     Key used in the Active Object Map.
  //
  // = DESCRIPTION
  //     This key keeps information of the index and the generation
  //     count of the slot it represents.  Since the index information
  //     is part of the key, lookups are super fast and predictable,
public:
  ACE_Active_Map_Manager_Key (void);
  // Default constructor.

  ACE_Active_Map_Manager_Key (ACE_UINT32 slot_index,
                              ACE_UINT32 slot_generation);
  // Constructor given the <slot_index> and <slot_generation> number.
  // This is useful once the user has somehow recovered the
  // <slot_index> and <slot_generation> number from the client.

  ACE_UINT32 slot_index (void) const;
  void slot_index (ACE_UINT32 i);
  // Get/Set the <slot_index>.

  ACE_UINT32 slot_generation (void) const;
  void slot_generation (ACE_UINT32 g);
  // Get/Set the <slot_generation> number.

  static size_t size (void);
  // Size required to store information about active key.

  void decode (const void *data);
  // Recover state of active key from <data>.  User must make sure
  // that <data> encoded using the <encode> method.

  void encode (void *data) const;
  // Encode state of the active key into <data>.  <data> must be as
  // big as the value returned from <size>.

  int operator== (const ACE_Active_Map_Manager_Key &rhs) const;
  int operator!= (const ACE_Active_Map_Manager_Key &rhs) const;
  // Compare keys.

  // = This really should be protected but because of template
  // friends, they are not.

  void increment_slot_generation_count (void);
  // Increment the <slot_generation> number.

private:
  ACE_UINT32 slot_index_;
  // Slot index in the active map.

  ACE_UINT32 slot_generation_;
  // Slot generation number of <slot_index_> slot in the active map.
};

#if defined (__ACE_INLINE__)
#include "ace/Active_Map_Manager.i"
#endif /* __ACE_INLINE__ */

// Include the templates here.
#include "ace/Active_Map_Manager_T.h"

#endif /* ACE_ACTIVE_MAP_MANAGER_H */

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

  ACE_Active_Map_Manager_Key (u_long index,
                              u_long generation);
  // Constructor given the index and generation number.  This is
  // useful once the user has somehow recovered the index and
  // generation number from the client.

  u_long index (void) const;
  void index (u_long i);
  // Get/Set the index.
  
  u_long generation (void) const;
  void generation (u_long g);
  // Get/Set the generation number.
  
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

  void increment_generation_count (void);
  // Increment the generation number.

private:
  u_long index_;
  // Index in the active map.

  u_long generation_;
  // Generation number of <index_> slot in the active map.
};

#if defined (__ACE_INLINE__)
#include "ace/Active_Map_Manager.i"
#endif /* __ACE_INLINE__ */

// Include the templates here.
#include "ace/Active_Map_Manager_T.h"

#endif /* ACE_ACTIVE_MAP_MANAGER_H */

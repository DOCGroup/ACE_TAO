/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Active_Map_Manager.h
 *
 *  $Id$
 *
 *  @author Irfan Pyarali
 */
//=============================================================================


#ifndef ACE_ACTIVE_MAP_MANAGER_H
#define ACE_ACTIVE_MAP_MANAGER_H
#include "ace/pre.h"

#include "ace/OS/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class ACE_Active_Map_Manager_Key
 *
 * @brief Key used in the Active Object Map.
 *
 * This key keeps information of the index and the generation
 * count of the slot it represents.  Since the index information
 * is part of the key, lookups are super fast and predictable,
 */
class ACE_Export ACE_Active_Map_Manager_Key
{
public:
  /// Default constructor.
  ACE_Active_Map_Manager_Key (void);

  /**
   * Constructor given the <slot_index> and <slot_generation> number.
   * This is useful once the user has somehow recovered the
   * <slot_index> and <slot_generation> number from the client.
   */
  ACE_Active_Map_Manager_Key (ACE_UINT32 slot_index,
                              ACE_UINT32 slot_generation);

  /// Get/Set the <slot_index>.
  ACE_UINT32 slot_index (void) const;
  void slot_index (ACE_UINT32 i);

  /// Get/Set the <slot_generation> number.
  ACE_UINT32 slot_generation (void) const;
  void slot_generation (ACE_UINT32 g);

  /// Size required to store information about active key.
  static size_t size (void);

  /// Recover state of active key from <data>.  User must make sure
  /// that <data> encoded using the <encode> method.
  void decode (const void *data);

  /// Encode state of the active key into <data>.  <data> must be as
  /// big as the value returned from <size>.
  void encode (void *data) const;

  /// Compare keys.
  int operator== (const ACE_Active_Map_Manager_Key &rhs) const;
  int operator!= (const ACE_Active_Map_Manager_Key &rhs) const;

  // = This really should be protected but because of template
  // friends, they are not.

  /// Increment the <slot_generation> number.
  void increment_slot_generation_count (void);

private:

  /**
   * @brief Data for the Active Object Map Key.
   *
   * This separate structure makes it easier to manage copying
   *  the index and the generation to and from the user buffer.
   *
   */
  struct key_data
  {
    /// Slot index in the active map.
    ACE_UINT32 slot_index_;

    /// Slot generation number of <slot_index_> slot in the active map.
    ACE_UINT32 slot_generation_;
  };

  /// Data for the Active Object Map Key.
  key_data key_data_;
};

#if defined (__ACE_INLINE__)
#include "ace/Utils/Active_Map_Manager.i"
#endif /* __ACE_INLINE__ */

// Include the templates here.
#include "ace/Utils/Active_Map_Manager_T.h"

#include "ace/post.h"
#endif /* ACE_ACTIVE_MAP_MANAGER_H */

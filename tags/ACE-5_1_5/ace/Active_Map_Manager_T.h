/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Active_Map_Manager_T.h
//
// = AUTHOR
//    Irfan Pyarali
//
// ============================================================================

#ifndef ACE_ACTIVE_MAP_MANAGER_T_H
#define ACE_ACTIVE_MAP_MANAGER_T_H
#include "ace/pre.h"

#include "ace/Map_Manager.h"
#include "ace/Active_Map_Manager.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

template <class T>
class ACE_Active_Map_Manager : public ACE_Map_Manager<ACE_Active_Map_Manager_Key, T, ACE_Null_Mutex>
{
  // = TITLE
  //     Define a map abstraction that associates system generated
  //     keys with user specified values.
  //
  // = DESCRIPTION
  //     Since the key is system generated, searches are very fast and
  //     take a constant amount of time.
public:

  // = Traits.
  typedef ACE_Active_Map_Manager_Key key_type;
  typedef T mapped_type;

  typedef ACE_Map_Entry<ACE_Active_Map_Manager_Key, T> ENTRY;
  typedef ACE_Map_Iterator<ACE_Active_Map_Manager_Key, T, ACE_Null_Mutex> ITERATOR;
  typedef ACE_Map_Reverse_Iterator<ACE_Active_Map_Manager_Key, T, ACE_Null_Mutex> REVERSE_ITERATOR;

  typedef ENTRY entry;
  typedef ITERATOR iterator;
  typedef REVERSE_ITERATOR reverse_iterator;

  // = Initialization and termination methods.
  ACE_Active_Map_Manager (ACE_Allocator *alloc = 0);
  // Initialize a <Active_Map_Manager> with the <ACE_DEFAULT_MAP_SIZE>.

  ACE_Active_Map_Manager (size_t size,
                          ACE_Allocator *alloc = 0);
  // Initialize a <Active_Map_Manager> with <size> entries.

  ~ACE_Active_Map_Manager (void);
  // Close down a <Active_Map_Manager> and release dynamically
  // allocated resources.

  int open (size_t length = ACE_DEFAULT_MAP_SIZE,
            ACE_Allocator *alloc = 0);
  // Initialize a <Active_Map_Manager> with size <length>.

  int close (void);
  // Close down a <Active_Map_Manager> and release dynamically
  // allocated resources.

  int bind (const T &value,
            ACE_Active_Map_Manager_Key &key);
  // Add <value> to the map, and the corresponding key produced by the
  // Active_Map_Manager is returned through <key>.

  int bind (const T &value);
  // Add <value> to the map.  The user does not care about the
  // corresponding key produced by the Active_Map_Manager.

  int bind (ACE_Active_Map_Manager_Key &key,
            T *&internal_value);
  // Reserves a slot in the internal structure and returns the key and
  // a pointer to the value.  User should place their <value> into
  // <*internal_value>.  This method is useful in reducing the number
  // of copies required in some cases.  Note that <internal_value> is
  // only a temporary pointer and will change when the map resizes.
  // Therefore, the user should use the pointer immediately and not
  // hold on to it.

  int rebind (const ACE_Active_Map_Manager_Key &key,
              const T &value);
  // Reassociate <key> with <value>. The function fails if <key> is
  // not in the map.

  int rebind (const ACE_Active_Map_Manager_Key &key,
              const T &value,
              T &old_value);
  // Reassociate <key> with <value>, storing the old value into the
  // "out" parameter <old_value>.  The function fails if <key> is not
  // in the map.

  int rebind (const ACE_Active_Map_Manager_Key &key,
              const T &value,
              ACE_Active_Map_Manager_Key &old_key,
              T &old_value);
  // Reassociate <key> with <value>, storing the old key and value
  // into the "out" parameter <old_key> and <old_value>.  The function
  // fails if <key> is not in the map.

  int find (const ACE_Active_Map_Manager_Key &key,
            T &value) const;
  // Locate <value> associated with <key>.

  int find (const ACE_Active_Map_Manager_Key &key) const;
  // Is <key> in the map?

  int find (const ACE_Active_Map_Manager_Key &key,
            T *&internal_value) const;
  // Locate <value> associated with <key>.  The value is returned via
  // <internal_value> and hence a copy is saved.  Note that
  // <internal_value> is only a temporary pointer and will change when
  // the map resizes.  Therefore, the user should use the pointer
  // immediately and not hold on to it.

  // Creates a key.  User should place their <value> into
  // <*internal_value>.  This method is useful in reducing the number
  // of copies required in some cases.

  int unbind (const ACE_Active_Map_Manager_Key &key);
  // Remove <key> from the map.

  int unbind (const ACE_Active_Map_Manager_Key &key,
              T &value);
  // Remove <key> from the map, and return the <value> associated with
  // <key>.

  int unbind (const ACE_Active_Map_Manager_Key &key,
              T *&internal_value);
  // Locate <value> associated with <key>.  The value is returned via
  // <internal_value> and hence a copy is saved.  Note that
  // <internal_value> is only a temporary pointer and will change when
  // the map resizes or when this slot is reused.  Therefore, the user
  // should use the pointer immediately and not hold on to it.

  size_t current_size (void) const;
  // Return the current size of the map.

  size_t total_size (void) const;
  // Return the total size of the map.

  static const ACE_Active_Map_Manager_Key npos (void);
  // Returns a key that cannot be found in the map.

  void dump (void) const;
  // Dump the state of an object.

  // = STL styled iterator factory functions.

  ACE_Map_Iterator<ACE_Active_Map_Manager_Key, T, ACE_Null_Mutex> begin (void);
  ACE_Map_Iterator<ACE_Active_Map_Manager_Key, T, ACE_Null_Mutex> end (void);
  // Return forward iterator.

  ACE_Map_Reverse_Iterator<ACE_Active_Map_Manager_Key, T, ACE_Null_Mutex> rbegin (void);
  ACE_Map_Reverse_Iterator<ACE_Active_Map_Manager_Key, T, ACE_Null_Mutex> rend (void);
  // Return reverse iterator.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:

  typedef ACE_Map_Manager<ACE_Active_Map_Manager_Key, T, ACE_Null_Mutex> ACE_AMM_BASE;
  // Private base class

private:

  // = Disallow these operations.
  ACE_UNIMPLEMENTED_FUNC (void operator= (const ACE_Active_Map_Manager<T> &))
  ACE_UNIMPLEMENTED_FUNC (ACE_Active_Map_Manager (const ACE_Active_Map_Manager<T> &))
};

#if defined (__ACE_INLINE__)
#include "ace/Active_Map_Manager_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Active_Map_Manager_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Active_Map_Manager_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* ACE_ACTIVE_MAP_MANAGER_T_H */

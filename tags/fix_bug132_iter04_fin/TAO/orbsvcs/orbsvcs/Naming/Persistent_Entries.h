/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    cos
//
// = FILENAME
//   Persistent_Entries.h
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_PERSISTENT_ENTRIES_H
#define TAO_PERSISTENT_ENTRIES_H
#include "ace/pre.h"

#include "ace/Hash_Map_With_Allocator_T.h"
#include "tao/corba.h"
#include "orbsvcs/CosNamingC.h"
#include "naming_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Naming_Export TAO_Persistent_IntId
{
  // = TITLE
  //     Helper class for TAO_Persistent_Bindings_Map: unifies several
  //     data items, so they can be stored together as a  <value>
  //     for a <key> in a hash table holding the state of a Persistent
  //     Naming Context.
  //
  // = DESCRIPTION
  //     This class holds a strigified IOR and a binding type, so
  //     they can be stored together as a <value> for a <key> in a
  //     hash table holding the state of a Persistent Naming Context.
  //
  //     Memory for the ior isn't allocated/deallocated, this class just
  //     copies a pointer.
public:
  // = Initialization and termination methods.
  TAO_Persistent_IntId (void);
  // Constructor.

  TAO_Persistent_IntId (char * obj_ref,
                        CosNaming::BindingType type /* = CosNaming::nobject */);
  // Constructor.

  TAO_Persistent_IntId (const TAO_Persistent_IntId & rhs);
  // Copy constructor.

  ~TAO_Persistent_IntId (void);
  // Destructor.

  void operator= (const TAO_Persistent_IntId & rhs);
  // Assignment operator.

  // = Data members.

  const char *ref_;
  // Stringified IOR to be stored in a Persistent Naming Context.

  CosNaming::BindingType type_;
  // Binding type for <ref_>.
};

class TAO_Naming_Export TAO_Persistent_ExtId
{
  // = TITLE
  //     Helper class for TAO_Persistent_Bindings_Map: unifies several
  //     data items, so they can be stored together as a  <key>
  //     for a <value> in a hash table holding the state of a Persistent
  //     Naming Context.
  //
  // = DESCRIPTION
  //     This class holds id and kind strings, so
  //     they can be stored together as a <key> for a <value> in a
  //     hash table holding the state of a Persistent Naming Context.
  //
  //     Memory for id and kind isn't allocated/deallocated, this
  //     class just copies pointers.
public:
  // = Initialization and termination methods.

  TAO_Persistent_ExtId (void);
  // Constructor.

  TAO_Persistent_ExtId (const char *id,
                        const char *kind);
  // Constructor.

  TAO_Persistent_ExtId (const TAO_Persistent_ExtId & rhs);
  // Copy constructor.

  ~TAO_Persistent_ExtId (void);
  // Destructor.

  // = Assignment and comparison methods.

  void operator= (const TAO_Persistent_ExtId & rhs);
  // Assignment operator (does copy memory).

  int operator== (const TAO_Persistent_ExtId &rhs) const;
  // Equality comparison operator (must match both id_ and kind_).

  int operator!= (const TAO_Persistent_ExtId &rhs) const;
  // Inequality comparison operator.

  u_long hash (void) const;
  // <hash> function is required in order for this class to be usable by
  // ACE_Hash_Map_Manager.

  // = Data members.

  const char * id_;
  // <kind> portion of the name to be associated with some object
  // reference in a Persistent Naming Context.

  const char * kind_;
  // <id> portion of the name to be associated with some object
  // reference in a Persistent Naming Context.

  // Accessors.

  const char * id (void);
  const char * kind (void);
};

class TAO_Naming_Export TAO_Persistent_Index_IntId
{
  // = TITLE
  //     Helper class for TAO_Persistent_Context_Index: unifies several
  //     data items, so they can be stored together as a  <value>
  //     for a <key> in a hash table holding the state of a Persistent
  //     Context Index.  (Persistent Context Index is like directory
  //     that stores info about every active Naming Context).
  //
  // = DESCRIPTION
  //     This class holds a counter and a hash map pointers, so
  //     they can be stored together as a <value> for a <key> in a
  //     hash table holding the state of a Persistent Context Index.
  //
public:
  // = Initialization and termination methods.
  TAO_Persistent_Index_IntId (void);
  // Constructor.

  TAO_Persistent_Index_IntId (ACE_UINT32 *counter,
                              ACE_Hash_Map_With_Allocator<TAO_Persistent_ExtId,
                              TAO_Persistent_IntId> * hash_map);
  // Constructor.

  TAO_Persistent_Index_IntId (const TAO_Persistent_Index_IntId & rhs);
  // Copy constructor.

  ~TAO_Persistent_Index_IntId (void);
  // Destructor.

  void operator= (const TAO_Persistent_Index_IntId & rhs);
  // Assignment operator (does copy memory).

  // = Data members.

  ACE_UINT32 *counter_;
  // Pointer to a Persistent Naming Context's counter.

  ACE_Hash_Map_With_Allocator<TAO_Persistent_ExtId,
    TAO_Persistent_IntId> * hash_map_;
  // Pointer to a Persistent Naming Context's hash map.
};

class TAO_Naming_Export TAO_Persistent_Index_ExtId
{
  // = TITLE
  //    Helper class for TAO_Persistent_Context_Index: holds
  //    Persistent Naming Context POA id, so it can be stored as a <key>
  //    for a <value> in a hash table holding state of a Persistent
  //    Context Index.  (Persistent Context Index is like directory
  //    that stores info about every active Naming Context).
  //
  // = DESCRIPTION
  //    We need this wrapper class around the actual data because we must
  //    provide <hash> function for it to work with
  //    ACE_Hash_Map_Manager.
  //
public:
  // = Initialization and termination methods.

  TAO_Persistent_Index_ExtId (void);
  // Constructor.

  TAO_Persistent_Index_ExtId (const char *poa_id);
  // Constructor.

  TAO_Persistent_Index_ExtId (const TAO_Persistent_Index_ExtId & rhs);
  // Copy constructor.

  ~TAO_Persistent_Index_ExtId (void);
  // Destructor.

  // = Assignment and comparison methods.

  void operator= (const TAO_Persistent_Index_ExtId & rhs);
  // Assignment operator (does copy memory).

  int operator== (const TAO_Persistent_Index_ExtId &rhs) const;
  // Equality comparison operator (must match both id_ and kind_).

  int operator!= (const TAO_Persistent_Index_ExtId &rhs) const;
  // Inequality comparison operator.

  u_long hash (void) const;
  // <hash> function is required in order for this class to be usable by
  // ACE_Hash_Map_Manager.

  // = Data member.

  const char * poa_id_;
  // POA id to be associated with the rest of the info for some
  // Persistent Naming Context in the Persistent Context Index.
};

#include "ace/post.h"
#endif /* TAO_PERSISTENT_ENTRIES_H */

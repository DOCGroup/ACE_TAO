// -*- C++ -*-

//=============================================================================
/**
 *  @file   Persistent_Entries.h
 *
 *  @author Marina Spivak <marina@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_PERSISTENT_ENTRIES_H
#define TAO_PERSISTENT_ENTRIES_H
#include /**/ "ace/pre.h"

#include "ace/Hash_Map_With_Allocator_T.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Naming/naming_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Persistent_IntId
 *
 * @brief Helper class for TAO_Persistent_Bindings_Map: unifies several
 * data items, so they can be stored together as a  <value>
 * for a <key> in a hash table holding the state of a Persistent
 * Naming Context.
 *
 * This class holds a strigified IOR and a binding type, so
 * they can be stored together as a <value> for a <key> in a
 * hash table holding the state of a Persistent Naming Context.
 * Memory for the ior isn't allocated/deallocated, this class just
 * copies a pointer.
 */
class TAO_Naming_Serv_Export TAO_Persistent_IntId
{
public:
  // = Initialization and termination methods.
  /// Constructor.
  TAO_Persistent_IntId (void);

  /// Constructor.
  TAO_Persistent_IntId (char * obj_ref,
                        CosNaming::BindingType type /* = CosNaming::nobject */);

  /// Copy constructor.
  TAO_Persistent_IntId (const TAO_Persistent_IntId & rhs);

  /// Destructor.
  ~TAO_Persistent_IntId (void);

  /// Assignment operator.
  void operator= (const TAO_Persistent_IntId & rhs);

  // = Data members.

  /// Stringified IOR to be stored in a Persistent Naming Context.
  const char *ref_;

  /// Binding type for <ref_>.
  CosNaming::BindingType type_;
};

/**
 * @class TAO_Persistent_ExtId
 *
 * @brief Helper class for TAO_Persistent_Bindings_Map: unifies several
 * data items, so they can be stored together as a <key>
 * for a <value> in a hash table holding the state of a Persistent
 * Naming Context.
 *
 * This class holds id and kind strings, so
 * they can be stored together as a <key> for a <value> in a
 * hash table holding the state of a Persistent Naming Context.
 * Memory for id and kind isn't allocated/deallocated, this
 * class just copies pointers.
 */
class TAO_Naming_Serv_Export TAO_Persistent_ExtId
{
public:
  // = Initialization and termination methods.

  /// Constructor.
  TAO_Persistent_ExtId (void);

  /// Constructor.
  TAO_Persistent_ExtId (const char *id,
                        const char *kind);

  /// Copy constructor.
  TAO_Persistent_ExtId (const TAO_Persistent_ExtId & rhs);

  /// Destructor.
  ~TAO_Persistent_ExtId (void);

  // = Assignment and comparison methods.

  /// Assignment operator (does copy memory).
  void operator= (const TAO_Persistent_ExtId & rhs);

  /// Equality comparison operator (must match both id_ and kind_).
  bool operator== (const TAO_Persistent_ExtId &rhs) const;

  /// Inequality comparison operator.
  bool operator!= (const TAO_Persistent_ExtId &rhs) const;

  /// hash() function is required in order for this class to be usable by
  /// ACE_Hash_Map_Manager.
  u_long hash (void) const;

  // = Data members.

  /// id portion of the name to be associated with some object
  /// reference in a Persistent Naming Context.
  const char * id_;

  /// kind portion of the name to be associated with some object
  /// reference in a Persistent Naming Context.
  const char * kind_;

  // Accessors.

  const char * id (void);
  const char * kind (void);
};

/**
 * @class TAO_Persistent_Index_IntId
 *
 * @brief Helper class for TAO_Persistent_Context_Index: unifies several
 * data items, so they can be stored together as a  <value>
 * for a <key> in a hash table holding the state of a Persistent
 * Context Index.  (Persistent Context Index is like directory
 * that stores info about every active Naming Context).
 *
 * This class holds a counter and a hash map pointers, so
 * they can be stored together as a <value> for a <key> in a
 * hash table holding the state of a Persistent Context Index.
 */
class TAO_Naming_Serv_Export TAO_Persistent_Index_IntId
{
public:
  // = Initialization and termination methods.
  /// Constructor.
  TAO_Persistent_Index_IntId (void);

  /// Constructor.
  TAO_Persistent_Index_IntId (ACE_UINT32 *counter,
                              ACE_Hash_Map_With_Allocator<TAO_Persistent_ExtId,
                              TAO_Persistent_IntId> * hash_map);

  /// Copy constructor.
  TAO_Persistent_Index_IntId (const TAO_Persistent_Index_IntId & rhs);

  /// Destructor.
  ~TAO_Persistent_Index_IntId (void);

  /// Assignment operator (does copy memory).
  void operator= (const TAO_Persistent_Index_IntId & rhs);

  // = Data members.

  /// Pointer to a Persistent Naming Context's counter.
  ACE_UINT32 *counter_;

  /// Pointer to a Persistent Naming Context's hash map.
  ACE_Hash_Map_With_Allocator<TAO_Persistent_ExtId,
    TAO_Persistent_IntId> * hash_map_;
};

/**
 * @class TAO_Persistent_Index_ExtId
 *
 * @brief Helper class for TAO_Persistent_Context_Index: holds
 * Persistent Naming Context POA id, so it can be stored as a <key>
 * for a <value> in a hash table holding state of a Persistent
 * Context Index.  (Persistent Context Index is like directory
 * that stores info about every active Naming Context).
 *
 * We need this wrapper class around the actual data because we must
 * provide <hash> function for it to work with
 * ACE_Hash_Map_Manager.
 */
class TAO_Naming_Serv_Export TAO_Persistent_Index_ExtId
{
public:
  // = Initialization and termination methods.

  /// Constructor.
  TAO_Persistent_Index_ExtId (void);

  /// Constructor.
  TAO_Persistent_Index_ExtId (const char *poa_id);

  /// Copy constructor.
  TAO_Persistent_Index_ExtId (const TAO_Persistent_Index_ExtId & rhs);

  /// Destructor.
  ~TAO_Persistent_Index_ExtId (void);

  // = Assignment and comparison methods.

  /// Assignment operator (does copy memory).
  void operator= (const TAO_Persistent_Index_ExtId & rhs);

  /// Equality comparison operator (must match both id_ and kind_).
  bool operator== (const TAO_Persistent_Index_ExtId &rhs) const;

  /// Inequality comparison operator.
  bool operator!= (const TAO_Persistent_Index_ExtId &rhs) const;

  /// <hash> function is required in order for this class to be usable by
  /// ACE_Hash_Map_Manager.
  u_long hash (void) const;

  // = Data member.

  /// POA id to be associated with the rest of the info for some
  /// Persistent Naming Context in the Persistent Context Index.
  const char * poa_id_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_PERSISTENT_ENTRIES_H */

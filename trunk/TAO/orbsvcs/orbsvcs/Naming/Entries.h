// -*- C++ -*-

//=============================================================================
/**
 *  @file   Entries.h
 *
 *  $Id$
 *
 *  @author Marina Spivak <marina@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_ENTRIES_H
#define TAO_ENTRIES_H
#include /**/ "ace/pre.h"

#include "ace/Hash_Map_Manager.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SString.h"

#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Naming/naming_serv_export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_IntId
 *
 * @brief Helper class for TAO_Transient_Bindings_Map: unifies several
 * data items, so they can be stored together as a  <value>
 * for a <key> in a hash table holding the state of a Transient
 * Naming Context.
 *
 * This class holds CORBA Object pointer and a binding type, so
 * they can be stored together as a <value> for a <key> in a
 * hash table holding the state of a Transient Naming Context.
 */
class TAO_Naming_Serv_Export TAO_IntId
{
public:
  // = Initialization and termination methods.
  /// Constructor.
  TAO_IntId (void);

  /// Constructor.
  TAO_IntId (CORBA::Object_ptr obj,
            CosNaming::BindingType type /* = CosNaming::nobject */);

  /// Copy constructor.
  TAO_IntId (const TAO_IntId & rhs);

  /// Destructor.
  ~TAO_IntId (void);

  /// Assignment operator (does copy memory).
  void operator= (const TAO_IntId & rhs);

  // = Data members.

  /// Object reference to be stored in a Transient Naming Context.
  CORBA::Object_ptr ref_;

  /// Binding type for <ref_>.
  CosNaming::BindingType type_;
};

/**
 * @class TAO_ExtId
 *
 * @brief Helper class for TAO_Transient_Bindings_Map: unifies several
 * data items, so they can be stored together as a  <key>
 * for a <value> in a hash table holding the state of a Transient
 * Naming Context.
 *
 * This class holds id and kind strings, so
 * they can be stored together as a <key> for a <value> in a
 * hash table holding the state of a Transient Naming Context.
 */
class TAO_Naming_Serv_Export TAO_ExtId
{
public:
  // = Initialization and termination methods.

  /// Constructor.
  TAO_ExtId (void);

  /// Constructor.
  TAO_ExtId (const char *id,
            const char *kind);

  /// Copy constructor.
  TAO_ExtId (const TAO_ExtId & rhs);

  /// Destructor.
  ~TAO_ExtId (void);

  // = Assignment and comparison operators.

  /// Assignment operator (does copy memory).
  void operator= (const TAO_ExtId & rhs);

  /// Equality comparison operator (must match both id_ and kind_).
  bool operator== (const TAO_ExtId &rhs) const;

  /// Inequality comparison operator.
  bool operator!= (const TAO_ExtId &rhs) const;

  /// <hash> function is required in order for this class to be usable by
  /// ACE_Hash_Map_Manager.
  u_long hash (void) const;

  // = Data members.

  /// <kind> portion of the name to be associated with some object
  /// reference in a Transient Naming Context.
  ACE_CString kind_;

  /// <id> portion of the name to be associated with some object
  /// reference in a Transient Naming Context.
  ACE_CString id_;

  // = Accessors

  /// Return <id_> in a const char * format.
  const char * id (void);

  /// Return <kind_> in a const char * format.
  const char * kind (void);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_ENTRIES_H */

/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    cos
//
// = FILENAME
//   Entries.h
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_ENTRIES_H
#define TAO_ENTRIES_H
#include "ace/pre.h"

#include "ace/Hash_Map_Manager.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Synch.h"
#include "ace/SString.h"

#include "tao/corba.h"
#include "orbsvcs/CosNamingC.h"
#include "naming_export.h"

class TAO_Naming_Export TAO_IntId
{
  // = TITLE
  //     Helper class for TAO_Transient_Bindings_Map: unifies several
  //     data items, so they can be stored together as a  <value>
  //     for a <key> in a hash table holding the state of a Transient
  //     Naming Context.
  //
  // = DESCRIPTION
  //     This class holds CORBA Object pointer and a binding type, so
  //     they can be stored together as a <value> for a <key> in a
  //     hash table holding the state of a Transient Naming Context.
  //
public:
  // = Initialization and termination methods.
  TAO_IntId (void);
  // Constructor.

  TAO_IntId (CORBA::Object_ptr obj,
            CosNaming::BindingType type /* = CosNaming::nobject */);
  // Constructor.

  TAO_IntId (const TAO_IntId & rhs);
  // Copy constructor.

  ~TAO_IntId (void);
  // Destructor.

  void operator= (const TAO_IntId & rhs);
  // Assignment operator (does copy memory).

  // = Data members.

  CORBA::Object_ptr ref_;
  // Object reference to be stored in a Transient Naming Context.

  CosNaming::BindingType type_;
  // Binding type for <ref_>.
};

class TAO_Naming_Export TAO_ExtId
{
  // = TITLE
  //     Helper class for TAO_Transient_Bindings_Map: unifies several
  //     data items, so they can be stored together as a  <key>
  //     for a <value> in a hash table holding the state of a Transient
  //     Naming Context.
  //
  // = DESCRIPTION
  //     This class holds id and kind strings, so
  //     they can be stored together as a <key> for a <value> in a
  //     hash table holding the state of a Transient Naming Context.
  //
public:
  // = Initialization and termination methods.

  TAO_ExtId (void);
  // Constructor.

  TAO_ExtId (const char *id,
            const char *kind);
  // Constructor.

  TAO_ExtId (const TAO_ExtId & rhs);
  // Copy constructor.

  ~TAO_ExtId (void);
  // Destructor.

  // = Assignment and comparison operators.

  void operator= (const TAO_ExtId & rhs);
  // Assignment operator (does copy memory).

  int operator== (const TAO_ExtId &rhs) const;
  // Equality comparison operator (must match both id_ and kind_).

  int operator!= (const TAO_ExtId &rhs) const;
  // Inequality comparison operator.

  u_long hash (void) const;
  // <hash> function is required in order for this class to be usable by
  // ACE_Hash_Map_Manager.

  // = Data members.

  ACE_CString kind_;
  // <kind> portion of the name to be associated with some object
  // reference in a Transient Naming Context.

  ACE_CString id_;
  // <id> portion of the name to be associated with some object
  // reference in a Transient Naming Context.

  // = Accessors: return <id_> and <kind_> in a const char * format.

  const char * id (void);
  const char * kind (void);
};

#include "ace/post.h"
#endif /* TAO_ENTRIES_H */

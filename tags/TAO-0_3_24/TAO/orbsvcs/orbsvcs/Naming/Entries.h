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

#include "ace/Hash_Map_Manager.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Synch.h"
#include "ace/SString.h"

#include "tao/corba.h"
#include "orbsvcs/CosNamingC.h"

class TAO_ORBSVCS_Export TAO_IntId
{
  // = TITLE
  //     Stores information a context keeps for each bound name.
  //
  // = DESCRIPTION
  //     Each bound name is associated with an object reference and
  //     the type of binding.
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

  CORBA::Object_ptr ref_;
  // CORBA object reference of the bound object.

  CosNaming::BindingType type_;
  // Indicator of whether the object is a NamingContext that should
  // participate in name resolution when compound names are used.
};

class TAO_ORBSVCS_Export TAO_ExtId
{
  // = TITLE
  //    Stores the name to which an object is bound.
  //
  // = DESCRIPTION
  //    This class is used as the External ID for the
  //    <ACE_Hash_Map_Manager>.
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

  void operator= (const TAO_ExtId & rhs);
  // Assignment operator (does copy memory).

  int operator== (const TAO_ExtId &rhs) const;
  // Equality comparison operator (must match both id_ and kind_).

  int operator!= (const TAO_ExtId &rhs) const;
  // Inequality comparison operator.

  u_long hash (void) const;
  // This class has to have a hash for use with ACE_Hash_Map_Manager.

  ACE_CString kind_;
  // Any information user wants to store (not used by Naming Service).

  ACE_CString id_;
  // Any information user wants to store (not used by Naming Service).
};

#endif /* TAO_ENTRIES_H */

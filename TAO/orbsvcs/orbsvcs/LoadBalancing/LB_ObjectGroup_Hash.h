// -*- C++ -*-
//
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO_LoadBalancing
//
// = FILENAME
//     LB_Hash.h
//
// = AUTHOR
//     Ossama Othman <ossama@uci.edu>
//
// ============================================================================

#ifndef TAO_LB_HASH_H
#define TAO_LB_HASH_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "LoadbalancingC.h"

class TAO_LB_ObjectGroup_Hash
{
  // = TITLE
  //     Hash function object for generating a hash for an ObjectGroup
  //     reference.
  //
  // = DESCRIPTION
  //     This class basically wraps the CORBA::Object::_hash() method
  //     so that it can be utilized by a Hash Map.

public:

  CORBA::ULong operator() (const TAO_LoadBalancing::ObjectGroup_ptr &) const;
  // Invokes the _hash() method on the ObjectGroup reference.
};


class TAO_LB_FactoryCreationId_Hash
{
  // = TITLE
  //     Hash function object for generating a hash for a
  //     FactoryCreationId.
  //
  // = DESCRIPTION
  //     Create a hash based on the contents of the FactoryCreationId.

public:

  CORBA::ULong operator() (const TAO_LoadBalancing::FactoryCreationId &) const;
  // Generates a hash value based on the contents of the
  // FactoryCreationId.
};


#if defined (__ACE_INLINE__)
#include "LB_Hash.inl"
#endif  /* __ACE_INLINE__ */

#include "ace/post.h"

#endif  /* TAO_LB_HASH_H */

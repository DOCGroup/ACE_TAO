// -*- C++ -*-
//
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO_LoadBalancing
//
// = FILENAME
//     LB_Replica_Hash.h
//
// = AUTHOR
//     Ossama Othman <ossama@uci.edu>
//
// ============================================================================

#ifndef TAO_LB_REPLICA_HASH_H
#define TAO_LB_REPLICA_HASH_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/ACE.h"
#include "LoadbalancingC.h"

class TAO_LB_Replica_Hash
{
  // = TITLE
  //     Hash function object for generating a hash for a Replica
  //     reference.
  //
  // = DESCRIPTION
  //     This class basically wraps the CORBA::Object::_hash() method
  //     so that it can be utilized by a Hash Map.

public:

  CORBA::ULong operator() (const CORBA::::Object_ptr &replica) const;
  // Invokes the _hash() method on the replica reference.
};


class TAO_LB_Location_Hash
{
  // = TITLE
  //     Hash function object for generating a hash for a
  //     Location.
  //
  // = DESCRIPTION
  //     Create a hash based on the contents of the Location.

public:

  CORBA::ULong operator() (const TAO_LoadBalancing::Location &) const;
  // Generates a hash value based on the contents of the
  // Location.
};


#if defined (__ACE_INLINE__)
#include "LB_Replica_Hash.inl"
#endif  /* __ACE_INLINE__ */

#include "ace/post.h"

#endif  /* TAO_LB_REPLICA_HASH_H */

// -*- C++ -*-
//
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO_LoadBalancing
//
// = FILENAME
//     LB_Replica_Equal_To.h
//
// = AUTHOR
//     Ossama Othman <ossama@uci.edu>
//
// ============================================================================

#ifndef TAO_LB_REPLICA_EQUAL_TO_H
#define TAO_LB_REPLICA_EQUAL_TO_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "LoadbalancingC.h"

class TAO_LB_Replica_Equal_To
{
  // = TITLE
  //     Function object for comparing equivalence of two Replica
  //     references.
  //
  // = DESCRIPTION
  //     This class basically wraps the
  //     CORBA::Object::_is_equivalent() method so that it can be
  //     utilized by a Hash Map to determine equivalence of two
  //     external IDs.

public:

  int operator() (const CORBA::Object_ptr &lhs,
                  const CORBA::Object_ptr &rhs) const;
  // Invokes the CORBA::Object::_is_equivalent() method to determine
  // if both references refer to the same object group.  However, this
  // is a weak test since is_equivalent() can only conclusively
  // determine if two references refer to the same object, but it
  // cannot determine if two refereneces do not refer to the same
  // object.
};

class TAO_LB_Location_Equal_To
{
  // = TITLE
  //     Function object for comparing equivalence of two Locations
  //
  // = DESCRIPTION
  //     This class iterates through the contents of the two Locations
  //     being compared.  If any of the corresponding contents of the
  //     Locations do not match, then the Locations are not equal.

public:

  int operator() (const TAO_LoadBalancing::Location &lhs,
                  const TAO_LoadBalancing::Location &rhs) const;
  // Iterate through the contents of each Location until a mismatch
  // occurs.  If no mismatch is found, then the Locations are
  // considered equal.
};


#if defined (__ACE_INLINE__)
#include "LB_Replica_Equal_To.inl"
#endif  /* __ACE_INLINE__ */

#include "ace/post.h"

#endif  /* TAO_LB_REPLICA_EQUAL_TO_H */

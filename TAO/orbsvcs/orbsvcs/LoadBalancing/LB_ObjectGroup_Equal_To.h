// -*- C++ -*-
//
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO_LoadBalancing
//
// = FILENAME
//     LB_ObjectGroup_Equal_To.h
//
// = AUTHOR
//     Ossama Othman <ossama@uci.edu>
//
// ============================================================================

#ifndef TAO_LB_OBJECTGROUP_EQUAL_TO_H
#define TAO_LB_OBJECTGROUP_EQUAL_TO_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "LoadbalancingC.h"

class TAO_LB_ObjectGroup_Equal_To
{
  // = TITLE
  //     Function object that compares equality of two ObjectGroup
  //     references.
  //
  // = DESCRIPTION
  //     This class basically wraps the
  //     CORBA::Object::_is_equivalent() method so that it can be
  //     utilized by a Hash Map as a way to compare equality.

public:

  int operator() (const TAO_LoadBalancing::ObjectGroup_ptr &lhs,
                  const TAO_LoadBalancing::ObjectGroup_ptr &rhs) const;
  // Invokes the CORBA::Object::_is_equivalent() method to determine
  // if both references refer to the same object group.  However, this
  // is a weak test since is_equivalent() can only conclusively
  // determine if two references refer to the same object, but it
  // cannot determine if two refereneces do not refer to the same
  // object.
};

class TAO_LB_FactoryCreationId_Equal_To
{
  // = TITLE
  //     Function object that compares equality of two
  //     FactoryCreationId values.
  //
  // = DESCRIPTION
  //     This class extracts the actual value of the FactoryCreationId
  //     from the CORBA::Any the FactoryCreationId is typedefed to.

public:

  int operator() (
    const TAO_LoadBalancing::FactoryCreationId &lhs,
    const TAO_LoadBalancing::FactoryCreationId &rhs) const;
  // Check if two FactoryCreationId values are the same.
};


#if defined (__ACE_INLINE__)
#include "LB_ObjectGroup_Equal_To.inl"
#endif  /* __ACE_INLINE__ */

#include "ace/post.h"

#endif  /* TAO_LB_OBJECTGROUP_EQUAL_TO_H */

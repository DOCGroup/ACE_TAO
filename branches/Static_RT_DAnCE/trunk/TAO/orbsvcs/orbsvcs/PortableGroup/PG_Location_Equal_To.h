// -*- C++ -*-

//=============================================================================
/**
 *  @file    PG_Location_Equal_To.h
 *
 *  $Id$
 *
 *  @author  Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_PG_LOCATION_EQUAL_TO_H
#define TAO_PG_LOCATION_EQUAL_TO_H

#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/PortableGroup/PG_Operators.h"
#include "orbsvcs/PortableGroup/portablegroup_export.h"
#include "orbsvcs/PortableGroupC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_PG_Location_Equal_To
 *
 * @brief "Equal_To" function object that determines if two location
 *        names are the same.
 *
 * This functor simply does a string comparison of each of the
 * elements in the given TAO_PortableGroup::Location name sequences.
 */
class TAO_PortableGroup_Export TAO_PG_Location_Equal_To
{
public:

  /// Perform a string comparison on each of the sequence elements in
  /// the given locations.
  int operator() (const PortableGroup::Location &lhs,
                  const PortableGroup::Location &rhs) const;

};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/PortableGroup/PG_Location_Equal_To.inl"
#endif  /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif  /* TAO_PG_LOCATION_EQUAL_TO_H */

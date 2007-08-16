// -*- C++ -*-

//=============================================================================
/**
 *  @file    PG_Location_Hash.h
 *
 *  $Id$
 *
 *  @author  Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_PG_LOCATION_HASH_H
#define TAO_PG_LOCATION_HASH_H

#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/PortableGroup/portablegroup_export.h"
#include "orbsvcs/PortableGroupC.h"
#include "ace/ACE.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_PG_Location_Hash
 *
 * @brief Hash function object for generating a hash for a Location.
 */
class TAO_PortableGroup_Export TAO_PG_Location_Hash
{

public:

  /// Creates a hash based on all of elements of the given Location
  CORBA::ULong operator() (const PortableGroup::Location &) const;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/PortableGroup/PG_Location_Hash.inl"
#endif  /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif  /* TAO_PG_LOCATION_HASH_H */

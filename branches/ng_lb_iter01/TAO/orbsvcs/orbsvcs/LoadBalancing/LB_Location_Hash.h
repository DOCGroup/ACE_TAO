// -*- C++ -*-

//=============================================================================
/**
 *  @file    LB_Location_Hash.h
 *
 *  $Id$
 *
 *  @author  Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_LB_LOCATION_HASH_H
#define TAO_LB_LOCATION_HASH_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/LoadBalancingC.h"

/**
 * @class TAO_LB_Location_Hash
 *
 * @brief Hash function object for generating a hash for a Location.
 */
class TAO_LB_Location_Hash
{

public:

  /// Creates a hash based on all of elements of the given Location
  CORBA::ULong operator() (const PortableGroup::Location &) const;
};


#if defined (__ACE_INLINE__)
#include "LB_Location_Hash.inl"
#endif  /* __ACE_INLINE__ */

#include "ace/post.h"

#endif  /* TAO_LB_LOCATION_HASH_H */

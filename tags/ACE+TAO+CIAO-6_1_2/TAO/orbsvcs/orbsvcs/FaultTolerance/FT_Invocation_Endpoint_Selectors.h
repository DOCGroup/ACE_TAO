// -*- C++ -*-

//=============================================================================
/**
 *  @file FT_Invocation_Endpoint_Selectors.h
 *
 *  $Id$
 *
 * Strategies for selecting profile/endpoint from an IOR for making an
 * invocation.
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_FT_INVOCATION_ENDPOINT_SELECTOR_H
#define TAO_FT_INVOCATION_ENDPOINT_SELECTOR_H

#include /**/ "ace/pre.h"

#include "orbsvcs/FaultTolerance/FT_ClientORB_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Invocation_Endpoint_Selectors.h"
#include "tao/Basic_Types.h"
#include "tao/ORB_Constants.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Profile;

/**
 * @class TAO_FT_Invocation_Endpoint_Selector
 *
 * @brief Implementation specific for FT_Service.
 *
 * We inherit from the Default endpoint selector, as this would give
 * us a chance to use the default endpoint selector options if
 * needed.
 *
 */

class TAO_FT_ClientORB_Export TAO_FT_Invocation_Endpoint_Selector :
  public TAO_Default_Endpoint_Selector
{

public:
  /// Constructor.
  TAO_FT_Invocation_Endpoint_Selector (void);

  /// Destructor.
  virtual ~TAO_FT_Invocation_Endpoint_Selector (void);

  /// Implementation for FT. Please refer to
  /// $TAO_ROOT/tao/Invocation_Endpoint_Selectors.h  for details of
  /// documentation.
  virtual void select_endpoint (TAO::Profile_Transport_Resolver *r,
                                ACE_Time_Value *val);

protected:
  /// Select the primary and try connecting to it.
  bool select_primary (TAO::Profile_Transport_Resolver *r,
                       ACE_Time_Value *val);

  /// Select the secondary and try connecting to it. Returns true if
  /// successfull.
  bool select_secondary (TAO::Profile_Transport_Resolver *r,
                         ACE_Time_Value *val);

  /// Helper method that tries to establish connections with all the
  /// endpoints in the profile.
  bool try_connect (TAO::Profile_Transport_Resolver *r,
                    TAO_Profile *profile,
                    ACE_Time_Value *max_wait_time);

  /// Helper method that checks whether the profile is a primary or
  /// not.
  bool check_profile_for_primary (TAO_Profile *);


};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* FT_TAO_INVOCATION_ENDPOINT_SELECTOR_H */

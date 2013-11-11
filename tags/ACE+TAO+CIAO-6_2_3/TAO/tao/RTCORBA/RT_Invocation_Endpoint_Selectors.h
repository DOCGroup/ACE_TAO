// -*- C++ -*-

//=============================================================================
/**
 *  @file   RT_Invocation_Endpoint_Selectors.h
 *
 *  $Id$
 *
 * Strategies for selecting profile/endpoint from an IOR for making an
 * invocation.
 *
 *
 *  @author Priyanka Gontla <pgontla@ece.uci.edu>
 */
//=============================================================================


#ifndef TAO_RT_INVOCATION_ENDPOINT_SELECTOR_H
#define TAO_RT_INVOCATION_ENDPOINT_SELECTOR_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/RTCORBA/RTCORBA.h"
#include "tao/RTCORBA/rtcorba_export.h"
#include "tao/Invocation_Endpoint_Selectors.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_RT_Invocation_Endpoint_Selector
 *
 * @brief TAO_RT_Invocation_Endpoint_Selector
 *
 * This strategy is used when the only policy that might be set is the
 * private connection policy.
 *
 **/
class TAO_RTCORBA_Export TAO_RT_Invocation_Endpoint_Selector
  : public TAO_Invocation_Endpoint_Selector
{
public:

  virtual void select_endpoint (TAO::Profile_Transport_Resolver *r,
                                ACE_Time_Value *val);

protected:
  void select_endpoint_based_on_client_protocol_policy (
      TAO::Profile_Transport_Resolver &r,
      RTCORBA::ClientProtocolPolicy_ptr client_protocol_policy,
      RTCORBA::ProtocolList &protocols,
      ACE_Time_Value *val);

  int endpoint_from_profile (TAO::Profile_Transport_Resolver &r,
                             ACE_Time_Value *v);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */

#include /**/ "ace/post.h"

#endif  /* TAO_RT_INVOCATION_ENDPOINT_SELECTOR_H */

// This may look like C, but it's really -*- C++ -*-

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
#include "ace/pre.h"

#include "tao/corbafwd.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "RTCORBA.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Invocation_Endpoint_Selectors.h"

class TAO_MProfile;
class TAO_Endpoint;

/**
 * @class TAO_RT_Invocation_Endpoint_Selector
 *
 * @brief TAO_RT_Invocation_Endpoint_Selector
 *
 * This strategy is used when the only policy that might be set is the
 * private connection policy.
 *
 **/
class TAO_RTCORBA_Export TAO_RT_Invocation_Endpoint_Selector :
  public TAO_Default_Endpoint_Selector
{
public:

  void select_endpoint (TAO_GIOP_Invocation *invocation
                        ACE_ENV_ARG_DECL);

  void select_endpoint_based_on_client_protocol_policy (TAO_GIOP_Invocation *invocation,
                                                        RTCORBA::ClientProtocolPolicy_ptr client_protocol_policy,
                                                        RTCORBA::ProtocolList &protocols
                                                        ACE_ENV_ARG_DECL);

  int endpoint_from_profile (TAO_GIOP_Invocation *invocation
                             ACE_ENV_ARG_DECL);
};

#if defined (__ACE_INLINE__)
# include "RT_Invocation_Endpoint_Selectors.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */

#include "ace/post.h"
#endif  /* TAO_RT_INVOCATION_ENDPOINT_SELECTOR_H */

// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file   RT_Endpoint_Selector_Factory.h
 *
 *  $Id$
 *
 * Strategies for selecting profile/endpoint from an IOR for making an
 * invocation.
 *
 *  @author Marina Spivak <marina@cs.wustl.edu>
 */
//=============================================================================


#ifndef RT_ENDPOINT_SELECTOR_FACTORY_H
#define RT_ENDPOINT_SELECTOR_FACTORY_H
#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Endpoint_Selector_Factory.h"

class TAO_Priority_Endpoint_Selector;
class TAO_Bands_Endpoint_Selector;
class TAO_Protocol_Endpoint_Selector;
class TAO_Priority_Protocol_Selector;
class TAO_Bands_Protocol_Selector;
class TAO_Client_Priority_Policy_Selector;

// ****************************************************************
#if (TAO_HAS_RT_CORBA == 1)

/**
 * @class RT_Endpoint_Selector_Factory
 *
 * @brief Factory for initializing <Endpoint_Selection_State> and
 * obtaining appropriate <Invocation_Endpoint_Selector>.
 *
 * Used by Invocation classes to intialize its endpoint selection
 * strategy and state based on the effective policies.
 * Endpoint selection strategies are stateless objects - all the
 * state they need is contained by Invocation in
 * <Endpoint_Selection_State>.  Thus, rather than allocating an
 * endpoint selection strategy object for each Invocation, the
 * factory simply returns the appropriate one from the
 * set preallocated in the ORB_Core.  One endpoint selection
 * strategy object can be used by many invocations concurrently.
 */
class TAO_Export RT_Endpoint_Selector_Factory
  : public TAO_Endpoint_Selector_Factory
{
public:
  /// Constructor.
  RT_Endpoint_Selector_Factory (void);

  /// Destructor.
  virtual ~RT_Endpoint_Selector_Factory (void);

  /// Get an Invocation's endpoint selection strategy and
  /// initialize the endpoint selection state instance.
  virtual TAO_Invocation_Endpoint_Selector *get_selector (
                             TAO_GIOP_Invocation *invocation,
                             CORBA::Environment &ACE_TRY_ENV);

protected:

  // = Helpers for <get_selector>.

  /**
   * Gets the appropriate selector if TAO_HAS_CLIENT_PRIORITY_POLICY
   * is enabled. Also initializes endpoint selection state as
   * necessary.   WARNING: TAO::Client_Pririority_Policy is
   * deprecated.  See TAO RTCORBA documentation for more details.
   */
  TAO_Invocation_Endpoint_Selector *check_client_priority_policy (
                                     TAO_GIOP_Invocation *invocation,
                                     CORBA::Environment &ACE_TRY_ENV);

  /// Initializes RTCORBA::ClientProtocolPolicy in the endpoint
  /// selection state.
  void init_client_protocol (TAO_GIOP_Invocation *invocation,
                             CORBA::Environment &ACE_TRY_ENV);

  /// Initializes RTCORBA::PriorityBandsPolicy in the endpoint
  /// selection state.
  void init_bands (TAO_GIOP_Invocation *invocation,
                   CORBA::Environment &ACE_TRY_ENV);

private:
  /// The possible endpoint selector strategies that can be
  /// returned by this factory

  TAO_Priority_Endpoint_Selector *priority_endpoint_selector_;
  TAO_Bands_Endpoint_Selector *bands_endpoint_selector_;
  TAO_Protocol_Endpoint_Selector *protocol_endpoint_selector_;
  TAO_Priority_Protocol_Selector *priority_protocol_selector_;
  TAO_Bands_Protocol_Selector *bands_protocol_selector_;
  TAO_Client_Priority_Policy_Selector *client_priority_policy_selector_;
};

#endif /* TAO_HAS_RT_CORBA == 1 */

#include "ace/post.h"
#endif  /* RT_ENDPOINT_SELECTOR_FACTORY_H */

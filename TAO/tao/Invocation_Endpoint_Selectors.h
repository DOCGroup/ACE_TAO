// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file   Invocation_Endpoint_Selectors.h
 *
 *  $Id$
 *
 * Strategies for selecting profile/endpoint from an IOR for making an
 * invocation.
 *
 *  @author Marina Spivak <marina@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_INVOCATION_ENDPOINT_SELECTOR_H
#define TAO_INVOCATION_ENDPOINT_SELECTOR_H
#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/MProfile.h"

class TAO_GIOP_Invocation;


/**
 * @class TAO_Invocation_Endpoint_Selector
 *
 * @brief Defines the interface for policy-based endpoint selection
 * strategies.
 *
 * Selects/reselects server endpoint for an Invocation.  Different
 * concrete strategies perform selection based on different
 * policies.
 * All endpoint selection strategies are stateless objects - any
 * necessary state is stored in Invocation's
 * <endpoint_selection_state_>.
 */
class TAO_Export TAO_Invocation_Endpoint_Selector
{
public:
  /// Constructor.
  TAO_Invocation_Endpoint_Selector (void);

  /// Destructor.
  virtual ~TAO_Invocation_Endpoint_Selector (void);

  /// Select the endpoint and set <invocation>'s <profile_> and
  /// <endpoint_> data members accordingly.
  virtual void select_endpoint (TAO_GIOP_Invocation *invocation
                                TAO_ENV_ARG_DECL_WITH_DEFAULTS) = 0;

  /**
   * This method must be called when previously selected endpoint
   * didn't work out, and we want to try another one, i.e., reselect.
   * This method performs the necessary state updates, so that next
   * <select_endpoint> call picks a new endpoint.
   */
    virtual void next (TAO_GIOP_Invocation *invocation
                   TAO_ENV_ARG_DECL_WITH_DEFAULTS) = 0;

  /**
   * This method must be called if the invocation attempt on a
   * selected endpoint resulted in location forward.  This method
   * performs the necessary state updates, so that next <select_endpoint>
   * call picks a new endpoint.
   */
  virtual void forward (TAO_GIOP_Invocation *invocation,
                        const TAO_MProfile &mprofile
                        TAO_ENV_ARG_DECL_WITH_DEFAULTS) = 0;

  /// Update the state to indicate that the selected endpoint/profile
  /// were used successfully.
  virtual void success (TAO_GIOP_Invocation *invocation) = 0;

  /// Update the state to reflect that the connection being used for
  /// the invocation has been closed by the server.
  virtual void close_connection (TAO_GIOP_Invocation *invocation) = 0;
};

// ****************************************************************

class TAO_Export TAO_Default_Endpoint_Selector :
  public TAO_Invocation_Endpoint_Selector
{
  // = TITLE
  //   Default strategy for endpoint selection.
  //
  // = DESCRIPTION
  //   This strategy is used when there are no policies set or when
  //   the only policy set is RTCORBA::PriorityModelPolicy and its
  //   value is RTCORBA::SERVER_DECLARED.
  //   The strategy is to try the first endpoint from the first
  //   profile in the IOR.  If that doesn't work, try the next
  //   profile, and so on.
  //
public:
  /// Constructor.
  TAO_Default_Endpoint_Selector (void);

  /// Destructor.
  virtual ~TAO_Default_Endpoint_Selector (void);

  virtual void select_endpoint (TAO_GIOP_Invocation *invocation
                                TAO_ENV_ARG_DECL_WITH_DEFAULTS);
  virtual void next (TAO_GIOP_Invocation *invocation
                     TAO_ENV_ARG_DECL_WITH_DEFAULTS);
  virtual void forward (TAO_GIOP_Invocation *invocation,
                        const TAO_MProfile &mprofile
                        TAO_ENV_ARG_DECL_WITH_DEFAULTS);
  virtual void success (TAO_GIOP_Invocation *invocation);
  virtual void close_connection (TAO_GIOP_Invocation *invocation);

protected:
  /// This method selects an endpoint from the profile in the
  /// invocation object.
  int endpoint_from_profile (TAO_GIOP_Invocation *invocation
                             TAO_ENV_ARG_DECL);
};

#if defined (__ACE_INLINE__)
# include "tao/Invocation_Endpoint_Selectors.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif  /* TAO_INVOCATION_ENDPOINT_SELECTOR_H */

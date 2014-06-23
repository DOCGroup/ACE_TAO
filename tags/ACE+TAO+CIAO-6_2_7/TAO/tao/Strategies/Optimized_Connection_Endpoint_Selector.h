// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file   Optimized_Connection_Endpoint_Selector.h
 *
 *  $Id$
 *
 * Strategies for selecting profile/endpoint from an IOR for making an
 * invocation.
 *
 *  @author Phil Mesnier <mesnier_p@ociweb.com>
 */
//=============================================================================


#ifndef TAO_OPTIMIZED_CONNECTION_ENDPOINT_SELECTOR_H
#define TAO_OPTIMIZED_CONNECTION_ENDPOINT_SELECTOR_H

#include /**/ "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Strategies/strategies_export.h"

#include "tao/Invocation_Endpoint_Selectors.h"
#include "ace/Time_Value.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_GIOP_Invocation;
class TAO_ORB_Core;
class TAO_Stub;
class TAO_Profile;

/**
 * @class TAO_Optimized_Connection_Endpoint_Selector
 *
 * @brief More efficient connection strategy for endpoint selection.
 *
 * This strategy builds on the default by first querying the connection
 * cache for all potential endpoints before iterating over the list to
 * attempt to create new connections.
 */

class TAO_Strategies_Export TAO_Optimized_Connection_Endpoint_Selector
 : public TAO_Default_Endpoint_Selector
{
public:
  /// Constructor.
  TAO_Optimized_Connection_Endpoint_Selector (const ACE_Time_Value& tv);

  /// Destructor.
  virtual ~TAO_Optimized_Connection_Endpoint_Selector (void);

  static void hook (TAO_ORB_Core *,
                    TAO_Stub *,
                    bool &has_timeout,
                    ACE_Time_Value &tv);

  virtual void select_endpoint (TAO::Profile_Transport_Resolver *,
                                ACE_Time_Value *max_wait_time);
private:

  int check_profile (TAO_Profile *,
                     TAO::Profile_Transport_Resolver *);

  static ACE_Time_Value timeout_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif  /* TAO_OPTIMIZED_CONNECTION_ENDPOINT_SELECTOR_H */

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
#include "rtcorba_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/MProfile.h"
#include "tao/Invocation_Endpoint_Selectors.h"

/**
 * @class TAO_RT_Default_Endpoint_Selector
 *
 * @brief TAO_RT_Default_Endpoint_Selector
 *
 * This strategy is used when the only policy that might be set is the
 * private connection policy.
 *
 **/
class TAO_RTCORBA_Export TAO_RT_Default_Endpoint_Selector :
  public TAO_Default_Endpoint_Selector
{
public:
  virtual void select_endpoint (TAO_GIOP_Invocation *invocation
                                ACE_ENV_ARG_DECL);
};

// ****************************************************************

/**
 * @class TAO_Priority_Endpoint_Selector
 *
 * @brief TAO_Priority_Endpoint_Selector
 *
 * This strategy is used when RTCORBA::PriorityModelPolicy is
 * set and its value is RTCORBA::CLIENT_PROPAGATED.
 *
 */
class TAO_RTCORBA_Export TAO_Priority_Endpoint_Selector :
  public TAO_Default_Endpoint_Selector
{
public:
  /// Constructor.
  TAO_Priority_Endpoint_Selector (void);

  /// Destructor.
  virtual ~TAO_Priority_Endpoint_Selector (void);

  virtual void select_endpoint (TAO_GIOP_Invocation *invocation
                                ACE_ENV_ARG_DECL);
private:
  /// Helper for <select_endpoint>.
  int is_multihomed (TAO_Endpoint *endpoint);
};

// ****************************************************************

/**
 * @class TAO_Bands_Endpoint_Selector
 *
 * @brief TAO_Bands_Endpoint_Selector
 *
 * This strategy is used when
 * RTCORBA::PriorityBandedConnectionPolicy is set.
 *
 */
class TAO_RTCORBA_Export TAO_Bands_Endpoint_Selector :
  public TAO_Default_Endpoint_Selector
{
public:
  /// Constructor.
  TAO_Bands_Endpoint_Selector (void);

  /// Destructor.
  virtual ~TAO_Bands_Endpoint_Selector (void);

  virtual void select_endpoint (TAO_GIOP_Invocation *invocation
                                ACE_ENV_ARG_DECL);
};

// ****************************************************************

/**
 * @class TAO_Protocol_Endpoint_Selector
 *
 * @brief TAO_Protocol_Endpoint_Selector
 *
 * This strategy is used when only RTCORBA::ClientProtocolPolicy is
 * set or RTCORBA::ClientProtocolPolicy plus
 * RTCORBA::SERVER_DECLARED priority model.
 *
 */
class TAO_RTCORBA_Export TAO_Protocol_Endpoint_Selector :
  public TAO_Invocation_Endpoint_Selector
{
public:
  /// Constructor.
  TAO_Protocol_Endpoint_Selector (void);

  /// Destructor.
  virtual ~TAO_Protocol_Endpoint_Selector (void);

  virtual void select_endpoint (TAO_GIOP_Invocation *invocation
                                ACE_ENV_ARG_DECL);
  virtual void next (TAO_GIOP_Invocation *invocation
                     ACE_ENV_ARG_DECL);
  virtual void forward (TAO_GIOP_Invocation *invocation,
                        const TAO_MProfile &mprofile
                        ACE_ENV_ARG_DECL);
  virtual void success (TAO_GIOP_Invocation *invocation);
  virtual void close_connection (TAO_GIOP_Invocation *invocation);
};

// ****************************************************************

/**
 * @class TAO_Priority_Protocol_Selector
 *
 * @brief TAO_Priority_Protocol_Selector
 *
 * This strategy is used when RTCORBA::ClientProtocolPolicy is
 * set and the priority model is RTCORBA::CLIENT_PROPAGATED.
 *
 */
class TAO_RTCORBA_Export TAO_Priority_Protocol_Selector :
  public TAO_Protocol_Endpoint_Selector
{
public:
  /// Constructor.
  TAO_Priority_Protocol_Selector (void);

  /// Destructor.
  virtual ~TAO_Priority_Protocol_Selector (void);

protected:
  /// Helper for <select_endpoint>.
  int is_multihomed (TAO_Endpoint *endpoint);
};

// ****************************************************************

/**
 * @class TAO_Bands_Protocol_Selector
 *
 * @brief TAO_Bands_Protocol_Selector
 *
 * This strategy is used when both RTCORBA::ClientProtocolPolicy
 * and RTCORBA::PriorityBandedConnectionPolicy are set.
 *
 */
class TAO_RTCORBA_Export TAO_Bands_Protocol_Selector :
  public TAO_Protocol_Endpoint_Selector
{
public:
  /// Constructor.
  TAO_Bands_Protocol_Selector (void);

  /// Destructor.
  virtual ~TAO_Bands_Protocol_Selector (void);
};

#if defined (__ACE_INLINE__)
# include "RT_Invocation_Endpoint_Selectors.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif  /* TAO_RT_INVOCATION_ENDPOINT_SELECTOR_H */

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

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/MProfile.h"
#include "tao/Invocation_Endpoint_Selectors.h"

#if (TAO_HAS_RT_CORBA == 1)

/**
 * @class TAO_Priority_Endpoint_Selector
 *
 * @brief TAO_Priority_Endpoint_Selector
 *
 * This strategy is used when RTCORBA::PriorityModelPolicy is
 * set and its value is RTCORBA::CLIENT_PROPAGATED.
 *
 */
class TAO_Export TAO_Priority_Endpoint_Selector :
  public TAO_Default_Endpoint_Selector
{
public:
  /// Constructor.
  TAO_Priority_Endpoint_Selector (void);

  /// Destructor.
  virtual ~TAO_Priority_Endpoint_Selector (void);

  virtual void select_endpoint (TAO_GIOP_Invocation *invocation,
                                CORBA::Environment &ACE_TRY_ENV =
                                TAO_default_environment ());
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
class TAO_Export TAO_Bands_Endpoint_Selector :
  public TAO_Default_Endpoint_Selector
{
public:
  /// Constructor.
  TAO_Bands_Endpoint_Selector (void);

  /// Destructor.
  virtual ~TAO_Bands_Endpoint_Selector (void);

  virtual void select_endpoint (TAO_GIOP_Invocation *invocation,
                                CORBA::Environment &ACE_TRY_ENV =
                                TAO_default_environment ());
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
class TAO_Export TAO_Protocol_Endpoint_Selector :
  public TAO_Invocation_Endpoint_Selector
{
public:
  /// Constructor.
  TAO_Protocol_Endpoint_Selector (void);

  /// Destructor.
  virtual ~TAO_Protocol_Endpoint_Selector (void);

  virtual void select_endpoint (TAO_GIOP_Invocation *invocation,
                                CORBA::Environment &ACE_TRY_ENV =
                                TAO_default_environment ());
  virtual void next (TAO_GIOP_Invocation *invocation,
                     CORBA::Environment &ACE_TRY_ENV =
                     TAO_default_environment ());
  virtual void forward (TAO_GIOP_Invocation *invocation,
                        const TAO_MProfile &mprofile,
                        CORBA::Environment &ACE_TRY_ENV =
                        TAO_default_environment ());
  virtual void success (TAO_GIOP_Invocation *invocation);
  virtual void close_connection (TAO_GIOP_Invocation *invocation);

protected:
  /// Helper for <select_endpoint>.
  virtual void endpoint (TAO_GIOP_Invocation *invocation,
                         CORBA::Environment &ACE_TRY_ENV =
                         TAO_default_environment ());
};

// ****************************************************************

/**
 * @class TAO_Client_Priority_Policy_Selector
 *
 * @brief TAO_Client_Priority_Policy_Selector
 *
 * TAO::Client_Priority_Policy is deprecated.  Similar (and
 * beyond) functionality can be achieved through standard RTCORBA
 * policies.  See TAO Real-Time CORBA documentation for more
 * information.
 *
 */
class TAO_Export TAO_Client_Priority_Policy_Selector :
  public TAO_Invocation_Endpoint_Selector
{
public:
  /// Constructor.
  TAO_Client_Priority_Policy_Selector (void);

  /// Destructor.
  virtual ~TAO_Client_Priority_Policy_Selector (void);

  virtual void select_endpoint (TAO_GIOP_Invocation *invocation,
                                CORBA::Environment &ACE_TRY_ENV =
                                TAO_default_environment ());
  virtual void next (TAO_GIOP_Invocation *invocation,
                     CORBA::Environment &ACE_TRY_ENV =
                     TAO_default_environment ());
  virtual void forward (TAO_GIOP_Invocation *invocation,
                        const TAO_MProfile &mprofile,
                        CORBA::Environment &ACE_TRY_ENV =
                        TAO_default_environment ());
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
class TAO_Export TAO_Priority_Protocol_Selector :
  public TAO_Protocol_Endpoint_Selector
{
public:
  /// Constructor.
  TAO_Priority_Protocol_Selector (void);

  /// Destructor.
  virtual ~TAO_Priority_Protocol_Selector (void);

protected:
  ///
  virtual void endpoint (TAO_GIOP_Invocation *invocation,
                         CORBA::Environment &ACE_TRY_ENV =
                         TAO_default_environment ());

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
class TAO_Export TAO_Bands_Protocol_Selector :
  public TAO_Protocol_Endpoint_Selector
{
public:
  /// Constructor.
  TAO_Bands_Protocol_Selector (void);

  /// Destructor.
  virtual ~TAO_Bands_Protocol_Selector (void);

protected:
  virtual void endpoint (TAO_GIOP_Invocation *invocation,
                         CORBA::Environment &ACE_TRY_ENV =
                         TAO_default_environment ());
};

#endif /* TAO_HAS_RT_CORBA == 1 */

#if defined (__ACE_INLINE__)
# include "tao/RT_Invocation_Endpoint_Selectors.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif  /* TAO_RT_INVOCATION_ENDPOINT_SELECTOR_H */

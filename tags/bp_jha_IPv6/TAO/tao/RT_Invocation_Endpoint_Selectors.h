// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO
//
// = FILENAME
//   RT_Invocation_Endpoint_Selectors.h
//
// = DESCRIPTION
//   Strategies for selecting profile/endpoint from an IOR for making an
//   invocation.
//
// = AUTHOR
//   Priyanka Gontla <pgontla@ece.uci.edu>
//
// ============================================================================

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

class TAO_Export TAO_Priority_Endpoint_Selector :
  public TAO_Default_Endpoint_Selector
{
  // = TITLE
  //   Strategy for selecting endpoints based on the priority of the
  //   client thread making the invocation.
  //
  // = DESCRIPTION
  //   This strategy is used when RTCORBA::PriorityModelPolicy is
  //   set and its value is RTCORBA::CLIENT_PROPAGATED.
  //
public:
  TAO_Priority_Endpoint_Selector (void);
  // Constructor.

  virtual ~TAO_Priority_Endpoint_Selector (void);
  // Destructor.

  virtual void select_endpoint (TAO_GIOP_Invocation *invocation,
                                CORBA::Environment &ACE_TRY_ENV =
                                TAO_default_environment ());
private:
  int is_multihomed (TAO_Endpoint *endpoint);
  // Helper for <select_endpoint>.
};

// ****************************************************************

class TAO_Export TAO_Bands_Endpoint_Selector :
  public TAO_Default_Endpoint_Selector
{
  // = TITLE
  //   Strategy for selecting endpoints based on the priority range.
  //
  // = DESCRIPTION
  //   This strategy is used when
  //   RTCORBA::PriorityBandedConnectionPolicy is set.
  //
public:
  TAO_Bands_Endpoint_Selector (void);
  // Constructor.

  virtual ~TAO_Bands_Endpoint_Selector (void);
  // Destructor.

  virtual void select_endpoint (TAO_GIOP_Invocation *invocation,
                                CORBA::Environment &ACE_TRY_ENV =
                                TAO_default_environment ());
};

// ****************************************************************

class TAO_Export TAO_Protocol_Endpoint_Selector :
  public TAO_Invocation_Endpoint_Selector
{
  // = TITLE
  //   Strategy for selecting endpoints based on protocols.
  //
  // = DESCRIPTION
  //   This strategy is used when only RTCORBA::ClientProtocolPolicy is
  //   set or RTCORBA::ClientProtocolPolicy plus
  //   RTCORBA::SERVER_DECLARED priority model.
  //
public:
  TAO_Protocol_Endpoint_Selector (void);
  // Constructor.

  virtual ~TAO_Protocol_Endpoint_Selector (void);
  // Destructor.

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
  virtual void endpoint (TAO_GIOP_Invocation *invocation,
                         CORBA::Environment &ACE_TRY_ENV =
                         TAO_default_environment ());
  // Helper for <select_endpoint>.
};

// ****************************************************************

class TAO_Export TAO_Client_Priority_Policy_Selector :
  public TAO_Invocation_Endpoint_Selector
{
  // = TITLE
  //   Strategy for selecting endpoints when
  //   TAO::Client_Priority_Policy is set.
  //
  // = DESCRIPTION
  //   TAO::Client_Priority_Policy is deprecated.  Similar (and
  //   beyond) functionality can be achieved through standard RTCORBA
  //   policies.  See TAO Real-Time CORBA documentation for more
  //   information.
  //
public:
  TAO_Client_Priority_Policy_Selector (void);
  // Constructor.

  virtual ~TAO_Client_Priority_Policy_Selector (void);
  // Destructor.

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

class TAO_Export TAO_Priority_Protocol_Selector :
  public TAO_Protocol_Endpoint_Selector
{
  // = TITLE
  //   Strategy for selecting endpoints based on protocols and the
  //   priority of the client thread making the invocation.
  //
  // = DESCRIPTION
  //   This strategy is used when RTCORBA::ClientProtocolPolicy is
  //   set and the priority model is RTCORBA::CLIENT_PROPAGATED.
  //
public:
  TAO_Priority_Protocol_Selector (void);
  // Constructor.

  virtual ~TAO_Priority_Protocol_Selector (void);
  // Destructor.

protected:
  virtual void endpoint (TAO_GIOP_Invocation *invocation,
                         CORBA::Environment &ACE_TRY_ENV =
                         TAO_default_environment ());
  //

  int is_multihomed (TAO_Endpoint *endpoint);
  // Helper for <select_endpoint>.
};

// ****************************************************************

class TAO_Export TAO_Bands_Protocol_Selector :
  public TAO_Protocol_Endpoint_Selector
{
  // = TITLE
  //   Strategy for selecting endpoints based on protocols and a
  //   priority range.
  //
  // = DESCRIPTION
  //   This strategy is used when both RTCORBA::ClientProtocolPolicy
  //   and RTCORBA::PriorityBandedConnectionPolicy are set.
  //
public:
  TAO_Bands_Protocol_Selector (void);
  // Constructor.

  virtual ~TAO_Bands_Protocol_Selector (void);
  // Destructor.

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

// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO
//
// = FILENAME
//   Invocation_Endpoint_Selectors.h
//
// = DESCRIPTION
//   Strategies for selecting profile/endpoint from an IOR for making an
//   invocation. 
//
// = AUTHOR
//   Marina Spivak <marina@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_INVOCATION_ENDPOINT_SELECTOR_H
#define TAO_INVOCATION_ENDPOINT_SELECTOR_H
#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/MProfile.h"

class TAO_GIOP_Invocation;
class TAO_Endpoint;
class TAO_PrivateConnectionPolicy;
class TAO_PriorityModelPolicy;
class TAO_ClientProtocolPolicy;
class TAO_PriorityBandedConnectionPolicy;

class TAO_Export TAO_Endpoint_Selector_Factory
{
  // = TITLE
  //   Factory for initializing <Endpoint_Selection_State> and
  //   obtaining appropriate <Invocation_Endpoint_Selector>.
  //
  // = DESCRIPTION
  //   Used by Invocation classes to intialize its endpoint selection
  //   strategy and state based on the effective policies.  
  //   Endpoint selection strategies are stateless objects - all the
  //   state they need is contained by Invocation in
  //   <Endpoint_Selection_State>.  Thus, rather than allocating an
  //   endpoint selection strategy object for each Invocation, the
  //   factory simply returns the appropriate one from the
  //   set preallocated in the ORB_Core.  One endpoint selection
  //   strategy object can be used by many invocations concurrently.
  // 
public:
  TAO_Endpoint_Selector_Factory (void);
  // Constructor.

  ~TAO_Endpoint_Selector_Factory (void);
  // Destructor.

  void get_selector (TAO_GIOP_Invocation *invocation,
                     CORBA::Environment &ACE_TRY_ENV =
                     TAO_default_environment ());
  // Initialize Invocation's endpoint selection strategy and
  // state.

private:
  
  // = Helpers for <get_selector>.

#if (TAO_HAS_RT_CORBA == 1)

  void check_client_priority_policy (TAO_GIOP_Invocation *invocation,
                                     CORBA::Environment &ACE_TRY_ENV);
  // Gets the appropriate selector if TAO_HAS_CLIENT_PRIORITY_POLICY
  // is enabled. Also initializes endpoint selection state as
  // necessary.   WARNING: TAO::Client_Pririority_Policy is
  // deprecated.  See TAO RTCORBA documentation for more details.

  void init_client_protocol (TAO_GIOP_Invocation *invocation,
                             CORBA::Environment &ACE_TRY_ENV);
  // Initializes RTCORBA::ClientProtocolPolicy in the endpoint
  // selection state. 

  void init_bands (TAO_GIOP_Invocation *invocation,
                   CORBA::Environment &ACE_TRY_ENV);
  // Initializes RTCORBA::PriorityBandsPolicy in the endpoint
  // selection state. 

#endif /* TAO_HAS_RT_CORBA == 1 */
};

// ****************************************************************

#if (TAO_HAS_RT_CORBA == 1)

class TAO_Export TAO_Endpoint_Selection_State
{
  // = TITLE
  //   
  //
public:

  // = Initialization and termination.

  TAO_Endpoint_Selection_State (void);
  // Constructor.

  ~TAO_Endpoint_Selection_State (void);
  // Destructor.

  // = State.

  // RTCORBA policies that affect profile/endpoint selection.
  TAO_PriorityModelPolicy *priority_model_policy_;
  // Can be 0.
  TAO_ClientProtocolPolicy *client_protocol_policy_;
  // Should never be 0 because of ORB default.
  TAO_PrivateConnectionPolicy *private_connection_;
  // Can be 0.
  TAO_PriorityBandedConnectionPolicy *bands_policy_;
  // Can be 0.

  CORBA::ULong client_protocol_index_;
  // Index into the RTCORBA::ProtocolList of the client protocol
  // policy, identifying the protocol we should attempt using next.

  int valid_endpoint_found_;
  // Flag indicating whether at least one endpoint satisfying all the
  // policies was found during the lifetime of this object.

  CORBA::Short client_priority_;
  // If using RTCORBA::CLIENT_PROPAGATED priority model, cache our
  // client priority here.

  CORBA::Short min_priority_;
  CORBA::Short max_priority_;

};
#endif /* TAO_HAS_RT_CORBA == 1 */

// ****************************************************************

class TAO_Export TAO_Invocation_Endpoint_Selector
{
  // = TITLE
  //   Defines the Endpoint interface in the Pluggable Protocol
  //   framework. 
  //
  // = DESCRIPTION
  //   Lightweight encapsulation of addressing information for a
  //   single acceptor endpoint.  In other words, Endpoint represents
  //   a single point of contact for the server, and is the smallest
  //   unit of addressing information necessary for a client to connect
  //   to a server. 
  //   A Profile contains one or more Endpoints, i.e., knows of
  //   one or more ways to contact server(s).  
  //
public:
  TAO_Invocation_Endpoint_Selector (void);
  // Constructor.

  virtual ~TAO_Invocation_Endpoint_Selector (void);
  // Destructor.

  virtual void select_endpoint (TAO_GIOP_Invocation *invocation,
                                CORBA::Environment &ACE_TRY_ENV =
                                TAO_default_environment ()) = 0;
  //
  
  virtual void next (TAO_GIOP_Invocation *invocation,
                     CORBA::Environment &ACE_TRY_ENV =
                     TAO_default_environment ()) = 0;
  //

  virtual void forward (TAO_GIOP_Invocation *invocation,
                        const TAO_MProfile &mprofile,
                        CORBA::Environment &ACE_TRY_ENV = 
                        TAO_default_environment ()) = 0;
  //

  virtual void success (TAO_GIOP_Invocation *invocation) = 0;
  //

  virtual void close_connection (TAO_GIOP_Invocation *invocation) = 0;
  //
};

// ****************************************************************

class TAO_Export TAO_Default_Endpoint_Selector : 
  public TAO_Invocation_Endpoint_Selector
{
public:
  TAO_Default_Endpoint_Selector (void);
  // Constructor.

  virtual ~TAO_Default_Endpoint_Selector (void);
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

#if (TAO_HAS_RT_CORBA == 1)

class TAO_Export TAO_Priority_Endpoint_Selector : 
  public TAO_Default_Endpoint_Selector
{
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
# include "tao/Invocation_Endpoint_Selectors.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif  /* TAO_INVOCATION_ENDPOINT_SELECTOR_H */

// -*- C++ -*-
//
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO_SSLIOP
//
// = FILENAME
//     SSLIOP_Connector.h
//
// = DESCRIPTION
//     SSLIOP specific connector processing
//
// = AUTHOR
//     Carlos O'Ryan <coryan@ece.uci.edu>
//     Ossama Othman <ossama@ece.uci.edu>
//
// ============================================================================

#ifndef TAO_SSLIOP_CONNECTOR_H
#define TAO_SSLIOP_CONNECTOR_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SSL/SSL_SOCK_Connector.h"
#include "IIOP_SSL_Connector.h"
#include "SSLIOP_Connect.h"
#include "tao/Resource_Factory.h"


class TAO_Base_Connection_Property;
// ****************************************************************

class TAO_SSLIOP_Export TAO_SSLIOP_Connect_Creation_Strategy
  : public ACE_Creation_Strategy<TAO_SSLIOP_Client_Connection_Handler>
{
  // = TITLE
  //   Helper creation strategy
  //
  // = DESCRIPTION
  //   Creates SSLIOP_Client_Connection_Handler objects but satisfies
  //   the interface required by the
  //   ACE_Creation_Strategy<TAO_SSLIOP_Client_Connection_Handler>.
  //
public:
  TAO_SSLIOP_Connect_Creation_Strategy (ACE_Thread_Manager * = 0,
                                        TAO_ORB_Core* orb_core = 0,
                                        void *arg = 0);
  // Constructor. <arg> parameter is used to pass any special
  // state/info to the service handler upon creation.  Currently used
  // to pass protocol configuration properties.

  virtual int make_svc_handler (TAO_SSLIOP_Client_Connection_Handler *&sh);
  // Makes TAO_SSLIOP_Client_Connection_Handlers

private:
  TAO_ORB_Core* orb_core_;
  // The ORB

  void *arg_;
  // Some info/state to be passed to the service handler we create.
};

// ****************************************************************

class TAO_SSLIOP_Export TAO_SSLIOP_Connector : public TAO_IIOP_SSL_Connector
{
  // = TITLE
  //   IIOP-specific Connector bridge for pluggable protocols.
  //
  // = DESCRIPTION
  //   Concrete instance of the TAO_Connector class.  Responsible
  //   for establishing a connection with a server and is called from
  //   the Connector_Registry.
  //
public:
  // = Initialization and termination methods.
  TAO_SSLIOP_Connector (int no_protection);
  // Constructor.

  // = The TAO_Connector methods, please check the documentation on
  // Pluggable.h
  virtual int open (TAO_ORB_Core *orb_core);
  virtual int close (void);
  virtual int connect (TAO_Connection_Descriptor_Interface *desc,
                       TAO_Transport *&transport,
                       ACE_Time_Value *max_wait_time,
                       CORBA::Environment &ACE_TRY_ENV);
  virtual TAO_Profile *create_profile (TAO_InputCDR& cdr);


protected:
  // = More TAO_Connector methods, please check the documentation on
  //   Pluggable.h
  virtual void make_profile (const char *endpoint,
                             TAO_Profile *&,
                             CORBA::Environment &ACE_TRY_ENV =
                               TAO_default_environment ());

public:

  typedef ACE_NOOP_Concurrency_Strategy<TAO_SSLIOP_Client_Connection_Handler>
        TAO_SSLIOP_NULL_ACTIVATION_STRATEGY;

  typedef ACE_Connect_Strategy<TAO_SSLIOP_Client_Connection_Handler,
                               ACE_SSL_SOCK_CONNECTOR>
        TAO_SSLIOP_CONNECT_STRATEGY;

  typedef ACE_Strategy_Connector<TAO_SSLIOP_Client_Connection_Handler,
                                 ACE_SSL_SOCK_CONNECTOR>
        TAO_SSLIOP_BASE_CONNECTOR;

private:

  int no_protection_;
  // If zero, connect to IIOP over SSL port by default.
  // Otherwise, connect to the insecure IIOP port.

  TAO_SSLIOP_NULL_ACTIVATION_STRATEGY null_activation_strategy_;
  // Our activation strategy

  TAO_SSLIOP_CONNECT_STRATEGY connect_strategy_;
  // Our connect strategy

  TAO_SSLIOP_BASE_CONNECTOR base_connector_;
  // The connector initiating connection requests for IIOP.
};


#include "ace/post.h"

#endif  /* TAO_SSLIOP_CONNECTOR_H */

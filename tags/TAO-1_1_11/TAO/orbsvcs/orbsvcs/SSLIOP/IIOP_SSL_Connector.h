// -*- C++ -*-

//=============================================================================
/**
 *  @file    IIOP_SSL_Connector.h
 *
 *  $Id$
 *
 *  IIOP specific connector processing -- SSL aware version
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_IIOP_SSL_CONNECTOR_H
#define TAO_IIOP_SSL_CONNECTOR_H

#include "ace/pre.h"

#include "SSLIOP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "IIOP_SSL_Connect.h"
#include "tao/IIOP_Connector.h"


// ****************************************************************
/**
 * @class TAO_IIOP_SSL_Connect_Creation_Strategy
 *
 * @brief Helper creation strategy
 *
 * Creates UIOP_Client_Connection_Handler objects but satisfies
 * the interface required by the
 * ACE_Creation_Strategy<TAO_IIOP_SSL_Client_Connection_Handler>
 */
class TAO_SSLIOP_Export TAO_IIOP_SSL_Connect_Creation_Strategy
  : public ACE_Creation_Strategy<TAO_IIOP_SSL_Client_Connection_Handler>
{
public:
  /**
   * Constructor. <arg> parameter is used to pass any special
   * state/info to the service handler upon creation.  Currently used
   * by IIOP and UIOP to pass protocol configuration properties.
   */
  TAO_IIOP_SSL_Connect_Creation_Strategy (ACE_Thread_Manager * = 0,
                                          TAO_ORB_Core* orb_core = 0,
                                          void *arg = 0,
                                          CORBA::Boolean flag = 0);

  /// Default destructor g++, 2.7.2 appear to need it.
  ~TAO_IIOP_SSL_Connect_Creation_Strategy (void);

  /// Makes TAO_IIOP_SSL_Client_Connection_Handlers
  virtual int make_svc_handler (TAO_IIOP_SSL_Client_Connection_Handler *&sh);

private:

  /// The ORB core.
  TAO_ORB_Core* orb_core_;

  /// Some info/state to be passed to the service handler we create.
  void *arg_;

  /// Are we using GIOP lite?
  CORBA::Boolean lite_flag_;
};

// ****************************************************************

/**
 * @class TAO_IIOP_SSL_Connector
 *
 * @brief IIOP-specific Connector (SSL aware) bridge for pluggable
 * protocols.
 *
 * Concrete instance of the TAO_Connector class.  Responsible
 * for establishing a connection with a server and is called from the
 * Connector_Registry.
 */
class TAO_SSLIOP_Export TAO_IIOP_SSL_Connector : public TAO_IIOP_Connector
{
public:

  /// Constructor.
  TAO_IIOP_SSL_Connector (CORBA::Boolean flag = 0);

  /// Destructor.
  ~TAO_IIOP_SSL_Connector (void);

  int open (TAO_ORB_Core *orb_core);
  int close (void);
  int connect (TAO_Connection_Descriptor_Interface *desc,
               TAO_Transport *&transport,
               ACE_Time_Value *max_wait_time,
               CORBA::Environment &ACE_TRY_ENV);
  int preconnect (const char *preconnections);

public:

  typedef ACE_NOOP_Concurrency_Strategy<TAO_IIOP_SSL_Client_Connection_Handler>
          TAO_NULL_ACTIVATION_STRATEGY;

  typedef ACE_Connect_Strategy<TAO_IIOP_SSL_Client_Connection_Handler,
                               ACE_SOCK_CONNECTOR>
          TAO_CONNECT_STRATEGY ;

  typedef ACE_Strategy_Connector<TAO_IIOP_SSL_Client_Connection_Handler,
                                 ACE_SOCK_CONNECTOR>
          TAO_IIOP_SSL_BASE_CONNECTOR;

private:

  /// Our activation strategy
  TAO_NULL_ACTIVATION_STRATEGY null_activation_strategy_;

  /// Our connect strategy
  TAO_CONNECT_STRATEGY connect_strategy_;

  /// The connector initiating connection requests for IIOP.
  TAO_IIOP_SSL_BASE_CONNECTOR base_connector_;

};

#include "ace/post.h"

#endif  /* TAO_IIOP_SSL_CONNECTOR_H */

// -*- C++ -*-

//=============================================================================
/**
 *  @file     SSLIOP_Connector.h
 *
 *  $Id$
 *
 *  SSLIOP specific connector processing
 *
 *  @author Carlos O'Ryan <coryan@uci.edu>
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_SSLIOP_CONNECTOR_H
#define TAO_SSLIOP_CONNECTOR_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SSL/SSL_SOCK_Connector.h"
#include "IIOP_SSL_Connector.h"
#include "SSLIOP_Connection_Handler.h"


/// Forward declarations.
class TAO_Base_Transport_Property;
class TAO_SSLIOP_Endpoint;
class TAO_SSLIOP_Credentials;

/**
 * @class TAO_SSLIOP_Connector
 *
 * @brief SSLIOP-specific Connector bridge for pluggable protocols.
 *
 * Concrete instance of the TAO_Connector class.  Responsible
 * for establishing a connection with a server and is called from
 * the Connector_Registry.
 */
class TAO_SSLIOP_Export TAO_SSLIOP_Connector : public TAO_IIOP_SSL_Connector
{
public:
  /// Constructor.
  TAO_SSLIOP_Connector (Security::QOP qop);

  /**
   * @name The TAO_Connector methods
   *
   * Please check the documentation in Pluggable.h
   */
  //@{
  virtual int open (TAO_ORB_Core *orb_core);
  virtual int close (void);
  virtual int connect (TAO_GIOP_Invocation *invocation,
                       TAO_Transport_Descriptor_Interface *desc
                       TAO_ENV_ARG_DECL);
  virtual TAO_Profile *create_profile (TAO_InputCDR& cdr);
  //@}

protected:

  /**
   * @name More TAO_Connector methods
   *
   * Please check the documentation in Pluggable.h.
   */
  virtual TAO_Profile * make_profile (TAO_ENV_SINGLE_ARG_DECL);

  /// IIOP-specific connection establishment.
  /**
   * @note The IIOP endpoint is extracted from the SSLIOP endpoint.
   */
  int iiop_connect (TAO_SSLIOP_Endpoint *ssliop_endpoint,
                    TAO_GIOP_Invocation *invocation
                    TAO_ENV_ARG_DECL);

  /// SSLIOP-specific connection establishment.
  int ssliop_connect (TAO_SSLIOP_Endpoint *ssliop_endpoint,
                      Security::QOP qop,
                      const Security::EstablishTrust &trust,
                      TAO_GIOP_Invocation *invocation,
                      TAO_Transport_Descriptor_Interface *desc
                      TAO_ENV_ARG_DECL);

  /// Retrieve SSLIOP credentials from the policy overrides list and
  /// set up the underlying SSL connection to use the X.509
  /// certificates stored within them.
  TAO_SSLIOP_Credentials * retrieve_credentials (TAO_Stub *stub,
                                                 SSL *ssl
                                                 TAO_ENV_ARG_DECL);

public:


  typedef TAO_Connect_Concurrency_Strategy<TAO_SSLIOP_Connection_Handler>
          TAO_SSLIOP_CONNECT_CONCURRENCY_STRATEGY;

  typedef TAO_Connect_Creation_Strategy<TAO_SSLIOP_Connection_Handler>
          TAO_SSLIOP_CONNECT_CREATION_STRATEGY;

  typedef ACE_Connect_Strategy<TAO_SSLIOP_Connection_Handler,
                               ACE_SSL_SOCK_CONNECTOR>
          TAO_SSLIOP_CONNECT_STRATEGY ;

  typedef ACE_Strategy_Connector<TAO_SSLIOP_Connection_Handler,
                                 ACE_SSL_SOCK_CONNECTOR>
          TAO_SSLIOP_BASE_CONNECTOR;

private:

  /// If zero, connect to IIOP over SSL port by default.
  /// Otherwise, connect to the insecure IIOP port.
  Security::QOP qop_;

  /// Our connect strategy
  TAO_SSLIOP_CONNECT_STRATEGY connect_strategy_;

  /// The connector initiating connection requests for IIOP.
  TAO_SSLIOP_BASE_CONNECTOR base_connector_;

  /// State that will be passed to each SSLIOP connection handler upon
  /// creation.
  TAO_SSLIOP_Connection_Handler_State handler_state_;

};


#include "ace/post.h"

#endif  /* TAO_SSLIOP_CONNECTOR_H */

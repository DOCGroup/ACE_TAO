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

#include "IIOP_SSL_Connection_Handler.h"
#include "SSLIOP_Connection_Handler.h"
#include "tao/IIOP_Connector.h"


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
  int connect (TAO_GIOP_Invocation *invocation,
               TAO_Transport_Descriptor_Interface *desc
               ACE_ENV_ARG_DECL);

public:

  typedef TAO_Connect_Concurrency_Strategy<TAO_IIOP_SSL_Connection_Handler>
          TAO_IIOP_SSL_CONNECT_CONCURRENCY_STRATEGY;

  typedef TAO_Connect_Creation_Strategy<TAO_IIOP_SSL_Connection_Handler>
          TAO_IIOP_SSL_CONNECT_CREATION_STRATEGY;

  typedef ACE_Connect_Strategy<TAO_IIOP_SSL_Connection_Handler,
                               ACE_SOCK_CONNECTOR>
          TAO_IIOP_SSL_CONNECT_STRATEGY ;

  typedef ACE_Strategy_Connector<TAO_IIOP_SSL_Connection_Handler,
                                 ACE_SOCK_CONNECTOR>
          TAO_IIOP_SSL_BASE_CONNECTOR;

private:

  /// Our connect strategy
  TAO_IIOP_SSL_CONNECT_STRATEGY connect_strategy_;

  /// The connector initiating connection requests for IIOP_SSL.
  TAO_IIOP_SSL_BASE_CONNECTOR base_connector_;

  /// State that will be passed to each SSLIOP connection handler upon
  /// creation.
  TAO_SSLIOP_Connection_Handler_State handler_state_;

};

#include "ace/post.h"

#endif  /* TAO_IIOP_SSL_CONNECTOR_H */

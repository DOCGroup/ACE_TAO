// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    IIOP_Connector.h
 *
 *  $Id$
 *
 *  IIOP specific connector processing
 *
 *
 *  @author Fred Kuhns <fredk@cs.wustl.edu>
 *  @author Ossama Othman <ossama@uci.edu>
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_IIOP_CONNECTOR_H
#define TAO_IIOP_CONNECTOR_H
#include "ace/pre.h"
#include "ace/SOCK_Connector.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_IIOP_Endpoint;

#include "ace/Connector.h"
#include "Transport_Connector.h"
#include "tao/Connector_Impl.h"
#include "tao/IIOP_Connection_Handler.h"

// ****************************************************************

/**
 * @class TAO_IIOP_Connector
 *
 * @brief IIOP-specific Connector bridge for pluggable protocols.
 *
 * Concrete instance of the TAO_Connector class.  Responsible
 * for establishing a connection with a server and is called from the
 * Connector_Registory.
 */
class TAO_Export TAO_IIOP_Connector : public TAO_Connector
{
public:
  // = Initialization and termination methods.
  /// Constructor.
  TAO_IIOP_Connector (CORBA::Boolean flag = 0);

  /// Destructor.
  ~TAO_IIOP_Connector (void);

  // = The TAO_Connector methods, please check the documentation on
  // Transport_Connector.h
  int open (TAO_ORB_Core *orb_core);
  int close (void);
  TAO_Profile *create_profile (TAO_InputCDR& cdr);

  virtual int check_prefix (const char *endpoint);

  virtual char object_key_delimiter (void) const;

public:

  typedef TAO_Connect_Concurrency_Strategy<TAO_IIOP_Connection_Handler>
          TAO_IIOP_CONNECT_CONCURRENCY_STRATEGY;

  typedef TAO_Connect_Creation_Strategy<TAO_IIOP_Connection_Handler>
          TAO_IIOP_CONNECT_CREATION_STRATEGY;

  typedef ACE_Connect_Strategy<TAO_IIOP_Connection_Handler,
                               ACE_SOCK_CONNECTOR>
          TAO_IIOP_CONNECT_STRATEGY ;

  typedef ACE_Strategy_Connector<TAO_IIOP_Connection_Handler,
                                 ACE_SOCK_CONNECTOR>
          TAO_IIOP_BASE_CONNECTOR;

protected:

  // = The TAO_Connector methods, please check the documentation on
  // Transport_Connector.h
  int set_validate_endpoint (TAO_Endpoint *ep);

  int make_connection (TAO_GIOP_Invocation *invocation,
                       TAO_Transport_Descriptor_Interface *desc,
                       ACE_Time_Value *timeout = 0);

  TAO_Transport *make_connection (TAO::Profile_Connection_Resolver *r,
                                  TAO_Transport_Descriptor_Interface &desc,
                                  ACE_Time_Value *timeout = 0);

  /// More TAO_Connector methods, please check the documentation on
  /// Transport_Connector.h
  virtual TAO_Profile *make_profile (ACE_ENV_SINGLE_ARG_DECL);

  /// Obtain tcp properties that must be used by this connector, i.e.,
  /// initialize <tcp_properties_>.
  int init_tcp_properties (void);

protected:

  /// TCP configuration properties to be used for all
  /// connections established by this connector.
  TAO_IIOP_Properties tcp_properties_;

  /// Do we need to use a GIOP_Lite for sending messages?
  CORBA::Boolean lite_flag_;


private:

  /// Return the remote endpoint, a helper function
  TAO_IIOP_Endpoint *remote_endpoint (TAO_Endpoint *ep);

private:

  /// Our connect strategy
  TAO_IIOP_CONNECT_STRATEGY connect_strategy_;

  /// The connector initiating connection requests for IIOP.
  TAO_IIOP_BASE_CONNECTOR base_connector_;
};


#include "ace/post.h"
#endif  /* TAO_IIOP_CONNECTOR_H */

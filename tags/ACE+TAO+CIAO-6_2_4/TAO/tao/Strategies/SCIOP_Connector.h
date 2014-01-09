// -*- C++ -*-

// $Id$
//=============================================================================
/**
 *  @file    SCIOP_Connector.h
 *
 *  SCIOP specific connector processing
 *
 *  @author  Jason Cohen, Lockheed Martin ATL  <jcohen@atl.lmco.com>
 *  @author  Keith O'Hara, Lockheed Martin ATL
 */
//=============================================================================

#ifndef TAO_SCIOP_CONNECTOR_H
#define TAO_SCIOP_CONNECTOR_H

#include /**/ "ace/pre.h"
#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if TAO_HAS_SCIOP == 1

#include "tao/Transport_Connector.h"
#include "tao/Connector_Impl.h"
#include "tao/Strategies/SCIOP_Connection_Handler.h"
#include "ace/SOCK_SEQPACK_Connector.h"
#include "ace/Connector.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_SCIOP_Endpoint;

// ****************************************************************

/**
 * @class TAO_SCIOP_Connector
 *
 * @brief SCIOP-specific Connector bridge for pluggable protocols.
 *
 * Concrete instance of the TAO_Connector class.  Responsible
 * for establishing a connection with a server and is called from the
 * Connector_Registory.
 */
class TAO_Strategies_Export TAO_SCIOP_Connector : public TAO_Connector
{
public:
  // = Initialization and termination methods.
  /// Constructor.
  TAO_SCIOP_Connector (void);

  /// Destructor.
  ~TAO_SCIOP_Connector (void);

  // = The TAO_Connector methods, please check the documentation on
  // Transport_Connector.h
  int open (TAO_ORB_Core *orb_core);
  int close (void);
  TAO_Profile *create_profile (TAO_InputCDR& cdr);

  virtual int check_prefix (const char *endpoint);

  virtual char object_key_delimiter (void) const;

public:

  typedef TAO_Connect_Concurrency_Strategy<TAO_SCIOP_Connection_Handler>
          TAO_SCIOP_CONNECT_CONCURRENCY_STRATEGY;

  typedef TAO_Connect_Creation_Strategy<TAO_SCIOP_Connection_Handler>
          TAO_SCIOP_CONNECT_CREATION_STRATEGY;

  typedef ACE_Connect_Strategy<TAO_SCIOP_Connection_Handler,
                               ACE_SOCK_SEQPACK_CONNECTOR>
          TAO_SCIOP_CONNECT_STRATEGY;

  typedef ACE_Strategy_Connector<TAO_SCIOP_Connection_Handler,
                                 ACE_SOCK_SEQPACK_CONNECTOR>
          TAO_SCIOP_BASE_CONNECTOR;

protected:

  // = The TAO_Connector methods, please check the documentation on
  // Transport_Connector.h
  int set_validate_endpoint (TAO_Endpoint *ep);

  TAO_Transport *make_connection (TAO::Profile_Transport_Resolver *r,
                                  TAO_Transport_Descriptor_Interface &desc,
                                  ACE_Time_Value *timeout = 0);

  /// More TAO_Connector methods, please check the documentation on
  /// Transport_Connector.h
  virtual TAO_Profile *make_profile (void);

  /// Cancel the passed cvs handler from the connector
  int cancel_svc_handler (TAO_Connection_Handler * svc_handler);

private:

  /// Return the remote endpoint, a helper function
  TAO_SCIOP_Endpoint *remote_endpoint (TAO_Endpoint *ep);

  /// Try to make a connection to the next endpoint in the list.
  TAO_Transport *make_connection_i (TAO::Profile_Transport_Resolver *r,
                                    TAO_Transport_Descriptor_Interface &desc,
                                    ACE_Time_Value *timeout,
                                    TAO_SCIOP_Endpoint *sciop_endpoint);

private:

  /// Our connect strategy
  TAO_SCIOP_CONNECT_STRATEGY connect_strategy_;

  /// The connector initiating connection requests for SCIOP.
  TAO_SCIOP_BASE_CONNECTOR base_connector_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_SCIOP == 1 */


#include /**/ "ace/post.h"

#endif  /* TAO_SCIOP_CONNECTOR_H */

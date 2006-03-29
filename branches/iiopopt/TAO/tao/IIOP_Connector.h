// -*- C++ -*-

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

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_HAS_IIOP) && (TAO_HAS_IIOP != 0)

#include "tao/Transport_Connector.h"
#include "tao/Connector_Impl.h"
#include "tao/IIOP_Connection_Handler.h"

#include "ace/SOCK_Connector.h"
#include "ace/Connector.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_IIOP_Endpoint;

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

  //@@ TAO_CONNECTOR_SPL_PUBLIC_METHODS_COPY_HOOK_START

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
  //@@ TAO_CONNECTOR_SPL_PUBLIC_METHODS_COPY_HOOK_END

protected:

  // = The TAO_Connector methods, please check the documentation on
  // Transport_Connector.h
  int set_validate_endpoint (TAO_Endpoint *ep);

  TAO_Transport *make_connection (TAO::Profile_Transport_Resolver *r,
                                  TAO_Transport_Descriptor_Interface &desc,
                                  ACE_Time_Value *timeout = 0);

  /// More TAO_Connector methods, please check the documentation on
  /// Transport_Connector.h
  virtual TAO_Profile *make_profile (ACE_ENV_SINGLE_ARG_DECL);

  /// Cancel the passed cvs handler from the connector
  virtual int cancel_svc_handler (TAO_Connection_Handler * svc_handler);

/*
 * Hook to copy over the protected and private data from this class to
 * base Connector class.
 */
//@@ TAO_CONNECTOR_SPL_COPY_HOOK_START

protected:

  /// Do we need to use a GIOP_Lite for sending messages?
  const bool lite_flag_;

private:

  /// Return the remote endpoint, a helper function
  TAO_IIOP_Endpoint *remote_endpoint (TAO_Endpoint *ep);

private:

  /// Our connect strategy
  TAO_IIOP_CONNECT_STRATEGY connect_strategy_;

  /// The connector initiating connection requests for IIOP.
  TAO_IIOP_BASE_CONNECTOR base_connector_;

//@@ TAO_CONNECTOR_SPL_COPY_HOOK_END

};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_IIOP && TAO_HAS_IIOP != 0 */

#include /**/ "ace/post.h"
#endif  /* TAO_IIOP_CONNECTOR_H */

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
  TAO_IIOP_Connector (void);

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
          TAO_IIOP_CONNECT_STRATEGY;

  typedef ACE_Strategy_Connector<TAO_IIOP_Connection_Handler,
                                 ACE_SOCK_CONNECTOR>
          TAO_IIOP_BASE_CONNECTOR;
  //@@ TAO_CONNECTOR_SPL_PUBLIC_METHODS_COPY_HOOK_END

protected:
  /// A flag indicating the actual connector supports parallel
  /// connection attempts. The base implementation always returns
  /// 0. Override to return non-zero if parallel connection attempts
  /// may be tried.
  virtual int supports_parallel_connects (void) const;

  // = The TAO_Connector methods, please check the documentation on
  // Transport_Connector.h
  int set_validate_endpoint (TAO_Endpoint *ep);

  virtual TAO_Transport *make_connection (
      TAO::Profile_Transport_Resolver *r,
      TAO_Transport_Descriptor_Interface &desc,
      ACE_Time_Value *timeout = 0);

  virtual TAO_Transport *make_parallel_connection (
      TAO::Profile_Transport_Resolver *r,
                                  TAO_Transport_Descriptor_Interface &desc,
                                  ACE_Time_Value *timeout = 0);

  /// More TAO_Connector methods, please check the documentation on
  /// Transport_Connector.h
  virtual TAO_Profile *make_profile (void);

  /// Cancel the passed cvs handler from the connector
  virtual int cancel_svc_handler (TAO_Connection_Handler * svc_handler);

/*
 * Hook to copy over the protected and private data from this class to
 * base Connector class.
 */
//@@ TAO_CONNECTOR_SPL_COPY_HOOK_START

private:
  /// This is the first half of making a connection. Both make_connection
  /// and make_parallel_connection will start out using begin_connection.
  int begin_connection (TAO_IIOP_Connection_Handler *&svc_handler,
                        TAO::Profile_Transport_Resolver *r,
                        TAO_IIOP_Endpoint *endpoint,
                        ACE_Time_Value *timeout = 0);

  /// This is the second half of making a connection when several endpoints
  /// are involved. This works with modified wait strategies to wait for one
  /// of many transports, and when once completes it will cancel the rest.
  /// The winning transport is returned.
  TAO_Transport *complete_connection (int result,
                                      TAO_Transport_Descriptor_Interface &desc,
                                      TAO_IIOP_Connection_Handler **&sh_list,
                                      TAO_IIOP_Endpoint **ep_list,
                                      unsigned count,
                                      TAO::Profile_Transport_Resolver *r,
                                      TAO_LF_Multi_Event *mev,
                                      ACE_Time_Value *timeout = 0);



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

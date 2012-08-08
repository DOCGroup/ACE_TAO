// -*- C++ -*-

//=============================================================================
/**
 *  @file     UIPMC_Mcast_Connection_Handler.h
 *
 *  $Id$
 *
 *  @author Vadym Ridosh <vridosh@prismtech.com>
 */
//=============================================================================

#ifndef TAO_UIPMC_MCAST_CONNECTION_HANDLER_H
#define TAO_UIPMC_MCAST_CONNECTION_HANDLER_H

#include /**/ "ace/pre.h"

#include "orbsvcs/PortableGroup/portablegroup_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/PortableGroup/UIPMC_Transport.h"

#include "tao/Wait_Strategy.h"
#include "tao/Connection_Handler.h"

#include "ace/Acceptor.h"
#include "ace/Reactor.h"
#include "ace/SOCK_Dgram_Mcast.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// This connection handler.
typedef ACE_Svc_Handler<ACE_SOCK_DGRAM_MCAST, ACE_NULL_SYNCH>
        TAO_UIPMC_MCAST_SVC_HANDLER;

#if defined ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT
template class TAO_PortableGroup_Export ACE_Svc_Handler<ACE_SOCK_DGRAM_MCAST, ACE_NULL_SYNCH>;
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT */

// ****************************************************************

/**
 * @class TAO_UIPMC_Mcast_Connection_Handler
 *
 * @brief  Handles requests on a single connection.
 *
 * Since MIOP is asymmetric then this Connection handler
 * is for only use in the Acceptor.
 */
class TAO_PortableGroup_Export TAO_UIPMC_Mcast_Connection_Handler :
  public TAO_UIPMC_MCAST_SVC_HANDLER,
  public TAO_Connection_Handler
{

public:

  TAO_UIPMC_Mcast_Connection_Handler (ACE_Thread_Manager* t = 0);

  /// Constructor. arg parameter is used by the Acceptor to pass the
  /// protocol configuration properties for this connection.
  TAO_UIPMC_Mcast_Connection_Handler (TAO_ORB_Core *orb_core);

  /// Destructor.
  ~TAO_UIPMC_Mcast_Connection_Handler (void);

  //@{
  /** @name Connection Handler overloads
   */
  virtual int open_handler (void *v);
  //@}

  /// Close called by the Acceptor or Connector when connection
  /// establishment fails.
  int close (u_long = 0);

  //@{
  /** @name Event Handler overloads
   */
  virtual int resume_handler (void);
  virtual int close_connection (void);
  virtual int handle_input (ACE_HANDLE);
  virtual int handle_output (ACE_HANDLE);
  virtual int handle_close (ACE_HANDLE, ACE_Reactor_Mask);
  virtual int handle_timeout (const ACE_Time_Value &current_time,
                              const void *act = 0);
  virtual int open (void *);
  //@}

  /// Add ourselves to Cache.
  int add_transport_to_cache (void);

  // UIPMC Additions - Begin
  const ACE_INET_Addr &addr (void);

  void addr (const ACE_INET_Addr &addr);

  const ACE_INET_Addr &local_addr (void);

  void local_addr (const ACE_INET_Addr &addr);

  /// This is only to be able to use client and server
  /// connection handlers in the same way in transport.
  ssize_t send (const iovec iov[],
                int n,
                const ACE_Addr &addr,
                int flags = 0) const;

  /// Set this to listen on all interfaces
  void listen_on_all(bool value);

  // UIPMC Additions - End

protected:

  // UIPMC Additions - Begin

  // This is always the remote address
  ACE_INET_Addr addr_;

  // This is always the local address for the connector
  ACE_INET_Addr local_addr_;

  // Should we listen on all interfaces
  bool listen_on_all_;

  // UIPMC Additions - End

  //@{
  /**
   * @name TAO_Connection Handler overloads
   */
  virtual int release_os_resources (void);
  virtual int handle_write_ready (const ACE_Time_Value *timeout);
  //@}

};

// Transport for this handler.
typedef TAO_UIPMC_Transport<TAO_UIPMC_Mcast_Connection_Handler>
        UIPMC_MULTICAST_TRANSPORT;

#if defined ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT
template class TAO_PortableGroup_Export TAO_UIPMC_Transport<TAO_UIPMC_Mcast_Connection_Handler>;
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT */

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_UIPMC_MCAST_CONNECTION_HANDLER_H */

// -*- C++ -*-

//=============================================================================
/**
 *  @file     UIPMC_Connection_Handler.h
 *
 *  $Id$
 *
 *  @author Frank Hunleth <fhunleth@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_UIPMC_CONNECTION_HANDLER_H
#define TAO_UIPMC_CONNECTION_HANDLER_H
#include "ace/pre.h"

#include "ace/Reactor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Acceptor.h"

#include "tao/corbafwd.h"
#include "tao/Wait_Strategy.h"
#include "tao/Connection_Handler.h"
#include "UIPMC_Transport.h"
#include "portablegroup_export.h"

#include "ace/SOCK_Dgram_Mcast.h"
#include "ace/SOCK_Dgram.h"


// Forward Decls
class TAO_Pluggable_Messaging;

// ****************************************************************

/**
 * @class TAO_UIPMC_Properties
 *
 * @brief UIPMC properties specification for a set of
 *  connections.
 *
 */

class TAO_PortableGroup_Export TAO_UIPMC_Properties
{

public:
  int ttl;
};



// ****************************************************************

/**
 * @class TAO_UIPMC_Connection_Handler
 *
 * @brief  Handles requests on a single connection.
 *
 * The Connection handler which is common for the Acceptor and
 * the Connector
 */


class TAO_PortableGroup_Export TAO_UIPMC_Connection_Handler :
  public TAO_UIPMC_SVC_HANDLER,
  public TAO_Connection_Handler
{

public:

  TAO_UIPMC_Connection_Handler (ACE_Thread_Manager* t = 0);

  /// Constructor. <arg> parameter is used by the Acceptor to pass the
  /// protocol configuration properties for this connection.
  TAO_UIPMC_Connection_Handler (TAO_ORB_Core *orb_core,
                               void *arg);


  /// Destructor.
  ~TAO_UIPMC_Connection_Handler (void);

  //@{
  /** @name Connection Handler overloads
   */
  virtual int open_handler (void *v);
  //@}

  // @@ Frank: Similar to open, but called on server
  virtual int open_server (void);

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
  ACE_HANDLE get_handle (void) const;

  const ACE_INET_Addr &addr (void);

  void addr (const ACE_INET_Addr &addr);

  const ACE_INET_Addr &local_addr (void);

  void local_addr (const ACE_INET_Addr &addr);

  const ACE_INET_Addr &server_addr (void);

  void server_addr (const ACE_INET_Addr &addr);

  const ACE_SOCK_Dgram &dgram (void);

  const ACE_SOCK_Dgram_Mcast &mcast_dgram (void);
  // UIPMC Additions - End

protected:

  // UIPMC Additions - Begin

  /// Client side UDP socket (send only).
  ACE_SOCK_Dgram udp_socket_;

  /// Server side Mcast UDP socket (receive only).
  ACE_SOCK_Dgram_Mcast mcast_socket_;

  /// Flag that specifies whether multicast is in use or not.
  CORBA::Boolean using_mcast_;

  // This is always the remote address
  ACE_INET_Addr addr_;

  // This is always the local address for the connector
  ACE_INET_Addr local_addr_;

  // UIPMC Additions - End

  //@{
  /**
   * @name TAO_Connection Handler overloads
   */
  virtual int release_os_resources (void);
  //@}

private:

  /// UIPMC configuration for this connection.
  TAO_UIPMC_Properties *uipmc_properties_;
};


#if defined (__ACE_INLINE__)
#include "UIPMC_Connection_Handler.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_UIPMC_CONNECTION_HANDLER_H */

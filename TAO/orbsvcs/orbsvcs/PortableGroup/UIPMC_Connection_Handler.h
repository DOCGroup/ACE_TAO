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

  /// Called by the <Strategy_Acceptor> when the handler is completely
  /// connected.  Argument is unused.
  virtual int open (void *);

  // @@ Frank: Similar to open, but called on server
  virtual int open_server (void);

  /// = Active object activation method.
  virtual int activate (long flags = THR_NEW_LWP,
                        int n_threads = 1,
                        int force_active = 0,
                        long priority = ACE_DEFAULT_THREAD_PRIORITY,
                        int grp_id = -1,
                        ACE_Task_Base *task = 0,
                        ACE_hthread_t thread_handles[] = 0,
                        void *stack[] = 0,
                        size_t stack_size[] = 0,
                        ACE_thread_t  thread_names[] = 0);

  /// Only used when the handler is turned into an active object by
  /// calling <activate>.  This serves as the event loop in such cases.
  virtual int svc (void);

  /// Perform appropriate closing.
  virtual int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
                            ACE_Reactor_Mask = ACE_Event_Handler::NULL_MASK);

  /// Event handler overload..
  virtual int resume_handler (void);

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

  /// = Event Handler overloads

  /// Reads a message from the <peer()>, dispatching and servicing it
  /// appropriately.
  /// handle_input() just delegates on handle_input_i() which timeouts
  /// after <max_wait_time>, this is used in thread-per-connection to
  /// ensure that server threads eventually exit.

  virtual int handle_input (ACE_HANDLE = ACE_INVALID_HANDLE);
  virtual int handle_cleanup ();

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

private:

  /// Perform appropriate closing
  void handle_close_i (void);

private:

  /// UIPMC configuration for this connection.
  TAO_UIPMC_Properties *uipmc_properties_;
};


#if defined (__ACE_INLINE__)
#include "UIPMC_Connection_Handler.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_UIPMC_CONNECTION_HANDLER_H */

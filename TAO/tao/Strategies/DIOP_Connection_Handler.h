// This may look like C, but it's really -*- C++ -*-

// ===================================================================
/**
 *  @file   DIOP_Connection_Handler.h
 *
 *  $Id$
 *
 *  @author  Michael Kircher
 */
// ===================================================================

#ifndef TAO_DIOP_CONNECTION_HANDLER_H
#define TAO_DIOP_CONNECTION_HANDLER_H
#include "ace/pre.h"

#include "tao/corbafwd.h"

#if defined (TAO_HAS_DIOP) && (TAO_HAS_DIOP != 0)

#include "ace/Reactor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Acceptor.h"

#include "tao/corbafwd.h"
#include "tao/Wait_Strategy.h"
#include "tao/Connection_Handler.h"
#include "DIOP_Transport.h"
#include "ace/SOCK_Dgram.h"
#include "strategies_export.h"

// Forward Decls
class TAO_Pluggable_Messaging;

// ****************************************************************

/**
 * @class TAO_DIOP_Properties
 *
 * @brief TCP protocol properties specification for a set of
 *  connections.
 *
 */

class TAO_Strategies_Export TAO_DIOP_Properties
{

public:
  int send_buffer_size;
  int recv_buffer_size;
  int no_delay;
};



// ****************************************************************

/**
 * @class TAO_DIOP_Connection_Handler
 *
 * @brief  Handles requests on a single connection.
 *
 * The Connection handler which is common for the Acceptor and
 * the Connector
 */


class TAO_Strategies_Export TAO_DIOP_Connection_Handler :
  public TAO_DIOP_SVC_HANDLER,
  public TAO_Connection_Handler
{

public:

  TAO_DIOP_Connection_Handler (ACE_Thread_Manager* t = 0);

  /// Constructor. <arg> parameter is used by the Acceptor to pass the
  /// protocol configuration properties for this connection.
  TAO_DIOP_Connection_Handler (TAO_ORB_Core *orb_core,
                               CORBA::Boolean flag,
                               void *arg);


  /// Destructor.
  ~TAO_DIOP_Connection_Handler (void);

  /// Called by the <Strategy_Acceptor> when the handler is completely
  /// connected.  Argument is unused.
  virtual int open (void *);

  // @@ Frank: Similar to open, but called on server
  virtual int open_server (void);

  /// Active object activation method.
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

  //@{
  /** @name Event Handler overloads
   */
  virtual int resume_handler (void);
  virtual int close_connection (void);
  virtual int handle_input (ACE_HANDLE);
  virtual int handle_output (ACE_HANDLE);
  virtual int handle_close (ACE_HANDLE, ACE_Reactor_Mask);
  //@}

  /// Add ourselves to Cache.
  int add_transport_to_cache (void);

  // @@ Frank: Not needed
  /*
  /// Process the <listen_list>
  int process_listen_point_list (DIOP::ListenPointList &listen_list);
  */

  // DIOP Additions - Begin
  /**
   * @name The DIOP Additions
   *
   */
  //@{
  ACE_HANDLE get_handle (void) const;

  const ACE_INET_Addr &addr (void);

  void addr (const ACE_INET_Addr &addr);

  const ACE_INET_Addr &local_addr (void);

  void local_addr (const ACE_INET_Addr &addr);

  const ACE_INET_Addr &server_addr (void);

  void server_addr (const ACE_INET_Addr &addr);

  const ACE_SOCK_Dgram &dgram (void);
  //@}
  // DIOP Additions - End

protected:

  //@{
  /**
   * @name TAO_Connection Handler overloads
   */
  virtual int release_os_resources (void);
  //@}

  // DIOP Additions - Begin
  /**
   * @name The DIOP Additions
   *
   */
  //@{
  ACE_SOCK_Dgram udp_socket_;

  /// This is always the remote address
  ACE_INET_Addr addr_;

  /// This is always the local address for the connector
  ACE_INET_Addr local_addr_;
  //@}
  // DIOP Additions - End

private:
  /// TCP configuration for this connection.
  TAO_DIOP_Properties *tcp_properties_;
};

#if defined (__ACE_INLINE__)
#include "DIOP_Connection_Handler.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_HAS_DIOP && TAO_HAS_DIOP != 0 */

#include "ace/post.h"
#endif /* TAO_DIOP_CONNECTION_HANDLER_H */

// This may look like C, but it's really -*- C++ -*-

// ===================================================================
/**
 *  @file   IIOP_Connection_Handler.h
 *
 *  $Id$
 *
 *  @author Originally by Chris Cleeland as IIOP_Connect.h
 *  @author modified by Balachandran Natarajan <bala@cs.wustl.edu>
 */
// ===================================================================

#ifndef TAO_IIOP_CONNECTION_HANDLER_H
#define TAO_IIOP_CONNECTION_HANDLER_H
#include "ace/pre.h"

#include "ace/Reactor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corbafwd.h"
#include "tao/Connection_Handler.h"
#include "tao/IIOPC.h"

#include "ace/Svc_Handler.h"
#include "ace/SOCK_Stream.h"


// Service Handler for this transport
typedef ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
        TAO_IIOP_SVC_HANDLER;

#if defined ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT
template class TAO_Export ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>;
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT */


// Forward Decls
class TAO_Pluggable_Messaging;

// ****************************************************************

/**
 * @class TAO_IIOP_Properties
 *
 * @brief TCP protocol properties specification for a set of
 *  connections.
 *
 */

#define IPDSFIELD_DSCP_DEFAULT  0x00

class TAO_Export TAO_IIOP_Properties
{

public:
  int send_buffer_size;
  int recv_buffer_size;
  int no_delay;
  int enable_network_priority;
};



// ****************************************************************

/**
 * @class TAO_IIOP_Connection_Handler
 *
 * @brief  Handles requests on a single connection.
 *
 * The Connection handler which is common for the Acceptor and
 * the Connector
 */


class TAO_Export TAO_IIOP_Connection_Handler : public TAO_IIOP_SVC_HANDLER,
                                               public TAO_Connection_Handler
{

public:

  TAO_IIOP_Connection_Handler (ACE_Thread_Manager* t = 0);

  /// Constructor. <arg> parameter is used by the Acceptor to pass the
  /// protocol configuration properties for this connection.
  TAO_IIOP_Connection_Handler (TAO_ORB_Core *orb_core,
                               CORBA::Boolean flag,
                               void *arg);


  /// Destructor.
  ~TAO_IIOP_Connection_Handler (void);

  /// Called by the <Strategy_Acceptor> when the handler is completely
  /// connected.  Argument is unused.
  virtual int open (void *);


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

  /// Process the <listen_list>
  int process_listen_point_list (IIOP::ListenPointList &listen_list);

    ///Check if network priority needs to be enabled
  int enable_network_priority (void);

  ///Set the Diff-Serv codepoint if the Policy dicates the setting of Network Priority
  int set_dscp_codepoint (void);

  int set_dscp_codepoint (int tos);

  /// Update the tcp properties of the hanlder to the most recent
  /// properties set after the last invocation
  virtual void update_protocol_properties (int send_buffer_size,
                                           int recv_buffer_size,
                                           int no_delay,
                                           int enable_network_priority);

protected:

  /// Constructor that could be used by the derived classes.
  /**
   * Sometimes new pluggbale protocols which have similarties with
   * IIOP may be tempted to this class for their use. Classical
   * example being that of IIOP_SSL_Connection_Handler. This
   * constructor just initializes its base class and sets all of its
   * contents to the default value, if any
   */
  TAO_IIOP_Connection_Handler (TAO_ORB_Core *orb_core,
                               void *arg);

  //@{
  /**
   * @name TAO_Connection Handler overloads
   */
  virtual int release_os_resources (void);
  //@}

private:
  /// TCP configuration for this connection.
  TAO_IIOP_Properties tcp_properties_;

  /// Stores the type of service value.
  int dscp_codepoint_;
};

#if defined (__ACE_INLINE__)
#include "tao/IIOP_Connection_Handler.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_IIOP_CONNECTION_HANDLER_H */

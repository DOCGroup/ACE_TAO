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

#include "ace/Acceptor.h"

#include "tao/corbafwd.h"
#include "tao/Wait_Strategy.h"
#include "tao/Connection_Handler.h"
#include "tao/IIOP_Transport.h"
#include "tao/IIOPC.h"

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

class TAO_Export TAO_IIOP_Properties
{

public:
  int send_buffer_size;
  int recv_buffer_size;
  int no_delay;
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

  /// Perform appropriate closing.
  virtual int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
                            ACE_Reactor_Mask = ACE_Event_Handler::NULL_MASK);

  /// Return the underlying transport object
  TAO_Transport *transport (void);

  /// Return the underlying handle
  virtual ACE_HANDLE fetch_handle (void);

  /// Called when buffering timer expires.
  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *arg = 0);

  /// Object termination hook.
  virtual int close (u_long flags = 0);

  /// Add ourselves to Cache.
  int add_handler_to_cache (void);

  /// Process the <listen_list>
  int process_listen_point_list (IIOP::ListenPointList &listen_list);
protected:

  /// = Event Handler overloads

  /// Reads a message from the <peer()>, dispatching and servicing it
  /// appropriately.
  /// handle_input() just delegates on handle_input_i() which timeouts
  /// after <max_wait_time>, this is used in thread-per-connection to
  /// ensure that server threads eventually exit.

  virtual int handle_input (ACE_HANDLE = ACE_INVALID_HANDLE);
  virtual int handle_input_i (ACE_HANDLE = ACE_INVALID_HANDLE,
                              ACE_Time_Value *max_wait_time = 0);

private:

  /// Transport object reference.
  TAO_IIOP_Transport transport_;

  /// Reference count.It is used to count nested upcalls on this
  /// svc_handler i.e., the connection can close during nested upcalls,
  /// you should not delete the svc_handler until the stack unwinds
  /// from the nested upcalls.
  u_long refcount_;

  /// TCP configuration for this connection.
  TAO_IIOP_Properties *tcp_properties_;
};


#if defined (__ACE_INLINE__)
#include "tao/IIOP_Connection_Handler.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_IIOP_CONNECTION_HANDLER_H */

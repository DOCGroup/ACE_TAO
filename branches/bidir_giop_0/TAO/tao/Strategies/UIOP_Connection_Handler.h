// This may look like C, but it's really -*- C++ -*-

// ===================================================================
/**
 *  @file   UIOP_Connection_Handler.h
 *
 *  $Id$
 *
 *  @author Originally by Ossama Othman <ossama@ece.uci.edu> as
 *   UIOP_Connect.h
 *  @author modified by Balachandran Natarajan <bala@cs.wustl.edu>
 */
// ===================================================================
#ifndef TAO_UIOP_CONNECTION_HANDLER_H
#define TAO_UIOP_CONNECTION_HANDLER_H
#include "ace/pre.h"

#include "UIOP_Transport.h"

#if TAO_HAS_UIOP == 1

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Acceptor.h"
#include "ace/Reactor.h"
#include "tao/Connection_Handler.h"
#include "tao/corbafwd.h"
#include "tao/Wait_Strategy.h"


// Forward Decls
class TAO_Pluggable_Messaging;

/**
 * @class TAO_IIOP_Properties
 *
 * @brief Unix Domain Sockets protocol properties specification for a
 *  set of  connections.
 */

class TAO_Strategies_Export TAO_UIOP_Properties
{
public:

  int send_buffer_size;
  int recv_buffer_size;
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


class TAO_Strategies_Export TAO_UIOP_Connection_Handler : public TAO_UIOP_SVC_HANDLER,
                                                          public TAO_Connection_Handler
{

public:

  TAO_UIOP_Connection_Handler (ACE_Thread_Manager* t = 0);

  /// Constructor. <arg> parameter is used by the Acceptor to pass the
  /// protocol configuration properties for this connection.
  TAO_UIOP_Connection_Handler (TAO_ORB_Core *orb_core,
                               CORBA::Boolean flag,
                               void *arg);


  /// Destructor.
  ~TAO_UIOP_Connection_Handler (void);

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
  TAO_UIOP_Transport transport_;

  /// Reference count.It is used to count nested upcalls on this
  /// svc_handler i.e., the connection can close during nested upcalls,
  /// you should not delete the svc_handler until the stack unwinds
  /// from the nested upcalls.
  u_long refcount_;

  /// TCP configuration for this connection.
  TAO_UIOP_Properties *uiop_properties_;
};



#if defined (__ACE_INLINE__)
#include "UIOP_Connection_Handler.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_HAS_UIOP == 1 */

#include "ace/post.h"
#endif /* TAO_UIOP_CONNECT_H */

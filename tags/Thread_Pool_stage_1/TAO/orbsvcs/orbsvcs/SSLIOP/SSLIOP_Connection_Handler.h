// -*- C++ -*-
//
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO_SSLIOP
//
// = FILENAME
//    SSLIOP_Connect.h
//
// = AUTHOR
//    Carlos O'Ryan <coryan@uci.edu>
//    Ossama Othman <ossama@uci.edu>
//
// ============================================================================

#ifndef TAO_SSLIOP_CONNECT_H
#define TAO_SSLIOP_CONNECT_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Reactor.h"

#include "tao/corbafwd.h"
#include "tao/Wait_Strategy.h"
#include "tao/Connection_Handler.h"
#include "tao/IIOP_Connection_Handler.h"
#include "tao/IIOPC.h"

#include "SSLIOP_Transport.h"
#include "SSLIOP_Current_Impl.h"
#include "orbsvcs/SSLIOPC.h"



/**
 * @class TAO_SSLIOP_Connection_Handler
 *
 * @brief  Handles requests on a single connection.
 *
 * The Connection handler which is common for the Acceptor and
 * the Connector
 */


class TAO_SSLIOP_Export TAO_SSLIOP_Connection_Handler : public TAO_SSL_SVC_HANDLER,
                                                        public TAO_Connection_Handler
{

public:

  TAO_SSLIOP_Connection_Handler (ACE_Thread_Manager* t = 0);

  /// Constructor. <arg> parameter is used by the Acceptor to pass the
  /// protocol configuration properties for this connection.
  TAO_SSLIOP_Connection_Handler (TAO_ORB_Core *orb_core,
                                 CORBA::Boolean flag,
                                 void *arg);


  /// Destructor.
  ~TAO_SSLIOP_Connection_Handler (void);

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

  /// Return the underlying handle
  virtual ACE_HANDLE fetch_handle (void);

  /// Documented in ACE_Event_Handler
  virtual int handle_output (ACE_HANDLE);

  /// Add ourselves to Cache.
  int add_transport_to_cache (void);

  /// Process the <listen_list>
  int process_listen_point_list (IIOP::ListenPointList &listen_list);

  /// Make the SSL session state available to the SSLIOP::Current
  /// object.
  int setup_ssl_state (TAO_ORB_Core *orb_core);

  /// Teardown the SSL session state.
  void teardown_ssl_state (void);

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



protected:

  /// Reference to the SSLIOP::Current object.
  SSLIOP::Current_var current_;

  /// The portion of the SSLIOP::Current object that is placed in
  /// TSS.
  TAO_SSLIOP_Current_Impl current_impl_;

private:

  /// Reference count.It is used to count nested upcalls on this
  /// svc_handler i.e., the connection can close during nested upcalls,
  /// you should not delete the svc_handler until the stack unwinds
  /// from the nested upcalls.
  long pending_upcalls_;

  /// TCP configuration for this connection.
  TAO_IIOP_Properties *tcp_properties_;
};


// ****************************************************************

/**
 * @class TAO_SSL_State_Guard
 *
 * @brief
 * This Class that sets up TSS SSL states upon instantiation, and
 * tears down the TSS SSL state when that instance goes out of scope.
 *
 * This guard is used to make TSS SSL state configuration and
 * deconfiguration during an upcall exception safe.  Exceptions are
 * not supposed to be propagated up to the scope this guard is used
 * in, so this guard may be unnecessary.  However, since proper TSS
 * SSL state configuration/deconfiguration is critical to proper
 * security support, this guard is used to ensure that
 * configuration/deconfiguration is exception safe.
 */
class TAO_SSLIOP_Export TAO_SSL_State_Guard
{
public:

  /// Constructor that sets up the TSS SSL state.
  TAO_SSL_State_Guard (TAO_SSLIOP_Connection_Handler *handler,
                       TAO_ORB_Core *orb_core,
                       int &result);

  /// Destructor that tears down the TSS SSL state.
  ~TAO_SSL_State_Guard (void);

private:

  /// Pointer to the connection handler currently handling the
  /// request/upcall.
  TAO_SSLIOP_Connection_Handler *handler_;
};


#if defined (__ACE_INLINE__)
#include "SSLIOP_Connection_Handler.i"
#endif /* __ACE_INLINE__ */


#include "ace/post.h"

#endif /* TAO_SSLIOP_CONNECT_H */

// -*- C++ -*-

//=============================================================================
/**
 *  @file    SSLIOP_Connection_Handler.h
 *
 *  $Id$
 *
 *  @author  Carlos O'Ryan <coryan@uci.edu>
 *  @author  Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_SSLIOP_CONNECTION_HANDLER_H
#define TAO_SSLIOP_CONNECTION_HANDLER_H

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
#include "SSLIOP_Current.h"


class TAO_SSLIOP_Connection_Handler_State;
class TAO_SSLIOP_Connection_Handler;
typedef TAO_SSLIOP_Connection_Handler TAO_SSLIOP_SVC_HANDLER;


/**
 * @class TAO_SSLIOP_Connection_Handler
 *
 * @brief  Handles requests on a single connection.
 *
 * The Connection handler which is common for the Acceptor and
 * the Connector
 */
class TAO_SSLIOP_Export TAO_SSLIOP_Connection_Handler
  : public TAO_SSL_SVC_HANDLER,
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

  /// Documented in ACE_Event_Handler
  virtual int handle_output (ACE_HANDLE);

  /// Doumented in ACE_Event_Handler
  virtual int resume_handler (void);

  /// Add ourselves to Cache.
  int add_transport_to_cache (void);

  /// Process the <listen_list>
  int process_listen_point_list (IIOP::ListenPointList &listen_list);

  /// Make the SSL session state available to the SSLIOP::Current
  /// object.
  int setup_ssl_state (TAO_SSLIOP_Current_Impl *&previous_current_impl,
                       TAO_SSLIOP_Current_Impl *new_current_impl,
                       CORBA::Boolean &setup_done);

  /// Teardown the SSL session state.
  void teardown_ssl_state (TAO_SSLIOP_Current_Impl *previous_current_impl,
                           CORBA::Boolean &setup_done);

protected:

  /**
   * @name Event Handler overloads
   *
   * SSLIOP-specific event handling methods.
   */
  //@{

  /// Reads a message from the <peer()>, dispatching and servicing it
  /// appropriately.
  /// handle_input() just delegates on handle_input_i().
  virtual int handle_input (ACE_HANDLE = ACE_INVALID_HANDLE);

  //@}

protected:

  /// Reference to the SSLIOP::Current object (downcast to gain access
  /// to the low-level management methods).
  TAO_SSLIOP_Current_var current_;

private:

  /// Perform appropriate closing..
  void handle_close_i (ACE_HANDLE);

private:

  /// TCP configuration for this connection.
  TAO_IIOP_Properties *tcp_properties_;

};

// ****************************************************************

class TAO_SSLIOP_Connection_Handler_State
{
public:

  /// TCP configuration for the connection associated with the
  /// connection handler.
  TAO_IIOP_Properties *tcp_properties;

  /// Reference to the (downcast) SSLIOP::Current object.
  TAO_SSLIOP_Current_var ssliop_current;

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
class TAO_SSL_State_Guard
{
public:

  /// Constructor that sets up the TSS SSL state.
  TAO_SSL_State_Guard (TAO_SSLIOP_Connection_Handler *handler,
                       int &result);

  /// Destructor that tears down the TSS SSL state.
  ~TAO_SSL_State_Guard (void);

private:

  /// Pointer to the connection handler currently handling the
  /// request/upcall.
  TAO_SSLIOP_Connection_Handler *handler_;

  /// The SSLIOP::Current implementation that was previously
  /// associated with the current thread and invocation.
  /**
   * It is stored here until the invocation completes, after which it
   * placed back into TSS.
   */
  TAO_SSLIOP_Current_Impl *previous_current_impl_;

  /// The SSLIOP::Current implementation to be associated with the
  /// current invocation.
  TAO_SSLIOP_Current_Impl current_impl_;

  /// Flag that specifies whether or not setup of the SSLIOP::Current
  /// object completed for the current thread and invocation.
  CORBA::Boolean setup_done_;

};


#if defined (__ACE_INLINE__)
#include "SSLIOP_Connection_Handler.i"
#endif /* __ACE_INLINE__ */


#include "ace/post.h"

#endif /* TAO_SSLIOP_CONNECTION_HANDLER_H */

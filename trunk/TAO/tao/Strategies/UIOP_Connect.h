// This may look like C, but it's really -*- C++ -*-
// $Id$


// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     UIOP_Connect.h
//
// = AUTHOR
//     Ossama Othman
//
// ============================================================================

#ifndef TAO_UIOP_CONNECT_H
#define TAO_UIOP_CONNECT_H
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

class TAO_Strategies_Export TAO_UIOP_Properties
{
  // = TITLE
  //   Unix Domain Sockets protocol properties for a set of
  //   connections.
  //
public:
  int send_buffer_size;
  int recv_buffer_size;
};

// ****************************************************************

class TAO_Strategies_Export TAO_UIOP_Server_Connection_Handler : public TAO_UIOP_SVC_HANDLER,
                                                      public TAO_Connection_Handler
{
  // = TITLE
  //   Handles requests on a single connection in a server.

public:
  TAO_UIOP_Server_Connection_Handler (ACE_Thread_Manager* t = 0);
  TAO_UIOP_Server_Connection_Handler (TAO_ORB_Core *orb_core,
                                      CORBA::Boolean flag,
                                      void *arg);
  // Constructor. <arg> parameter is used by the Acceptor to pass the
  // protocol configuration properties for this connection.

  ~TAO_UIOP_Server_Connection_Handler (void);
  // Destructor

  virtual int open (void *);
  // Called by the <Strategy_Acceptor> when the handler is completely
  // connected.  Argument is unused.

  // = Active object activation method.
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

  virtual int svc (void);
  // Only used when the handler is turned into an active object by
  // calling <activate>.  This serves as the event loop in such cases.

  // = Template Methods Called by <handle_input>

  TAO_Transport *transport (void);

  virtual ACE_HANDLE fetch_handle (void);
  // Get the underlying handle

protected:

  TAO_UIOP_Server_Transport transport_;
  // @@ New transport object reference.

  TAO_Pluggable_Messaging *acceptor_factory_;
  // Messaging acceptor factory

  // = Event Handler overloads

  virtual int handle_input (ACE_HANDLE = ACE_INVALID_HANDLE);
  virtual int handle_input_i (ACE_HANDLE = ACE_INVALID_HANDLE,
                              ACE_Time_Value *max_wait_time = 0);
  // Reads a message from the <peer()>, dispatching and servicing it
  // appropriately.
  // handle_input() just delegates on handle_input_i() which timeouts
  // after <max_wait_time>, this is used in thread-per-connection to
  // ensure that server threads eventually exit.

  virtual int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
                            ACE_Reactor_Mask = ACE_Event_Handler::NULL_MASK);
  // Perform appropriate closing.

  u_long refcount_;
  // Reference count.  It is used to count nested upcalls on this
  // svc_handler i.e., the connection can close during nested upcalls,
  // you should not delete the svc_handler until the stack unwinds
  // from the nested upcalls.

  TAO_UIOP_Properties *uiop_properties_;
  // UIOP configuration properties for this connection.
};


// ****************************************************************

class TAO_Strategies_Export TAO_UIOP_Client_Connection_Handler : public TAO_UIOP_SVC_HANDLER,
                                                      public TAO_Connection_Handler
{
  // = TITLE
  //      <Svc_Handler> used on the client side and returned by the
  //      <TAO_CONNECTOR>.
public:
  // = Intialization methods.

  TAO_UIOP_Client_Connection_Handler (ACE_Thread_Manager* t = 0);
  // This constructor should *never* get called, it is just here to
  // make the compiler happy: the default implementation of the
  // Creation_Strategy requires a constructor with that signature, we
  // don't use that implementation, but some (most?) compilers
  // instantiate it anyway.

  TAO_UIOP_Client_Connection_Handler (ACE_Thread_Manager *t,
                                      TAO_ORB_Core* orb_core,
                                      CORBA::Boolean flag,
                                      void *arg);
  // Constructor. <arg> parameter is used by the Connector to pass the
  // protocol configuration properties for this connection.

  virtual ~TAO_UIOP_Client_Connection_Handler (void);

  // = <Connector> hook.
  virtual int open (void *);
  // Activation template method.

  // = Event Handler overloads

  virtual int handle_input (ACE_HANDLE = ACE_INVALID_HANDLE);
  // Called when a response from a twoway invocation is available.

  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *arg = 0);
  // Called when buffering timer expires.

  virtual int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
                            ACE_Reactor_Mask = ACE_Event_Handler::NULL_MASK);
  // Perform appropriate closing.

  virtual int handle_close_i (ACE_HANDLE = ACE_INVALID_HANDLE,
                              ACE_Reactor_Mask = ACE_Event_Handler::NULL_MASK);
  // Perform appropriate closing but without grabbing any locks.

  virtual int close (u_long flags = 0);
  // Object termination hook.

  virtual TAO_Transport *transport (void);

  virtual ACE_HANDLE fetch_handle (void);
  // Get the underlying handle

protected:
  int handle_cleanup (void);
  // This method deregisters the handler from the reactor and closes it.

  TAO_UIOP_Client_Transport transport_;
  // Reference to the transport object, it is owned by this class.

  TAO_UIOP_Properties *uiop_properties_;
  // UIOP configuration properties for this connection.

private:

  virtual int handle_input_i (ACE_HANDLE = ACE_INVALID_HANDLE,
                              ACE_Time_Value *max_wait_time = 0);
  // Will not be called at all. As a matter of fact should not be
  // called. This is just to override the pure virtual function in the
  // TAO_Connection_Handler class
};



#if defined (__ACE_INLINE__)
#include "UIOP_Connect.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_HAS_UIOP == 1 */

#include "ace/post.h"
#endif /* TAO_UIOP_CONNECT_H */

// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//   SHMIOP_Connect.h
//
// = AUTHOR
//   Nanbor Wang <nanbor@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_SHMIOP_CONNECT_H
#define TAO_SHMIOP_CONNECT_H
#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_HAS_SHMIOP) && (TAO_HAS_SHMIOP != 0)

#include "ace/Reactor.h"
#include "ace/Acceptor.h"


#include "tao/corbafwd.h"
#include "tao/Wait_Strategy.h"
#include "tao/Connection_Handler.h"
#include "tao/SHMIOP_Transport.h"






// ****************************************************************

class TAO_Export TAO_SHMIOP_Client_Connection_Handler : public TAO_SHMIOP_SVC_HANDLER,
                                                        public TAO_Connection_Handler
{
  // = TITLE
  //      <Svc_Handler> used on the client side and returned by the
  //      <TAO_CONNECTOR>.
public:

  // = Intialization method.
  TAO_SHMIOP_Client_Connection_Handler (ACE_Thread_Manager *t = 0,
                                        TAO_ORB_Core* orb_core = 0,
                                        CORBA::Boolean flag = 0);

  virtual ~TAO_SHMIOP_Client_Connection_Handler (void);

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
  // Return the transport objects

protected:
  int handle_cleanup (void);
  // This method deregisters the handler from the reactor and closes it.

  TAO_SHMIOP_Client_Transport transport_;
  // Reference to the transport object, it is owned by this class.

private:
  virtual int handle_input_i (ACE_HANDLE = ACE_INVALID_HANDLE,
                              ACE_Time_Value *max_wait_time = 0);
  // Will not be called at all. As a matter of fact should not be
  // called. This is just to override the pure virtual function in the
  // TAO_Connection_Handler class
};

// ****************************************************************

class TAO_Export TAO_SHMIOP_Server_Connection_Handler : public TAO_SHMIOP_SVC_HANDLER,
                                                        public TAO_Connection_Handler
{
  // = TITLE
  //   Handles requests on a single connection in a server.

public:
  TAO_SHMIOP_Server_Connection_Handler (ACE_Thread_Manager* t = 0);
  TAO_SHMIOP_Server_Connection_Handler (TAO_ORB_Core *orb_core,
                                        CORBA::Boolean flag,
                                        void *);
  // Constructor.
  // Last argument currently not used.

  ~TAO_SHMIOP_Server_Connection_Handler (void);

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

protected:

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

protected:
  TAO_SHMIOP_Server_Transport transport_;
  // @@ New transport object reference.

  TAO_Pluggable_Messaging *acceptor_factory_;
  // Messaging acceptor factory

  u_long refcount_;
  // Reference count.  It is used to count nested upcalls on this
  // svc_handler i.e., the connection can close during nested upcalls,
  // you should not delete the svc_handler until the stack unwinds
  // from the nested upcalls.
};

#if defined (__ACE_INLINE__)
#include "tao/SHMIOP_Connect.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_HAS_SHMIOP && TAO_HAS_SHMIOP != 0 */

#include "ace/post.h"
#endif /* TAO_SHMIOP_CONNECT_H */

// This may look like C, but it's really -*- C++ -*-
// $Id$

#ifndef TAO_CONNECT_H
#  define TAO_CONNECT_H

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    Connect.h
//
// = AUTHOR
//     Chris Cleeland
//
// ============================================================================

#  include "ace/Reactor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#  include "ace/Acceptor.h"
#  include "ace/SOCK_Acceptor.h"
#  include "ace/Synch.h"
#  include "ace/Svc_Handler.h"

#  include "tao/corbafwd.h"

// Forward Decls
class TAO_Transport;
class TAO_IIOP_Transport;
class TAO_IIOP_Client_Transport;
class TAO_IIOP_Server_Transport;
class TAO_ORB_Core;

typedef ACE_Svc_Handler<TAO_SOCK_STREAM, ACE_NULL_SYNCH>
        TAO_SVC_HANDLER;

class TAO_IIOP_Handler_Base : public TAO_SVC_HANDLER
{
public:
  TAO_IIOP_Handler_Base (ACE_Thread_Manager *t);
  TAO_IIOP_Handler_Base (TAO_ORB_Core *orb_core);

  virtual TAO_Transport *transport (void) = 0;

  virtual int resume_handler (ACE_Reactor *reactor);
  // Resume the handler.
};

class TAO_Export TAO_Client_Connection_Handler : public TAO_IIOP_Handler_Base
{
  // = TITLE
  //      <Svc_Handler> used on the client side and returned by the
  //      <TAO_CONNECTOR>.
public:
  // = Intialization method.
  TAO_Client_Connection_Handler (ACE_Thread_Manager *t = 0);

  virtual ~TAO_Client_Connection_Handler (void);

  // = <Connector> hook.
  virtual int open (void *);
  // Activation template method.

  virtual int send_request (TAO_ORB_Core* orb_core,
                            TAO_OutputCDR &stream,
                            int is_twoway);
  // Send the request in <stream>.  If it is a twoway invocation, then
  // this re-enters the reactor event loop so that incoming requests
  // can continue to be serviced.  This insures that a nested upcall,
  // i.e., an invocation coming back from the remote during this
  // invocation, will still be handled and deadlock averted.

  // = Event Handler overloads

  virtual int handle_input (ACE_HANDLE = ACE_INVALID_HANDLE);
  // Called when a a response from a twoway invocation is available.

  virtual int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
                            ACE_Reactor_Mask = ACE_Event_Handler::NULL_MASK);
  // Perform appropriate closing.

  virtual int close (u_long flags = 0);
  // Object termination hook.

  virtual TAO_Transport *transport (void);

protected:

  TAO_IIOP_Client_Transport *iiop_transport_;
  // @@ New transport object reference.
  // The handler is responsible for creating this object when
  // it is instantiated. fredk

  int check_unexpected_data (void);
  // This method checks for unexpected data

  int expecting_response_;
  // State flag which, if non-zero, indicates that this handler is
  // looking to get input.  Otherwise, any input received is
  // unexpected.

  int input_available_;
  // Flag indicating whether or not input is available.  Only valid
  // when <expecting_response_> is non-zero.
};

class TAO_Export TAO_RW_Client_Connection_Handler : public TAO_Client_Connection_Handler
{
public:
  TAO_RW_Client_Connection_Handler (ACE_Thread_Manager *t = 0);

  virtual ~TAO_RW_Client_Connection_Handler (void);

  virtual int send_request (TAO_ORB_Core* orb_core,
                            TAO_OutputCDR &stream,
                            int is_twoway);
  // Send the request in <stream>.  Since this class simply
  // reads/writes from a socket (and does not handle nested upcalls),
  // there is no need to register with a reactor.

  virtual int resume_handler (ACE_Reactor *reactor);
  // Resume the handler.

protected:

};

class TAO_Export TAO_ST_Client_Connection_Handler : public TAO_Client_Connection_Handler
{
public:
  TAO_ST_Client_Connection_Handler (ACE_Thread_Manager *t = 0);

  virtual ~TAO_ST_Client_Connection_Handler (void);

  virtual int open (void *);
  // Initialize the handler.

  virtual int send_request (TAO_ORB_Core* orb_core,
                            TAO_OutputCDR &stream,
                            int is_twoway);
  // Send the request in <stream>.  If it is a twoway invocation, then
  // this re-enters the reactor event loop so that incoming requests
  // can continue to be serviced.  This insures that a nested upcall,
  // i.e., an invocation coming back from the remote during this
  // invocation, will still be handled and deadlock averted.

  virtual int handle_input (ACE_HANDLE = ACE_INVALID_HANDLE);
  // Called when a a response from a twoway invocation is available.

  virtual int resume_handler (ACE_Reactor *reactor);
  // Resume the handler.

protected:

};

class TAO_Export TAO_MT_Client_Connection_Handler : public TAO_Client_Connection_Handler
{
public:
  TAO_MT_Client_Connection_Handler (ACE_Thread_Manager *t = 0);

  virtual ~TAO_MT_Client_Connection_Handler (void);

  virtual int open (void *);
  // Initialize the handler.

  virtual int send_request (TAO_ORB_Core* orb_core,
                            TAO_OutputCDR &stream,
                            int is_twoway);
  // Send the request in <stream>.  If it is a twoway invocation, then
  // this re-enters the reactor event loop so that incoming requests
  // can continue to be serviced.  This insures that a nested upcall,
  // i.e., an invocation coming back from the remote during this
  // invocation, will still be handled and deadlock averted.

  virtual int handle_input (ACE_HANDLE = ACE_INVALID_HANDLE);
  // Called when a a response from a twoway invocation is available.

  virtual int resume_handler (ACE_Reactor *reactor);
  // Resume the handler.

protected:
  ACE_SYNCH_CONDITION* cond_response_available (TAO_ORB_Core* orb_core);
  // Return the cond_response_available, initializing it if necessary.

  ACE_thread_t calling_thread_;
  // the thread ID of the thread we were running in.

  ACE_SYNCH_CONDITION* cond_response_available_;
  // wait on reponse if the leader-follower model is active

  TAO_ORB_Core* orb_core_;
  // The ORB core where we are executing a request.
};

// ****************************************************************

class TAO_ORB_Core;

class TAO_Export TAO_Server_Connection_Handler : public TAO_IIOP_Handler_Base
{
  // = TITLE
  //   Handles requests on a single connection in a server.

public:
  TAO_Server_Connection_Handler (ACE_Thread_Manager* t = 0);
  TAO_Server_Connection_Handler (TAO_ORB_Core *orb_core);
  // Constructor.

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

  virtual int handle_message (TAO_InputCDR &msg,
                              TAO_OutputCDR &response,
                              CORBA::Boolean &response_required,
                              CORBA::ULong &request_id,
                              CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // Handle processing of the request residing in <msg>, setting
  // <response_required> to zero if the request is for a oneway or
  // non-zero if for a two-way and <response> to any necessary
  // response (including errors).  In case of errors, -1 is returned
  // and additional information carried in <env>.

  TAO_Transport *transport (void);

protected:
  TAO_IIOP_Server_Transport *iiop_transport_;
  // @@ New transport object reference.

  virtual int handle_locate (TAO_InputCDR &msg,
                             TAO_OutputCDR &response,
                             CORBA::Boolean &response_required,
                             CORBA::ULong &request_id,
                             CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // Handle processing of the location request residing in <msg>,
  // setting <response_required> to one if no errors are encountered.
  // The LocateRequestReply is placed into <response>.  In case of
  // errors, -1 is returned and additional information carried in
  // <TAO_IN_ENV>.

  virtual void send_response (TAO_OutputCDR &response);
  // Send <response> to the client on the other end.

  void send_error (CORBA::ULong request_id,
                   CORBA::Exception *ex);
  // Send <error> to the client on the other end, which
  // means basically sending the exception.

  // = Event Handler overloads

  virtual int handle_input (ACE_HANDLE = ACE_INVALID_HANDLE);
  // Reads a message from the <peer()>, dispatching and servicing it
  // appropriately.

  virtual int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
                            ACE_Reactor_Mask = ACE_Event_Handler::NULL_MASK);
  // Perform appropriate closing.

  TAO_ORB_Core *orb_core_;
  // Cache the ORB Core to minimize
};

#if defined (__ACE_INLINE__)
# include "tao/Connect.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_CONNECT_H */

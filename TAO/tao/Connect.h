// This may look like C, but it's really -*- C++ -*-
// $Id$

#if !defined (TAO_CONNECT_H)
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
#  include "ace/Acceptor.h"
#  include "ace/SOCK_Acceptor.h"
#  include "ace/Synch.h"
#  include "ace/Svc_Handler.h"

#  include "tao/corba.h"

// Forward Decls
class TAO_OA_Parameters;

typedef ACE_Svc_Handler<TAO_SOCK_STREAM, ACE_NULL_SYNCH>
        TAO_SVC_HANDLER;

class TAO_Client_Connection_Handler : public TAO_SVC_HANDLER
{
  // = TITLE
  //      <Svc_Handler> used on the client side and returned by the
  //      <TAO_CONNECTOR>.
public:
  // = Intialization method.
  TAO_Client_Connection_Handler (ACE_Thread_Manager * = 0);

  ~TAO_Client_Connection_Handler ();

  // = <Connector> hook.
  virtual int open (void *);
  // Activation template method.

  virtual int close (u_long flags = 0);
  // Termination template method.

  int send_request (TAO_OutputCDR &stream, int is_twoway);
  // Send the request in <stream>.  If it is a twoway invocation, then
  // this re-enters the reactor event loop so that incoming requests
  // can continue to be serviced.  This insures that a nested upcall,
  // i.e., an invocation coming back from the remote during this
  // invocation, will still be handled and deadlock averted.

  // = Event Handler overloads

  virtual int handle_input (ACE_HANDLE = ACE_INVALID_HANDLE);
  // Called when a a response from a twoway invocation is available.

  virtual int handle_close (ACE_HANDLE, ACE_Reactor_Mask);
  // Perform appropriate closing of the connection.

private:
  typedef TAO_SVC_HANDLER BASECLASS;
  // Trait indicating the base class.

  int expecting_response_;
  // State flag which, if non-zero, indicates that this handler is
  // looking to get input.  Otherwise, any input received is
  // unexpected.

  int input_available_;
  // Flag indicating whether or not input is available.  Only valid
  // when <expecting_response_> is non-zero.

  ACE_thread_t calling_thread_;
  // the thread ID of the thread we were running in.

  ACE_SYNCH_CONDITION* cond_response_available_;
  // wait on reponse if the leader-follower model is active
};

class TAO_ORB_Core;

class TAO_Server_Connection_Handler : public TAO_SVC_HANDLER
{
  // = TITLE
  //   Handles requests on a single connection in a server.

public:
  TAO_Server_Connection_Handler (ACE_Thread_Manager *t = ACE_Thread_Manager::instance ());
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

  TAO_OA_Parameters *params (void);
  void params (TAO_OA_Parameters *p);

  // = Template Methods Called by <handle_input>

  virtual int handle_message (TAO_InputCDR &msg,
                              TAO_OutputCDR &response,
                              int &response_required,
                              CORBA::ULong &request_id,
                              CORBA::Environment &env);
  // Handle processing of the request residing in <msg>, setting
  // <response_required> to zero if the request is for a oneway or
  // non-zero if for a two-way and <response> to any necessary
  // response (including errors).  In case of errors, -1 is returned
  // and additional information carried in <env>.

protected:
  virtual int handle_locate (TAO_InputCDR &msg,
                             TAO_OutputCDR &response,
                             int &response_required,
                             CORBA::ULong &request_id,
                             CORBA::Environment &env);
  // Handle processing of the location request residing in <msg>,
  // setting <response_required> to one if no errors are encountered.
  // The LocateRequestReply is placed into <response>.  In case of
  // errors, -1 is returned and additional information carried in
  // <env>.

  virtual void send_response (TAO_OutputCDR &response);
  // Send <response> to the client on the other end.
  
  void send_error (CORBA::ULong request_id, CORBA::Environment &env);
  // Send <error> to the client on the other end, which
  // means basically sending the exception.

  // = Event Handler overloads

  virtual int handle_input (ACE_HANDLE = ACE_INVALID_HANDLE);
  // Reads a message from the <peer()>, dispatching and servicing it
  // appropriately.

  virtual int handle_close (ACE_HANDLE,
                            ACE_Reactor_Mask);
  // Perform appropriate closing of the connection.

  TAO_ORB_Core *parent_;

  TAO_OA_Parameters *params_;
  // Pointer to the object adapter parameters.
};

#if defined (__ACE_INLINE__)
# include "tao/Connect.i"
#endif /* __ACE_INLINE__ */

typedef ACE_Strategy_Acceptor<TAO_Server_Connection_Handler,
                              TAO_SOCK_ACCEPTOR>
        TAO_ACCEPTOR;

#endif /* TAO_CONNECT_H */

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

// Forward Decls
class TAO_OA_Parameters;
class TAO_GIOP_RequestHeader;

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
  // Do-nothing constructor.

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

  u_char expecting_response_;
  // State flag which, if non-zero, indicates that this handler is
  // looking to get input.  Otherwise, any input received is
  // unexpected.

  u_char input_available_;
  // Flag indicating whether or not input is available.  Only valid
  // when <expecting_response_> is non-zero.
};

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

  virtual int svc (void);
  // Only used when the handler is turned into an active object by
  // calling <activate>.  This serves as the event loop in such cases.

  TAO_OA_Parameters *params (void);
  void params (TAO_OA_Parameters *p);

  // = Template Methods Called by <handle_input>

  enum RequestStatus
  {
    Error = -1,
    Request,      // A CORBA Request was received
    LocateRequest // A CORBA LocateRequest was received
  };

  virtual RequestStatus recv_request (TAO_InputCDR &msg,
				      CORBA::Environment &env);
  // Extract a message from the stream associated with <peer()> and
  // place it into <msg>.  Return either <Request> or <LocateRequest>
  // if success, <Error> with <errno> and <env> set if problems.

  virtual int handle_message (TAO_InputCDR &msg,
			      int &response_required,
                              TAO_OutputCDR &response,
			      CORBA::Environment &env);
  // Handle processing of the request residing in <msg>, setting
  // <response_required> to zero if the request is for a oneway or
  // non-zero if for a two-way and <response> to any necessary
  // response (including errors).  In case of errors, -1 is returned
  // and additional information carried in <env>.

protected:
  virtual int handle_locate (TAO_InputCDR &msg,
			     int &response_required,
                             TAO_OutputCDR &response,
			     CORBA::Environment &env);
  // Handle processing of the location request residing in <msg>,
  // setting <response_required> to one if no errors are encountered.
  // The LocateRequestReply is placed into <response>.  In case of
  // errors, -1 is returned and additional information carried in
  // <env>.

  virtual void handle_request (const TAO_GIOP_RequestHeader &hdr,
                               TAO_InputCDR &request_body,
                               TAO_OutputCDR &response,
                               TAO_Dispatch_Context *some_info,
                               CORBA::Environment &env);
  // Once a request is found in a message, this finds the appropriate
  // POA and dispatches it, then takes care to properly format any
  // response.

  virtual void send_response (TAO_OutputCDR &response);
  // Send <response> to the client on the other end.

  // = Event Handler overloads

  virtual int handle_input (ACE_HANDLE = ACE_INVALID_HANDLE);
  // Reads a message from the <peer()>, dispatching and servicing it
  // appropriately.

  virtual int handle_close (ACE_HANDLE,
                            ACE_Reactor_Mask);
  // Perform appropriate closing of the connection.

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

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

#include "ace/Reactor.h"

#if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Acceptor.h"
#include "ace/LSOCK_Acceptor.h"
#include "ace/Synch.h"
#include "ace/Svc_Handler.h"

#include "tao/corbafwd.h"
#include "tao/GIOP.h"

// Forward Decls
class TAO_Transport;
class TAO_ORB_Core;
class TAO_ORB_Core_TSS_Resources;

class TAO_UIOP_Transport;
class TAO_UIOP_Client_Transport;
class TAO_UIOP_Server_Transport;

typedef ACE_Svc_Handler<ACE_LSOCK_STREAM, ACE_NULL_SYNCH>
        TAO_UIOP_SVC_HANDLER;

class TAO_UIOP_Handler_Base : public TAO_UIOP_SVC_HANDLER
{
public:
  TAO_UIOP_Handler_Base (ACE_Thread_Manager *t);
  TAO_UIOP_Handler_Base (TAO_ORB_Core *orb_core);

  virtual TAO_Transport *transport (void) = 0;
};

// ****************************************************************

class TAO_Export TAO_UIOP_Client_Connection_Handler : public TAO_UIOP_Handler_Base
{
  // = TITLE
  //      <Svc_Handler> used on the client side and returned by the
  //      <TAO_CONNECTOR>.
public:
  // = Intialization method.
  TAO_UIOP_Client_Connection_Handler (ACE_Thread_Manager *t = 0,
                                      TAO_ORB_Core* orb_core = 0);

  virtual ~TAO_UIOP_Client_Connection_Handler (void);

  // = <Connector> hook.
  virtual int open (void *);
  // Activation template method.

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
  TAO_UIOP_Client_Transport *uiop_transport_;
  // Reference to the transport object, it is owned by this class.
};

// ****************************************************************

class TAO_Export TAO_UIOP_Server_Connection_Handler : public TAO_UIOP_Handler_Base
{
  // = TITLE
  //   Handles requests on a single connection in a server.

public:
  TAO_UIOP_Server_Connection_Handler (ACE_Thread_Manager* t = 0);
  TAO_UIOP_Server_Connection_Handler (TAO_ORB_Core *orb_core);
  // Constructor.

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

  virtual int handle_message (TAO_InputCDR &msg,
                              TAO_OutputCDR &response,
                              CORBA::Boolean &response_required,
                              CORBA::ULong &request_id,
                              CORBA_Environment &TAO_IN_ENV =
                                  TAO_default_environment ());
  // Handle processing of the request residing in <msg>, setting
  // <response_required> to zero if the request is for a oneway or
  // non-zero if for a two-way and <response> to any necessary
  // response (including errors).  In case of errors, -1 is returned
  // and additional information carried in <env>.

  TAO_Transport *transport (void);

protected:
  TAO_UIOP_Server_Transport *uiop_transport_;
  // @@ New transport object reference.

  virtual int handle_locate (TAO_InputCDR &msg,
                             TAO_OutputCDR &response,
                             CORBA::Boolean &response_required,
                             CORBA::ULong &request_id,
                             CORBA_Environment &TAO_IN_ENV =
                                 TAO_default_environment ());
  // Handle processing of the location request residing in <msg>,
  // setting <response_required> to one if no errors are encountered.
  // The LocateRequestReply is placed into <response>.  In case of
  // errors, -1 is returned and additional information carried in
  // <TAO_IN_ENV>.

  virtual void send_response (TAO_OutputCDR &response);
  // Send <response> to the client on the other end.

  // = Event Handler overloads

  virtual int handle_input (ACE_HANDLE = ACE_INVALID_HANDLE);
  // Reads a message from the <peer()>, dispatching and servicing it
  // appropriately.

  virtual int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
                            ACE_Reactor_Mask = ACE_Event_Handler::NULL_MASK);
  // Perform appropriate closing.

  TAO_ORB_Core *orb_core_;
  // Cache the ORB Core to minimize

  TAO_ORB_Core_TSS_Resources *tss_resources_;
  // Cached tss resources of the ORB that activated this object.

  TAO_GIOP_MessageHeader message_header_;
  CORBA::ULong current_offset_;
  ACE_Message_Block payload_;
  // This keep the state of the current message, to enable
  // non-blocking reads.
};

#if defined (__ACE_INLINE__)
#include "tao/UIOP_Connect.i"
#endif /* __ACE_INLINE__ */

#endif /* !ACE_LACKS_UNIX_DOMAIN_SOCKETS */

#endif /* TAO_UIOP_CONNECT_H */

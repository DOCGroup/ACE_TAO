// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     SHMIOP_Transport.h
//
// = DESCRIPTION
//     IIOP Transport specific processing
//
// = AUTHOR
//     Nanbor Wang <nanbor@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_SHMIOP_TRANSPORT_H
#define TAO_SHMIOP_TRANSPORT_H
#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_HAS_SHMIOP) && (TAO_HAS_SHMIOP != 0)

#include "strategies_export.h"

#include "tao/Pluggable.h"
#include "tao/GIOP_Message_State.h"
#include "ace/MEM_Acceptor.h"
#include "ace/Synch.h"
#include "ace/Svc_Handler.h"

// Forward decls.
class TAO_SHMIOP_Client_Connection_Handler;
class TAO_SHMIOP_Server_Connection_Handler;
class TAO_ORB_Core;

typedef ACE_Svc_Handler<ACE_MEM_STREAM, ACE_NULL_SYNCH>
        TAO_SHMIOP_SVC_HANDLER;

class TAO_Strategies_Export TAO_SHMIOP_Transport : public TAO_Transport
{
  // = TITLE
  //   This class acts as a bridge class to the transport specific
  //   connection handler (handler_).
  //
  // = DESCRIPTION
  //   Specialization of the base TAO_Transport class to handle the IIOP
  //   protocol.  This class in turn will be further specialized for
  //   the client and server side.
public:
  TAO_SHMIOP_Transport (TAO_ORB_Core *orb_core);
  // Base object's creator method.

  ~TAO_SHMIOP_Transport (void);
  // Default destructor.

  // = The TAO_Transport methods, please check the documentation in
  //   "tao/Pluggable.h" for more details.
  virtual void close_connection (void);

  virtual ACE_HANDLE handle (void);
  virtual ACE_Event_Handler *event_handler (void);
  virtual ssize_t send (TAO_Stub *stub,
                        int two_way,
                        const ACE_Message_Block *mblk,
                        const ACE_Time_Value *s = 0);
  virtual ssize_t send (const ACE_Message_Block *mblk,
                        const ACE_Time_Value *s = 0,
                        size_t *bytes_transferred = 0);
  virtual ssize_t send (const u_char *buf,
                        size_t len,
                        const ACE_Time_Value *s = 0);
  virtual ssize_t recv (char *buf,
                        size_t len,
                        const ACE_Time_Value *s = 0);
  virtual int send_request (TAO_Stub *stub,
                            TAO_ORB_Core *orb_core ,
                            TAO_OutputCDR &stream,
                            int twoway,
                            ACE_Time_Value *max_wait_time);
  virtual CORBA::Boolean
  send_request_header (TAO_Operation_Details &opdetails,
                       TAO_Target_Specification &spec,
                       TAO_OutputCDR &msg);

  virtual TAO_SHMIOP_SVC_HANDLER *service_handler (void) = 0;
  // Return the underlying the service handler
};

class TAO_Strategies_Export TAO_SHMIOP_Client_Transport : public TAO_SHMIOP_Transport
{
  // = TITLE
  //   The Transport class used for Client side communication with a
  //   server.
  //
  // = DESCRIPTION
  //   Specialization of the TAO_SHMIOP_Transport class for client
  //   side.  Methods related to sending one and two way requests
  //   lives here.
public:
  TAO_SHMIOP_Client_Transport (TAO_SHMIOP_Client_Connection_Handler *handler,
                               TAO_ORB_Core *orb_core);
  // Constructor.

  ~TAO_SHMIOP_Client_Transport (void);
  // destructor

  // = The TAO_Transport methods, please check the documentation in
  //   "tao/Pluggable.h" for more details.
  virtual int idle (void);

  virtual void start_request (TAO_ORB_Core *orb_core,
                              TAO_Target_Specification & /*spec */,
                              TAO_OutputCDR &output,
                              CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void start_locate (TAO_ORB_Core *orb_core,
                             TAO_Target_Specification &spec,
                             TAO_Operation_Details &opdetails,
                             TAO_OutputCDR &output,
                             CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual int send_request (TAO_Stub *stub,
                            TAO_ORB_Core *orb_core,
                            TAO_OutputCDR &stream,
                            int twoway,
                            ACE_Time_Value *max_wait_time);
  virtual int handle_client_input (int block = 0,
                                   ACE_Time_Value *max_time_value = 0);

  virtual int register_handler (void);

  virtual CORBA::Boolean
  send_request_header (TAO_Operation_Details &opdetails,
                       TAO_Target_Specification &spec,
                       TAO_OutputCDR &msg);

  virtual TAO_SHMIOP_SVC_HANDLER *service_handler (void);

  int messaging_init (CORBA::Octet major,
                      CORBA::Octet minor);
  // Initialising the messaging object

  void use_lite (CORBA::Boolean flag);
  // Sets the lite flag

private:

  TAO_SHMIOP_Client_Connection_Handler *handler_;
  // the connection service handler used for accessing lower layer
  // communication protocols.

  TAO_Pluggable_Messaging *client_mesg_factory_;
  // The message_factor instance specific for this particular
  // transport protocol.

  TAO_ORB_Core *orb_core_;
  // Our ORB core

  CORBA::Boolean lite_flag_;
  // Are we using lite?

  TAO_Pluggable_Reply_Params params_;
  // The reply data that is sent back by the server
};

// ****************************************************************

class TAO_Strategies_Export TAO_SHMIOP_Server_Transport : public TAO_SHMIOP_Transport
{
  // = TITLE
  //   The Transport class used for server communication with a
  //   connected client.
  //
  // = DESCRIPTION
  //   Specialization of the TAO_SHMIOP_Transport class for the server side.
  //   methods for reading messages (requests) and sending replies live
  //   here.
public:

  TAO_SHMIOP_Server_Transport (TAO_SHMIOP_Server_Connection_Handler *handler,
                               TAO_ORB_Core *orb_core);
  //  Default creator method.

  ~TAO_SHMIOP_Server_Transport (void);
  // Default destructor

  TAO_GIOP_Message_State message_state_;
  // This keep the state of the current message, to enable
  // non-blocking reads, fragment reassembly, etc.

  // Please see Pluggable.h for documentation
  virtual int idle (void);

  virtual TAO_SHMIOP_SVC_HANDLER *service_handler (void);

private:

  TAO_SHMIOP_Server_Connection_Handler *handler_;
  // the connection service handler used for accessing lower layer
  // communication protocols.
};

#if defined (__ACE_INLINE__)
#include "SHMIOP_Transport.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_HAS_SHMIOP && TAO_HAS_SHMIOP != 0 */

#include "ace/post.h"
#endif  /* TAO_SHMIOP_TRANSPORT_H */

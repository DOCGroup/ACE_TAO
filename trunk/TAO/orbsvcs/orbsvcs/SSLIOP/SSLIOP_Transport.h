// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO_SSLIOP
//
// = FILENAME
//     SSLIOP_Transport.h
//
// = DESCRIPTION
//     SSLIOP Transport specific processing
//
// = AUTHOR
//     Carlos O'Ryan <coryan@ece.uci.edu>
//     Ossama Othman <ossama@ece.uci.edu>
//
// ============================================================================

#ifndef TAO_SSLIOP_TRANSPORT_H
#define TAO_SSLIOP_TRANSPORT_H

#include "ace/pre.h"

#include "tao/Pluggable.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/operation_details.h"
#include "tao/GIOP_Message_State.h"
#include "tao/Pluggable_Messaging_Utils.h"

#include "SSLIOP_Export.h"

// Forward decls.
class TAO_SSLIOP_Handler_Base;
class TAO_SSLIOP_Client_Connection_Handler;
class TAO_SSLIOP_Server_Connection_Handler;
class TAO_ORB_Core;

class TAO_SSLIOP_Export TAO_SSLIOP_Transport : public TAO_Transport
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
  TAO_SSLIOP_Transport (TAO_SSLIOP_Handler_Base *handler,
                      TAO_ORB_Core *orb_core);
  // Base object's creator method.

  ~TAO_SSLIOP_Transport (void);
  // Default destructor.

  TAO_SSLIOP_Handler_Base *&handler (void);
  // Return a reference to the corresponding connection handler.

  // = The TAO_Transport methods, please check the documentation in
  //   "tao/Pluggable.h" for more details.
  virtual void close_connection (void);
  virtual int idle (void);
  virtual ACE_HANDLE handle (void);
  virtual ACE_Event_Handler *event_handler (void);
  virtual ssize_t send (TAO_Stub *stub,
                        int two_way,
                        const ACE_Message_Block *mblk,
                        const ACE_Time_Value *s = 0);
  virtual ssize_t send (const ACE_Message_Block *mblk,
                        const ACE_Time_Value *s = 0);
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

protected:
  TAO_SSLIOP_Handler_Base *handler_;
  // the connection service handler used for accessing lower layer
  // communication protocols.
};

class TAO_SSLIOP_Export TAO_SSLIOP_Client_Transport : public TAO_SSLIOP_Transport
{
  // = TITLE
  //   The Transport class used for Client side communication with a
  //   server.
  //
  // = DESCRIPTION
  //   Specialization of the TAO_SSLIOP_Transport class for client
  //   side.  Methods related to sending one and two way requests
  //   lives here.
public:
  TAO_SSLIOP_Client_Transport (TAO_SSLIOP_Client_Connection_Handler *handler,
                             TAO_ORB_Core *orb_core);
  // Constructor.  Note, TAO_SSLIOP_Handler_Base is the base class for
  // both TAO_SSLIOP_Client_Connection_Handler and
  // TAO_SSLIOP_Server_Connection_Handler.

  ~TAO_SSLIOP_Client_Transport (void);
  // destructor

  // = The TAO_Transport methods, please check the documentation in
  //   "tao/Pluggable.h" for more details.
  virtual void start_request (TAO_ORB_Core *orb_core,
                              TAO_Target_Specification &spec,
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

  int messaging_init (CORBA::Octet major,
                      CORBA::Octet minor);
  // Initialize the messaging object.

  // void use_lite (CORBA::Boolean flag);
  // Use the lite GIOP implementation
  // @@ The lite implementation of GIOP is not supported for SSLIOP
  //    since it introduces security holes.
  //       -Ossama

private:
  TAO_Pluggable_Messaging *client_mesg_factory_;
  // The message_factor instance specific for this particular
  // transport protocol.

  TAO_ORB_Core *orb_core_;
  // Our ORB core

  TAO_Pluggable_Reply_Params params_;
  // The reply data that is sent back by the server
};

// ****************************************************************

class TAO_SSLIOP_Export TAO_SSLIOP_Server_Transport : public TAO_SSLIOP_Transport
{
  // = TITLE
  //   The Transport class used for server communication with a
  //   connected client.
  //
  // = DESCRIPTION
  //   Specialization of the TAO_SSLIOP_Transport class for the server side.
  //   methods for reading messages (requests) and sending replies live
  //   here.
public:

  TAO_SSLIOP_Server_Transport (TAO_SSLIOP_Server_Connection_Handler *handler,
                             TAO_ORB_Core *orb_core);
  //  Default creator method.

  ~TAO_SSLIOP_Server_Transport (void);
  // Default destructor

  TAO_GIOP_Message_State message_state_;
  // This keep the state of the current message, to enable
  // non-blocking reads, fragment reassembly, etc.
};

#include "ace/post.h"

#endif  /* TAO_SSLIOP_TRANSPORT_H */

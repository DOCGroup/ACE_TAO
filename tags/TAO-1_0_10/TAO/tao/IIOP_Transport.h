// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     IIOP_Transport.h
//
// = DESCRIPTION
//     IIOP Transport specific processing
//
// = AUTHOR
//     Fred Kuhns <fredk@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_IIOP_TRANSPORT_H
#define TAO_IIOP_TRANSPORT_H

#include "tao/Pluggable.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/GIOP.h"

// Forward decls.
class TAO_IIOP_Handler_Base;
class TAO_IIOP_Client_Connection_Handler;
class TAO_IIOP_Server_Connection_Handler;
class TAO_ORB_Core;

class TAO_Export TAO_IIOP_Transport : public TAO_Transport
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
  TAO_IIOP_Transport (TAO_IIOP_Handler_Base *handler,
                      TAO_ORB_Core *orb_core);
  // Base object's creator method.

  ~TAO_IIOP_Transport (void);
  // Default destructor.

  TAO_IIOP_Handler_Base *&handler (void);
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

protected:
  TAO_IIOP_Handler_Base *handler_;
  // the connection service handler used for accessing lower layer
  // communication protocols.
};

class TAO_Export TAO_IIOP_Client_Transport : public TAO_IIOP_Transport
{
  // = TITLE
  //   The Transport class used for Client side communication with a
  //   server.
  //
  // = DESCRIPTION
  //   Specialization of the TAO_IIOP_Transport class for client
  //   side.  Methods related to sending one and two way requests
  //   lives here.
public:
  TAO_IIOP_Client_Transport (TAO_IIOP_Client_Connection_Handler *handler,
                             TAO_ORB_Core *orb_core);
  // Constructor.  Note, TAO_IIOP_Handler_Base is the base class for
  // both TAO_IIOP_Client_Connection_Handler and
  // TAO_IIOP_Server_Connection_Handler.

  ~TAO_IIOP_Client_Transport (void);
  // destructor

  TAO_IIOP_Client_Connection_Handler *client_handler (void);
  // return a pointer to the client's connection handler.

  // = The TAO_Transport methods, please check the documentation in
  //   "tao/Pluggable.h" for more details.
  virtual void start_request (TAO_ORB_Core *orb_core,
                              const TAO_Profile *profile,
                              TAO_OutputCDR &output,
                              CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual void start_locate (TAO_ORB_Core *orb_core,
                             const TAO_Profile *profile,
                             CORBA::ULong request_id,
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

private:
  TAO_IIOP_Client_Connection_Handler *client_handler_;
  // pointer to the corresponding client side connection handler.
};

// ****************************************************************

class TAO_Export TAO_IIOP_Server_Transport : public TAO_IIOP_Transport
{
  // = TITLE
  //   The Transport class used for server communication with a
  //   connected client.
  //
  // = DESCRIPTION
  //   Specialization of the TAO_IIOP_Transport class for the server side.
  //   methods for reading messages (requests) and sending replies live
  //   here.
public:

  TAO_IIOP_Server_Transport (TAO_IIOP_Server_Connection_Handler *handler,
                             TAO_ORB_Core *orb_core);
  //  Default creator method.

  ~TAO_IIOP_Server_Transport (void);
  // Default destructor

  TAO_IIOP_Server_Connection_Handler *server_handler_;
  // Pointer to the corresponding connection handler.

  TAO_GIOP_Message_State message_state_;
  // This keep the state of the current message, to enable
  // non-blocking reads, fragment reassembly, etc.
};

#endif  /* TAO_IIOP_TRANSPORT_H */

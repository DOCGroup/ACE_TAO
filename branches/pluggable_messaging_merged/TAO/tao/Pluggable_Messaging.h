// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     Pluggable_Messaging.h
//
// = DESCRIPTION
//     Interface for the TAO pluggable messaging framework.
//
// = AUTHOR
//     Balachandran Natarajan <bala@cs.wustl.edu>
//
// ============================================================================
#ifndef _TAO_PLUGGABLE_MESSAGE_H_
#define _TAO_PLUGGABLE_MESSAGE_H_

#include "tao/corbafwd.h"
#include "tao/Pluggable.h"
#include "tao/target_identifier.h"

class TAO_Message_State_Factory;

typedef enum MessageType
{
  // = DESCRIPTION
  //   Provide an external interface for the users of this pluggable
  //   messaging framework to denote  existing message types. This has
  //   an inspiration from GIOP. So if anybody wants to add more message
  //   types you are welcome but please do not change the numbering
  //   scheme as this would affect GIOP. 
  
  TAO_MESSAGE_REQUEST = 0,                // sent by client.
  TAO_MESSAGE_REPLY = 1,                  // by server.
  TAO_MESSAGE_CANCELREQUEST = 2,          // by client.
  TAO_MESSAGE_LOCATEREQUEST = 3,          // by client.
  TAO_MESSAGE_LOCATEREPLY = 4,
  TAO_MESSAGE_CLOSECONNECTION = 5,        
  TAO_MESSAGE_MESSAGERROR = 6,           // by both.
  TAO_MESSAGE_FRAGMENT = 7                // by both.
}TAO_Pluggable_Message_Type;


class TAO_Export TAO_Pluggable_Client_Message_Factory
{
  // = TITLE
  //   Generic definitions for the Client side of the  Pluggable
  //   Messaging class. 
  //
  // = DESCRIPTION
  //   

public:
  virtual ~TAO_Pluggable_Client_Message_Factory (void);
  // Dtor
  
  virtual int handle_input (TAO_Transport *transport,
                            TAO_ORB_Core *orb_core,
                            TAO_Message_State_Factory &mesg_state,
                            ACE_Time_Value *max_time_value) = 0;

  virtual CORBA::Boolean write_request_header (const IOP::ServiceContextList& svc_ctx,
                                               CORBA::ULong request_id,
                                               CORBA::Octet response_flags,
                                               TAO_Target_Specification &spec,
                                               const char* opname,
                                               TAO_OutputCDR &msg) = 0;
                                               
  // Write the GIOP request header.

  virtual CORBA::Boolean write_locate_request_header (CORBA::ULong request_id,
                                                      TAO_Target_Specification &spec,
                                                      TAO_OutputCDR &msg) = 0;
  // Write the GIOP locate request header.


  virtual int send_message (TAO_Transport *transport,
                            TAO_OutputCDR &stream,
                            ACE_Time_Value *max_wait_time = 0,
                            TAO_Stub *stub = 0) = 0;
  // Send message, returns TRUE if success, else FALSE.

  virtual CORBA::Boolean start_message (TAO_Pluggable_Message_Type t,
                                        TAO_OutputCDR &msg) = 0;
  // Start writing the header for a message in to the stream <msg>

  virtual int parse_reply (TAO_Transport *transport,
                           TAO_Message_State_Factory &state,
                           IOP::ServiceContextList& reply_ctx,
                           CORBA::ULong &request_id,
                           CORBA::ULong &reply_status) = 0;
  // Parse the reply.. 

};

class TAO_Export TAO_Pluggable_Server_Message_Factory
{
  // = TITLE
  //   Generic definitions for the server side of the Pluggable
  //   Message .factory
  //
  // = DESCRIPTION
  //   

public:
  
  virtual ~TAO_Pluggable_Server_Message_Factory (void);
  // Dtor

  /*virtual int send_message (TAO_Transport *transport,
                            TAO_OutputCDR &stream,
                            ACE_Time_Value *max_wait_time = 0,
                            TAO_Stub *stub = 0) = 0;*/
  // Send message, returns TRUE if success, else FALSE.

  virtual int handle_input (TAO_Transport *transport,
                            TAO_ORB_Core *orb_core,
                            TAO_Message_State_Factory &state,
                            ACE_Time_Value *max_wait_time = 0) = 0;

  
  virtual int process_connector_messages (TAO_Transport *transport,
                                         TAO_ORB_Core *orb_core,
                                         TAO_InputCDR &input,
                                         CORBA::Octet message_type) = 0;
  // Process messages from the connectors
};



class TAO_Export TAO_Message_State_Factory
{
  // = TITLE
  //   Generic definitions for Message States.  
  //
  // = DESCRIPTION
  //   This would represnt the state of the incoming message states.
  //   As the ORB processes incoming messages it need to keep track of
  //   how much of the message has been read. if there are any
  //   fragments following this message etc. This class attempts to
  //   give a generic interface to all the messaging protocols message
  //   states so that the Transport layer does not really know with
  //   whom it is interacting with.
public:
  virtual ~TAO_Message_State_Factory (void);
  // Dtor
  
  virtual void reset (int reset_contents = 1) = 0;
  // Reset the message header state and prepare it to receive the next
  // event.
  
  virtual CORBA::Boolean  header_received (void) const = 0;
  // Has the header been received?

  virtual int is_complete (void) = 0;
  // Check if the current message is complete, adjusting the fragments
  // if required...
  
  
private:
  
};


#if defined (__ACE_INLINE__)
# include "tao/Pluggable_Messaging.i"
#endif /* __ACE_INLINE__ */

#endif /*_TAO_PLUGGABLE_MESSAGE_H_*/

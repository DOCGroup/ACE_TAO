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
#include "tao/Pluggable_Messaging_Utils.h"

class TAO_Message_State_Factory;

class TAO_Export TAO_Pluggable_Messaging_Interface
{
  // = TITLE
  //   Generic definitions  Messaging class. 
  //
  // = DESCRIPTION
  //   

public:
  virtual ~TAO_Pluggable_Messaging_Interface (void);
  // Dtor
  
  virtual int handle_input (TAO_Transport *transport,
                            TAO_ORB_Core *orb_core,
                            TAO_Message_State_Factory &mesg_state,
                            ACE_Time_Value *max_time_value) = 0;

  virtual CORBA::Boolean 
  write_message_header (const TAO_Pluggable_Connector_Params &params,
                        TAO_Pluggable_Header_Type header_type,
                        TAO_Target_Specification &spec,
                        TAO_OutputCDR &msg) = 0;
                                               
  // Write the  header.

  virtual int send_message (TAO_Transport *transport,
                            TAO_OutputCDR &stream,
                            ACE_Time_Value *max_wait_time = 0,
                            TAO_Stub *stub = 0) = 0;
  // Send message, returns TRUE if success, else FALSE.

  virtual CORBA::Boolean write_protocol_header (TAO_Pluggable_Message_Type t,
                                                TAO_OutputCDR &msg) = 0;
  // Start writing the header for a message in to the stream <msg>

  
  virtual int parse_reply (TAO_Message_State_Factory &state,
                           TAO_Pluggable_Connector_Params &params,
                           CORBA::ULong &reply_status) = 0;
  // Parse the reply.. 

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

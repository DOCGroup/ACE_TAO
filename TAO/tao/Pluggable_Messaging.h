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
  //   This interface tries to define generic methods that could be
  //   different messaging protocols

public:
  virtual ~TAO_Pluggable_Messaging_Interface (void);
  // Dtor
  
  virtual int handle_input (TAO_Transport *transport,
                            TAO_ORB_Core *orb_core,
                            TAO_Message_State_Factory &mesg_state,
                            ACE_Time_Value *max_time_value) = 0;
  // This method reads the input from the transport layer.

  virtual int send_message (TAO_Transport *transport,
                            TAO_OutputCDR &stream,
                            ACE_Time_Value *max_wait_time = 0,
                            TAO_Stub *stub = 0) = 0;
  // This is a complement of the previous method. This method sends
  // the CDR through the transport layer
  
  virtual CORBA::Boolean 
  write_message_header (const TAO_Pluggable_Connector_Params &params,
                        TAO_Pluggable_Header_Type header_type,
                        TAO_Target_Specification &spec,
                        TAO_OutputCDR &msg) = 0;
                                               

  // Start writing the header of a message type stream <msg>. This is
  // influenced by GIOP, which has the protocol header, followed by
  // the message specific header with the message at the end.


  virtual CORBA::Boolean write_protocol_header (TAO_Pluggable_Message_Type t,
                                                TAO_OutputCDR &msg) 
    = 0; 
  // This is a generic method that is used to write the protocol
  // header in to the Output CDR stream. This may not be really
  // necessary, but our Invocation classes seesm to be looking for
  // something like this. Further, the invocation classes seem to do
  // what our IDL compiler wants.
  

  
  virtual int parse_reply (TAO_Message_State_Factory &state,
                           TAO_Pluggable_Connector_Params &params,
                           CORBA::ULong &reply_status) = 0;
  // Parse the reply.. 

  virtual int process_connector_messages (TAO_Transport *transport,
                                          TAO_ORB_Core *orb_core,
                                          TAO_InputCDR &input,
                                          CORBA::Octet message_type) = 0;
  // Process messages from the connectors. This is the hert of the
  // server side processing 
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

  // @@The above comments are the intent. Will be doing so in the next
  // stage of work.
public:
  virtual ~TAO_Message_State_Factory (void);
  // Dtor
  
  virtual void reset (int reset_contents = 1) = 0;
  // Reset the message header state and prepare it to receive the next
  // event.
};


#if defined (__ACE_INLINE__)
# include "tao/Pluggable_Messaging.i"
#endif /* __ACE_INLINE__ */

#endif /*_TAO_PLUGGABLE_MESSAGE_H_*/

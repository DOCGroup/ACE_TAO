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

// @@ Bala: please try to be consistent among:
//    - The name of the file
//    - The name of the macro protecting the file against multiple
//    #includes
//    - The name of the class or classes in the file.
//
// In this case you have: Pluggable_Messaging.h, PLUGGABLE_MESSAGE_H
// and Pluggable_Messaging_Interface.
//
#ifndef TAO_PLUGGABLE_MESSAGE_H
#define TAO_PLUGGABLE_MESSAGE_H

#include "tao/corbafwd.h"

// @@ It seems like you don't need to #include all this stuff, please
//    use forward declarations when possible.  The rules are not that
//    bad, if you only need pointers and references a forward
//    reference is enough.

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
                            TAO_Stub *stub = 0,
                            int two_way = 1) = 0;
  // This is a complement of the previous method. This method sends
  // the CDR through the transport layer

  // @@ Bala: here is one example of things that should be more
  // decoupled, for example you could have methods
  //
  // write_request_header()
  // write_reply_header()
  // write_locate_request_header()
  //
  // of course some of them would return an error if the pluggable
  // messaging protocol does not support it.  Furthermore, maybe the
  // right approach is to have *each* pluggable messaging define its
  // own methods.  Then the pluggable transport will deal with them.
  // Don't jump into this yet, it is only an idea for discussion.
  //
  virtual CORBA::Boolean 
  write_message_header (const TAO_Operation_Details &opdetails,
                        TAO_Pluggable_Header_Type header_type,
                        TAO_Target_Specification &spec,
                        TAO_OutputCDR &msg) = 0;
                                               

  // Start writing the header of a message type stream <msg>. This is
  // influenced by GIOP, which has the protocol header, followed by
  // the message specific header with the message at the end.

  // @@ Bala: What if the protocol only has message headers and not
  //    'protocol headers'?
  virtual CORBA::Boolean write_protocol_header (TAO_Pluggable_Message_Type t,
                                                TAO_OutputCDR &msg) 
    = 0; 
  // This is a generic method that is used to write the protocol
  // header in to the Output CDR stream. This may not be really
  // necessary, but our Invocation classes seesm to be looking for
  // something like this. Further, the invocation classes seem to do
  // what our IDL compiler wants.

  // @@ Bala: Please see my comments on the Connector_Params
  // struct.  BTW, 'Connector_Params' is a horrible name.  "request
  // params" or something like that sounds better.
  virtual int parse_reply (TAO_Message_State_Factory &state,
                           TAO_Pluggable_Connector_Params &params) = 0;
  // Parse the reply.. 

  // @@ Bala: calling this 'connector' is confusing, the Connector and
  // Acceptor patterns are engraved in our minds to mean
  // something different.
  // @@ I believe that 'process message' is a better name, first, it
  // is a single message that you are processing (singular), next it
  // is a generic message, there is nothing 'Connector' about it.
  // @@ I just thought that you may mean Connector as in IIOP_Connect
  // or UIOP_Connect files.  Those files are "accidental" from the
  // perspective of the pluggable protocols framework.  Think about
  // them as dirty laundry ;-) ;-)
  virtual int process_connector_messages (TAO_Transport *transport,
                                          TAO_ORB_Core *orb_core,
                                          TAO_InputCDR &input,
                                          CORBA::Octet message_type) = 0;
  // Process messages from the connectors. This is the hert of the
  // server side processing 

protected:
  // @@ Bala: is this a good name? Why not just "send mesage", or
  // something like that?
  int transport_message (TAO_Transport *transport,
                         TAO_OutputCDR &stream,
                         int two_way = 1,
                         TAO_Stub *stub = 0,
                         ACE_Time_Value *max_wait_time = 0);
  // This method uses the Transport <transport> objects send () method
  // to send the contents in <stream>. Most of the derived classes, at
  // one stage or other would tend to send the data to the socket. So,
  // it is better if we have the method that does the sending and
  // error checking in a seperate place for ease of maintenance.
};

// @@ Bala: note the separator
// ****************************************************************

class TAO_Export TAO_Message_State_Factory
{
  // = TITLE
  //   Generic definitions for Message States.  
  //
  // = DESCRIPTION
  //   @@ Bala: please read your first sentence, it makes no sense..
  //   This would represnt the state of the incoming message states.
  //
  //   @@ Bala: how do you know if other protocols support fragments,
  //      or need them? What about Dgram based protocol where there
  //      are no partial reads?
  //      
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

#endif /*TAO_PLUGGABLE_MESSAGE_H*/

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
//
#ifndef TAO_PLUGGABLE_MESSAGING_H
#define TAO_PLUGGABLE_MESSAGING_H
#include "ace/pre.h"

#include "tao/Pluggable_Messaging_Utils.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Message_State_Factory;
class TAO_Target_Specification;
class TAO_Pluggable_Reply_Params;
class TAO_Transport;
class TAO_Operation_Details;
class TAO_Target_Specification;
class TAO_OutputCDR;

class TAO_Export TAO_Pluggable_Messaging
{
  // = TITLE
  //   Generic definitions  Messaging class.
  //
  // = DESCRIPTION
  //   This interface tries to define generic methods that could be
  //   different messaging protocols

public:
  virtual ~TAO_Pluggable_Messaging (void);
  // Dtor

  // @@ Bala: only the pluggable transport protocols can invoke the
  // handle_input() method, but they must know what messaging protocol
  // is in place already.  In consequence this method should not be
  // part of the public interface for Pluggable_Messaging....
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
  // @@Carlos: I see what you are saying, but we would be expanding
  // this interface for all the protocols that we intend to
  // support or the users would like to use. I would tend to leave the
  // implementor of the messaging layer to decide his own
  // implementation by wading through a minimal set of interface. I
  // probably dont like the way we have things in TAO_Transport
  // class.
  // An after thought. I looked at your comments in the
  // Pluggable_Messaging_utils.h and I feel that we can start exposing
  // methods the way you have suggested. I will start that too.
  //
  // @@ Bala: another way to say the same thing: what services are
  // provided by a pluggable message protocol:
  //   - Generate request messages
  //   - Generate locate request messages
  //   - Send a full message
  //   - Receive a full message
  //   - Parse a full message into
  //     - Request
  //     - Locate
  //     - Reply
  //     - LocateReply
  //
  // @@ Other messages, like Fragment could be hidden from the higher
  // level components of the ORB, because they don't care.  What do
  // you think?
  //
  virtual CORBA::Boolean
  write_message_header (const TAO_Operation_Details &opdetails,
                        TAO_Pluggable_Header_Type header_type,
                        TAO_Target_Specification &spec,
                        TAO_OutputCDR &msg) = 0;
  // Start writing the header of a message type stream <msg>. This is
  // influenced by GIOP, which has the protocol header, followed by
  // the message specific header with the message at the end.

  virtual CORBA::Boolean
  write_reply_header (TAO_OutputCDR &cdr,
                      TAO_Pluggable_Reply_Params &params,
                      CORBA::Environment &ACE_TRY_ENV =
                      TAO_default_environment ())
     ACE_THROW_SPEC ((CORBA::SystemException)) = 0;
  // Write the reply header
  // The reply header is a part of any messaging protocol. The
  // messaging protocol implementation would implement the way the
  // reply header is written. 

  // @@ Bala: What if the protocol only has message headers and not
  //    'protocol headers'?
  // @@ Carlos: The same comment that you gave above. They have to
  // return an error.
  virtual CORBA::Boolean write_protocol_header (TAO_Pluggable_Message_Type t,
                                                TAO_OutputCDR &msg) = 0;
  // This is a generic method that is used to write the protocol
  // header in to the Output CDR stream. This may not be really
  // necessary, but our Invocation classes seesm to be looking for
  // something like this. Further, the invocation classes seem to do
  // what our IDL compiler wants.

  virtual int parse_reply (TAO_Message_State_Factory &state,
                           TAO_Pluggable_Reply_Params &params) = 0;
  // Parse the reply..

  // @@ Bala: it seems to me this is a counter-intuitive name.
  // Furthermore, why should the caller know that there are certain
  // messages that the server should not receive?
  // @@ Bala: this is the same principle that we applied before, we
  // need to think about what services is the Pluggable_Messaging
  // layer going to provide to the rest of the ORB.  IMHO what it
  // should provide is the ability to:
  //   1 Prepare and send requests.
  //   2 Prepare and send locate request messages
  //   3 Upcall to 'something' when a reply has arrived
  //   4 Upcall to 'something' when a request has arrived
  //
  // @@ (1) and (2) imply a transfer syntax, for the time beign we
  // will hardcode it to CDR, but we must think in more generic
  // terms.
  // @@ Notice that sending and receiving are provided as part of a
  // single service, you must keep in mind that certain protocols
  // (even GIOP) could send the message in multiple fragments, so
  // while the higher level components in the ORB believe that they
  // are just preparing a message, in fact parts of it may have been
  // sent already.  The application may request a flush() operation or
  // 'message_complete()' or something like that....
  //  @@ Also notice that the replies are requests are upcalls, that
  //  is how we organized the transport layer, and it cleaned up a
  //  bunch of stuff. For example, there would be no need to expose
  //  'parse' operations, the parsing would be done by the pluggable
  //  messaging layer, and only when completed it will upcall and pass
  //  the resulting reply (or request) to the upper ORB layer.
  //  @@ In my mind, all this stuff should be provided by the
  //  transport layer, 'pluggable messaging' makes no sense because
  //  you cannot plug a messaging protocol and use transport classes
  //  created for another protocol (except in the GIOP / GIOPLite
  //  case, which is purely an accident).
  //    
  virtual int process_client_message (TAO_Transport *transport,
                                      TAO_ORB_Core *orb_core,
                                      TAO_InputCDR &input,
                                      CORBA::Octet message_type) = 0;
  // Process messages from the clients. This is the heart of the
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

// ****************************************************************

// @@ Bala: if this is a *factory* why is it that it does not create
// *anything*?  Wouldn't TAO_Message_State be a much better name?
class TAO_Export TAO_Message_State_Factory
{
  // = TITLE
  //   Generic definitions for Message States.
  //
  // = DESCRIPTION
  //   This would represent the state of the incoming message.
  //   As the ORB processes incoming messages it need to keep track of
  //   how much of the message has been read. if there are any
  //   fragments following this message etc. This class attempts to
  //   give a generic interface to all the messaging protocols message
  //   states so that the Transport layer does not really know with
  //   whom it is interacting with.
  // @@The above comments are the intent. Will be doing so in the next
  // stage of work.
  //   @@ Bala: how do you know if other protocols support fragments,
  //      or need them? What about Dgram based protocol where there
  //      are no partial reads?
  //   @@ Carlos: I don't know. But that is why we are trying to build
  //      abstract interface.  I opine that if an user wants to
  //      implement a new protocol instead of GIOP, I would assume
  //      that he may want to use our Invocation and reply despatcher
  //      classes for SMI, AMI, DII deferred etc.  Further I would
  //      also assume that he would use our Transport_Mux_Strategy. I
  //      would agree if you would say that the Transport_Mux_Strategy
  //      classes are specific to IIOP/UIOP and the user should be .  I put
  //      this class thinking some gurus like you & Ossama, would try
  //      extending the existing Mux strategy.  If at all we had such
  //      an idea, we should be able to mask them from having an idea
  //      that they are using GIOP. Under such circumstances, we may
  //      want to give them an interface through which they can access
  //      data that they need and leave the rest to the Transport
  //      classes.
  //


public:
  virtual ~TAO_Message_State_Factory (void);
  // Dtor

  virtual void reset (int reset_contents = 1) = 0;
  // Reset the message header state and prepare it to receive the next
  // event.
  // @@ Bala: I fail to see how a class with just a meak interface is
  // reusable.  In all instances where i seen it used it was
  // downcasted, usually the flow was something like:
  //   - In context A we have a concrete Message_State, we decide to
  //   forget its real type and pass it down to context B as a generic
  //   Message_State_Factory
  //   - In context B we downcast from the generic
  //   Message_State_Factory to the concrete message state, because it
  //   is the only way to do something useful...
  // IMHO this abstraction is not giving you anything.
};


#if defined (__ACE_INLINE__)
# include "tao/Pluggable_Messaging.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /*TAO_PLUGGABLE_MESSAGING_H*/

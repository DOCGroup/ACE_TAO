// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     GIOP_Message.h
//
// = DESCRIPTION
//     Interface for the GIOP messaging protocol
//
// = AUTHOR
//     Balachandran Natarajan <bala@cs.wustl.edu>
//
// ============================================================================
#ifndef _TAO_GIOP_UTILS_H_
#define _TAO_GIOP_UTILS_H_
#include "tao/Pluggable_Messaging.h"


typedef enum GIOP_Messages
{
  // = DESCRIPTION
  //   All GIOP messages include a header and message type.  Not
  //   really a message type, but needed to bring that information
  //   back somehow.
  
  // = GIOP message types.
  TAO_GIOP_REQUEST = 0,                // sent by client.
  TAO_GIOP_REPLY = 1,                  // by server.
  TAO_GIOP_CANCELREQUEST = 2,          // by client.
  TAO_GIOP_LOCATEREQUEST = 3,          // by client.
  TAO_GIOP_LOCATEREPLY = 4,
  TAO_GIOP_CLOSECONNECTION = 5,
  TAO_GIOP_MESSAGERROR = 6,           // by both.
  TAO_GIOP_FRAGMENT = 7                // by both.
}TAO_GIOP_Message_Type;



class TAO_Export TAO_GIOP_Version
{
  // = TITLE
  //   Major and Minor version number of the Inter-ORB Protocol.
public:
  CORBA::Octet major;
  // Major version number

  CORBA::Octet minor;
  // Minor version number

  TAO_GIOP_Version (const TAO_GIOP_Version &src);
  // Copy constructor

  TAO_GIOP_Version (CORBA::Octet maj = TAO_DEF_GIOP_MAJOR,
                    CORBA::Octet min = TAO_DEF_GIOP_MINOR);
  // Default constructor.

  ~TAO_GIOP_Version (void);
  // Destructor.

  void set_version (CORBA::Octet maj, CORBA::Octet min);
  // Explicitly set the major and minor version.

  TAO_GIOP_Version &operator= (const TAO_GIOP_Version &src);
  // Copy operator.

  int operator== (const TAO_GIOP_Version &src);
  int operator!= (const TAO_GIOP_Version &src);
  // Equality operator
};


class TAO_Export TAO_GIOP_Message_State: public TAO_Message_State_Factory
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
  TAO_GIOP_Message_State (TAO_ORB_Core *orb_core);
  // Ctor

  ~TAO_GIOP_Message_State (void);
  // Dtor
  
  void reset (int reset_contents = 1);
  //Reset the message header state and prepare it to receive the next
  // event.
  
  CORBA::Boolean  header_received (void) const;
  // Has the header been received?

  int is_complete (void);
  // Check if the current message is complete, adjusting the fragments
  // if required...

  TAO_GIOP_Version giop_version; 
  // Version info

  CORBA::Octet byte_order;       
  // 0 = big, 1 = little
  
  CORBA::Octet more_fragments;   
  // (Requests and Replys)
  
  CORBA::Octet message_type;     
  // MsgType above
  
  CORBA::ULong message_size;     
  // in byte_order!

  CORBA::ULong current_offset;
  // How much of the payload has been received

  TAO_InputCDR cdr;
  // This is the InputCDR that will be used to decode the message.

  ACE_Message_Block* fragments_begin;
  ACE_Message_Block* fragments_end;
  // The fragments are collected in a chain of message blocks (using
  // the cont() field).  When the complete message is received the
  // chain is reassembled into <cdr>

  CORBA::Octet first_fragment_byte_order;
  // The byte order for the the first fragment
  // @@ The current implementation cannot handle fragments with
  //    different byte orders, this should not be a major problem
  //    because:
  //    1) It is unlikely that we are going to receive fragments.
  //    2) The spec *seems* to allow different byte_orders, but it is
  //       unlikely that any ORB will do that.
  //    3) Even if we allowed that at this layer the CDR classes are
  //       not prepared to handle that.

  TAO_GIOP_Version first_fragment_giop_version;
  // The GIOP version for the first fragment
  // @@ Same as above, all GIOP versions must match.

  CORBA::Octet first_fragment_message_type;
  // If the messages are chained this represents the message type for
  // the *complete* message (remember that the last message will be
  // fragment and the upper level needs to know if it is a request,
  // locate request or what).
  
private:
  int append_fragment (ACE_Message_Block* current);
  // Append <current> to the list of fragments
  // Also resets the state, because the current message was consumed.

};



class TAO_Export TAO_GIOP_Utils
{
  // = TITLE
  //   Some utility methods for GIOP independent of versions of GIOP
  //
  // = DESCRIPTION
  //   Utility methods that would be used by both the Client and
  //   Server side methods in GIOP factory

public:
  
  static void dump_msg (const char *label,
                        const u_char *ptr,
                        size_t len);
  // Print out a debug messages..  

  static CORBA::Boolean start_message (const TAO_GIOP_Version &version,
                                       TAO_Pluggable_Message_Type t,
                                       TAO_OutputCDR &msg);
  
  // Build the header for a message of type <t> into stream <msg>.
                                       

};

// Some constant definitions that would not change for sometime. I
// think. So we can have them here till they change
const size_t TAO_GIOP_HEADER_LEN = 12;
const size_t TAO_GIOP_LITE_HEADER_LEN = 5;

const size_t TAO_GIOP_VERSION_MINOR_OFFSET = 5;
const size_t TAO_GIOP_VERSION_MAJOR_OFFSET = 4;
const size_t TAO_GIOP_MESSAGE_FLAGS_OFFSET = 6;
const size_t TAO_GIOP_MESSAGE_TYPE_OFFSET  = 7;
const size_t TAO_GIOP_MESSAGE_SIZE_OFFSET  = 8;

#if defined (__ACE_INLINE__)
# include "tao/GIOP_Utils.i"
#endif /* __ACE_INLINE__ */

#endif /*_TAO_GIOP_UTILS_H_ */

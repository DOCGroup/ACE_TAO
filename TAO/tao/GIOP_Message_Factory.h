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
//     Copyright 1994-1995 by Sun Microsystems Inc.,
//     Balachandran Natarajan <bala@cs.wustl.edu>
//
// ============================================================================
#ifndef _TAO_GIOP_MESSAGE_H_
#define _TAO_GIOP_MESSAGE_H_

#include "tao/Pluggable_Messaging.h"
#include "tao/debug.h"


class TAO_Export TAO_GIOP_Message_Factory :public TAO_Pluggable_Message_Factory
{
  // = TITLE
  //   Definitions of GIOP specific stuff
  //
  // = DESCRIPTION
  //   This class will hold the specific details common to all the
  //   GIOP versions. Some of them which are here may be shifted if
  //   things start changing between versions

  // IMPORTANT: This code was based on the GIOP.h & GIOP.cpp
  
 public:
  TAO_GIOP_Message_Factory (void);
  
  virtual ~TAO_GIOP_Message_Factory (void);
  // Dtor

  enum Message_Type
  {
    // = DESCRIPTION
    //   All GIOP messages include a header and message type.  Not
    //   really a message type, but needed to bring that information
    //   back somehow.

    // = GIOP message types.
    Request = 0,                // sent by client.
    Reply = 1,                  // by server.
    CancelRequest = 2,          // by client.
    LocateRequest = 3,          // by client.
    LocateReply = 4,            // by server.
    CloseConnection = 5,        // by server.
    MessageError = 6,           // by both.
    Fragment = 7                // by both.
  };

  
  /**********************************************************/
  // Methods related to the messages that would be sent by the client
  /**********************************************************/
  virtual CORBA::Boolean write_request_header (const IOP::ServiceContextList& svc_ctx,
                                               CORBA::ULong request_id,
                                               CORBA::Octet response_flags,
                                               TAO_Stub *stub,
                                               const CORBA::Short address_disposition,
                                               const char* opname,
                                               TAO_OutputCDR &msg);
                                               
  // Write the GIOP request header.

  CORBA::Boolean write_locate_request_header (CORBA::ULong request_id,
                                              TAO_Stub *stub,
                                              const CORBA::Short address_disposition,
                                              TAO_OutputCDR &msg) = 0;
  // Write the GIOP locate request header.


  virtual int send_message (TAO_Transport *transport,
                            TAO_OutputCDR &stream,
                            ACE_Time_Value *max_wait_time = 0,
                            TAO_Stub *stub = 0);
  // Send message, returns TRUE if success, else FALSE.
  
  virtual const size_t get_header_len (void) = 0;
  // This will give the size of the header for different variants of
  // GIOP. 

  virtual const size_t get_message_size_offset (void) = 0;
  // This will give the message_size offset as specified by different
  // variants of GIOP

  void dump_msg (const char *label,
                 const u_char *ptr,
                 size_t len);
  // Print out a debug messages..

  // We need to add a Cancel request. But TAO does not support.


  /**********************************************************/
  // Methods related to the messages that would be sent by the server. 
  /**********************************************************/
  //virtual CORBA::Boolean start_message (const TAO_GIOP_Version &version,
  //                                    TAO_GIOP_Message_Factory::Message_Type t,
  //                                    TAO_OutputCDR &msg);
  // Build the header for a message of type <t> into stream
  // <msg>. Other GIOP related protocols that do not use this can
  // override this. Like GIOP_lite
private:

};

#if defined (__ACE_INLINE__)
# include "tao/GIOP_Message_Factory.i"
#endif /* __ACE_INLINE__ */

#endif /*_TAO_GIOP_MESSAGE_H_*/

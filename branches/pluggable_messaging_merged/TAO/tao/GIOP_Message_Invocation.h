// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     GIOP_Message_Invocation.h
//
// = DESCRIPTION
//     Interface for the GIOP version 1.1 of the protocol
//
// = AUTHOR
//     Balachandran Natarajan <bala@cs.wustl.edu>
//
// ============================================================================
#ifndef _TAO_GIOP_MESSAGE_1_1_H_
#define _TAO_GIOP_MESSAGE_1_1_H_

#include "tao/GIOP_Message_Factory.h"

class TAO_Export TAO_GIOP_Client_Message_1_1:
  public TAO_GIOP_Client_Message_Factory
{
  // = TITLE
  //   Definitions of GIOP 1.1 specific stuff
  //
  // = DESCRIPTION
  //   This class will hold the specific details of 1.1 
  //   
public:
  
  TAO_GIOP_Client_Message_1_1 (void);
  // Ctor

  ~TAO_GIOP_Client_Message_1_1 (void);
  // Dtor

  int parse_magic_bytes (TAO_InputCDR &input);
    
  int parse_header (TAO_GIOP_Message_State *mesg_state);

  CORBA::Boolean
  write_request_header (const IOP::ServiceContextList& svc_ctx,
                        CORBA::ULong request_id,
                        CORBA::Octet response_flags,
                        TAO_Target_Specification &spec,
                        const char *opname,
                        TAO_OutputCDR &msg);
  // Writes the rquest header.

  CORBA::Boolean write_locate_request_header (CORBA::ULong request_id,
                                              TAO_Target_Specification &spec,
                                              TAO_OutputCDR &msg);
  // Write the locate request header


  CORBA::Boolean start_message (TAO_Pluggable_Message_Type t,
                                TAO_OutputCDR &msg);

  int parse_reply (TAO_Transport *transport,
                   TAO_Message_State_Factory &mesg_state,
                   IOP::ServiceContextList& reply_ctx,
                   CORBA::ULong &request_id,
                   CORBA::ULong &reply_status);
  // Parse the reply message from the server
  
  const size_t get_header_len (void);
  // Returns the header length 
  
  const size_t get_message_size_offset (void);
  // Returns the message size offset
  

private:
  CORBA::Octet our_major_version_;
  CORBA::Octet our_minor_version_;
};


#if defined (__ACE_INLINE__)
# include "tao/GIOP_Message_Invocation.i"
#endif /* __ACE_INLINE__ */

#endif /*TAO_IIOP_ACCEPTOR_1_1_H_ */

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

  CORBA::Boolean
  write_request_header (const IOP::ServiceContextList& svc_ctx,
                        CORBA::ULong request_id,
                        CORBA::Octet response_flags,
                        TAO_Stub *stub,
                        const CORBA::Short address_disposition,
                        const char *opname,
                        TAO_OutputCDR &msg);
  // Writes the rquest header.

  CORBA::Boolean write_locate_request_header (CORBA::ULong request_id,
                                              TAO_Stub *stub,
                                              const CORBA::Short address_disposition,
                                              TAO_OutputCDR &msg);
  // Write the locate request header

private:
  const size_t get_header_len (void);
  // Returns the header length 

  const size_t get_message_size_offset (void);
  // Returns the message size offset
};



const size_t TAO_GIOP_1_1_HEADER_LEN = 12;
const size_t TAO_GIOP_1_1_MESSAGE_SIZE_OFFSET = 8;

#if defined (__ACE_INLINE__)
# include "tao/GIOP_Message_1_1.i"
#endif /* __ACE_INLINE__ */

#endif /*TAO_IIOP_ACCEPTOR_1_1_H_ */

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

class TAO_Export TAO_Pluggable_Message_Factory
{
  // = TITLE
  //   Generic definitions for the new Pluggable Messaging class.
  //
  // = DESCRIPTION
  //   

public:
  
  TAO_Pluggable_Message_Factory (void);
  // Ctor

  virtual ~TAO_Pluggable_Message_Factory (void);
  // Dtor

  /**********************************************************/
  // Methods related to the messages that would be sent by the client
  /**********************************************************/
  virtual CORBA::Boolean write_request_header (const IOP::ServiceContextList& svc_ctx,
                                               CORBA::ULong request_id,
                                               CORBA::Octet response_flags,
                                               TAO_Stub *stub,
                                               const CORBA::Short address_disposition,
                                               const char* opname,
                                               TAO_OutputCDR &msg) = 0;
                                               
  // Write the GIOP request header.

  virtual CORBA::Boolean write_locate_request_header (CORBA::ULong request_id,
                                                      TAO_Stub *stub,
                                                      const CORBA::Short address_disposition,
                                                      TAO_OutputCDR &msg) = 0;
  // Write the GIOP locate request header.


  virtual int send_message (TAO_Transport *transport,
                            TAO_OutputCDR &stream,
                            ACE_Time_Value *max_wait_time = 0,
                            TAO_Stub *stub = 0) = 0;
  // Send message, returns TRUE if success, else FALSE.



  /**********************************************************/
  // Methods related to the messages that would be sent by the server. 
  /**********************************************************/
  //virtual CORBA::Boolean start_message (const TAO_GIOP_Version &version,
  //                                    TAO_GIOP_Message_Factory::Message_Type t,
  //                                    TAO_OutputCDR &msg);
  // Build the header for a message of type <t> into stream
  // <msg>. Other GIOP related protocols that do not use this can
  // override this. Like GIOP_lite

};


#if defined (__ACE_INLINE__)
# include "tao/Pluggable_Messaging.i"
#endif /* __ACE_INLINE__ */

#endif /*_TAO_PLUGGABLE_MESSAGE_H_*/

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
#ifndef _TAO_GIOP_MESSAGE_H_
#define _TAO_GIOP_MESSAGE_H_



class TAO_Export TAO_GIOP_Message_Factory :public TAO_Pluggable_Message
{
  // = TITLE
  //   Definitions of GIOP specific stuff
  //
  // = DESCRIPTION
  //   This class will hold the specific details common to all the
  //   GIOP versions. Some of them which are here may be shifted if
  //   things start changing between versions
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

  
  virtual CORBA::Boolean start_message (const TAO_GIOP_Version &version,
                                        TAO_GIOP_Message_Factory::Message_Type t,
                                        TAO_OutputCDR &msg);
  // Build the header for a message of type <t> into stream
  // <msg>. Other GIOP related protocols that do not use this can
  // override this. Like GIOP_lite

  virtual CORBA::Boolean write_request_header (const IOP::ServiceContextList& svc_ctx,
                                               CORBA::ULong request_id,
                                               CORBA::Octet response_flags,
                                               const TAO_opaque& key,
                                               const char* opname,
                                               CORBA::Principal_ptr principal,
                                               TAO_OutputCDR &msg,
                                               TAO_ORB_Core *orb_core);
  // Write the GIOP request header.

  CORBA::Boolean write_locate_request_header (CORBA::ULong request_id,
                                              const TAO_opaque &key,
                                              TAO_OutputCDR &msg);
  // Write the GIOP locate request header.

  int send_message (TAO_Transport *transport,
                    TAO_OutputCDR &stream,
                    TAO_ORB_Core* orb_core,
                    ACE_Time_Value *max_wait_time = 0,
                    TAO_Stub *stub = 0);
  // Send message, returns TRUE if success, else FALSE.
  
private:

};

#endif /*_TAO_GIOP_MESSAGE_H_*/

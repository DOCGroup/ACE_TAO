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
#ifndef _TAO_GIOP_SERVER_STATE_
#define _TAO_GIOP_SERVER_STATE_
#include "tao/GIOP_Utils.h"
#include "tao/GIOP_Assorted_Headers.h"

class TAO_GIOP_ServerRequest;


class TAO_Export TAO_GIOP_Accept_State
{
  // = TITLE
  //   An abstract interface for different versions of the GIOP
  //   implementation classes.
  //
  // = DESCRIPTION
  //
  //
public:

  int parse_header (TAO_GIOP_Message_State *state);
  // Parse the Message header 

  int read_header (TAO_GIOP_Message_State *state);
  
  int  send_error (TAO_Transport *transport);

  // Send an error message 

  void send_close_connection (const TAO_GIOP_Version &version,
                              TAO_Transport *transport,
                              void *ctx);
  // Close a connection, first sending GIOP::CloseConnection.

  int process_connector_request (TAO_Transport *transport,
                                 TAO_ORB_Core* orb_core,
                                 TAO_InputCDR &input,
                                 TAO_OutputCDR &output);
  // A request was received on the server side. <transport> is the
  // source of the message (and thus where the  replies should be
  // sent). <orb_core> is the ORB that received the message <input>
  // contains the message <output> can be used to store any responses
  // <request_id> and <response_required> are set as part of the
  // message processing. 
  
  
  int process_connector_locate (TAO_Transport *transport,
                                TAO_ORB_Core* orb_core,
                                TAO_InputCDR &input,
                                TAO_OutputCDR &output);
  // A LocateRequest was received on the server side. <transport> is
  // the source of the message (and thus where the replies should be
  // sent). <orb_core> is the ORB that received the message <input>
  // contains the message <output> can be used to store any responses
  // <request_id> and <response_required> are set as part of the
  // message processing. 
  
  virtual const CORBA::Octet& major_version (void) = 0;
  virtual const CORBA::Octet& minor_version (void) = 0;
  // Return the majopr and minor revsions
  
protected :
  virtual int parse_request_header (TAO_GIOP_ServerRequest &) = 0; 
  // Parse the Request Header from the incoming stream. This will do a
  // version specific parsing of the incoming Request header

  virtual int make_reply (const TAO_GIOP_Version &version,
                          TAO_GIOP_Message_Type t,
                          CORBA::ULong request_id,
                          TAO_OutputCDR &) = 0;
  // This would prepare a version specific reply to the 
  // messages from the client. The type of reply would be context
  // sensitive. Only the common stuff for all the replies to be
  // sent would be handled. 
  
  virtual int parse_locate_header (TAO_GIOP_Locate_Request_Header &) = 0; 
  // Parse the Loacte Request Header from the incoming stream. This will do a
  // version specific parsing of the incoming Request header

  
  virtual int make_locate_reply (TAO_Transport *transport,
                                 const TAO_GIOP_Version & version,
                                 TAO_OutputCDR & output,
                                 TAO_GIOP_Locate_Request_Header &request,
                                 TAO_GIOP_Locate_Status_Msg &status);
  // As on date 1.1 & 1.2 seem to have similar headers. Til an
  // unmanageable difference comes let them be implemented here. 

  virtual size_t header_length (void) = 0;
  // Return the header length of the GIOP message formats 

  virtual size_t offset_length (void) = 0;
  // Return the header length of the GIOP message offsets 

  int send_reply_exception (const TAO_GIOP_Version &version,
                            TAO_Transport *transport,
                            TAO_ORB_Core* orb_core,
                            CORBA::ULong request_id,
                            CORBA::Exception *x);
  // We must send a LocateReply through <transport>, this request
  // resulted in some kind of exception.
  
  static TAO_GIOP_Reply_Status_Type 
  convert_CORBA_to_GIOP_exception (CORBA::exception_type corba_type);
  // Convert the exception type from CORBA to GIOP
};

#if defined (__ACE_INLINE__)
# include "tao/GIOP_Accept_State.i"
#endif /* __ACE_INLINE__ */

#endif /*_TAO_GIOP_SERVER_STATE_*/

// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     Pluggable_Messaging_Utils.h
//
// = DESCRIPTION
//     Utility classes for the TAO pluggable messaging framework.
//
// = AUTHOR
//     Balachandran Natarajan <bala@cs.wustl.edu>
//
// ============================================================================
#ifndef _TAO_PLUGGABLE_MESSAGING_UTILS_H_
#define _TAO_PLUGGABLE_MESSAGING_UTILS_H_

class TAO_Export TAO_Pluggable_Connector_Params
{
  // = TITLE
  //   
  //
  // = DESCRIPTION
  //
public:
  IOP::ServiceContextList  svc_ctx;

  CORBA::ULong request_id;

  CORBA::Octet response_flags;

  const char *operation_name;

};

typedef enum HeaderType
{
  // = TITLE
  // = DESCRIPTION
  // 
  TAO_PLUGGABLE_MESSAGE_REQUEST_HEADER = 0,
  TAO_PLUGGABLE_MESSAGE_LOCATE_REQUEST_HEADER
}TAO_Pluggable_Header_Type;


typedef enum MessageType
{
  // = DESCRIPTION
  //   Provide an external interface for the users of this pluggable
  //   messaging framework to denote  existing message types. This has
  //   an inspiration from GIOP. So if anybody wants to add more message
  //   types you are welcome but please do not change the numbering
  //   scheme as this would affect GIOP. 
  
  TAO_PLUGGABLE_MESSAGE_REQUEST = 0,                // sent by client.
  TAO_PLUGGABLE_MESSAGE_REPLY = 1,                  // by server.
  TAO_PLUGGABLE_MESSAGE_CANCELREQUEST = 2,          // by client.
  TAO_PLUGGABLE_MESSAGE_LOCATEREQUEST = 3,          // by client.
  TAO_PLUGGABLE_MESSAGE_LOCATEREPLY = 4,
  TAO_PLUGGABLE_MESSAGE_CLOSECONNECTION = 5,        
  TAO_PLUGGABLE_MESSAGE_MESSAGERROR = 6,         
  TAO_PLUGGABLE_MESSAGE_FRAGMENT = 7             
}TAO_Pluggable_Message_Type;

#endif /*_TAO_PLUGGABLE_MESSAGING_UTILS_H_ */

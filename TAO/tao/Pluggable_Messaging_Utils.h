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
#ifndef TAO_PLUGGABLE_MESSAGING_UTILS_H
#define TAO_PLUGGABLE_MESSAGING_UTILS_H

class TAO_Export TAO_Pluggable_Connector_Params
{
  // = TITLE
  //   TAO_Pluggable_Connector_Params
  //
  // = DESCRIPTION
  //   This represents a set of data that would be received by the
  //   connector from the acceptor. 
public:
  TAO_Pluggable_Connector_Params (void);
  // Ctor

  IOP::ServiceContextList  svc_ctx_;
  // The IOP service context list

  CORBA::ULong request_id_;
  // The request id for which the reply we (connector) has received 

  CORBA::ULong reply_status_;
  // The reply status
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

typedef enum Exception_Types
{
  // = DESCRIPTION
  //   Provide an external interface for the users of this pluggable
  //   messaging framework to denote  existing Exception types. This has
  //   an inspiration from GIOP. So if anybody wants to add more message
  //   types you are welcome but please do not change the numbering
  //   scheme as this would affect GIOP. 
  TAO_PLUGGABLE_MESSAGE_NO_EXCEPTION = 0,
  // Request completed successfully

  TAO_PLUGGABLE_MESSAGE_USER_EXCEPTION,
  // Request terminated with user exception

  TAO_PLUGGABLE_MESSAGE_SYSTEM_EXCEPTION,
  // Request terminated with system exception

  TAO_PLUGGABLE_MESSAGE_LOCATION_FORWARD,
  // Reply is a location forward type
  
  TAO_PLUGGABLE_MESSAGE_LOCATION_FORWARD_PERM,
  // PLUGGABLE_MESSAGE 1.2, Reply is a location forward perm type..

  TAO_PLUGABLE_MESSAGE_NEEDS_ADDRESSING_MODE
  // GIOP1.2, 
}TAO_Pluggable_Message_Exception_Type;

#if defined (__ACE_INLINE__)
#include "tao/Pluggable_Messaging_Utils.i"
#endif /* __ACE_INLINE__ */

#endif /*_TAO_PLUGGABLE_MESSAGING_UTILS_H_ */

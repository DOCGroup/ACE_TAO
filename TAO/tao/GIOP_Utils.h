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
//     GIOP utility definitions
//
// = AUTHOR
//     Chris Cleeland <cleeland@cs.wustl.edu>
//     Carlos O' Ryan <coryan@uci.edu>
//
// ============================================================================
#ifndef TAO_GIOP_UTILS_H
#define TAO_GIOP_UTILS_H
#include "ace/pre.h"

#include "tao/Pluggable_Messaging.h"
#include "tao/Object.h"

// @@ Bala: i believe we still have a guideline to use #pragma once on
// platforms that support it...

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


typedef enum GIOP_LocateStatusType
{
  TAO_GIOP_UNKNOWN_OBJECT,
  TAO_GIOP_OBJECT_HERE,
  TAO_GIOP_OBJECT_FORWARD,
  TAO_GIOP_OBJECT_FORWARD_PERM,      //GIOP1.2
  TAO_GIOP_LOC_SYSTEM_EXCEPTION,     // GIOP1.2
  TAO_GIOP_LOC_NEEDS_ADDRESSING_MODE //GIOP 1.2
}TAO_GIOP_Locate_Status_Type;

// @@ Bala: what about the TAO_Export?  If a user tries to plug a
// protocol on NT and need this class for something he would not be
// able to use it without the export macro.

class TAO_GIOP_Locate_Status_Msg
{
  // =TITLE
  //  Hold the relevant information for every type of Locate mesg.
  //
  // =DESCRIPTION
  //  This class is there to hold the relevant info for different
  //  types of locate status messages. As on date we dont know much
  //  about other mesg types other than OBJECT_FORWARD. This clss can
  //  be clearly defined as time progresses.
  //
public:
  CORBA::Object_var forward_location_var;
  // The value will need to be used when the Message type is
  // TAO_GIOP_OBJECT_FORWARD

  TAO_GIOP_Locate_Status_Type status;
  // Stype of Locate status message
  //@@ Other mesg types.
};


typedef enum GIOP_ReplyStatusType
{
  TAO_GIOP_NO_EXCEPTION,
  // Request completed successfully

  TAO_GIOP_USER_EXCEPTION,
  // Request terminated with user exception

  TAO_GIOP_SYSTEM_EXCEPTION,
  // Request terminated with system exception

  TAO_GIOP_LOCATION_FORWARD,
  // Reply is a location forward type

  TAO_GIOP_LOCATION_FORWARD_PERM,
  // GIOP 1.2, Reply is a location forward perm type..

  NEEDS_ADDRESSING_MODE
  // GIOP1.2,

} TAO_GIOP_Reply_Status_Type;

// @@ Bala: more missing TAO_Export macros.

class TAO_GIOP_ReplyHeader
{
  // = TITLE
  //   This class embodies the header of a GIOP reply.
  // @@Not used. Could be used in future
public:
  IOP::ServiceContextList service_info;
  // Information

  CORBA::ULong request_id;
  // Unique identifier of the request for which this is a reply.

  TAO_GIOP_Reply_Status_Type reply_status;
  // Status of the reply (see above enum).
};

class TAO_Export TAO_GIOP_Utils
{
  // = TITLE
  //   Utility class that has some commonly used methods for both GIOP
  //   Base  & GIOP lite

  // = DESCRIPTION
public:
  static int read_bytes_input (TAO_Transport *transport,
                               TAO_InputCDR &cdr,
                               CORBA::ULong buf_size,
                               ACE_Time_Value *value = 0);

  static ssize_t read_buffer (TAO_Transport *transport,
                              char *buf,
                              size_t len,
                              ACE_Time_Value *max_wait_time = 0);

  static TAO_GIOP_Reply_Status_Type
  convert_CORBA_to_GIOP_exception (CORBA::exception_type corba_type);
  // Convert the exception type from CORBA to GIOP
};

#if defined (__ACE_INLINE__)
# include "tao/GIOP_Utils.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /*TAO_GIOP_UTILS_H */

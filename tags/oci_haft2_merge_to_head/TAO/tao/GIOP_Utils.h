// -*- C++ -*-

//=============================================================================
/**
 *  @file     GIOP_Utils.h
 *
 *  $Id$
 *
 *   GIOP utility definitions
 *
 *
 *  @author  Chris Cleeland <cleeland@cs.wustl.edu>  Carlos O' Ryan <coryan@uci.edu>
 */
//=============================================================================

#ifndef TAO_GIOP_UTILS_H
#define TAO_GIOP_UTILS_H
#include /**/ "ace/pre.h"

#include "tao/Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/IOP_IORC.h"

class TAO_Transport;

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

/**
 * @class TAO_GIOP_Locate_Status_Msg
 *
 * @brief Hold the relevant information for every type of Locate mesg.
 *
 * This class is there to hold the relevant info for different
 * types of locate status messages. As on date we dont know much
 * about other mesg types other than OBJECT_FORWARD. This clss can
 * be clearly defined as time progresses.
 */
class TAO_Export TAO_GIOP_Locate_Status_Msg
{
public:
  /// The value will need to be used when the Message type is
  /// TAO_GIOP_OBJECT_FORWARD
  CORBA::Object_var forward_location_var;

  /// Stype of Locate status message
  ///@@ Other mesg types.
  TAO_GIOP_Locate_Status_Type status;
};


typedef enum GIOP_ReplyStatusType
{
  /// Request completed successfully
  TAO_GIOP_NO_EXCEPTION,

  /// Request terminated with user exception
  TAO_GIOP_USER_EXCEPTION,

  /// Request terminated with system exception
  TAO_GIOP_SYSTEM_EXCEPTION,

  /// Reply is a location forward type
  TAO_GIOP_LOCATION_FORWARD,

  /// GIOP 1.2, Reply is a location forward perm type..
  TAO_GIOP_LOCATION_FORWARD_PERM,

  /// GIOP1.2,
  TAO_GIOP_NEEDS_ADDRESSING_MODE

} TAO_GIOP_Reply_Status_Type;


/**
 * @class TAO_GIOP_ReplyHeader
 *
 * @brief This class embodies the header of a GIOP reply.
 * @@Not used. Could be used in future
 */
class TAO_Export TAO_GIOP_ReplyHeader
{
public:
  /// Information
  IOP::ServiceContextList service_info;

  /// Unique identifier of the request for which this is a reply.
  CORBA::ULong request_id;

  /// Status of the reply (see above enum).
  TAO_GIOP_Reply_Status_Type reply_status;
};

/**
 * @class TAO_GIOP_Utils
 *
 * @brief Utility class that has some commonly used methods for both GIOP
 * Base  & GIOP lite
 */
class TAO_Export TAO_GIOP_Utils
{

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
};

#if defined (__ACE_INLINE__)
# include "tao/GIOP_Utils.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /*TAO_GIOP_UTILS_H */

// This may look like C, but it's really -*- C++ -*-
// -*- C++ -*-
// ===================================================================
/**
 *  @file   GIOP_Message_Handler.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 **/
// ===================================================================

#ifndef TAO_GIOP_MESSAGE_HANDLER_H
#define TAO_GIOP_MESSAGE_HANDLER_H
#include "ace/pre.h"
#include "ace/Message_Block.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/GIOP_Message_State.h"

class TAO_Transport;
class TAO_ORB_Core;

enum TAO_GIOP_Message_Status
{
  /// The buffer is waiting for the header of the message yet
  TAO_GIOP_WAITING_FOR_HEADER = 0,

  /// The buffer is waiting for the payload to appear on the socket
  TAO_GIOP_WAITING_FOR_PAYLOAD
};

/**
 * @class TAO_GIOP_Message_Handler
 *
 * @brief GIOP specific message handler class
 *
 * This class does some of the message handling for GIOP. This class
 * reads the message from the socket, splits the messages to create a
 * CDR stream out of it and passes that to the higher layers of the ORB.
 * The read from the socket is done using a single 'read' instead of
 * reading the header and the payload seperately.
 */

class TAO_GIOP_Message_Handler
{
public:

  /// Ctor
  TAO_GIOP_Message_Handler (TAO_ORB_Core *orb_core);

  /// Read the message from the transport in to the
  /// <current_buffer_>. This method delegates responsibility of
  /// parsing to some of the helper methods.
  int read_parse_message (TAO_Transport *transport);

  /// Check whether we have atleast one complete message ready for
  /// processing.
  int is_message_ready (void);

  /// Reset the contents of the <current_buffer_> if no more requests
  /// need to be processed. We reset the contents of the
  /// <message_state_> to parse and process the next request.
  void reset (int reset_flag);

  /// Return the underlying message state
  TAO_GIOP_Message_State &message_state (void);

  /// Return the pointer to the underlying Message Block
  ACE_Message_Block *message_block (void);

private:

  /// Parses the header information from the <current_buffer_>.
  int parse_header (void);

  /// Validates the first 4 bytes that contain the magic word
  /// "GIOP". Also calls the validate_version () on the incoming
  /// stream.
  int parse_magic_bytes (void);

  /// Gets the size of the payload from the <current_buffer_>. If the
  /// size of the current buffer is less than the payload size, the
  /// size of the buffer is increased.
  CORBA::ULong get_payload_size (void);

private:

  /// The state of the message in the buffer
  TAO_GIOP_Message_Status message_status_;

  /// The size of the message that is being read of the socket. This
  /// value is originally set to 1024 bytes. It is reset if we start
  /// receiving messages with payloads greater than that. The current
  /// value of <message_size_> would be the size of the last message
  /// received (ie. payload+headers).
  size_t message_size_;

  /// The buffer. rd_ptr() points to the beginning of the current
  /// message, properly aligned wr_ptr() points to where the next
  /// read() should put the data.
  ACE_Message_Block current_buffer_;

  /// The message state. It represents the status of the messages that
  /// have been read from the current_buffer_
  TAO_GIOP_Message_State message_state_;
};


const size_t TAO_GIOP_MESSAGE_HEADER_LEN = 12;
const size_t TAO_GIOP_MESSAGE_SIZE_OFFSET = 8;
const size_t TAO_GIOP_MESSAGE_FLAGS_OFFSET = 6;
const size_t TAO_GIOP_MESSAGE_TYPE_OFFSET  = 7;
const size_t TAO_GIOP_VERSION_MINOR_OFFSET = 5;
const size_t TAO_GIOP_VERSION_MAJOR_OFFSET = 4;

#if defined (__ACE_INLINE__)
# include "tao/GIOP_Message_Handler.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /*TAO_GIOP_MESSAGE_HANDLER_H*/

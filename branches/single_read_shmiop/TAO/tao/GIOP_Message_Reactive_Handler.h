// This may look like C, but it's really -*- C++ -*-
// -*- C++ -*-
// ===================================================================
/**
 *  @file   GIOP_Message_Reactive_Handler.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 **/
// ===================================================================

#ifndef TAO_GIOP_MESSAGE_REACTIVE_HANDLER_H
#define TAO_GIOP_MESSAGE_REACTIVE_HANDLER_H
#include "ace/pre.h"
#include "ace/Message_Block.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/GIOP_Message_State.h"

class TAO_Transport;
class TAO_ORB_Core;
class TAO_GIOP_Message_Base;

enum TAO_GIOP_Message_Status
{
  /// The buffer is waiting for the header of the message yet
  TAO_GIOP_WAITING_FOR_HEADER = 0,

  /// The buffer is waiting for the payload to appear on the socket
  TAO_GIOP_WAITING_FOR_PAYLOAD,

  /// The buffer has got multiple messages
  TAO_GIOP_MULTIPLE_MESSAGES
};

/**
 * @class TAO_GIOP_Message_Reactive_Handler
 *
 * @brief GIOP specific reactive message handler class
 *
 * This class does some of the message handling for GIOP. The class
 * is reactive. It relies on the reactor to call this class whenever
 * data appears or is left over in the socket. The class tries to read
 * whatever data is available for it to be read and expects the
 * reactor to call back if some data appears in the socket. In short,
 * the read has no notion of GIOP message details. Hence this class
 * reads the message from the socket, splits the messages to create a
 * CDR stream out of it and passes that to the higher layers of the ORB.
 * The read from the socket is done using a single 'read' instead of
 * reading the header and the payload seperately.
 */

class TAO_Export TAO_GIOP_Message_Reactive_Handler
{
public:

  /// Ctor
  TAO_GIOP_Message_Reactive_Handler (
      TAO_ORB_Core *orb_core,
      TAO_GIOP_Message_Base *base,
      size_t input_cdr_size = ACE_CDR::DEFAULT_BUFSIZE);


  /// Reads the message from the <transport> and sets the <wr_ptr> of
  /// the buffer appropriately.
  int read_messages (TAO_Transport *transport);

  /// Parse the GIOP message header if we have read bytes suffcient
  /// bytes. There are four  possibilities
  ///  - We did not read sufficient bytes, then make the reactor to
  ///    call us back. (return -2)
  ///  - We read a piece of message that was left out in the
  ///    socket. In such cases we just go ahead with more processing
  ///    (return 0).
  ///  - We have sufficient info for processing the header and we
  ///    processed it succesfully. (return 1);
  ///  - Any errors in processing will return a -1.
  int parse_message_header (void);

  /// Check whether we have atleast one complete message ready for
  /// processing.
  int is_message_ready (void);

  /// Return the underlying data block of the <current_buffer_>. At
  /// the sametime making a new data_block for itself. The read and
  /// write pointer positions would be reset.
  ACE_Data_Block *steal_data_block (void);

  /// Reset the contents of the <current_buffer_> if no more requests
  /// need to be processed. We reset the contents of the
  /// <message_state_> to parse and process the next request.
  void reset (int reset_flag = 0);

  /// Return the underlying message state
  TAO_GIOP_Message_State &message_state (void);

  /// Return the pointer to the data block within the message block
  ACE_Data_Block *data_block (void) const;

  /// Return the position of the read pointer in the <current_buffer_>
  size_t rd_pos (void) const;

  /// Return the position of the write pointer in the <current_buffer_>
  size_t wr_pos (void) const;

protected:

  /// Actually parses the header information
  int parse_message_header_i (char* buf);

  /// Parses the GIOP FRAGMENT_HEADER  information from the incoming
  /// stream.
  ///   - returns 1 on sucessful parsing,
  ///   - returns 0 if not found
  ///   - returns -1 if there is an error
  int parse_fragment_header (char *buf,
                             size_t length);

  /// GIOP 1.2 has a MESSAGE_FRAGMENT_HEADER. This flag indicates
  /// whether the MESSAGE_FRAGMENT_HEADER appeared while parsing the
  /// header of the message
private:

  /// Validates the first 4 bytes that contain the magic word
  /// "GIOP". Also calls the validate_version () on the incoming
  /// stream.
  int parse_magic_bytes (char *buf);

  /// Gets the size of the payload from the <current_buffer_>. If the
  /// size of the current buffer is less than the payload size, the
  /// size of the buffer is increased.
  CORBA::ULong get_payload_size (char *buf);

  /// Extract a CORBA::ULong from the <current_buffer_>
  CORBA::ULong read_ulong (const char *buf);

  /// Get the next message from the <supp_buffer_> in to the
  /// <current_buffer_>
  int get_message (void);

protected:
  /// The message state. It represents the status of the messages that
  /// have been read from the current_buffer_
  TAO_GIOP_Message_State message_state_;

private:

  /// The pointer to the object that holds us
  TAO_GIOP_Message_Base *mesg_base_;

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

  /// The supplementary buffer that holds just one message if the
  /// <current_buffer_> has more than one message. One message from
  /// the <current_buffer_> is taken and filled in this buffer, which
  /// is then sent to the higher layers of the ORB.
  ACE_Message_Block supp_buffer_;
};


const size_t TAO_GIOP_MESSAGE_HEADER_LEN = 12;
const size_t TAO_GIOP_MESSAGE_SIZE_OFFSET = 8;
const size_t TAO_GIOP_MESSAGE_FLAGS_OFFSET = 6;
const size_t TAO_GIOP_MESSAGE_TYPE_OFFSET  = 7;
const size_t TAO_GIOP_VERSION_MINOR_OFFSET = 5;
const size_t TAO_GIOP_VERSION_MAJOR_OFFSET = 4;
const size_t TAO_GIOP_MESSAGE_FRAGMENT_HEADER = 4;

#if defined (__ACE_INLINE__)
# include "tao/GIOP_Message_Reactive_Handler.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /*TAO_GIOP_MESSAGE_REACTIVE_HANDLER_H*/

// -*- C++ -*-

//=============================================================================
/**
 *  @file     GIOP_Message_State.h
 *
 *  $Id$
 *
 *   GIOP utility definitions
 *
 *
 *  @author  Chris Cleeland <cleeland@cs.wustl.edu>
 *  @author  Carlos O' Ryan <coryan@uci.edu>
 *  @author modified by Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================
#ifndef TAO_GIOP_MESSAGE_STATE_H
#define TAO_GIOP_MESSAGE_STATE_H
#include /**/ "ace/pre.h"
#include "tao/GIOP_Message_Version.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


class TAO_ORB_Core;
class TAO_GIOP_Message_Base;


/**
 * @class TAO_GIOP_Message_State
 *
 * @brief Generic definitions for Message States.
 *
 *  This helps to establish the state of the incoming messages.
 */

class TAO_Export TAO_GIOP_Message_State
{
public:

  /// Ctor
  TAO_GIOP_Message_State (TAO_ORB_Core *orb_core,
                          TAO_GIOP_Message_Base *base);

  /// Parse the message header.
  int parse_message_header (ACE_Message_Block &incoming);

  /// Return the message size
  CORBA::ULong message_size (void) const;

  /// Return the message size
  CORBA::ULong payload_size (void) const;

  /// Return the byte order information
  CORBA::Octet byte_order (void) const;

  /// Reset the state..
  void reset (void);

private:

  friend class TAO_GIOP_Message_Base;

  /// Parse the message header.
  int parse_message_header_i (ACE_Message_Block &incoming);

  /// Checks for the magic word 'GIOP' in the start of the incoing
  /// stream
  int parse_magic_bytes (char *buf);

  /// Extracts the version information from the incoming
  /// stream. Performs a check for  whether the version information is
  /// right and sets the information in the <state>
  int get_version_info (char *buf);

  /// Extracts the byte order information from the incoming
  /// stream. Performs a check for  whether the byte order information
  /// right and sets the information in the <state>
  int get_byte_order_info (char *buf);

  /// Gets the size of the payload and set the size in the <state>
  void get_payload_size (char *buf);

  /// Parses the GIOP FRAGMENT_HEADER  information from the incoming
  /// stream.
  int parse_fragment_header (char *buf,
                             size_t length);

  /// Read the unsigned long from the buffer. The <buf> should just
  /// point to the next 4 bytes data that represent the ULong
  CORBA::ULong read_ulong (char *buf);

private:

  /// The GIOP base class..
  TAO_GIOP_Message_Base *base_;

  // GIOP version information..
  TAO_GIOP_Message_Version giop_version_;

  /// 0 = big, 1 = little
  CORBA::Octet byte_order_;

  /// MsgType above
  CORBA::Octet message_type_;

  /// in byte_order!
  CORBA::ULong message_size_;

  /// Request Id from the Fragment header
  CORBA::ULong request_id_;

  /// (Requests and Replys)
  CORBA::Octet more_fragments_;

  /// Missing data
  CORBA::ULong missing_data_;
};


const size_t TAO_GIOP_MESSAGE_HEADER_LEN = 12;
const size_t TAO_GIOP_MESSAGE_SIZE_OFFSET = 8;
const size_t TAO_GIOP_MESSAGE_FLAGS_OFFSET = 6;
const size_t TAO_GIOP_MESSAGE_TYPE_OFFSET  = 7;
const size_t TAO_GIOP_VERSION_MINOR_OFFSET = 5;
const size_t TAO_GIOP_VERSION_MAJOR_OFFSET = 4;
const size_t TAO_GIOP_MESSAGE_FRAGMENT_HEADER = 4;

#if defined (__ACE_INLINE__)
# include "tao/GIOP_Message_State.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /*TAO_GIOP_MESSAGE_STATE_H*/

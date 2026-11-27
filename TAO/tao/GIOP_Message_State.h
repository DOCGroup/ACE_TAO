/**
 *  @file     GIOP_Message_State.h
 *
 *   GIOP utility definitions
 *
 *  @author  Chris Cleeland <cleeland@cs.wustl.edu>
 *  @author  Carlos O' Ryan <coryan@uci.edu>
 *  @author  Balachandran Natarajan <bala@cs.wustl.edu>
 *  @author  Johnny Willemsen <jwillemsen@remedy.nl>
 */
#ifndef TAO_GIOP_MESSAGE_STATE_H
#define TAO_GIOP_MESSAGE_STATE_H

#include /**/ "ace/pre.h"

#include "tao/GIOP_Message_Version.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/GIOPC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
  TAO_GIOP_Message_State () = default;
  ~TAO_GIOP_Message_State () = default;

  /// Parse the message header.
  ///
  /// @retval -1 There was an error parsing the GIOP header.
  /// @retval 0 The GIOP header was parsed correctly.
  /// @retval 1 The message block did not contain enough data to parse the header.
  int parse_message_header (ACE_Message_Block &incoming);

  /// Return the message size, including the GIOP header.
  CORBA::ULong message_size () const;

  /// Return the message payload size, excluding the GIOP header.
  CORBA::ULong payload_size () const;

  /// Return the byte order information
  CORBA::Octet byte_order () const;

  /// Return the message type
  GIOP::MsgType message_type () const;

  /// Return whether more fragments follow this message.
  CORBA::Boolean more_fragments () const;

  void more_fragments (CORBA::Boolean fragment);

  /// Get the GIOP version
  TAO_GIOP_Message_Version const &giop_version () const;

  /// Return whether the message is compressed.
  CORBA::Boolean compressed () const;

private:
  /// Parse the message header.
  int parse_message_header_i (ACE_Message_Block &incoming);

  /// Checks for the magic word 'GIOP' in the start of the incoming
  /// stream
  int parse_magic_bytes (char const *buf);

  /// Extracts and validates the version information from the incoming
  /// stream, then stores it in this object.
  int get_version_info (char const *buf);

  /// Extracts and validates the byte order information from the incoming
  /// stream, then stores it in this object.
  int get_byte_order_info (char const *buf);

  /// Gets the payload size and stores it in this object.
  void get_payload_size (char const *buf);

  /// Read the unsigned long from the buffer. The @a buf should just
  /// point to the next 4 bytes data that represent the ULong
  CORBA::ULong read_ulong (char const *buf) const;

private:
  /// GIOP version information.
  TAO_GIOP_Message_Version giop_version_ {TAO_DEF_GIOP_MAJOR, TAO_DEF_GIOP_MINOR};

  /// 0 = big, 1 = little
  CORBA::Octet byte_order_ {};

  /// MsgType above
  GIOP::MsgType message_type_ {GIOP::Request};

  /// In byte_order!
  CORBA::ULong payload_size_ {};

  /// Requests and replies.
  /// A value of zero indicates that this message does not have any
  /// fragments.  A value of non-zero indicates that it does have
  /// fragments.
  CORBA::Boolean more_fragments_ {};

  /// Compressed
  CORBA::Boolean compressed_ {};
};

TAO_END_VERSIONED_NAMESPACE_DECL

constexpr size_t TAO_GIOP_MESSAGE_HEADER_LEN = 12;
constexpr size_t TAO_GIOP_MESSAGE_SIZE_OFFSET = 8;
constexpr size_t TAO_GIOP_MESSAGE_FLAGS_OFFSET = 6;
constexpr size_t TAO_GIOP_MESSAGE_TYPE_OFFSET  = 7;
constexpr size_t TAO_GIOP_VERSION_MINOR_OFFSET = 5;
constexpr size_t TAO_GIOP_VERSION_MAJOR_OFFSET = 4;
constexpr size_t TAO_GIOP_MESSAGE_FRAGMENT_HEADER = 4;

#if defined (__ACE_INLINE__)
# include "tao/GIOP_Message_State.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /*TAO_GIOP_MESSAGE_STATE_H*/

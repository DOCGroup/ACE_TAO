/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    RMCast.h
//
// = AUTHOR
//    Carlos O'Ryan <coryan@uci.edu>
//
// ============================================================================

#ifndef ACE_RMCAST_H
#define ACE_RMCAST_H
#include "ace/pre.h"

#include "ace/OS.h"
#include "RMCast_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Message_Block;
class ACE_RMCast_Proxy;

// LynxOS 3.X defines the following macro
#if defined(MT_DATA)
#undef MT_DATA
#endif /* MT_DATA */

//! The RMCast namespace
/*!
  Several simple data structures and enums are shared by all the
  RMCast components, this is the place where we put them by default.
*/
class ACE_RMCast_Export ACE_RMCast
{
public:

  //! The message types
  /*!
    Each message includes a type field in the header used by the
    receiver to correctly parse it.
    Classes with the same name as the message type describe the actual
    format of the message.
  */
  enum Message_Type
  {
    // Sender initiated
    MT_POLL,
    MT_ACK_JOIN,
    MT_ACK_LEAVE,
    MT_DATA,
    // Receiver initiated
    MT_JOIN,
    MT_LEAVE,
    MT_ACK,
    MT_LAST
  };

  //! Simle enum used to describe the state transitions for senders
  /*!
   State transition (and actions) for the senders.
   This configuration is pesimistic, any invalid message is cause
   enough to reclaim all the resources.  This partially addresses
   situations where either accidentally or intentionally a sender is
   multicasting packets to the wrong group.

  <CODE>
              NON_EXISTENT   JOINED<BR>
   ------------------------------------------<BR>
   POLL       NON_EXISTENT   NON_EXISTENT<BR>
              Destroy        Destroy<BR>
  <BR>
   ACK        NON_EXISTENT   JOINED<BR>
              Noop           Process/Ack<BR>
  <BR>
   JOIN       JOINED         NON_EXISTENT<BR>
              Send/Join_Ack  Send/Join_Ack<BR>
  <BR>
   LEAVE      NON_EXISTENT   NON_EXISTENT<BR>
              Send/Leave_Ack Send/Leave_Ack<BR>
                             Destroy<BR>
  <BR>
   ACK_JOIN   NON_EXISTENT   NON_EXISTENT<BR>
              Noop           Destroy<BR>
  <BR>
   ACK_LEAVE  NON_EXISTENT   NON_EXISTENT<BR>
              Noop           Destroy<BR>
  <BR>
   SEND_DATA  NON_EXISTENT   NON_EXISTENT<BR>
              Noop           Destroy<BR>
  </CODE>
  */
  enum Sender_State
  {
    SS_NON_EXISTENT,
    SS_JOINED
  };


  // These structures define the basic layout of the messages.

  //! This is the main message sent by senders
  /*!
  <CODE>
   +---------+----------------------+<BR>
   | 8 bits  | DATA                 |<BR>
   +---------+----------------------+<BR>
   | 32 bits | sequence_number      |<BR>
   +---------+----------------------+<BR>
   | 32 bits | message_size         |<BR>
   +---------+----------------------+<BR>
   | 32 bits | fragment_offset      |<BR>
   +---------+----------------------+<BR>
   ? ? ? ? ? | 32 bits | payload_size         |<BR>
   ? ? ? ? ? +---------+----------------------+<BR>
   |         | payload              |<BR>
   +---------+----------------------+<BR>
  </CODE>
  */
  struct Data
  {
    // Source ID is implicit in recvfrom()...
    ACE_UINT32 sequence_number;
    ACE_UINT32 total_size;
    ACE_UINT32 fragment_offset;

    // @@ TODO: we may want to add optional fields, such as:
    //    - Polling clients for their status
    //    - Sending the range of messages in the queue
    //    - If we are using authentic group communication we may
    //    piggyback the ACK / NAK messages

    ACE_Message_Block *payload;

    //! Pass the proxy source between layers
    ACE_RMCast_Proxy *source;
  };

  /*!
    <CODE>
    +---------+----------------------+<BR>
    | 8 bits  | MT_POLL              |<BR>
    +---------+----------------------+<BR>
    </CODE>
  */
  struct Poll
  {
    //! Pass the proxy source between layers
    ACE_RMCast_Proxy *source;
  };

  //! Receivers accept new members using this message
  /*!
    <CODE>
    +---------+----------------------+<BR>
    | 8 bits  | MT_ACK_JOIN          |<BR>
    +---------+----------------------+<BR>
    | 32 bits | next_sequence_number |<BR>
    +---------+----------------------+<BR>
    </CODE>
  */
  struct Ack_Join
  {
    ACE_UINT32 next_sequence_number;

    //! Pass the proxy source between layers
    ACE_RMCast_Proxy *source;
  };

  //! Senders acknowledge when receivers try to leave
  /*!
  <CODE>
    +---------+----------------------+<BR>
    | 8 bits  | ACK_LEAVE            |<BR>
    +---------+----------------------+<BR>
    </CODE>
  */
  struct Ack_Leave
  {
    //! Pass the proxy source between layers
    ACE_RMCast_Proxy *source;
  };

  //! Provide feedback to the sender about messages received and sent
  //! so far.
  /*!
   *
   * This message is used to provide feedback information to senders.
   * It contains two sequence numbers:
   * - \param next_expected: is the sequence number of the next message
   *   expected, i.e. (next_expected-1) is the last message received
   *   without any losses before it.
   * - \param highest_received: is the highest sequence number among
   *   all the messages successfully received.
   * In other words, all messages lost (if any) are in the range:
   * [next_expected,highest_received)
   *
   * <CODE>
   * +---------+----------------------+<BR>
   * | 8 bits  | MT_ACK               |<BR>
   * +---------+----------------------+<BR>
   * | 32 bits | next_expected        |<BR>
   * +---------+----------------------+<BR>
   * | 32 bits | highest_received     |<BR>
   * +---------+----------------------+<BR>
   * </CODE>
   */
  struct Ack
  {
    //! The last message received without any losses before it.
    ACE_UINT32 next_expected;

    //! The last message successfully received
    ACE_UINT32 highest_received;

    //! Pass the proxy source between layers
    ACE_RMCast_Proxy *source;
  };

  //! Receivers send this message to indicate they want to join
  /*
  <CODE>
    +---------+----------------------+<BR>
    | 8 bits  | MT_JOIN              |<BR>
    +---------+----------------------+<BR>
  </CODE>
  */
  struct Join
  {
    //! Pass the proxy source between layers
    ACE_RMCast_Proxy *source;
  };

  //! Receivers send this message to disconnect gracefully
  /*!
  <CODE>
    +---------+----------------------+<BR>
    | 8 bits  | MT_LEAVE             |<BR>
    +---------+----------------------+<BR>
  </CODE>
  */
  struct Leave
  {
    //! Pass the proxy source between layers
    ACE_RMCast_Proxy *source;
  };
};

#if defined (__ACE_INLINE__)
#include "RMCast.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_RMCAST_H */

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

class ACE_RMCast_Export ACE_RMCast
{
public:

  // Message formats

  // From SENDER to RECEIVER
  //
  // POLL
  // +---------+----------------------+
  // | 8 bits  | MT_POLL              |
  // +---------+----------------------+
  //
  // ACK_JOIN
  // +---------+----------------------+
  // | 8 bits  | MT_ACK_JOIN          |
  // +---------+----------------------+
  // | 32 bits | next_sequence_number |
  // +---------+----------------------+
  //
  // ACK_LEAVE
  // +---------+----------------------+
  // | 8 bits  | ACK_LEAVE            |
  // +---------+----------------------+
  //
  // DATA
  // +---------+----------------------+
  // | 8 bits  | DATA                 |
  // +---------+----------------------+
  // | 32 bits | sequence_number      |
  // +---------+----------------------+
  // | 32 bits | message_size         |
  // +---------+----------------------+
  // | 32 bits | fragment_offset      |
  // +---------+----------------------+
  // ? ? ? ? ? | 32 bits | payload_size         |
  // ? ? ? ? ? +---------+----------------------+
  // |         | payload              |
  // +---------+----------------------+
  //

  // From RECEIVER to SENDER
  //
  // MT_JOIN
  // +---------+----------------------+
  // | 8 bits  | MT_JOIN              |
  // +---------+----------------------+
  //
  // MT_LEAVE
  // +---------+----------------------+
  // | 8 bits  | MT_LEAVE             |
  // +---------+----------------------+
  //
  // MT_ACK
  // +---------+----------------------+
  // | 8 bits  | MT_ACK               |
  // +---------+----------------------+
  // | 32 bits | highest_in_sequence  |
  // +---------+----------------------+
  // | 32 bits | highest_received     |
  // +---------+----------------------+
  //

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

  enum Receiver_State
  {
    RS_NON_EXISTENT,
    RS_JOINING,
    RS_JOINED,
    RS_LEAVING
  };

  // State transition (and actions) for the receivers.
  // This configuration is pesimistic, any invalid message is cause
  // enough to reclaim all the resources.  This partially addresses
  // situations where either accidentally or intentionally a sender is
  // multicasting packets to the wrong group.
  //
  //            NON_EXISTENT JOINING      JOINED       LEAVING
  // ----------------------------------------------------------------
  // POLL       JOINING      JOINING      JOINED       LEAVING
  //            Send/Join    Send/Join    Send/Ack     Send/Leave
  //
  // ACK        NON_EXISTENT NON_EXISTENT NON_EXISTENT NON_EXISTENT
  //            Noop         Destroy      Destroy      Destroy
  //
  // JOIN       NON_EXISTENT NON_EXISTENT NON_EXISTENT NON_EXISTENT
  //            Noop         Destroy      Destroy      Destroy
  //
  // LEAVE      NON_EXISTENT NON_EXISTENT NON_EXISTENT NON_EXISTENT
  //            Noop         Destroy      Destroy      Destroy
  //
  // ACK_JOIN   JOINING      JOINED       JOINED       LEAVING
  //            Send/Join    Update ACT   Update ACT   Send/Leave
  //
  // ACK_LEAVE  NON_EXISTENT NON_EXISTENT NON_EXISTENT NON_EXISTENT
  //            Noop         Destroy      Destroy      Destroy
  //
  // SEND_DATA  JOINING      JOINING      JOINED       LEAVING
  //            Send/Join    Send/Join    Recv/Data    Send/Leave
  //

  enum Sender_State
  {
    SS_NON_EXISTENT,
    SS_JOINED
  };

  // State transition (and actions) for the senders.
  // This configuration is pesimistic, any invalid message is cause
  // enough to reclaim all the resources.  This partially addresses
  // situations where either accidentally or intentionally a sender is
  // multicasting packets to the wrong group.
  //
  //            NON_EXISTENT   JOINED
  // ------------------------------------------
  // POLL       NON_EXISTENT   NON_EXISTENT
  //            Destroy        Destroy
  //
  // ACK        NON_EXISTENT   JOINED
  //            Noop           Process/Ack
  //
  // JOIN       JOINED         NON_EXISTENT
  //            Send/Join_Ack  Send/Join_Ack
  //
  // LEAVE      NON_EXISTENT   NON_EXISTENT
  //            Send/Leave_Ack Send/Leave_Ack
  //                           Destroy
  //
  // ACK_JOIN   NON_EXISTENT   NON_EXISTENT
  //            Noop           Destroy
  //
  // ACK_LEAVE  NON_EXISTENT   NON_EXISTENT
  //            Noop           Destroy
  //
  // SEND_DATA  NON_EXISTENT   NON_EXISTENT
  //            Noop           Destroy
  //


  // These structures define the basic layout of the messages.
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
  };

  struct Poll
  {
  };

  struct Ack_Join
  {
    ACE_INT32 next_sequence_number;
  };

  struct Ack_Leave
  {
  };

  struct Ack
  {
    ACE_UINT32 highest_in_sequence;
    ACE_UINT32 highest_received;
  };

  struct Join
  {
  };

  struct Leave
  {
  };
};

#if defined (__ACE_INLINE__)
#include "RMCast.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_RMCAST_H */

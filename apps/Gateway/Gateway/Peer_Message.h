/* -*- C++ -*- */
// @(#)Peer_Message.h	1.1	10/18/96


// ============================================================================
//
// = LIBRARY
//    apps
// 
// = FILENAME
//    Peer_Message.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (PEER_MESSAGE)
#define PEER_MESSAGE

// This is the unique connection identifier that denotes a particular
// Channel in the Gateway.
typedef short CONN_ID;

class Peer_Addr
  // = TITLE
  //     Peer address is used to identify the source/destination of a 
  //     routing message.
{
public:
  Peer_Addr (CONN_ID cid = -1, u_char lid = 0, u_char pay = 0)
    : conn_id_ (cid), logical_id_ (lid), payload_ (pay) {}

  int operator== (const Peer_Addr &pa) const
  {
    return this->conn_id_ == pa.conn_id_ 
      && this->logical_id_ == pa.logical_id_
      && this->payload_ == pa.payload_;
  }

  CONN_ID conn_id_;
  // Unique connection identifier that denotes a particular Channel.

  u_char logical_id_;
  // Logical ID.

  u_char payload_;
  // Payload type.
};


class Peer_Header
  // = TITLE
  //    Fixed sized header.
{
public:
  typedef u_short ROUTING_ID;
  // Type used to route messages from gatewayd.

  enum
  {
    INVALID_ID = -1 // No peer can validly use this number.
  };

  ROUTING_ID routing_id_;
  // Source ID.

  size_t  len_;
  // Length of the message in bytes.
};

class Peer_Message
  // = TITLE
  //    Variable-sized message (buf_ may be variable-sized between
  //    0 and MAX_PAYLOAD_SIZE).
{
public:
  enum { MAX_PAYLOAD_SIZE = 1024 };
  // The maximum size of an Peer message (see Peer protocol specs for
  // exact #).

  Peer_Header header_;
  // Message header.

  char buf_[MAX_PAYLOAD_SIZE];
  // Message payload.
};

#endif /* PEER_MESSAGE */

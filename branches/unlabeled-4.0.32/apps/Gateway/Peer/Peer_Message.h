/* -*- C++ -*- */
// @(#)Peer_Message.h	1.1	10/18/96

// Define the Peer message schema (this may change).

#if !defined (PEER_MESSAGE)
#define PEER_MESSAGE

// Fixed sized header.

class Peer_Header
{
public:
// Type used to route messages from gatewayd.
  typedef short ROUTING_ID;

  enum
  {
    INVALID_ID = -1 // No peer may use this number.
  };

  // Source ID.
  ROUTING_ID routing_id_;

  // Length of the message in bytes.
  size_t  len_;
};

// Variable-sized message (buf_ may be variable-sized between
// 0 and MAX_PAYLOAD_SIZE).

class Peer_Message
{
public:
  // The maximum size of an Peer message (see Peer protocol specs for exact #).
  enum { MAX_PAYLOAD_SIZE = 1024 };

  Peer_Header header_;

  // Message payload
  char buf_[MAX_PAYLOAD_SIZE];
};

#endif /* PEER_MESSAGE */

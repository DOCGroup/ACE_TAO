/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    apps
// 
// = FILENAME
//    Event.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (EVENT)
#define EVENT

// This is the unique connection identifier that denotes a particular
// IO_Handler in the Gateway.
typedef short CONN_ID;

class Event_Addr
  // = TITLE
  //     Address used to identify the source/destination of an event.
{
public:
  Event_Addr (CONN_ID cid = -1, unsigned char lid = 0, unsigned char pay = 0)
    : conn_id_ (cid), logical_id_ (lid), payload_ (pay) {}

  int operator== (const Event_Addr &pa) const
  {
    return this->conn_id_ == pa.conn_id_ 
      && this->logical_id_ == pa.logical_id_
      && this->payload_ == pa.payload_;
  }

  CONN_ID conn_id_;
  // Unique connection identifier that denotes a particular IO_Handler.

  unsigned char logical_id_;
  // Logical ID.

  unsigned char payload_;
  // Payload type.
};


class Event_Header
  // = TITLE
  //    Fixed sized header.
{
public:
  typedef unsigned short SUPPLIER_ID;
  // Type used to route messages from gatewayd.

  enum
  {
    INVALID_ID = -1 // No peer can validly use this number.
  };

  SUPPLIER_ID routing_id_;
  // Source ID.

  size_t len_;
  // Length of the message in bytes.
};

class Event
  // = TITLE
  //    Variable-sized message (buf_ may be variable-sized between
  //    0 and MAX_PAYLOAD_SIZE).
{
public:
  enum { MAX_PAYLOAD_SIZE = 1024 };
  // The maximum size of an Event.

  Event_Header header_;
  // Message header.

  char buf_[MAX_PAYLOAD_SIZE];
  // Message payload.
};

#endif /* EVENT */

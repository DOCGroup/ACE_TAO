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

#include "ace/OS.h"

// This is the unique connection identifier that denotes a particular
// Proxy_Handler in the Gateway.
typedef ACE_INT32 ACE_INT32;

class Event_Addr
  // = TITLE
  //     Address used to identify the source/destination of an event.
  //
  // = DESCRIPTION
  //     This is really a "virtual forwarding address" thatis used to
  //     decouple the filtering and forwarding logic of the Event
  //     Channel from the format of the data.
{
public:
  Event_Addr (ACE_INT32 cid = -1, 
	      u_char sid = 0, 
	      u_char type = 0)
    : conn_id_ (cid), 
      supplier_id_ (sid), 
      type_ (type) {}

  int operator== (const Event_Addr &event_addr) const
  {
    return this->conn_id_ == event_addr.conn_id_ 
      && this->supplier_id_ == event_addr.supplier_id_
      && this->type_ == event_addr.type_;
  }

  ACE_INT32 conn_id_;
  // Unique connection identifier that denotes a particular
  // Proxy_Handler.

  ACE_INT32 supplier_id_;
  // Logical ID.

  ACE_INT32 type_;
  // Event type.
};


class Event_Header
  // = TITLE
  //    Fixed sized header.
{
public:
  typedef ACE_INT32 SUPPLIER_ID;
  // Type used to forward events from gatewayd.

  enum
  {
    INVALID_ID = -1 // No peer can validly use this number.
  };

  SUPPLIER_ID supplier_id_;
  // Source ID.

  ACE_INT32 type_;
  // Event type.

  size_t len_;
  // Length of the entire event (including data payload) in bytes.
};

class Event
  // = TITLE
  //    Variable-sized event (data_ may be variable-sized between
  //    0 and MAX_PAYLOAD_SIZE).
{
public:
  enum { MAX_PAYLOAD_SIZE = 1024 };
  // The maximum size of an Event.

  Event_Header header_;
  // Event header.

  char data_[MAX_PAYLOAD_SIZE];
  // Event data.
};

#endif /* EVENT */

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

class Event_Key
  // = TITLE
  //     Address used to identify the source/destination of an event.
  //
  // = DESCRIPTION
  //     This is really a "virtual forwarding address" thatis used to
  //     decouple the filtering and forwarding logic of the Event
  //     Channel from the format of the data.
{
public:
  Event_Key (ACE_INT32 cid = -1, 
	      u_char sid = 0, 
	      u_char type = 0)
    : proxy_id_ (cid), 
      supplier_id_ (sid), 
      type_ (type) {}

  int operator== (const Event_Key &event_addr) const
  {
    return this->proxy_id_ == event_addr.proxy_id_ 
      && this->supplier_id_ == event_addr.supplier_id_
      && this->type_ == event_addr.type_;
  }

  ACE_INT32 proxy_id_;
  // Unique connection identifier that denotes a particular
  // Proxy_Handler.

  ACE_INT32 supplier_id_;
  // Logical ID.

  ACE_INT32 type_;
  // Event type.
};

class Event_Header
  // = TITLE
  //     Fixed sized header.
  //
  // = DESCRIPTION
  //     This is designed to have a sizeof (16) to avoid alignment
  //     problems on most platforms.
{
public:
  typedef ACE_INT32 SUPPLIER_ID;
  // Type used to forward events from gatewayd.

  enum
  {
    INVALID_ID = -1 // No peer can validly use this number.
  };

  void decode (void)
    {
      this->len_ = ntohl (this->len_);
      this->supplier_id_ = ntohl (this->supplier_id_);
      this->type_ = ntohl (this->type_);
      this->priority_ = ntohl (this->priority_);
    }
  // Decode from network byte order to host byte order.

  void encode (void)
    {
      this->len_ = htonl (this->len_);
      this->supplier_id_ = htonl (this->supplier_id_);
      this->type_ = htonl (this->type_);
      this->priority_ = htonl (this->priority_);
    }
  // Encode from host byte order to network byte order.

  size_t len_;
  // Length of the data_ payload, in bytes.

  SUPPLIER_ID supplier_id_;
  // Source ID.

  ACE_INT32 type_;
  // Event type.

  ACE_INT32 priority_;
  // Event priority.
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

/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    gateway
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

// = The following #defines should really be in a separate include
// file that is shared with the ../Peer/ directory.  For now, we'll
// keep them here to simplify the sharing between the two directories.
// BTW, this is also the reason why all the methods are inlined...

// Used by Peers to create Consumers in a Gateway.
#if !defined (DEFAULT_GATEWAY_CONSUMER_PORT)
#define DEFAULT_GATEWAY_CONSUMER_PORT 10009
#endif /* DEFAULT_GATEWAY_CONSUMER_PORT */

// Used by Peers create Suppliers in a Gateway.
#if !defined (DEFAULT_GATEWAY_SUPPLIER_PORT)
#define DEFAULT_GATEWAY_SUPPLIER_PORT 10010
#endif /* DEFAULT_GATEWAY_SUPPLIER_PORT */

// Used by a Gateway to create Consumers in a Peer.
#if !defined (DEFAULT_PEER_CONSUMER_PORT)
#define DEFAULT_PEER_CONSUMER_PORT 10011
#endif /* DEFAULT_PEER_CONSUMER_PORT */

// Used by a Gateway to create Suppliers in a Peer.
#if !defined (DEFAULT_PEER_SUPPLIER_PORT)
#define DEFAULT_PEER_SUPPLIER_PORT 10012
#endif /* DEFAULT_PEER_SUPPLIER_PORT */

// This is the unique supplier identifier that denotes a particular
// <Connection_Handler> in the Gateway.
typedef ACE_INT32 CONNECTION_ID;

class Event_Key
{
  // = TITLE
  //     Address used to identify the source/destination of an event.
  //
  // = DESCRIPTION
  //     This is really a "virtual forwarding address" thatis used to
  //     decouple the filtering and forwarding logic of the Event
  //     Channel from the format of the data.
public:
  Event_Key (CONNECTION_ID cid = -1, 
             u_char type = 0)
    : connection_id_ (cid), 
      type_ (type) {}

  int operator== (const Event_Key &event_addr) const
  {
    return this->connection_id_ == event_addr.connection_id_ 
      && this->type_ == event_addr.type_;
  }

  CONNECTION_ID connection_id_;
  // Unique connection identifier that denotes a particular
  // Connection_Handler.

  ACE_INT32 type_;
  // Event type.
};

class Event_Header
{
  // = TITLE
  //     Fixed sized header.
  //
  // = DESCRIPTION
  //     This is designed to have a sizeof (16) to avoid alignment
  //     problems on most platforms.
public:
  enum
  {
    INVALID_ID = -1 // No peer can validly use this number.
  };

  void decode (void)
    {
      this->len_ = ntohl (this->len_);
      this->type_ = ntohl (this->type_);
      this->priority_ = ntohl (this->priority_);
    }
  // Decode from network byte order to host byte order.

  void encode (void)
    {
      this->len_ = htonl (this->len_);
      this->type_ = htonl (this->type_);
      this->priority_ = htonl (this->priority_);
    }
  // Encode from host byte order to network byte order.

  size_t len_;
  // Length of the data_ payload, in bytes.

  CONNECTION_ID connection_id_;
  // Unique connection identifier that denotes a particular
  // Connection_Handler.

  ACE_INT32 type_;
  // Event type.

  ACE_INT32 priority_;
  // Event priority.
};

class Event
{
  // = TITLE
  //    Variable-sized event (data_ may be variable-sized between
  //    0 and MAX_PAYLOAD_SIZE).
public:
  enum { MAX_PAYLOAD_SIZE = 1024 };
  // The maximum size of an Event.

  Event_Header header_;
  // Event header.

  char data_[MAX_PAYLOAD_SIZE];
  // Event data.
};

#endif /* EVENT */

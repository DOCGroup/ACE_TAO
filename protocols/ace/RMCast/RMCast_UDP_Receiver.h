// $Id$

// ============================================================================
//
// = DESCRIPTION
//     Encapsulate the I/O layer for a UDP based receiver
//
// = AUTHOR
//    Carlos O'Ryan <coryan@uci.edu>
//
// ============================================================================

#ifndef ACE_RMCAST_UDP_RECEIVER_H
#define ACE_RMCAST_UDP_RECEIVER_H
#include "ace/pre.h"

#include "RMCast_UDP_Event_Handler.h"
#include "ace/SOCK_Dgram_Mcast.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Synch.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_RMCast_Sender_Proxy;
class ACE_RMCast_Sender_Proxy_Factory;
class ACE_Reactor;
class ACE_Time_Value;
class ACE_INET_Addr;

class ACE_RMCast_Export ACE_RMCast_UDP_Receiver
{
public:
  ACE_RMCast_UDP_Receiver (ACE_RMCast_Sender_Proxy_Factory *factory);
  // Constructor
  // <factory> is used to create the Sender_Proxy and Modules that
  // process incoming messages.
  // The caller owns <factory>.

  ~ACE_RMCast_UDP_Receiver (void);
  // Destructor

  int subscribe (const ACE_INET_Addr &mcast_addr,
                 int reuse_addr = 1,
                 const ACE_TCHAR *net_if = 0,
                 int protocol_family = PF_INET,
                 int protocol = 0);
  // Start receiving data for the <mcast_addr> multicast group.
  // Please read the documentation of <ACE_SOCK_Dgram_Mcast> for more
  // details.

  // The class can be used with a Reactor or using blocking I/O
  // depending on what method of the following two is called.

  int handle_events (ACE_Time_Value *tv = 0);
  // Wait for events for the period <tv>. If <tv> is zero it blocks
  // forever.

  int register_handlers (ACE_Reactor *reactor);
  // Register any event handlers into <reactor>

  int remove_handlers (void);
  // Remove all the handlers from the reactor

  int handle_input (ACE_HANDLE h);
  // There is data to read, read it and process it.

  ACE_HANDLE get_handle (void) const;
  // Obtain the handle for the underlying socket

private:
  int send_join (ACE_INET_Addr &from);
  // Send a JOIN message

  int send_ack (ACE_RMCast_Sender_Proxy *sender_proxy,
                ACE_INET_Addr &from);
  // Send an ACK message

  int send_leave (ACE_INET_Addr &from);
  // Send a LEAVE messsage

private:
  ACE_RMCast_Sender_Proxy_Factory *factory_;
  // The factory used to create Sender proxies

  ACE_SOCK_Dgram_Mcast dgram_;
  // The socket

  typedef ACE_Hash_Map_Manager<ACE_INET_Addr,ACE_RMCast_Sender_Proxy*,ACE_Null_Mutex> Map;
  Map map_;

  ACE_RMCast_UDP_Event_Handler eh_;
  // The event handler adapter
};

#if defined (__ACE_INLINE__)
#include "RMCast_UDP_Receiver.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_RMCAST_UDP_RECEIVER_H */

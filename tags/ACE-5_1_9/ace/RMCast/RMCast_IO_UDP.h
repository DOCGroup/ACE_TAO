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

#ifndef ACE_RMCAST_IO_UDP_H
#define ACE_RMCAST_IO_UDP_H
#include "ace/pre.h"

#include "RMCast_Module.h"
#include "RMCast_UDP_Event_Handler.h"
#include "ace/SOCK_Dgram_Mcast.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Synch.h"
#include "ace/INET_Addr.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_RMCast_UDP_Proxy;
class ACE_RMCast_Module_Factory;
class ACE_Reactor;
class ACE_Time_Value;

class ACE_RMCast_Export ACE_RMCast_IO_UDP : public ACE_RMCast_Module
{
public:
  ACE_RMCast_IO_UDP (ACE_RMCast_Module_Factory *factory);
  // Constructor
  // <factory> is used to create the modules for each proxy that
  // process incoming messages. The class does *not* assume ownership
  // of <factory>, the caller owns it.

  ~ACE_RMCast_IO_UDP (void);
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

  // Send back to the remove object represented by <proxy>
  int send_data (ACE_RMCast::Data &, const ACE_INET_Addr &);
  int send_poll (ACE_RMCast::Poll &, const ACE_INET_Addr &);
  int send_ack_join (ACE_RMCast::Ack_Join &, const ACE_INET_Addr &);
  int send_ack_leave (ACE_RMCast::Ack_Leave &, const ACE_INET_Addr &);
  int send_ack (ACE_RMCast::Ack &, const ACE_INET_Addr &);
  int send_join (ACE_RMCast::Join &, const ACE_INET_Addr &);
  int send_leave (ACE_RMCast::Leave &, const ACE_INET_Addr &);

  // = The RMCast_Module methods
  virtual int data (ACE_RMCast::Data &);
  virtual int poll (ACE_RMCast::Poll &);
  virtual int ack_join (ACE_RMCast::Ack_Join &);
  virtual int ack_leave (ACE_RMCast::Ack_Leave &);
  virtual int ack (ACE_RMCast::Ack &);
  virtual int join (ACE_RMCast::Join &);
  virtual int leave (ACE_RMCast::Leave &);
  // The messages are sent to the multicast group

private:
  ACE_RMCast_Module_Factory *factory_;
  // The factory used to create the modules attached to each proxy

  ACE_INET_Addr mcast_group_;
  // The multicast group we subscribe and send to

  ACE_SOCK_Dgram_Mcast dgram_;
  // The socket

  typedef ACE_Hash_Map_Manager<ACE_INET_Addr,ACE_RMCast_UDP_Proxy*,ACE_Null_Mutex> Map;
  Map map_;

  ACE_RMCast_UDP_Event_Handler eh_;
  // The event handler adapter
};

#if defined (__ACE_INLINE__)
#include "RMCast_IO_UDP.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_RMCAST_IO_UDP_H */

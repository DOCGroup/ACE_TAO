/* -*- C++ -*- */
// $Id$

#ifndef ACE_RMCAST_UDP_RELIABLE_RECEIVER_H
#define ACE_RMCAST_UDP_RELIABLE_RECEIVER_H
#include "ace/pre.h"

#include "RMCast_Singleton_Factory.h"
#include "RMCast_Reliable_Factory.h"
#include "RMCast_IO_UDP.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Reactor;

class ACE_RMCast_Export ACE_RMCast_UDP_Reliable_Receiver : public ACE_RMCast_Module
{
public:
  /// Constructor
  ACE_RMCast_UDP_Reliable_Receiver (ACE_RMCast_Module *user_control);

  /// Destructor
  virtual ~ACE_RMCast_UDP_Reliable_Receiver (void);

  /// Open the UDP I/O module.
  int init (const ACE_INET_Addr &mcast_group);

  /// Use the reactor to handle incoming messages
  void reactive_incoming_messages (ACE_Reactor *reactor);

private:
  /// All the proxys give their messages to user module
  ACE_RMCast_Singleton_Factory user_factory_;

  /// This factory creates the per-proxy stack
  ACE_RMCast_Reliable_Factory factory_;

  /// Handle all the UDP I/O
  ACE_RMCast_IO_UDP io_udp_;
};

#if defined (__ACE_INLINE__)
#include "RMCast_UDP_Reliable_Receiver.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_RMCAST_UDP_RELIABLE_RECEIVER_H */

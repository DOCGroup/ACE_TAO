/* -*- C++ -*- */
// $Id$

#ifndef ACE_RMCAST_UDP_RELIABLE_SENDER_H
#define ACE_RMCAST_UDP_RELIABLE_SENDER_H
#include "ace/pre.h"

#include "ace/RMCast/RMCast_Singleton_Factory.h"
#include "ace/RMCast/RMCast_IO_UDP.h"
#include "ace/RMCast/RMCast_Sequencer.h"
#include "ace/RMCast/RMCast_Retransmission.h"
#include "ace/RMCast/RMCast_Membership.h"
#include "ace/RMCast/RMCast_Fragment.h"
#include "ace/RMCast/RMCast_Fork.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Reactor;

class ACE_RMCast_Export ACE_RMCast_UDP_Reliable_Sender : public ACE_RMCast_Module
{
public:
  /// Constructor
  ACE_RMCast_UDP_Reliable_Sender (ACE_RMCast_Module *user_control);

  /// Destructor
  virtual ~ACE_RMCast_UDP_Reliable_Sender (void);

  /// Open the UDP I/O module.
  int init (const ACE_INET_Addr &mcast_group);

  /// Use the reactor to handle incoming messages
  void reactive_incoming_messages (ACE_Reactor *reactor);

  /// Use the reactor to periodically resend messages
  void reactive_resends (ACE_Reactor *reactor,
                         const ACE_Time_Value &period);

  /// Check if there is still some messages to send, return 0 if not.
  int has_data (void);

  /// Check if there are any members still connected
  int has_members (void);

private:
  /// The application-level control module
  ACE_RMCast_Module *user_control_;

  /// Assign sequence numbers
  ACE_RMCast_Sequencer sequencer_;

  /// The retransmission module
  ACE_RMCast_Retransmission retransmission_;

  /// All the proxys give their messages to the retransmission module
  ACE_RMCast_Singleton_Factory factory_;

  /// Handle all the UDP I/O
  ACE_RMCast_IO_UDP io_udp_;

  /// The membership module
  ACE_RMCast_Membership membership_;

  /// The fragmentation module
  ACE_RMCast_Fragment fragment_;

  /// Redirect control messages to the user supplied module
  ACE_RMCast_Fork fork_;
};

#if defined (__ACE_INLINE__)
#include "ace/RMCast/RMCast_UDP_Reliable_Sender.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_RMCAST_UDP_RELIABLE_SENDER_H */

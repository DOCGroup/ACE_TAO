/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    RMCast_UDP_Sender.h
//
// = AUTHOR
//    Carlos O'Ryan <coryan@uci.edu>
//
// ============================================================================

#ifndef ACE_RMCAST_UDP_SENDER_H
#define ACE_RMCAST_UDP_SENDER_H
#include "ace/pre.h"

#include "RMCast_Module.h"
#include "ace/SOCK_Dgram.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_RMCast_Export ACE_RMCast_UDP_Sender : public ACE_RMCast_Module
{
  // = TITLE
  //     Reliable Multicast UDP_Sender
  //
  // = DESCRIPTION
  //     Implements a Facade to the classes that implement a reliable
  //     multicast protocol.
public:
  // = Initialization and termination methods.
  ACE_RMCast_UDP_Sender (const ACE_INET_Addr &mcast_addr);
  // Constructor

  virtual ~ACE_RMCast_UDP_Sender (void);
  // Destructor

  // = The RMCast_Module methods
  virtual int open (void);
  virtual int close (void);
  virtual int put_data (ACE_RMCast::Data &data);
  // Send the Message block, this is the callback invoked at the end
  // of the stack.

protected:
  ACE_SOCK_Dgram dgram_;
  // This is the socket used to send the multicast data.
  // @@ This should be strategized, what if we want to use something
  // like ATM networks to send the data, then the types would be
  // different....

  ACE_INET_Addr mcast_addr_;
  // The multicast group we send to.
  // @@ Can we really strategize the addressing, without introducing
  // too much complexity?  How can we decouple the reliability aspect
  // from the transport aspects of the system???
};

#if defined (__ACE_INLINE__)
#include "RMCast_UDP_Sender.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_RMCAST_UDP_SENDER_H */

/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    RMCast_UDP_Proxy.h
//
// = AUTHOR
//    Carlos O'Ryan <coryan@uci.edu>
//
// ============================================================================

#ifndef ACE_RMCAST_UDP_PROXY_H
#define ACE_RMCAST_UDP_PROXY_H
#include "ace/pre.h"

#include "RMCast.h"
#include "ace/INET_Addr.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_RMCast_Module;
class ACE_RMCast_IO_UDP;

class ACE_RMCast_Export ACE_RMCast_UDP_Proxy
{
  // = TITLE
  //   Reliable Multicast Sender Ambassador
  //
  // = DESCRIPTION
  //   Implement an Ambassador for the reliable multicast senders.
  //
public:
  ACE_RMCast_UDP_Proxy (ACE_RMCast_IO_UDP *io_udp,
                        const ACE_INET_Addr &peer_addr,
                        ACE_RMCast_Module *module);
  // Constructor

  virtual ~ACE_RMCast_UDP_Proxy (void);
  // Destructor

  int receive_message (char *buffer, size_t size);
  // Receive the message

  const ACE_INET_Addr &peer_addr (void) const;
  // The address of the peer

  ACE_RMCast_Module *module (void) const;
  // The module

private:
  ACE_RMCast_IO_UDP *io_udp_;
  // The IO facade

  ACE_INET_Addr peer_addr_;
  // The address of the peer

  ACE_RMCast_Module *module_;
  // Process the data and control messages.
};

#if defined (__ACE_INLINE__)
#include "RMCast_UDP_Proxy.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_RMCAST_UDP_PROXY_H */

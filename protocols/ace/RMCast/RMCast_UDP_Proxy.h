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

#include "RMCast_Proxy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/INET_Addr.h"

class ACE_RMCast_IO_UDP;

class ACE_RMCast_Export ACE_RMCast_UDP_Proxy : public ACE_RMCast_Proxy
{
  // = TITLE
  //     Proxy for UDP-based RMCast
  //
  // = DESCRIPTION
  //
public:
  ACE_RMCast_UDP_Proxy (ACE_RMCast_IO_UDP *io_udp,
                        const ACE_INET_Addr &peer_addr);
  // Constructor

  virtual ~ACE_RMCast_UDP_Proxy (void);
  // Destructor

  int receive_message (char *buffer, size_t size);
  // Receive the message

  const ACE_INET_Addr &peer_addr (void) const;
  // The address of the peer

  // = The ACE_RMCast_Proxy methods
  virtual int reply_data (ACE_RMCast::Data &);
  virtual int reply_poll (ACE_RMCast::Poll &);
  virtual int reply_ack_join (ACE_RMCast::Ack_Join &);
  virtual int reply_ack_leave (ACE_RMCast::Ack_Leave &);
  virtual int reply_ack (ACE_RMCast::Ack &);
  virtual int reply_join (ACE_RMCast::Join &);
  virtual int reply_leave (ACE_RMCast::Leave &);

private:
  ACE_RMCast_IO_UDP *io_udp_;
  // The IO facade

  ACE_INET_Addr peer_addr_;
  // The address of the peer
};

#if defined (__ACE_INLINE__)
#include "RMCast_UDP_Proxy.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_RMCAST_UDP_PROXY_H */

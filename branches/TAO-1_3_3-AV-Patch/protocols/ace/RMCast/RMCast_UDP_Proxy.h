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
#include /**/ "ace/pre.h"

#include "RMCast_Proxy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/INET_Addr.h"

class ACE_RMCast_IO_UDP;

/// Define the proxy implementation for UDP based communication
/**
 * Proxy objects are transport specific, they are responsible for
 * storing the remote peer addressing information.
 * This class implements the UDP version of a proxy.
 */
class ACE_RMCast_Export ACE_RMCast_UDP_Proxy : public ACE_RMCast_Proxy
{
public:
  /// Constructor
  /**
   * The \param io_udp argument is kept to send the replys through the
   * right socket.
   * The \param peer_addr is the address used byu the peer to receive
   * responses.
   */
  ACE_RMCast_UDP_Proxy (ACE_RMCast_IO_UDP *io_udp,
                        const ACE_INET_Addr &peer_addr);

  /// Destructor
  virtual ~ACE_RMCast_UDP_Proxy (void);

  /// Receive a message, parse and send it upstream in the right
  /// format.
  int receive_message (char *buffer, size_t size);

  /// Make the peer address available
  const ACE_INET_Addr &peer_addr (void) const;

  //@{
  /**
   * Implement the ACE_RMCast_Proxy methods, in this case we use the
   * @var io_udp_ object to send the data, using the address of our
   * remote peer.
   */
  virtual int reply_data (ACE_RMCast::Data &);
  virtual int reply_poll (ACE_RMCast::Poll &);
  virtual int reply_ack_join (ACE_RMCast::Ack_Join &);
  virtual int reply_ack_leave (ACE_RMCast::Ack_Leave &);
  virtual int reply_ack (ACE_RMCast::Ack &);
  virtual int reply_join (ACE_RMCast::Join &);
  virtual int reply_leave (ACE_RMCast::Leave &);
  //@}

private:
  /// The IO facade
  ACE_RMCast_IO_UDP *io_udp_;

  /// The remote peer's address
  ACE_INET_Addr peer_addr_;
};

#if defined (__ACE_INLINE__)
#include "RMCast_UDP_Proxy.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* ACE_RMCAST_UDP_PROXY_H */

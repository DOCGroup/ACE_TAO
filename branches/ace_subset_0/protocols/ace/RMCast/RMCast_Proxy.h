/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    RMCast_Proxy.h
//
// = AUTHOR
//    Carlos O'Ryan <coryan@uci.edu>
//
// ============================================================================

#ifndef ACE_RMCAST_PROXY_H
#define ACE_RMCAST_PROXY_H
#include "ace/pre.h"

#include "ace/RMCast/RMCast_Module.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Message_Block;
class ACE_Time_Value;

//! Local representation for remote peers
/*!
  Both senders and receivers in the multicast group need to maintain
  explicit representations of their "peers".  For example, a sender
  needs to know the list of all the receivers and what messages they
  have reported as successfully received.
  Likewise, the receiver needs to maintain separate state for each
  remote sender, and must be able to disconnect from all of them
  gracefully when needed.
  The RMCast_Proxy class is an opaque representation of such a peer,
  and hides all the networking details from the rest of the system.
*/
class ACE_RMCast_Export ACE_RMCast_Proxy : public ACE_RMCast_Module
{
public:
  //! Constructor
  ACE_RMCast_Proxy (void);
  // Constructor

  //! Destructor
  virtual ~ACE_RMCast_Proxy (void);

  //! Return the next sequence number expected by the peer. Only
  //! applies to remote receiver proxies.
  /*!
   * Please read the documentation in ACE_RMCast::Ack
   */
  virtual ACE_UINT32 next_expected (void) const;

  //! Return the highest sequence number successfully received.
  //! Only applies to remote receiver proxies.
  /*!
   * Please read the documentation in ACE_RMCast::Ack
   */
  virtual ACE_UINT32 highest_received (void) const;

  //@{
  //! Send messages directly to the peer.
  /*!
   * Send a message directly to the peer, i.e. the message is not
   * sent through the multicast group and it may not be processed by
   * all the layers in the stack.
   */
  virtual int reply_data (ACE_RMCast::Data &) = 0;
  virtual int reply_poll (ACE_RMCast::Poll &) = 0;
  virtual int reply_ack_join (ACE_RMCast::Ack_Join &);
  virtual int reply_ack_leave (ACE_RMCast::Ack_Leave &) = 0;
  virtual int reply_ack (ACE_RMCast::Ack &) = 0;
  virtual int reply_join (ACE_RMCast::Join &) = 0;
  virtual int reply_leave (ACE_RMCast::Leave &) = 0;
  //@}

  /*!
   * Proxies process the ACK sequence numbers to cache the ack
   * information from the peer.
   */
  virtual int ack (ACE_RMCast::Ack &);

private:
  //@{
  //! Cache the sequence numbers reported from the remote peer using
  //! Ack messages
  ACE_UINT32 next_expected_;
  ACE_UINT32 highest_received_;
  //@}
};

#if defined (__ACE_INLINE__)
#include "ace/RMCast/RMCast_Proxy.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_RMCAST_PROXY_H */

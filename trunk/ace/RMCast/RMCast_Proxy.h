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

#include "RMCast_Module.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Message_Block;
class ACE_Time_Value;

class ACE_RMCast_Export ACE_RMCast_Proxy : public ACE_RMCast_Module
{
  // = TITLE
  //     Reliable Multicast Proxy
  //
  // = DESCRIPTION
  //     The proxy is used to send back messages to either a single
  //     receiver (o supplier).
  //
public:
  // = Initialization and termination methods.
  ACE_RMCast_Proxy (void);
  // Constructor

  virtual ~ACE_RMCast_Proxy (void);
  // Destructor

  virtual ACE_UINT32 highest_in_sequence (void) const;
  virtual ACE_UINT32 highest_received (void) const;
  // Get the sequence numbers received by the remote proxy.
  // Return 0 for sender proxies

  virtual int reply_data (ACE_RMCast::Data &) = 0;
  virtual int reply_poll (ACE_RMCast::Poll &) = 0;
  virtual int reply_ack_join (ACE_RMCast::Ack_Join &) = 0;
  virtual int reply_ack_leave (ACE_RMCast::Ack_Leave &) = 0;
  virtual int reply_ack (ACE_RMCast::Ack &) = 0;
  virtual int reply_join (ACE_RMCast::Join &) = 0;
  virtual int reply_leave (ACE_RMCast::Leave &) = 0;
  // Push data back to the remote proxy

  // = The RMCast_Module methods
  virtual int ack (ACE_RMCast::Ack &);

private:
  ACE_UINT32 highest_in_sequence_;
  ACE_UINT32 highest_received_;
  // Cache the sequence numbers reported from the remote peer using
  // Ack messages
};

#if defined (__ACE_INLINE__)
#include "RMCast_Proxy.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_RMCAST_PROXY_H */

/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    RMCast_Sender_Proxy.h
//
// = AUTHOR
//    Carlos O'Ryan <coryan@uci.edu>
//
// ============================================================================

#ifndef ACE_RMCAST_SENDER_PROXY_H
#define ACE_RMCAST_SENDER_PROXY_H
#include "ace/pre.h"

#include "RMCast.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_RMCast_Module;

class ACE_RMCast_Export ACE_RMCast_Sender_Proxy
{
  // = TITLE
  //   Reliable Multicast Sender Ambassador
  //
  // = DESCRIPTION
  //   Implement an Ambassador for the reliable multicast senders.
  //
public:
  ACE_RMCast_Sender_Proxy (ACE_RMCast_Module *module);
  // Constructor

  virtual ~ACE_RMCast_Sender_Proxy (void);
  // Destructor

  ACE_RMCast_Module *module (void) const;
  // Return the internal module

  virtual int receive_message (char *buffer, size_t size) = 0;
  // A new message has been received, process it

private:
  ACE_RMCast_Module *module_;
  // Process the data, control messages are processed by the Sender
  // proxy
};

#if defined (__ACE_INLINE__)
#include "RMCast_Sender_Proxy.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_RMCAST_SENDER_PROXY_H */

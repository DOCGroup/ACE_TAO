/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    RMCast_Sender_Proxy_Best_Effort.h
//
// = AUTHOR
//    Carlos O'Ryan <coryan@uci.edu>
//
// ============================================================================

#ifndef ACE_RMCAST_SENDER_PROXY_BEST_EFFORT_H
#define ACE_RMCAST_SENDER_PROXY_BEST_EFFORT_H
#include "ace/pre.h"

#include "RMCast_Sender_Proxy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_RMCast_Module;

class ACE_RMCast_Export ACE_RMCast_Sender_Proxy_Best_Effort : public ACE_RMCast_Sender_Proxy
{
  // = TITLE
  //   Reliable Multicast Sender Ambassador
  //
  // = DESCRIPTION
  //   Implement an Ambassador for the reliable multicast senders.
  //
public:
  ACE_RMCast_Sender_Proxy_Best_Effort (ACE_RMCast_Module *module);
  // Constructor

  ~ACE_RMCast_Sender_Proxy_Best_Effort (void);
  // Destructor

  virtual int receive_message (char *buffer, size_t size);
  // A DATA message was received.
};

#if defined (__ACE_INLINE__)
#include "RMCast_Sender_Proxy_Best_Effort.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_RMCAST_SENDER_PROXY_BEST_EFFORT_H */

/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    RMCast_Membership.h
//
// = AUTHOR
//    Carlos O'Ryan <coryan@uci.edu>
//
// ============================================================================

#ifndef ACE_RMCAST_MEMBERSHIP_H
#define ACE_RMCAST_MEMBERSHIP_H
#include "ace/pre.h"

#include "RMCast_Module.h"
#include "ace/Containers.h"
#include "ace/Synch.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_RMCast_Proxy;

class ACE_RMCast_Export ACE_RMCast_Membership : public ACE_RMCast_Module
{
  // = TITLE
  //     Track Receiver membership
  //
  // = DESCRIPTION
  //     Define the interface for all reliable multicast membership
public:
  // = Initialization and termination methods.
  ACE_RMCast_Membership (void);
  // Constructor

  virtual ~ACE_RMCast_Membership (void);
  // Destructor

  // = The RMCast_Module methods
  virtual int ack (ACE_RMCast::Ack &);
  virtual int join (ACE_RMCast::Join &);
  virtual int leave (ACE_RMCast::Leave &);

protected:
  typedef ACE_Unbounded_Set<ACE_RMCast_Proxy*> Proxy_Collection;
  typedef ACE_Unbounded_Set_Iterator<ACE_RMCast_Proxy*> Proxy_Iterator;

  Proxy_Collection proxies_;
  // The membership buffer

  ACE_UINT32 highest_in_sequence_;
  // The smallest value of <highest_in_sequence> for all the proxies

  ACE_UINT32 highest_received_;
  // The highest value of <highest_received> for all the proxies

  ACE_SYNCH_MUTEX mutex_;
  // Synchronization
};

#if defined (__ACE_INLINE__)
#include "RMCast_Membership.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_RMCAST_MEMBERSHIP_H */

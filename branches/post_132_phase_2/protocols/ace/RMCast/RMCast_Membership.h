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

/// Track peer membership
/**
 * Reliable senders of events need to know exactly how many peers are
 * receiving the events, and how many events has each peer received so
 * far.
 * This class uses the Join, Leave and Ack messages to build that
 * information, it also summarizes the Ack events and propagate only
 * the global info to the upper layer.
 */
class ACE_RMCast_Export ACE_RMCast_Membership : public ACE_RMCast_Module
{
public:
  /// Constructor
  ACE_RMCast_Membership (void);

  /// Destructor
  virtual ~ACE_RMCast_Membership (void);

  /// Return 1 if there are still members in the group
  int has_members (void);

  /// Receive an process an Ack message
  /**
   * After receiving the Ack message we find out what is the lowest
   * sequence number received in order among all the acks received by
   * the proxies in the collection. We also find out what is the
   * highest sequence number received by any proxy.
   * We only propagate that information back to the upper layer, and
   * then only if there are any news since the last Ack.
   */
  virtual int ack (ACE_RMCast::Ack &);

  /// Add a new member to the collection, using the <source> field in
  /// the Join message
  virtual int join (ACE_RMCast::Join &);

  /// Remove a member from the collection, using the <source> field in
  /// the Join message
  virtual int leave (ACE_RMCast::Leave &);

private:
  /// Generate an Ack message, normally due to changes in the
  /// collection, such as new proxys joining or leaving
  int generate_ack (ACE_RMCast_Proxy *proxy);

  /// Compute an Ack message to propagate to the upper layers.
  int compute_ack_i (ACE_RMCast_Proxy *source,
                     ACE_RMCast::Ack &next_ack);

protected:
  /// Use an unbounded set to maintain the collection of proxies.
  typedef ACE_Unbounded_Set<ACE_RMCast_Proxy*> Proxy_Collection;
  typedef ACE_Unbounded_Set_Iterator<ACE_RMCast_Proxy*> Proxy_Iterator;

  /// The collection of proxies
  Proxy_Collection proxies_;

  /// The smallest value of \param next_expected for all the proxies
  ACE_UINT32 next_expected_;

  /// The highest value of \param highest_received for all the proxies
  ACE_UINT32 highest_received_;

  /// Synchronization
  ACE_SYNCH_MUTEX mutex_;
};

#if defined (__ACE_INLINE__)
#include "RMCast_Membership.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_RMCAST_MEMBERSHIP_H */

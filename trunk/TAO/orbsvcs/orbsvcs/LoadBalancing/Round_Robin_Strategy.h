// -*- C++ -*-

// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO_LoadBalancing
//
// = FILENAME
//    Round_Robin_Strategy.h
//
// = AUTHOR
//    Ossama Othman <ossama@uci.edu>
//
// ============================================================================

#ifndef ROUND_ROBIN_STRATEGY_H
#define ROUND_ROBIN_STRATEGY_H

#include "ace/pre.h"

#include "LoadBalancing_Strategy.h"
#include "orbsvcs/LoadBalancingS.h"
#include "ace/Containers.h"
#include "ace/Synch.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#  pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward declarations
class TAO_LB_ReplicaProxy;

// @@ Ossama: is this the kind of data structure that you want to use
// in this case? I mean the iterator is invalidated on each insertion
// and extraction, the class is not thread safe and it is hard to find
// the element with the least load...  Then again the number of
// servers should be small (say around 10), and additions/removals
// rare, so the data structure is probably not a real problem.

typedef ACE_Unbounded_Set<TAO_LB_ReplicaProxy *> TAO_LB_ReplicaProxySet;
typedef ACE_Unbounded_Set_Iterator<TAO_LB_ReplicaProxy *> TAO_LB_ReplicaProxySetIterator;

class TAO_LoadBalancing_Export TAO_LB_Round_Robin_Strategy : public TAO_LB_LoadBalancing_Strategy
{
  // = TITLE
  //    Round Robin load balancing strategy

  // = DESCRIPTION
  //    Simple load balancing strategy that causes requests to be
  //    forwarded to next Replica in the set, i.e. in a "round robin"
  //    fashion.

public:
  TAO_LB_Round_Robin_Strategy (void);
  // Constructor.

  ~TAO_LB_Round_Robin_Strategy (void);
  // Destructor

  // = The Load_Balancing_Strategy methods
  virtual CORBA::Object_ptr replica (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual int insert (TAO_LB_ReplicaProxy *);
  virtual int remove (TAO_LB_ReplicaProxy *);

private:
  TAO_LB_ReplicaProxySet proxies_;
  // Set containing the ReplicaProxy servants.

  ACE_Unbounded_Set_Iterator<TAO_LB_ReplicaProxy *> next_replica_;

  TAO_SYNCH_MUTEX lock_;
  // Lock used to ensure access to state within this class is atomic.
};

#include "ace/post.h"

#endif  /* ROUND_ROBIN_STRATEGY_H */

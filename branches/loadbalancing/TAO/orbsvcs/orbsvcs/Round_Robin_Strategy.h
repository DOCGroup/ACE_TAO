// -*- C++ -*-

// $Id$

// ============================================================================
//
// = LIBRARY
//    orbsvcs
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

#include "LoadBalancing_Strategy.h"
#include "orbsvcs/LoadBalancingS.h"
#include "ace/Containers.h"

// Forward declarations
class ReplicaProxy_Impl;

// @@ Ossama: is this the kind of data structure that you want to use
// in this case? I mean the iterator is invalidated on each insertion
// and extraction, the class is not thread safe and it is hard to find
// the element with the least load...  Then again the number of
// servers should be small (say around 10), and additions/removals
// rare, so the data structure is probably not a real problem.

typedef ACE_Unbounded_Set<ReplicaProxy_Impl *> ReplicaProxySet;
typedef ACE_Unbounded_Set_Iterator<ReplicaProxy_Impl *> ReplicaProxySetIterator;

class TAO_LoadBalancing_Export Round_Robin_Strategy : public Load_Balancing_Strategy
{
  // = TITLE
  //    Round Robin load balancing strategy

  // = DESCRIPTION
  //    Simple load balancing strategy that causes requests to be
  //    forwarded to next Replica in the set, i.e. in a "round robin"
  //    fashion.

public:
  Round_Robin_Strategy (void);
  // Constructor.

  ~Round_Robin_Strategy (void);
  // Destructor

  // = The Load_Balancing_Strategy methods
  virtual CORBA::Object_ptr replica (CORBA_Environment &ACE_TRY_ENV);
  virtual int insert (ReplicaProxy_Impl *);
  virtual int remove (ReplicaProxy_Impl *);

private:
  ReplicaProxySet proxies_;
  // Set containing the ReplicaProxy servants.

  ACE_Unbounded_Set_Iterator<ReplicaProxy_Impl*> next_replica_;
};

#endif  /* ROUND_ROBIN_STRATEGY_H */

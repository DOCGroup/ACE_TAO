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

#include "orbsvcs/LoadBalancingS.h"
#include "orbsvcs/LoadBalancing/LoadBalancing_Strategy.h"


#ifndef ROUND_ROBIN_STRATEGY_H
#define ROUND_ROBIN_STRATEGY_H


// Forward declarations
class ReplicaProxy_Impl;
template class ACE_Unbounded_Set<ReplicaProxy_Impl *>;

typedef ACE_Unbounded_Set<ReplicaProxy_Impl *>
        ReplicaProxySet;


class Round_Robin_Strategy : public Load_Balancing_Strategy
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

  virtual CORBA::Object_ptr replica (CORBA_Environment &ACE_TRY_ENV);
  // Return the object reference to the Replica to which requests should
  // be redirected.

private:

  ReplicaProxySet proxies_;
  // Set containing the ReplicaProxy servants.
};


#endif  /* ROUND_ROBIN_STRATEGY_H */

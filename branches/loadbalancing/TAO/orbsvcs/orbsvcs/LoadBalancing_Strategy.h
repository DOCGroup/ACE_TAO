// -*- C++ -*-

// $Id$

// ============================================================================
//
// = LIBRARY
//    orbsvcs
//
// = FILENAME
//    LoadBalancing_Strategy.h
//
// = AUTHOR
//    Ossama Othman <ossama@uci.edu>
//
// ============================================================================

#ifndef LOAD_BALANCING_STRATEGY_H
#define LOAD_BALANCING_STRATEGY_H

#include "orbsvcs/LoadBalancingS.h"

class ReplicaProxy_Impl;

// The abstract load balancing strategy class.

class Load_Balancing_Strategy
{
  // = TITLE
  //    Load balancing strategy abstract base class.

  // = DESCRIPTION
  //    This abstract base class defines the interface load balancing
  //    strategies should implement.

public:

  virtual CORBA::Object_ptr replica (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException)) = 0;
  // Return the object reference to the Replica to which requests should
  // be redirected.

  virtual int insert (ReplicaProxy_Impl *) = 0;
  // Insert ReplicaProxy servant into the set of replica proxies upon
  // which the load balancing algorithm is performed.

  virtual int remove (ReplicaProxy_Impl *) = 0;
  // Remove ReplicaProxy servant from the set of replica proxies.
};


#if !defined (__ACE_INLINE__)
#include "LoadBalancing_Strategy.i"
#endif /* __ACE_INLINE__ */

#endif  /* LOAD_BALANCING_STRATEGY_H */

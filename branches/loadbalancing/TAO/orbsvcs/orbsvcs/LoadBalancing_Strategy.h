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

// @@ Ossama: the general comments on filenames, class names,
// TAO_XXX_Export an other things like that apply.

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
  virtual ~Load_Balancing_Strategy (void);
  // Destructor

  // @@ The name of the method should be more meaningful, what about:
  //    select_replica(), select_unloaded_replica() or something like
  //    that?
  virtual CORBA::Object_ptr replica (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException)) = 0;
  // Return the object reference to the Replica to which requests should
  // be redirected.

  virtual int insert (ReplicaProxy_Impl *) = 0;
  // Insert ReplicaProxy servant into the set of replica proxies upon
  // which the load balancing algorithm is performed.

  virtual int remove (ReplicaProxy_Impl *) = 0;
  // Remove ReplicaProxy servant from the set of replica proxies.

  virtual void load_changed (ReplicaProxy_Impl *impl,
                             CORBA::Environment &ACE_TRY_ENV);
  // The load on one proxy has changed.
  // @@ Ossama: see my comments on LoadBalancing_i about changing the
  // signature of this method.
};

#if !defined (__ACE_INLINE__)
#include "LoadBalancing_Strategy.i"
#endif /* __ACE_INLINE__ */

#endif  /* LOAD_BALANCING_STRATEGY_H */

// -*- C++ -*-

// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO_LoadBalancing
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

#include "ace/pre.h"

#include "orbsvcs/LoadBalancingS.h"
#include "LoadBalancing_export.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_LB_ReplicaProxy;

// The abstract load balancing strategy class.

class TAO_LoadBalancing_Export TAO_LB_LoadBalancing_Strategy
{
  // = TITLE
  //    Load balancing strategy abstract base class.

  // = DESCRIPTION
  //    This abstract base class defines the interface load balancing
  //    strategies should implement.

public:
  virtual ~TAO_LB_LoadBalancing_Strategy (void);
  // Destructor

  // @@ The name of the method should be more meaningful, what about:
  //    select_replica(), select_unloaded_replica() or something like
  //    that?
  virtual CORBA::Object_ptr replica (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException)) = 0;
  // Return the object reference to the Replica to which requests should
  // be redirected.

  virtual int insert (TAO_LB_ReplicaProxy *) = 0;
  // Insert ReplicaProxy servant into the set of replica proxies upon
  // which the load balancing algorithm is performed.

  virtual int remove (TAO_LB_ReplicaProxy *) = 0;
  // Remove ReplicaProxy servant from the set of replica proxies.

  virtual void load_changed (TAO_LB_ReplicaProxy *proxy
                             ACE_ENV_ARG_DECL);
  // The load on one proxy has changed.
  // @@ Ossama: see my comments on LoadBalancing_i about changing the
  // signature of this method.
};

#if !defined (__ACE_INLINE__)
#include "LoadBalancing_Strategy.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"

#endif  /* LOAD_BALANCING_STRATEGY_H */

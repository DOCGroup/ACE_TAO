// -*- C++ -*-

// $Id$

// ============================================================================
//
// = LIBRARY
//    orbsvcs
//
// = FILENAME
//    ReplicaProxy.h
//
// = AUTHOR
//    Ossama Othman <ossama@uci.edu>
//
// ============================================================================

#ifndef TAO_REPLICAPROXY_H
#define TAO_REPLICAPROXY_H

#include "orbsvcs/LoadBalancingS.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class LoadBalancer_Impl;
// Forward declaration

class TAO_LoadBalancing_Export ReplicaProxy_Impl :
  public virtual POA_LoadBalancing::ReplicaProxy,
  public virtual PortableServer::RefCountServantBase
{
  friend LoadBalancer_Impl;
public:

  ReplicaProxy_Impl (void);

  CORBA::Float current_load (void) const;
  // Read the stored load of the Object being load balanced from the
  // ReplicaProxy.

  CORBA::Object_ptr replica (void);
  // Return a reference to the Replica.

  virtual void current_load (CORBA::Float load,
                             CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((LoadBalancing::ReplicaProxy::InvalidLoad,
                     CORBA::SystemException));
  // Send the value of the current load on the Object being load
  // balanced, where the definition of "load" is load balancing
  // algorithm specific.
  // @@ Ossama: i had to implement dampening mechanisms in the load,
  // otherwise the spikes where killing me, but i'm not sure if that
  // should be implemented in the client or the server.
  // @@ I think that the 'Right Thing' is to have the client send
  // 'instantaneous load' and get the load balancing service to do the
  // dampening... that way we can modify the algorithm in the server,
  // without affecting the client, or detect rapid raises in load and
  // react proactively to those...

  virtual void disconnect (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Send a request to disconnect from the LoadBalancer.

  // @@ Ossama: could you please fix the mess i left here? it is my
  // fault, but i was running short on time.
  int has_high_load_;
  // Has the high load advisory been sent
  // @@ Ossama: i added this flag to avoid multiple nomimal load
  // advisories and high load advisories to be sent.

  LoadBalancing::ReplicaControl_var control_;
  // Reference to the ReplicaControl.

private:

  void connect (LoadBalancer_Impl *balancer,
                LoadBalancing::ReplicaControl_ptr control,
                CORBA::Object_ptr replica,
                CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((LoadBalancing::ReplicaProxy::NilControl,
                     LoadBalancing::ReplicaProxy::NilReplica,
                     CORBA::SystemException));
  // Register the ReplicaControl and the Object being load balanced
  // with the ReplicaProxy.

private:
  CORBA::Object_var replica_;
  // Reference to the Object being load balanced.

  LoadBalancer_Impl *balancer_;
  // Pointer to the Load Balancer this ReplicaProxy is registered
  // with.

  CORBA::Float current_load_;
  // The current load on the Object being load balanced.

  CORBA::Boolean connected_;
  // 1 == currently connected to load balancer
  // 0 == currently disconnected from load balancer
};

#if defined(__ACE_INLINE__)
#include "ReplicaProxy.i"
#endif /* __ACE_INLINE__ */

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif  /* TAO_REPLICAPROXY_H */

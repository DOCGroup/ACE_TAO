// -*- C++ -*-

// $Id$

// ============================================================================
//
// = LIBRARY
//    orbsvcs
//
// = FILENAME
//    LoadBalancer_i.h
//
// = AUTHOR
//    Ossama Othman <ossama@uci.edu>
//
// ============================================================================

#ifndef TAO_LOADBALANCER_I_H
#define TAO_LOADBALANCER_I_H

#include "orbsvcs/LoadBalancingS.h"
#include "orbsvcs/LoadBalancing/ReplicaProxy.h"
#include "orbsvcs/LoadBalancing/DSI_ForwardingProxy.h"
#include "orbsvcs/LoadBalancing/LoadBalancing_Strategy.h"

// @@ Ossama: please always remember the #pragma once.

// @@ Ossama: The name of this class should be TAO_XXX_LoadBalancer,
// or something similar (i vote for XXX == LB)
// @@ Ossama: the name of the file and the class are inconsistent.
// @@ Ossama: now is when your idea of keeping the strategy separate
// pays off: we could add strategies to detect misbehaving replicas,
// for example, if they don't send any updates in X seconds then they
// are removed, or maybe we poll them (using _non_existent) before
// removing.  I'm sure there are other aspects of the system that
// could be strategized.

class TAO_LoadBalancing_Export LoadBalancer_Impl : public virtual POA_LoadBalancing::LoadBalancer
{
public:
  LoadBalancer_Impl (const char *interface_id,
                     Load_Balancing_Strategy *strategy,
                     PortableServer::POA_ptr poa);
  // Constructor that initializes this Load Balancer for use with a
  // Replica that has the specified interface repository ID.

  ~LoadBalancer_Impl (void);
  // Destructor.

  // Local methods

  // @@ Ossama: this method is something of my own invention, but i
  // think we need to change the interface, to something like:
  //    load_changed (float old_load, float new_load, proxy)
  // that should let us keep the average load (and other similar
  // things) pre-computed.
  //
  void load_changed (ReplicaProxy_Impl *proxy,
                     CORBA::Environment &ACE_TRY_ENV);
  // The load for <proxy> has changed, the LoadBalancer can use this
  // opportunity to determine if the load on one of the services is
  // too high.

  void disconnect (ReplicaProxy_Impl *proxy,
                   CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Disconnect proxy from the Load Balancer.

  CORBA::Object_ptr replica (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Return a reference to the Replica to which client requests should
  // be redirected next.

  // = See the descriptions in LoadBalancing.idl
  virtual LoadBalancing::ReplicaProxy_ptr connect (
        LoadBalancing::ReplicaControl_ptr control,
        CORBA::Object_ptr replica,
        CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((LoadBalancing::ReplicaProxy::NilControl,
                     LoadBalancing::ReplicaProxy::NilReplica,
                     CORBA::SystemException));
  virtual CORBA::Object_ptr group_identity (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  DSI_ForwardingProxy redirector_;
  // The object that tells the invoking client to forward its requests
  // from the LoadBalancer to an actual replica.

  Load_Balancing_Strategy *strategy_;
  // The underlying load balancing strategy.

  PortableServer::POA_var poa_;
  // The POA where the forwarding proxy is activated.

  CORBA::Object_var group_identity_;
  // The group identity
};

#if defined (__ACE_INLINE__)
#include "LoadBalancer_i.i"
// @@ Ossama: notice that i use relative paths!
#endif /* __ACE_INLINE__ */

#endif  /* TAO_LOADBALANCER_I_H */

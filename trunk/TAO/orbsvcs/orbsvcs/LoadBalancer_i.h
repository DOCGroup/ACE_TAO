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
#include "ReplicaProxy.h"
#include "DSI_ForwardingProxy.h"
#include "LoadBalancing_Strategy.h"
#include "LoadBalancing_export.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

// @@ Ossama: the name of the file and the class are inconsistent.
// @@ Ossama: now is when your idea of keeping the strategy separate
// pays off: we could add strategies to detect misbehaving replicas,
// for example, if they don't send any updates in X seconds then they
// are removed, or maybe we poll them (using _non_existent) before
// removing.  I'm sure there are other aspects of the system that
// could be strategized.

class TAO_LoadBalancing_Export TAO_LB_LoadBalancer : public virtual POA_LoadBalancing::LoadBalancer
{
public:
  TAO_LB_LoadBalancer (const char *interface_id,
                       TAO_LB_LoadBalancing_Strategy *strategy,
                       PortableServer::POA_ptr poa);
  // Constructor that initializes this Load Balancer for use with a
  // Replica that has the specified interface repository ID.

  ~TAO_LB_LoadBalancer (void);
  // Destructor.

  // Local methods

  // @@ Ossama: this method is something of my own invention, but i
  // think we need to change the interface, to something like:
  //    load_changed (float old_load, float new_load, proxy)
  // that should let us keep the average load (and other similar
  // things) pre-computed.
  //
  void load_changed (TAO_LB_ReplicaProxy *proxy,
                     CORBA::Environment &ACE_TRY_ENV);
  // The load for <proxy> has changed, the LoadBalancer can use this
  // opportunity to determine if the load on one of the services is
  // too high.

  void disconnect (TAO_LB_ReplicaProxy *proxy,
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
  int init (void);
  // Initialize the <redirector_> DSI forwarding proxy.

private:
  TAO_LB_DSI_ForwardingProxy redirector_;
  // The object that tells the invoking client to forward its requests
  // from the LoadBalancer to an actual replica.

  TAO_LB_LoadBalancing_Strategy *strategy_;
  // The underlying load balancing strategy.

  PortableServer::POA_var poa_;
  // The POA where the forwarding proxy is activated.

  CORBA::Object_var group_identity_;
  // The group identity
};

#if defined (__ACE_INLINE__)
#include "LoadBalancer_i.i"
#endif /* __ACE_INLINE__ */

#endif  /* TAO_LOADBALANCER_I_H */

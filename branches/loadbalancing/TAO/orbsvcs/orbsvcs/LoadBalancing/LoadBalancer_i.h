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

  virtual LoadBalancing::ReplicaProxy_ptr connect (
        LoadBalancing::ReplicaControl_ptr control,
        CORBA::Object_ptr replica,
        CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((LoadBalancing::ReplicaProxy::NilControl,
                     LoadBalancing::ReplicaProxy::NilReplica,
                     CORBA::SystemException));
  // Register ReplicaControl and Object being load balanced with
  // ReplicaProxy and connect to Load Balancer.

  virtual CORBA::Object_ptr group_identity (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Return the reference to the object that represents the Replica
  // group being load balanced.  This "group identity" object will
  // cause the client to redirect its requests to a Replica that fits
  // a specific load balancing criteria.

  // Local methods

  void load_changed (ReplicaProxy_Impl *proxy,
                     CORBA::Environment &ACE_TRY_ENV);

  void disconnect (ReplicaProxy_Impl *proxy,
                   CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Disconnect proxy from the Load Balancer.

  CORBA::Object_ptr replica (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Return a reference to the Replica to which client requests should
  // be redirected next.

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
#endif /* __ACE_INLINE__ */

#endif  /* TAO_LOADBALANCER_I_H */

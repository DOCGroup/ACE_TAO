// -*- C++ -*-

// $Id$

#include "orbsvcs/LoadBalancingS.h"

#ifndef TAO_HASH_REPLICACONTROL_H
#define TAO_HASH_REPLICACONTROL_H

class Hash_ReplicaControl : public virtual POA_LoadBalancing::ReplicaControl
{
public:
  Hash_ReplicaControl (LoadBalancing::LoadBalancer_ptr balancer);
  // Constructor.

  virtual void high_load_advisory (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void nominal_load_advisory (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::Object_ptr replica (void);
  // Return a reference to the replica being load balanced.

private:
  LoadBalancing::LoadBalancer_var balancer_;
  // Object reference to the load balancer.

  Hash_Replica_Impl replica_;
  // Replica being load balanced.

  
};

#endif  /* TAO_HASH_REPLICACONTROL_H */

// -*- C++ -*-

// $Id$


#include "Hash_ReplicaControl.h"

Hash_ReplicaControl::Hash_ReplicaControl (
  LoadBalancing::LoadBalancer_ptr balancer)
  : balancer_ (balancer),
    replica_ (balancer->group_identity ())
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      LoadBalancing::ReplicaControl_var control =
        this->_this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::Object_var replica = this->replica_._this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      this->replica.balancer_proxy (
                      this->balancer_->connect (control,
                                                replica,
                                                ACE_TRY_ENV));
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "LoadBalancer::connect");
    }
  ACE_END_TRY;
}

void
Hash_ReplicaControl::high_load_advisory (CORBA::Environment &
                                         /* ACE_TRY_ENV */)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Notify the replica that it should reject all requests.
  this->replica_.reject_requests (1);
}

void
Hash_ReplicaControl::nominal_load_advisory (CORBA::Environment &
                                            /* ACE_TRY_ENV */)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Notify the replica that it should once again accept requests.
  this->replica_.reject_requests (0);
}

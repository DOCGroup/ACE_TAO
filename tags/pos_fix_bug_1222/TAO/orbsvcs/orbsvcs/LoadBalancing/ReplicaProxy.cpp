// -*- C++ -*-

// $Id$

#include "ReplicaProxy.h"
#include "LoadBalancer_i.h"

#if !defined(__ACE_INLINE__)
#include "ReplicaProxy.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(orbsvcs, ReplicaProxy, "$Id$")

TAO_LB_ReplicaProxy::TAO_LB_ReplicaProxy (void)
  : has_high_load_ (0),
    lock_ (),
    balancer_ (0),
    current_load_ (0),
    connected_ (0)
{
  // Nothing as yet
}

void
TAO_LB_ReplicaProxy::current_load (CORBA::Float load
                                   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ Ossama: this is the point were the load dampening should
  // happen. Probably strategized....

  // Do not lock here.  Locking is done in the load_changed() method,
  // below.

  this->current_load_ = load;
  // ACE_DEBUG ((LM_DEBUG, "Load[%x] = %f\n", long(this), load));

  this->balancer_->load_changed (this ACE_ENV_ARG_PARAMETER);
}

void
TAO_LB_ReplicaProxy::disconnect (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((LoadBalancing::ReplicaProxy::NotConnected,
                   CORBA::SystemException))
{
  ACE_MT (ACE_GUARD (TAO_SYNCH_MUTEX,
                     guard,
                     this->lock_));

  if (this->connected_)
    {
      this->balancer_->disconnect (this ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      this->connected_ = 0;
    }
  else
    {
      ACE_THROW (LoadBalancing::ReplicaProxy::NotConnected ());
    }
}

void
TAO_LB_ReplicaProxy::connect (TAO_LB_LoadBalancer *balancer,
                              LoadBalancing::ReplicaControl_ptr control,
                              CORBA::Object_ptr replica
                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((LoadBalancing::ReplicaProxy::NilControl,
                   LoadBalancing::ReplicaProxy::NilReplica,
                   CORBA::SystemException))
{
  ACE_MT (ACE_GUARD (TAO_SYNCH_MUTEX,
                     guard,
                     this->lock_));

  if (balancer == 0)
    ACE_THROW (CORBA::BAD_PARAM (
      CORBA_SystemException::_tao_minor_code (
        TAO_DEFAULT_MINOR_CODE,
        EINVAL),
      CORBA::COMPLETED_NO));
  else
    this->balancer_ = balancer;

  if (!CORBA::is_nil (control))
    this->control_ = LoadBalancing::ReplicaControl::_duplicate (control);
  else
    ACE_THROW (LoadBalancing::ReplicaProxy::NilControl ());

  if (!CORBA::is_nil (control))
      this->replica_ = CORBA::Object::_duplicate (replica);
  else
    ACE_THROW (LoadBalancing::ReplicaProxy::NilReplica ());

  // Now indicate that connection to LoadBalancer has been made.
  this->connected_ = 1;
}

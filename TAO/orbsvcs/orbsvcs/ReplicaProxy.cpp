// -*- C++ -*-

// $Id$

#include "orbsvcs/LoadBalancing/ReplicaProxy.h"
#include "orbsvcs/LoadBalancing/LoadBalancer_i.h"

ACE_RCSID(orbsvcs, ReplicaProxy, "$Id$")

ReplicaProxy_Impl::ReplicaProxy_Impl (void)
  : control_ (),
    replica_ (),
    balancer_ (0),
    current_load_ (0),
    connected_ (0)
{
  // Nothing as yet
}

void
ReplicaProxy_Impl::current_load (CORBA::Float load,
                                 CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((LoadBalancing::ReplicaProxy::InvalidLoad,
                   CORBA::SystemException))
{
  if (load >= 0)  // @@ Is zero a valid load?
      this->current_load_ = load;
  else
    ACE_THROW (LoadBalancing::ReplicaProxy::InvalidLoad ());
}

void
ReplicaProxy_Impl::disconnect (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((LoadBalancing::ReplicaProxy::NotConnected,
                   CORBA::SystemException))
{
  if (this->connected_)
    {
      this->balancer_->disconnect (this, ACE_TRY_ENV);
      ACE_CHECK;

      this->connected_ = 0;
    }
  else
    {
      ACE_THROW (LoadBalancing::ReplicaProxy::NotConnected ());
    }
}

void ReplicaProxy_Impl::connect (LoadBalancer_Impl *balancer,
                                 LoadBalancing::ReplicaControl_ptr control,
                                 CORBA::Object_ptr replica,
                                 CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((LoadBalancing::ReplicaProxy::NilControl,
                   LoadBalancing::ReplicaProxy::NilReplica,
                   CORBA::SystemException))
{
  if (balancer == 0)
    ACE_THROW (CORBA::BAD_PARAM (
      CORBA_SystemException::_tao_minor_code (
        TAO_NULL_POINTER_MINOR_CODE,
        0),
      CORBA::COMPLETED_NO));
  else
    this->balancer_ = balancer;

  if (!CORBA::is_nil (control))
    this->control_ =  LoadBalancing::ReplicaControl::_duplicate (control);
  else
    ACE_THROW (LoadBalancing::ReplicaProxy::NilControl ());
  
  if (!CORBA::is_nil (control))
      this->replica_ = CORBA::Object::_duplicate (replica);
  else
    ACE_THROW (LoadBalancing::ReplicaProxy::NilReplica ());

  // Now indicate that connection to LoadBalancer has been made.
  this->connected_ = 1;
}

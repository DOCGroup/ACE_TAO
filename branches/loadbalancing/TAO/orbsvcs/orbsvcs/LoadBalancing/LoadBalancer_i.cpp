// -*- C++ -*-

// $Id$

#include "orbsvcs/LoadBalancing/LoadBalancer_i.h"
#include "orbsvcs/LoadBalancing/ReplicaProxy.h"

ACE_RCSID(orbsvcs, Load_Balancer_i, "$Id$")

#if !defined (__ACE_INLINE__)
#include "LoadBalancer_i.i"
#endif /* __ACE_INLINE__ */

LoadBalancer_Impl::LoadBalancer_Impl (const char *interface_id,
                                      Load_Balancing_Strategy *strategy,
                                      PortableServer::POA_ptr poa)
  : redirector_ (this, interface_id),
    strategy_ (strategy),
    poa_ (PortableServer::POA::_duplicate (poa))
{
  // Nothing else

  ACE_TRY_NEW_ENV
    {
      PortableServer::ObjectId_var oid =
        this->poa_->activate_object (&this->redirector_,
                                     ACE_TRY_ENV);
      ACE_TRY_CHECK;

      this->group_identity_ =
        this->poa_->id_to_reference (oid.in (),
                                     ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;
}

LoadBalancer_Impl::~LoadBalancer_Impl (void)
{
  // Nothing else
}

LoadBalancing::ReplicaProxy_ptr
LoadBalancer_Impl::connect (LoadBalancing::ReplicaControl_ptr control,
                            CORBA::Object_ptr replica,
                            CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((LoadBalancing::ReplicaProxy::NilControl,
                   LoadBalancing::ReplicaProxy::NilReplica,
                   CORBA::SystemException))
{
  ReplicaProxy_Impl *impl = 0;
  ACE_NEW_THROW_EX (impl,
                    ReplicaProxy_Impl,
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (LoadBalancing::ReplicaProxy::_nil ());

  PortableServer::ServantBase_var proxy_servant = impl;

  impl->connect (this, control, replica, ACE_TRY_ENV);
  ACE_CHECK_RETURN (LoadBalancing::ReplicaProxy::_nil ());

  if (this->strategy_->insert (impl) == -1)
    {
      ACE_THROW_RETURN (CORBA::INTERNAL (),
                        LoadBalancing::ReplicaProxy::_nil ());
    }

  return impl->_this (ACE_TRY_ENV);
}

CORBA::Object_ptr
LoadBalancer_Impl::group_identity (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::Object::_duplicate (this->group_identity_.in ());
}

void
LoadBalancer_Impl::load_changed (ReplicaProxy_Impl *proxy,
                                 CORBA::Environment &ACE_TRY_ENV)
{
  this->strategy_->load_changed (proxy, ACE_TRY_ENV);
}

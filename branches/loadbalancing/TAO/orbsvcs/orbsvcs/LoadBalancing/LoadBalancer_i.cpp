// -*- C++ -*-

// $Id$

#include "LoadBalancer_i.h"
#include "ReplicaProxy.h"

ACE_RCSID(orbsvcs, LoadBalancer, "$Id$")

#if !defined (__ACE_INLINE__)
#include "LoadBalancer_i.i"
#endif /* __ACE_INLINE__ */

TAO_LB_LoadBalancer::TAO_LB_LoadBalancer (
                            const char *interface_id,
                            TAO_LB_LoadBalancing_Strategy *strategy,
                            PortableServer::POA_ptr poa)
  : redirector_ (this, interface_id),
    strategy_ (strategy),
    poa_ (PortableServer::POA::_duplicate (poa))
{
  (void) this->init ();
}

TAO_LB_LoadBalancer::~TAO_LB_LoadBalancer (void)
{
  // Nothing else
}

LoadBalancing::ReplicaProxy_ptr
TAO_LB_LoadBalancer::connect (LoadBalancing::ReplicaControl_ptr control,
                              CORBA::Object_ptr replica,
                              CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((LoadBalancing::ReplicaProxy::NilControl,
                   LoadBalancing::ReplicaProxy::NilReplica,
                   CORBA::SystemException))
{
  TAO_LB_ReplicaProxy *proxy = 0;
  ACE_NEW_THROW_EX (proxy,
                    TAO_LB_ReplicaProxy,
                    CORBA::NO_MEMORY (
                      CORBA_SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (LoadBalancing::ReplicaProxy::_nil ());

  PortableServer::ServantBase_var proxy_servant = proxy;

  proxy->connect (this, control, replica, ACE_TRY_ENV);
  ACE_CHECK_RETURN (LoadBalancing::ReplicaProxy::_nil ());

  if (this->strategy_->insert (proxy) == -1)
    {
      ACE_THROW_RETURN (CORBA::INTERNAL (),
                        LoadBalancing::ReplicaProxy::_nil ());
    }

  return proxy->_this (ACE_TRY_ENV);
}

CORBA::Object_ptr
TAO_LB_LoadBalancer::group_identity (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::Object::_duplicate (this->group_identity_.in ());
}

void
TAO_LB_LoadBalancer::load_changed (TAO_LB_ReplicaProxy *proxy,
                                   CORBA::Environment &ACE_TRY_ENV)
{
  this->strategy_->load_changed (proxy, ACE_TRY_ENV);
}

int
TAO_LB_LoadBalancer::init (void)
{
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
      // @@ Should we do anything here?
    }
  ACE_ENDTRY;
}

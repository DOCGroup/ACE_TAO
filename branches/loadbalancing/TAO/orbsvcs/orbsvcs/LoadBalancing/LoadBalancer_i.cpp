// -*- C++ -*-

// $Id$

#include "orbsvcs/LoadBalancing/LoadBalancer_i.h"
#include "orbsvcs/LoadBalancing/ReplicaProxy.h"

ACE_RCSID(orbsvcs, Load_Balancer_i, "$Id$")


#if defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "orbsvcs/LoadBalancing/LoadBalancer_i.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */
  
LoadBalancer_Impl::LoadBalancer_Impl (const char *interface_id,
                                      Load_Balancing_Strategy *strategy)
  : redirector_ (this, interface_id),
    strategy_ (strategy)
{
  // Nothing else
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
LoadBalancer_Impl::group_identity (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->redirector_._this (ACE_TRY_ENV);
}

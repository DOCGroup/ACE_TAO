// -*- C++ -*-

// $Id$


ACE_INLINE void
TAO_LB_LoadBalancer::disconnect (TAO_LB_ReplicaProxy *proxy
                                 TAO_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ Ossama, what is the we cannot raise that exception here!

  (void) this->strategy_->remove (proxy);

  // @@ Deactivate the proxy servant.

#if 0
  if (this->strategy_->remove (proxy) != 0)
    ACE_THROW (LoadBalancing::LoadBalancer::InvalidReplicaProxy ());
#endif /* 0 */
}


ACE_INLINE CORBA::Object_ptr
TAO_LB_LoadBalancer::replica (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->strategy_->replica (TAO_ENV_SINGLE_ARG_PARAMETER);
}

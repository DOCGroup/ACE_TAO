// -*- C++ -*-

// $Id$


ACE_INLINE void
TAO_LB_LoadBalancer::disconnect (TAO_LB_ReplicaProxy *proxy,
                                 CORBA::Environment &)
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
TAO_LB_LoadBalancer::replica (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->strategy_->replica (ACE_TRY_ENV);
}

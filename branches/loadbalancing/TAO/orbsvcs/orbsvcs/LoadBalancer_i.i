// -*- C++ -*-

// $Id$


ACE_INLINE void
LoadBalancer_Impl::disconnect (ReplicaProxy_Impl *proxy,
                               CORBA::Environment &ACE_TRY_ENV)
{
  if (this->strategy_->remove (proxy) != 0)
    ACE_THROW (LoadBalancing::LoadBalancer::InvalidReplicaProxy ());
}


ACE_INLINE CORBA::Object_ptr
LoadBalancer_Impl::replica (CORBA::Environment &ACE_TRY_ENV)
{
  return this->strategy_->replica (ACE_TRY_ENV);
}

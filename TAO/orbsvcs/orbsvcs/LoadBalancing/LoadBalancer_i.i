// -*- C++ -*-

// $Id$


ACE_INLINE void
TAO_LB_LoadBalancer::disconnect (TAO_LB_ReplicaProxy *proxy,
                                 CORBA::Environment &ACE_TRY_ENV)
{
  if (this->strategy_->remove (proxy) != 0)
    ACE_THROW (LoadBalancing::LoadBalancer::InvalidReplicaProxy ());
}


ACE_INLINE CORBA::Object_ptr
TAO_LB_LoadBalancer::replica (CORBA::Environment &ACE_TRY_ENV)
{
  return this->strategy_->replica (ACE_TRY_ENV);
}

// $Id$

#include "Properties.h"

ACE_INLINE TAO_NS_Factory*
TAO_NS_Properties::factory (void)
{
  return this->factory_;
}

ACE_INLINE void
TAO_NS_Properties::factory (TAO_NS_Factory* factory)
{
  this->factory_ = factory;
}

ACE_INLINE TAO_NS_Builder*
TAO_NS_Properties::builder (void)
{
  return this->builder_;
}

ACE_INLINE void
TAO_NS_Properties::builder (TAO_NS_Builder* builder)
{
  this->builder_ = builder;
}

ACE_INLINE CORBA::ORB_ptr
TAO_NS_Properties::orb (void)
{
  return CORBA::ORB::_duplicate (orb_.in ());
}

ACE_INLINE void 
TAO_NS_Properties::orb (CORBA::ORB_ptr orb)
{
  orb_ = CORBA::ORB::_duplicate (orb);
}

ACE_INLINE PortableServer::POA_ptr
TAO_NS_Properties::default_poa (void)
{
  return PortableServer::POA::_duplicate (this->default_poa_.in ());
}

ACE_INLINE void 
TAO_NS_Properties::default_poa (PortableServer::POA_ptr default_poa)
{
  this->default_poa_ = PortableServer::POA::_duplicate (default_poa);
}

ACE_INLINE long
TAO_NS_Properties::scope_policy (void)
{
  return thr_scope_policy_;
}

ACE_INLINE void
TAO_NS_Properties::scope_policy (long scope_policy)
{
  thr_scope_policy_ = scope_policy;
}
  
ACE_INLINE long
TAO_NS_Properties::sched_policy (void)
{
  return thr_sched_policy_;
}

ACE_INLINE void
TAO_NS_Properties::sched_policy (long sched_policy)
{
  thr_sched_policy_ = sched_policy;
}

// $Id$

ACE_INLINE PortableServer::ThreadPolicyValue
TAO_POA_Cached_Policies::thread (void) const
{
  return this->thread_;
}

ACE_INLINE PortableServer::LifespanPolicyValue
TAO_POA_Cached_Policies::lifespan (void) const
{
  return this->lifespan_;
}

ACE_INLINE PortableServer::IdUniquenessPolicyValue
TAO_POA_Cached_Policies::id_uniqueness (void) const
{
  return this->id_uniqueness_;
}

ACE_INLINE PortableServer::IdAssignmentPolicyValue
TAO_POA_Cached_Policies::id_assignment (void) const
{
  return this->id_assignment_;
}

ACE_INLINE PortableServer::ImplicitActivationPolicyValue
TAO_POA_Cached_Policies::implicit_activation (void) const
{
  return this->implicit_activation_;
}

ACE_INLINE PortableServer::ServantRetentionPolicyValue
TAO_POA_Cached_Policies::servant_retention (void) const
{
  return this->servant_retention_;
}

ACE_INLINE PortableServer::RequestProcessingPolicyValue
TAO_POA_Cached_Policies::request_processing (void) const
{
  return this->request_processing_;
}

ACE_INLINE TAO_POA_Cached_Policies::PriorityModel
TAO_POA_Cached_Policies::priority_model (void) const
{
  return this->priority_model_;
}

ACE_INLINE CORBA::Short
TAO_POA_Cached_Policies::server_priority (void) const
{
  return this->server_priority_;
}

ACE_INLINE void
TAO_POA_Cached_Policies::priority_model (PriorityModel priority_model)
{
  this->priority_model_ = priority_model;
}

ACE_INLINE void
TAO_POA_Cached_Policies::server_priority (CORBA::Short priority)
{
  this->server_priority_ = priority;
}

ACE_INLINE void
TAO_POA_Cached_Policies::implicit_activation (PortableServer::ImplicitActivationPolicyValue value)
{
  this->implicit_activation_ = value;
}

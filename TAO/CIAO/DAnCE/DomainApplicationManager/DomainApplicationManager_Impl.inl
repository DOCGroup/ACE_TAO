// -*- C++ -*-
//
// $Id$



ACE_INLINE const char *
CIAO::DomainApplicationManager_Impl_Base::get_uuid () const
{
  return this->uuid_;
}

ACE_INLINE void
CIAO::DomainApplicationManager_Impl_Base::set_uuid (const char * uuid)
{
  // Copy this uuid reference
  this->uuid_ = CORBA::string_dup (uuid);
}

ACE_INLINE
CIAO::DomainApplicationManager_Impl::
DomainApplicationManager_Impl (CORBA::ORB_ptr orb,
                               PortableServer::POA_ptr poa,
                               Deployment::TargetManager_ptr manager,
                               const Deployment::DeploymentPlan & plan,
                               const char * deployment_file)
  ACE_THROW_SPEC ((CORBA::SystemException))
    : DomainApplicationManager_Impl_Base (orb, poa, manager, plan, deployment_file)
{
}

ACE_INLINE
CIAO::DomainApplicationManager_Impl::
~DomainApplicationManager_Impl ()
{
}

ACE_INLINE ::Deployment::NodeManager_ptr
CIAO::DomainApplicationManager_Impl::
get_node_manager (const char* name)
{
  return this->deployment_config_.get_node_manager (name);
}

ACE_INLINE
CIAO::Static_DomainApplicationManager_Impl::
~Static_DomainApplicationManager_Impl ()
{
}

ACE_INLINE ::Deployment::NodeManager_ptr
CIAO::Static_DomainApplicationManager_Impl::
get_node_manager (const char* name)
{
  ACE_UNUSED_ARG (name);
  return ::Deployment::NodeManager::_duplicate (static_node_manager_.in ());
}

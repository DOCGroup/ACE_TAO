/* -*- C++ -*- */
// $Id$

ACE_INLINE
CIAO::NodeApplicationManager_Impl_Base::
NodeApplicationManager_Impl_Base (CORBA::ORB_ptr o,
                                  PortableServer::POA_ptr p)
  : orb_ (CORBA::ORB::_duplicate (o)),
    poa_ (PortableServer::POA::_duplicate (p)),
    callback_poa_ (PortableServer::POA::_nil ()),
    nodeapp_ (Deployment::NodeApplication::_nil ()),
    spawn_delay_ (5),
    waitCond_ (this->mutex_)
  // @@ (OO) The default size for an ACE_Hash_Map_Mapanger is quiet
  //         large.  The maximum size of an ACE_Hash_Map_Manager is
  //         also fixed, i.e. it does not grow dynamically on demand.
  //         Make sure the default size of component_map_ is
  //         appropriate for your needs.  You may also want to make
  //         the size configurable at compile-time, at least.
{
}

ACE_INLINE
void
CIAO::NodeApplicationManager_Impl_Base::
parse_config_value (ACE_CString &)
{
  throw CORBA::NO_IMPLEMENT();
}

ACE_INLINE
Deployment::NodeApplicationManager_ptr
CIAO::NodeApplicationManager_Impl_Base::get_nodeapp_manager (void)
{
  return this->objref_.in ();
}

ACE_INLINE void
CIAO::NodeApplicationManager_Impl_Base::
reset_plan (const ::Deployment::DeploymentPlan & plan)
{
  this->plan_ = plan;
}


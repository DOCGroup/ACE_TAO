/* -*- C++ -*- */
// $Id$

ACE_INLINE
CIAO::ComponentServer_Impl::ComponentServer_Impl (CORBA::ORB_ptr o,
                                                  PortableServer::POA_ptr p)
  : orb_ (CORBA::ORB::_duplicate (o)),
    poa_ (PortableServer::POA::_duplicate (p))
{
}

ACE_INLINE void
CIAO::ComponentServer_Impl::set_objref (Components::Deployment::ServerActivator_ptr act,
                                        const Components::ConfigValues &config,
                                        Components::Deployment::ComponentServer_ptr cs
                                        ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (!CORBA::is_nil (this->activator_.in ()) || !CORBA::is_nil (this->objref_.in ()))
    ACE_THROW (CORBA::BAD_INV_ORDER ());

  this->config_ = new Components::ConfigValues (config);
  this->activator_ = Components::Deployment::ServerActivator::_duplicate (act);
  this->objref_ = Components::Deployment::ComponentServer::_duplicate (cs);
}

ACE_INLINE Components::Deployment::ComponentServer_ptr
CIAO::ComponentServer_Impl::get_objref (ACE_ENV_SINGLE_ARG_DECL)
{
  if (CORBA::is_nil (this->objref_.in ()))
    {
      this->objref_ = this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }

  return this->objref_.in ();
}

ACE_INLINE Components::Deployment::ComponentInstallation_ptr
CIAO::ComponentServer_Impl::get_component_installation (ACE_ENV_SINGLE_ARG_DECL)
{
  if (CORBA::is_nil (this->installation_.in ()))
    {
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, 0);

      CORBA::Object_var tmp = this->orb_->resolve_initial_references ("ComponentInstallation"
                                                                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      this->installation_ =
        Components::Deployment::ComponentInstallation::_narrow (tmp.in ()
                                                                ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      if (CORBA::is_nil (this->installation_.in ()))
        ACE_THROW_RETURN (CORBA::INTERNAL (), 0);
    }

  return this->installation_.in ();
}

/* -*- C++ -*- */
// $Id$

ACE_INLINE
CIAO::NodeApplication_Impl::
NodeApplication_Impl (CORBA::ORB_ptr o,
                      PortableServer::POA_ptr p)

  : orb_ (CORBA::ORB::_duplicate (o)),
    poa_ (PortableServer::POA::_duplicate (p))
{
}

// Access the readonly attribute
Deployment::Properties *
CIAO::NodeApplication_Impl::properties (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  Deployment::Properties * tmp;

  ACE_NEW_THROW_EX (tmp,
                    Deployment::Properties (this->properties_),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  return tmp;
}

PortableServer::POA_ptr
CIAO::NodeApplication_Impl::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}


CORBA::Object_ptr
CIAO::NodeApplication_Impl::get_node_application_manager (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return ::CORBA::Object::_duplicate (this->node_app_manager_.in ());
}

/*
ACE_INLINE void
CIAO::NodeApplication_Impl::
set_objref (Components::Deployment::ServerActivator_ptr act,
            const Components::ConfigValues &config,
            Components::Deployment::ComponentServer_ptr cs
            ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (!CORBA::is_nil (this->node_app_manager_.in ()) || 
      !CORBA::is_nil (this->objref_.in ()))
    ACE_THROW (CORBA::BAD_INV_ORDER ());

  this->config_ = new Components::ConfigValues (config);
  this->activator_ = Components::Deployment::ServerActivator::_duplicate (act);
  this->objref_ = Components::Deployment::ComponentServer::_duplicate (cs);
}
*/

ACE_INLINE ::Deployment::NodeApplication_ptr
CIAO::NodeApplication_Impl::
get_objref (ACE_ENV_SINGLE_ARG_DECL)
{
  if (CORBA::is_nil (this->objref_.in ()))
    {
      this->objref_ = this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }

  return this->objref_.in ();
}

/* -*- C++ -*- */
// $Id$

ACE_INLINE
CIAO::Container_Impl::Container_Impl (CORBA::ORB_ptr o,
                                      PortableServer::POA_ptr p,
                                      Components::Deployment::ComponentServer_ptr s)
  : orb_ (CORBA::ORB::_duplicate (o)),
    poa_ (PortableServer::POA::_duplicate (p)),
    comserv_ (Components::Deployment::ComponentServer::_duplicate (s))
{
}

ACE_INLINE void
CIAO::Container_Impl::set_objref (Components::Deployment::Container_ptr o
                                  ACE_ENV_ARG_DECL_WITH_DEFAULTS)
{
  if (!CORBA::is_nil (this->objref_.in ()))
    ACE_THROW (CORBA::BAD_INV_ORDER ());

  this->objref_ = Components::Deployment::Container::_duplicate (o);
}

ACE_INLINE Components::Deployment::Container_ptr
CIAO::Container_Impl::get_objref (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
{
  if (CORBA::is_nil (this->objref_.in ()))
    {
      this->objref_ = this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }
  return this->objref_.in ();
}

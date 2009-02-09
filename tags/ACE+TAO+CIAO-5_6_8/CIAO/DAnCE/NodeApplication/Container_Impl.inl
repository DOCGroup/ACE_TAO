/* -*- C++ -*- */
// $Id$

ACE_INLINE
CIAO::Container_Impl::Container_Impl (CORBA::ORB_ptr o,
                                      PortableServer::POA_ptr p,
                                      ::Deployment::NodeApplication_ptr server,
                                      NodeApp_Configurator &c,
                                      const Static_Config_EntryPoints_Maps* static_entrypts_maps)
  : orb_ (CORBA::ORB::_duplicate (o)),
    poa_ (PortableServer::POA::_duplicate (p)),
    nodeapp_ (::Deployment::NodeApplication::_duplicate (server)),
    configurator_ (c),
    static_entrypts_maps_ (static_entrypts_maps)
{
}

ACE_INLINE void
CIAO::Container_Impl::set_objref (Deployment::Container_ptr o)
{
  CIAO_TRACE("CIAO::Container_Imp::set_objref");

  if (!CORBA::is_nil (this->objref_.in ()))
    throw CORBA::BAD_INV_ORDER ();

  this->objref_ = Deployment::Container::_duplicate (o);
}

ACE_INLINE ::Deployment::Container_ptr
CIAO::Container_Impl::get_objref ()
{
  CIAO_TRACE("CIAO::Container_Imp::get_objref");

  if (CORBA::is_nil (this->objref_.in ()))
    {
      this->objref_ = this->_this ();
    }
  return Deployment::Container::_duplicate (this->objref_.in ());
}

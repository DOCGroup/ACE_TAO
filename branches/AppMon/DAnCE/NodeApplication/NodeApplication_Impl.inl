/* -*- C++ -*- */
// $Id$
#include "Configurator_Factory.h"

ACE_INLINE
CIAO::NodeApplication_Impl::
NodeApplication_Impl (CORBA::ORB_ptr o,
                      PortableServer::POA_ptr p,
                      NodeApp_Configurator &c,
                      const Static_Config_EntryPoints_Maps* static_entrypts_maps)
  : orb_ (CORBA::ORB::_duplicate (o)),
    poa_ (PortableServer::POA::_duplicate (p)),
    configurator_ (c),
    es_factory_ (o, p),
    static_entrypts_maps_ (static_entrypts_maps)
{
}

// Access the readonly attribute
ACE_INLINE
Deployment::Properties *
CIAO::NodeApplication_Impl::properties ()
{
  Deployment::Properties * tmp = 0;

  ACE_NEW_THROW_EX (tmp,
                    Deployment::Properties (this->properties_),
                    CORBA::NO_MEMORY ());

  return tmp;
}

ACE_INLINE
PortableServer::POA_ptr
CIAO::NodeApplication_Impl::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}


ACE_INLINE
CORBA::Object_ptr
CIAO::NodeApplication_Impl::get_node_application_manager ()
{
  return ::CORBA::Object::_duplicate (this->node_app_manager_.in ());
}

/*
ACE_INLINE void
CIAO::NodeApplication_Impl::
set_objref (Components::Deployment::ServerActivator_ptr act,
            const Components::ConfigValues &config,
            Components::Deployment::ComponentServer_ptr cs)
{
  if (!CORBA::is_nil (this->node_app_manager_.in ()) ||
      !CORBA::is_nil (this->objref_.in ()))
    throw CORBA::BAD_INV_ORDER ();

  this->config_ = new Components::ConfigValues (config);
  this->activator_ = Components::Deployment::ServerActivator::_duplicate (act);
  this->objref_ = Components::Deployment::ComponentServer::_duplicate (cs);
}
*/

ACE_INLINE ::Deployment::NodeApplication_ptr
CIAO::NodeApplication_Impl::
get_objref ()
{
  if (CORBA::is_nil (this->objref_.in ()))
    {
      this->objref_ = this->_this ();
    }

  return this->objref_.in ();
}

// $Id$



#include "Servant_Impl_Base.h"
#include "StandardConfigurator_Impl.h"
#include "Container_Base.h"

namespace CIAO
{
  Servant_Impl_Base::Servant_Impl_Base (Session_Container * c)
    : container_ (c)
  {
  }

  Servant_Impl_Base::~Servant_Impl_Base (void)
  {
  }

  void
  Servant_Impl_Base::add_facet (const char *port_name,
                                CORBA::Object_ptr port_ref)
  {
    ::Components::FacetDescription *fd = 0;
    ACE_NEW (fd,
             OBV_Components::FacetDescription);
    ::Components::FacetDescription_var safe = fd;

    fd->name (port_name);
    fd->type_id (port_ref->_interface_repository_id ());
    fd->facet_ref (port_ref);

    if (this->facet_table_.bind (port_name, fd) == 0)
      {
        safe._retn ();
      }
  }

  void
  Servant_Impl_Base::add_consumer (
      const char *port_name,
      ::Components::EventConsumerBase_ptr port_ref
    )
  {
    ::Components::ConsumerDescription *cd = 0;
    ACE_NEW (cd,
             OBV_Components::ConsumerDescription);
    ::Components::ConsumerDescription_var safe = cd;

    cd->name (port_name);
    cd->type_id (port_ref->_interface_repository_id ());
    cd->consumer (port_ref);

    if (this->consumer_table_.bind (port_name, cd) == 0)
      {
        safe._retn ();
      }
  }


  Components::StandardConfigurator*
  Servant_Impl_Base::get_standard_configurator (
    ACE_ENV_SINGLE_ARG_DECL)
  {
    //create the configurator servant
    StandardConfigurator_Impl *config_impl = 0;

    ACE_NEW_THROW_EX (config_impl,
                      StandardConfigurator_Impl (this),
                      CORBA::NO_MEMORY ());


    Components::StandardConfigurator_var configurator =
      config_impl->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (Components::StandardConfigurator::_nil ());

    return configurator._retn ();
  }


  PortableServer::POA_ptr
  Servant_Impl_Base::_default_POA (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  {

    return
      PortableServer::POA::_duplicate (container_->_ciao_the_POA ());
  }
}

// $Id$

#include "Servant_Impl_Base.h"
#include "StandardConfigurator_Impl.h"
#include "Container_Base.h"

namespace CIAO
{
  Servant_Impl_Base::Servant_Impl_Base (void)
  {
    ACE_ASSERT (0);
  }

  Servant_Impl_Base::Servant_Impl_Base (Session_Container * c)
    : container_ (c)
  {
  }

  Servant_Impl_Base::~Servant_Impl_Base (void)
  {
  }

  // Operations for CCMObject interface.

  ::Components::PrimaryKeyBase *
  Servant_Impl_Base::get_primary_key (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     ::Components::NoKeyAvailable))
  {
    ACE_THROW_RETURN (::Components::NoKeyAvailable (), 0);

    return 0;
  }

  CORBA::IRObject_ptr
  Servant_Impl_Base::get_component_def (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                      CORBA::IRObject::_nil ());
  }

  void
  Servant_Impl_Base::configuration_complete (
      ACE_ENV_SINGLE_ARG_DECL_NOT_USED
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::InvalidConfiguration))
  {
    // CIAO to-do
  }

  void
  Servant_Impl_Base::remove (
      ACE_ENV_SINGLE_ARG_DECL_NOT_USED
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::RemoveFailure))
  {
    // CIAO to-do
  }

  ::Components::ConnectionDescriptions *
  Servant_Impl_Base::get_connections (
      const char * /* name */
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::InvalidName))
  {
    ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
  }

  ::Components::ComponentPortDescription *
  Servant_Impl_Base::get_all_ports (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    OBV_Components::ComponentPortDescription *cps = 0;
    ACE_NEW_RETURN (cps,
                    OBV_Components::ComponentPortDescription,
                    0);
    ::Components::ComponentPortDescription_var retv = cps;

    ::Components::FacetDescriptions_var facets_desc =
      this->get_all_facets (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);

    ::Components::ReceptacleDescriptions_var receptacle_desc =
      this->get_all_receptacles (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);

    ::Components::ConsumerDescriptions_var consumer_desc =
      this->get_all_consumers (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);

    ::Components::EmitterDescriptions_var emitter_desc =
      this->get_all_emitters (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);

    ::Components::PublisherDescriptions_var publisher_desc =
      this->get_all_publishers (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);

    retv->facets (facets_desc.in ());
    retv->receptacles (receptacle_desc.in ());
    retv->consumers (consumer_desc.in ());
    retv->emitters (emitter_desc.in ());
    retv->publishers (publisher_desc.in ());

    return retv._retn ();
  }

  CORBA::Object_ptr
  Servant_Impl_Base::provide_facet (
      const char *name
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::InvalidName))
  {
    if (name == 0)
      {
        ACE_THROW_RETURN (Components::InvalidName (),
                          CORBA::Object::_nil ());
      }

    CORBA::Object_ptr retval = this->lookup_facet (name);

    if (CORBA::is_nil (retval))
      {
        ACE_THROW_RETURN (Components::InvalidName (),
                          CORBA::Object::_nil ());
      }

    return retval;
  }

  Components::FacetDescriptions *
  Servant_Impl_Base::get_named_facets (
      const ::Components::NameList & names
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::InvalidName))
  {
    Components::FacetDescriptions *retval = 0;
    ACE_NEW_RETURN (retval,
                    ::Components::FacetDescriptions,
                    0);
    Components::FacetDescriptions_var safe_retval = retval;
    CORBA::ULong len = names.length ();
    safe_retval->length (len);
    ::Components::FacetDescription *tmp = 0;

    for (CORBA::ULong i = 0; i < len; ++i)
      {
        tmp = this->lookup_facet_description (names[i].in ());

        if (tmp == 0)
          {
            ACE_THROW_RETURN (Components::InvalidName (),
                              0);
          }

        safe_retval[i] = tmp;
      }

    return safe_retval._retn ();
  }

  ::Components::FacetDescriptions *
  Servant_Impl_Base::get_all_facets (
      ACE_ENV_SINGLE_ARG_DECL_NOT_USED
    )
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    ::Components::FacetDescriptions *tmp = 0;
    ACE_NEW_RETURN (tmp,
                    ::Components::FacetDescriptions,
                    0);

    ::Components::FacetDescriptions_var retval = tmp;

    retval->length (this->facet_table_.current_size ());
    CORBA::ULong i = 0;

    for (FacetTable::iterator iter = this->facet_table_.begin ();
         iter != this->facet_table_.end ();
         ++iter, ++i)
      {
        FacetTable::ENTRY & entry = *iter;
        retval[i] = entry.int_id_;
      }

    return retval._retn ();
  }

  ::Components::ConsumerDescriptions *
  Servant_Impl_Base::get_all_consumers (
      ACE_ENV_SINGLE_ARG_DECL_NOT_USED
    )
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    ::Components::ConsumerDescriptions *tmp = 0;
    ACE_NEW_RETURN (tmp,
                    ::Components::ConsumerDescriptions,
                    0);

    ::Components::ConsumerDescriptions_var retval = tmp;

    retval->length (this->consumer_table_.current_size ());
    CORBA::ULong i = 0;

    for (ConsumerTable::iterator iter = this->consumer_table_.begin ();
         iter != this->consumer_table_.end ();
         ++iter, ++i)
      {
        ConsumerTable::ENTRY & entry = *iter;
        retval[i] = entry.int_id_;
      }

    return retval._retn ();
  }

  ::Components::EventConsumerBase_ptr
  Servant_Impl_Base::get_consumer (
      const char *sink_name
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::InvalidName))
  {
    if (sink_name == 0)
      {
        ACE_THROW_RETURN (Components::InvalidName (),
                          Components::EventConsumerBase::_nil ());
      }

    Components::EventConsumerBase_ptr retval =
      this->lookup_consumer (sink_name);

    if (CORBA::is_nil (retval))
      {
        ACE_THROW_RETURN (Components::InvalidName (),
                          Components::EventConsumerBase::_nil ());
      }

    return retval;
  }

  ::Components::EventConsumerBase_ptr
  Servant_Impl_Base::disconnect_consumer (
      const char * /* source_name */
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::InvalidName,
                     Components::NoConnection))
  {
    ACE_THROW_RETURN (::CORBA::NO_IMPLEMENT (), 0);
  }

  ::Components::ConsumerDescriptions *
  Servant_Impl_Base::get_named_consumers (
      const ::Components::NameList & names
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::InvalidName))
  {
    Components::ConsumerDescriptions *retval = 0;
    ACE_NEW_RETURN (retval,
                    ::Components::ConsumerDescriptions,
                    0);
    Components::ConsumerDescriptions_var safe_retval = retval;
    CORBA::ULong len = names.length ();
    safe_retval->length (len);
    ::Components::ConsumerDescription *tmp = 0;

    for (CORBA::ULong i = 0; i < len; ++i)
      {
        tmp = this->lookup_consumer_description (names[i].in ());

        if (tmp == 0)
          {
            ACE_THROW_RETURN (Components::InvalidName (),
                              0);
          }

        safe_retval[i] = tmp;
      }

    return safe_retval._retn ();
  }

  ::Components::EmitterDescriptions *
  Servant_Impl_Base::get_all_emitters (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
  }

  ::Components::EmitterDescriptions *
  Servant_Impl_Base::get_named_emitters (
      const ::Components::NameList & /* names */
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::InvalidName))
  {
    ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
  }

  ::Components::ReceptacleDescriptions *
  Servant_Impl_Base::get_all_receptacles (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    ACE_THROW_RETURN (::CORBA::NO_IMPLEMENT (), 0);
  }

  ::Components::ReceptacleDescriptions *
  Servant_Impl_Base::get_named_receptacles (
      const ::Components::NameList & /* names */
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::InvalidName))
  {
    ACE_THROW_RETURN (::CORBA::NO_IMPLEMENT (), 0);
  }

  ::Components::PublisherDescriptions *
  Servant_Impl_Base::get_all_publishers (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    ACE_THROW_RETURN (::CORBA::NO_IMPLEMENT (), 0);
  }

  ::Components::PublisherDescriptions *
  Servant_Impl_Base::get_named_publishers (
      const ::Components::NameList & /* names */
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::InvalidName))
  {
    ACE_THROW_RETURN (::CORBA::NO_IMPLEMENT (), 0);
  }

  /// Protected operations.

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

  CORBA::Object_ptr
  Servant_Impl_Base::lookup_facet (const char *port_name)
  {
    ::Components::FacetDescription_var fd;

    if (this->facet_table_.find (port_name, fd) != 0)
      {
        return CORBA::Object::_nil ();
      }

    return CORBA::Object::_duplicate (fd.in ()->facet_ref ());
  }

  ::Components::FacetDescription *
  Servant_Impl_Base::lookup_facet_description (const char *port_name)
  {
    ::Components::FacetDescription_var fd;

    if (this->facet_table_.find (port_name, fd) != 0)
      {
        return 0;
      }

    return fd._retn ();
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

  ::Components::EventConsumerBase_ptr
  Servant_Impl_Base::lookup_consumer (const char *port_name)
  {
    ::Components::ConsumerDescription_var cd;

    if (this->consumer_table_.find (port_name, cd) != 0)
      {
        return ::Components::EventConsumerBase::_nil ();
      }

    return
      ::Components::EventConsumerBase::_duplicate (cd.in ()->consumer ());
  }

  ::Components::ConsumerDescription *
  Servant_Impl_Base::lookup_consumer_description (
      const char *port_name
    )
  {
    ::Components::ConsumerDescription_var cd;

    if (this->consumer_table_.find (port_name, cd) != 0)
      {
        return 0;
      }

    return cd._retn ();
  }

  Components::StandardConfigurator*
  Servant_Impl_Base::get_standard_configurator (
    ACE_ENV_SINGLE_ARG_DECL)
  {
    // Create the configurator servant.
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
      PortableServer::POA::_duplicate (container_->the_POA ());
  }
}

// $Id$

#include "Connector_Servant_Impl_Base.h"

#include "StandardConfigurator_Impl.h"
#include "ciao/Logger/Log_Macros.h"
#include "ciao/Containers/Container_BaseC.h"

namespace CIAO
{
  Connector_Servant_Impl_Base::Connector_Servant_Impl_Base (
        Components::CCMHome_ptr home,
        Home_Servant_Impl_Base *home_servant,
        Container_ptr c)
    : container_ (Container::_duplicate (c)),
      home_ (Components::CCMHome::_duplicate (home)),
      home_servant_ (home_servant)
  {
    if (home_servant_ != 0)
      {
        home_servant_->_add_ref ();
      }
  }

  Connector_Servant_Impl_Base::~Connector_Servant_Impl_Base (void)
  {
  }

  // Operations for CCMObject interface.

  void
  Connector_Servant_Impl_Base::remove (void)
  {
    CIAO_TRACE("Connector_Servant_Impl_Base::remove (void)");

    try
    {
      // Removing Facets
      for (FacetTable::const_iterator iter =
             this->facet_table_.begin ();
           iter != this->facet_table_.end ();
           ++iter)
        {
          PortableServer::ObjectId_var facet_id =
            this->container_->the_port_POA ()->reference_to_id (
              iter->second);

          this->container_->the_port_POA ()->deactivate_object (
            facet_id);

          CIAO::Servant_Activator_var sa =
            this->container_->ports_servant_activator ();

          if (!CORBA::is_nil (sa.in ()))
            {
              sa->update_port_activator (facet_id.in ());
            }
        }

      Components::SessionComponent_var temp = this->get_executor ();

      if (!::CORBA::is_nil (temp.in ()))
        {
          temp->ccm_remove ();
        }

      PortableServer::ObjectId_var oid;
      this->container_->uninstall_servant (
        this,
        Container_Types::COMPONENT_t,
        oid.out ());

      if (this->home_servant_)
        {
          this->home_servant_->update_component_map (oid);
          this->home_servant_->_remove_ref ();
          this->home_servant_ = 0;
        }
    }
    catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Port not active\n");
    }
  }

#if !defined (CCM_LW)
  CORBA::IRObject_ptr
  Connector_Servant_Impl_Base::get_component_def (void)
  {
    CIAO_TRACE("Connector_Servant_Impl_Base::get_component_def");
    throw ::CORBA::NO_IMPLEMENT ();
  }
#endif

#if !defined (CCM_LW)
  ::Components::ConnectionDescriptions *
  Connector_Servant_Impl_Base::get_connections (
    const char * /* name */)
  {
    CIAO_TRACE("Connector_Servant_Impl_Base::get_connections (const char * /* name */)");
    throw CORBA::NO_IMPLEMENT ();
  }
#endif

  ::CORBA::Object_ptr
  Connector_Servant_Impl_Base::provide_facet (const char *name)
  {
    CIAO_TRACE("Connector_Servant_Impl_Base::provide_facet (const char *name)");

    if (0 == name)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO "Connector_Servant_Impl_Base::provide_facet - Got nil name"));

        throw ::Components::InvalidName ();
      }

    CORBA::Object_ptr retval = this->lookup_facet (name);

    if ( ::CORBA::is_nil (retval))
      {
        throw ::Components::InvalidName ();
      }

    return retval;
  }

 #if !defined (CCM_LW)
  Components::FacetDescriptions *
  Connector_Servant_Impl_Base::get_named_facets (
    const ::Components::NameList & names)
  {
    CIAO_TRACE("Connector_Servant_Impl_Base::get_named_facets");

    Components::FacetDescriptions *retval = 0;
    ACE_NEW_THROW_EX (retval,
                      ::Components::FacetDescriptions,
                      CORBA::NO_MEMORY ());

    Components::FacetDescriptions_var safe_retval = retval;
    CORBA::ULong const len = names.length ();
    safe_retval->length (len);

    for (CORBA::ULong i = 0; i < len; ++i)
      {
        ::Components::FacetDescription *tmp =
          this->lookup_facet_description (names[i]);

        if (tmp == 0)
          {
            throw Components::InvalidName ();
          }

        safe_retval[i] = tmp;
      }

    return safe_retval._retn ();
  }
#endif

 #if !defined (CCM_LW)
  ::Components::FacetDescriptions *
  Connector_Servant_Impl_Base::get_all_facets (void)
  {
    CIAO_TRACE ("Connector_Servant_Impl_Base::get_all_facets (void)");

    ::Components::FacetDescriptions_var retval = 0;
    ACE_NEW_THROW_EX (retval,
                      ::Components::FacetDescriptions,
                      CORBA::NO_MEMORY ());
    retval->length (this->facet_table_.size ());
    CORBA::ULong i = 0UL;

    for (FacetTable::const_iterator iter =
           this->facet_table_.begin ();
         iter != this->facet_table_.end ();
         ++iter, ++i)
      {
        ::Components::FacetDescription *fd = 0;
        ACE_NEW_THROW_EX (fd,
                          ::OBV_Components::FacetDescription (iter->first.c_str (),
                                                              iter->second->_interface_repository_id (),
                                                              iter->second),
                          CORBA::NO_MEMORY ());
        retval[i] = fd;
      }

    return retval._retn ();
  }
#endif

#if !defined (CCM_LW)
  ::Components::PrimaryKeyBase *
  Connector_Servant_Impl_Base::get_primary_key (void)
  {
    return 0;
  }
#endif

#if !defined (CCM_LW)
  ::Components::ComponentPortDescription *
  Connector_Servant_Impl_Base::get_all_ports (void)
  {
    CIAO_TRACE("Connector_Servant_Impl_Base::get_all_ports (void)");

    OBV_Components::ComponentPortDescription *cps = 0;
    ACE_NEW_THROW_EX (cps,
                      OBV_Components::ComponentPortDescription,
                      CORBA::NO_MEMORY ());
    ::Components::ComponentPortDescription_var retv = cps;

    ::Components::FacetDescriptions_var facets_desc =
      this->get_all_facets ();

    ::Components::ReceptacleDescriptions_var receptacle_desc =
      this->get_all_receptacles ();

    retv->facets (facets_desc.in ());
    retv->receptacles (receptacle_desc.in ());

    return retv._retn ();
  }
#endif

#if !defined (CCM_LW)
  ::Components::ConsumerDescriptions *
  Connector_Servant_Impl_Base::get_all_consumers (void)
  {
    return 0;
  }
#endif

  ::Components::EventConsumerBase_ptr
  Connector_Servant_Impl_Base::get_consumer (
    const char * /* name */)
  {
    return ::Components::EventConsumerBase::_nil ();
  }

#if !defined (CCM_LW)
  ::Components::ConsumerDescriptions *
  Connector_Servant_Impl_Base::get_named_consumers (
    const ::Components::NameList & /* names */)
  {
    return 0;
  }
#endif

#if !defined (CCM_LW)
  ::Components::EmitterDescriptions *
  Connector_Servant_Impl_Base::get_all_emitters (void)
  {
    return 0;
  }
#endif

#if !defined (CCM_LW)
  ::Components::EmitterDescriptions *
  Connector_Servant_Impl_Base::get_named_emitters (
      const ::Components::NameList & /* names */)
  {
    return 0;
  }
#endif

#if !defined (CCM_LW)
  ::Components::ReceptacleDescriptions *
  Connector_Servant_Impl_Base::get_named_receptacles (
    const ::Components::NameList & /* names */)
  {
    CIAO_TRACE("Connector_Servant_Impl_Base::get_named_receptacles");
    throw ::CORBA::NO_IMPLEMENT ();
  }
#endif

#if !defined (CCM_LW)
  ::Components::PublisherDescriptions *
  Connector_Servant_Impl_Base::get_all_publishers (void)
  {
    return 0;
  }
#endif

#if !defined (CCM_LW)
  ::Components::PublisherDescriptions *
  Connector_Servant_Impl_Base::get_named_publishers (
    const ::Components::NameList & /* names */)
  {
    return 0;
  }
#endif

  ::Components::Cookie *
  Connector_Servant_Impl_Base::subscribe (
    const char * /* publisher_name */,
    ::Components::EventConsumerBase_ptr /* subscriber */)
  {
    throw ::Components::InvalidName ();
    return 0;
  }

  ::Components::EventConsumerBase_ptr
  Connector_Servant_Impl_Base::unsubscribe (
    const char * /* publisher_name */,
    ::Components::Cookie * /* ck */)
  {
    throw ::Components::InvalidName ();
    return ::Components::EventConsumerBase::_nil ();
  }

  void
  Connector_Servant_Impl_Base::connect_consumer (
    const char * emitter_name,
    ::Components::EventConsumerBase_ptr /* consumer */)
  {
    if (emitter_name == 0)
      {
        throw ::CORBA::BAD_PARAM ();
      }

    throw ::Components::InvalidName ();
  }

  ::Components::EventConsumerBase_ptr
  Connector_Servant_Impl_Base::disconnect_consumer (
    const char * source_name)
  {
    if (source_name == 0)
      {
        throw ::CORBA::BAD_PARAM ();
      }

    throw ::Components::InvalidName ();
  }

  ::Components::Cookie *
  Connector_Servant_Impl_Base::connect (
    const char * name,
    ::CORBA::Object_ptr /* connection */)
  {
    if (name == 0)
      {
        throw ::CORBA::BAD_PARAM ();
      }

    throw ::Components::InvalidName ();
  }

  ::CORBA::Object_ptr
  Connector_Servant_Impl_Base::disconnect (
    const char * name,
    ::Components::Cookie * /* ck */)
  {
    if (name == 0)
      {
        throw ::CORBA::BAD_PARAM ();
      }

    throw ::Components::InvalidName ();
  }

  /// CIAO-specific public methods.

  void
  Connector_Servant_Impl_Base::set_attributes (
    const Components::ConfigValues & /* descr */)
  {
  }

#if !defined (CCM_LW)
  ::Components::StandardConfigurator_ptr
  Connector_Servant_Impl_Base::get_standard_configurator (void)
  {
    CIAO_TRACE("Connector_Servant_Impl_Base::get_standard_configurator (void)");

    // Create the configurator servant.
    StandardConfigurator_Impl *config_impl = 0;

    ACE_NEW_THROW_EX (config_impl,
                      StandardConfigurator_Impl (this),
                      CORBA::NO_MEMORY ());


    Components::StandardConfigurator_var configurator =
      config_impl->_this ();

    return configurator._retn ();
  }
#endif

  PortableServer::POA_ptr
  Connector_Servant_Impl_Base::_default_POA (void)
  {
    CIAO_TRACE("Connector_Servant_Impl_Base::_default_POA (void)");
    return PortableServer::POA::_duplicate (container_->the_POA ());
  }

  ::CORBA::Object_ptr
  Connector_Servant_Impl_Base::get_facet_executor (
    const char * /* name */)
  {
    throw ::Components::InvalidName ();
  }

  /// Protected operations.
  void
  Connector_Servant_Impl_Base::add_facet (
    const char *port_name,
    ::CORBA::Object_ptr port_ref)
  {
    CIAO_TRACE("Connector_Servant_Impl_Base::add_facet");

    if (0 == port_name || ::CORBA::is_nil (port_ref))
      {
        throw ::CORBA::BAD_PARAM ();
      }

    FacetTable::value_type entry;
    entry.first = port_name;
    entry.second = ::CORBA::Object::_duplicate (port_ref);

    {
      ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, mon, this->lock_,
                          CORBA::NO_RESOURCES ());

      (void) this->facet_table_.insert (entry);
    }
  }

  CORBA::Object_ptr
  Connector_Servant_Impl_Base::lookup_facet (const char *port_name)
  {
    CIAO_TRACE("Connector_Servant_Impl_Base::lookup_facet");
    if (!port_name)
      {
        return CORBA::Object::_nil ();
      }

    ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX,
                        mon,
                        this->lock_,
                        CORBA::NO_RESOURCES ());

    FacetTable::const_iterator iter = this->facet_table_.find (port_name);

    if (iter == this->facet_table_.end ())
      {
        return CORBA::Object::_nil ();
      }

    return CORBA::Object::_duplicate (iter->second);
  }

#if !defined (CCM_LW)
  ::Components::FacetDescription *
  Connector_Servant_Impl_Base::lookup_facet_description (const char *port_name)
  {
    CIAO_TRACE ("Connector_Servant_Impl_Base::lookup_facet_description");

    if (!port_name)
      {
        // Calling function will throw InvalidName after getting this.
        return 0;
      }

    ::Components::FacetDescription_var safe;

    {
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                        mon,
                        this->lock_,
                        0);
      FacetTable::const_iterator iter =
        this->facet_table_.find (port_name);

      if (iter != this->facet_table_.end ())
        {
          ::Components::FacetDescription *fd = 0;
          ACE_NEW_THROW_EX (fd,
                            ::OBV_Components::FacetDescription (iter->first.c_str (),
                                                                iter->second->_interface_repository_id (),
                                                                iter->second),
                            CORBA::NO_MEMORY ());
          safe = fd;
        }
    }

    return safe._retn ();
  }
#endif
}


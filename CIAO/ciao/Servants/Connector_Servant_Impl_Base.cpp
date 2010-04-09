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
 #if !defined (CCM_LW)
    // @todo, reimplement for LwCCM
    CIAO_TRACE("Connector_Servant_Impl_Base::remove (void)");

    try
    {
      // Removing Facets
      Components::FacetDescriptions_var facets =
        this->get_all_facets ();

      CORBA::ULong const facet_len = facets->length ();

      for (CORBA::ULong i = 0; i < facet_len; ++i)
        {
          PortableServer::ObjectId_var facet_id =
            this->container_->the_port_POA ()->reference_to_id (
              facets[i]->facet_ref ());

          this->container_->the_port_POA ()->deactivate_object (
            facet_id);

          CIAO::Servant_Activator_var sa =
            this->container_->ports_servant_activator ();

          if (!CORBA::is_nil (sa.in ()))
            {
              sa->update_port_activator (facet_id.in ());
            }
        }

      Components::SessionComponent_var temp =
        this->get_executor ();

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
#endif
  }

  CORBA::IRObject_ptr
  Connector_Servant_Impl_Base::get_component_def (void)
  {
    CIAO_TRACE("Connector_Servant_Impl_Base::get_component_def");
    throw ::CORBA::NO_IMPLEMENT ();
  }

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

    ::Components::FacetDescriptions *tmp = 0;
    ACE_NEW_THROW_EX (tmp,
                      ::Components::FacetDescriptions,
                      CORBA::NO_MEMORY ());

    ::Components::FacetDescriptions_var retval = tmp;

    retval->length (this->facet_table_.size ());
    CORBA::ULong i = 0UL;

    for (FacetTable::const_iterator iter =
           this->facet_table_.begin ();
         iter != this->facet_table_.end ();
         ++iter, ++i)
      {
        retval[i] = iter->second;
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
  Connector_Servant_Impl_Base::get_named_emitters (
      const ::Components::NameList & /* names */)
  {
    return 0;
  }

  ::Components::EmitterDescriptions *
  Connector_Servant_Impl_Base::get_all_emitters (void)
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
  ::Components::ReceptacleDescriptions *
  Connector_Servant_Impl_Base::get_all_receptacles (void)
  {
    CIAO_TRACE("  Connector_Servant_Impl_Base::get_all_receptacles (void)");
    CIAO_DEBUG (9,
                (LM_TRACE,
                 CLINFO
                 "Connector_Servant_Impl_Base::get_all_receptacles\n"));

    ::Components::ReceptacleDescriptions *tmp = 0;
    ACE_NEW_THROW_EX (tmp,
                      ::Components::ReceptacleDescriptions,
                      CORBA::NO_MEMORY ());

    ::Components::ReceptacleDescriptions_var retval = tmp;

    retval->length (this->receptacle_table_.current_size ());
    CORBA::ULong i = 0;

    CIAO_DEBUG (6,
                (LM_DEBUG,
                 CLINFO
                 "Connector_Servant_Impl_Base::get_all_receptacles - Building sequence of length %d\n",
                 retval->length ()));

    for (ReceptacleTable::iterator iter =
           this->receptacle_table_.begin ();
         iter != this->receptacle_table_.end ();
         ++iter, ++i)
      {
        CIAO_DEBUG (9,
                    (LM_TRACE,
                     CLINFO
                     "Connector_Servant_Impl_Base::get_all_receptacles - Starting loop iteration...\n",
                     retval->length ()));

        ReceptacleTable::ENTRY & entry = *iter;
        retval[i] = entry.int_id_;
      }

    CIAO_DEBUG (9,
                (LM_TRACE,
                 CLINFO
                 "Connector_Servant_Impl_Base::get_all_receptacles - Escaped loop.\n"));

    return retval._retn ();
  }
#endif

#if !defined (CCM_LW)
  ::Components::PublisherDescriptions *
  Connector_Servant_Impl_Base::get_named_publishers (
    const ::Components::NameList & /* names */)
  {
    return 0;
  }

  ::Components::PublisherDescriptions *
  Connector_Servant_Impl_Base::get_all_publishers (void)
  {
    return 0;
  }
#endif

  ::Components::Cookie *
  Connector_Servant_Impl_Base::subscribe (
    const char * /* publisher_name */,
    ::Components::EventConsumerBase_ptr /* subscriber */)
  {
    return 0;
  }

  ::Components::EventConsumerBase_ptr
  Connector_Servant_Impl_Base::unsubscribe (
    const char * /* publisher_name */,
    ::Components::Cookie * /* ck */)
  {
    return ::Components::EventConsumerBase::_nil ();
  }

  void
  Connector_Servant_Impl_Base::connect_consumer (
    const char * /* emitter_name */,
    ::Components::EventConsumerBase_ptr /* consumer */)
  {
  }

  ::Components::EventConsumerBase_ptr
  Connector_Servant_Impl_Base::disconnect_consumer (
    const char * /* source_name */)
  {
    return ::Components::EventConsumerBase::_nil ();
  }

  ::Components::Cookie *
  Connector_Servant_Impl_Base::connect (
    const char * /* name */,
    ::CORBA::Object_ptr /* connection */)
  {
    return 0;
  }

  ::CORBA::Object_ptr
  Connector_Servant_Impl_Base::disconnect (
    const char * /* name */,
    ::Components::Cookie * /* ck */)
  {
    return ::CORBA::Object::_nil ();
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
 #if !defined (CCM_LW)
    // @todo reimplement for LwCCM
    CIAO_TRACE("Connector_Servant_Impl_Base::add_facet");

    if (0 == port_name || ::CORBA::is_nil (port_ref))
      {
        throw ::CORBA::BAD_PARAM ();
      }

    ::Components::FacetDescription *fd = 0;
    ACE_NEW_THROW_EX (fd,
                      ::OBV_Components::FacetDescription (port_name,
                                                          port_ref->_interface_repository_id (),
                                                          port_ref),
                      CORBA::NO_MEMORY ());
    ::Components::FacetDescription_var safe = fd;

    FacetTable::value_type entry;
    entry.first = port_name;
    entry.second = safe._retn ();

    {
      ACE_WRITE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, mon, this->lock_,
                                CORBA::NO_RESOURCES ());

      (void) this->facet_table_.insert (entry);
    }
#endif
  }

  CORBA::Object_ptr
  Connector_Servant_Impl_Base::lookup_facet (const char *port_name)
  {
    CIAO_TRACE("Connector_Servant_Impl_Base::lookup_facet");
 #if !defined (CCM_LW)
    // @todo Reimplement for LwCCM
    if (!port_name)
      {
        return CORBA::Object::_nil ();
      }

    ACE_READ_GUARD_THROW_EX (TAO_SYNCH_MUTEX,
                             mon,
                             this->lock_,
                             CORBA::NO_RESOURCES ());

    FacetTable::const_iterator iter = this->facet_table_.find (port_name);

    if (iter == this->facet_table_.end ())
      {
        return CORBA::Object::_nil ();
      }

    return CORBA::Object::_duplicate (iter->second->facet_ref ());
#else
    return CORBA::Object::_nil ();
#endif
  }

#if !defined (CCM_LW)
  ::Components::FacetDescription *
  Connector_Servant_Impl_Base::lookup_facet_description (const char *port_name)
  {
    CIAO_TRACE ("Connector_Servant_Impl_Base::lookup_facet_description");

    if (!port_name)
      {
        /// Calling function will throw InvalidName after getting this.
        return 0;
      }

    ::Components::FacetDescription_var fd;

    {
      ACE_READ_GUARD_RETURN (TAO_SYNCH_MUTEX,
                             mon,
                             this->lock_,
                             0);
      FacetTable::const_iterator iter =
        this->facet_table_.find (port_name);

      if (iter != this->facet_table_.end ())
        {
          fd = iter->second;
        }
    }

    return fd._retn ();
  }
#endif

  void
  Connector_Servant_Impl_Base::add_receptacle (
    const char *receptacle_name,
    CORBA::Object_ptr recept_ref,
    ::Components::Cookie * cookie)
  {
#if !defined (CCM_LW)
    // @todo Reimplement for LwCCM
    CIAO_TRACE("Connector_Servant_Impl_Base::add_receptacle");
    CIAO_DEBUG (6,
                (LM_INFO,
                 CLINFO
                 "Connector_Servant_Impl_Base::add_receptacle - attempting to add new connection to receptacle (%C)\n",
                 receptacle_name));

    ::Components::ReceptacleDescription_var safe;
    ::Components::ReceptacleDescription *rd = 0;

    if (this->receptacle_table_.find (receptacle_name, safe) == -1)
      {
        CIAO_DEBUG (6,
                    (LM_DEBUG,
                     CLINFO
                     "Connector_Servant_Impl_Base::add_receptacle - Found no receptacle named (%C), creating it...\n",
                     receptacle_name));

        ACE_NEW_THROW_EX (rd,
                          OBV_Components::ReceptacleDescription,
                          CORBA::NO_MEMORY ());
        safe = rd;

        rd->name (receptacle_name);
        rd->type_id ();
        // The receptacle is a multiplex receptacle if and only if a
        // cookie was given.
        rd->is_multiple (cookie != 0);

        ::Components::ConnectionDescription *cd = 0;
        ACE_NEW_THROW_EX (cd,
                          OBV_Components::ConnectionDescription (cookie,
                                                                 recept_ref),
                          CORBA::NO_MEMORY ());

        ::Components::ConnectionDescription_var cd_safe = cd;
        ::Components::ConnectionDescriptions cds (1);

        cds.length (1);
        cds[0] = cd_safe;
        rd->connections (cds);
      }
    else
      {
        CIAO_DEBUG (6,
                    (LM_DEBUG,
                     CLINFO
                     "Connector_Servant_Impl_Base::add_receptacle - Found a receptacle named (%C)\n",
                     receptacle_name));

        rd = safe.inout ();

        ::Components::ConnectionDescription *cd = 0;
        ACE_NEW_THROW_EX (cd,
                          OBV_Components::ConnectionDescription (cookie,
                                                                 recept_ref),
                          CORBA::NO_MEMORY ());

        ::Components::ConnectionDescription_var cd_safe = cd;
        ::Components::ConnectionDescriptions & cds =
          rd->connections ();
        CORBA::ULong old_length = cds.length ();
        cds.length (old_length + 1);
        cds [old_length] = cd_safe;

        CIAO_DEBUG (6,
                    (LM_DEBUG,
                     CLINFO
                     "Connector_Servant_Impl_Base::add_receptacle - Added new connection to "
                     "existing receptacle named (%C)\n",
                     receptacle_name));
      }

    if (this->receptacle_table_.bind (receptacle_name, safe) == 0)
      {
        CIAO_DEBUG (6,
                    (LM_INFO,
                     CLINFO
                     "Connector_Servant_Impl_Base::add_receptacle - Successfully added new receptacle named (%C)\n",
                     receptacle_name));
      }
#endif      
  }
}


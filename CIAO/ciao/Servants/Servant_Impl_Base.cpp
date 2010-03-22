// $Id$

#include "Servant_Impl_Base.h"

#include "StandardConfigurator_Impl.h"
#include "ciao/Logger/Log_Macros.h"
#include "ciao/Containers/Container_BaseC.h"

namespace CIAO
{
  Servant_Impl_Base::Servant_Impl_Base (Components::CCMHome_ptr home,
                                        Home_Servant_Impl_Base *home_servant,
                                        Container_ptr c)
    : home_ (Components::CCMHome::_duplicate (home)),
      home_servant_ (home_servant),
      container_ (Container::_duplicate (c))
  {
    if (home_servant_)
      {
        home_servant_->_add_ref ();
      }
  }

  Servant_Impl_Base::~Servant_Impl_Base (void)
  {
  }

  // Operations for CCMObject interface.

  ::Components::PrimaryKeyBase *
  Servant_Impl_Base::get_primary_key (void)
  {
    CIAO_TRACE("Servant_Impl_Base::get_primary_key");
    throw ::Components::NoKeyAvailable ();
  }

  CORBA::IRObject_ptr
  Servant_Impl_Base::get_component_def (void)
  {
    CIAO_TRACE("Servant_Impl_Base::get_component_def");
    throw ::CORBA::NO_IMPLEMENT ();
  }

  void
  Servant_Impl_Base::remove (void)
  {
    CIAO_TRACE("Servant_Impl_Base::remove (void)");
    try
    {
      // Removing Facets
      Components::FacetDescriptions_var facets = this->get_all_facets ();

      CORBA::ULong const facet_len = facets->length ();

      for (CORBA::ULong i = 0; i < facet_len; ++i)
      {
        PortableServer::ObjectId_var facet_id =
          this->container_->the_port_POA ()->reference_to_id (
            facets[i]->facet_ref ());

        this->container_->the_port_POA ()->deactivate_object (facet_id);

        CIAO::Servant_Activator_var sa =
          this->container_->ports_servant_activator ();

        if (!CORBA::is_nil (sa.in ()))
          {
            sa->update_port_activator (facet_id.in ());
          }
      }

      // Removed Facets

      // Removing Consumers
      Components::ConsumerDescriptions_var consumers =
        this->get_all_consumers ();

      CORBA::ULong const consumer_len = consumers->length ();

      for (CORBA::ULong j = 0; j < consumer_len; ++j)
      {
        PortableServer::ObjectId_var cons_id =
          this->container_->the_port_POA ()->reference_to_id (
            consumers[j]->consumer ());

        this->container_->the_port_POA ()->deactivate_object (
          cons_id);

        CIAO::Servant_Activator_var sa =
          this->container_->ports_servant_activator ();

        if (!CORBA::is_nil (sa.in ()))
          {
            sa->update_port_activator (cons_id.in ());
          }
      }

      Components::SessionComponent_var temp = this->get_executor ();
      temp->ccm_remove ();

      CORBA::Object_var objref = this->container_->get_objref (this);

      Components::CCMObject_var ccmobjref =
        Components::CCMObject::_narrow (objref.in ());

      PortableServer::ObjectId_var oid;

      this->container_->uninstall_servant (this,
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

  ::Components::ConnectionDescriptions *
  Servant_Impl_Base::get_connections (const char * /* name */)
  {
    CIAO_TRACE("Servant_Impl_Base::get_connections (const char * /* name */)");
    throw CORBA::NO_IMPLEMENT ();
  }

  ::Components::ComponentPortDescription *
  Servant_Impl_Base::get_all_ports (void)
  {
    CIAO_TRACE("Servant_Impl_Base::get_all_ports (void)");
    OBV_Components::ComponentPortDescription *cps = 0;
    ACE_NEW_THROW_EX (cps,
                      OBV_Components::ComponentPortDescription,
                      CORBA::NO_MEMORY ());
    ::Components::ComponentPortDescription_var retv = cps;

    ::Components::FacetDescriptions_var facets_desc =
      this->get_all_facets ();

    ::Components::ReceptacleDescriptions_var receptacle_desc =
      this->get_all_receptacles ();

    ::Components::ConsumerDescriptions_var consumer_desc =
      this->get_all_consumers ();

    ::Components::EmitterDescriptions_var emitter_desc =
      this->get_all_emitters ();

    ::Components::PublisherDescriptions_var publisher_desc =
      this->get_all_publishers ();

    retv->facets (facets_desc.in ());
    retv->receptacles (receptacle_desc.in ());
    retv->consumers (consumer_desc.in ());
    retv->emitters (emitter_desc.in ());
    retv->publishers (publisher_desc.in ());

    return retv._retn ();
  }

  ::CORBA::Object_ptr
  Servant_Impl_Base::provide_facet (const char *name)
  {
    CIAO_TRACE("Servant_Impl_Base::provide_facet (const char *name)");
    if (0 == name)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO "Servant_Impl_Base::provide_facet - Got nil name"));

        throw Components::InvalidName ();
      }

    CORBA::Object_ptr retval = this->lookup_facet (name);

    if ( ::CORBA::is_nil (retval))
      {
        throw Components::InvalidName ();
      }

    return retval;
  }

  Components::FacetDescriptions *
  Servant_Impl_Base::get_named_facets (
    const ::Components::NameList & names)
  {
    CIAO_TRACE("Servant_Impl_Base::get_named_facets");
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

        if (!tmp)
          {
            throw Components::InvalidName ();
          }

        safe_retval[i] = tmp;
      }

    return safe_retval._retn ();
  }

  ::Components::FacetDescriptions *
  Servant_Impl_Base::get_all_facets (void)
  {
    CIAO_TRACE ("Servant_Impl_Base::get_all_facets (void)");
    ::Components::FacetDescriptions *tmp = 0;
    ACE_NEW_THROW_EX (tmp,
                      ::Components::FacetDescriptions,
                      CORBA::NO_MEMORY ());

    ::Components::FacetDescriptions_var retval = tmp;

    retval->length (this->facet_table_.size ());
    CORBA::ULong i = 0UL;

    for (FacetTable::const_iterator iter = this->facet_table_.begin ();
         iter != this->facet_table_.end ();
         ++iter, ++i)
      {
        retval[i] = iter->second;
      }

    return retval._retn ();
  }


  ::Components::ConsumerDescriptions *
  Servant_Impl_Base::get_all_consumers (void)
  {
    CIAO_TRACE("Servant_Impl_Base::get_all_consumers (void)");
    ::Components::ConsumerDescriptions *tmp = 0;
    ACE_NEW_THROW_EX (tmp,
                      ::Components::ConsumerDescriptions (
                      this->consumer_table_.size ()),
                      CORBA::NO_MEMORY ());

    ::Components::ConsumerDescriptions_var retval = tmp;

    retval->length (this->consumer_table_.size ());
    CORBA::ULong i = 0UL;

    for (ConsumerTable::const_iterator iter = this->consumer_table_.begin ();
         iter != this->consumer_table_.end ();
         ++iter, ++i)
      {
        retval[i] = iter->second;
      }

    return retval._retn ();
  }


  ::Components::EventConsumerBase_ptr
  Servant_Impl_Base::get_consumer (const char *sink_name)
  {
    CIAO_TRACE("Servant_Impl_Base::get_consumer (const char *sink_name)");
    if (!sink_name)
      {
        throw Components::InvalidName ();
      }

    Components::EventConsumerBase_ptr retval =
      this->lookup_consumer (sink_name);

    if (CORBA::is_nil (retval))
      {
        throw Components::InvalidName ();
      }

    return retval;
  }

  ::Components::ConsumerDescriptions *
  Servant_Impl_Base::get_named_consumers (
    const ::Components::NameList & names)
  {
    CIAO_TRACE("Servant_Impl_Base::get_named_consumers");
    Components::ConsumerDescriptions *retval = 0;
    ACE_NEW_THROW_EX (retval,
                    ::Components::ConsumerDescriptions,
                      CORBA::NO_MEMORY ());
    Components::ConsumerDescriptions_var safe_retval = retval;
    CORBA::ULong const len = names.length ();
    safe_retval->length (len);

    for (CORBA::ULong i = 0; i < len; ++i)
      {
        ::Components::ConsumerDescription *tmp =
          this->lookup_consumer_description (names[i]);

        if (!tmp)
          {
            throw Components::InvalidName ();
          }

        safe_retval[i] = tmp;
      }

    return safe_retval._retn ();
  }

  ::Components::EmitterDescriptions *
  Servant_Impl_Base::get_named_emitters (
      const ::Components::NameList & /* names */)
  {
    CIAO_TRACE("Servant_Impl_Base::get_named_emitters");
    throw CORBA::NO_IMPLEMENT ();
  }

  ::Components::ReceptacleDescriptions *
  Servant_Impl_Base::get_all_receptacles (void)
  {
    CIAO_TRACE("  Servant_Impl_Base::get_all_receptacles (void)");
    CIAO_DEBUG (9, (LM_TRACE, CLINFO "Servant_Impl_Base::get_all_receptacles\n"));

    ::Components::ReceptacleDescriptions *tmp = 0;
    ACE_NEW_THROW_EX (tmp,
                      ::Components::ReceptacleDescriptions,
                      CORBA::NO_MEMORY ());

    ::Components::ReceptacleDescriptions_var retval = tmp;

    retval->length (this->receptacle_table_.current_size ());
    CORBA::ULong i = 0;

    CIAO_DEBUG (6, (LM_DEBUG, CLINFO
                "Servant_Impl_Base::get_all_receptacles - Building sequence of length %d\n",
                retval->length()));

    for (ReceptacleTable::iterator iter = this->receptacle_table_.begin ();
         iter != this->receptacle_table_.end ();
         ++iter, ++i)
      {
        CIAO_DEBUG (9, (LM_TRACE, CLINFO
                    "Servant_Impl_Base::get_all_receptacles - Starting loop iteration...\n",
                    retval->length()));

        ReceptacleTable::ENTRY & entry = *iter;
        retval[i] = entry.int_id_;
      }

    CIAO_DEBUG (9, (LM_TRACE, CLINFO "Servant_Impl_Base::get_all_receptacles - Escaped loop.\n"));
    return retval._retn ();
  }

  ::Components::ReceptacleDescriptions *
  Servant_Impl_Base::get_named_receptacles (
    const ::Components::NameList & /* names */)
  {
    CIAO_TRACE("Servant_Impl_Base::get_named_receptacles");
    throw ::CORBA::NO_IMPLEMENT ();
  }

  ::Components::PublisherDescriptions *
  Servant_Impl_Base::get_named_publishers (
    const ::Components::NameList & /* names */)
  {
    CIAO_TRACE("Servant_Impl_Base::get_named_publishers");
    throw ::CORBA::NO_IMPLEMENT ();
  }

  /// Protected operations.
  void
  Servant_Impl_Base::add_facet (const char *port_name,
                                ::CORBA::Object_ptr port_ref)
  {
    CIAO_TRACE("Servant_Impl_Base::add_facet");
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
  }

  CORBA::Object_ptr
  Servant_Impl_Base::lookup_facet (const char *port_name)
  {
    CIAO_TRACE("Servant_Impl_Base::lookup_facet");
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
  }

  ::Components::FacetDescription *
  Servant_Impl_Base::lookup_facet_description (const char *port_name)
  {
    CIAO_TRACE("Servant_Impl_Base::lookup_facet_description");
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

  void
  Servant_Impl_Base::add_receptacle (const char *receptacle_name,
                                     CORBA::Object_ptr recept_ref,
                                     ::Components::Cookie * cookie)
  {
    CIAO_TRACE("Servant_Impl_Base::add_receptacle");
    CIAO_DEBUG (6, (LM_INFO, CLINFO
                "Servant_Impl_Base::add_receptacle - attempting to add new connection to receptacle (%C)\n",
                receptacle_name));

    ::Components::ReceptacleDescription_var safe;
    ::Components::ReceptacleDescription *rd = 0;

    if (this->receptacle_table_.find (receptacle_name, safe) == -1)
    {
      CIAO_DEBUG (6, (LM_DEBUG, CLINFO
                  "Servant_Impl_Base::add_receptacle - Found no receptacle named (%C), creating it...\n",
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
      CIAO_DEBUG (6, (LM_DEBUG, CLINFO
                  "Servant_Impl_Base::add_receptacle - Found a receptacle named (%C)\n",
                  receptacle_name));
      rd = safe.inout ();

      ::Components::ConnectionDescription *cd = 0;
      ACE_NEW_THROW_EX (cd,
                        OBV_Components::ConnectionDescription (cookie,
                                                               recept_ref),
                        CORBA::NO_MEMORY ());
      ::Components::ConnectionDescription_var cd_safe = cd;
      ::Components::ConnectionDescriptions & cds = rd->connections ();
      CORBA::ULong old_length = cds.length ();
      cds.length (old_length + 1);
      cds [old_length] = cd_safe;

      CIAO_DEBUG (6, (LM_DEBUG, CLINFO
                  "Servant_Impl_Base::add_receptacle - Added new connection to "
                  "existing receptacle named (%C)\n",
                  receptacle_name));
    }

    if (this->receptacle_table_.bind (receptacle_name, safe) == 0)
      {
        CIAO_DEBUG (6, (LM_INFO, CLINFO
                    "Servant_Impl_Base::add_receptacle - Successfully added new receptacle named (%C)\n",
                    receptacle_name));
      }
  }

  void
  Servant_Impl_Base::add_consumer (const char *port_name,
                                   ::Components::EventConsumerBase_ptr port_ref)
  {
    CIAO_TRACE("Servant_Impl_Base::add_consumer");
    if (0 == port_name || ::CORBA::is_nil (port_ref))
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO "Servant_Impl_Base::add_consumer - Bad port name [%C] or bad objref\n",
                    port_name));
        throw ::CORBA::BAD_PARAM ();
        return;
      }

    ::Components::ConsumerDescription *cd = 0;
    ACE_NEW_THROW_EX (cd,
                      ::OBV_Components::ConsumerDescription,
                      CORBA::NO_MEMORY ());

    ::Components::ConsumerDescription_var safe = cd;

    cd->name (port_name);
    cd->type_id (port_ref->_interface_repository_id ());
    cd->consumer (port_ref);

    ConsumerTable::value_type entry;
    entry.first = port_name;
    entry.second = safe._retn ();

    ACE_WRITE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, mon, this->lock_,
                              CORBA::NO_RESOURCES ());

    (void) this->consumer_table_.insert (entry);
  }

  ::Components::EventConsumerBase_ptr
  Servant_Impl_Base::lookup_consumer (const char *port_name)
  {
    CIAO_TRACE("Servant_Impl_Base::lookup_consumer");
    if (0 == port_name)
      {
        return ::Components::EventConsumerBase::_nil ();
      }

    ACE_READ_GUARD_THROW_EX (TAO_SYNCH_MUTEX,
                             mon,
                             this->lock_,
                             CORBA::NO_RESOURCES ());

    ConsumerTable::const_iterator iter =
      this->consumer_table_.find (port_name);

    if (iter == this->consumer_table_.end ())
      {
        return ::Components::EventConsumerBase::_nil ();
      }

    return
      ::Components::EventConsumerBase::_duplicate (
        iter->second->consumer ());
  }

  ::Components::ConsumerDescription *
  Servant_Impl_Base::lookup_consumer_description (
    const char *port_name)
  {
    CIAO_TRACE("Servant_Impl_Base::lookup_consumer_description");
    if (0 == port_name)
      {
        /// Calling function will throw InvalidName after getting this.
        return 0;
      }

    ::Components::ConsumerDescription_var cd;
    ConsumerTable::const_iterator iter;

    {
      ACE_READ_GUARD_THROW_EX (TAO_SYNCH_MUTEX,
                               mon,
                               this->lock_,
                               CORBA::NO_RESOURCES ());

      iter = this->consumer_table_.find (port_name);

      if (iter != this->consumer_table_.end ())
        {
          cd = iter->second;
        }
    }

    return cd._retn ();
  }

  ::Components::StandardConfigurator_ptr
  Servant_Impl_Base::get_standard_configurator (void)
  {
    CIAO_TRACE("Servant_Impl_Base::get_standard_configurator (void)");
    // Create the configurator servant.
    StandardConfigurator_Impl *config_impl = 0;

    ACE_NEW_THROW_EX (config_impl,
                      StandardConfigurator_Impl (this),
                      CORBA::NO_MEMORY ());


    Components::StandardConfigurator_var configurator =
      config_impl->_this ();

    return configurator._retn ();
  }

  PortableServer::POA_ptr
  Servant_Impl_Base::_default_POA (void)
  {
    CIAO_TRACE("Servant_Impl_Base::_default_POA (void)");
    return PortableServer::POA::_duplicate (container_->the_POA ());
  }
}


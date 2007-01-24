// $Id$

#include "Servant_Impl_Base.h"
#include "StandardConfigurator_Impl.h"
#include "Container_Base.h"

namespace CIAO
{
  Servant_Impl_Base::Servant_Impl_Base (Components::CCMHome_ptr home,
                                        Home_Servant_Impl_Base *home_servant,
                                        Session_Container * c)
    : home_ (Components::CCMHome::_duplicate (home)),
      home_servant_ (home_servant),
      container_ (c)
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
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::RemoveFailure))
  {
    ACE_TRY
    {
      // Removing Facets
      Components::FacetDescriptions_var facets =
        this->get_all_facets ();

      CORBA::ULong const facet_len = facets->length ();
      for (CORBA::ULong i = 0; i < facet_len; ++i)
      {
        PortableServer::ObjectId_var facet_id =
          this->container_->the_facet_cons_POA ()->reference_to_id
              (facets[i]->facet_ref () ACE_ENV_ARG_PARAMETER);

        CIAO::Servant_Activator *sa =
          this->container_->ports_servant_activator ();

        sa->update_port_activator (facet_id.in () ACE_ENV_ARG_PARAMETER);

        this->container_->the_facet_cons_POA ()->deactivate_object
          (facet_id ACE_ENV_ARG_PARAMETER);
      }

      // Removed Facets

      // Removing Consumers
      Components::ConsumerDescriptions_var consumers =
        this->get_all_consumers ();

      CORBA::ULong const consumer_len = consumers->length ();
      for (CORBA::ULong j = 0; j < consumer_len; ++j)
      {
        PortableServer::ObjectId_var cons_id =
          this->container_->the_facet_cons_POA ()->reference_to_id
              (consumers[j]->consumer () ACE_ENV_ARG_PARAMETER);

        CIAO::Servant_Activator *sa =
          this->container_->ports_servant_activator ();
        sa->update_port_activator (cons_id.in () ACE_ENV_ARG_PARAMETER);

        this->container_->the_facet_cons_POA ()->deactivate_object
          (cons_id ACE_ENV_ARG_PARAMETER);
      }

      Components::SessionComponent_var temp = this->get_executor ();
      temp->ccm_remove ();

      CORBA::Object_var objref =
        this->container_->get_objref (this);

      Components::CCMObject_var ccmobjref =
        Components::CCMObject::_narrow (objref.in ()
                                        ACE_ENV_ARG_PARAMETER);

      PortableServer::ObjectId_var oid;

      this->container_->uninstall_component (ccmobjref.in (),
                                             oid.out ()
                                             ACE_ENV_ARG_PARAMETER);

      this->home_servant_->update_component_map (oid);
    }
    ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Port not active\n");
    }
    ACE_ENDTRY;
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

  CORBA::Object_ptr
  Servant_Impl_Base::provide_facet (
      const char *name
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::InvalidName))
  {
    if (0 == name)
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
    CORBA::ULong const len = names.length ();
    safe_retval->length (len);

    for (CORBA::ULong i = 0; i < len; ++i)
      {
        ::Components::FacetDescription *tmp =
          this->lookup_facet_description (names[i]);

        if (0 == tmp)
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

    retval->length (this->facet_table_.size ());
    CORBA::ULong i = 0;

    for (FacetTable::const_iterator iter = this->facet_table_.begin ();
         iter != this->facet_table_.end ();
         ++iter, ++i)
      {
        FacetTable::const_reference entry = *iter;
        retval[i] = entry.second;
      }

    return retval._retn ();
  }


  ::Components::ConsumerDescriptions *
  Servant_Impl_Base::get_all_consumers (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    ::Components::ConsumerDescriptions *tmp = 0;
    ACE_NEW_THROW_EX (tmp,
                      ::Components::ConsumerDescriptions (
                      this->consumer_table_.size ()),
                      CORBA::NO_MEMORY ());

    ::Components::ConsumerDescriptions_var retval = tmp;

    retval->length (this->consumer_table_.size ());
    CORBA::ULong i = 0;

    for (ConsumerTable::const_iterator iter = this->consumer_table_.begin ();
         iter != this->consumer_table_.end ();
         ++iter, ++i)
      {
        ConsumerTable::const_reference entry = *iter;
        retval[i] = entry.second;
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
    if (0 == sink_name)
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
    CORBA::ULong const len = names.length ();
    safe_retval->length (len);

    for (CORBA::ULong i = 0; i < len; ++i)
      {
        ::Components::ConsumerDescription *tmp =
          this->lookup_consumer_description (names[i]);

        if (0 == tmp)
          {
            ACE_THROW_RETURN (Components::InvalidName (),
                              0);
          }

        safe_retval[i] = tmp;
      }

    return safe_retval._retn ();
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
    ACE_DEBUG ((LM_DEBUG, "In Servant_Impl_Base::get_all_receptacles\n"));

    ::Components::ReceptacleDescriptions *tmp = 0;
    ACE_NEW_RETURN (tmp,
                    ::Components::ReceptacleDescriptions,
                    0);

    ::Components::ReceptacleDescriptions_var retval = tmp;

    retval->length (this->receptacle_table_.current_size ());
    CORBA::ULong i = 0;

    ACE_DEBUG ((LM_DEBUG, "Building sequence of length %d\n", retval->length()));

    for (ReceptacleTable::iterator iter = this->receptacle_table_.begin ();
         iter != this->receptacle_table_.end ();
         ++iter, ++i)
      {
        ACE_DEBUG ((LM_DEBUG, "Starting loop iteration...\n", retval->length()));

        ReceptacleTable::ENTRY & entry = *iter;
        retval[i] = entry.int_id_;
      }

    ACE_DEBUG ((LM_DEBUG, "Escaped loop.\n"));
    return retval._retn ();
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
                                ::CORBA::Object_ptr port_ref
                                ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC (( ::CORBA::SystemException))
  {
    if (0 == port_name || ::CORBA::is_nil (port_ref))
      {
        ACE_THROW ( ::CORBA::BAD_PARAM ());
        return;
      }

    ::Components::FacetDescription *fd = 0;
    ACE_NEW (fd,
             ::OBV_Components::FacetDescription (
              port_name,
              port_ref->_interface_repository_id (),
              port_ref));
    ::Components::FacetDescription_var safe = fd;

    FacetTable::value_type entry;
    entry.first = port_name;
    entry.second = safe._retn ();

    {
      ACE_WRITE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);

      (void) this->facet_table_.insert (entry);
    }
  }

  CORBA::Object_ptr
  Servant_Impl_Base::lookup_facet (const char *port_name)
  {
    if (0 == port_name)
      {
        return CORBA::Object::_nil ();
      }

    ACE_READ_GUARD_RETURN (TAO_SYNCH_MUTEX,
                           mon,
                           this->lock_,
                           CORBA::Object::_nil ());

    FacetTable::const_iterator iter =
      this->facet_table_.find (port_name);

    if (iter == this->facet_table_.end ())
      {
        return CORBA::Object::_nil ();
      }

    return CORBA::Object::_duplicate (iter->second->facet_ref ());
  }

  ::Components::FacetDescription *
  Servant_Impl_Base::lookup_facet_description (const char *port_name)
  {
    if (0 == port_name)
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
    ACE_DEBUG ((LM_DEBUG, "In Servant_Impl_Base::add_receptacle (%s)\n", receptacle_name));

    ::Components::ReceptacleDescription_var safe;
    ::Components::ReceptacleDescription *rd = 0;

    if (this->receptacle_table_.find (receptacle_name,
                                      safe) == -1)
    {
      ACE_DEBUG ((LM_DEBUG, "Found no receptacle named (%s)\n", receptacle_name));

      ACE_NEW (rd,
               OBV_Components::ReceptacleDescription);
      safe = rd;

      rd->name (receptacle_name);
      rd->type_id ();
      // The receptacle is a multiplex receptacle if and only if a
      // cookie was given.
      rd->is_multiple (cookie != 0);

      ::Components::ConnectionDescription *cd = 0;
      ACE_NEW (cd,
               OBV_Components::ConnectionDescription (
                cookie,
                recept_ref));
      ::Components::ConnectionDescription_var cd_safe = cd;
      ::Components::ConnectionDescriptions cds (1);

      cds.length (1);
      cds[0] = cd_safe;
      rd->connections (cds);
    }
    else
    {
      ACE_DEBUG ((LM_DEBUG, "Found a receptacle named (%s)\n", receptacle_name));
      rd = safe.inout ();

      ::Components::ConnectionDescription *cd = 0;
      ACE_NEW (cd,
               OBV_Components::ConnectionDescription (
                cookie,
                recept_ref));
      ::Components::ConnectionDescription_var cd_safe = cd;
      ::Components::ConnectionDescriptions & cds = rd->connections ();
      CORBA::ULong old_length = cds.length ();
      ACE_DEBUG ((LM_DEBUG, "Old length was %d\n", old_length));
      cds.length (old_length + 1);
      ACE_DEBUG ((LM_DEBUG, "New length  %d\n", cds.length ()));
      cds [old_length] = cd_safe;
      ACE_DEBUG ((LM_DEBUG, "Added new connection to receptacle named  (%s)\n", receptacle_name));
    }

    if (this->receptacle_table_.bind (receptacle_name, safe) == 0)
      {
        ACE_DEBUG ((LM_DEBUG, "Successfully added new receptacle named (%s)\n", receptacle_name));
      }
  }

  void
  Servant_Impl_Base::add_consumer (
      const char *port_name,
      ::Components::EventConsumerBase_ptr port_ref
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC (( ::CORBA::SystemException))
  {
    if (0 == port_name || ::CORBA::is_nil (port_ref))
      {
        ACE_THROW ( ::CORBA::BAD_PARAM ());
        return;
      }

    ::Components::ConsumerDescription *cd = 0;
    ACE_NEW (cd,
             ::OBV_Components::ConsumerDescription);
    ::Components::ConsumerDescription_var safe = cd;

    cd->name (port_name);
    cd->type_id (port_ref->_interface_repository_id ());
    cd->consumer (port_ref);

    ConsumerTable::value_type entry;
    entry.first = port_name;
    entry.second = safe._retn ();

    ACE_WRITE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);

    (void) this->consumer_table_.insert (entry);
  }

  ::Components::EventConsumerBase_ptr
  Servant_Impl_Base::lookup_consumer (const char *port_name)
  {
    if (0 == port_name)
      {
        return ::Components::EventConsumerBase::_nil ();
      }

    ACE_READ_GUARD_RETURN (TAO_SYNCH_MUTEX,
                           mon,
                           this->lock_,
                           ::Components::EventConsumerBase::_nil ());

    ConsumerTable::const_iterator iter =
      this->consumer_table_.find (port_name);

    if (iter == this->consumer_table_.end ())
      {
        return ::Components::EventConsumerBase::_nil ();
      }

    return
      ::Components::EventConsumerBase::_duplicate (
          iter->second->consumer ()
        );
  }

  ::Components::ConsumerDescription *
  Servant_Impl_Base::lookup_consumer_description (
      const char *port_name
    )
  {
    if (0 == port_name)
      {
        /// Calling function will throw InvalidName after getting this.
        return 0;
      }

    ::Components::ConsumerDescription_var cd;
    ConsumerTable::const_iterator iter;

    {
      ACE_READ_GUARD_RETURN (TAO_SYNCH_MUTEX,
                             mon,
                             this->lock_,
                             0);

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
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
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
  Servant_Impl_Base::_default_POA (
        ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  {
    return
      PortableServer::POA::_duplicate (container_->the_POA ());
  }
}


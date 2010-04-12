// $Id$

#include "Servant_Impl_Base.h"

#include "ciao/Logger/Log_Macros.h"

namespace CIAO
{
  Servant_Impl_Base::Servant_Impl_Base (Components::CCMHome_ptr home,
                                        Home_Servant_Impl_Base *home_servant,
                                        Container_ptr c)
    : Connector_Servant_Impl_Base (home, home_servant, c)
  {
  }

  Servant_Impl_Base::~Servant_Impl_Base (void)
  {
  }

  void
  Servant_Impl_Base::remove (void)
  {
    CIAO_TRACE("Servant_Impl_Base::remove (void)");
#if !defined (CCM_LW)

    try
    {
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
    }
    catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Port not active\n");
      return;
    }
#endif

    /// This call deactivates facets, removes executor and home
    /// servant (if any), and uninstalls us from the container.
    /// It has its own try/catch blocks.
    this->Connector_Servant_Impl_Base::remove ();
  }

#if !defined (CCM_LW)
  ::Components::ComponentPortDescription *
  Servant_Impl_Base::get_all_ports (void)
  {
    CIAO_TRACE("Servant_Impl_Base::get_all_ports (void)");

    ::Components::ComponentPortDescription_var retv =
      this->Connector_Servant_Impl_Base::get_all_ports ();

    ::Components::ConsumerDescriptions_var consumer_desc =
      this->get_all_consumers ();

    ::Components::EmitterDescriptions_var emitter_desc =
      this->get_all_emitters ();

    ::Components::PublisherDescriptions_var publisher_desc =
      this->get_all_publishers ();

    retv->consumers (consumer_desc.in ());
    retv->emitters (emitter_desc.in ());
    retv->publishers (publisher_desc.in ());

    return retv._retn ();
  }
#endif

#if !defined (CCM_LW)
  ::Components::PrimaryKeyBase *
  Servant_Impl_Base::get_primary_key (void)
  {
    CIAO_TRACE("Servant_Impl_Base::get_primary_key");
    throw ::Components::NoKeyAvailable ();
  }
#endif

#if !defined (CCM_LW)
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

    for (ConsumerTable::const_iterator iter =
           this->consumer_table_.begin ();
         iter != this->consumer_table_.end ();
         ++iter, ++i)
      {
        retval[i] = iter->second;
      }

    return retval._retn ();
  }
#endif

  ::Components::EventConsumerBase_ptr
  Servant_Impl_Base::get_consumer (const char *sink_name)
  {
    CIAO_TRACE("Servant_Impl_Base::get_consumer (const char *sink_name)");

    if (sink_name == 0)
      {
        throw ::CORBA::BAD_PARAM ();
      }

    Components::EventConsumerBase_ptr retval =
      this->lookup_consumer (sink_name);

    if (CORBA::is_nil (retval))
      {
        throw Components::InvalidName ();
      }

    return retval;
  }

#if !defined (CCM_LW)
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
  ::Components::EmitterDescriptions *
  Servant_Impl_Base::get_named_emitters (
      const ::Components::NameList & /* names */)
  {
    CIAO_TRACE("Servant_Impl_Base::get_named_emitters");
    throw CORBA::NO_IMPLEMENT ();
  }
#endif

#if !defined (CCM_LW)
  ::Components::ReceptacleDescriptions *
  Servant_Impl_Base::get_all_receptacles (void)
  {
    CIAO_TRACE("  Servant_Impl_Base::get_all_receptacles (void)");
    CIAO_DEBUG (9,
                (LM_TRACE,
                 CLINFO
                 "Servant_Impl_Base::get_all_receptacles\n"));

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
                 "Servant_Impl_Base::get_all_receptacles "
                 "- Building sequence of length %d\n",
                 retval->length ()));

    for (ReceptacleTable::iterator iter = this->receptacle_table_.begin ();
         iter != this->receptacle_table_.end ();
         ++iter, ++i)
      {
        CIAO_DEBUG (9,
                    (LM_TRACE,
                     CLINFO
                     "Servant_Impl_Base::get_all_receptacles - Starting loop iteration...\n",
                     retval->length ()));

        ReceptacleTable::ENTRY & entry = *iter;
        retval[i] = entry.int_id_;
      }

    CIAO_DEBUG (9,
                (LM_TRACE,
                 CLINFO
                 "Servant_Impl_Base::get_all_receptacles - Escaped loop.\n"));

    return retval._retn ();
  }
#endif

#if !defined (CCM_LW)
  ::Components::ReceptacleDescriptions *
  Servant_Impl_Base::get_named_receptacles (
    const ::Components::NameList & /* names */)
  {
    CIAO_TRACE("Servant_Impl_Base::get_named_receptacles");
    throw ::CORBA::NO_IMPLEMENT ();
  }
#endif

#if !defined (CCM_LW)
  ::Components::PublisherDescriptions *
  Servant_Impl_Base::get_named_publishers (
    const ::Components::NameList & /* names */)
  {
    CIAO_TRACE("Servant_Impl_Base::get_named_publishers");
    throw ::CORBA::NO_IMPLEMENT ();
  }
#endif

  /// Protected operations.
  void
  Servant_Impl_Base::add_consumer (const char *port_name,
                                   ::Components::EventConsumerBase_ptr port_ref)
  {
    CIAO_TRACE("Servant_Impl_Base::add_consumer");

    if (0 == port_name || ::CORBA::is_nil (port_ref))
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Servant_Impl_Base::add_consumer - Bad port name [%C] or bad objref\n",
                     port_name));

        throw ::CORBA::BAD_PARAM ();
        return;
      }

#if !defined (CCM_LW)
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

    ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, mon, this->lock_,
                        CORBA::NO_RESOURCES ());

    (void) this->consumer_table_.insert (entry);
#endif
  }

  ::Components::EventConsumerBase_ptr
  Servant_Impl_Base::lookup_consumer (const char *port_name)
  {
    CIAO_TRACE("Servant_Impl_Base::lookup_consumer");

#if !defined (CCM_LW)
    if (0 == port_name)
      {
        return ::Components::EventConsumerBase::_nil ();
      }

    ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX,
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
#else
    // @todo reimplement for lwccm
    ACE_UNUSED_ARG (port_name);
    return ::Components::EventConsumerBase::_nil ();
#endif
  }

#if !defined (CCM_LW)
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
      ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX,
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
#endif
}


// $Id$

#include "Connector_Servant_Impl_Base.h"
#include "StandardConfigurator_Impl.h"
#include "ciao/Logger/Log_Macros.h"

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
      Container_var cnt_safe = Container::_duplicate(this->container_.in ());
      PortableServer::POA_var port_poa = cnt_safe->the_port_POA ();

      {
        ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX,
                            mon,
                            this->lock_,
                            CORBA::NO_RESOURCES ());
        // Removing Facets
        for (FacetTable::const_iterator iter =
              this->facet_table_.begin ();
            iter != this->facet_table_.end ();
            ++iter)
          {
            PortableServer::ObjectId_var facet_id =
              port_poa->reference_to_id (iter->second);

            port_poa->deactivate_object (facet_id);
          }
      }

      this->ccm_remove ();

      PortableServer::ObjectId_var oid;
      cnt_safe->uninstall_servant (this,
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
      // log error and propagate so error gets reported to deployment framework
      CIAO_ERROR (1, (LM_ERROR, CLINFO "Connector_Servant_Impl_Base::remove - CORBA exception : %C\n", ex._info ().c_str ()));
      ex._raise (); // propagate
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
    const char * name)
  {
    CIAO_TRACE("Connector_Servant_Impl_Base::get_connections");

    ::Components::ReceptacleDescription_var receptacle_desc =
      this->lookup_receptacle_description (name);
    if (::CORBA::is_nil (receptacle_desc.in ()))
      {
        throw ::Components::InvalidName ();
      }
    ::Components::ConnectionDescriptions_var retval;
    ACE_NEW_THROW_EX (retval,
                      ::Components::ConnectionDescriptions,
                      ::CORBA::NO_MEMORY ());

    ::Components::ConnectionDescription * cd = 0;
    ACE_NEW_THROW_EX (cd,
                      ::OBV_Components::ConnectionDescription,
                      ::CORBA::NO_MEMORY ());
    CORBA::Object_var obj = dynamic_cast < ::CORBA::Object_ptr > (receptacle_desc.in ());
    cd->objref (::CORBA::Object::_duplicate (obj.in ()));

    ::CORBA::ULong i = 0UL;
    retval->length (1);
    retval[i] = cd;

    return retval._retn ();
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

    CORBA::Object_var retval = this->lookup_facet (name);

    if ( ::CORBA::is_nil (retval. in()))
      {
        throw ::Components::InvalidName ();
      }

    return retval._retn ();
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

    for (CORBA::ULong i = 0UL; i < len; ++i)
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

    {
      ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX,
                          mon,
                          this->lock_,
                          CORBA::NO_RESOURCES ());
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
    }

    return retval._retn ();
  }
#endif

#if !defined (CCM_LW)
  ::Components::ReceptacleDescriptions *
  Connector_Servant_Impl_Base::get_all_receptacles (void)
  {
    CIAO_TRACE ("Connector_Servant_Impl_Base::get_all_receptacles (void)");

    ::Components::ReceptacleDescriptions * retval = 0;
    ACE_NEW_THROW_EX (retval,
                      ::Components::ReceptacleDescriptions,
                      ::CORBA::NO_MEMORY ());
    ::Components::ReceptacleDescriptions_var safe_retval = retval;
    safe_retval->length (0UL);

    return safe_retval._retn ();
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

#if !defined (CCM_LW) && !defined (CCM_NOEVENT)
  ::Components::ConsumerDescriptions *
  Connector_Servant_Impl_Base::get_all_consumers (void)
  {
    return 0;
  }
#endif

#if !defined (CCM_NOEVENT)
  ::Components::EventConsumerBase_ptr
  Connector_Servant_Impl_Base::get_consumer (
    const char * /* name */)
  {
    return ::Components::EventConsumerBase::_nil ();
  }
#endif

#if !defined (CCM_LW) && !defined (CCM_NOEVENT)

  ::Components::ConsumerDescriptions *
  Connector_Servant_Impl_Base::get_named_consumers (
    const ::Components::NameList & /* names */)
  {
    return 0;
  }
#endif

#if !defined (CCM_LW) && !defined (CCM_NOEVENT)
  ::Components::EmitterDescriptions *
  Connector_Servant_Impl_Base::get_all_emitters (void)
  {
    ::Components::EmitterDescriptions *retval = 0;
    ACE_NEW_THROW_EX (retval,
                      ::Components::EmitterDescriptions,
                      ::CORBA::NO_MEMORY ());

    ::Components::EmitterDescriptions_var safe_retval = retval;
    safe_retval->length (0UL);

    return safe_retval._retn ();
  }
#endif

#if !defined (CCM_LW) && !defined (CCM_NOEVENT)
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
    const ::Components::NameList & names)
  {
    CIAO_TRACE("Connector_Servant_Impl_Base::get_named_receptacles");


    ::Components::ReceptacleDescriptions_var retval;
    ACE_NEW_THROW_EX (retval,
                      ::Components::ReceptacleDescriptions,
                      ::CORBA::NO_MEMORY ());

    retval->length (names.length ());
    ::CORBA::ULong count = 0;

    for (::CORBA::ULong name = 0;
         name < names.length ();
         ++name)
      {
        ::Components::ReceptacleDescription * desc =
          this->lookup_receptacle_description (names[name].in ());
        if (desc)
          {
            retval[count++] = desc;
          }
        else
          {
            throw ::Components::InvalidName ();
          }
      }
    ::Components::ReceptacleDescriptions_var safe_retval = retval;
    return safe_retval._retn ();
  }
#endif

#if !defined (CCM_LW) && !defined (CCM_NOEVENT)
  ::Components::PublisherDescriptions *
  Connector_Servant_Impl_Base::get_all_publishers (void)
  {
    ::Components::PublisherDescriptions *retval = 0;
    ACE_NEW_THROW_EX (retval,
                      ::Components::PublisherDescriptions,
                      ::CORBA::NO_MEMORY ());

    ::Components::PublisherDescriptions_var safe_retval = retval;
    safe_retval->length (0UL);

    return safe_retval._retn ();
  }
#endif

#if !defined (CCM_LW) && !defined (CCM_NOEVENT)
  ::Components::PublisherDescriptions *
  Connector_Servant_Impl_Base::get_named_publishers (
    const ::Components::NameList & /* names */)
  {
    return 0;
  }
#endif

#if !defined (CCM_NOEVENT)
  ::Components::Cookie *
  Connector_Servant_Impl_Base::subscribe (
    const char * /* publisher_name */,
    ::Components::EventConsumerBase_ptr /* subscriber */)
  {
    throw ::Components::InvalidName ();
  }
#endif

#if !defined (CCM_NOEVENT)
  ::Components::EventConsumerBase_ptr
  Connector_Servant_Impl_Base::unsubscribe (
    const char * /* publisher_name */,
    ::Components::Cookie * /* ck */)
  {
    throw ::Components::InvalidName ();
  }
#endif

#if !defined (CCM_NOEVENT)
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
#endif

#if !defined (CCM_NOEVENT)
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
#endif

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

    Container_var cnt_safe =
      Container::_duplicate(this->container_.in ());
    return cnt_safe->the_POA ();
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

    {
      ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX,
                          mon,
                          this->lock_,
                          CORBA::NO_RESOURCES ());

      this->facet_table_[port_name] = ::CORBA::Object::_duplicate (port_ref);
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
  ::Components::ReceptacleDescription *
  Connector_Servant_Impl_Base::lookup_receptacle_description (
    const char * receptacle_name)
  {
    CIAO_TRACE ("Connector_Servant_Impl_Base::lookup_receptacle_description");

    ::Components::ReceptacleDescriptions_var all_receptacles =
      this->get_all_receptacles ();

    if (!receptacle_name || all_receptacles->length () == 0)
      {
        // Calling function will throw InvalidName after getting this.
        return 0;
      }
    for (::CORBA::ULong receptacle = 0;
          receptacle < all_receptacles->length ();
          ++receptacle)
      {
        ::Components::ReceptacleDescription *receptacle_desc =
          all_receptacles[receptacle];
        if (::ACE_OS::strcmp (receptacle_name, receptacle_desc->name ()) == 0)
          {
            ::Components::ReceptacleDescription *rd = 0;

            ACE_NEW_THROW_EX (rd,
                              ::OBV_Components::ReceptacleDescription (
                                receptacle_desc->name (),
                                receptacle_desc->type_id (),
                                receptacle_desc->is_multiple (),
                                receptacle_desc->connections ()),
                              CORBA::NO_MEMORY ());
            ::Components::ReceptacleDescription_var safe = rd;
            return safe._retn ();
          }
      }
    return 0;
  }
#endif

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


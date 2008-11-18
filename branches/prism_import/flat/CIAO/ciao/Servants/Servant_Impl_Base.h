// -*- C++ -*-

//=============================================================================
/**
 *  @file    Servant_Impl_Base.h
 *
 *  $Id$
 *
 *  This file contains the non-template declaration of a base class for
 *  the template mixin for the generated servant class.
 *
 *  @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef CIAO_SERVANT_IMPL_BASE_H
#define CIAO_SERVANT_IMPL_BASE_H

#include /**/ "ace/pre.h"

#include "CIAO_Servant_Impl_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Home_Servant_Impl_Base.h"
#include "ciao/Containers/CIAO_Servant_ActivatorC.h"

#include "ace/Hash_Map_Manager_T.h"
#include "ace/Array_Map.h"
#include "ccm/CCM_ContainerC.h"
#include "ccm/CCM_ObjectS.h"
#include "ccm/CCM_StandardConfiguratorC.h"



namespace CIAO
{
  class Container;
  typedef Container *Container_ptr;
  typedef TAO_Objref_Var_T<Container> Container_var;

  /**
   * @class Servant_Impl_Base
   *
   * @brief Non-template base class for Servant_Impl.
   *
   * Holds the non-template parts of its child class
   * Servant_Impl.
   */
  class CIAO_Servant_Impl_Export Servant_Impl_Base
    : public virtual POA_Components::CCMObject
  {
  protected:
    Servant_Impl_Base (Components::CCMHome_ptr home,
                       Home_Servant_Impl_Base *home_servant,
                       Container_ptr  c);

  public:
    virtual ~Servant_Impl_Base (void);

    /// Operations for CCMObject interface.

    virtual ::Components::PrimaryKeyBase * get_primary_key (void);

    virtual CORBA::IRObject_ptr get_component_def (void);

    virtual Components::SessionComponent_ptr get_executor (void);

    virtual void configuration_complete (void);

    virtual void activate_component (void);

    virtual void passivate_component (void);

    virtual ::Components::CCMHome_ptr get_ccm_home (void);

    virtual void remove (void);

    virtual ::Components::ConnectionDescriptions *
    get_connections (const char *name);

    virtual ::Components::ComponentPortDescription * get_all_ports (void);

    virtual CORBA::Object_ptr provide_facet (const char *name);

    virtual ::Components::FacetDescriptions *
    get_named_facets (const ::Components::NameList & names);

    virtual ::Components::FacetDescriptions * get_all_facets (void);

    virtual ::Components::ConsumerDescriptions * get_all_consumers (void);

    virtual ::Components::EventConsumerBase_ptr
    get_consumer (const char *sink_name);

    virtual ::Components::ConsumerDescriptions *
    get_named_consumers (const ::Components::NameList & names);

    virtual ::Components::EmitterDescriptions *
    get_named_emitters (const ::Components::NameList & names);

    virtual ::Components::ReceptacleDescriptions *
    get_named_receptacles (const ::Components::NameList & names);

    virtual ::Components::ReceptacleDescriptions *
    get_all_receptacles (void);

    virtual ::Components::PublisherDescriptions *
    get_named_publishers (const ::Components::NameList & names);

    /// Operation to set attributes on the component.
    virtual void set_attributes (const Components::ConfigValues &descr);

    // Creates and returns the StandardConfigurator for the component.
    virtual ::Components::StandardConfigurator_ptr
    get_standard_configurator (void);

    /// Override that returns the (passed-in) default POA of our member
    /// component's container, to ensure that we get registered
    /// to that POA when _this() is called.
    virtual PortableServer::POA_ptr _default_POA (void);

    virtual ::Components::Cookie * subscribe (const char * publisher_name,
                                              ::Components::EventConsumerBase_ptr subscriber);

    virtual ::Components::EventConsumerBase_ptr unsubscribe (const char * publisher_name,
                                                             ::Components::Cookie * ck);

    virtual void connect_consumer (const char * emitter_name,
                                   ::Components::EventConsumerBase_ptr consumer);

    virtual ::Components::EventConsumerBase_ptr disconnect_consumer (const char * source_name);

    virtual ::Components::EmitterDescriptions * get_all_emitters (void);

    virtual ::Components::PublisherDescriptions * get_all_publishers (void);

    virtual ::CORBA::Boolean same_component (::CORBA::Object_ptr object_ref);

    virtual ::Components::Cookie * connect (const char * name,
                                            ::CORBA::Object_ptr connection);

    virtual ::CORBA::Object_ptr disconnect (const char * name,
                                            ::Components::Cookie * ck);


  protected:
    void add_facet (const char *port_name,
                    ::CORBA::Object_ptr port_ref);

    CORBA::Object_ptr lookup_facet (const char *port_name);

    ::Components::FacetDescription *lookup_facet_description (
      const char *port_name);

    void add_receptacle (const char *receptacle_name,
                         CORBA::Object_ptr recept_ref,
                         ::Components::Cookie * cookie);

    void add_consumer (const char *port_name,
                       ::Components::EventConsumerBase_ptr port_ref);

    ::Components::EventConsumerBase_ptr lookup_consumer (
      const char *port_name);

    ::Components::ConsumerDescription *lookup_consumer_description (
      const char *port_name);

    /// Called from generated servant class to help with
    /// get_all_*() methods.

    template<typename T_var>
    static void describe_simplex_receptacle (
        const char *port_name,
        const char *port_type_repo_id,
        T_var &connection,
        ::Components::ReceptacleDescriptions_var &descriptions,
        CORBA::ULong slot);

    template<typename T_var>
    static void describe_multiplex_receptacle (
        const char *port_name,
        const char *port_type_repo_id,
        ACE_Array_Map<ptrdiff_t, T_var> &objrefs,
        ::Components::ReceptacleDescriptions_var &descriptions,
        CORBA::ULong slot);

    template<typename T_var>
    static void describe_pub_event_source (
        const char *port_name,
        const char *port_type_repo_id,
        ACE_Array_Map<ptrdiff_t, T_var> &consumers,
        ::Components::PublisherDescriptions_var &descriptions,
        CORBA::ULong slot);

    template<typename T_var>
    static void describe_emit_event_source (
        const char *port_name,
        const char *port_type_repo_id,
        T_var &consumer_ref,
        ::Components::EmitterDescriptions_var &descriptions,
        CORBA::ULong slot);

  protected:
    typedef ACE_Array_Map<ACE_CString,
                          ::Components::FacetDescription_var>
       FacetTable;

    typedef ACE_Array_Map<ACE_CString,
                          ::Components::ConsumerDescription_var>
       ConsumerTable;

    typedef ACE_Hash_Map_Manager_Ex<const char *,
                                    ::Components::ReceptacleDescription_var,
                                    ACE_Hash<const char *>,
                                    ACE_Equal_To<const char *>,
                                    ACE_Null_Mutex>
       ReceptacleTable;

    FacetTable facet_table_;
    ConsumerTable consumer_table_;
    ReceptacleTable receptacle_table_;
    Components::CCMHome_var home_;
    Home_Servant_Impl_Base *home_servant_;
    Container_var container_;

  private:
    /// For internal locking of table reads and writes.
    TAO_SYNCH_MUTEX lock_;
  };
}

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Servant_Impl_Utils_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Servant_Impl_Utils_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* CIAO_SERVANT_IMPL_T_H */

// -*- C++ -*-

//=============================================================================
/**
 *  @file    Connector_Servant_Impl_Base.h
 *
 *  $Id$
 *
 *  This file contains the non-template declaration of a base class for
 *  the template mixin for the generated connector servant class.
 *
 *  @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef CIAO_CONNECTOR_SERVANT_IMPL_BASE_H
#define CIAO_CONNECTOR_SERVANT_IMPL_BASE_H

#include /**/ "ace/pre.h"

#include "CIAO_Servant_Impl_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Home_Servant_Impl_Base.h"

#include "ciao/Containers/CIAO_Servant_ActivatorC.h"
#include "ccm/Session/CCM_SessionComponentC.h"
#include "ccm/CCM_ObjectS.h"
#include "ccm/CCM_StandardConfiguratorC.h"
#include "ciao/Containers/Container_BaseC.h"

#include "ace/Array_Map.h"

namespace CIAO
{
#if !defined (CCM_LW)
  namespace Servant
  {
    template<typename T_var>
    void describe_simplex_receptacle (
        const char *port_name,
        const char *port_type_repo_id,
        T_var &connection,
        ::Components::ReceptacleDescriptions_var &descriptions,
        CORBA::ULong slot);

    template<typename T_var>
    void describe_multiplex_receptacle (
        const char *port_name,
        const char *port_type_repo_id,
        ACE_Array_Map<ptrdiff_t, T_var> &objrefs,
        ::Components::ReceptacleDescriptions_var &descriptions,
        CORBA::ULong slot);
  } /* Servant */
#endif

  /**
   * @class Servant_Impl_Base
   *
   * @brief Non-template base class for Connector_Servant_Impl.
   *
   * Holds the non-template parts of its child class
   * Connector_Servant_Impl.
   */
  class CIAO_Servant_Impl_Export Connector_Servant_Impl_Base
    : public virtual POA_Components::CCMObject
  {
  protected:
    Connector_Servant_Impl_Base (
      Components::CCMHome_ptr home,
      Home_Servant_Impl_Base *home_servant,
      Container_ptr  c);

  public:
    virtual ~Connector_Servant_Impl_Base (void);

    /// Operations for CCMObject interface.

    virtual void remove (void);

#if !defined (CCM_LW)
    virtual CORBA::IRObject_ptr get_component_def (void);
#endif

    /* @todo To be removed, Session knowledge in our base :-(
     */
    virtual Components::SessionComponent_ptr get_executor () = 0;

#if !defined (CCM_LW)
    virtual ::Components::ConnectionDescriptions *
    get_connections (const char *name);
#endif

    virtual CORBA::Object_ptr provide_facet (const char *name);

#if !defined (CCM_LW)
    virtual ::Components::FacetDescriptions *
    get_named_facets (const ::Components::NameList & names);
#endif

#if !defined (CCM_LW)
    virtual ::Components::FacetDescriptions * get_all_facets (void);
#endif

#if !defined (CCM_LW)
    virtual ::Components::PrimaryKeyBase * get_primary_key (void);
#endif

#if !defined (CCM_LW)
    virtual ::Components::ComponentPortDescription * get_all_ports (void);
#endif

#if !defined (CCM_LW)
    virtual ::Components::ConsumerDescriptions * get_all_consumers (void);
#endif

    virtual ::Components::EventConsumerBase_ptr
    get_consumer (const char *sink_name);

#if !defined (CCM_LW)
    virtual ::Components::ConsumerDescriptions *
    get_named_consumers (const ::Components::NameList & names);
#endif

#if !defined (CCM_LW)
    virtual ::Components::EmitterDescriptions *
    get_all_emitters (void);
#endif

#if !defined (CCM_LW)
    virtual ::Components::EmitterDescriptions *
    get_named_emitters (const ::Components::NameList & names);
#endif

 #if !defined (CCM_LW)
    virtual ::Components::ReceptacleDescriptions *
    get_named_receptacles (const ::Components::NameList & names);
#endif

#if !defined (CCM_LW)
    virtual ::Components::PublisherDescriptions *
    get_all_publishers (void);
#endif

#if !defined (CCM_LW)
    virtual ::Components::PublisherDescriptions *
    get_named_publishers (const ::Components::NameList & names);
#endif

    virtual ::Components::Cookie *
    subscribe (const char * publisher_name,
               ::Components::EventConsumerBase_ptr subscriber);

    virtual ::Components::EventConsumerBase_ptr
    unsubscribe (const char * publisher_name,
                 ::Components::Cookie * ck);

    virtual void
    connect_consumer (const char * emitter_name,
                      ::Components::EventConsumerBase_ptr consumer);

    virtual ::Components::EventConsumerBase_ptr
    disconnect_consumer (const char * source_name);

    virtual ::Components::Cookie *
    connect (const char * name,
             ::CORBA::Object_ptr connection);

    virtual ::CORBA::Object_ptr
    disconnect (const char * name,
                ::Components::Cookie * ck);

    /// Operation to set attributes on the component.
    virtual void set_attributes (const Components::ConfigValues &descr);

#if !defined (CCM_LW)
    // Creates and returns the StandardConfigurator for the component.
    virtual ::Components::StandardConfigurator_ptr
    get_standard_configurator (void);
#endif

    /// Override that returns the (passed-in) default POA of our member
    /// component's container, to ensure that we get registered
    /// to that POA when _this() is called.
    virtual PortableServer::POA_ptr _default_POA (void);

    virtual ::CORBA::Object_ptr get_facet_executor (const char *name);

    virtual void activate_component (void) = 0;
    virtual void passivate_component (void) = 0;

  protected:
    void add_facet (const char *port_name,
                    ::CORBA::Object_ptr port_ref);

    CORBA::Object_ptr lookup_facet (const char *port_name);

#if !defined (CCM_LW)
    ::Components::FacetDescription *lookup_facet_description (
      const char *port_name);
#endif

#if !defined (CCM_LW)
    ::Components::ReceptacleDescription *lookup_receptacle_description (
      const char * receptacle_name);
#endif

  protected:
    typedef ACE_Array_Map<ACE_CString,
                          ::CORBA::Object_var>
       FacetTable;

    FacetTable facet_table_;
    Container_var container_;
    Components::CCMHome_var home_;
    Home_Servant_Impl_Base *home_servant_;

    /// For internal locking of table reads and writes.
    TAO_SYNCH_MUTEX lock_;
  };
}

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Connector_Servant_Impl_Utils_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Connector_Servant_Impl_Utils_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* CIAO_CONNECTOR_SERVANT_IMPL_BASE_H */

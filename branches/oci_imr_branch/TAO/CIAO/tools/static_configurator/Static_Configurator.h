// -*- C++ -*-

//=============================================================================
/**
 *  @file    Static_Configurator.h
 *
 *  $Id$
 *
 *  This file contains implementation of the Static_Configurator in CIAO.
 *
 */
//=============================================================================

#ifndef CIAO_STATIC_CONFIGURATOR_H
#define CIAO_STATIC_CONFIGURATOR_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Static_Assembly.h"

namespace CIAO
{
  class Static_Configurator
  {
  public:
  int configure(
                CORBA::ORB_ptr orb,
                Components::Deployment::ComponentServer_ptr component_server,
                CIAO::Static_Config::ContainerAttributes* containers,
                int containers_count,
                CIAO::Static_Config::HomeAttributes* homes,
                int homes_count,
                CIAO::Static_Config::ComponentAttributes* components,
                int components_count,
                CIAO::Assembly_Placement::componentinstantiation::Register_Info*
                component_instantiations,
                int component_instantiations_count,
                CIAO::Static_Config::ConnectionAttributes* connections,
                int connections_count,
                CIAO::Static_Config::ResolveInfoAttributes* resolvers,
                int resolvers_count);

    int create_components (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
    int create_homes (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
    int create_connections (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
    void make_connection (int connections_table_index
                          ACE_ENV_ARG_DECL);
    CORBA::Object_ptr resolve_interface (int resolvers_table_index
                                         ACE_ENV_ARG_DECL_WITH_DEFAULTS);
    Components::CCMObject_ptr resolve_component (int resolvers_table_index
                                                 ACE_ENV_ARG_DECL_WITH_DEFAULTS);
    Components::CCMHome_ptr resolve_home (int resolvers_table_index
                                          ACE_ENV_ARG_DECL_WITH_DEFAULTS);
    Components::EventConsumerBase_ptr resolve_consumer (int resolvers_table_index
                                                        ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    Components::Deployment::Container_ptr
    get_container (const ACE_CString& rtpolicy ACE_ENV_ARG_DECL);

    void register_component(
     Assembly_Placement::componentinstantiation::Register_Info& i,
     Components::CCMObject_ptr c ACE_ENV_ARG_DECL);

  protected:
    CORBA::ORB_ptr orb_;
    Components::Deployment::ComponentServer_ptr component_server_;
    CIAO::Static_Config::ContainerAttributes* containers_;
    int containers_count_;
    CIAO::Static_Config::HomeAttributes* homes_;
    int homes_count_;
    CIAO::Static_Config::ComponentAttributes* components_;
    int components_count_;
    CIAO::Assembly_Placement::componentinstantiation::Register_Info*
    component_instantiations_;
    int component_instantiations_count_;
    CIAO::Static_Config::ConnectionAttributes* connections_;
    int connections_count_;
    CIAO::Static_Config::ResolveInfoAttributes* resolvers_;
    int resolvers_count_;
    typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                    Components::CCMHome_var,
                                    ACE_Hash<ACE_CString>,
                                    ACE_Equal_To<ACE_CString>,
                                    ACE_Null_Mutex> HOME_MAP;
    HOME_MAP installed_homes_;

    typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                    Components::CCMObject_var,
                                    ACE_Hash<ACE_CString>,
                                    ACE_Equal_To<ACE_CString>,
                                    ACE_Null_Mutex> COMP_MAP;
    COMP_MAP instantiated_components_;
  };
}

#include /**/ "ace/post.h"
#endif /* CIAO_STATIC_CONFIGURATOR_H */

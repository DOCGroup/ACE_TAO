// -*- C++ -*-

//=============================================================================
/**
 *  @file    Static_Assembly_Visitors.h
 *
 *  $Id$
 *
 *  This file contains implementations for various visitors that
 *  traverse and process CIAO::Assembly_Spec.
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================


#ifndef CIAO_STATIC_ASSEMBLY_VISITOR_H
#define CIAO_STATIC_ASSEMBLY_VISITOR_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Hash_Map_Manager_T.h"
#include "../XML_Helpers/Assembly_Spec.h"
//#include "Deployment_Configuration.h"
#include "Static_Assembly.h"
#include "ace/Configuration.h"
#include "../XML_Helpers/RTConfigurationC.h"

namespace CIAO
{
  /**
   * @class Static_Assembly_Builder_Visitor
   *
   * @brief A visitor implementation that "build" up the application
   * ACE_Hash_Map_Manager_Exby visiting nodes in an Assembly_Spec
   */
  class Static_Assembly_Builder_Visitor : public Assembly_Placement::Visitor
  {
  public:
    /// Constructor
    Static_Assembly_Builder_Visitor (ID_IMPL_MAP &idmap,
                                     const char*,
                                     Static_Config::Static_Config_Info&);

    /// Destructor
    virtual ~Static_Assembly_Builder_Visitor (void);

    virtual int visit_Container (Assembly_Placement::Container *c
                                 ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    virtual int visit_hostcollocation (Assembly_Placement::hostcollocation *hc
                                       ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    virtual int visit_processcollocation (Assembly_Placement::processcollocation *pc
                                          ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    virtual int visit_homeplacement (Assembly_Placement::homeplacement *hp
                                     ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    virtual int visit_componentinstantiation (Assembly_Placement::componentinstantiation *ci
                                              ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    void build_connections (Assembly_Spec::CONNECTION_QUEUE& connections);

    void dump_static_config_info ();

    void generate_static_header_file (const char* header_file_name);
    void generate_static_app_driver (const char* app_driver_file_name);
    int generate_static_app_mpc (const char* );

  protected:
    /// Context to build on.
    ID_IMPL_MAP &impl_idref_map_;

    /// Deployment Configuration Info.
    //Deployment_Configuration &deployment_config_;

    Static_Config::Static_Config_Info& static_config_info_;

    Static_Config::ContainerAttributes   *containers_table_;
    Static_Config::HomeAttributes        *homes_table_;
    Static_Config::ComponentAttributes   *components_table_;
    
    Assembly_Placement::componentinstantiation::Register_Info
                                         *component_registrations_table_;

    Static_Config::ConnectionAttributes  *connections_table_;
    Static_Config::ResolveInfoAttributes *resolvers_table_;

    Static_Config::ThreadPoolAttributes      *thread_pool_table_;
    Static_Config::LaneAttributes            *lane_table_;
    Static_Config::ThreadPoolLanesAttributes *thread_pool_lanes_table_;
    Static_Config::BandAttributes            *band_table_;
    Static_Config::PriorityBandsAttributes   *priority_band_table_;
    Static_Config::PolicyConfigAttributes    *policy_config_table_;
    Static_Config::PolicySetAttributes       *policy_set_table_;

    int& containers_table_last_index_;
    int& homes_table_last_index_;
    int& components_table_last_index_;
    int& component_registrations_table_last_index_;
    int& connections_table_last_index_;
    int& resolvers_table_last_index_;
    int& thread_pool_table_last_index_;
    int& lane_table_last_index_;
    int& thread_pool_lanes_table_last_index_;
    int& band_table_last_index_;
    int& priority_band_table_last_index_;
    int& policy_config_table_last_index_;
    int& policy_set_table_last_index_;

    ACE_CString installation_file_;
    ACE_Configuration* installation_;
    ACE_CString section_name_;
    ACE_Configuration_Section_Key section_;

    bool is_realtime_;

  protected:
    int find_container (const ACE_CString& rtpolicy_name);
    int build_resolver_info (Assembly_Connection::IF_Resolver_Info* info);

    int  build_rt_resource_info(CIAO::RTConfiguration::RTORB_Resource_Info &rt_resources);
    int  build_rt_policy_set(CIAO::RTConfiguration::Policy_Sets &pset);

    void generate_rt_static_app_driver (const char* app_driver_file_name);
  };
}

#include /**/ "ace/post.h"
#endif /* CIAO_ASSEMBLY_VISITOR_H */

// -*- C++ -*-

//=============================================================================
/**
 *  @file    Static_Assembly.h
 *
 *  $Id$
 *
 *  This file contains structure definitions for static configuration
 *
 *  @author Venkita Subramonian <venkita@cs.wustl.edu>
 */
//=============================================================================

#ifndef CIAO_STATIC_ASSEMBLY_H
#define CIAO_STATIC_ASSEMBLY_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "../XML_Helpers/Assembly_Spec.h"
//#include "Deployment_Configuration.h"
#include "Container_Base.h"

namespace CIAO
{

namespace Static_Config
{

struct ContainerAttributes
{
  // idref to the name of RTPolicySet
  ACE_CString rtpolicyset_ref_;

  Components::Deployment::Container_var container_;

};

struct HomeAttributes
{
  ACE_CString id_;

  /// Specify the pathname to component executor DLL.
  ACE_TString executor_UUID_;

  /// Specify the entrypoint to component executor DLL.
  ACE_TString executor_entrypt_;

  ::CIAO::HomeFactory executor_fptr_;

  /// Specify the pathname to component servant DLL.
  ACE_TString servant_UUID_;

  /// Specify the entrypoint to component servant DLL.
  ACE_TString servant_entrypt_;

  ::CIAO::ServantFactory servant_fptr_;

  int containers_table_index_;

  Components::KeylessCCMHome_var home_;
};

struct ComponentAttributes
{
  ACE_CString id_;

  int component_registration_begin_index_;
  int component_registration_end_index_;
  int homes_table_index_;
};

struct ConnectionAttributes
{
  CIAO::Assembly_Connection::Connection_Type type_;
  ACE_CString id_;
  ACE_CString name_;
  int component_index_;
  int interface_index_;
  CIAO::Assembly_Connection::IF_Resolver_Info *component__;
  CIAO::Assembly_Connection::IF_Resolver_Info *interface_;
};

struct ResolveInfoAttributes
{
  CIAO::Assembly_Connection::IF_Resolution_Method resolver_type_;
  ACE_CString resolver_info_;
  int nested_resolver_index_;
  CIAO::Assembly_Connection::IF_Resolver_Info *nested_resolver_;
};

struct ThreadPoolAttributes
{
 ACE_CString       name_;
 unsigned long     stacksize_;
 unsigned long     static_threads_;
 unsigned long     dynamic_threads_;
 short             default_priority_;
 int               allow_request_buffering_;
 unsigned long     max_buffered_requests_;
 unsigned long     max_request_buffer_size_;
};

struct LaneAttributes
{
 short             lane_priority_;
 unsigned long     static_threads_;
 unsigned long     dynamic_threads_;
};

struct ThreadPoolLanesAttributes
{
 ACE_CString       name_;
 unsigned long     stacksize_;
 unsigned int      lane_begin_index_;
 unsigned int      lane_end_index_;
 int               allow_borrowing_;
 int               allow_request_buffering_;
 unsigned long     max_buffered_requests_;
 unsigned long     max_request_buffer_size_;
};

struct BandAttributes
{
 short low_;
 short high_;
};

struct PriorityBandsAttributes
{
 ACE_CString       name_;
 unsigned int      band_begin_index_;
 unsigned int      band_end_index_;
};

struct PolicyConfigAttributes
{
 CORBA::PolicyType      type_;
 ACE_CString            name_;
 RTCORBA::PriorityModel model_;
 RTCORBA::Priority      default_priority_;
};

struct PolicySetAttributes
{
 ACE_CString      name_;
 unsigned int     config_begin_index_;
 unsigned int     config_end_index_;
};

static const int MAX_CONTAINERS = 100;
static const int MAX_HOMES = 1000;
static const int MAX_COMPONENTS = 10000;
static const int MAX_COMPONENT_REGISTRATIONS = 30000;
static const int MAX_CONNECTIONS = 30000;
static const int MAX_THREADPOOLS = 1000;
static const int MAX_LANES = 1000;
static const int MAX_THREAD_POOL_LANES = 1000;
static const int MAX_BANDS = 1000;
static const int MAX_PRIORITY_BANDS = 1000;
static const int MAX_POLICY_CONFIGS = 1000;
static const int MAX_POLICY_SETS = 1000;

struct Static_Config_Info
{
  ContainerAttributes containers_table_[MAX_CONTAINERS];
  HomeAttributes homes_table_[MAX_HOMES];
  ComponentAttributes components_table_[MAX_COMPONENTS];
  CIAO::Assembly_Placement::componentinstantiation::Register_Info component_registrations_table_[MAX_COMPONENT_REGISTRATIONS];
  ConnectionAttributes connections_table_[MAX_CONNECTIONS];
  ResolveInfoAttributes resolvers_table_[MAX_CONNECTIONS];
  ThreadPoolAttributes      thread_pool_table_[MAX_THREADPOOLS];
  LaneAttributes            lane_table_[MAX_LANES];
  ThreadPoolLanesAttributes thread_pool_lanes_table_[MAX_THREAD_POOL_LANES];
  BandAttributes            band_table_[MAX_BANDS];
  PriorityBandsAttributes   priority_band_table_[MAX_PRIORITY_BANDS];
  PolicyConfigAttributes    policy_config_table_[MAX_POLICY_CONFIGS];
  PolicySetAttributes       policy_set_table_[MAX_POLICY_SETS];

  int containers_table_last_index_;
  int homes_table_last_index_;
  int components_table_last_index_;
  int component_registrations_table_last_index_;
  int connections_table_last_index_;
  int resolvers_table_last_index_;
  int thread_pool_table_last_index_;
  int lane_table_last_index_;
  int thread_pool_lanes_table_last_index_;
  int band_table_last_index_;
  int priority_band_table_last_index_;
  int policy_config_table_last_index_;
  int policy_set_table_last_index_;

  Static_Config_Info ()
    :containers_table_last_index_ (-1),
     homes_table_last_index_ (-1),
     components_table_last_index_ (-1),
     component_registrations_table_last_index_ (-1),
     connections_table_last_index_ (-1),
     resolvers_table_last_index_ (-1),
     thread_pool_table_last_index_ (-1),
     lane_table_last_index_ (-1),
     thread_pool_lanes_table_last_index_ (-1),
     band_table_last_index_ (-1),
     priority_band_table_last_index_ (-1),
     policy_config_table_last_index_ (-1),
     policy_set_table_last_index_ (-1)
  {
  }
};

}

}

#include /**/ "ace/post.h"
#endif /* CIAO_STATIC_ASSEMBLY_H */

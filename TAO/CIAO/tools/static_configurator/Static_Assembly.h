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

static const int MAX_CONTAINERS = 100;
static const int MAX_HOMES = 1000;
static const int MAX_COMPONENTS = 10000;
static const int MAX_COMPONENT_REGISTRATIONS = 30000;
static const int MAX_CONNECTIONS = 30000;

struct Static_Config_Info
{
  ContainerAttributes containers_table_[MAX_CONTAINERS];
  HomeAttributes homes_table_[MAX_HOMES];
  ComponentAttributes components_table_[MAX_COMPONENTS];
  CIAO::Assembly_Placement::componentinstantiation::Register_Info component_registrations_table_[MAX_COMPONENT_REGISTRATIONS];
  ConnectionAttributes connections_table_[MAX_CONNECTIONS];
  ResolveInfoAttributes resolvers_table_[MAX_CONNECTIONS];

  int containers_table_last_index_;
  int homes_table_last_index_;
  int components_table_last_index_;
  int component_registrations_table_last_index_;
  int connections_table_last_index_;
  int resolvers_table_last_index_;

  Static_Config_Info ()
    :containers_table_last_index_ (-1),
     homes_table_last_index_ (-1),
     components_table_last_index_ (-1),
     component_registrations_table_last_index_ (-1),
     connections_table_last_index_ (-1),
     resolvers_table_last_index_ (-1)
  {
  }
};

}

}

#include /**/ "ace/post.h"
#endif /* CIAO_STATIC_ASSEMBLY_H */

// -*- C++ -*-

//=============================================================================
/**
 * @file LB_PropertyManager.h
 *
 * $Id$
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_LB_PROPERTY_MANAGER_H
#define TAO_LB_PROPERTY_MANAGER_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/LoadBalancingC.h"

class TAO_LB_PropertyManager
  : public virtual LoadBalancing::PropertyManager
{
public:

  /**
   * @name TAO_LoadBalancer::PropertyManager methods
   */
  //@{

  /// Set the default properties to be used by all object groups.
  virtual void set_default_properties (
      const LoadBalancing::Properties & props,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     LoadBalancing::InvalidProperty,
                     LoadBalancing::UnsupportedProperty));

  /// Get the default properties used by all object groups.
  virtual LoadBalancing::Properties * get_default_properties (
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Remove default properties.
  virtual void remove_default_properties (
      const LoadBalancing::Properties & props,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     LoadBalancing::InvalidProperty,
                     LoadBalancing::UnsupportedProperty));

  /// Set properties associated with a given Replica type.  These
  /// properties override the default properties.
  virtual void set_type_properties (
      const char * type_id,
      const LoadBalancing::Properties & overrides,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     LoadBalancing::InvalidProperty,
                     LoadBalancing::UnsupportedProperty));

  /**
   * Return the properties associated with a give Replica type.  These
   * properties include the type-specific properties in use, in
   * addition to the default properties that were not overridden.
   */
  virtual LoadBalancing::Properties * get_type_properties (
      const char * type_id,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Remove the given properties associated with the Replica type ID.
  virtual void remove_type_properties (
      const char * type_id,
      const LoadBalancing::Properties & props,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     LoadBalancing::InvalidProperty,
                     LoadBalancing::UnsupportedProperty));

  /**
   * Dynamically set the properties associated with a given object
   * group as the load balancer and replicas are being executed.
   * These properties override the type-specific and default
   * properties.
   */
  virtual void set_properties_dynamically (
      LoadBalancing::ObjectGroup_ptr object_group,
      const LoadBalancing::Properties & overrides,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     LoadBalancing::ObjectGroupNotFound,
                     LoadBalancing::InvalidProperty,
                     LoadBalancing::UnsupportedProperty));

  /**
   * Return the properties currently in use by the given object
   * group.  These properties include those that were set dynamically,
   * type-specific properties that weren't overridden, properties that
   * were used when the Replica was created, and default properties
   * that weren't overridden.
   */
  virtual LoadBalancing::Properties * get_properties (
      LoadBalancing::ObjectGroup_ptr object_group,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     LoadBalancing::ObjectGroupNotFound));

  //@}


  /// Verify that the given properties are valid and/or supported by
  /// the Load Balancer.
  void validate_properties (const LoadBalancing::Properties &props,
                            CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((LoadBalancing::InvalidProperty,
                     LoadBalancing::UnsupportedProperty));

  /**
   * @name TAO-specific Load Balancing PropertyManager Helper Methods
   *
   * These methods are used to avoid the allocations indirectly caused
   * by conformance to the C++ mapping.  Specifically, the IDL defined
   * PropertyManager methods above must return a set of properties
   * whose storage must be released by the caller.
   */
  //@{

  /// Return the membership style for the given object group.
  LoadBalancing::MembershipStyle membership_style (
    CORBA::Object_ptr object_group) const;

  /// Return the load monitoring style for the given object group.
  LoadBalancing::MonitoringStyle load_monitoring_style (
    CORBA::Object_ptr object_group) const;

  /// Return the load monitoring granularity for the given object
  /// group.
  LoadBalancing::MonitoringGranularity
    load_monitoring_granularity (CORBA::Object_ptr object_group) const;

  /// Return the initial number of replicas for the given object
  /// group.
  LoadBalancing::InitialNumberReplicas
    initial_number_replicas (CORBA::Object_ptr object_group) const;

  /// Return the minimum number of replicas for the given object
  /// group.
  LoadBalancing::MinimumNumberReplicas
    minimum_number_replicas (CORBA::Object_ptr object_group) const;

  /// Return the sequence FactoryInfos associated with the given
  /// object group.
  LoadBalancing::FactoryInfos *
    factory_infos (CORBA::Object_ptr object_group) const;

  //@}

  /// Type-specific property hash map.
  typedef ACE_Hash_Map_Manager_Ex<
    const char *,
    LoadBalancing::Properties,
    ACE_Hash<const char *>,
    ACE_Equal_To<const char *>,
    TAO_SYNCH_MUTEX> Type_Prop_Table;

  /// Properties used when a given object group was created.
  typedef ACE_Hash_Map_Manager_Ex<
    PortableServer::ObjectId,
    LoadBalancing::Properties,
    TAO_ObjectId_Hash,
    ACE_Equal_To<PortableServer::ObjectId>,
    TAO_SYNCH_MUTEX> Dynamic_Prop_Table;

private:

  /// Default properties.
  LoadBalancing::Properties default_properties_;

  /// Table of type-specific object group properties.
  Type_Prop_Table type_properties_;

  /// Table of object group properties used at run-time.
  Dynamic_Prop_Table dynamic_properties_;

};


/// Property sequence equality operator.
CORBA::Boolean operator== (const LoadBalancing::Properties &lhs,
                           const LoadBalancing::Properties &rhs);


#include "ace/post.h"

#endif /* TAO_LB_PROPERTY_MANAGER_H */

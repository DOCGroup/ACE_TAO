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
  LoadBalancing::LoadMonitoringStyle load_monitoring_style (
    CORBA::Object_ptr object_group) const;

  /// Return the load monitoring granularity for the given object
  /// group.
  LoadBalancing::LoadMonitoringGranularity
    load_monitoring_granularity (CORBA::Object_ptr object_group) const;

  /// Return the initial number of replicas for the given object
  /// group.
  LoadBalancing::InitialNumberReplicas
    initial_number_replicas (CORBA::Object_ptr object_group) const;

  /// Return the minimum number of replicas for the given object
  /// group.
  LoadBalancing::MinimumNumberReplicas
    minimum_number_replicas (CORBA::Object_ptr object_group) const;

  //@}

//   /// Type-specific property hash map.
//   typedef ACE_Hash_Map_Manager_Ex<
//     const char *,
//     LoadBalancing::Property,
//     ACE_Hash<const ACE_TCHAR *>,
//     TAO_LB_Property_Equal_To,
//     ACE_SYNCH_MUTEX> Type_Prop_Table;

//   typedef ACE_Hash_Map_Manager_Ex<
//     ACE_UINT32,                // Use the FactoryCreationId as the hash.
//     LoadBalancing::Property,
//     ACE_Hash<ACE_UINT32>,
//     TAO_LB_Property_Equal_To,
//     ACE_SYNCH_MUTEX> Creation_Prop_Table;

//   typedef Creation_Prop_Table Dynamic_Prop_Table;

private:

  /// Table of global default object group properties.
//   Default_Prop_Table default_properties_;

//   /// Table of type-specific object group properties.
//   Type_Prop_Table type_properties_;

//   /// Table of object group properties used when the object group was
//   /// created.
//   Creation_Prop_Table creation_properties_;

//   /// Table of object group properties used at run-time.
//   Dynamic_Prop_Table dynamic_properties_;

};

#include "ace/post.h"

#endif /* TAO_LB_PROPERTY_MANAGER_H */

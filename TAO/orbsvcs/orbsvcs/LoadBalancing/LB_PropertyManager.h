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

#include "ace/Functor.h"
#include "ace/Hash_Map_Manager_T.h"
#include "orbsvcs/LoadBalancingC.h"


/// Forward declarations.
class TAO_LB_ObjectGroup_Map;


/**
 * @class TAO_LB_PropertyManager
 *
 * @brief Class that implements the LoadBalancing::PropertyManager
 *        interface.
 *
 * Only the default and type-specific properties are housed in this
 * class.  The properties used at creation time of an object group and
 * those set dynamically after object group creation are stored in the
 * TAO_LB_ObjectGroup_Map_Entry structure.  However, the
 * PropertyManager is still used to manage those properties.
 */
class TAO_LB_PropertyManager
  : public virtual LoadBalancing::PropertyManager
{
public:

  /// Constructor.
  TAO_LB_PropertyManager (TAO_LB_ObjectGroup_Map &object_group_map);

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
   * were used when the replica was created, and default properties
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
    initial_number_replicas (const char *type_id,
                             const LoadBalancing::Criteria &the_criteria,
                             CORBA::Environment &ACE_TRY_ENV) const;

  /// Return the minimum number of replicas for the given object
  /// group.
  LoadBalancing::MinimumNumberReplicas
    minimum_number_replicas (CORBA::Object_ptr object_group) const;

  /// Return the sequence FactoryInfos associated with the given
  /// object group.
  LoadBalancing::FactoryInfos *
    factory_infos (const char *type_id,
                   const LoadBalancing::Criteria &the_criteria,
                   CORBA::Environment &ACE_TRY_ENV) const;

  //@}

  /// Type-specific property hash map.
  typedef ACE_Hash_Map_Manager_Ex<
    const char *,
    LoadBalancing::Properties,
    ACE_Hash<const char *>,
    ACE_Equal_To<const char *>,
    ACE_Null_Mutex> Type_Prop_Table;

private:

  /// Table that maps ObjectId to Object Group related information.
  TAO_LB_ObjectGroup_Map &object_group_map_;

  /// Default properties.
  LoadBalancing::Properties default_properties_;

  /// Table of type-specific object group properties.
  Type_Prop_Table type_properties_;

  /// Lock used to synchronize access to the default properties and
  /// the type-specific properties.
  TAO_SYNCH_MUTEX lock_;

};


#include "ace/post.h"

#endif /* TAO_LB_PROPERTY_MANAGER_H */

// -*- C++ -*-

//=============================================================================
/**
 * @file LoadBalancingI.h
 *
 * $Id$
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_LOADBALANCINGI_H
#define TAO_LOADBALANCINGI_H

#include "ace/pre.h"

#include "ace/Synch.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/LoadBalancingS.h"

#include "LB_ObjectGroup_Map.h"
#include "LB_PropertyManager.h"
#include "LB_GenericFactory.h"
#include "LB_ObjectGroupManager.h"


/// Forward declarations
class TAO_LB_Balancing_Strategy;


class TAO_LoadBalancing_Export TAO_LoadBalancing_ReplicationManager_i
  : public virtual POA_LoadBalancing::ReplicationManager
{
public:

  /// Constructor.
  TAO_LoadBalancing_ReplicationManager_i (void);

  /// Destructor.
  ~TAO_LoadBalancing_ReplicationManager_i (void);

  /// Register a load notifier with the load balancer
  /// ReplicationManager.
  virtual void register_load_notifier (
      LoadBalancing::LoadNotifier_ptr load_notifier,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return a reference to the load notifier in use.
  virtual LoadBalancing::LoadNotifier_ptr get_load_notifier (
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     LoadBalancing::InterfaceNotFound));

  /// Register a load monitor with the load balancer
  /// ReplicationManager.
  virtual void register_load_monitor (
      LoadBalancing::LoadMonitor_ptr load_monitor,
      const LoadBalancing::Location & the_location,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return a reference to the load notifier in use.
  virtual LoadBalancing::LoadMonitor_ptr get_load_monitor (
      const LoadBalancing::Location & the_location,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     LoadBalancing::InterfaceNotFound));

  /**
   * @name LoadBalancing::PropertyManager methods
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
   * @name LoadBalancing::ObjectGroupManager methods
   */
  //@{

  /// Create a member using the load balancer ObjectGroupManager, and
  /// add the created object to the ObjectGroup.
  virtual LoadBalancing::ObjectGroup_ptr create_member (
      LoadBalancing::ObjectGroup_ptr object_group,
      const LoadBalancing::Location & the_location,
      const char * type_id,
      const LoadBalancing::Criteria & the_criteria,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     LoadBalancing::ObjectGroupNotFound,
                     LoadBalancing::MemberAlreadyPresent,
                     LoadBalancing::NoFactory,
                     LoadBalancing::ObjectNotCreated,
                     LoadBalancing::InvalidCriteria,
                     LoadBalancing::CannotMeetCriteria));

  /// Add an existing object to the ObjectGroup.
  virtual LoadBalancing::ObjectGroup_ptr add_member (
      LoadBalancing::ObjectGroup_ptr object_group,
      const LoadBalancing::Location & the_location,
      CORBA::Object_ptr member,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     LoadBalancing::ObjectGroupNotFound,
                     LoadBalancing::MemberAlreadyPresent,
                     LoadBalancing::ObjectNotAdded));

  /**
   * Remove an object at a specific location from the given
   * ObjectGroup.  Deletion of application created objects must be
   * deleted by the application.  Objects created by the
   * infrastructure (load balancer) will be deleted by the
   * infrastructure.
   */
  virtual LoadBalancing::ObjectGroup_ptr remove_member (
      LoadBalancing::ObjectGroup_ptr object_group,
      const LoadBalancing::Location & the_location,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     LoadBalancing::ObjectGroupNotFound,
                     LoadBalancing::MemberNotFound));

  /// Return the locations of the members in the given ObjectGroup.
  virtual LoadBalancing::Locations * locations_of_members (
      LoadBalancing::ObjectGroup_ptr object_group,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     LoadBalancing::ObjectGroupNotFound));

  /// Return the ObjectGroupId for the given ObjectGroup.
  virtual LoadBalancing::ObjectGroupId get_object_group_id (
      LoadBalancing::ObjectGroup_ptr object_group,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     LoadBalancing::ObjectGroupNotFound));

  /// @note Does this method make sense for load balanced objects?
  virtual LoadBalancing::ObjectGroup_ptr get_object_group_ref (
      LoadBalancing::ObjectGroup_ptr object_group,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     LoadBalancing::ObjectGroupNotFound));

  /// Return the reference corresponding to the Replica of a given
  /// ObjectGroup at the given location.
  virtual CORBA::Object_ptr get_member_ref (
      LoadBalancing::ObjectGroup_ptr object_group,
      const LoadBalancing::Location & loc,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     LoadBalancing::ObjectGroupNotFound,
                     LoadBalancing::MemberNotFound));

  //@}

  /**
   * @name LoadBalancing::GenericFactory methods
   */
  //@{

  /**
   * Create an object of the specified type that adheres to the
   * restrictions defined by the provided Criteria.  The out
   * FactoryCreationId parameter may be passed to the delete_object()
   * method to delete the object.
   */
  virtual CORBA::Object_ptr create_object (
      const char * type_id,
      const LoadBalancing::Criteria & the_criteria,
      LoadBalancing::GenericFactory::FactoryCreationId_out
        factory_creation_id,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     LoadBalancing::NoFactory,
                     LoadBalancing::ObjectNotCreated,
                     LoadBalancing::InvalidCriteria,
                     LoadBalancing::InvalidProperty,
                     LoadBalancing::CannotMeetCriteria));

  /**
   * Delete the object corresponding to the provided
   * FactoryCreationId.  If the object is actually an ObjectGroup,
   * then all members within the ObjectGroup will be deleted.
   * Afterward, the ObjectGroup itself will be deleted.
   */
  virtual void delete_object (
      const LoadBalancing::GenericFactory::FactoryCreationId &
        factory_creation_id,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     LoadBalancing::ObjectNotFound));

  //@}

public:

  /// Return a reference to the replica to which the current client
  /// request will be forwarded.
  /**
   * The load balancer does the right thing and figures which object
   * group the replica should be chosen from.
   */
  CORBA::Object_ptr replica (const PortableServer::ObjectId &oid,
                             CORBA::Environment &ACE_TRY_ENV);


  /// Initialize the load balancer.  This will cause a child POA to be
  /// created with the appropriate policies to support ServantLocators
  /// (i.e. for the ReplicaLocator).
  void init (PortableServer::POA_ptr root_poa,
             CORBA::Environment &ACE_TRY_ENV);

private:

  /// Extract the value of the InitialNumberReplicas property from
  /// the_criteria.
  int get_initial_number_replicas (
    const char *type_id,
    const LoadBalancing::Criteria &the_criteria,
    CORBA::UShort &initial_number_replicas) const;

  /// Extract the value of the Factories property from the_criteria.
  /**
   * This method ensures that the locations in the returned
   * FactoryInfos are unique.  This is necessary to ensure that only
   * one replica of a given type is created by the load balancer at a
   * given location.
   */
  int get_factory_infos (
    const char *type_id,
    const LoadBalancing::Criteria &the_criteria,
    LoadBalancing::FactoryInfos &factory_infos) const;

private:

  /// The POA that dispatches requests to the ReplicaLocator.
  PortableServer::POA_var poa_;

  /// Mutex that provides synchronization.
  TAO_SYNCH_MUTEX lock_;

  /// Map between RepositoryId, ObjectGroup reference, replica list and
  /// factory creation ID.
  TAO_LB_ObjectGroup_Map object_group_map_;

  /// The PropertyManager that is reponsible for parsing all criteria,
  /// and keeping track of property-type_id associations.
  TAO_LB_PropertyManager property_manager_;

  /// The GenericFactory responsible for creating all object groups.
  TAO_LB_GenericFactory generic_factory_;

  /// The ObjectGroupManager that implements the functionality
  /// necessary for application-controlled object group membership.
  TAO_LB_ObjectGroupManager object_group_manager_;

  /// The load balancing strategy used when making load balancing
  /// decisions.
  TAO_LB_Balancing_Strategy *balancing_strategy_;

};


#include "ace/post.h"

#endif  /* TAO_LOADBALANCINGI_H */

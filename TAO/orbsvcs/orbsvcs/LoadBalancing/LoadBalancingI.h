// -*- C++ -*-

//=============================================================================
/**
 * @file ReplicaLocator.h
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

class TAO_LoadBalancing_ReplicationManager_i
  : public virtual POA_TAO_LoadBalancing::ReplicationManager
{
public:

  /// Constructor.
  TAO_LoadBalancing_ReplicationManager_i (void);

  /// Destructor.
  ~TAO_LoadBalancing_ReplicationManager_i (void);

  /// Register a load notifier with the load balancer
  /// ReplicationManager.
  virtual void register_load_notifier (
      TAO_LoadBalancing::LoadNotifier_ptr load_notifier,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return a reference to the load notifier in use.
  virtual TAO_LoadBalancing::LoadNotifier_ptr get_load_notifier (
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     TAO_LoadBalancing::InterfaceNotFound));

  /**
   * @name TAO_LoadBalancer::PropertyManager methods
   */
  //@{

  /// Set the default properties to be used by all object groups.
  virtual void set_default_properties (
      const TAO_LoadBalancing::Properties & props,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     TAO_LoadBalancing::InvalidProperty,
                     TAO_LoadBalancing::UnsupportedProperty));

  /// Get the default properties used by all object groups.
  virtual TAO_LoadBalancing::Properties * get_default_properties (
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Remove default properties.
  virtual void remove_default_properties (
      const TAO_LoadBalancing::Properties & props,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     TAO_LoadBalancing::InvalidProperty,
                     TAO_LoadBalancing::UnsupportedProperty));

  /// Set properties associated with a given Replica type.  These
  /// properties override the default properties.
  virtual void set_type_properties (
      const char * type_id,
      const TAO_LoadBalancing::Properties & overrides,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     TAO_LoadBalancing::InvalidProperty,
                     TAO_LoadBalancing::UnsupportedProperty));

  /**
   * Return the properties associated with a give Replica type.  These
   * properties include the type-specific properties in use, in
   * addition to the default properties that were not overridden.
   */
  virtual TAO_LoadBalancing::Properties * get_type_properties (
      const char * type_id,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Remove the given properties associated with the Replica type ID.
  virtual void remove_type_properties (
      const char * type_id,
      const TAO_LoadBalancing::Properties & props,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     TAO_LoadBalancing::InvalidProperty,
                     TAO_LoadBalancing::UnsupportedProperty));

  /**
   * Dynamically set the properties associated with a given object
   * group as the load balancer and replicas are being executed.
   * These properties override the type-specific and default
   * properties.
   */
  virtual void set_properties_dynamically (
      TAO_LoadBalancing::ObjectGroup_ptr object_group,
      const TAO_LoadBalancing::Properties & overrides,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     TAO_LoadBalancing::ObjectGroupNotFound,
                     TAO_LoadBalancing::InvalidProperty,
                     TAO_LoadBalancing::UnsupportedProperty));

  /**
   * Return the properties currently in use by the given object
   * group.  These properties include those that were set dynamically,
   * type-specific properties that weren't overridden, properties that
   * were used when the Replica was created, and default properties
   * that weren't overridden.
   */
  virtual TAO_LoadBalancing::Properties * get_properties (
      TAO_LoadBalancing::ObjectGroup_ptr object_group,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     TAO_LoadBalancing::ObjectGroupNotFound));

  //@}

  /**
   * @name TAO_LoadBalancer::ObjectGroupManager methods
   */
  //@{

  /// Create a member using the load balancer ObjectGroupManager, and
  /// add the created object to the ObjectGroup.
  virtual TAO_LoadBalancing::ObjectGroup_ptr create_member (
      TAO_LoadBalancing::ObjectGroup_ptr object_group,
      const TAO_LoadBalancing::Location & the_location,
      const char * type_id,
      const TAO_LoadBalancing::Criteria & the_criteria,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     TAO_LoadBalancing::ObjectGroupNotFound,
                     TAO_LoadBalancing::MemberAlreadyPresent,
                     TAO_LoadBalancing::NoFactory,
                     TAO_LoadBalancing::ObjectNotCreated,
                     TAO_LoadBalancing::InvalidCriteria,
                     TAO_LoadBalancing::CannotMeetCriteria));

  /// Add an existing object to the ObjectGroup.
  virtual TAO_LoadBalancing::ObjectGroup_ptr add_member (
      TAO_LoadBalancing::ObjectGroup_ptr object_group,
      const TAO_LoadBalancing::Location & the_location,
      CORBA::Object_ptr member,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     TAO_LoadBalancing::ObjectGroupNotFound,
                     TAO_LoadBalancing::MemberAlreadyPresent,
                     TAO_LoadBalancing::ObjectNotAdded));

  /**
   * Remove an object at a specific location from the given
   * ObjectGroup.  Deletion of application created objects must be
   * deleted by the application.  Objects created by the
   * infrastructure (load balancer) will be deleted by the
   * infrastructure.
   */
  virtual TAO_LoadBalancing::ObjectGroup_ptr remove_member (
      TAO_LoadBalancing::ObjectGroup_ptr object_group,
      const TAO_LoadBalancing::Location & the_location,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     TAO_LoadBalancing::ObjectGroupNotFound,
                     TAO_LoadBalancing::MemberNotFound));

  /// Return the locations of the members in the given ObjectGroup.
  virtual TAO_LoadBalancing::Locations * locations_of_members (
      TAO_LoadBalancing::ObjectGroup_ptr object_group,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     TAO_LoadBalancing::ObjectGroupNotFound));

  /// Return the ObjectGroupId for the given ObjectGroup.
  virtual TAO_LoadBalancing::ObjectGroupId get_object_group_id (
      TAO_LoadBalancing::ObjectGroup_ptr object_group,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     TAO_LoadBalancing::ObjectGroupNotFound));

  /// @note Does this method make sense for load balanced objects?
  virtual TAO_LoadBalancing::ObjectGroup_ptr get_object_group_ref (
      TAO_LoadBalancing::ObjectGroup_ptr object_group,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     TAO_LoadBalancing::ObjectGroupNotFound));

  /// Return the reference corresponding to the Replica of a given
  /// ObjectGroup at the given location.
  virtual CORBA::Object_ptr get_member_ref (
      TAO_LoadBalancing::ObjectGroup_ptr object_group,
      const TAO_LoadBalancing::Location & loc,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     TAO_LoadBalancing::ObjectGroupNotFound,
                     TAO_LoadBalancing::MemberNotFound));

  //@}

  /**
   * @name TAO_LoadBalancer::GenericFactory methods
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
      const TAO_LoadBalancing::Criteria & the_criteria,
      TAO_LoadBalancing::GenericFactory::FactoryCreationId_out
        factory_creation_id,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     TAO_LoadBalancing::NoFactory,
                     TAO_LoadBalancing::ObjectNotCreated,
                     TAO_LoadBalancing::InvalidCriteria,
                     TAO_LoadBalancing::InvalidProperty,
                     TAO_LoadBalancing::CannotMeetCriteria));

  /**
   * Delete the object corresponding to the provided
   * FactoryCreationId.  If the object is actually an ObjectGroup,
   * then all members within the ObjectGroup will be deleted.
   * Afterward, the ObjectGroup itself will be deleted.
   */
  virtual void delete_object (
      const TAO_LoadBalancing::GenericFactory::FactoryCreationId &
        factory_creation_id,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     TAO_LoadBalancing::ObjectNotFound));

  //@}

private:

  /// Helper method that creates replicas of the given type.
  void create_object_i (
    const char * type_id,
    CORBA::UShort initial_number_replicas,
    TAO_LoadBalancing::FactoryInfos &factory_infos,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::NoFactory,
                   TAO_LoadBalancing::ObjectNotCreated,
                   TAO_LoadBalancing::InvalidCriteria,
                   TAO_LoadBalancing::InvalidProperty,
                   TAO_LoadBalancing::CannotMeetCriteria));

  /// Extract the value of the InitialNumberReplicas property from
  /// the_criteria.
  int get_initial_number_replicas (
    const char *type_id,
    const TAO_LoadBalancing::Criteria &the_criteria,
    CORBA::UShort &initial_number_replicas) const;

  /**
   * Extract the value of the Factories property from the_criteria.
   * This method ensures that the locations in the returned
   * FactoryInfos are unique.  This is necessary to ensure that only
   * one replica of a given type is created by the load balancer at a
   * given location.
   */
  int get_factory_infos (
    const char *type_id,
    const TAO_LoadBalancing::Criteria &the_criteria,
    TAO_LoadBalancing::FactoryInfos &factory_infos) const;

  /// Create a POA with the appropriate policies to support
  /// ServantLocators (i.e. the ReplicaLocator).
  int init (PortableServer::POA_ptr root_poa);

  /**
   * Get a new ObjectId to be used when creating a new ObjectGroup.
   * An ObjectId created by this method will never be reused within
   * the scope of a given ReplicationManager.  A value suitable for
   * use in a map association <ext_id> is also returned.
   */
  void get_ObjectId (PortableServer::ObjectId &oid,
                     TAO_LB_ObjectGroup_EXT_ID &ext_id);

  /// Assignment operator for TAO_LoadBalancer::FactoryInfo instances.
  void operator= (TAO_LoadBalancer::FactoryInfo &lhs,
                  const TAO_LoadBalancer::FactoryInfo &rhs);

private:

  /// The object that tells the invoking client to forward its requests
  /// from the LoadBalancer to an actual replica.
  TAO_LB_ReplicaLocator locator_;

  /// The POA that dispatches requests to the ReplicaLocator.
  PortableServer::POA_var poa_;

  /// ObjectId to be used for the next ObjectGroup that is created.
  CORBA::ULong next_oid_;

  /// Map between RepositoryId, ObjectGroup reference, replica list and
  /// factory creation ID.
  TAO_LB_ObjectGroup_Map object_group_map_;

  /**
   * Value that is used when assigning a FactoryCreationId to the
   * factory that was used to create a given ObjectGroup.  The
   * FactoryCreationId is typically comprised of this value in
   * addition to another value that makes it unique to a given Load
   * Balancer.
   */
  CORBA::ULong next_factory_num_;

};

#include "ace/post.h"

#endif  /* LOADBALANCINGI_H */

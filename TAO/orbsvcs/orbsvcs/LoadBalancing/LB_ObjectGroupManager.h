// -*- C++ -*-

//=============================================================================
/**
 * @file LB_ObjectGroupManager.h
 *
 * $Id$
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_LB_OBJECT_GROUPMANAGER_H
#define TAO_LB_OBJECT_GROUPMANAGER_H

#include "ace/pre.h"

#include "orbsvcs/LoadBalancingC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "LB_ObjectGroup_Map.h"

/// Forward declarations.
class TAO_LB_PropertyManager;

/**
 * @class TAO_LB_ObjectGroupManager
 *
 * @brief LoadBalancing::ObjectGroupManager implementation.
 *
 * The ObjectGroupManager provides the interface necessary to
 * facilitate application-controlled object group membership.
 */
class TAO_LB_ObjectGroupManager
  : public virtual LoadBalancing::ObjectGroupManager
{
public:

  /// Constructor.
  TAO_LB_ObjectGroupManager (TAO_LB_PropertyManager &property_manager,
                             TAO_LB_ObjectGroup_Map &map);

  /**
   * @name TAO_LoadBalancer::ObjectGroupManager methods
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
  /// @note Does this method make sense for load balanced objects?
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
   * @name Helper methods used internally by the load balancer's
   *       GenericFactory.
   */
  //@{

  /// Create a member using the load balancer ObjectGroupManager, and
  /// add the created object to the ObjectGroup.
  LoadBalancing::ObjectGroup_ptr create_member_i (
      LoadBalancing::ObjectGroup_ptr object_group,
      const LoadBalancing::Location & the_location,
      const char * type_id,
      const LoadBalancing::Criteria & the_criteria,
      CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     LoadBalancing::MemberAlreadyPresent,
                     LoadBalancing::NoFactory,
                     LoadBalancing::ObjectNotCreated,
                     LoadBalancing::InvalidCriteria,
                     LoadBalancing::CannotMeetCriteria));

  //@}

  /// Set the reference to the POA that created the object group
  /// references passed to the ObjectGroupManager methods.
  void poa (PortableServer::POA_ptr poa);

private:

  /// Obtain the ObjectGroup hash map entry corresponding to the given
  /// ObjectGroup reference.
  TAO_LB_ObjectGroup_Map_Entry *get_group_entry (
      LoadBalancing::ObjectGroup_ptr object_group,
      CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((LoadBalancing::ObjectGroupNotFound));

private:

  /// Reference to the POA that created the object group references
  /// passed to the ObjectGroupManager methods.
  PortableServer::POA_var poa_;

  /// Reference to the PropertyManager.
  TAO_LB_PropertyManager &property_manager_;

  /// Pointer to the ObjectGroup map.
  TAO_LB_ObjectGroup_Map &object_group_map_;

  /// Map that contains list of all replicas at a given location.
  TAO_LB_Location_Map location_map_;
};

#endif  /* TAO_LB_OBJECT_GROUP_MANAGER_H */

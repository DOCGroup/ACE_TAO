// -*- C++ -*-

//=============================================================================
/**
 * @file ObjectGroupManager.h
 *
 * $Id$
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_LB_OBJECT_GROUPMANAGER_H
#define TAO_LB_OBJECT_GROUPMANAGER_H

#include "ace/pre.h"

#include "Loadbalancing_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


/**
 * @class TAO_LB_ObjectGroupManager
 *
 * @brief 
 *
 */
class TAO_LoadBalancing_Export TAO_LB_ObjectGroupManager
  : public virtual POA_TAO_LoadBalancing::ObjectGroupManager
{
public:

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

private:

  // @@ NEED MAP FOR ObjectId TO ObjectGroup
  // @@ NEED MAP FOR FactoryCreationId TO ObjectGroup

};

#endif  /* TAO_LB_OBJECT_GROUP_MANAGER_H */

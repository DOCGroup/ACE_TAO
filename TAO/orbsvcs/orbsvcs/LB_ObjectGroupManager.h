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

#include "orbsvcs/PortableGroupC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "LB_Location_Map.h"


/// Forward declarations.
class TAO_LB_ObjectGroup_Map;
class TAO_LB_PropertyManager;


/**
 * @class TAO_LB_ObjectGroupManager
 *
 * @brief PortableGroup::ObjectGroupManager implementation.
 *
 * The ObjectGroupManager provides the interface necessary to
 * facilitate application-controlled object group membership.
 */
class TAO_LB_ObjectGroupManager
  : public virtual PortableGroup::ObjectGroupManager
{
public:

  /// Constructor.
  TAO_LB_ObjectGroupManager (TAO_LB_Location_Map &location_map,
                             TAO_LB_ObjectGroup_Map &object_group_map,
                             TAO_LB_PropertyManager &property_manager);

  /**
   * @name PortableGroup::ObjectGroupManager methods
   */
  //@{

  /// Create a member using the load balancer ObjectGroupManager, and
  /// add the created object to the ObjectGroup.
  virtual PortableGroup::ObjectGroup_ptr create_member (
      PortableGroup::ObjectGroup_ptr object_group,
      const PortableGroup::Location & the_location,
      const char * type_id,
      const PortableGroup::Criteria & the_criteria,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableGroup::ObjectGroupNotFound,
                     PortableGroup::MemberAlreadyPresent,
                     PortableGroup::NoFactory,
                     PortableGroup::ObjectNotCreated,
                     PortableGroup::InvalidCriteria,
                     PortableGroup::CannotMeetCriteria));

  /// Add an existing object to the ObjectGroup.
  virtual PortableGroup::ObjectGroup_ptr add_member (
      PortableGroup::ObjectGroup_ptr object_group,
      const PortableGroup::Location & the_location,
      CORBA::Object_ptr member,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableGroup::ObjectGroupNotFound,
                     PortableGroup::MemberAlreadyPresent,
                     PortableGroup::ObjectNotAdded));

  /**
   * Remove an object at a specific location from the given
   * ObjectGroup.  Deletion of application created objects must be
   * deleted by the application.  Objects created by the
   * infrastructure (load balancer) will be deleted by the
   * infrastructure.
   */
  virtual PortableGroup::ObjectGroup_ptr remove_member (
      PortableGroup::ObjectGroup_ptr object_group,
      const PortableGroup::Location & the_location,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableGroup::ObjectGroupNotFound,
                     PortableGroup::MemberNotFound));

  /// Return the locations of the members in the given ObjectGroup.
  virtual PortableGroup::Locations * locations_of_members (
      PortableGroup::ObjectGroup_ptr object_group,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableGroup::ObjectGroupNotFound));

  /// Return the ObjectGroupId for the given ObjectGroup.
  /// @note Does this method make sense for load balanced objects?
  virtual PortableGroup::ObjectGroupId get_object_group_id (
      PortableGroup::ObjectGroup_ptr object_group,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableGroup::ObjectGroupNotFound));

  /// @note Does this method make sense for load balanced objects?
  virtual PortableGroup::ObjectGroup_ptr get_object_group_ref (
      PortableGroup::ObjectGroup_ptr object_group,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableGroup::ObjectGroupNotFound));

  /// Return the reference corresponding to the Replica of a given
  /// ObjectGroup at the given location.
  virtual CORBA::Object_ptr get_member_ref (
      PortableGroup::ObjectGroup_ptr object_group,
      const PortableGroup::Location & loc,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableGroup::ObjectGroupNotFound,
                     PortableGroup::MemberNotFound));

  //@}

  /**
   * @name Helper methods used internally by the load balancer's
   *       GenericFactory.
   */
  //@{

  /// Create a member using the load balancer ObjectGroupManager, and
  /// add the created object to the ObjectGroup.
  PortableGroup::ObjectGroup_ptr create_member_i (
      PortableGroup::ObjectGroup_ptr object_group,
      const PortableGroup::Location & the_location,
      const char * type_id,
      const PortableGroup::Criteria & the_criteria,
      CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableGroup::MemberAlreadyPresent,
                     PortableGroup::NoFactory,
                     PortableGroup::ObjectNotCreated,
                     PortableGroup::InvalidCriteria,
                     PortableGroup::CannotMeetCriteria));

  //@}

private:

  /// Map that contains list of all replicas at a given location, in
  /// addition to the load monitor at that location.
  TAO_LB_Location_Map &location_map_;

  /// Pointer to the ObjectGroup map.
  TAO_LB_ObjectGroup_Map &object_group_map_;

  /// Reference to the PropertyManager.
  TAO_LB_PropertyManager &property_manager_;

};

#endif  /* TAO_LB_OBJECT_GROUP_MANAGER_H */

// -*- C++ -*-

//=============================================================================
/**
 * @file PG_ObjectGroupManager.h
 *
 * $Id$
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_PG_OBJECT_GROUP_MANAGER_H
#define TAO_PG_OBJECT_GROUP_MANAGER_H

#include "ace/pre.h"

#include "orbsvcs/PortableGroupS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "PG_ObjectGroup_Map.h"
#include "PG_Location_Map.h"

#include "tao/PortableServer/Key_Adapters.h"
#include "tao/PortableServer/PortableServerC.h"


/**
 * @class TAO_PG_ObjectGroupManager
 *
 * @brief PortableGroup::ObjectGroupManager implementation.
 *
 * The ObjectGroupManager provides the interface necessary to
 * facilitate application-controlled object group membership.
 */
class TAO_PortableGroup_Export TAO_PG_ObjectGroupManager
  : public virtual POA_PortableGroup::ObjectGroupManager
{
public:

  /// Constructor.
  TAO_PG_ObjectGroupManager (void);

  /// Destructor.
  ~TAO_PG_ObjectGroupManager (void);

  /**
   * @name PortableGroup::ObjectGroupManager methods
   *
   * Methods required by the PortableGroup::ObjectGroupManager
   * interface.
   */
  //@{

  /// Create a member and add it to the given object group.
  virtual PortableGroup::ObjectGroup_ptr create_member (
      PortableGroup::ObjectGroup_ptr object_group,
      const PortableGroup::Location & the_location,
      const char * type_id,
      const PortableGroup::Criteria & the_criteria
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
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
      CORBA::Object_ptr member
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
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
      const PortableGroup::Location & the_location
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableGroup::ObjectGroupNotFound,
                     PortableGroup::MemberNotFound));

  /// Return the locations of the members in the given ObjectGroup.
  virtual PortableGroup::Locations * locations_of_members (
      PortableGroup::ObjectGroup_ptr object_group
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableGroup::ObjectGroupNotFound));

  /// Return the ObjectGroupId for the given ObjectGroup.
  /// @note Does this method make sense for load balanced objects?
  virtual PortableGroup::ObjectGroupId get_object_group_id (
      PortableGroup::ObjectGroup_ptr object_group
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableGroup::ObjectGroupNotFound));

  /// @note Does this method make sense for load balanced objects?
  virtual PortableGroup::ObjectGroup_ptr get_object_group_ref (
      PortableGroup::ObjectGroup_ptr object_group
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableGroup::ObjectGroupNotFound));

  /// Return the reference corresponding to the member of a given
  /// ObjectGroup at the given location.
  virtual CORBA::Object_ptr get_member_ref (
      PortableGroup::ObjectGroup_ptr object_group,
      const PortableGroup::Location & loc
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableGroup::ObjectGroupNotFound,
                     PortableGroup::MemberNotFound));

  //@}

  /// Create object group hash map entry that represents an actual
  /// ObjectGroup.
  /**
   * @note This method is used mainly by the
   *       GenericFactory::create_object() method.
   */
  PortableGroup::ObjectGroup_ptr create_object_group (
    CORBA::ULong group_id,
    const PortableServer::ObjectId &oid,
    const char * type_id,
    const PortableGroup::Criteria & the_criteria
    ACE_ENV_ARG_DECL);

  /// Destroy the object group corresponding to the given ObjectId.
  /**
   * @note This method is used mainly by the
   *       GenericFactory::delete_object() method.
   */
  void destroy_object_group (const PortableServer::ObjectId & oid
                             ACE_ENV_ARG_DECL);

  /// Return the properties set when the object group was created, and
  /// the dynamic properties that may have overridden them.
  PortableGroup::Properties * get_properties (
      PortableGroup::ObjectGroup_ptr object_group
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableGroup::ObjectGroupNotFound));

  /// Return the type_id for the given object group.
  char * type_id (PortableGroup::ObjectGroup_ptr object_group
                  ACE_ENV_ARG_DECL);

  /// Return the object group associated with the given ObjectId.
  /**
   * @return Returns PortableGroup::ObjectGroup::_nil() if no object
   *         group corresponding to the given ObjectId exists.
   */
  PortableGroup::ObjectGroup_ptr object_group (
    const PortableServer::ObjectId & oid);

  /// Set the POA to use when converting object group references to
  /// ObjectIds.
  void poa (PortableServer::POA_ptr p);

protected:

  /// Obtain the ObjectGroup hash map entry corresponding to the given
  /// ObjectGroup reference.
  TAO_PG_ObjectGroup_Map_Entry * get_group_entry (
      PortableGroup::ObjectGroup_ptr object_group
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableGroup::ObjectGroupNotFound));


  /// Check if a member resides at the location to which the group
  /// array belongs to.
  /**
   * A member is actually represented by the object group to which it
   * belongs.  In this implementation, a pointer to a object group
   * hash map entry is associated with a given a location.
   */
  CORBA::Boolean member_already_present (
    const TAO_PG_ObjectGroup_Array & groups,
    TAO_PG_ObjectGroup_Map_Entry * group_entry);

  /// Verify that the member type_id matches the object group type_id.
  /**
   * @todo Strategize this -- e.g. strict type checking.
   */
  CORBA::Boolean valid_type_id (
    PortableGroup::ObjectGroup_ptr object_group,
    TAO_PG_ObjectGroup_Map_Entry * group_entry,
    CORBA::Object_ptr member
    ACE_ENV_ARG_DECL);

private:

  /// Reference to the POA that created the object group references.
  PortableServer::POA_var poa_;

  /// The underlying table that contains all object group
  /// information.
  TAO_PG_ObjectGroup_Map object_group_map_;

  /// Map that contains list of all members at a given location, in
  /// addition to the load monitor at that location.
  TAO_PG_Location_Map location_map_;

  /// Lock used to synchronize access to the underlying tables.
  TAO_SYNCH_MUTEX lock_;

};


#include "ace/post.h"

#endif  /* TAO_PG_OBJECT_GROUP_MANAGER_H */

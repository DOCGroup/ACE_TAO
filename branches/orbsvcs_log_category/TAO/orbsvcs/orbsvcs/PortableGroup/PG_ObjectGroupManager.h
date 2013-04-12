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

#include /**/ "ace/pre.h"

#include "orbsvcs/PortableGroup/portablegroup_export.h"
#include "orbsvcs/PortableGroupS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/PortableGroup/PG_ObjectGroup_Map.h"
#include "orbsvcs/PortableGroup/PG_Location_Map.h"

#include "tao/PortableServer/Key_Adapters.h"
#include "tao/PortableServer/PortableServerC.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/// Forward declarations
class TAO_PG_GenericFactory;


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
      const PortableGroup::Criteria & the_criteria);

  /// Add an existing object to the ObjectGroup.
  virtual PortableGroup::ObjectGroup_ptr add_member (
      PortableGroup::ObjectGroup_ptr object_group,
      const PortableGroup::Location & the_location,
      CORBA::Object_ptr member);

  /**
   * Remove an object at a specific location from the given
   * ObjectGroup.  Deletion of application created objects must be
   * deleted by the application.  Objects created by the
   * infrastructure (load balancer) will be deleted by the
   * infrastructure.
   */
  virtual PortableGroup::ObjectGroup_ptr remove_member (
      PortableGroup::ObjectGroup_ptr object_group,
      const PortableGroup::Location & the_location);

  /// Return the locations of the members in the given ObjectGroup.
  virtual PortableGroup::Locations * locations_of_members (
      PortableGroup::ObjectGroup_ptr object_group);

  /// Return the locations of the members in the given ObjectGroup.
  virtual PortableGroup::ObjectGroups * groups_at_location (
      const PortableGroup::Location & the_location);

  /// Return the ObjectGroupId for the given ObjectGroup.
  /// @note Does this method make sense for load balanced objects?
  virtual PortableGroup::ObjectGroupId get_object_group_id (
      PortableGroup::ObjectGroup_ptr object_group);

  /// @note Does this method make sense for load balanced objects?
  virtual PortableGroup::ObjectGroup_ptr get_object_group_ref (
      PortableGroup::ObjectGroup_ptr object_group);

  /// Return the reference corresponding to the member of a given
  /// ObjectGroup at the given location.
  virtual CORBA::Object_ptr get_member_ref (
      PortableGroup::ObjectGroup_ptr object_group,
      const PortableGroup::Location & loc);
  /**
   * TAO-specific extension.
   * Return the ObjectGroup reference for the given ObjectGroupId.
   */
   virtual PortableGroup::ObjectGroup_ptr get_object_group_ref_from_id (
        PortableGroup::ObjectGroupId group_id
      );

  //@}

  /// TAO-specific member addition method.
  /**
   * This method is meant to be invoked by TAO's GenericFactory
   * implementation.  It is designed to allow only certain exceptions
   * to be propagated to the caller, and to prevent redundant remote
   * RepositoryId checking.
   */
  PortableGroup::ObjectGroup_ptr _tao_add_member (
      PortableGroup::ObjectGroup_ptr object_group,
      const PortableGroup::Location & the_location,
      CORBA::Object_ptr member,
      const char * type_id,
      const CORBA::Boolean propagate_member_already_present);

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
    const PortableGroup::Criteria & the_criteria);

  /// Destroy the object group corresponding to the given ObjectId.
  /**
   * @note This method is used mainly by the
   *       GenericFactory::delete_object() method.
   */
  void destroy_object_group (const PortableServer::ObjectId & oid);

  /// Return the properties set when the object group was created, and
  /// the dynamic properties that may have overridden them.
  PortableGroup::Properties * get_properties (
      PortableGroup::ObjectGroup_ptr object_group);

  /// Return the type_id for the given object group.
  char * type_id (PortableGroup::ObjectGroup_ptr object_group);

  /// Return the object group associated with the given ObjectId.
  /**
   * @return Returns PortableGroup::ObjectGroup::_nil() if no object
   *         group corresponding to the given ObjectId exists.
   */
  PortableGroup::ObjectGroup_ptr object_group (
    const PortableServer::ObjectId & oid);

  /// Return the number of members in the given object group.
  CORBA::ULong member_count (PortableGroup::ObjectGroup_ptr group);

  /// Set the POA to use when converting object group references to
  /// ObjectIds.
  void poa (PortableServer::POA_ptr p);

  /// Set the pointer to the GenericFactory associated with this
  /// ObjectGroupManager.
  /**
   * The TAO_PG_GenericFactory will only be used when
   * ObjectGroupManager::remove_member() is explicitly called so that
   * the infrastructure may be given an opportunity to clean up any
   * object group members it may have created.
   */
  void generic_factory (TAO_PG_GenericFactory * generic_factory);

  /// Get active or inactive member count in a group.
  size_t member_count (
    const PortableServer::ObjectId & oid, bool is_active);

  /// Verify if the member is active from local cache.
  bool is_alive (
    const PortableServer::ObjectId & oid,
    CORBA::Object_ptr member);

  /// Remove the inactive members.
  void remove_inactive_members ();

  /// Validate all active members.
  void validate_members (CORBA::ORB_ptr orb, const TimeBase::TimeT& timeout);

protected:

  /// Underlying and non-locking implementation of the add_member()
  /// and _tao_add_member() methods in this class.
  PortableGroup::ObjectGroup_ptr add_member_i (
    PortableGroup::ObjectGroup_ptr object_group,
    const PortableGroup::Location & the_location,
    CORBA::Object_ptr member,
    const CORBA::Boolean check_type_id);

  /// Obtain the ObjectGroup hash map entry corresponding to the given
  /// ObjectGroup reference.
  TAO_PG_ObjectGroup_Map_Entry * get_group_entry (
      PortableGroup::ObjectGroup_ptr object_group);


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

  /// get the position of the object_group_map_entry
  /// -1 indicate group is not found.
  int get_object_group_position (
    const TAO_PG_ObjectGroup_Array & groups,
    TAO_PG_ObjectGroup_Map_Entry * group_entry);

  /// Verify that the member type_id matches the object group type_id.
  /**
   * @todo Strategize this -- e.g. strict type checking.
   */
  CORBA::Boolean valid_type_id (
    PortableGroup::ObjectGroup_ptr object_group,
    TAO_PG_ObjectGroup_Map_Entry * group_entry,
    CORBA::Object_ptr member);

  /// Remove references to the object group from location map. There may be
  /// multiple of them.
  int remove_group_from_location_map (TAO_PG_ObjectGroup_Map_Entry * group_entry);

  /// Remove the entry from the group array and return the new size.
  size_t remove_entry_from_groups (int to_be_removed,
                                   TAO_PG_ObjectGroup_Array * groups);

private:

  /// Help function to get active or inactive member infos.
  TAO_PG_MemberInfo_Set get_members (bool is_alive);

  /// Ping the remote to verify the connections.
  bool ping (CORBA::ORB_ptr orb,
             CORBA::Object_var& obj,
             const TimeBase::TimeT& tt);

  /// Reference to the POA that created the object group references.
  PortableServer::POA_var poa_;

  /// The underlying table that contains all object group
  /// information.
  TAO_PG_ObjectGroup_Map object_group_map_;

  /// Map that contains list of all members at a given location, in
  /// addition to the load monitor at that location.
  TAO_PG_Location_Map location_map_;

  /// Pointer to the TAO_PG_GenericFactory class responsible for
  /// object group creation/destruction.
  TAO_PG_GenericFactory * generic_factory_;

  /// Lock used to synchronize access to the underlying tables.
  TAO_SYNCH_MUTEX lock_;

  /// The list of inactive members.
  TAO_PG_MemberInfo_Set inactive_members_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_PG_OBJECT_GROUP_MANAGER_H */

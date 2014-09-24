// -*- C++ -*-

//=============================================================================
/**
 * @file PG_Object_Group_Manipulator.h
 *
 * $Id$
 *
 * @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================


#ifndef TAO_PG_OBJECT_GROUP_MANIPULATOR_H
#define TAO_PG_OBJECT_GROUP_MANIPULATOR_H

#include /**/ "ace/pre.h"

#include "orbsvcs/PortableGroup/portablegroup_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/PortableGroupS.h"

#include "tao/IORManipulation/IORManip_Loader.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{

  /**
   * @class TAO::PG_Object_Group_Manipulator
   *
   * @brief PortableGroup::ObjectGroupManager implementation.
   *
   * The ObjectGroupManager provides the interface necessary to
   * facilitate application-controlled object group membership.
   */
  class TAO_PortableGroup_Export PG_Object_Group_Manipulator
  {
  public:

    /// Constructor.
    PG_Object_Group_Manipulator ();

    /// Destructor.
    ~PG_Object_Group_Manipulator ();

    /**
     * Initializes the group creator.
     */
    void init (CORBA::ORB_ptr orb,
               PortableServer::POA_ptr poa);

    /**
     * Create an empty object group.
     */
    PortableGroup::ObjectGroup_ptr create_object_group (
      const char * type_id,
      const char * domain_id,
      PortableGroup::ObjectGroupId & group_id);

    /**
     * Create an empty object group using a specific ID.
     * Note that this should not also be used with create_object_group ()
     * as the next object group ID to use will not necessarily be unique.
     */
    PortableGroup::ObjectGroup_ptr create_object_group_using_id (
      const char * type_id,
      const char * domain_id,
      const PortableGroup::ObjectGroupId & group_id);

    PortableGroup::ObjectGroup_ptr remove_profiles (
      PortableGroup::ObjectGroup_ptr group,
      PortableGroup::ObjectGroup_ptr profile) const;

    PortableGroup::ObjectGroup_ptr merge_iors (
      TAO_IOP::TAO_IOR_Manipulation::IORList & iors) const;

    int set_primary (
      TAO_IOP::TAO_IOR_Property * prop,
      PortableGroup::ObjectGroup_ptr reference,
      CORBA::Object_ptr new_primary) const;

  void dump_membership (const char * label,
                        PortableGroup::ObjectGroup_ptr member) const;

  private:

    /**
     * Allocate an ogid for a new object group
     */
    void allocate_ogid (PortableGroup::ObjectGroupId & ogid);

    /**
     * convert numeric OGID to Sequence<Octet> oid
     */
    PortableServer::ObjectId *
      convert_ogid_to_oid (PortableGroup::ObjectGroupId ogid) const;

  private:

    /// The orb
    CORBA::ORB_var orb_;

    /// Reference to the POA that created the object group references.
    PortableServer::POA_var poa_;

    /// The ORBs IORManipulation object
    TAO_IOP::TAO_IOR_Manipulation_var iorm_;

    /// Lock used to synchronize access to next_ogid_.
    TAO_SYNCH_MUTEX lock_ogid_;

    /// Next ogid to be allocated.
    PortableGroup::ObjectGroupId next_ogid_;

  };
} //namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO::PG_OBJECT_GROUP_CREATOR_H */

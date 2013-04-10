// -*- C++ -*-

//=============================================================================
/**
 * @file  PG_Group_Factory.h
 *
 * $Id$
 *
 * @author Dale Wilson <wilson_d@ociweb.com>
 * @author Byron Harris <harrisb@ociweb.com>
 */
//=============================================================================

#ifndef TAO_PG_GROUP_FACTORY_H
#define TAO_PG_GROUP_FACTORY_H

#include /**/ "ace/pre.h"

#include "orbsvcs/PortableGroup/portablegroup_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/PortableGroup/PG_Object_Group_Manipulator.h"
#include "orbsvcs/PortableGroup/PG_Property_Set.h"
#include "orbsvcs/PortableGroupC.h"

#include "tao/PortableServer/PortableServer.h"

#include "ace/ACE.h"
#include "ace/Hash_Map_Manager.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

//////////////////
// Forward references
namespace TAO
{
  class PG_Group_List_Store;
  class PG_Object_Group_Storable;
  class Storable_Factory;
}

namespace TAO
{
  /////////////////////
  // forward references
  class PG_Object_Group;

  /**
   * class PG_Group_Factory
   */
  class TAO_PortableGroup_Export PG_Group_Factory
  {
    ////////////////////////////////////////////////////////////
    // typedef private implementation classes
    typedef ACE_Hash_Map_Manager_Ex <
        PortableGroup::ObjectGroupId,
        ::TAO::PG_Object_Group *,
        ACE_Hash<ACE_UINT64>,
        ACE_Equal_To<ACE_UINT64>,
         TAO_SYNCH_MUTEX> Group_Map;

    typedef ACE_Hash_Map_Entry <
      PortableGroup::ObjectGroupId,
      ::TAO::PG_Object_Group *> Group_Map_Entry;

    typedef ACE_Hash_Map_Iterator_Ex <
      PortableGroup::ObjectGroupId,
      ::TAO::PG_Object_Group *,
      ACE_Hash<ACE_UINT64>,
      ACE_Equal_To<ACE_UINT64>,
       TAO_SYNCH_MUTEX> Group_Map_Iterator;

  public:

    /// Constructor.
    PG_Group_Factory ();

    /// Destructor.
    virtual ~PG_Group_Factory ();

    void init (
      CORBA::ORB_ptr orb,
      PortableServer::POA_ptr poa,
      PortableGroup::FactoryRegistry_ptr factory_registry);


    TAO::PG_Object_Group * create_group (
        const char * type_id,
        const PortableGroup::Criteria & the_criteria,
        const TAO::PG_Property_Set_var & typeid_properties);


    void delete_group (PortableGroup::ObjectGroup_ptr object_group);


    void delete_group (PortableGroup::ObjectGroupId group_id);

    PortableGroup::ObjectGroups *
    groups_at_location (
        const PortableGroup::Location & the_location);

    /**
     * return all groups in the factory
     */
    PortableGroup::ObjectGroups *
    all_groups (void);


    /**
     * insert existing group.  Take ownership
     * note: uses group id extracted from group object
     * @return bool true if insertion successful
     */
    int insert_group (::TAO::PG_Object_Group * group);

    /**
     * insert group.  Take ownership
     * @return bool true if insertion successful
     */
    int insert_group (
      PortableGroup::ObjectGroupId group_id,
      ::TAO::PG_Object_Group * group);

    /**
     * find group
     * @return bool true if found
     */
    int find_group (
      PortableGroup::ObjectGroupId group_id,
      ::TAO::PG_Object_Group *& group);

    /**
     * find group
     * note: uses group id extracted from object_group
     * @return bool true if found
     */
    int find_group (
      PortableGroup::ObjectGroup_ptr object_group,
      ::TAO::PG_Object_Group *& group);

    /**
     * find group with the property with the designated value
     * @return bool true if found
     */
    int find_group_with_name (
      const char* group_name,
      ::TAO::PG_Object_Group *& group);

    /**
     * remove group from map and delete it.
     * @return bool true if found
     */
    int destroy_group (PortableGroup::ObjectGroupId object_group_id);

    /**
     * remove group from map and delete it.
     * note: uses group id extracted from object_group
     * @return bool true if found
     */
    int destroy_group (PortableGroup::ObjectGroup_ptr object_group);

    /**
     * persist internal state to file for fault tolerant purposes.
     */
    void set_object_group_storable_factory (TAO::Storable_Factory * factory);

  protected:

    /**
     * Factory function to create a storable object object from
     * scratch.
     */
    virtual PG_Object_Group_Storable * create_persistent_group (
      CORBA::ORB_ptr orb,
      PortableGroup::FactoryRegistry_ptr factory_registry,
      TAO::PG_Object_Group_Manipulator & manipulator,
      CORBA::Object_ptr empty_group,
      const PortableGroup::TagGroupTaggedComponent & tagged_component,
      const char * type_id,
      const PortableGroup::Criteria & the_criteria,
      const TAO::PG_Property_Set_var & type_properties,
      TAO::Storable_Factory & storable_factory);

    /**
     * Factory function to restore an object group from
     * persistent store.
     */
    virtual PG_Object_Group_Storable * restore_persistent_group (
      PortableGroup::ObjectGroupId group_id,
      CORBA::ORB_ptr orb,
      PortableGroup::FactoryRegistry_ptr factory_registry,
      TAO::PG_Object_Group_Manipulator & manipulator,
      TAO::Storable_Factory & storable_factory);

    bool use_persistence_;

    PG_Group_List_Store * list_store_;

  private:

    CORBA::ORB_var orb_;

    /// Reference to the POA used to create object group references.
    PortableServer::POA_var poa_;

    /// The factory registry for replica factories
    PortableGroup::FactoryRegistry_var factory_registry_;

    ::TAO::PG_Object_Group_Manipulator manipulator_;

    const char * domain_id_;

    /**
     * If persistence is being used, update the map as
     * necessary based on what's in the group list store.
     */
    Group_Map & get_group_map ();

    Group_Map group_map_;

    ///// Support for object group persistent /////

    // Lazily read groups from store
    bool groups_read_;
    Storable_Factory * storable_factory_;

  };
} // namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_PG_GROUP_FACTORY_H */

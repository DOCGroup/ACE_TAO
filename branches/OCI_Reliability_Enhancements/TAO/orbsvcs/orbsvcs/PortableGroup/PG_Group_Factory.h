// -*- C++ -*-

//=============================================================================
/**
 * @file  PG_Group_Factory.h
 *
 * $Id$
 *
 * @author Dale Wilson <wilson_d@ociweb.com>
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

#include "orbsvcs/PortableGroupC.h"

#include "tao/PortableServer/PortableServer.h"

#include "ace/ACE.h"
#include "ace/Hash_Map_Manager.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

//////////////////
// Forward reference
namespace TAO
{
  class PG_Property_Set;
} // namespace TAO_PG

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
    typedef ACE_Hash_Map_Manager_Ex<
        PortableGroup::ObjectGroupId,
        ::TAO::PG_Object_Group *,
        ACE_Hash<ACE_UINT64>,
        ACE_Equal_To<ACE_UINT64>,
         TAO_SYNCH_MUTEX> Group_Map;

    typedef ACE_Hash_Map_Entry <PortableGroup::ObjectGroupId, ::TAO::PG_Object_Group *> Group_Map_Entry;

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
    ~PG_Group_Factory ();

    void init (
      CORBA::ORB_ptr orb,
      PortableServer::POA_ptr poa,
      PortableGroup::FactoryRegistry_ptr factory_registry);


    TAO::PG_Object_Group * create_group (
        const char * type_id,
        const PortableGroup::Criteria & the_criteria,
        TAO::PG_Property_Set * typeid_properties);


    void delete_group (PortableGroup::ObjectGroup_ptr object_group);


    void delete_group (PortableGroup::ObjectGroupId group_id);

    PortableGroup::ObjectGroups *
    groups_at_location (
        const PortableGroup::Location & the_location);



    /**
     * insert existing group.  Take ownership
     * note: uses group id extracted from group object
     * @return bool true if insertion successful
     */
    int insert_group ( ::TAO::PG_Object_Group * group);

    /**
     * insert group.  Take ownership
     * @return bool true if insertion successful
     */
    int insert_group (PortableGroup::ObjectGroupId group_id, ::TAO::PG_Object_Group * group);

    /**
     * find group
     * @return bool true if found
     */
    int find_group (PortableGroup::ObjectGroupId group_id, ::TAO::PG_Object_Group *& group) const;

    /**
     * find group
     * note: uses group id extracted from object_group
     * @return bool true if found
     */
    int find_group (PortableGroup::ObjectGroup_ptr object_group, ::TAO::PG_Object_Group *& group) const;

    /**
     * find group with the property with the designated value
     * @return bool true if found
     */
    int find_group (const PortableGroup::Property&, ::TAO::PG_Object_Group *& group);

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

  private:

  private:

    CORBA::ORB_var orb_;

    /// Reference to the POA used to create object group references.
    PortableServer::POA_var poa_;

    /// The factory registry for replica factories
    PortableGroup::FactoryRegistry_var factory_registry_;

    ::TAO::PG_Object_Group_Manipulator manipulator_;

    const char * domain_id_;

    Group_Map group_map_;


  };
} // namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_PG_GROUP_FACTORY_H */

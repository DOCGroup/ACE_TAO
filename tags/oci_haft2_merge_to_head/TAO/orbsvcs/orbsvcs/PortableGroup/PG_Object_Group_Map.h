// -*- C++ -*-

//=============================================================================
/**
 * @file PG_Object_Group_Map.h
 *
 * $Id$
 *
 *  Container for TAO::PG_Object_Groups indexed by ObjectGroupId
 *  Note this was developed as part of FT CORBA
 *  @@ For now its but one of several parallel containers for object group information
 *  @@ TODO: Combine these parallel containers into a single one that consolidates everything
 *  @@ known about object groups.
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================


#ifndef TAO_PG_OBJECT_GROUP_MAP_H
#define TAO_PG_OBJECT_GROUP_MAP_H

#include /**/ "ace/pre.h"
#include "orbsvcs/PortableGroupS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

////////////////////////////////////////////////////
// Forward reference to class(es) defined in this header
namespace TAO
{
  class PG_Object_Group_Map;
} // namespace TAO

//#include "PG_Object_Group.h"
namespace TAO
{
  class PG_Object_Group;
} // namespace TAO

namespace TAO
{
  /**
   * class TAO::PG_Object_Group_Map
   *  A container of Object_Groups indexed by ObjectGroupId
   */
  class TAO_PortableGroup_Export PG_Object_Group_Map
    : public ACE_Hash_Map_Manager_Ex<
        PortableGroup::ObjectGroupId,
        ::TAO::PG_Object_Group *,
        ACE_Hash<ACE_UINT64>,
        ACE_Equal_To<ACE_UINT64>,
         TAO_SYNCH_MUTEX>
  {
  public:
    typedef ACE_Hash_Map_Entry <PortableGroup::ObjectGroupId, ::TAO::PG_Object_Group *> Entry;
    typedef ACE_Hash_Map_Iterator_Ex <
      PortableGroup::ObjectGroupId,
      ::TAO::PG_Object_Group *,
      ACE_Hash<ACE_UINT64>,
      ACE_Equal_To<ACE_UINT64>,
       TAO_SYNCH_MUTEX> Iterator;

    /// Constructor.
    PG_Object_Group_Map ();

    /// Destructor.
    virtual ~PG_Object_Group_Map ();

    /// insert existing group.  Take ownership
    /// note: uses group id extracted from group object
    /// @return bool true if insertion successful
    int insert_group ( ::TAO::PG_Object_Group * group);

    /// insert group.  Take ownership
    /// @return bool true if insertion successful
    int insert_group (PortableGroup::ObjectGroupId group_id, ::TAO::PG_Object_Group * group);

    /// find group
    /// @return bool true if found
    int find_group (PortableGroup::ObjectGroupId group_id, ::TAO::PG_Object_Group *& group) const;

    /// find group
    /// note: uses group id extracted from object_group
    /// @return bool true if found
    int find_group (PortableGroup::ObjectGroup_ptr object_group, ::TAO::PG_Object_Group *& group) const;

    /// remove group from map and delete it.
    /// @return bool true if found
    int destroy_group (PortableGroup::ObjectGroupId object_group_id);

    /// remove group from map and delete it.
    /// note: uses group id extracted from object_group
    /// @return bool true if found
    int destroy_group (PortableGroup::ObjectGroup_ptr object_group);

    ////////////////////////////
    // Implementation methods
   private:

    ///////
    // Data
   private:
  };
} // namespace TAO

#include /**/ "ace/post.h"

#endif  /* TAO_PG_OBJECT_GROUP_MAP_H */

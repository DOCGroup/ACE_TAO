// -*- C++ -*-
//
//=============================================================================
/**
 * @file PG_Object_Group_Map.cpp
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

#include "PG_Object_Group_Map.h"
#include "PG_Object_Group.h"
#include <orbsvcs/orbsvcs/PortableGroup/PG_Utils.h>

TAO::PG_Object_Group_Map::PG_Object_Group_Map ()
{
}

TAO::PG_Object_Group_Map::~PG_Object_Group_Map ()
{
  // todo: destroy all members
}

//////////////////////////////////////////////////////
// PG_Object_Group_Map public methods


    // insert group.  Take ownership
int TAO::PG_Object_Group_Map::insert_group ( ::TAO::PG_Object_Group * group)
{
  return insert_group (group->get_object_group_id(), group);
}

int TAO::PG_Object_Group_Map::insert_group (PortableGroup::ObjectGroupId group_id, ::TAO::PG_Object_Group * group)
{
  return (this->bind (group_id, group) == 0);
}

int TAO::PG_Object_Group_Map::find_group (PortableGroup::ObjectGroupId group_id, ::TAO::PG_Object_Group *& group) const
{
  return (this->find (group_id , group) == 0);
}

int TAO::PG_Object_Group_Map::find_group (PortableGroup::ObjectGroup_ptr object_group, ::TAO::PG_Object_Group *& group) const
{
  PortableGroup::TagGroupTaggedComponent tc;
  TAO::PG_Utils::get_tagged_component (object_group, tc);
  return find_group (tc.object_group_id, group);
}

int TAO::PG_Object_Group_Map::destroy_group (PortableGroup::ObjectGroupId group_id)
{
  ::TAO::PG_Object_Group * group;
  int result = (this->unbind (group_id, group) == 0);
  if (result)
  {
    delete group;
  }
  return result;
}

int TAO::PG_Object_Group_Map::destroy_group (PortableGroup::ObjectGroup_ptr object_group)
{
  PortableGroup::TagGroupTaggedComponent tc;
  TAO::PG_Utils::get_tagged_component (object_group, tc);
  return destroy_group (tc.object_group_id);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

  template class ACE_Hash_Map_Manager_Ex<
      PortableGroup::ObjectGroupId,
      ::TAO::PG_Object_Group *,
      ACE_Hash<ACE_UINT64>,
      ACE_Equal_To<ACE_UINT64>,
      TAO_Synch_Mutex>;

  template class ACE_Hash_Map_Entry <
      PortableGroup::ObjectGroupId, 
      ::TAO::PG_Object_Group *>;

  template class ACE_Hash_Map_Iterator_Ex <
      PortableGroup::ObjectGroupId, 
      ::TAO::PG_Object_Group *, 
      ACE_Hash<ACE_UINT64>,
      ACE_Equal_To<ACE_UINT64>,
      TAO_Synch_Mutex>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

# pragma instantiate ACE_Hash_Map_Manager_Ex<
      PortableGroup::ObjectGroupId,
      ::TAO::PG_Object_Group *,
      ACE_Hash<ACE_UINT64>,
      ACE_Equal_To<ACE_UINT64>,
      TAO_Synch_Mutex>;

# pragma instantiate ACE_Hash_Map_Entry <
      PortableGroup::ObjectGroupId, 
      ::TAO::PG_Object_Group *>;

# pragma instantiate ACE_Hash_Map_Iterator_Ex <
      PortableGroup::ObjectGroupId, 
      ::TAO::PG_Object_Group *, 
      ACE_Hash<ACE_UINT64>,
      ACE_Equal_To<ACE_UINT64>,
      TAO_Synch_Mutex>;

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

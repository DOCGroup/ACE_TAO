// -*- C++ -*-

#include "LB_ObjectGroupManager.h"

#include "LB_Location_Map.h"
#include "LB_ObjectGroup_Map.h"


ACE_RCSID (LoadBalancing,
           LB_ObjectGroupManager,
           "$Id$")


TAO_LB_ObjectGroupManager::TAO_LB_ObjectGroupManager (
  TAO_LB_Location_Map &location_map,
  TAO_LB_ObjectGroup_Map &object_group_map,
  TAO_LB_PropertyManager &property_manager)
  : location_map_ (location_map),
    object_group_map_ (object_group_map),
    property_manager_ (property_manager)
{
}

PortableGroup::ObjectGroup_ptr
TAO_LB_ObjectGroupManager::create_member (
    PortableGroup::ObjectGroup_ptr /* object_group */,
    const PortableGroup::Location & /* the_location */,
    const char * /* type_id */,
    const PortableGroup::Criteria & /* the_criteria */,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound,
                   PortableGroup::MemberAlreadyPresent,
                   PortableGroup::NoFactory,
                   PortableGroup::ObjectNotCreated,
                   PortableGroup::InvalidCriteria,
                   PortableGroup::CannotMeetCriteria))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    PortableGroup::ObjectGroup::_nil ());
}

PortableGroup::ObjectGroup_ptr
TAO_LB_ObjectGroupManager::add_member (
    PortableGroup::ObjectGroup_ptr /* object_group */,
    const PortableGroup::Location & /* the_location */,
    CORBA::Object_ptr /* member */,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound,
                   PortableGroup::MemberAlreadyPresent,
                   PortableGroup::ObjectNotAdded))
{
#if 0
  TAO_LB_ObjectGroup_Map_Entry group_entry =
    this->object_group_map_.get_group_entry (object_group, ACE_TRY_ENV);
  ACE_CHECK_RETURN (PortableGroup::ObjectGroup::_nil ());

  ACE_NEW_THROW_EX (replica_info,
                    TAO_LB_Replica_Info,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (PortableGroup::ObjectGroup::_nil ());

  auto_ptr<TAO_LB_Replica_Info> safe_replica_info = replica_info;

  CORBA::Object_var obj = CORBA::Object::_duplicate (member);

  int result = this->location_map_.bind (the_location, member);
  if (result == 1)
    ACE_THROW_RETURN (PortableGroup::MemberAlreadyPresent (),
                      PortableGroup::ObjectGroup::_nil ());
  else if (result == -1)
    ACE_THROW_RETURN (PortableGroup::ObjectNotAdded (),
                      PortableGroup::ObjectGroup::_nil ());

  // Transfer ownership to the location map.
  (void) obj._retn ();
#else
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    PortableGroup::ObjectGroup::_nil ());
#endif  /* 0 */
}

PortableGroup::ObjectGroup_ptr
TAO_LB_ObjectGroupManager::remove_member (
    PortableGroup::ObjectGroup_ptr /* object_group */,
    const PortableGroup::Location & /* the_location */,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound,
                   PortableGroup::MemberNotFound))
{
  // @@ DO NOT REMOVE MEMBER IF ITS GENERICFACTORY REFERENCE IS NOT
  //    NIL!
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    PortableGroup::ObjectGroup::_nil ());
}

PortableGroup::Locations *
TAO_LB_ObjectGroupManager::locations_of_members (
    PortableGroup::ObjectGroup_ptr /* object_group */,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound))
{
#if 0
  TAO_LB_ObjectGroup_Map_Entry group_entry =
    this->get_group_entry (object_group, ACE_TRY_ENV);
  ACE_CHECK_RETURN (PortableGroup::ObjectGroup::_nil ());

  PortableGroup::Locations *temp = 0;
  ACE_NEW_THROW_EX (temp,
                    PortableGroup::Locations,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (0);

  PortableGroup::Locations_var locations = temp;

  {
    ACE_GUARD (TAO_SYNCH_MUTEX, guard, object_group->lock);

    TAO_LB_ReplicaInfo_Set &replica_infos =
      object_group->replica_infos;

    locations->length (replica_infos.size ());

    CORBA::ULong loc = 0;
    TAO_LB_ReplicaInfo_Set_Iterator end = replica_infos.end ();

    for (TAO_LB_ReplicaInfo_Set_Iterator i = replica_infos.begin ();
         i != end;
         ++loc, ++i)
      locations[loc] = (*i)->factory_info.the_location;
  }

  return locations._retn ();
#else
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
#endif  /* 0 */
}

PortableGroup::ObjectGroupId
TAO_LB_ObjectGroupManager::get_object_group_id (
    PortableGroup::ObjectGroup_ptr /* object_group */,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

PortableGroup::ObjectGroup_ptr
TAO_LB_ObjectGroupManager::get_object_group_ref (
    PortableGroup::ObjectGroup_ptr /* object_group */,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    PortableGroup::ObjectGroup::_nil ());
}

CORBA::Object_ptr
TAO_LB_ObjectGroupManager::get_member_ref (
    PortableGroup::ObjectGroup_ptr /* object_group */,
    const PortableGroup::Location & /* loc */,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound,
                   PortableGroup::MemberNotFound))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    CORBA::Object::_nil ());
}

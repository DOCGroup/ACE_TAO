// -*- C++ -*-

#include "LB_ObjectGroupManager.h"

ACE_RCSID (LoadBalancing,
           LB_ObjectGroupManager,
           "$Id$")

#if !defined (__ACE_INLINE__)
#include "LB_ObjectGroupManager.inl"
#endif /* __ACE_INLINE__ */

#include "LB_ObjectGroup_Map.h"


TAO_LB_ObjectGroupManager::TAO_LB_ObjectGroupManager (
  TAO_LB_PropertyManager &property_manager,
  TAO_LB_ObjectGroup_Map &map)
  : property_manager_ (property_manager),
    object_group_map_ (map)
{
}

LoadBalancing::ObjectGroup_ptr
TAO_LB_ObjectGroupManager::create_member (
    LoadBalancing::ObjectGroup_ptr /* object_group */,
    const LoadBalancing::Location & /* the_location */,
    const char * /* type_id */,
    const LoadBalancing::Criteria & /* the_criteria */,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   LoadBalancing::ObjectGroupNotFound,
                   LoadBalancing::MemberAlreadyPresent,
                   LoadBalancing::NoFactory,
                   LoadBalancing::ObjectNotCreated,
                   LoadBalancing::InvalidCriteria,
                   LoadBalancing::CannotMeetCriteria))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    LoadBalancing::ObjectGroup::_nil ());
}

LoadBalancing::ObjectGroup_ptr
TAO_LB_ObjectGroupManager::add_member (
    LoadBalancing::ObjectGroup_ptr /* object_group */,
    const LoadBalancing::Location & /* the_location */,
    CORBA::Object_ptr /* member */,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   LoadBalancing::ObjectGroupNotFound,
                   LoadBalancing::MemberAlreadyPresent,
                   LoadBalancing::ObjectNotAdded))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    LoadBalancing::ObjectGroup::_nil ());
}

LoadBalancing::ObjectGroup_ptr
TAO_LB_ObjectGroupManager::remove_member (
    LoadBalancing::ObjectGroup_ptr /* object_group */,
    const LoadBalancing::Location & /* the_location */,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   LoadBalancing::ObjectGroupNotFound,
                   LoadBalancing::MemberNotFound))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    LoadBalancing::ObjectGroup::_nil ());
}

LoadBalancing::ObjectGroup_ptr
TAO_LB_ObjectGroupManager::set_primary_member (
    LoadBalancing::ObjectGroup_ptr /* object_group */,
    const LoadBalancing::Location & /* the_location */,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   LoadBalancing::ObjectGroupNotFound,
                   LoadBalancing::MemberNotFound,
                   LoadBalancing::PrimaryNotSet,
                   LoadBalancing::BadReplicationStyle))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    LoadBalancing::ObjectGroup::_nil ());
}

LoadBalancing::Locations *
TAO_LB_ObjectGroupManager::locations_of_members (
    LoadBalancing::ObjectGroup_ptr /* object_group */,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   LoadBalancing::ObjectGroupNotFound))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

LoadBalancing::ObjectGroupId
TAO_LB_ObjectGroupManager::get_object_group_id (
    LoadBalancing::ObjectGroup_ptr /* object_group */,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   LoadBalancing::ObjectGroupNotFound))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

LoadBalancing::ObjectGroup_ptr
TAO_LB_ObjectGroupManager::get_object_group_ref (
    LoadBalancing::ObjectGroup_ptr /* object_group */,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   LoadBalancing::ObjectGroupNotFound))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    LoadBalancing::ObjectGroup::_nil ());
}

CORBA::Object_ptr
TAO_LB_ObjectGroupManager::get_member_ref (
    LoadBalancing::ObjectGroup_ptr /* object_group */,
    const LoadBalancing::Location & /* loc */,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   LoadBalancing::ObjectGroupNotFound,
                   LoadBalancing::MemberNotFound))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    CORBA::Object::_nil ());
}

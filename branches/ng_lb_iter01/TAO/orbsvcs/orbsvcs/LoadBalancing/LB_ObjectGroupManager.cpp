// -*- C++ -*-

#include "LB_ObjectGroupManager.h"

ACE_RCSID (LoadBalancing,
           LB_ObjectGroupManager,
           "$Id$")

#if !defined (__ACE_INLINE__)
#include "LB_ObjectGroupManager.inl"
#endif /* __ACE_INLINE__ */

TAO_LB_ObjectGroupManager::TAO_LB_ObjectGroupManager (
  TAO_LB_ObjectGroup_Map *map)
  : object_group_map_ (map)
{
}

TAO_LoadBalancing::ObjectGroup_ptr
TAO_LB_ObjectGroupManager::create_member (
    TAO_LoadBalancing::ObjectGroup_ptr /* object_group */,
    const TAO_LoadBalancing::Location & /* the_location */,
    const char * /* type_id */,
    const TAO_LoadBalancing::Criteria & /* the_criteria */,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::ObjectGroupNotFound,
                   TAO_LoadBalancing::MemberAlreadyPresent,
                   TAO_LoadBalancing::NoFactory,
                   TAO_LoadBalancing::ObjectNotCreated,
                   TAO_LoadBalancing::InvalidCriteria,
                   TAO_LoadBalancing::CannotMeetCriteria))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

TAO_LoadBalancing::ObjectGroup_ptr
TAO_LB_ObjectGroupManager::add_member (
    TAO_LoadBalancing::ObjectGroup_ptr /* object_group */,
    const TAO_LoadBalancing::Location & /* the_location */,
    CORBA::Object_ptr /* member */,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::ObjectGroupNotFound,
                   TAO_LoadBalancing::MemberAlreadyPresent,
                   TAO_LoadBalancing::ObjectNotAdded))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

TAO_LoadBalancing::ObjectGroup_ptr
TAO_LB_ObjectGroupManager::remove_member (
    TAO_LoadBalancing::ObjectGroup_ptr /* object_group */,
    const TAO_LoadBalancing::Location & /* the_location */,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::ObjectGroupNotFound,
                   TAO_LoadBalancing::MemberNotFound))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

TAO_LoadBalancing::ObjectGroup_ptr
TAO_LB_ObjectGroupManager::set_primary_member (
    TAO_LoadBalancing::ObjectGroup_ptr /* object_group */,
    const TAO_LoadBalancing::Location & /* the_location */,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::ObjectGroupNotFound,
                   TAO_LoadBalancing::MemberNotFound,
                   TAO_LoadBalancing::PrimaryNotSet,
                   TAO_LoadBalancing::BadReplicationStyle))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

TAO_LoadBalancing::Locations *
TAO_LB_ObjectGroupManager::locations_of_members (
    TAO_LoadBalancing::ObjectGroup_ptr /* object_group */,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::ObjectGroupNotFound))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

TAO_LoadBalancing::ObjectGroupId
TAO_LB_ObjectGroupManager::get_object_group_id (
    TAO_LoadBalancing::ObjectGroup_ptr /* object_group */,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::ObjectGroupNotFound))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

TAO_LoadBalancing::ObjectGroup_ptr
TAO_LB_ObjectGroupManager::get_object_group_ref (
    TAO_LoadBalancing::ObjectGroup_ptr /* object_group */,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::ObjectGroupNotFound))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

CORBA::Object_ptr
TAO_LB_ObjectGroupManager::get_member_ref (
    TAO_LoadBalancing::ObjectGroup_ptr /* object_group */,
    const TAO_LoadBalancing::Location & loc,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::ObjectGroupNotFound,
                   TAO_LoadBalancing::MemberNotFound))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

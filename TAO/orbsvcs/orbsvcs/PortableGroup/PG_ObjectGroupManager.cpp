// $Id$

#include "orbsvcs/PortableGroup/PG_ObjectGroupManager.h"
#include "orbsvcs/PortableGroup/PG_GenericFactory.h"
#include "orbsvcs/PortableGroup/PG_conf.h"
#include "orbsvcs/PortableGroup/PG_Operators.h"

#include "tao/debug.h"
#include "tao/ORB_Constants.h"

#include "ace/Auto_Ptr.h"
#include "ace/Reverse_Lock_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_PG_ObjectGroupManager::TAO_PG_ObjectGroupManager (void)
  : poa_ (),
    object_group_map_ (TAO_PG_MAX_OBJECT_GROUPS),
    location_map_ (TAO_PG_MAX_LOCATIONS),
    generic_factory_ (0),
    lock_ ()
{
}

TAO_PG_ObjectGroupManager::~TAO_PG_ObjectGroupManager (void)
{
  for (TAO_PG_Location_Map::iterator i = this->location_map_.begin ();
       i != this->location_map_.end ();
       ++i)
    {
      // Destroy the group array
      delete (*i).int_id_;
    }
  (void) this->location_map_.close ();

  for (TAO_PG_ObjectGroup_Map::iterator j = this->object_group_map_.begin ();
       j != this->object_group_map_.end ();
       ++j)
    {
      // Destroy the object group entry
      delete (*j).int_id_;
    }
  (void) this->object_group_map_.close ();
}

PortableGroup::ObjectGroup_ptr
TAO_PG_ObjectGroupManager::create_member (
    PortableGroup::ObjectGroup_ptr /* object_group */,
    const PortableGroup::Location & /* the_location */,
    const char * /* type_id */,
    const PortableGroup::Criteria & /* the_criteria */)
{
  throw CORBA::NO_IMPLEMENT ();
}

PortableGroup::ObjectGroup_ptr
TAO_PG_ObjectGroupManager::add_member (
    PortableGroup::ObjectGroup_ptr object_group,
    const PortableGroup::Location & the_location,
    CORBA::Object_ptr member)
{
  if (CORBA::is_nil (member))
    throw CORBA::BAD_PARAM ();

  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    guard,
                    this->lock_,
                    PortableGroup::ObjectGroup::_nil ());

  // Verify that the member's RepositoryId matches the object group's
  // type ID.
  const CORBA::Boolean check_type_id = 1;

  return this->add_member_i (object_group,
                             the_location,
                             member,
                             check_type_id);

}


PortableGroup::ObjectGroup_ptr
TAO_PG_ObjectGroupManager::_tao_add_member (
    PortableGroup::ObjectGroup_ptr object_group,
    const PortableGroup::Location & the_location,
    CORBA::Object_ptr member,
    const char * type_id,
    const CORBA::Boolean propagate_member_already_present)
{
  if (CORBA::is_nil (member))
    throw CORBA::BAD_PARAM ();

  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    guard,
                    this->lock_,
                    PortableGroup::ObjectGroup::_nil ());

  PortableGroup::ObjectGroup_var new_group;

  try
    {
      // TypeId already checked by GenericFactory.
      const CORBA::Boolean check_type_id = 0;

      new_group = this->add_member_i (object_group,
                                      the_location,
                                      member,
                                      check_type_id);
    }
  catch (const PortableGroup::ObjectGroupNotFound&)
    {
      throw CORBA::INTERNAL ();
    }
  catch (const PortableGroup::MemberAlreadyPresent&)
    {
      if (propagate_member_already_present)
        throw;
      else
        throw CORBA::INTERNAL ();
    }
  catch (const PortableGroup::ObjectNotAdded&)
    {
      throw PortableGroup::NoFactory (the_location, type_id);
    }

  return new_group._retn ();
}

PortableGroup::ObjectGroup_ptr
TAO_PG_ObjectGroupManager::add_member_i (
    PortableGroup::ObjectGroup_ptr object_group,
    const PortableGroup::Location & the_location,
    CORBA::Object_ptr member,
    const CORBA::Boolean check_type_id)
{
  TAO_PG_ObjectGroup_Map_Entry * group_entry =
    this->get_group_entry (object_group);

  if (check_type_id)
    {
      CORBA::Boolean right_type_id =
        this->valid_type_id (object_group,
                             group_entry,
                             member);

      if (!right_type_id)
        {
          // The member's type_id does not match the object group's
          // type_id.
          throw PortableGroup::ObjectNotAdded ();
        }
    }

  TAO_PG_ObjectGroup_Array * groups = 0;
  if (this->location_map_.find (the_location, groups) == 0
      && this->member_already_present (*groups, group_entry))
    throw PortableGroup::MemberAlreadyPresent ();

  TAO_PG_MemberInfo member_info;
  member_info.group    = PortableGroup::ObjectGroup::_duplicate (object_group);
  member_info.member   = CORBA::Object::_duplicate (member);
  member_info.location = the_location;
  member_info.is_alive = true;

  if (groups == 0)
    {
      ACE_NEW_THROW_EX (groups,
                        TAO_PG_ObjectGroup_Array,
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID,
                            ENOMEM),
                          CORBA::COMPLETED_NO));

      auto_ptr<TAO_PG_ObjectGroup_Array> safe_groups (groups);

      // This should not fail!
      if (this->location_map_.bind (the_location, groups) != 0)
        {
          throw PortableGroup::ObjectNotAdded ();
        }

      (void) safe_groups.release ();
    }

  // No object group member of the object group with the given
  // ObjectGroupId resides at the location.  Add the object group
  // entry to array of object groups residing at the location.
  const size_t groups_len = groups->size ();
  groups->size (groups_len + 1);
  (*groups)[groups_len] = group_entry;

  // Don't bother checking for duplicates since a check is already
  // performed when binding to the location map above.
  if (group_entry->member_infos.insert_tail (member_info) != 0)
    throw PortableGroup::ObjectNotAdded ();

  return PortableGroup::ObjectGroup::_duplicate (object_group);
}

PortableGroup::ObjectGroup_ptr
TAO_PG_ObjectGroupManager::remove_member (
    PortableGroup::ObjectGroup_ptr object_group,
    const PortableGroup::Location & the_location)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->lock_, 0);

  TAO_PG_ObjectGroup_Map_Entry * group_entry =
    this->get_group_entry (object_group);

  TAO_PG_ObjectGroup_Array * groups = 0;
  if (this->location_map_.find (the_location, groups) != 0)
    throw PortableGroup::ObjectGroupNotFound ();

  // Multiple members from different object groups may reside at the
  // same location.  Iterate through the list to attempt to find a
  // match for the exact object group. If no match, -1 is returned.
  int to_be_removed = this->get_object_group_position (*groups, group_entry);

  if (to_be_removed == -1)
    throw PortableGroup::ObjectGroupNotFound ();

  // remove the element from the array and resize the array.
  const size_t groups_len = groups->size ();
  size_t j;
  for (size_t i = to_be_removed; i < groups_len - 1; ++i)
    {
      j = i + 1;
      (*groups)[i] = (*groups)[j];
    }

  groups->size (groups_len - 1);

  TAO_PG_MemberInfo_Set & member_infos = group_entry->member_infos;

  TAO_PG_MemberInfo_Set::iterator end = member_infos.end ();

  for (TAO_PG_MemberInfo_Set::iterator iter = member_infos.begin ();
       iter != end;
       ++iter)
    {
      const TAO_PG_MemberInfo & info = *iter;

      if (info.location == the_location)
        {
          // Give the GenericFactory a chance to delete a member if
          // its membership is under infrastructure control.
          if (this->generic_factory_)
            {
              CORBA::ULong group_id = group_entry->group_id;
              // Release the lock during the factory delete_member()
              // which may invoke delete_object()on registered factory.
              // No need to block other threads during the invocation.
              ACE_Reverse_Lock<TAO_SYNCH_MUTEX> reverse_lock (this->lock_);

              ACE_GUARD_RETURN (ACE_Reverse_Lock<TAO_SYNCH_MUTEX>,
                                reverse_guard,
                                reverse_lock,
                                PortableGroup::ObjectGroup::_nil ());

              this->generic_factory_->delete_member (group_id,
                                                     the_location);
            }

          if (member_infos.remove (info) == 0)
            {
              if (this->generic_factory_)
                {
                  this->generic_factory_->check_minimum_number_members (
                    object_group,
                    group_entry->group_id,
                    group_entry->type_id.in ());
                }

              return PortableGroup::ObjectGroup::_duplicate (object_group);
            }
          else
            break;
        }
    }

  throw PortableGroup::MemberNotFound ();
}


PortableGroup::Locations *
TAO_PG_ObjectGroupManager::locations_of_members (
    PortableGroup::ObjectGroup_ptr object_group)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->lock_, 0);

  TAO_PG_ObjectGroup_Map_Entry * group_entry =
    this->get_group_entry (object_group);

  PortableGroup::Locations *temp = 0;
  ACE_NEW_THROW_EX (temp,
                    PortableGroup::Locations,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  PortableGroup::Locations_var locations = temp;

  TAO_PG_MemberInfo_Set & member_infos = group_entry->member_infos;

  CORBA::ULong loc = 0;
  TAO_PG_MemberInfo_Set::iterator end = member_infos.end ();

  for (TAO_PG_MemberInfo_Set::iterator i = member_infos.begin ();
       i != end;
       ++i)
  {
    // This flag is only set to false and used by TAO_LB_LoadManager
    // which uses this flag to help avoid same member be returned by
    // next member multiple times.
    // The flag is always true for any other non-LB services.
    if ((*i).is_alive)
    {
      locations->length (loc + 1);
      locations[loc++] = (*i).location;
    }
  }
  return locations._retn ();
}


PortableGroup::ObjectGroups *
TAO_PG_ObjectGroupManager::groups_at_location (
    const PortableGroup::Location & the_location)
{
  PortableGroup::ObjectGroups * ogs;
  ACE_NEW_THROW_EX (ogs,
                    PortableGroup::ObjectGroups,
                    CORBA::NO_MEMORY ());

  PortableGroup::ObjectGroups_var object_groups = ogs;

  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->lock_, 0);

  TAO_PG_ObjectGroup_Array * groups = 0;
  if (this->location_map_.find (the_location, groups) == 0)
    {
      CORBA::ULong len = static_cast<CORBA::ULong> (groups->size ());

      ogs->length (len);

      for (CORBA::ULong i = 0; i < len; ++i)
        {
          object_groups[i] =
            PortableGroup::ObjectGroup::_duplicate (
              (*groups)[i]->object_group.in ());
        }
    }

  return object_groups._retn ();
}

PortableGroup::ObjectGroupId
TAO_PG_ObjectGroupManager::get_object_group_id (
    PortableGroup::ObjectGroup_ptr object_group)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    guard,
                    this->lock_,
                    0);

  TAO_PG_ObjectGroup_Map_Entry * entry =
    this->get_group_entry (object_group);

  if (entry == 0)
    throw CORBA::INTERNAL ();

  // Only the lower 32 bits of the 64 bit PortableGroup::ObjectGroupId
  // are ever used.
  return entry->group_id;
}

PortableGroup::ObjectGroup_ptr
TAO_PG_ObjectGroupManager::get_object_group_ref (
    PortableGroup::ObjectGroup_ptr object_group)
{
  TAO_PG_ObjectGroup_Map_Entry * entry = 0;

  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                      guard,
                      this->lock_,
                      PortableGroup::ObjectGroup::_nil ());


    entry = this->get_group_entry (object_group);
  }

  if (entry == 0)
    throw CORBA::INTERNAL ();

  // This implemenation does not change the object group reference.
  return PortableGroup::ObjectGroup::_duplicate (object_group);
}

CORBA::Object_ptr
TAO_PG_ObjectGroupManager::get_member_ref (
    PortableGroup::ObjectGroup_ptr object_group,
    const PortableGroup::Location & loc)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    guard,
                    this->lock_,
                    CORBA::Object::_nil ());

  TAO_PG_ObjectGroup_Map_Entry * group_entry =
    this->get_group_entry (object_group);

  // This method assumes that it is faster to check for non-existence
  // of an object group (and hence the member) at a given location,
  // instead of existence of a member at a given location.

  TAO_PG_ObjectGroup_Array * groups = 0;
  if (this->location_map_.find (loc, groups) == 0
      && this->member_already_present (*groups, group_entry))
    {
      TAO_PG_MemberInfo_Set & member_infos = group_entry->member_infos;

      TAO_PG_MemberInfo_Set::iterator end = member_infos.end ();

      // @todo If the object group contains a large number of members,
      //       this loop could take a while.  Explore potentially
      //       faster containers for the list of PG_MemberInfos in the
      //       future.
      for (TAO_PG_MemberInfo_Set::iterator i = member_infos.begin ();
           i != end;
           ++i)
        if ((*i).location == loc)
          return CORBA::Object::_duplicate ((*i).member.in ());
    }

  // No member of the given object group is present at the given
  // location.
  throw PortableGroup::MemberNotFound ();
}

PortableGroup::ObjectGroup_ptr
TAO_PG_ObjectGroupManager::get_object_group_ref_from_id (
        PortableGroup::ObjectGroupId group_id
      )
{
  //@@ If we change the PG's concept of ObjectGroupId from
  // PortableServer::ObjectId to PortableGroup::ObjectGroupId, can
  // just call TAO_PG_ObjectGroupManager::object_group() here.

  TAO_PG_ObjectGroup_Map_Entry * group_entry = 0;
  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                      guard,
                      this->lock_,
                      PortableGroup::ObjectGroup::_nil ());

    if (this->object_group_map_.find (ACE_U64_TO_U32 (group_id),
                                      group_entry)
         != 0)
      {
        throw PortableGroup::ObjectGroupNotFound ();
      }
  }

  if (group_entry == 0)
    {
      throw CORBA::INTERNAL ();
    }

  return
    PortableGroup::ObjectGroup::_duplicate (group_entry->object_group.in ());
}

PortableGroup::ObjectGroup_ptr
TAO_PG_ObjectGroupManager::create_object_group (
  CORBA::ULong group_id,
  const PortableServer::ObjectId &oid,
  const char * type_id,
  const PortableGroup::Criteria & the_criteria)
{
  if (CORBA::is_nil (this->poa_.in ()))
    throw CORBA::INTERNAL ();

  // Create a reference for the ObjectGroup corresponding to the
  // RepositoryId of the object being created.
  CORBA::Object_var object_group =
    this->poa_->create_reference_with_id (oid,
                                          type_id);

  TAO_PG_ObjectGroup_Map_Entry * group_entry = 0;
  ACE_NEW_THROW_EX (group_entry,
                    TAO_PG_ObjectGroup_Map_Entry,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  auto_ptr<TAO_PG_ObjectGroup_Map_Entry> safe_group_entry (group_entry);

  // Set the RepositoryId associated with the created ObjectGroup_Map
  // entry.
  group_entry->type_id = CORBA::string_dup (type_id);

  group_entry->group_id = group_id;

  group_entry->object_group = object_group;

  CORBA::ULong len = the_criteria.length ();
  group_entry->properties.length (len);
  for (CORBA::ULong i = 0; i < len; ++i)
    group_entry->properties[i] = the_criteria[i];

  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                      guard,
                      this->lock_,
                      0);

    if (this->object_group_map_.bind (oid, group_entry) != 0)
      throw PortableGroup::ObjectNotCreated ();
  }

  (void) safe_group_entry.release ();

  return object_group._retn ();
}

void
TAO_PG_ObjectGroupManager::destroy_object_group (
  const PortableServer::ObjectId & oid)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->lock_);

  TAO_PG_ObjectGroup_Map_Entry * group_entry = 0;
  if (this->object_group_map_.unbind (oid, group_entry) != 0)
    throw PortableGroup::ObjectNotFound ();

  delete group_entry;
}

char *
TAO_PG_ObjectGroupManager::type_id (
  PortableGroup::ObjectGroup_ptr object_group)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    guard,
                    this->lock_,
                    0);

  TAO_PG_ObjectGroup_Map_Entry * group_entry =
    this->get_group_entry (object_group);

  return CORBA::string_dup (group_entry->type_id.in ());
}

PortableGroup::ObjectGroup_ptr
TAO_PG_ObjectGroupManager::object_group (const PortableServer::ObjectId & oid)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    guard,
                    this->lock_,
                    PortableGroup::ObjectGroup::_nil ());

  TAO_PG_ObjectGroup_Map_Entry * group_entry = 0;
  if (this->object_group_map_.find (oid, group_entry) == 0)
    return
      PortableGroup::ObjectGroup::_duplicate (group_entry->object_group.in ());
  else
    return PortableGroup::ObjectGroup::_nil ();
}

CORBA::ULong
TAO_PG_ObjectGroupManager::member_count (
    PortableGroup::ObjectGroup_ptr group)
{
//   ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
//                     guard,
//                     this->lock_,
//                     0);

  TAO_PG_ObjectGroup_Map_Entry * group_entry =
    this->get_group_entry (group);

  return static_cast<CORBA::ULong> (group_entry->member_infos.size ());
}

void
TAO_PG_ObjectGroupManager::poa (PortableServer::POA_ptr p)
{
  this->poa_ = PortableServer::POA::_duplicate (p);
}


PortableGroup::Properties *
TAO_PG_ObjectGroupManager::get_properties (
    PortableGroup::ObjectGroup_ptr object_group)
{
  PortableGroup::Properties * properties = 0;
  ACE_NEW_THROW_EX (properties,
                    PortableGroup::Properties,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  PortableGroup::Properties_var safe_properties = properties;

  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                      guard,
                      this->lock_,
                      0);

    TAO_PG_ObjectGroup_Map_Entry * group_entry =
      this->get_group_entry (object_group);

    *properties = group_entry->properties;
  }

  return safe_properties._retn ();
}

TAO_PG_ObjectGroup_Map_Entry *
TAO_PG_ObjectGroupManager::get_group_entry (
    CORBA::Object_ptr object_group)
{
  if (CORBA::is_nil (this->poa_.in ()))
    throw CORBA::INTERNAL ();

  PortableServer::ObjectId_var oid;
  try
    {
      oid = this->poa_->reference_to_id (object_group);
    }
  catch (const PortableServer::POA::WrongAdapter& ex)
    {
      if (TAO_debug_level > 0)
        ex._tao_print_exception ("TAO_PG (%P|%t) Unexpected exception\n");

      throw CORBA::INTERNAL ();
    }
  catch (const PortableServer::POA::WrongPolicy& ex)
    {
      if (TAO_debug_level > 0)
        ex._tao_print_exception ("TAO_PG (%P|%t) Unexpected exception\n");

      throw CORBA::INTERNAL ();
    }

  TAO_PG_ObjectGroup_Map_Entry * group_entry = 0;
  if (this->object_group_map_.find (oid.in (), group_entry) != 0)
    throw PortableGroup::ObjectGroupNotFound ();

  return group_entry;
}

CORBA::Boolean
TAO_PG_ObjectGroupManager::member_already_present (
  const TAO_PG_ObjectGroup_Array &groups,
  TAO_PG_ObjectGroup_Map_Entry * group_entry)
{
  // Multiple members from different object groups may reside at the
  // same location.  Iterate through the list to attempt to find a
  // match.
  size_t len = groups.size ();
  for (size_t i = 0; i < len; ++i)
    {
      // It should be enough just to compare the group_entry pointers,
      // but that seems brittle.  Better to check a controlled value,
      // like the ObjectGroupId.
      if (groups[i]->group_id == group_entry->group_id)
        {
          // Member with given type ID exists at the given
          // location.
          return 1;
        }
    }

  // No member with given type ID present at the given location.
  return 0;
}


int
TAO_PG_ObjectGroupManager::get_object_group_position (
  const TAO_PG_ObjectGroup_Array &groups,
  TAO_PG_ObjectGroup_Map_Entry * group_entry)
{
  // Multiple members from different object groups may reside at the
  // same location.  Iterate through the list to attempt to find a
  // match.
  size_t len = groups.size ();
  for (size_t i = 0; i < len; ++i)
    {
      // It should be enough just to compare the group_entry pointers,
      // but that seems brittle.  Better to check a controlled value,
      // like the ObjectGroupId.
      if (groups[i]->group_id == group_entry->group_id)
        {
          // Member with given type ID exists at the given
          // location.
          return (int)i;
        }
    }

  // No member with given type ID present at the given location.
  return -1;
}

CORBA::Boolean
TAO_PG_ObjectGroupManager::valid_type_id (
  PortableGroup::ObjectGroup_ptr object_group,
  TAO_PG_ObjectGroup_Map_Entry * group_entry,
  CORBA::Object_ptr member)
{
  // @todo Strategize this -- e.g. strict type checking.

  if (CORBA::is_nil (member))
    throw CORBA::BAD_PARAM ();

  // Before we can use this code, i.e. the reverse lock, the
  // TAO_PG_ObjectGroup_Entry should be made so that it is reference
  // counted.  This is necessary since releasing the lock would
  // allow other threads to destroy/unbind the object group entry.
  // Another alternative is to simply attempt to reacquire the
  // object group map entry once the lock is reacquired, which is
  // easier to implement.

  // Copy the type_id before releasing the lock to avoid a race
  // condition.
  CORBA::String_var type_id =
    CORBA::string_dup (group_entry->type_id.in ());

  CORBA::Boolean right_type_id = 0;
  {
    // Release the lock during the type_id check.  No need to block
    // other threads during the invocation.
    ACE_Reverse_Lock<TAO_SYNCH_MUTEX> reverse_lock (this->lock_);

    ACE_GUARD_RETURN (ACE_Reverse_Lock<TAO_SYNCH_MUTEX>,
                      reverse_guard,
                      reverse_lock,
                      right_type_id);

    // Make sure an Object of the correct type was created.  It is
    // possible that an object of the wrong type was created if the
    // type_id parameter does not match the type of object the
    // GenericFactory creates.
    right_type_id =
      member->_is_a (type_id.in ());
  }

  // Make sure the group entry still exists.  It may have been
  // destroyed by another thread.
  group_entry = this->get_group_entry (object_group);

  return right_type_id;
}

void
TAO_PG_ObjectGroupManager::generic_factory (
  TAO_PG_GenericFactory * generic_factory)
{
  this->generic_factory_ = generic_factory;
}


size_t
TAO_PG_ObjectGroupManager::member_count (
    const PortableServer::ObjectId & oid,
    bool is_alive)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->lock_, 0);

  TAO_PG_ObjectGroup_Map_Entry * group_entry = 0;
  if (this->object_group_map_.find (oid, group_entry) != 0)
    throw PortableGroup::ObjectGroupNotFound ();

  int count = 0;
  TAO_PG_MemberInfo_Set & member_infos = group_entry->member_infos;

  TAO_PG_MemberInfo_Set::iterator end = member_infos.end ();

  for (TAO_PG_MemberInfo_Set::iterator i = member_infos.begin ();
       i != end;
       ++i)
  {
    if ((*i).is_alive == is_alive)
    {
      ++count;
    }
  }

  return count;
}


bool
TAO_PG_ObjectGroupManager::is_alive (
    const PortableServer::ObjectId & oid,
    CORBA::Object_ptr member)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->lock_, 0);

  TAO_PG_ObjectGroup_Map_Entry * group_entry = 0;
  if (this->object_group_map_.find (oid, group_entry) != 0)
    throw PortableGroup::ObjectGroupNotFound ();

  TAO_PG_MemberInfo_Set & member_infos = group_entry->member_infos;

  TAO_PG_MemberInfo_Set::iterator end = member_infos.end ();

  for (TAO_PG_MemberInfo_Set::iterator i = member_infos.begin ();
       i != end;
       ++i)
  {
    if ((*i).member->_is_equivalent (member))
    {
      return (*i).is_alive;
    }
  }

  throw PortableGroup::MemberNotFound ();
}


void TAO_PG_ObjectGroupManager::remove_inactive_members ()
{
  TAO_PG_MemberInfo_Set inactive_members;

  {
    ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->lock_);
    inactive_members = this->inactive_members_;
    this->inactive_members_.reset ();
  }

  TAO_PG_MemberInfo_Set::iterator end = inactive_members.end ();
  for (TAO_PG_MemberInfo_Set::iterator i = inactive_members.begin ();
    i != end;
    ++i)
  {
    this->remove_member ((*i).group.in (), (*i).location);
  }
}

TAO_PG_MemberInfo_Set
TAO_PG_ObjectGroupManager::get_members (bool is_alive)
{
  TAO_PG_MemberInfo_Set members;
  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->lock_, TAO_PG_MemberInfo_Set());

    for (TAO_PG_ObjectGroup_Map::iterator j = this->object_group_map_.begin ();
      j != this->object_group_map_.end ();
      ++j)
    {
      TAO_PG_MemberInfo_Set & member_infos = j->int_id_->member_infos;
      TAO_PG_MemberInfo_Set::iterator end = member_infos.end ();

      for (TAO_PG_MemberInfo_Set::iterator i = member_infos.begin ();
        i != end;
        ++i)
      {
        if ((*i).is_alive == is_alive)
        {
          if (members.insert_tail (*i) !=0)
          {
            throw CORBA::INTERNAL ();
          }
        }
      }
    }
  }

  return members;
}

void
TAO_PG_ObjectGroupManager::validate_members (CORBA::ORB_ptr orb,
                                             const TimeBase::TimeT& timeout)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t)TAO_PG_ObjectGroupManager::validate_members start\n")));

  /// Get list of active member info.
  TAO_PG_MemberInfo_Set active_members = this->get_members (true);

  /// Ping each member in the list and add ping failed member to
  /// inactive member list.
  TAO_PG_MemberInfo_Set inactive_members;
  for (TAO_PG_MemberInfo_Set::iterator j = active_members.begin ();
      j != active_members.end ();
      ++j)
  {
    if (! this->ping (orb, (*j).member, timeout))
    {
      if (inactive_members.insert_tail (*j) !=0)
      {
        throw CORBA::INTERNAL ();
      }
    }
  }
  /// Mark the members as inactive in internal group maps.
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->lock_);
    for (TAO_PG_MemberInfo_Set::iterator i = inactive_members.begin ();
      i != inactive_members.end ();
      ++i)
    {
      TAO_PG_ObjectGroup_Map_Entry * group_entry =
        this->get_group_entry ((*i).group.in ());

      TAO_PG_MemberInfo_Set & members = group_entry->member_infos;
      for (TAO_PG_MemberInfo_Set::iterator j = members.begin ();
        j != members.end ();
        ++j)
      {
        if ((*j) == (*i))
          (*j).is_alive = false;
      }
    }

    /// keep a copy of inactive member for removing.
    this->inactive_members_ = inactive_members;
  }

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t)TAO_PG_ObjectGroupManager::validate_members end\n")));
}

bool
TAO_PG_ObjectGroupManager::ping (CORBA::ORB_ptr orb,
                                 CORBA::Object_var& obj,
                                 const TimeBase::TimeT& tt)
{
  bool status = true;
  if (CORBA::is_nil (obj.in ()))
    throw CORBA::OBJECT_NOT_EXIST ();

  // The ping() is used by LoadBalancer which may use RW strategy.
  // The validate thread invokes the _non_existent call to members
  // sequencially. We have to put a timeout on the call in case
  // the client side is not processing ORB requests at this time.
  // In the event that the timeout exception occurs, we will assume
  // that the peer is still around.  If we get any other exception
  // we will say that the member is not available anymore.
  TimeBase::TimeT timeout = tt;
  CORBA::Any timeout_any;
  timeout_any <<= timeout;

  CORBA::PolicyList policy_list (1);
  policy_list.length (1);
  policy_list[0] = orb->
                    create_policy (
                          Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE,
                          timeout_any);
  CORBA::Object_var rtt_obj =
    obj->_set_policy_overrides (policy_list,
                                CORBA::ADD_OVERRIDE);

  // Clean up the policy that was allocated in the try/catch
  for (CORBA::ULong i = 0; i < policy_list.length (); i++)
    policy_list[i]->destroy ();

  try {
    status = ! rtt_obj->_non_existent ();
  }
  catch (CORBA::TIMEOUT& ex)
  {
    if (TAO_debug_level > 8)
    {
      ex._tao_print_exception ("TAO_PG_ObjectGroupManager::ping");
    }
  }
  catch (CORBA::Exception& ex)
  {
    if (TAO_debug_level > 8)
    {
      ex._tao_print_exception ("TAO_PG_ObjectGroupManager::ping");
    }

    status = false;
  }

  return status;
}


TAO_END_VERSIONED_NAMESPACE_DECL

#include "PG_ObjectGroupManager.h"
#include "PG_GenericFactory.h"
#include "PG_conf.h"
#include "PG_Operators.h"

#include "tao/debug.h"

#include "ace/Auto_Ptr.h"


ACE_RCSID (PortableGroup,
           PG_ObjectGroupManager,
           "$Id$")


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
    const PortableGroup::Criteria & /* the_criteria */
    ACE_ENV_ARG_DECL)
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
TAO_PG_ObjectGroupManager::add_member (
    PortableGroup::ObjectGroup_ptr object_group,
    const PortableGroup::Location & the_location,
    CORBA::Object_ptr member
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound,
                   PortableGroup::MemberAlreadyPresent,
                   PortableGroup::ObjectNotAdded))
{
  if (CORBA::is_nil (member))
    ACE_THROW_RETURN (CORBA::BAD_PARAM (),
                      PortableGroup::ObjectGroup::_nil ());

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
                             check_type_id
                             ACE_ENV_ARG_PARAMETER);

}


PortableGroup::ObjectGroup_ptr
TAO_PG_ObjectGroupManager::_tao_add_member (
    PortableGroup::ObjectGroup_ptr object_group,
    const PortableGroup::Location & the_location,
    CORBA::Object_ptr member,
    const char * type_id,
    const CORBA::Boolean propagate_member_already_present
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::MemberAlreadyPresent,
                   PortableGroup::NoFactory))
{
  if (CORBA::is_nil (member))
    ACE_THROW_RETURN (CORBA::BAD_PARAM (),
                      PortableGroup::ObjectGroup::_nil ());

  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    guard,
                    this->lock_,
                    PortableGroup::ObjectGroup::_nil ());

  PortableGroup::ObjectGroup_var new_group;

  ACE_TRY
    {
      // TypeId already checked by GenericFactory.
      const CORBA::Boolean check_type_id = 0;

      new_group = this->add_member_i (object_group,
                                      the_location,
                                      member,
                                      check_type_id
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (PortableGroup::ObjectGroupNotFound, ex)
    {
      ACE_TRY_THROW (CORBA::INTERNAL ());
    }
  ACE_CATCH (PortableGroup::MemberAlreadyPresent, ex)
    {
      if (propagate_member_already_present)
        ACE_RE_THROW;
      else
        ACE_TRY_THROW (CORBA::INTERNAL ());
    }
  ACE_CATCH (PortableGroup::ObjectNotAdded, ex)
    {
      ACE_TRY_THROW (PortableGroup::NoFactory (the_location,
                                               type_id));
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (PortableGroup::ObjectGroup::_nil ());

  return new_group._retn ();
}

PortableGroup::ObjectGroup_ptr
TAO_PG_ObjectGroupManager::add_member_i (
    PortableGroup::ObjectGroup_ptr object_group,
    const PortableGroup::Location & the_location,
    CORBA::Object_ptr member,
    const CORBA::Boolean check_type_id
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound,
                   PortableGroup::MemberAlreadyPresent,
                   PortableGroup::ObjectNotAdded))
{
  TAO_PG_ObjectGroup_Map_Entry * group_entry =
    this->get_group_entry (object_group
                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (PortableGroup::ObjectGroup::_nil ());

  if (check_type_id)
    {
      CORBA::Boolean right_type_id =
        this->valid_type_id (object_group,
                             group_entry,
                             member
                             ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (PortableGroup::ObjectGroup::_nil ());

      if (!right_type_id)
        {
          // The member's type_id does not match the object group's
          // type_id.
          ACE_THROW_RETURN (PortableGroup::ObjectNotAdded (),
                            PortableGroup::ObjectGroup::_nil ());
        }
    }

  TAO_PG_ObjectGroup_Array * groups = 0;
  if (this->location_map_.find (the_location, groups) == 0
      && this->member_already_present (*groups, group_entry))
    ACE_THROW_RETURN (PortableGroup::MemberAlreadyPresent (),
                      PortableGroup::ObjectGroup::_nil ());

  TAO_PG_MemberInfo member_info;
  member_info.member   = CORBA::Object::_duplicate (member);
  member_info.location = the_location;

  if (groups == 0)
    {
      ACE_NEW_THROW_EX (groups,
                        TAO_PG_ObjectGroup_Array,
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      ACE_CHECK_RETURN (0);

      auto_ptr<TAO_PG_ObjectGroup_Array> safe_groups (groups);

      // This should not fail!
      if (this->location_map_.bind (the_location, groups) != 0)
        {
          ACE_THROW_RETURN (PortableGroup::ObjectNotAdded (),
                            PortableGroup::ObjectGroup::_nil ());
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
    ACE_THROW_RETURN (PortableGroup::ObjectNotAdded (),
                      PortableGroup::ObjectGroup::_nil ());

  return PortableGroup::ObjectGroup::_duplicate (object_group);
}

PortableGroup::ObjectGroup_ptr
TAO_PG_ObjectGroupManager::remove_member (
    PortableGroup::ObjectGroup_ptr object_group,
    const PortableGroup::Location & the_location
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound,
                   PortableGroup::MemberNotFound))
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->lock_, 0);

  TAO_PG_ObjectGroup_Map_Entry * group_entry =
    this->get_group_entry (object_group
                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (PortableGroup::ObjectGroup::_nil ());

  TAO_PG_MemberInfo_Set & member_infos = group_entry->member_infos;

  TAO_PG_MemberInfo_Set::iterator end = member_infos.end ();

  for (TAO_PG_MemberInfo_Set::iterator i = member_infos.begin ();
       i != end;
       ++i)
    {
      const TAO_PG_MemberInfo & info = *i;

      if (info.location == the_location)
        {
          // Give the GenericFactory a chance to delete a member if
          // its membership is under infrastructure control.
          this->generic_factory_->delete_member (group_entry->group_id,
                                                 the_location
                                                 ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (PortableGroup::ObjectGroup::_nil ());

          if (member_infos.remove (info) == 0)
            {
              this->generic_factory_->check_minimum_number_members (
                object_group,
                group_entry->group_id,
                group_entry->type_id.in ()
                ACE_ENV_ARG_PARAMETER);
              ACE_CHECK_RETURN (PortableGroup::ObjectGroup::_nil ());

              return PortableGroup::ObjectGroup::_duplicate (object_group);
            }
          else
            break;
        }
    }

  ACE_THROW_RETURN (PortableGroup::MemberNotFound (),
                    PortableGroup::ObjectGroup::_nil ());
}

PortableGroup::Locations *
TAO_PG_ObjectGroupManager::locations_of_members (
    PortableGroup::ObjectGroup_ptr object_group
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound))
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->lock_, 0);

  TAO_PG_ObjectGroup_Map_Entry * group_entry =
    this->get_group_entry (object_group
                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

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

  TAO_PG_MemberInfo_Set & member_infos = group_entry->member_infos;

  locations->length (ACE_static_cast (CORBA::ULong, member_infos.size ()));

  CORBA::ULong loc = 0;
  TAO_PG_MemberInfo_Set::iterator end = member_infos.end ();

  for (TAO_PG_MemberInfo_Set::iterator i = member_infos.begin ();
       i != end;
       ++i)
    locations[loc++] = (*i).location;

  return locations._retn ();
}

PortableGroup::ObjectGroups *
TAO_PG_ObjectGroupManager::groups_at_location (
    const PortableGroup::Location & the_location
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableGroup::ObjectGroups * ogs;
  ACE_NEW_THROW_EX (ogs,
                    PortableGroup::ObjectGroups,
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  PortableGroup::ObjectGroups_var object_groups = ogs;

  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->lock_, 0);

  TAO_PG_ObjectGroup_Array * groups;
  if (this->location_map_.find (the_location, groups) == 0)
    {
      CORBA::ULong len = ACE_static_cast (CORBA::ULong, groups->size ());

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
    PortableGroup::ObjectGroup_ptr object_group
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound))
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    guard,
                    this->lock_,
                    0);

  TAO_PG_ObjectGroup_Map_Entry * entry =
    this->get_group_entry (object_group
                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (entry == 0)
    ACE_THROW_RETURN (CORBA::INTERNAL (), 0);

  // Only the lower 32 bits of the 64 bit PortableGroup::ObjectGroupId
  // are ever used.
  return entry->group_id;
}

PortableGroup::ObjectGroup_ptr
TAO_PG_ObjectGroupManager::get_object_group_ref (
    PortableGroup::ObjectGroup_ptr object_group
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound))
{
  TAO_PG_ObjectGroup_Map_Entry * entry = 0;

  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                      guard,
                      this->lock_,
                      PortableGroup::ObjectGroup::_nil ());


    entry = this->get_group_entry (object_group
                                   ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (PortableGroup::ObjectGroup::_nil ());
  }

  if (entry == 0)
    ACE_THROW_RETURN (CORBA::INTERNAL (),
                      PortableGroup::ObjectGroup::_nil ());

  // This implemenation does not change the object group reference.
  return PortableGroup::ObjectGroup::_duplicate (object_group);
}

CORBA::Object_ptr
TAO_PG_ObjectGroupManager::get_member_ref (
    PortableGroup::ObjectGroup_ptr object_group,
    const PortableGroup::Location & loc
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound,
                   PortableGroup::MemberNotFound))
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    guard,
                    this->lock_,
                    CORBA::Object::_nil ());

  TAO_PG_ObjectGroup_Map_Entry * group_entry =
    this->get_group_entry (object_group
                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (PortableGroup::ObjectGroup::_nil ());

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
  ACE_THROW_RETURN (PortableGroup::MemberNotFound (),
                    CORBA::Object::_nil ());
}


PortableGroup::ObjectGroup_ptr
TAO_PG_ObjectGroupManager::create_object_group (
  CORBA::ULong group_id,
  const PortableServer::ObjectId &oid,
  const char * type_id,
  const PortableGroup::Criteria & the_criteria
  ACE_ENV_ARG_DECL)
{
  // Create a reference for the ObjectGroup corresponding to the
  // RepositoryId of the object being created.
  CORBA::Object_var object_group =
    this->poa_->create_reference_with_id (oid,
                                          type_id
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  TAO_PG_ObjectGroup_Map_Entry * group_entry = 0;
  ACE_NEW_THROW_EX (group_entry,
                    TAO_PG_ObjectGroup_Map_Entry,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

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
      ACE_THROW_RETURN (PortableGroup::ObjectNotCreated (),
                        PortableGroup::ObjectGroup::_nil ());
  }

  (void) safe_group_entry.release ();

  return object_group._retn ();
}

void
TAO_PG_ObjectGroupManager::destroy_object_group (
  const PortableServer::ObjectId & oid
  ACE_ENV_ARG_DECL)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->lock_);

  TAO_PG_ObjectGroup_Map_Entry * group_entry = 0;
  if (this->object_group_map_.unbind (oid, group_entry) != 0)
    ACE_THROW (PortableGroup::ObjectNotFound ());

  delete group_entry;
}

char *
TAO_PG_ObjectGroupManager::type_id (
  PortableGroup::ObjectGroup_ptr object_group
  ACE_ENV_ARG_DECL)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    guard,
                    this->lock_,
                    0);

  TAO_PG_ObjectGroup_Map_Entry * group_entry =
    this->get_group_entry (object_group
                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

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
    PortableGroup::ObjectGroup_ptr group
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound))
{
//   ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
//                     guard,
//                     this->lock_,
//                     0);

  TAO_PG_ObjectGroup_Map_Entry * group_entry =
    this->get_group_entry (group
                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return ACE_static_cast (CORBA::ULong, group_entry->member_infos.size ());
}

void
TAO_PG_ObjectGroupManager::poa (PortableServer::POA_ptr p)
{
  ACE_ASSERT (CORBA::is_nil (this->poa_.in ())
              && !CORBA::is_nil (p));

  this->poa_ = PortableServer::POA::_duplicate (p);
}


PortableGroup::Properties *
TAO_PG_ObjectGroupManager::get_properties (
    PortableGroup::ObjectGroup_ptr object_group
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound))
{
  PortableGroup::Properties * properties = 0;
  ACE_NEW_THROW_EX (properties,
                    PortableGroup::Properties,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (0);

  PortableGroup::Properties_var safe_properties = properties;

  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                      guard,
                      this->lock_,
                      0);

    TAO_PG_ObjectGroup_Map_Entry * group_entry =
      this->get_group_entry (object_group
                             ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);

    *properties = group_entry->properties;
  }

  return safe_properties._retn ();
}

TAO_PG_ObjectGroup_Map_Entry *
TAO_PG_ObjectGroupManager::get_group_entry (
    CORBA::Object_ptr object_group
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound))
{
  if (CORBA::is_nil (this->poa_.in ()))
    ACE_THROW_RETURN (CORBA::INTERNAL (), 0);

  PortableServer::ObjectId_var oid;
  ACE_TRY
    {
      oid = this->poa_->reference_to_id (object_group
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (PortableServer::POA::WrongAdapter, ex)
    {
      if (TAO_debug_level > 0)
        ACE_PRINT_EXCEPTION (ex, "TAO_PG (%P|%t) Unexpected exception\n");

      ACE_TRY_THROW (CORBA::INTERNAL ());
    }
  ACE_CATCH (PortableServer::POA::WrongPolicy, ex)
    {
      if (TAO_debug_level > 0)
        ACE_PRINT_EXCEPTION (ex, "TAO_PG (%P|%t) Unexpected exception\n");

      ACE_TRY_THROW (CORBA::INTERNAL ());
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);

  TAO_PG_ObjectGroup_Map_Entry * group_entry = 0;
  if (this->object_group_map_.find (oid.in (), group_entry) != 0)
    ACE_THROW_RETURN (PortableGroup::ObjectGroupNotFound (),
                      0);

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

CORBA::Boolean
TAO_PG_ObjectGroupManager::valid_type_id (
  PortableGroup::ObjectGroup_ptr object_group,
  TAO_PG_ObjectGroup_Map_Entry * group_entry,
  CORBA::Object_ptr member
  ACE_ENV_ARG_DECL)
{
  // @todo Strategize this -- e.g. strict type checking.

  ACE_ASSERT (!CORBA::is_nil (member));  // This assertion should
                                         // never be tripped.

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
      member->_is_a (type_id.in ()
                     ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (right_type_id);
  }

  // Make sure the group entry still exists.  It may have been
  // destroyed by another thread.
  group_entry = this->get_group_entry (object_group
                                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (right_type_id);

  return right_type_id;
}

void
TAO_PG_ObjectGroupManager::generic_factory (
  TAO_PG_GenericFactory * generic_factory)
{
  ACE_ASSERT (generic_factory != 0);

  this->generic_factory_ = generic_factory;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class auto_ptr<TAO_PG_ObjectGroup_Array>;
template class auto_ptr<TAO_PG_ObjectGroup_Map_Entry>;

#  if defined (ACE_LACKS_AUTO_PTR) \
      || !(defined (ACE_HAS_STANDARD_CPP_LIBRARY) \
           && (ACE_HAS_STANDARD_CPP_LIBRARY != 0))
template class ACE_Auto_Basic_Ptr<TAO_PG_ObjectGroup_Array>;
template class ACE_Auto_Basic_Ptr<TAO_PG_ObjectGroup_Map_Entry>;
#  endif  /* ACE_LACKS_AUTO_PTR */

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate auto_ptr<TAO_PG_ObjectGroup_Array>
#pragma instantiate auto_ptr<TAO_PG_ObjectGroup_Map_Entry>

#  if defined (ACE_LACKS_AUTO_PTR) \
      || !(defined (ACE_HAS_STANDARD_CPP_LIBRARY) \
           && (ACE_HAS_STANDARD_CPP_LIBRARY != 0))
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_PG_ObjectGroup_Array>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_PG_ObjectGroup_Map_Entry>
#  endif  /* ACE_LACKS_AUTO_PTR */

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

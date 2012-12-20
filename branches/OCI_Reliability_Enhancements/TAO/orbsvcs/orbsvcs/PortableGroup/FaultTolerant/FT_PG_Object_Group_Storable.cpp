// $Id$

#include "orbsvcs/PortableGroup/FaultTolerant/FT_PG_Object_Group_Storable.h"
#include "orbsvcs/PortableGroup/PG_Object_Group_Storable.h"

#include "orbsvcs/Naming/FaultTolerant/FT_Naming_Replication_Manager.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::FT_PG_Object_Group_Storable::FT_PG_Object_Group_Storable (
  CORBA::ORB_ptr orb,
  PortableGroup::FactoryRegistry_ptr factory_registry,
  TAO::PG_Object_Group_Manipulator & manipulator,
  CORBA::Object_ptr empty_group,
  const PortableGroup::TagGroupTaggedComponent & tagged_component,
  const char * type_id,
  const PortableGroup::Criteria & the_criteria,
  TAO::PG_Property_Set * type_properties,
  TAO::Storable_Factory & storable_factory)
  : PG_Object_Group_Storable(orb,
                             factory_registry,
                             manipulator,
                             empty_group,
                             tagged_component,
                             type_id,
                             the_criteria,
                             type_properties,
                             storable_factory)
  , stale_ (false)
{
}

TAO::FT_PG_Object_Group_Storable::FT_PG_Object_Group_Storable (
  PortableGroup::ObjectGroupId group_id,
  CORBA::ORB_ptr orb,
  PortableGroup::FactoryRegistry_ptr factory_registry,
  TAO::PG_Object_Group_Manipulator & manipulator,
  TAO::Storable_Factory & storable_factory)
  : PG_Object_Group_Storable(group_id,
                             orb,
                             factory_registry,
                             manipulator,
                             storable_factory)
  , stale_ (false)
{
}

TAO::FT_PG_Object_Group_Storable::~FT_PG_Object_Group_Storable (void)
{
}

void
TAO::FT_PG_Object_Group_Storable::stale (bool is_stale)
{
  this->stale_ = is_stale;
}

bool
TAO::FT_PG_Object_Group_Storable::stale ()
{
  return this->stale_;
}

int
TAO::FT_PG_Object_Group_Storable::propagate_update_notification
  (FT_Naming::ChangeType change_type)
{
  // Notify the peer of the changed context
  FT_Naming::ReplicationManager_var peer =
    TAO_FT_Naming_Replication_Manager::peer_replica ();

  if (CORBA::is_nil (peer.in ()))
    {
      // Replication is not supported without a peer replica.
      return 1;
    }

  FT_Naming::ObjectGroupUpdate object_group_info;
  object_group_info.id = PG_Object_Group::get_object_group_id ();
  object_group_info.change_type = change_type;

  try {
    // Notify the naming_manager of the updated context
    if (TAO_debug_level > 3)
      {

        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%T %n (%P|%t) - ")
                    ACE_TEXT ("Notifying peer that object group with ID %lld ")
                    ACE_TEXT ("has been updated"), object_group_info.id
                    ));
        peer->notify_updated_object_group (object_group_info);
      }
  }
  catch (CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("Unable to communicate with peer.\n"));
      return -1;
    }

  return 0;
}

void
TAO::FT_PG_Object_Group_Storable::state_written (void)
{
  FT_Naming::ChangeType change_type;

  if (this->destroyed_)
    change_type = FT_Naming::DELETED;
  else
    change_type = FT_Naming::UPDATED;

  // If peer is available notify that state has changed.
  // Otherwise, rely on file time stamps exclusively
  // for update notification.
  this->propagate_update_notification (change_type);

  this->write_occurred_ = false;
}

bool
TAO::FT_PG_Object_Group_Storable::is_obsolete (time_t stored_time)
{
  ACE_UNUSED_ARG (stored_time);
  return (!this->loaded_from_stream_) || this->stale_;
}

PortableGroup::ObjectGroup_ptr
TAO::FT_PG_Object_Group_Storable::add_member_to_iogr(CORBA::Object_ptr member)
{
  // assume internals is locked

  PortableGroup::ObjectGroup_var result;

  // If this is the first member, then we should create a new object of the
  // type of the member and add them together.

 ////////////////////////////
  // @@ HACK ALERT
  // The PortableGroup::ObjectGroupManager creates an object reference
  // containing a dummy entry so it will have a place to store the
  // tagged group component. If this is the first entry, we need to
  // remove that entry once we have a *real* member. This can be
  // avoided when we get support for TAG_MULTIPLE_COMPONENTS.   For
  // now, we already have a copy of the tagGroupTagged component and
  // we're going to use it below wen we increment the group version so
  // we can clean out the dummy entry.
  PortableGroup::ObjectGroup_var cleaned =
    PortableGroup::ObjectGroup::_duplicate (this->reference_.in ());
  if (this->empty_)
    {
      // remove the original profile.  It's a dummy entry supplied by
      // create_object.
      cleaned =
        this->manipulator_.remove_profiles (cleaned.in (),
                                            this->reference_.in ());
      this->empty_ = 0;
    }

  // create a list of references to be merged
  TAO_IOP::TAO_IOR_Manipulation::IORList iors (2);
  iors.length (2);
  iors [0] = CORBA::Object::_duplicate (cleaned.in());
  iors [1] = CORBA::Object::_duplicate (member);

  // Now merge the list into one new IOGR
  result =
    this->manipulator_.merge_iors (iors);
  return result._retn ();
}

TAO_END_VERSIONED_NAMESPACE_DECL

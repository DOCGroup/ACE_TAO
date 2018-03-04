
#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Naming/FaultTolerant/FT_PG_Object_Group_Storable.h"
#include "orbsvcs/PortableGroup/PG_Object_Group_Storable.h"
#include "orbsvcs/Naming/FaultTolerant/FT_PG_Group_Factory.h"
#include "orbsvcs/Naming/FaultTolerant/FT_Naming_Replication_Manager.h"
#include "tao/Stub.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::FT_PG_Object_Group_Storable::FT_PG_Object_Group_Storable
(CORBA::ORB_ptr orb,
 PortableGroup::FactoryRegistry_ptr factory_registry,
 TAO::PG_Object_Group_Manipulator & manipulator,
 CORBA::Object_ptr empty_group,
 const PortableGroup::TagGroupTaggedComponent & tagged_component,
 const char * type_id,
 const PortableGroup::Criteria & the_criteria,
 const TAO::PG_Property_Set_var & type_properties,
 TAO::Storable_Factory & storable_factory,
 TAO_FT_Naming_Replication_Manager *repl)
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
  , file_created_ (false)
  , replicator_ (repl)
{
}

TAO::FT_PG_Object_Group_Storable::FT_PG_Object_Group_Storable
( PortableGroup::ObjectGroupId group_id,
  CORBA::ORB_ptr orb,
  PortableGroup::FactoryRegistry_ptr factory_registry,
  TAO::PG_Object_Group_Manipulator & manipulator,
  TAO::Storable_Factory & storable_factory,
  TAO_FT_Naming_Replication_Manager *repl)
  : PG_Object_Group_Storable(group_id,
                             orb,
                             factory_registry,
                             manipulator,
                             storable_factory)
  , stale_ (false)
  , file_created_ (true)
  , replicator_ (repl)
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
    if (this->replicator_)
      this->replicator_->send_objgrp_update (this->get_object_group_id(),
                                            change_type);
    return 0;
}

void
TAO::FT_PG_Object_Group_Storable::state_written (void)
{
  FT_Naming::ChangeType change_type;
  if (!this->file_created_)
    {
      change_type = FT_Naming::NEW;
      this->file_created_ = true;
    }
  else if (this->destroyed_)
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
TAO::FT_PG_Object_Group_Storable::is_obsolete (time_t )
{
  return (!this->loaded_from_stream_) || this->stale_;
}

PortableGroup::ObjectGroup_ptr
TAO::FT_PG_Object_Group_Storable::add_member_to_iogr (CORBA::Object_ptr member)
{
  // If this is the first member added to the group and it's type_id does
  // not match the member, then the object group should assume the same
  // type id as the first member.  We will need to replace the object
  // reference with an empty reference of the specified type id.

  if (CORBA::is_nil (member))
    {// A null object reference is not an acceptable member of the group.
      ORBSVCS_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) ERROR: Unable to add null member ")
                  ACE_TEXT ("to object group with id: %s\n"),
                  this->tagged_component_.object_group_id));
      return CORBA::Object::_nil ();
    }

  const char* member_type_id = member->_stubobj ()->type_id.in ();

  if ((this->members_.current_size () == 0) &&
      (ACE_OS::strcmp (this->type_id_, member_type_id) != 0) )
    {
      try {
        this->type_id_ = member_type_id;
        this->reference_ = manipulator_.create_object_group_using_id (
           this->type_id_,
           this->tagged_component_.group_domain_id,
           this->tagged_component_.object_group_id);
      }
      catch (const CORBA::Exception&)
        {
          ORBSVCS_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) ERROR: Unable to add member ")
                      ACE_TEXT ("to object group with id: %s for object ")
                      ACE_TEXT ("of type: %s\n"),
                      this->tagged_component_.object_group_id,
                      member_type_id));
          return CORBA::Object::_nil ();
        }
    }

  return PG_Object_Group::add_member_to_iogr (member);
}

TAO_END_VERSIONED_NAMESPACE_DECL

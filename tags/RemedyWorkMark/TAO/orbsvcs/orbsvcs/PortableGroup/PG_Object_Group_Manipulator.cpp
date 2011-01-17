// -*- C++ -*-

//=============================================================================
/**
 * @file PG_Object_Group_Manipulator.cpp
 *
 * $Id$
 *
 * @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================

#include "orbsvcs/PortableGroup/PG_Object_Group_Manipulator.h"
#include "orbsvcs/PortableGroup/PG_Utils.h"

#include "tao/debug.h"
#include <ace/OS_NS_stdio.h>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::PG_Object_Group_Manipulator::PG_Object_Group_Manipulator ()
  : orb_ (CORBA::ORB::_nil ())
  , poa_ (PortableServer::POA::_nil ())
  , iorm_ ()
  , lock_ogid_ ()
  , next_ogid_ (1)   // don't use ogid 0
{
}

TAO::PG_Object_Group_Manipulator::~PG_Object_Group_Manipulator ()
{
}

void TAO::PG_Object_Group_Manipulator::allocate_ogid (PortableGroup::ObjectGroupId & ogid)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->lock_ogid_);

  // The numerical value used for the ObjectId increases
  // monotonically.

  ogid = this->next_ogid_;
  this->next_ogid_ += 1;
}

PortableServer::ObjectId * TAO::PG_Object_Group_Manipulator::convert_ogid_to_oid (PortableGroup::ObjectGroupId ogid) const
{
  // 4294967295 -- Largest 32 bit unsigned integer
  char oid_str[11];
  ACE_OS::snprintf (oid_str, sizeof(oid_str),
                   "%lu",
                   static_cast<ACE_UINT32> (ogid));
  oid_str[sizeof(oid_str) - 1] = '\0';

  return PortableServer::string_to_ObjectId (oid_str);
}

PortableGroup::ObjectGroup_ptr
TAO::PG_Object_Group_Manipulator::create_object_group (
  const char * type_id,
  const char * domain_id,
  PortableGroup::ObjectGroupId & group_id)
{
   allocate_ogid(group_id);
   PortableServer::ObjectId_var oid = convert_ogid_to_oid (group_id);

  // Create a reference for the ObjectGroup
  CORBA::Object_var object_group =
    this->poa_->create_reference_with_id (oid.in(),
                                          type_id);

  PortableGroup::TagGroupTaggedComponent tag_component;

  tag_component.component_version.major = (CORBA::Octet) 1;
  tag_component.component_version.minor = (CORBA::Octet) 0;
  tag_component.group_domain_id = domain_id;
  tag_component.object_group_id = group_id;
  tag_component.object_group_ref_version = 0;

  // Set the property
  TAO::PG_Utils::set_tagged_component (object_group,
                                       tag_component);

  return object_group._retn ();
}

void
TAO::PG_Object_Group_Manipulator::init (CORBA::ORB_ptr orb,
                                        PortableServer::POA_ptr poa)
{
  ACE_ASSERT (CORBA::is_nil (this->orb_.in ()) && !CORBA::is_nil (orb));
  this->orb_ = CORBA::ORB::_duplicate (orb);

  ACE_ASSERT (CORBA::is_nil (this->poa_.in ()) && !CORBA::is_nil (poa));
  this->poa_ = PortableServer::POA::_duplicate (poa);

  // Get an object reference for the ORBs IORManipulation object!
  CORBA::Object_var IORM = this->orb_->resolve_initial_references (
    TAO_OBJID_IORMANIPULATION, 0);

  this->iorm_ = TAO_IOP::TAO_IOR_Manipulation::_narrow (
    IORM.in ());
}

int TAO::PG_Object_Group_Manipulator::set_primary (
      TAO_IOP::TAO_IOR_Property * prop,
      PortableGroup::ObjectGroup_ptr group,
      CORBA::Object_ptr new_primary) const
{
  int sts = this->iorm_->is_primary_set (prop, group);
  if (sts)
  {
    (void)this->iorm_->remove_primary_tag (prop, group);
  }
  /////note: iorm takes it's parameters in the "wrong" order for this call
  return this->iorm_->set_primary (prop, new_primary, group);
}

PortableGroup::ObjectGroup_ptr TAO::PG_Object_Group_Manipulator::merge_iors(
  TAO_IOP::TAO_IOR_Manipulation::IORList & list) const
{
  return this->iorm_->merge_iors (list);
}

PortableGroup::ObjectGroup_ptr TAO::PG_Object_Group_Manipulator::remove_profiles(
      PortableGroup::ObjectGroup_ptr group,
      PortableGroup::ObjectGroup_ptr profile) const
{
  return this->iorm_->remove_profiles(group, profile);
}

void dump_membership (const char * label, PortableGroup::ObjectGroup_ptr member)
{
  ACE_UNUSED_ARG (label);
  ACE_UNUSED_ARG (member);
#if 0
  PortableGroup::TagFTGroupTaggedComponent ft_tag_component;
  TAO_FT_IOGR_Property prop (ft_tag_component);
  if (this->iorm_->is_primary_set (&prop, member))
  {
    ACE_DEBUG ( (LM_DEBUG,
      ACE_TEXT ("%T %n (%P|%t) - %s: PRIMARY member.\n"),
      label
      ));
  }
  else
  {
    ACE_DEBUG ( (LM_DEBUG,
      ACE_TEXT ("%T %n (%P|%t) - %s: backup member.\n"),
      label
      ));
  }

  PortableGroup::TagGroupTaggedComponent tag_component;
  if (TAO::PG_Utils::get_tagged_component (member, tag_component))
  {
    ACE_DEBUG ( (LM_DEBUG,
      ACE_TEXT ("%T %n (%P|%t) - %s: Group: .")
      ACE_TEXT (" version: %u\n"),

      label,
      tag_component.object_group_ref_version
      ));
  }
  else
  {
    ACE_DEBUG ( (LM_DEBUG,
      ACE_TEXT ("%T %n (%P|%t) - %s: No group information found.\n"),
      label
      ));
  }
#endif
}

TAO_END_VERSIONED_NAMESPACE_DECL

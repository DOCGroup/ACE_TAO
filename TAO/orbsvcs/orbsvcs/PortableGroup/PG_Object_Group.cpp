// -*- C++ -*-
//
// $Id$

#include "PG_Object_Group.h"

#include <ace/Get_Opt.h>
#include <ace/Vector_T.h>
#include <tao/PortableServer/ORB_Manager.h>
#include <orbsvcs/PortableGroup/PG_Operators.h> // Borrow operator == on CosNaming::Name
#include <orbsvcs/PortableGroup/PG_Utils.h>

TAO::PG_Object_Group::MemberInfo::MemberInfo (
    CORBA::Object_ptr member,
    const PortableGroup::Location & location)
  : member_ (CORBA::Object::_duplicate (member))
  , factory_(PortableGroup::GenericFactory::_nil())
  , location_ (location)
  , is_primary_ (0)
{
}

TAO::PG_Object_Group::MemberInfo::MemberInfo (
    CORBA::Object_ptr member,
    const PortableGroup::Location & location,
    PortableGroup::GenericFactory_ptr factory,
    PortableGroup::GenericFactory::FactoryCreationId factory_id)
  : member_ (CORBA::Object::_duplicate (member))
  , factory_ (PortableGroup::GenericFactory::_duplicate (factory))
  , factory_id_ (factory_id)
  , location_ (location)
  , is_primary_ (0)
{
}

TAO::PG_Object_Group::MemberInfo::~MemberInfo ()
{
  if( ! CORBA::is_nil (this->factory_.in()))
  {
    ACE_TRY_NEW_ENV
    {
      this->factory_->delete_object (this->factory_id_);
    }
    ACE_CATCHANY;
    {
      // ignore this.  It may have faulted and gone away.
      // (and besides, we're in a destructor.
    }
    ACE_ENDTRY;
  }
}

TAO::PG_Object_Group::PG_Object_Group (
  CORBA::ORB_ptr orb,
  TAO::PG_Object_Group_Manipulator * manipulator,
  CORBA::Object_ptr empty_group,
  const PortableGroup::TagGroupTaggedComponent & tagged_component,
  const char * type_id,
  const PortableGroup::Criteria & the_criteria,
  TAO_PG::Properties_Decoder * type_properties)
    : internals_()
    , orb_ (CORBA::ORB::_duplicate (orb))
    , manipulator_ (manipulator)
    , empty_ (1)
    , role_ (type_id)
    , tagged_component_ (tagged_component)
    , reference_ (CORBA::Object::_duplicate(empty_group))
//      MemberMap members_ self initialized
    , primary_location_(0)
    , properties_ (the_criteria, type_properties)
    , membership_style_ (0)
    , initial_number_members_ (0)
    , minimum_number_members_ (0)
//    group_specific_factories_ self initialized
{
}

//static
TAO::PG_Object_Group * TAO::PG_Object_Group::create (
  CORBA::ORB_ptr orb,
  PortableServer::POA_ptr poa,
  CORBA::Object_ptr empty_group, // empty group as created by ObjectManager
  const char * type_id,
  const PortableGroup::Criteria & the_criteria,
  TAO_PG::Properties_Decoder * type_properties
  ACE_ENV_ARG_DECL)
{
  TAO::PG_Object_Group_Manipulator * manipulator = 0;
  ACE_NEW_THROW_EX (manipulator,
    TAO::PG_Object_Group_Manipulator (),
    CORBA::NO_MEMORY ());
  int init_ok = manipulator->init (orb, poa ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);
  if(init_ok)
  {
    if (TAO_debug_level > 3)
    {
      ACE_ERROR ( (LM_ERROR,
        ACE_TEXT (
          "%T %n (%P|%t) - "
          "Could not find an IOR Manipulator.\n")));
    }
    return 0;
  }

  // pick up the object group information as assigned by
  // ObjectGroupManager

  PortableGroup::TagGroupTaggedComponent tagged_component;

  if (! TAO::PG_Utils::get_tagged_component (empty_group, tagged_component))
  {
    ACE_THROW_RETURN (PortableGroup::ObjectGroupNotFound(), 0);
  }

  TAO::PG_Object_Group * objectGroup = 0;
  ACE_NEW_THROW_EX (
    objectGroup,
    TAO::PG_Object_Group (
      orb,
      manipulator,
      empty_group,
      tagged_component,
      type_id,
      the_criteria,
      type_properties
      ),
    CORBA::NO_MEMORY());
  return objectGroup;
}


TAO::PG_Object_Group::~PG_Object_Group ()
{
  delete manipulator_;
  manipulator_ = 0;
  for (MemberMap_Iterator it = this->members_.begin();
      it != this->members_.end();
      this->members_.begin())
  {
    MemberInfo * member = (*it).int_id_;
    delete member;
    this->members_.unbind((*it).ext_id_);
  }
}



#if 0   // may want this again someday
/////////////////////
// q&d debug function
static void dump_ior (const char * base, const char * ext, unsigned long version, const char * iogr)
{
  char filename[1000];
  ACE_OS::sprintf(filename, "%s_%lu.%s", base, version, ext );

  FILE * iorfile = ACE_OS::fopen(filename, "w");
  ACE_OS::fwrite (iogr, 1, ACE_OS::strlen(iogr), iorfile);
  ACE_OS::fclose (iorfile);
}
#endif  // may want this again someday

PortableGroup::ObjectGroup_ptr TAO::PG_Object_Group::reference()const
{
  // const cast to simulate mutable
  InternalGuard guard(ACE_const_cast (TAO::PG_Object_Group *, this)->internals_);
  return PortableGroup::ObjectGroup::_duplicate (this->reference_);
}

void TAO::PG_Object_Group::set_membership_style (PortableGroup::MembershipStyleValue style)
{
  InternalGuard guard(this->internals_);
  this->membership_style_ = style;
}

PortableGroup::MembershipStyleValue TAO::PG_Object_Group::get_membership_style () const
{
  // const cast to simulate mutable
  InternalGuard guard(ACE_const_cast (TAO::PG_Object_Group *, this)->internals_);
  return this->membership_style_;
}

void TAO::PG_Object_Group::set_initial_number_members (PortableGroup::InitialNumberMembersValue count)
{
  InternalGuard guard(this->internals_);
  this->initial_number_members_ = count;
}

PortableGroup::InitialNumberMembersValue TAO::PG_Object_Group::get_initial_number_members () const
{
  // const cast to simulate mutable
  InternalGuard guard(ACE_const_cast (TAO::PG_Object_Group *, this)->internals_);
  return this->initial_number_members_;
}

void TAO::PG_Object_Group::set_minimum_number_members (PortableGroup::MinimumNumberMembersValue count)
{
  InternalGuard guard(this->internals_);
  this->minimum_number_members_ = count;
}

PortableGroup::MinimumNumberMembersValue TAO::PG_Object_Group::get_minimum_number_members ()const
{
  // const cast to simulate mutable
  InternalGuard guard(ACE_const_cast (TAO::PG_Object_Group *, this)->internals_);
  return this->minimum_number_members_;
}

void TAO::PG_Object_Group::set_group_specific_factories (const PortableGroup::FactoryInfos & infos)
{
  InternalGuard guard(this->internals_);
  this->group_specific_factories_ = infos;
}


void TAO::PG_Object_Group::get_group_specific_factories (PortableGroup::FactoryInfos & result) const
{
  // const cast to simulate mutable
  InternalGuard guard(ACE_const_cast (TAO::PG_Object_Group *, this)->internals_);
  // copy is needed to have some semblance of thread safeness.
  // if performance is an issue avoid this method.
  result = this->group_specific_factories_;
}


const PortableGroup::Location & TAO::PG_Object_Group::primary_location() const
{
  // const cast to simulate mutable
  InternalGuard guard(ACE_const_cast (TAO::PG_Object_Group *, this)->internals_);
  return this->primary_location_;
}

void TAO::PG_Object_Group::add_member (
    const PortableGroup::Location & the_location,
    CORBA::Object_ptr member
    ACE_ENV_ARG_PARAMETER)
  ACE_THROW_SPEC ( (CORBA::SystemException,
                   PortableGroup::ObjectNotAdded))

{
  InternalGuard guard(this->internals_);

  if (TAO_debug_level > 6)
  {
    ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT("PG (%P|%t) enter Object_Group add_member \n")
      ));
  }

  /////////////////////////////////////////
  // Convert the new member to a string IOR
  // This keeps a clean IOR (not and IOGR!)
  // while we add it to a group.  We need a
  // IORs, not IOGRs to send new IOGRs out
  // to replicas.

  CORBA::String_var member_ior_string = orb_->object_to_string (member ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

 ////////////////////////////
  // @@ HACK ALERT
  // The PortableGroup::ObjectGroupManager creates an object reference containing
  // a dummy entry so it will have a place to store the tagged group component.
  // If this is the first entry, we need to remove that entry once we have a *real* member.
  // This can be avoided when we get support for TAG_MULTIPLE_COMPONENTS
  // For now, we already have a copy of the tagGroupTagged component and we're going to use
  // it below wen we increment the group version so we can clean out the dummy entry.
  PortableGroup::ObjectGroup_var cleaned = PortableGroup::ObjectGroup::_duplicate (this->reference_.in ());
  if (this->empty_)
  {
    // remove the original profile.  It's a dummy entry supplied by create_object.
    cleaned =
      this->manipulator_->remove_profiles (cleaned.in (), this->reference_.in () ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
    this->empty_ = 0;
  }

  // create a list of references to be merged
  TAO_IOP::TAO_IOR_Manipulation::IORList iors (2);
  iors.length (2);
  iors [0] = CORBA::Object::_duplicate (cleaned.in());
  iors [1] = CORBA::Object::_duplicate (member);

  // Now merge the list into one new IOGR
  PortableGroup::ObjectGroup_var new_reference =
    this->manipulator_->merge_iors (iors ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Object_var member_ior = this->orb_->string_to_object (member_ior_string ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  MemberInfo * info = 0;
  ACE_NEW_THROW_EX (info, MemberInfo(member_ior.in(), the_location),
    CORBA::NO_MEMORY());

  if (this->members_.bind (the_location, info) != 0)
  {
    ACE_THROW(CORBA::NO_MEMORY());
  }

  this->reference_ = new_reference; // note var-to-var assignment does a duplicate
  if (this->increment_version ())
  {
    this->distribute_iogr (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;
  }
  else
  {
    ACE_THROW (PortableGroup::ObjectNotAdded());
  }

  if (TAO_debug_level > 6)
  {
    ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT("PG (%P|%t) exit Object_Group add_member \n")
      ));
  }
}

int TAO::PG_Object_Group::set_primary_member (
  TAO_IOP::TAO_IOR_Property * prop,
  const PortableGroup::Location & the_location
  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , PortableGroup::MemberNotFound
  ))
{
  InternalGuard guard(this->internals_);
  int result = 1;
  MemberInfo * info;
  if (this->members_.find (the_location, info) == 0)
  {
    int cleared = 0;
    this->primary_location_ = the_location;
    for (MemberMap_Iterator it = this->members_.begin();
        !cleared && it != this->members_.end();
        ++it)
    {
      cleared = (*it).int_id_->is_primary_;
      (*it).int_id_->is_primary_ = 0;
    }
    info->is_primary_ = 1;

    int set_ok = this->manipulator_->set_primary (prop, this->reference_.in (), info->member_.in () ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);
    if (! set_ok)
    {
      if (TAO_debug_level > 3)
      {
        ACE_ERROR ( (LM_ERROR,
          ACE_TEXT ("%T %n (%P|%t) - Can't set primary in IOGR .\n")
          ));
      }
//@@: ACE_THROW (FT::PrimaryNotSet());
      result = 0;
    }

    if (result && this->increment_version())
    {
      this->distribute_iogr (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }
    else
    {
      if (TAO_debug_level > 3)
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT("TAO-PG (%P|%t) - set_primary_location throwing PrimaryNotSet because increment version failed.\n")
          ));
      }
//@@: ACE_THROW (FT::PrimaryNotSet());
      result = 0;
    }
  }
  else
  {
    if (TAO_debug_level > 3)
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("TAO-PG (%P|%t) - set_primary_location throwing MemberNotFound.\n")
        ));
    }
    ACE_THROW (PortableGroup::MemberNotFound());
  }
  return result;
}


void TAO::PG_Object_Group::remove_member (
    const PortableGroup::Location & the_location
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ( (CORBA::SystemException,
                   PortableGroup::MemberNotFound))
{
  InternalGuard guard(this->internals_);
  MemberInfo * info;
  if (this->members_.unbind (the_location, info) == 0)
  {
    if (this->members_.current_size() > 0)
    {
      this->reference_ =
        this->manipulator_->remove_profiles (this->reference_.in (), info->member_.in () ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
    else
    {
      empty_ = 1;
    }

    delete info;

    if (the_location == this->primary_location_)
    {
      this->primary_location_.length(0);
    }

    if (this->increment_version ())
    {
      this->distribute_iogr (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }

  }
  else
  {
    if (TAO_debug_level > 6)
    {

      ACE_DEBUG ((LM_DEBUG,
        "TAO-PG (%P|%t) - remove_member throwing MemberNotFound.\n"
        ));
    }
    ACE_THROW (PortableGroup::MemberNotFound() );
  }
}


PortableGroup::ObjectGroupId TAO::PG_Object_Group::get_object_group_id () const
{
    // const cast to simulate mutable
  InternalGuard guard(ACE_const_cast (TAO::PG_Object_Group *, this)->internals_);
  return this->tagged_component_.object_group_id;
}

void TAO::PG_Object_Group::set_properties_dynamically (
    const PortableGroup::Properties & overrides
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::InvalidProperty,
                   PortableGroup::UnsupportedProperty))
{
  InternalGuard guard(this->internals_);
  this->properties_.decode (overrides ACE_ENV_ARG_PARAMETER);
  //@@  int todo_parse_properties_for_special_value;
  //@@ int todo_override_rather_than_replace_question;
}

void TAO::PG_Object_Group::get_properties (PortableGroup::Properties_var & result ACE_ENV_ARG_DECL) const
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // const cast to simulate mutable
  InternalGuard guard(ACE_const_cast (TAO::PG_Object_Group *, this)->internals_);
  this->properties_.export_properties(*result ACE_ENV_ARG_PARAMETER);
}


PortableGroup::TypeId TAO::PG_Object_Group::get_type_id () const
{
  // const cast to simulate mutable
  InternalGuard guard(ACE_const_cast (TAO::PG_Object_Group *, this)->internals_);
  return CORBA::string_dup(this->type_id_);
}


///////////////////
// Internal method

int TAO::PG_Object_Group::increment_version ()
{
  // assume internals is locked
  int result = 0;
  this->tagged_component_.object_group_ref_version += 1;
  if (TAO_debug_level > 3)
  {
    ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("%T %n (%P|%t) - Setting IOGR version to %u\n"),
      ACE_static_cast(unsigned, this->tagged_component_.object_group_ref_version)
      ));
  }

  // Set the version
  if ( TAO::PG_Utils::set_tagged_component (this->reference_, this->tagged_component_) )
  {
    result = 1;
  }
  return result;
}


//////////////////
// Internal method
void TAO::PG_Object_Group::distribute_iogr (ACE_ENV_ARG_DECL)
{
  // assume internals is locked
  CORBA::String_var iogr = this->orb_->object_to_string (this->reference_.in() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

//  size_t n_rep = 0; // for dump_ior below
  for ( MemberMap_Iterator it = this->members_.begin();
    it != this->members_.end();
    ++it)
  {
    MemberInfo const * info = (*it).int_id_;
    //
    // Unchecked narrow means the member doesn't have to actually implement the TAO_UpdateObjectGroup interface
    // PortableGroup::TAO_UpdateObjectGroup_var uog = PortableGroup::TAO_UpdateObjectGroup::_unchecked_narrow ( info->member_);
    // but it doesn work: error message at replica is:
    // TAO-FT (2996|976) - Wrong version information within the interceptor [1 | 0]
    // TAO_Perfect_Hash_OpTable:find for operation 'tao_update_object_group' (length=23) failed
    // back to using _narrow
    PortableGroup::TAO_UpdateObjectGroup_var uog = PortableGroup::TAO_UpdateObjectGroup::_narrow ( info->member_.in ());
    if (! CORBA::is_nil (uog.in ()) )
    {
      ACE_TRY_NEW_ENV
      {
        if (TAO_debug_level > 3)
        {
          ACE_DEBUG ((LM_DEBUG,
            "PG (%P|%t) -  Object_Group pushing IOGR to %s member: %s@%s.\n",
            (info->is_primary_ ? "Primary" : "Backup"),
            this->role_.c_str(),
            ACE_static_cast(const char *, info->location_[0].id)
            ));
        }
//        dump_ior ("group", "iogr", this->tagged_component_.object_group_ref_version, iogr);
//        CORBA::String_var replica_ior = this->orb_->object_to_string(uog.in() ACE_ENV_ARG_PARAMETER);
//        dump_ior (info->location_[0].id, "ior", (this->tagged_component_.object_group_ref_version * 100) + n_rep++, replica_ior);
        uog->tao_update_object_group (iogr, this->tagged_component_.object_group_ref_version, info->is_primary_ ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

      }
      ACE_CATCHANY
      {
        // we expect an exception
        // tao_update_object_group is not a real method
      }
      ACE_ENDTRY;
    }
    else
    {
      ACE_ERROR ((LM_ERROR,
        "TAO::PG_Object_Group::distribute iogr can't narrow member reference to PortableGroup::TAO_UpdateObjectGroup.\n"
        ));
    }
  }
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

  template class ACE_Hash_Map_Manager_Ex <
    PortableGroup::Location,
    TAO::PG_Object_Group::MemberInfo *,
    TAO_PG_Location_Hash,
    TAO_PG_Location_Equal_To,
    TAO_SYNCH_MUTEX>;
  template class ACE_Hash_Map_Entry <
    PortableGroup::Location,
    TAO::PG_Object_Group::MemberInfo *>;
  template class ACE_Hash_Map_Iterator_Ex <
    PortableGroup::Location,
    TAO::PG_Object_Group::MemberInfo *,
    TAO_PG_Location_Hash,
    TAO_PG_Location_Equal_To,
    TAO_SYNCH_MUTEX>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

# pragma instantiate ACE_Hash_Map_Manager_Ex <
    PortableGroup::Location,
    TAO::PG_Object_Group::MemberInfo *,
    TAO_PG_Location_Hash,
    TAO_PG_Location_Equal_To,
    TAO::PG_Object_Group::MemberMapMutex>
# pragma instantiate ACE_Hash_Map_Entry <PortableGroup::Location, TAO::PG_Object_Group::MemberInfo *>
# pragma instantiate ACE_Hash_Map_Iterator_Ex <
    PortableGroup::Location,
    TAO::PG_Object_Group::MemberInfo *,
    TAO_PG_Location_Hash,
    TAO_PG_Location_Equal_To,
    TAO::PG_Object_Group::MemberMapMutex>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

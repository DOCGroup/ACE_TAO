// $Id$

#include "orbsvcs/PortableGroup/PG_Object_Group.h"
#include "orbsvcs/PortableGroup/PG_conf.h"

#include "orbsvcs/PortableGroup/PG_Operators.h" // Borrow operator== on CosNaming::Name
#include "orbsvcs/PortableGroup/PG_Utils.h"

#include "tao/debug.h"

#include "ace/Get_Opt.h"
#include "ace/Vector_T.h"


// Borland Developer Studio 2006 and earlier give a warning about comparing
// signed and unsigned values in the minimum_polulate() and
// initial_populate() warnings. The comparison uses a unsigned long and
// unsigned short and it seems that the compiler promotes the unsigned
// short of an int and this then gives the warning. Just for Borland
// disabled the warning in this file.
#if defined (__BORLANDC__) && (__BORLANDC__ <= 0x582)
# pragma option push -w-csu
# pragma nopushoptwarn
# pragma nopackwarning
#endif /* __BORLANDC__ && __BORLANDC__ <= 0x582 */

#define TODO int todo;

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::PG_Object_Group::MemberInfo::MemberInfo (
    CORBA::Object_ptr member,
    const PortableGroup::Location & location)
  : member_ (CORBA::Object::_duplicate (member))
  , factory_(PortableGroup::GenericFactory::_nil ())
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

TAO::PG_Object_Group::MemberInfo::~MemberInfo (void)
{
  if( ! CORBA::is_nil (this->factory_.in()))
    {
      this->factory_->delete_object (this->factory_id_);
    }
}


TAO::PG_Object_Group::PG_Object_Group (
  CORBA::ORB_ptr orb,
  PortableGroup::FactoryRegistry_ptr factory_registry,
  TAO::PG_Object_Group_Manipulator & manipulator,
  CORBA::Object_ptr empty_group,
  const PortableGroup::TagGroupTaggedComponent & tagged_component,
  const char * type_id,
  const PortableGroup::Criteria & the_criteria,
  TAO::PG_Property_Set * type_properties)
  : internals_()
  , orb_ (CORBA::ORB::_duplicate (orb))
  , factory_registry_ (PortableGroup::FactoryRegistry::_duplicate (factory_registry))
  , manipulator_ (manipulator)
  , empty_ (1)
  , role_ (type_id)
  , type_id_ (CORBA::string_dup (type_id))
  , tagged_component_ (tagged_component)
  , reference_ (CORBA::Object::_duplicate(empty_group))
  , members_ ()
  , primary_location_(0)
  , properties_ (the_criteria, type_properties)
  , initial_number_members_ (0)
  , minimum_number_members_ (0)
  , group_specific_factories_ ()
{
}

TAO::PG_Object_Group::~PG_Object_Group (void)
{
  for (MemberMap_Iterator it = this->members_.begin();
      it != this->members_.end();
      ++it)
    {
      MemberInfo * member = (*it).int_id_;
      delete member;
    }
  this->members_.unbind_all ();
}

#if 0   // may want this again someday
/////////////////////
// q&d debug function
static void
dump_ior (const char * base,
          const char * ext,
          unsigned long version,
          const char * iogr)
{
  char filename[1000];
  ACE_OS::sprintf(filename, "%s_%lu.%s", base, version, ext );

  FILE * iorfile = ACE_OS::fopen(filename, "w");
  ACE_OS::fwrite (iogr, 1, ACE_OS::strlen(iogr), iorfile);
  ACE_OS::fclose (iorfile);
}
#endif  // may want this again someday

PortableGroup::ObjectGroup_ptr
TAO::PG_Object_Group::reference (void) const
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    guard,
                    this->internals_,
                    PortableGroup::ObjectGroup::_nil ());
  return PortableGroup::ObjectGroup::_duplicate (this->reference_);
}

void
TAO::PG_Object_Group::get_group_specific_factories (
  PortableGroup::FactoryInfos & result) const
{
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->internals_);

  // copy is needed to have some semblance of thread safeness.
  // if performance is an issue avoid this method.
  result = this->group_specific_factories_;
}

const PortableGroup::Location &
TAO::PG_Object_Group::get_primary_location (void) const
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    guard,
                    this->internals_,
                    this->primary_location_);
  return this->primary_location_;
}


PortableGroup::ObjectGroup_ptr
TAO::PG_Object_Group::add_member_to_iogr (CORBA::Object_ptr member
                                          ACE_ENV_ARG_DECL)
{
  // assume internals is locked

  PortableGroup::ObjectGroup_var result;

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
                                            this->reference_.in ()
                                            ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (PortableGroup::ObjectGroup::_nil());
      this->empty_ = 0;
    }

  // create a list of references to be merged
  TAO_IOP::TAO_IOR_Manipulation::IORList iors (2);
  iors.length (2);
  iors [0] = CORBA::Object::_duplicate (cleaned.in());
  iors [1] = CORBA::Object::_duplicate (member);

  // Now merge the list into one new IOGR
  result =
    this->manipulator_.merge_iors (iors ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (PortableGroup::ObjectGroup::_nil ());
  return result._retn ();
}

void
TAO::PG_Object_Group::add_member (const PortableGroup::Location & the_location,
                                  CORBA::Object_ptr member
                                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectNotAdded))

{
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->internals_);

  /////////////////////////////////////////
  // Convert the new member to a string IOR
  // This keeps a clean IOR (not an IOGR!)
  // while we add it to a group.  We need a
  // IORs, not IOGRs to send new IOGRs out
  // to replicas.

  CORBA::String_var member_ior_string =
    orb_->object_to_string (member
                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  PortableGroup::ObjectGroup_var new_reference =
    add_member_to_iogr (member
                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Convert new member back to a (non group) ior.
  CORBA::Object_var member_ior =
    this->orb_->string_to_object (member_ior_string.in ()
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  MemberInfo * info = 0;
  ACE_NEW_THROW_EX (info,
                    MemberInfo (member_ior.in (),
                                the_location),
                    CORBA::NO_MEMORY());

  if (this->members_.bind (the_location, info) != 0)
    {
      // @@ Dale why this is a NO MEMORY exception?
      ACE_THROW(CORBA::NO_MEMORY());
    }

  this->reference_ = new_reference; // note var-to-var assignment does
                                    // a duplicate
  if (this->increment_version ())
    {
      this->distribute_iogr (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    {
      ACE_THROW (PortableGroup::ObjectNotAdded ());
    }

  if (TAO_debug_level > 6)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("PG (%P|%t) exit Object_Group add_member \n")));
    }
}

int
TAO::PG_Object_Group::set_primary_member (
    TAO_IOP::TAO_IOR_Property * prop,
    const PortableGroup::Location & the_location
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::MemberNotFound))
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    guard,
                    this->internals_,
                    0);
  int result = 1;
  MemberInfo * info = 0;
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

      int set_ok =
        this->manipulator_.set_primary (prop,
                                        this->reference_.in (),
                                        info->member_.in ()
                                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
      if (!set_ok)
        {
          if (TAO_debug_level > 3)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("%T %n (%P|%t) - ")
                          ACE_TEXT ("Can't set primary in IOGR .\n")
                          ));
            }
//@@: ACE_THROW (FT::PrimaryNotSet());
          result = 0;
        }

      if (result && this->increment_version ())
        {
          this->distribute_iogr (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);
        }
      else
        {
          if (TAO_debug_level > 3)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT("TAO-PG (%P|%t) - set_primary_location ")
                          ACE_TEXT("throwing PrimaryNotSet because increment")
                          ACE_TEXT("version failed.\n")
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
                      ACE_TEXT ("TAO-PG (%P|%t) - set_primary_location ")
                      ACE_TEXT ("throwing MemberNotFound.\n")));
        }
      ACE_THROW_RETURN (PortableGroup::MemberNotFound(),
                        -1);
    }

  return result;
}


void
TAO::PG_Object_Group::remove_member (
    const PortableGroup::Location & the_location
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ( (CORBA::SystemException,
                   PortableGroup::MemberNotFound))
{
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->internals_);
  MemberInfo * info = 0;
  if (this->members_.unbind (the_location, info) == 0)
    {
      if (this->members_.current_size() > 0)
        {
          this->reference_ =
            this->manipulator_.remove_profiles (this->reference_.in (),
                                                info->member_.in ()
                                                ACE_ENV_ARG_PARAMETER);
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
                      "TAO-PG (%P|%t) - "
                      "remove_member throwing MemberNotFound.\n"
                      ));
        }
      ACE_THROW (PortableGroup::MemberNotFound() );
    }
}


PortableGroup::ObjectGroupId
TAO::PG_Object_Group::get_object_group_id (void) const
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    guard,
                    this->internals_,
                    0);
  return this->tagged_component_.object_group_id;
}

void
TAO::PG_Object_Group::set_properties_dynamically (
    const PortableGroup::Properties & overrides
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::InvalidProperty,
                   PortableGroup::UnsupportedProperty))
{
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->internals_);

  this->properties_.decode (overrides ACE_ENV_ARG_PARAMETER);

  //@@ int todo_override_rather_than_replace?
}

void
TAO::PG_Object_Group::get_properties (
    PortableGroup::Properties_var & result) const
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->internals_);
  this->properties_.export_properties(*result);
}


PortableGroup::TypeId
TAO::PG_Object_Group::get_type_id (void) const
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    guard,
                    this->internals_,
                    0);
  return CORBA::string_dup (this->type_id_);
}


///////////////////
// Internal method

int
TAO::PG_Object_Group::increment_version (void)
{
  // assume internals is locked
  int result = 0;
  this->tagged_component_.object_group_ref_version += 1;
  if (TAO_debug_level > 3)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%T %n (%P|%t) - Setting IOGR version to %u\n"),
                  static_cast<unsigned> (this->tagged_component_.object_group_ref_version)
                  ));
    }

  // Set the version
  if (TAO::PG_Utils::set_tagged_component (this->reference_,
                                           this->tagged_component_))
    {
      result = 1;
    }
  return result;
}


//////////////////
// Internal method
void
TAO::PG_Object_Group::distribute_iogr (ACE_ENV_SINGLE_ARG_DECL)
{
  // assume internals is locked
  CORBA::String_var iogr =
    this->orb_->object_to_string (this->reference_.in()
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

//  size_t n_rep = 0; // for dump_ior below
  for (MemberMap_Iterator it = this->members_.begin();
       it != this->members_.end ();
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
      PortableGroup::TAO_UpdateObjectGroup_var uog =
        PortableGroup::TAO_UpdateObjectGroup::_narrow ( info->member_.in ());
      if (!CORBA::is_nil (uog.in ()))
        {
          ACE_TRY_NEW_ENV
            {
              if (TAO_debug_level > 3)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              "PG (%P|%t) -  Object_Group pushing "
                              "IOGR to %s member: %s@%s.\n",
                              (info->is_primary_ ? "Primary" : "Backup"),
                              this->role_.c_str (),
                              static_cast<const char *> (info->location_[0].id)
                              ));
                }
              //        dump_ior ("group", "iogr", this->tagged_component_.object_group_ref_version, iogr);
              //        CORBA::String_var replica_ior = this->orb_->object_to_string(uog.in() ACE_ENV_ARG_PARAMETER);
              //        dump_ior (info->location_[0].id, "ior", (this->tagged_component_.object_group_ref_version * 100) + n_rep++, replica_ior);
              uog->tao_update_object_group (iogr.in (),
                                            this->tagged_component_.object_group_ref_version,
                                            info->is_primary_
                                            ACE_ENV_ARG_PARAMETER);
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
                      "TAO::PG_Object_Group::distribute iogr can't "
                      "narrow member reference to "
                      "PortableGroup::TAO_UpdateObjectGroup.\n"
                      ));
        }
    }
}

PortableGroup::Locations *
TAO::PG_Object_Group::locations_of_members (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    guard,
                    this->internals_,
                    0);

  PortableGroup::Locations * result = 0;

  size_t count = this->members_.current_size ();

  ACE_NEW_THROW_EX (
    result,
    PortableGroup::Locations (count),
    CORBA::NO_MEMORY() );
  ACE_CHECK_RETURN (0);

  result->length (count);

  size_t pos = 0;
  for (MemberMap_Iterator it = this->members_.begin();
      it != this->members_.end();
      ++it)
  {
    const PortableGroup::Location & location = (*it).ext_id_;
    PortableGroup::Location & out = (*result)[pos];
    out = location;
  }
  return result;
}

CORBA::Object_ptr
TAO::PG_Object_Group::get_member_reference (
    const PortableGroup::Location & the_location
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    PortableGroup::MemberNotFound))
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    guard,
                    this->internals_,
                    CORBA::Object::_nil ());

  CORBA::Object_var result;

  MemberInfo * info = 0;
  if (this->members_.find (the_location, info) == 0)
    {
      result = CORBA::Object::_duplicate (info->member_.in ());
    }
  else
    {
      ACE_THROW_RETURN (PortableGroup::MemberNotFound(), result._retn ());
    }
  return result._retn ();
}


PortableGroup::MembershipStyleValue
TAO::PG_Object_Group::get_membership_style (void) const
{
  PortableGroup::MembershipStyleValue membership_style = 0;
  if (!TAO::find (properties_,
                  PortableGroup::PG_MEMBERSHIP_STYLE,
                  membership_style))
    {
      membership_style = TAO_PG_MEMBERSHIP_STYLE;
    }
  return membership_style;
}


PortableGroup::MinimumNumberMembersValue
TAO::PG_Object_Group::get_minimum_number_members (void) const
{
  PortableGroup::MinimumNumberMembersValue minimum_number_members = 0;
  if (!TAO::find (properties_,
                  PortableGroup::PG_MINIMUM_NUMBER_MEMBERS,
                  minimum_number_members))
    {
      minimum_number_members = TAO_PG_MINIMUM_NUMBER_MEMBERS;
    }
  return minimum_number_members;
}

PortableGroup::InitialNumberMembersValue
TAO::PG_Object_Group::get_initial_number_members (void) const
{
  PortableGroup::InitialNumberMembersValue initial_number_members = 0;
  if (!TAO::find (properties_,
                  PortableGroup::PG_INITIAL_NUMBER_MEMBERS,
                  initial_number_members))
    {
      initial_number_members = TAO_PG_INITIAL_NUMBER_MEMBERS;
    }
  return initial_number_members;
}

void
TAO::PG_Object_Group::create_member (
    const PortableGroup::Location & the_location,
    const char * type_id,
    const PortableGroup::Criteria & the_criteria
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ( (CORBA::SystemException,
    PortableGroup::MemberAlreadyPresent,
    PortableGroup::NoFactory,
    PortableGroup::ObjectNotCreated,
    PortableGroup::InvalidCriteria,
    PortableGroup::CannotMeetCriteria))
{
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->internals_);


  if (0 != this->members_.find (the_location))
    {
      // @@ what if factories were passed as criteria?

      CORBA::String_var factory_type;
      PortableGroup::FactoryInfos_var factories =
        this->factory_registry_->list_factories_by_role (
          role_.c_str(),
          factory_type.out ()
          ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // @@ what if factory_type != type_id != this->type_id_

      int created = 0; // bool
      CORBA::ULong factory_count = factories->length ();
      for (CORBA::ULong factory_pos = 0;
           ! created && factory_pos < factory_count;
           ++factory_pos)
        {
          const PortableGroup::FactoryInfo & factory_info =
            (*factories)[factory_pos];
          if (factory_info.the_location == the_location)
            {
              // @@ should we merge the_criteria with
              // factory_info.the_criteria?

              PortableGroup::GenericFactory::FactoryCreationId_var fcid;
              CORBA::Object_var member =
                factory_info.the_factory->create_object (
                  type_id,
                  the_criteria,
                  fcid. out()
                  ACE_ENV_ARG_PARAMETER);
              ACE_CHECK;

              // convert the new member to a stringified IOR to avoid
              // contamination with group info
              CORBA::String_var member_ior_string =
                orb_->object_to_string (member.in ()
                                        ACE_ENV_ARG_PARAMETER);
              ACE_CHECK;

              PortableGroup::ObjectGroup_var new_reference =
                this->add_member_to_iogr (member.in ()
                                          ACE_ENV_ARG_PARAMETER);
              ACE_CHECK;

              // Convert new member back to a (non group) ior.
              CORBA::Object_var member_ior =
                this->orb_->string_to_object (member_ior_string.in ()
                                              ACE_ENV_ARG_PARAMETER);
              ACE_CHECK;

              MemberInfo * info = 0;
              ACE_NEW_THROW_EX (info, MemberInfo(
                                                 member_ior.in(),
                                                 the_location,
                                                 factory_info.the_factory,
                                                 fcid.in ()),
                                CORBA::NO_MEMORY());
              ACE_CHECK;

              if (this->members_.bind (the_location, info) != 0)
                {
                  ACE_THROW(CORBA::NO_MEMORY());
                }

              this->reference_ = new_reference; // note var-to-var
                                                // assignment does a
                                                // duplicate
              if (this->increment_version ())
                {
                  this->distribute_iogr (ACE_ENV_SINGLE_ARG_PARAMETER);
                  ACE_CHECK;
                }
              created = 1;
            }
        }
      if (! created)
        {
          ACE_THROW (PortableGroup::NoFactory ());
        }
    }
  else
    {
      ACE_THROW (PortableGroup::MemberAlreadyPresent ());
    }
}

void
TAO::PG_Object_Group::create_members (size_t count ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::NoFactory))
{
  // assume internals is locked
  // @@ what if factories were passed as criteria?

  CORBA::String_var factory_type;
  PortableGroup::FactoryInfos_var factories =
  this->factory_registry_->list_factories_by_role (
        role_.c_str(),
        factory_type.out ()
        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::ULong factory_count = factories->length ();
  if (factory_count > 0)
    {
      CORBA::ULong factory_pos = 0;
      while (members_.current_size () < count && factory_pos < factory_count)
        {
          const PortableGroup::FactoryInfo & factory_info =
            (*factories)[factory_pos];
          const PortableGroup::Location & factory_location =
            factory_info.the_location;
          if (0 != this->members_.find (factory_location))
            {
              ///////////////////////////////////////////
              // If a factory refuses to create a replica
              // it's not fatal.
              ACE_TRY_NEW_ENV
                {
                  PortableGroup::GenericFactory::FactoryCreationId_var fcid;
                  CORBA::Object_var member =
                    factory_info.the_factory->create_object (
                      this->type_id_.in (),
                      factory_info.the_criteria,
                      fcid. out()
                      ACE_ENV_ARG_PARAMETER);
                  ACE_TRY_CHECK;

                  // convert the new member to a stringified IOR to avoid
                  // contamination with group info
                  CORBA::String_var member_ior_string =
                    orb_->object_to_string (member.in ()
                                            ACE_ENV_ARG_PARAMETER);
                  ACE_TRY_CHECK;

                  PortableGroup::ObjectGroup_var new_reference =
                    this->add_member_to_iogr (member.in ()
                                              ACE_ENV_ARG_PARAMETER);
                  ACE_TRY_CHECK;

                  // Convert new member back to a (non group) ior.
                  CORBA::Object_var member_ior =
                    this->orb_->string_to_object (member_ior_string.in ()
                                                  ACE_ENV_ARG_PARAMETER);
                  ACE_TRY_CHECK;

                  MemberInfo * info = 0;
                  ACE_NEW_THROW_EX (info, MemberInfo(
                                                     member_ior.in(),
                                                     factory_location,
                                                     factory_info.the_factory,
                                                     fcid.in ()),
                                    CORBA::NO_MEMORY());
                  ACE_TRY_CHECK;

                  if (this->members_.bind (factory_location, info) != 0)
                    {
                      ACE_TRY_THROW(CORBA::NO_MEMORY());
                    }
                  this->reference_ =
                    new_reference; // note var-to-var assignment does
                                   // a duplicate
                }
              ACE_CATCHANY
                {
                  // log, but otherwise ignore the errorf
                  if (TAO_debug_level > 0)
                    {
                      ACE_ERROR ((LM_ERROR,
                                  ACE_TEXT ("PG (%P|%t) Replica Factory ")
                                  ACE_TEXT ("@ %s refused create_object ")
                                  ACE_TEXT ("request for type %s\n"),
                                  static_cast<const char *> (factory_info.the_location[0].id),
                                  static_cast<const char *> (this->type_id_.in ())
                                  ));
                    }
                }
              ACE_ENDTRY;
            }
        }

      if (this->increment_version ())
        {
          this->distribute_iogr (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;
        }
    }
  else
    {
      ACE_THROW (PortableGroup::NoFactory());
    }
}

void
TAO::PG_Object_Group::initial_populate (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->internals_);

  if (this->get_membership_style () == PortableGroup::MEMB_INF_CTRL)
    {
      PortableGroup::InitialNumberMembersValue initial_number_members =
        this->get_initial_number_members ();

      if (this->members_.current_size () < initial_number_members)
        {
          this->create_members (initial_number_members
                                ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }
    }
}

void
TAO::PG_Object_Group::minimum_populate (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->internals_);

  if ( this->get_membership_style () == PortableGroup::MEMB_INF_CTRL )
    {
      PortableGroup::MinimumNumberMembersValue minimum_number_members =
        this->get_minimum_number_members ();
      if (members_.current_size () < minimum_number_members)
        {
          this->create_members (minimum_number_members ACE_ENV_ARG_PARAMETER);
        }
    }
}

int
TAO::PG_Object_Group::has_member_at (const PortableGroup::Location & location)
{
  return (0 == this->members_.find (location));
}

TAO_END_VERSIONED_NAMESPACE_DECL

// Restore original compiler flags.
#if defined (__BORLANDC__) && (__BORLANDC__ <= 0x582)
# pragma option pop
# pragma nopushoptwarn
# pragma nopackwarning
#endif /* __BORLANDC__ && __BORLANDC__ <= 0x582 */

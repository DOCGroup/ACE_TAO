// -*- C++ -*-
//
// $Id$

#include "PG_Object_Group.h"

#include <ace/Get_Opt.h>
#include <ace/Vector_T.h>
#include <tao/PortableServer/ORB_Manager.h>
#include <orbsvcs/PortableGroup/PG_Operators.h> // Borrow operator == on CosNaming::Name

/**
 * The single POA used to manage object groups
 */
//static
PortableServer::POA_var TAO::PG_Object_Group::poa_;


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
  if( ! CORBA::is_nil (factory_.in()))
  {
    ACE_TRY_NEW_ENV
    {
      factory_->delete_object (factory_id_);
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
  PortableGroup::ObjectGroupId oid,
  const char * type_id,
  PortableGroup::Criteria the_criteria)
    : orb_ (CORBA::ORB::_duplicate (orb))
    , role_ (type_id)
    , type_id_ (CORBA::string_dup(type_id))
    , group_id_(oid)
    , reference_ (0)
    , primary_location_(0)
    , properties_ (the_criteria)
{
}

TAO::PG_Object_Group::~PG_Object_Group ()
{
  for (MemberMap_Iterator it = this->members_.begin();
      it != this->members_.end();
      this->members_.begin())
  {
    MemberInfo * member = (*it).int_id_;
    delete member;
    members_.unbind((*it).ext_id_);
  }
}


void dump_ior (const char * base, const char * ext, unsigned long version, const char * iogr)
{
  char filename[1000];
  sprintf(filename, "%s_%lu.%s", base, version, ext );

  FILE * iorfile = fopen(filename, "w");
  fwrite (iogr, 1, strlen(iogr), iorfile);
  fclose (iorfile);
}


void TAO::PG_Object_Group::set_reference (
  PortableGroup::ObjectGroup_ptr reference,
  PortableGroup::ObjectGroupRefVersion version,
  int distribute
  ACE_ENV_ARG_DECL)
{
  this->reference_ = PortableGroup::ObjectGroup::_duplicate (reference);
  this->version_ = version;
  this->IOGR_ = this->orb_->object_to_string (reference ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (distribute)
  {
    size_t n_rep = 0; // for dump_ior below
    for ( MemberMap_Iterator it = members_.begin();
      it != members_.end();
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
      PortableGroup::TAO_UpdateObjectGroup_var uog = PortableGroup::TAO_UpdateObjectGroup::_narrow ( info->member_);
      if (! CORBA::is_nil (uog.in ()) )
      {
        ACE_TRY_NEW_ENV
        {
          ACE_DEBUG ((LM_DEBUG,
            "PG (%P|%t) -  Object_Group pushing IOGR to  %s member: %s@%s.\n",
            (info->is_primary_ ? "Primary" : "Backup"),
            this->role_.c_str(),
            ACE_static_cast(const char *, info->location_[0].id)
            ));
          dump_ior ("group", "iogr", this->version_, this->IOGR_);
          CORBA::String_var replica_ior = this->orb_->object_to_string(uog.in() ACE_ENV_ARG_PARAMETER);
          dump_ior (info->location_[0].id, "ior", (this->version_ * 100) + n_rep++, replica_ior);

          uog->tao_update_object_group (this->IOGR_, this->version_, info->is_primary_);
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
          "TAO::PG_Object_Group::set_reference can't narrow member reference to PortableGroup::TAO_UpdateObjectGroup.\n"
          ));
      }
    }
  }
}


PortableGroup::ObjectGroup_ptr TAO::PG_Object_Group::reference()const
{
  return PortableGroup::ObjectGroup::_duplicate (this->reference_);
}

void TAO::PG_Object_Group::set_membership_style (PortableGroup::MembershipStyleValue style)
{
  this->membership_style_ = style;
}

PortableGroup::MembershipStyleValue TAO::PG_Object_Group::membership_style () const
{
  return this->membership_style_;
}

void TAO::PG_Object_Group::initial_number_members (PortableGroup::InitialNumberMembersValue count)
{
  this->initial_number_members_ = count;
}

PortableGroup::InitialNumberMembersValue TAO::PG_Object_Group::initial_number_members () const
{
  return this->initial_number_members_;
}

void TAO::PG_Object_Group::set_minimum_number_members (PortableGroup::MinimumNumberMembersValue count)
{
  this->minimum_number_members_ = count;
}

PortableGroup::MinimumNumberMembersValue TAO::PG_Object_Group::minimum_number_members ()const
{
  return this->minimum_number_members_;
}

void TAO::PG_Object_Group::set_group_specific_factories (const PortableGroup::FactoryInfos & infos)
{
  this->group_specific_factories_ = infos;
}


void TAO::PG_Object_Group::group_specific_factories (PortableGroup::FactoryInfos & result) const
{
  // copy is needed to have some semblance of thread safeness.
  // if performance is an issue avoid this method.
  result = this->group_specific_factories_;
}


void TAO::PG_Object_Group::set_primary_location (
    const PortableGroup::Location & location
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((PortableGroup::MemberNotFound))
{
  MemberInfo * info;
  if (this->members_.find (location, info) == 0)
  {
    int cleared = 0;
    this->primary_location_ = location;
    for (MemberMap_Iterator it = this->members_.begin();
        !cleared && it != this->members_.end();
        ++it)
    {
      cleared = (*it).int_id_->is_primary_;
      (*it).int_id_->is_primary_ = 0;
    }
    info->is_primary_ = 1;
  }
  else
  {
    ACE_DEBUG ((LM_DEBUG,
      "TAO-PG (%P|%t) - set_primary_location throwing MemberNotFound.\n"
      ));
    ACE_THROW (PortableGroup::MemberNotFound())
    ACE_CHECK;
  }
}

const PortableGroup::Location & TAO::PG_Object_Group::primary_location() const
{
  return this->primary_location_;
}

#ifdef NOT_IMPLEMENTED
void TAO::PG_Object_Group::create_member (
    const PortableGroup::Location & the_location,
    const char * type_id,
    const PortableGroup::Criteria & the_criteria
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    PortableGroup::MemberAlreadyPresent,
    PortableGroup::ObjectNotCreated,
    PortableGroup::InvalidCriteria,
    PortableGroup::CannotMeetCriteria))
{
  ACE_ASSERT(0);
  int todo;
}
#endif

void TAO::PG_Object_Group::add_member (
    const PortableGroup::Location & the_location,
    CORBA::Object_ptr member
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    PortableGroup::MemberAlreadyPresent,
    PortableGroup::ObjectNotAdded))
{
  MemberInfo * info = 0;
  ACE_NEW_THROW_EX (info, MemberInfo(member, the_location),
    CORBA::NO_MEMORY());

  if (members_.bind (the_location, info) != 0)
  {
    ACE_THROW(CORBA::NO_MEMORY());
    ACE_CHECK;
  }
}

#ifdef NOT_IMPLEMENTED
void TAO::PG_Object_Group::remove_member (
    const PortableGroup::Location & the_location
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    PortableGroup::MemberNotFound))
{
  ACE_ASSERT(0);
  int todo;
}
#endif

#ifdef NOT_IMPLEMENTED
void TAO::PG_Object_Group::locations_of_members (
    PortableGroup::Locations & locations
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_ASSERT(0);
  int todo;
}
#endif

PortableGroup::ObjectGroupId TAO::PG_Object_Group::get_object_group_id () const
{
  return this->group_id_;
}

#ifdef NOT_IMPLEMENTED

CORBA::Object_ptr TAO::PG_Object_Group::get_member_ref (
    const PortableGroup::Location & loc
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    PortableGroup::MemberNotFound))
{
  CORBA::Object_ptr result = CORBA::Object::_nil();
  ACE_ASSERT(0);
  int todo;
  return result;
}
#endif

void TAO::PG_Object_Group::set_properties_dynamically (
    const PortableGroup::Properties & overrides
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::InvalidProperty,
                   PortableGroup::UnsupportedProperty))
{
  this->properties_ = overrides;
  //@@  int todo_parse_properties_for_special_value;
  //@@ int todo_override_rather_than_replace_question;
}

void TAO::PG_Object_Group::get_properties (PortableGroup::Properties_var & result) const
{
  (*result) = this->properties_;
}


PortableGroup::TypeId TAO::PG_Object_Group::get_type_id () const
{
  return CORBA::string_dup(this->type_id_);
}



#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

  template ACE_Hash_Map_Manager_Ex <PortableGroup::Location, MemberInfo *, TAO_PG_Location_Hash, TAO_PG_Location_Equal_To, MapMutex>;
  template ACE_Hash_Map_Entry <PortableGroup::Location, MemberInfo *>;
  template ACE_Hash_Map_Iterator_Ex <PortableGroup::Location, MemberInfo *, TAO_PG_Location_Hash, TAO_PG_Location_Equal_To, MapMutex>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

# pragma instantiate ACE_Hash_Map_Manager_Ex <PortableGroup::Location, MemberInfo *, TAO_PG_Location_Hash, TAO_PG_Location_Equal_To, MapMutex>
# pragma instantiate ACE_Hash_Map_Entry <PortableGroup::Location, MemberInfo *>
# pragma instantiate ACE_Hash_Map_Iterator_Ex <PortableGroup::Location, MemberInfo *, TAO_PG_Location_Hash, TAO_PG_Location_Equal_To, MapMutex>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

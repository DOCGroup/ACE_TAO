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


TAO::PG_Object_Group::MemberInfo::MemberInfo (CORBA::Object_var member)
  : member_(member)
  , factory_(PortableGroup::GenericFactory::_nil())
{
}

TAO::PG_Object_Group::MemberInfo::MemberInfo (
    CORBA::Object_var member, 
    PortableGroup::GenericFactory_var factory, 
    PortableGroup::GenericFactory::FactoryCreationId factory_id)
  : member_(member)
  , factory_(factory)
  , factory_id_(factory_id)
{
}

TAO::PG_Object_Group::MemberInfo::~MemberInfo ()
{
  if( ! CORBA::is_nil (factory_))
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

TAO::PG_Object_Group::PG_Object_Group ()
    : role_ ("")
    , type_id_ ("")
    , group_id_( 0)
    , reference_ (0)
    , primary_location_(0)
    , properties_ (0)
{
}


TAO::PG_Object_Group::PG_Object_Group (
  PortableGroup::ObjectGroupId oid,
  const char * type_id,
  PortableGroup::Criteria the_criteria)
    : role_ (type_id)
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

void TAO::PG_Object_Group::set_reference (PortableGroup::ObjectGroup_ptr reference, int distribute)
{
  this->reference_ = PortableGroup::ObjectGroup::_duplicate (reference);
  if (distribute)
  {
    for ( MemberMap_Iterator it = members_.begin();
      it != members_.end();
      ++it)
    {
      MemberInfo const * info = (*it).int_id_;
      PortableGroup::TAO_UpdateObjectGroup_var uog = PortableGroup::TAO_UpdateObjectGroup::_narrow ( info->member_);
      if (! CORBA::is_nil (uog) )
      {
        ACE_TRY_NEW_ENV
        {
          uog->tao_update_object_group (PortableGroup::ObjectGroup::_duplicate (this->reference_));
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

void TAO::PG_Object_Group::initial_number_replicas (PortableGroup::InitialNumberReplicasValue count)
{
  this->initial_number_replicas_ = count;
}

PortableGroup::InitialNumberReplicasValue TAO::PG_Object_Group::initial_number_replicas () const
{
  return this->initial_number_replicas_;
}

void TAO::PG_Object_Group::set_minimum_number_replicas (PortableGroup::MinimumNumberReplicasValue count)
{
  this->minimum_number_replicas_ = count;
}

PortableGroup::MinimumNumberReplicasValue TAO::PG_Object_Group::minimum_number_replicas ()const
{
  return this->minimum_number_replicas_;
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


void TAO::PG_Object_Group::set_primary_location (PortableGroup::Location & location)
{
  this->primary_location_ = location;
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
  ACE_NEW_THROW_EX (info, MemberInfo(member),
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

#ifdef NOT_IMPLEMENTED
void TAO::PG_Object_Group::set_properties_dynamically (
    const PortableGroup::Properties & overrides
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::InvalidProperty,
                   PortableGroup::UnsupportedProperty))
{
  int todo;
}
#endif

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

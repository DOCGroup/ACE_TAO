// -*- C++ -*-

//=============================================================================
/**
 * @file  PG_Group_Factory.cpp
 *
 * $Id$
 *
 * @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================

#include "PG_Group_Factory.h"
#include "PG_Property_Utils.h"
#include "PG_conf.h"
#include "orbsvcs/PortableGroupC.h"
#include "PG_Object_Group.h"
#include <orbsvcs/orbsvcs/PortableGroup/PG_Utils.h>

//#include "PG_Properties_Encoder.h"
//#include "PG_Properties_Decoder.h"


ACE_RCSID (PortableGroup,
           PG_Group_Factory,
           "$Id$")


TAO::PG_Group_Factory::PG_Group_Factory ()
  : orb_ (CORBA::ORB::_nil())
  , poa_ (PortableServer::POA::_nil())
  , manipulator_ ()
  , domain_id_ ("default-domain")

{
}

TAO::PG_Group_Factory::~PG_Group_Factory (void)
{
  // todo: destroy all members
}


void TAO::PG_Group_Factory::init (
  CORBA::ORB_ptr orb,
  PortableServer::POA_ptr poa ACE_ENV_ARG_DECL)
{
  ACE_ASSERT (CORBA::is_nil (this->orb_.in ()));
  ACE_ASSERT (CORBA::is_nil (this->poa_.in ()));

  this->orb_ = CORBA::ORB::_duplicate(orb);
  this->poa_ = PortableServer::POA::_duplicate (poa);

  ACE_ASSERT (!CORBA::is_nil (this->orb_.in ()));
  ACE_ASSERT (!CORBA::is_nil (this->poa_.in ()));

  this->manipulator_.init (orb, poa ACE_ENV_ARG_PARAMETER);
//  ACE_CHECK;
}


TAO::PG_Object_Group * TAO::PG_Group_Factory::create_group (
    const char * type_id,
    const PortableGroup::Criteria & the_criteria,
    TAO_PG::Properties_Decoder * typeid_properties
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::NoFactory,
                   PortableGroup::ObjectNotCreated,
                   PortableGroup::InvalidCriteria,
                   PortableGroup::InvalidProperty,
                   PortableGroup::CannotMeetCriteria))
{
  ///////////////////////////////////
  // Create an empty group reference

  PortableGroup::ObjectGroupId group_id = 0;
  PortableGroup::ObjectGroup_var empty_group =
    this->manipulator_.create_object_group (
      type_id,
      this->domain_id_,
      group_id
      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // pick up the object group information as assigned by
  // ObjectGroupManager

  PortableGroup::TagGroupTaggedComponent tagged_component;
  if (! TAO::PG_Utils::get_tagged_component (empty_group, tagged_component))
  {
    ACE_THROW_RETURN (PortableGroup::ObjectNotCreated(), 0);
  }

  TAO::PG_Object_Group * objectGroup = 0;

  ACE_NEW_THROW_EX (
    objectGroup,
    TAO::PG_Object_Group (
      this->orb_.in (),
//      this->poa_.in (),
      this->manipulator_,
      empty_group.in (),
      tagged_component,
      type_id,
      the_criteria,
      typeid_properties
      ),
    CORBA::NO_MEMORY());

  if (this->group_map_.bind (group_id, objectGroup) != 0)
  {
    delete objectGroup;
    ACE_THROW_RETURN (PortableGroup::ObjectNotCreated(), 0);
  }
  return objectGroup;
}


#if 0
Initialize object group

  PortableGroup::ObjectGroup_var object_group = PortableGroup::ObjectGroup::_nil();

  PortableGroup::Properties_var properties =
    this->property_manager_.get_type_properties (type_id
                                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  PortableGroup::MembershipStyleValue membership_style =
    TAO_PG_MEMBERSHIP_STYLE;
  PortableGroup::FactoriesValue factory_infos(0);

  PortableGroup::InitialNumberMembersValue initial_number_members =
    TAO_PG_INITIAL_NUMBER_MEMBERS;
  PortableGroup::MinimumNumberMembersValue minimum_number_members =
    TAO_PG_MINIMUM_NUMBER_MEMBERS;

  // Make sure the criteria for the object group being created are
  // valid.
  this->process_criteria (type_id,
                          the_criteria,
                          membership_style,
                          factory_infos,
                          initial_number_members,
                          minimum_number_members
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());


  PortableGroup::ObjectGroupId group_id;
//  PortableGroup::ObjectGroup_var
  object_group =
    this->object_group_manager_.create_object_group (type_id,               // in
                                                     this->domain_id_,      // in
                                                     the_criteria,          // in
                                                     group_id               // out
                                                     ACE_ENV_ARG_PARAMETER);

  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  // Allocate a new FactoryCreationId for use as an "out" parameter.
  PortableGroup::GenericFactory::FactoryCreationId_var factory_id = 0;
  ACE_NEW_THROW_EX (factory_id,
                    PortableGroup::GenericFactory::FactoryCreationId,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::Object::_nil ());
  *factory_id <<= group_id;


  ACE_CHECK_RETURN (CORBA::Object::_nil ());


  TAO_PG_Factory_Set factory_set;

  const CORBA::ULong factory_infos_count = factory_infos.length ();

  ACE_TRY
    {
      if (factory_infos_count > 0
          && membership_style == PortableGroup::MEMB_INF_CTRL)
        {
          this->populate_object_group (object_group.in (),
                                       type_id,
                                       factory_infos,
                                       minimum_number_members,
                                       factory_set
                                       ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;


          ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                            guard,
                            this->lock_,
                            CORBA::Object::_nil ());


          if (this->factory_map_.bind (group_id, factory_set) != 0)
             ACE_TRY_THROW (PortableGroup::ObjectNotCreated ());
        }
    }
  ACE_CATCHANY
    {
      this->delete_object_i (factory_set,
                             1 /* Ignore exceptions */
                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->object_group_manager_.destroy_object_group (group_id
                                                        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  factory_creation_id = factory_id._retn();
  return result;
}
#endif


void TAO::PG_Group_Factory::delete_group (PortableGroup::ObjectGroup_ptr object_group
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectNotFound))
{
  if (! destroy_group (object_group))
  {
    ACE_THROW (PortableGroup::ObjectNotFound ());
  }
}


void TAO::PG_Group_Factory::delete_group (PortableGroup::ObjectGroupId group_id
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectNotFound))
{
  if (! destroy_group (group_id))
  {
    ACE_THROW (PortableGroup::ObjectNotFound ());
  }
}




PortableGroup::GenericFactory::FactoryCreationId *
TAO::PG_Group_Factory::create_member (
    PortableGroup::ObjectGroup_ptr object_group,
    const PortableGroup::FactoryInfo & factory_info,
    const char * type_id,
    const CORBA::Boolean propagate_member_already_present
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::NoFactory,
                   PortableGroup::ObjectNotCreated,
                   PortableGroup::InvalidCriteria,
                   PortableGroup::InvalidProperty,
                   PortableGroup::CannotMeetCriteria,
                   PortableGroup::MemberAlreadyPresent))
{
  PortableGroup::GenericFactory::FactoryCreationId_var fcid;

#if 0
  CORBA::Object_var member =
    factory_info.the_factory->create_object (type_id,
                                             factory_info.the_criteria,
                                             fcid.out ()
                                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  ACE_TRY
    {
      // @@ Should an "_is_a()" be performed here?  While it
      //    appears to be the right thing to do, it can be
      //    expensive.
      //
      // Make sure an Object of the correct type was created.
      // It is possible that an object of the wrong type was
      // created if the type_id parameter does not match the
      // type of object the GenericFactory creates.
      CORBA::Boolean right_type_id =
        member->_is_a (type_id
                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // @todo Strategize this -- e.g. strict type checking.
      if (!right_type_id)
        {
          // An Object of incorrect type was created.  Delete
          // it, and throw a NoFactory exception.
          factory_info.the_factory->delete_object (fcid.in ()
                                                   ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          ACE_TRY_THROW (PortableGroup::NoFactory (factory_info.the_location,
                                                   type_id));
        }

      this->object_group_manager_._tao_add_member (
        object_group,
        factory_info.the_location,
        member.in (),
        type_id,
        propagate_member_already_present
        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // If the member reference is not nil, then the factory
      // was successfully invoked.  Since an exception was
      // thrown, clean up the up created member.
      if (!CORBA::is_nil (member.in ()))
        {
          factory_info.the_factory->delete_object (fcid.in ()
                                                   ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
#endif
  return fcid._retn ();
}

    // insert group.  Take ownership
int TAO::PG_Group_Factory::insert_group ( ::TAO::PG_Object_Group * group)
{
  return insert_group (group->get_object_group_id(), group);
}

int TAO::PG_Group_Factory::insert_group (PortableGroup::ObjectGroupId group_id, ::TAO::PG_Object_Group * group)
{
  return (this->group_map_.bind (group_id, group) == 0);
}

int TAO::PG_Group_Factory::find_group (PortableGroup::ObjectGroupId group_id, ::TAO::PG_Object_Group *& group) const
{
  return (this->group_map_.find (group_id , group) == 0);
}

int TAO::PG_Group_Factory::find_group (PortableGroup::ObjectGroup_ptr object_group, ::TAO::PG_Object_Group *& group) const
{
  int result = 0;
  PortableGroup::TagGroupTaggedComponent tc;
  if (TAO::PG_Utils::get_tagged_component (object_group, tc))
  {
    result = find_group (tc.object_group_id, group);
  }
  return result;
}

int TAO::PG_Group_Factory::destroy_group (PortableGroup::ObjectGroupId group_id)
{
  ::TAO::PG_Object_Group * group;
  int result = (this->group_map_.unbind (group_id, group) == 0);
  if (result)
  {
    delete group;
  }
  return result;
}

int TAO::PG_Group_Factory::destroy_group (PortableGroup::ObjectGroup_ptr object_group)
{
  PortableGroup::TagGroupTaggedComponent tc;
  TAO::PG_Utils::get_tagged_component (object_group, tc);
  return destroy_group (tc.object_group_id);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

  template class ACE_Hash_Map_Manager_Ex<
      PortableGroup::ObjectGroupId,
      ::TAO::PG_Object_Group *,
      ACE_Hash<ACE_UINT64>,
      ACE_Equal_To<ACE_UINT64>,
      TAO_SYNCH_MUTEX>;

  template class ACE_Hash_Map_Entry <
      PortableGroup::ObjectGroupId,
      ::TAO::PG_Object_Group *>;

  template class ACE_Hash_Map_Iterator_Ex <
      PortableGroup::ObjectGroupId,
      ::TAO::PG_Object_Group *,
      ACE_Hash<ACE_UINT64>,
      ACE_Equal_To<ACE_UINT64>,
      TAO_SYNCH_MUTEX>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

# pragma instantiate ACE_Hash_Map_Manager_Ex<
      PortableGroup::ObjectGroupId,
      ::TAO::PG_Object_Group *,
      ACE_Hash<ACE_UINT64>,
      ACE_Equal_To<ACE_UINT64>,
      TAO_SYNCH_MUTEX>;

# pragma instantiate ACE_Hash_Map_Entry <
      PortableGroup::ObjectGroupId,
      ::TAO::PG_Object_Group *>;

# pragma instantiate ACE_Hash_Map_Iterator_Ex <
      PortableGroup::ObjectGroupId,
      ::TAO::PG_Object_Group *,
      ACE_Hash<ACE_UINT64>,
      ACE_Equal_To<ACE_UINT64>,
      TAO_SYNCH_MUTEX>;

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    FT_ReplicationManager.cpp
 *
 *  $Id$
 *
 *  This file is part of Fault Tolerant CORBA.
 *  This file implements the FT_ReplicationManager class as declared in
 *  FT_Replication_Manager.h.
 *
 *  @author Curt Hibbs <hibbs_c@ociweb.com>
 */
//=============================================================================
#include "ace/pre.h"
#include "FT_ReplicationManager.h"
#include "FT_Property_Validator.h"

#include "ace/Get_Opt.h"
#include "orbsvcs/CosNamingC.h"
#include "tao/PortableServer/ORB_Manager.h"
#include "orbsvcs/PortableGroup/PG_Properties_Decoder.h"
#include "orbsvcs/PortableGroup/PG_Properties_Encoder.h"

#include "orbsvcs/PortableGroup/PG_Property_Utils.h"
#include "orbsvcs/PortableGroup/PG_conf.h"

#include "tao/Messaging/Messaging.h"
#include "tao/debug.h"

#include "orbsvcs/FaultTolerance/FT_IOGR_Property.h"


ACE_RCSID (FT_ReplicationManager,
           FT_ReplicationManager,
           "$Id$")


FT_ReplicationManager::FT_ReplicationManager ()
  : ior_output_file_(0)
  , nsName_(0)
  , internals_ ()
  , object_group_manager_ ()
  , property_manager_ (object_group_manager_)
  , generic_factory_ (object_group_manager_, property_manager_)
{
  // @note "this->init()" is not called here (in the constructor)
  //       since it may thrown an exception.  Throwing an exception in
  //       a constructor in an emulated exception environment is
  //       problematic since native exception semantics cannot be
  //       reproduced in such a case.  As such, init() must be called
  //       by whatever code instantiates this ReplicationManager.
}

FT_ReplicationManager::~FT_ReplicationManager (void)
{
}

//////////////////////////////////////////////////////
// FT_ReplicationManager public, non-CORBA methods

int FT_ReplicationManager::parse_args (int argc, char * argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:q");
  int c;

  while ((c = get_opts ()) != -1)
  {
    switch (c)
    {
      case 'o':
        ior_output_file_ = get_opts.opt_arg ();
        break;

      case '?':
        // fall thru
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " -o <iorfile>"
                           "\n",
                           argv [0]),
                          -1);
      break;
    }
  }
  // Indicates sucessful parsing of the command line
  return 0;
}

const char * FT_ReplicationManager::identity () const
{
  return identity_.c_str();
}

int FT_ReplicationManager::write_IOR()
{
  int result = -1;
  FILE* out = ACE_OS::fopen (ior_output_file_, "w");
  if (out)
  {
    ACE_OS::fprintf (out, "%s", static_cast<const char *>(ior_));
    ACE_OS::fclose (out);
    result = 0;
  }
  else
  {
    ACE_ERROR ((LM_ERROR,
      "Open failed for %s\n", ior_output_file_
    ));
  }
  return result;
}

int
FT_ReplicationManager::init (TAO_ORB_Manager & orbManager
                             ACE_ENV_ARG_DECL)
{
  InternalGuard guard (internals_);
  int result = 0;
  orb_ = orbManager.orb();

  // set our property validator
  FT_Property_Validator * property_validator;
  ACE_NEW_THROW_EX (
      property_validator,
      FT_Property_Validator,
      CORBA::NO_MEMORY ()
  );
  ACE_CHECK;

  property_manager_.init (property_validator);

  // Register with the ORB.
  ior_ = orbManager.activate (this
      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  //TODO - the poa should be externally settable
  this->object_group_manager_.poa(orbManager.root_poa());

  // Get an object reference for the ORBs IORManipulation object!
  CORBA::Object_var IORM =
    orb_->resolve_initial_references (TAO_OBJID_IORMANIPULATION,
                                      0
                                      ACE_ENV_ARG_PARAMETER);
  ACE_TRY_CHECK;

  iorm_ = TAO_IOP::TAO_IOR_Manipulation::_narrow (IORM.in ()
                                                  ACE_ENV_ARG_PARAMETER);
  ACE_TRY_CHECK;

  if (ior_output_file_ != 0)
  {
    identity_ = "file:";
    identity_ += ior_output_file_;
    result = write_IOR();
  }
  else
  {
    // if no IOR file specified,
    // then always try to register with name service
    nsName_ = "ReplicationManager";
  }

  if (nsName_ != 0)
  {
    identity_ = "name:";
    identity_ += nsName_;

    CORBA::Object_var naming_obj =
      orb_->resolve_initial_references ("NameService" ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    if (CORBA::is_nil(naming_obj.in ())){
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%T %n (%P|%t) Unable to find the Naming Service\n"),
                        1);
    }

    naming_context_ =
      CosNaming::NamingContext::_narrow (naming_obj.in () ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    this_name_.length (1);
    this_name_[0].id = CORBA::string_dup (nsName_);

    naming_context_->rebind (this_name_, _this()
                            ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
  }

  return result;
}


//////////////////////////////////////////////////////
// FT::ReplicationManager methods

/// Registers the Fault Notifier with the Replication Manager.
void
FT_ReplicationManager::register_fault_notifier (
    FT::FaultNotifier_ptr fault_notifier
    ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->fault_notifier_ = fault_notifier;
}


/// Returns the reference of the Fault Notifier.
FT::FaultNotifier_ptr
FT_ReplicationManager::get_fault_notifier (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
ACE_THROW_SPEC ((
    CORBA::SystemException
  , FT::InterfaceNotFound
))
{
  return this->fault_notifier_;
}

//////////////////////////////////////////////////////
// PortableGroup::PropertyManager methods

void
FT_ReplicationManager::set_default_properties (
    const PortableGroup::Properties & props
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::InvalidProperty,
                   PortableGroup::UnsupportedProperty))
{
  this->property_manager_.set_default_properties (props
                                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

PortableGroup::Properties *
FT_ReplicationManager::get_default_properties (
    ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return
    this->property_manager_.get_default_properties (
      ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
FT_ReplicationManager::remove_default_properties (
    const PortableGroup::Properties & props
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::InvalidProperty,
                   PortableGroup::UnsupportedProperty))
{
  this->property_manager_.remove_default_properties (props
                                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
FT_ReplicationManager::set_type_properties (
    const char *type_id,
    const PortableGroup::Properties & overrides
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::InvalidProperty,
                   PortableGroup::UnsupportedProperty))
{
  this->property_manager_.set_type_properties (type_id,
                                               overrides
                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

PortableGroup::Properties *
FT_ReplicationManager::get_type_properties (
    const char *type_id
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return
    this->property_manager_.get_type_properties (type_id
                                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
FT_ReplicationManager::remove_type_properties (
    const char *type_id,
    const PortableGroup::Properties & props
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::InvalidProperty,
                   PortableGroup::UnsupportedProperty))
{
  this->property_manager_.remove_type_properties (type_id,
                                                  props
                                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
FT_ReplicationManager::set_properties_dynamically (
    PortableGroup::ObjectGroup_ptr object_group,
    const PortableGroup::Properties & overrides
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound,
                   PortableGroup::InvalidProperty,
                   PortableGroup::UnsupportedProperty))
{
  this->property_manager_.set_properties_dynamically (object_group,
                                                      overrides
                                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

PortableGroup::Properties *
FT_ReplicationManager::get_properties (
    PortableGroup::ObjectGroup_ptr object_group
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound))
{
  return
    this->property_manager_.get_properties (object_group
                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}


//////////////////////////////////////////////////////
// FT::FTObjectGroupManager methods

/// Sets the primary member of a group.
PortableGroup::ObjectGroup_ptr
FT_ReplicationManager::set_primary_member (
    FT::ObjectGroup_ptr object_group,
    const FT::Location & the_location
    ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((
    CORBA::SystemException
  , PortableGroup::ObjectGroupNotFound
  , PortableGroup::MemberNotFound
  , FT::PrimaryNotSet
  , FT::BadReplicationStyle
))
{
  return (FT::ObjectGroup_ptr)0;
}

PortableGroup::ObjectGroup_ptr
FT_ReplicationManager::create_member (
    PortableGroup::ObjectGroup_ptr object_group,
    const PortableGroup::Location & the_location,
    const char * type_id,
    const PortableGroup::Criteria & the_criteria
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound,
                   PortableGroup::MemberAlreadyPresent,
                   PortableGroup::NoFactory,
                   PortableGroup::ObjectNotCreated,
                   PortableGroup::InvalidCriteria,
                   PortableGroup::CannotMeetCriteria))
{
  return
    this->object_group_manager_.create_member (object_group,
                                               the_location,
                                               type_id,
                                               the_criteria
                                               ACE_ENV_ARG_PARAMETER);
}

PortableGroup::ObjectGroup_ptr
FT_ReplicationManager::add_member (
    PortableGroup::ObjectGroup_ptr object_group,
    const PortableGroup::Location & the_location,
    CORBA::Object_ptr member
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound,
                   PortableGroup::MemberAlreadyPresent,
                   PortableGroup::ObjectNotAdded))
{
  return
    this->object_group_manager_.add_member (object_group,
                                            the_location,
                                            member
                                            ACE_ENV_ARG_PARAMETER);
}

PortableGroup::ObjectGroup_ptr
FT_ReplicationManager::remove_member (
    PortableGroup::ObjectGroup_ptr object_group,
    const PortableGroup::Location & the_location
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound,
                   PortableGroup::MemberNotFound))
{
  return
    this->object_group_manager_.remove_member (object_group,
                                               the_location
                                               ACE_ENV_ARG_PARAMETER);
}

PortableGroup::Locations *
FT_ReplicationManager::locations_of_members (
    PortableGroup::ObjectGroup_ptr object_group
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound))
{
  return
    this->object_group_manager_.locations_of_members (object_group
                                                      ACE_ENV_ARG_PARAMETER);
}

PortableGroup::ObjectGroups *
FT_ReplicationManager::groups_at_location (
    const PortableGroup::Location & the_location
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return
    this->object_group_manager_.groups_at_location (the_location
                                                    ACE_ENV_ARG_PARAMETER);
}

PortableGroup::ObjectGroupId
FT_ReplicationManager::get_object_group_id (
    PortableGroup::ObjectGroup_ptr object_group
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound))
{
  return
    this->object_group_manager_.get_object_group_id (object_group
                                                     ACE_ENV_ARG_PARAMETER);
}

PortableGroup::ObjectGroup_ptr
FT_ReplicationManager::get_object_group_ref (
    PortableGroup::ObjectGroup_ptr object_group
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound))
{
  return
    this->object_group_manager_.get_object_group_ref (object_group
                                                      ACE_ENV_ARG_PARAMETER);
}

CORBA::Object_ptr
FT_ReplicationManager::get_member_ref (
    PortableGroup::ObjectGroup_ptr object_group,
    const PortableGroup::Location & the_location
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound,
                   PortableGroup::MemberNotFound))
{
  return
    this->object_group_manager_.get_member_ref (object_group,
                                                the_location
                                                ACE_ENV_ARG_PARAMETER);
}


//////////////////////////////////////////////////////
// PortableGroup::GenericFactory methods

CORBA::Object_ptr
FT_ReplicationManager::create_object (
    const char * type_id,
    const PortableGroup::Criteria & the_criteria,
    PortableGroup::GenericFactory::FactoryCreationId_out
      factory_creation_id
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::NoFactory,
                   PortableGroup::ObjectNotCreated,
                   PortableGroup::InvalidCriteria,
                   PortableGroup::InvalidProperty,
                   PortableGroup::CannotMeetCriteria))
{
  PortableGroup::Criteria new_criteria (the_criteria);

  CORBA::Object_ptr obj =
    this->generic_factory_.create_object (type_id,
                                          new_criteria,
                                          factory_creation_id
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  ACE_CHECK_RETURN (CORBA::Object::_nil (obj));

  return obj;
}

void
FT_ReplicationManager::delete_object (
    const PortableGroup::GenericFactory::FactoryCreationId &
      factory_creation_id
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectNotFound))
{
  this->generic_factory_.delete_object (factory_creation_id
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

// Returns a dummy IOGR for unit tests
CORBA::Object_ptr
FT_ReplicationManager::create_test_iogr (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
{
  ACE_DEBUG ((LM_DEBUG, "---------------------------------------------\n"));
  ACE_DEBUG ((LM_DEBUG, "Creating an IOGR for the Unit Tests.\n"));

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Create a few fictitious IORs
      CORBA::Object_var name1 =
        orb_->string_to_object ("iiop://acme.cs.wustl.edu:6060/xyz"
                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      CORBA::Object_var name2 =
        orb_->string_to_object ("iiop://tango.cs.wustl.edu:7070/xyz"
                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // **********************************************************************
      // Create IOR list for use with merge_iors.
      TAO_IOP::TAO_IOR_Manipulation::IORList iors (2);
      iors.length (2);
      iors [0] = name1;
      iors [1] = name2;
      // **********************************************************************

      test_iogr_ = iorm_->merge_iors (iors ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Make a dummy property set
      FT::TagFTGroupTaggedComponent ft_tag_component;
      TAO_FT_IOGR_Property prop (ft_tag_component);

      // Check for set and get primaries
      CORBA::Boolean retval =
        iorm_->set_primary (&prop, name2.in (), test_iogr_.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (retval != 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("\tThe primary has been set\n")));
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("\tError in setting primary\n")));
        }

      // Check whether a primary has been set
      retval = iorm_->is_primary_set (&prop,
                                    test_iogr_.in ()
                                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (retval)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("\tis_primary_set () returned true\n")));
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("\tis_primary_set () returned false\n")));
        }

      // Get the primary
      CORBA::Object_var prim =
        iorm_->get_primary (&prop,
                          test_iogr_.in ()
                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Check whether we got back the right primary
      if (prim->_is_equivalent (name2.in ()))
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("\tWe got the right primary back\n")));
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("\tWe have a problem in getting the right primary\n")));
        }
      // **********************************************************************
      // Set properties
      // Property values

      // Major and Minor revision numbers
      ft_tag_component.version.major = (CORBA::Octet) 1;
      ft_tag_component.version.minor = (CORBA::Octet) 0;

      // Domain id
      const char *id = "IDL:FT_TEST/TestReplica:1.0";
      ft_tag_component.ft_domain_id = id;

      // Object group id
      test_iogr_group_id_ = (CORBA::ULongLong) 10;
      ft_tag_component.object_group_id = test_iogr_group_id_;

      // Version
      ft_tag_component.object_group_ref_version =
        (CORBA::ULong) 5;


      // create a property set
      TAO_PG::Properties_Encoder encoder;
      PortableGroup::Value value;

      value <<= 99;
      encoder.add(::FT::FT_MINIMUM_NUMBER_REPLICAS, value);

      // allocate and populate the criteria
      FT::Properties_var props_in;
      ACE_NEW_NORETURN (props_in, FT::Properties);
      if (props_in.ptr() == 0)
      {
        ACE_ERROR((LM_ERROR, "Error cannot allocate properties.\n" ));
      }
      else
      {
        encoder.encode(props_in);
      }

      //TODO - eventually this will really create an IOGR and this whole test_iogr code
      //       will no longer be needed.
      object_group_manager_.create_object_group(
        ft_tag_component.object_group_id,
        "my-dummy-type-id",
        props_in);

      // Set the property
      retval = iorm_->set_property (&prop,
                                    test_iogr_.in ()
                                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (retval)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("\tWe have set the property\n")));
    }
  ACE_CATCH (TAO_IOP::NotFound, userex)
    {
      ACE_PRINT_EXCEPTION (userex,
                          ACE_TEXT ("Unexpected NotFound Exception!\n"));
    }
  ACE_CATCH (TAO_IOP::Duplicate, userex)
    {
      ACE_PRINT_EXCEPTION (userex,
                          "Unexpected Duplicate Exception!\n");
    }
  ACE_CATCH (TAO_IOP::Invalid_IOR, userex)
    {
      ACE_PRINT_EXCEPTION (userex,
                          "Unexpected Invalid_IOR Exception!\n");
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      ACE_PRINT_EXCEPTION (sysex,
                          "Unexpected system Exception!!\n");
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                          "Unexpected ACE_CATCHANY Exception!\n");
    }
  ACE_ENDTRY;

  ACE_DEBUG ((LM_DEBUG, "---------------------------------------------\n"));

  return test_iogr_;
}

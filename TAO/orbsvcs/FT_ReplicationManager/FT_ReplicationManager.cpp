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


TAO::FT_ReplicationManager::FT_ReplicationManager ()
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

TAO::FT_ReplicationManager::~FT_ReplicationManager (void)
{
}

//////////////////////////////////////////////////////
// FT_ReplicationManager public, non-CORBA methods

int TAO::FT_ReplicationManager::parse_args (int argc, char * argv[])
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

const char * TAO::FT_ReplicationManager::identity () const
{
  return identity_.c_str();
}

int TAO::FT_ReplicationManager::write_IOR()
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
TAO::FT_ReplicationManager::init (TAO_ORB_Manager & orbManager
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

  this->property_manager_.init (property_validator);

  // Register with the ORB.
  ior_ = orbManager.activate (this
      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  //TODO - the poa should be externally settable
  this->object_group_manager_.init (orb_, orbManager.root_poa());

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

    //@@ Do NOT use _this() here.  Need to use the POA with which we
    // were activated to get the IOR.  Another reason not to use the
    // TAO_ORB_Manager; it does not give us much flexibility w.r.t.
    // POA usage.  -- Steve Totten
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
TAO::FT_ReplicationManager::register_fault_notifier (
    FT::FaultNotifier_ptr fault_notifier
    ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->fault_notifier_ = FT::FaultNotifier::_duplicate (fault_notifier);
}


/// Returns the reference of the Fault Notifier.
FT::FaultNotifier_ptr
TAO::FT_ReplicationManager::get_fault_notifier (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
ACE_THROW_SPEC ((
    CORBA::SystemException
  , FT::InterfaceNotFound
))
{
  return FT::FaultNotifier::_duplicate (fault_notifier_.in());
}

//////////////////////////////////////////////////////
// PortableGroup::PropertyManager methods

void
TAO::FT_ReplicationManager::set_default_properties (
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
TAO::FT_ReplicationManager::get_default_properties (
    ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return
    this->property_manager_.get_default_properties (
      ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO::FT_ReplicationManager::remove_default_properties (
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
TAO::FT_ReplicationManager::set_type_properties (
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
TAO::FT_ReplicationManager::get_type_properties (
    const char *type_id
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return
    this->property_manager_.get_type_properties (type_id
                                                 ACE_ENV_ARG_PARAMETER);
}

void
TAO::FT_ReplicationManager::remove_type_properties (
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
TAO::FT_ReplicationManager::set_properties_dynamically (
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
TAO::FT_ReplicationManager::get_properties (
    PortableGroup::ObjectGroup_ptr object_group
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound))
{
  return
    this->property_manager_.get_properties (object_group
                                            ACE_ENV_ARG_PARAMETER);
}


//////////////////////////////////////////////////////
// FT::FTObjectGroupManager methods

/// Sets the primary member of a group.
PortableGroup::ObjectGroup_ptr
TAO::FT_ReplicationManager::set_primary_member (
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
	// not yes implemented
	{
		int _TODO_must_implement_set_primary_;
	}
  return (FT::ObjectGroup::_nil ());

}

PortableGroup::ObjectGroup_ptr
TAO::FT_ReplicationManager::create_member (
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
TAO::FT_ReplicationManager::add_member (
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
TAO::FT_ReplicationManager::remove_member (
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
TAO::FT_ReplicationManager::locations_of_members (
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
TAO::FT_ReplicationManager::groups_at_location (
    const PortableGroup::Location & the_location
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return
    this->object_group_manager_.groups_at_location (the_location
                                                    ACE_ENV_ARG_PARAMETER);
}

PortableGroup::ObjectGroupId
TAO::FT_ReplicationManager::get_object_group_id (
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
TAO::FT_ReplicationManager::get_object_group_ref (
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
TAO::FT_ReplicationManager::get_member_ref (
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
TAO::FT_ReplicationManager::create_object (
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

  //@@ I've corrected the following code.  -- Steve Totten
  //@@ Also, can we really use the TAO_PG_GenericFactory
  // implementation to create an object group?
  CORBA::Object_var obj = CORBA::Object::_nil ();
  obj = this->generic_factory_.create_object (type_id,
                                          new_criteria,
                                          factory_creation_id
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());
  return obj._retn();
}

void
TAO::FT_ReplicationManager::delete_object (
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
TAO::FT_ReplicationManager::create_test_iogr (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
{
  ACE_DEBUG ((LM_DEBUG, "---------------------------------------------\n"));
  ACE_DEBUG ((LM_DEBUG, "Creating an IOGR for the Unit Tests.\n"));

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {

      // Domain id
      const char * domain_id = "TestFTDomains";

      // Object group id
      test_iogr_group_id_ = (CORBA::ULongLong) 10;

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

      // Create a few fictitious IORs
      //@@ Corrected to use corbaloc ObjectURL syntax.  -- Steve Totten
      CORBA::Object_var name1 =
        orb_->string_to_object ("corbaloc:iiop:acme.cs.wustl.edu:6060/xyz"
                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      CORBA::Object_var name2 =
        orb_->string_to_object ("corbaloc::iiop:tango.cs.wustl.edu:7070/xyz"
                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Create IOR list for use with merge_iors.
      TAO_IOP::TAO_IOR_Manipulation::IORList iors (3);
      iors.length (3);
      iors [0] = object_group_manager_.create_object_group(
                         test_iogr_group_id_,
                         domain_id,
                         "my-dummy-type-id",
                         props_in);
      iors [1] = name1;
      iors [2] = name2;

      test_iogr_ = iorm_->merge_iors (iors ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // we only need this so we can call IORManipulation's set_primary
      FT::TagFTGroupTaggedComponent ft_tag_component;
      TAO_FT_IOGR_Property ft_prop (ft_tag_component);

      // set primary
      CORBA::Boolean retval =
        iorm_->set_primary (&ft_prop, name2.in (), test_iogr_.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (retval != 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("\tThe primary has been set\n")));
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("\tError in setting primary\n")));
        }
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

  //@@ Add a final ACE_CHECK_RETURN in case there was an "exception".
  // -- Steve Totten
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  ACE_DEBUG ((LM_DEBUG, "---------------------------------------------\n"));

  //@@ Object_var (test_iogr_) must give up ownership to return IOGR
  // as an Object_ptr.  -- Steve Totten
  return test_iogr_._retn();
}


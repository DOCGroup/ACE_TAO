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
  : internals_()
  , orb_(CORBA::ORB::_nil())
  , poa_(PortableServer::POA::_nil())
  , iorm_(TAO_IOP::TAO_IOR_Manipulation::_nil())
  , ior_output_file_(0)
  , ns_name_(0)
  , replication_manager_ref_(FT::ReplicationManager::_nil())
  , naming_context_(CosNaming::NamingContext::_nil())
  , object_group_manager_()
  , property_manager_ (this->object_group_manager_)
  , generic_factory_ (this->object_group_manager_, this->property_manager_)
  , fault_notifier_(FT::FaultNotifier::_nil())
  , fault_notifier_ior_file_(0)
  , fault_consumer_()
  , test_iogr_(CORBA::Object::_nil())
{
  //@@Note: this->init() is not called here (in the constructor)
  // since it may throw an exception.  Throwing an exception in
  // a constructor in an emulated exception environment is
  // problematic since native exception semantics cannot be
  // reproduced in such a case.  As such, init() must be called
  // by whatever code instantiates this ReplicationManager.
}

TAO::FT_ReplicationManager::~FT_ReplicationManager (void)
{
}


//////////////////////////////////////////////////////
// FT_ReplicationManager public, non-CORBA methods

int TAO::FT_ReplicationManager::parse_args (int argc, char * argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "n:o:f:");
  int c;

  while ((c = get_opts ()) != -1)
  {
    switch (c)
    {
      case 'o':
        this->ior_output_file_ = get_opts.opt_arg ();
        break;

      case 'n':
        this->ns_name_ = get_opts.opt_arg ();
        break;

      case 'f':
        this->fault_notifier_ior_file_ = get_opts.opt_arg ();
        break;

      case '?':
        // fall thru
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " -o <iorfile>"
                           " -n <name-to-bind-in-NameService>"
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
  return this->identity_.c_str();
}

int TAO::FT_ReplicationManager::init (CORBA::ORB_ptr orb ACE_ENV_ARG_DECL)
{
#if (TAO_DEBUG_LEVEL_NEEDED == 1)
  if (TAO_debug_level > 1)
#endif /* (TAO_DEBUG_LEVEL_NEEDED == 1) */
  {
    ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT (
        "Enter TAO::FT_ReplicationManager::init.\n")
    ));
  }

  int result = 0;
  this->orb_ = CORBA::ORB::_duplicate (orb);

  // Create our Property Validator and set it on the Property Manager.
  TAO::FT_Property_Validator * property_validator = 0;
  ACE_NEW_RETURN (property_validator, TAO::FT_Property_Validator (), -1);
  if (property_validator != 0)
  {
    this->property_manager_.init (property_validator);
  }
  else
  {
    ACE_ERROR_RETURN ((LM_ERROR,
      ACE_TEXT (
        "TAO::FT_ReplicationManager::init: "
        "Could not create Property Validator.\n")),
      -1);
  }

  // Get the RootPOA.
  CORBA::Object_var poa_obj = this->orb_->resolve_initial_references (
    TAO_OBJID_ROOTPOA ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  this->poa_ = PortableServer::POA::_narrow (
    poa_obj.in() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Initialize the Object Group Manager.
  result = this->object_group_manager_.init (
    this->orb_.in(), this->poa_.in());
  if (result != 0)
  {
    ACE_ERROR_RETURN ((LM_ERROR,
      ACE_TEXT (
        "TAO::FT_ReplicationManager::init: "
        "Could not initialize the Object Group Manager.\n")),
      -1);
  }

  // Get an object reference for the ORB's IORManipulation object.
  CORBA::Object_var iorm_obj = this->orb_->resolve_initial_references (
    TAO_OBJID_IORMANIPULATION ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  this->iorm_ = TAO_IOP::TAO_IOR_Manipulation::_narrow (
    iorm_obj.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Activate ourself in the POA.
  PortableServer::ObjectId_var oid = this->poa_->activate_object (
    this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  CORBA::Object_var this_obj = this->poa_->id_to_reference (
    oid.in() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  this->replication_manager_ref_ = FT::ReplicationManager::_narrow (
    this_obj.in() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // If we were given an initial IOR string for a Fault Notifier on the
  // command line, convert it to an IOR.
  if (this->fault_notifier_ior_file_ != 0)
  {
    CORBA::String_var notifierIOR;
    if (this->readIORFile (this->fault_notifier_ior_file_, notifierIOR))
    {
      CORBA::Object_var notifier_obj = this->orb_->string_to_object (
        notifierIOR.in() ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
      FT::FaultNotifier_var notifier = FT::FaultNotifier::_narrow (
        notifier_obj.in() ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
      this->register_fault_notifier_i (notifier.in() ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
    }
    else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
        ACE_TEXT (
          "TAO::FT_ReplicationManager::init: "
          "Could not read %s.\n"),
        this->fault_notifier_ior_file_),
        -1);
    }
  }

  // Activate the RootPOA.
  PortableServer::POAManager_var poa_mgr =
    this->poa_->the_POAManager (ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  poa_mgr->activate (ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Publish our IOR, either to a file or the Naming Service.
  if (this->ior_output_file_ != 0)
  {
    this->identity_ = "file:";
    this->identity_ += this->ior_output_file_;
    result = this->write_ior();
  }
  else
  {
    this->ns_name_ = "ReplicationManager";
  }

  if (this->ns_name_ != 0)
  {
    this->identity_ = "name:";
    this->identity_ += this->ns_name_;

    CORBA::Object_var naming_obj = this->orb_->resolve_initial_references (
        TAO_OBJID_NAMESERVICE ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);

    if (CORBA::is_nil(naming_obj.in ()))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
        ACE_TEXT ("%T %n (%P|%t) Unable to find the Naming Service.\n")),
        -1);
    }

    this->naming_context_ =
      CosNaming::NamingContext::_narrow (
        naming_obj.in () ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);

    this->this_name_.length (1);
    this->this_name_[0].id = CORBA::string_dup (this->ns_name_);

    this->naming_context_->rebind (
      this->this_name_,
      replication_manager_ref_.in()
      ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);
  }

#if (TAO_DEBUG_LEVEL_NEEDED == 1)
  if (TAO_debug_level > 1)
#endif /* (TAO_DEBUG_LEVEL_NEEDED == 1) */
  {
    ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT (
        "Leave TAO::FT_ReplicationManager::init.\n")
    ));
  }

  return result;
}

int TAO::FT_ReplicationManager::idle (int & result)
{
  ACE_UNUSED_ARG (result);
  int quit = 0;  // never quit
  return quit;
}


int TAO::FT_ReplicationManager::fini (ACE_ENV_SINGLE_ARG_DECL)
{
  int result = 0;

  //@@ Should we ever return -1 from this function, or should we
  // catch and swallow all exceptions?

  if (this->ior_output_file_ != 0)
  {
    ACE_OS::unlink (this->ior_output_file_);
    this->ior_output_file_ = 0;
  }
  if (this->ns_name_ != 0)
  {
    this->naming_context_->unbind (this->this_name_ ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);
    this->ns_name_ = 0;
  }

  result = this->fault_consumer_.fini (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return result;
}

////////////////////////////////////////////
// FT_ReplicationManager private methods

int TAO::FT_ReplicationManager::write_ior()
{
  int result = -1;
  FILE* out = ACE_OS::fopen (this->ior_output_file_, "w");
  if (out)
  {
    CORBA::String_var ior_str = this->orb_->object_to_string (
      this->replication_manager_ref_.in());
    ACE_OS::fprintf (out, "%s", ior_str.in());
    ACE_OS::fclose (out);
    result = 0;
  }
  else
  {
    ACE_ERROR ((LM_ERROR,
      ACE_TEXT("Open failed for %s\n"), this->ior_output_file_
    ));
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
  this->register_fault_notifier_i (fault_notifier ACE_ENV_ARG_DECL);
}

void
TAO::FT_ReplicationManager::register_fault_notifier_i (
  FT::FaultNotifier_ptr fault_notifier
  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

#if (TAO_DEBUG_LEVEL_NEEDED == 1)
  if (TAO_debug_level > 1)
#endif /* (TAO_DEBUG_LEVEL_NEEDED == 1) */
  {
    ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT (
        "Enter TAO::FT_ReplicationManager::register_fault_notifier_i.\n")
    ));
  }

  if (CORBA::is_nil (fault_notifier))
  {
    ACE_ERROR ((LM_ERROR,
      ACE_TEXT (
        "TAO::FT_ReplicationManager::register_fault_notifier_i: "
        "Bad Fault Notifier object reference provided.\n")
    ));
    ACE_THROW (CORBA::BAD_PARAM (
      CORBA::SystemException::_tao_minor_code (
        TAO_DEFAULT_MINOR_CODE,
        EINVAL),
      CORBA::COMPLETED_NO));
  }

#if (TAO_DEBUG_LEVEL_NEEDED == 1)
  if (TAO_debug_level > 1)
#endif /* (TAO_DEBUG_LEVEL_NEEDED == 1) */
  {
    ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT (
        "TAO::FT_ReplicationManager::register_fault_notifier_i: "
        "Duplicate FaultNotifier object reference.\n")
    ));
  }

  // Cache new Fault Notifier object reference.
  this->fault_notifier_ = FT::FaultNotifier::_duplicate (fault_notifier);

  // Re-initialize our consumer.
  // Swallow any exception.
  int result = 0;
  ACE_TRY_NEW_ENV
  {

#if (TAO_DEBUG_LEVEL_NEEDED == 1)
    if (TAO_debug_level > 1)
#endif /* (TAO_DEBUG_LEVEL_NEEDED == 1) */
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT (
          "TAO::FT_ReplicationManager::register_fault_notifier_i: "
          "Calling fault_consumer_.fini().\n")
      ));
    }

    result = this->fault_consumer_.fini (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;

#if (TAO_DEBUG_LEVEL_NEEDED == 1)
    if (TAO_debug_level > 1)
#endif /* (TAO_DEBUG_LEVEL_NEEDED == 1) */
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT (
          "TAO::FT_ReplicationManager::register_fault_notifier_i: "
          "Calling fault_consumer_.init().\n")
      ));
    }

    result = this->fault_consumer_.init (
      this->poa_.in(),
      this->fault_notifier_.in(),
      this->replication_manager_ref_.in()
      ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
  }
  ACE_CATCHANY
  {
    ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
      ACE_TEXT (
        "TAO::FT_ReplicationManager::register_fault_notifier_i: "
        "Error reinitializing FT_FaultConsumer.\n")
    );
  }
  ACE_ENDTRY;

  if (result != 0)
  {
    ACE_ERROR((LM_ERROR,
      ACE_TEXT (
        "TAO::FT_ReplicationManager::register_fault_notifier_i: "
        "Could not re-initialize FT_FaultConsumer.\n")
    ));

    ACE_THROW (CORBA::INTERNAL (
      CORBA::SystemException::_tao_minor_code (
        TAO_DEFAULT_MINOR_CODE,
        EINVAL),
      CORBA::COMPLETED_NO));
  }

#if (TAO_DEBUG_LEVEL_NEEDED == 1)
  if (TAO_debug_level > 1)
#endif /* (TAO_DEBUG_LEVEL_NEEDED == 1) */
  {
    ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT (
        "Leave TAO::FT_ReplicationManager::register_fault_notifier_i.\n")
    ));
  }

}


/// Returns the reference of the Fault Notifier.
FT::FaultNotifier_ptr
TAO::FT_ReplicationManager::get_fault_notifier (
  ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException, FT::InterfaceNotFound))
{
  return FT::FaultNotifier::_duplicate (this->fault_notifier_.in());
}


/// TAO-specific find factory registry
::PortableGroup::FactoryRegistry_ptr
TAO::FT_ReplicationManager::get_factory_registry (
  const PortableGroup::Criteria & selection_criteria
  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ::PortableGroup::FactoryRegistry_var registry = ::PortableGroup::FactoryRegistry::_nil();
  int todo_implement_factory_registry;
  return registry;
}

/// TAO-specific shutdown operation.
void TAO::FT_ReplicationManager::shutdown (
  ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->shutdown_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void TAO::FT_ReplicationManager::shutdown_i (
  ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  int result = 0;
  result = this->fini (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->orb_->shutdown (0 ACE_ENV_SINGLE_ARG_PARAMETER);
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
  // Not yet implemented.
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

PortableGroup::ObjectGroup_ptr
TAO::FT_ReplicationManager::get_object_group_ref_from_id (
    PortableGroup::ObjectGroupId group_id
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , PortableGroup::ObjectGroupNotFound
  ))
{
  return
    this->object_group_manager_.get_object_group_ref_from_id (group_id
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
  PortableGroup::GenericFactory::FactoryCreationId_out factory_creation_id
  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::NoFactory,
                   PortableGroup::ObjectNotCreated,
                   PortableGroup::InvalidCriteria,
                   PortableGroup::InvalidProperty,
                   PortableGroup::CannotMeetCriteria))
{
  //@@ Can we really use the TAO_PG_GenericFactory
  // implementation to create an object group?
  CORBA::Object_var obj = this->generic_factory_.create_object (
    type_id,
    the_criteria,
    factory_creation_id
    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());
  return obj._retn();
}

void
TAO::FT_ReplicationManager::delete_object (
  const PortableGroup::GenericFactory::FactoryCreationId & factory_creation_id
  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectNotFound))
{
  this->generic_factory_.delete_object (factory_creation_id
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

#if 0
// Returns a dummy IOGR for unit tests
CORBA::Object_ptr
TAO::FT_ReplicationManager::create_test_iogr (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_DEBUG ((LM_DEBUG, "---------------------------------------------\n"));
  ACE_DEBUG ((LM_DEBUG, "Creating an IOGR for the Unit Tests.\n"));

  ACE_TRY_NEW_ENV
  {
    // Domain id
    const char * domain_id = "TestFTDomains";

    // Object group id
    this->test_iogr_group_id_ = (CORBA::ULongLong) 10;

    // create a property set
    TAO_PG::Properties_Encoder encoder;
    PortableGroup::Value value;

    value <<= 99;
    encoder.add(::FT::FT_MINIMUM_NUMBER_REPLICAS, value);

    // allocate and populate the criteria
    FT::Properties * props_in;
    FT::Properties_var props;
    ACE_NEW_NORETURN (props_in, FT::Properties);
    if (props_in == 0)
    {
      ACE_ERROR((LM_ERROR, "Error cannot allocate properties.\n"));
    }
    else
    {
      props = props_in;
      encoder.encode(props);
    }

    // Create a few fictitious IORs
    CORBA::Object_var name1 =
      this->orb_->string_to_object (
        "corbaloc:iiop:acme.cs.wustl.edu:6060/xyz" ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    CORBA::Object_var name2 =
      this->orb_->string_to_object (
        "corbaloc::iiop:tango.cs.wustl.edu:7070/xyz" ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    // Create IOR list for use with merge_iors.
    TAO_IOP::TAO_IOR_Manipulation::IORList iors (3);
    iors.length (3);
    iors [0] = this->object_group_manager_.create_object_group (
      this->test_iogr_group_id_,
      "my-dummy-type-id",
      domain_id,
      props.in()
      ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    iors [1] = name1;
    iors [2] = name2;

    this->test_iogr_ = this->iorm_->merge_iors (iors ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    // We only need this so we can call IORManipulation's set_primary.
    FT::TagFTGroupTaggedComponent ft_tag_component;
    TAO_FT_IOGR_Property ft_prop (ft_tag_component);

    // set primary
    CORBA::Boolean retval =
      this->iorm_->set_primary (
        &ft_prop, name2.in (),
        this->test_iogr_.in () ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    if (retval != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("\tThe primary has been set\n")));
    }
    else
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("\tError in setting primary\n")));
    }
  }
  ACE_CATCHANY
  {
    ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception!\n");
  }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  ACE_DEBUG ((LM_DEBUG, "---------------------------------------------\n"));

  return this->test_iogr_._retn();
}
#endif

int TAO::FT_ReplicationManager::readIORFile (
  const char * filename,
  CORBA::String_var & ior)
{
  int result = 0;
  FILE *in = ACE_OS::fopen (filename, "r");
  ACE_OS::fseek(in, 0, SEEK_END);
  size_t fileSize = ACE_OS::ftell(in);
  ACE_OS::fseek(in, 0, SEEK_SET);
  char * buffer;
  ACE_NEW_NORETURN (buffer,
    char[fileSize+1]);
  if (buffer != 0)
  {
    if( fileSize == ACE_OS::fread(buffer, 1, fileSize, in))
    {
      buffer[fileSize] = '\0';
      ior = CORBA::string_dup(buffer);
      ACE_TRY_CHECK;
      result = 1; // success
    }
    delete[] buffer;
  }
  return result;
}


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
#include "tao/Messaging/Messaging.h"
#include "tao/IORTable/IORTable.h"
#include "tao/debug.h"
#include "orbsvcs/PortableGroup/PG_Properties_Decoder.h"
#include "orbsvcs/PortableGroup/PG_Properties_Encoder.h"
#include "orbsvcs/PortableGroup/PG_Property_Utils.h"
#include "orbsvcs/PortableGroup/PG_conf.h"
#include "orbsvcs/FaultTolerance/FT_IOGR_Property.h"
#include "orbsvcs/FT_ReplicationManager/FT_ReplicationManagerFaultAnalyzer.h"

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
  , fault_notifier_ior_(0)
  , fault_consumer_()
  , factory_registry_("ReplicationManager::FactoryRegistry")
  , quit_(0)
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
        this->fault_notifier_ior_ = get_opts.opt_arg ();
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


  // initialize the FactoryRegistry
  this->factory_registry_.init(orb_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(-1);

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
  // command line, convert it to an IOR, then register the fault
  // notifier.
  if (this->fault_notifier_ior_ != 0)
  {
    CORBA::Object_var notifier_obj = this->orb_->string_to_object (
      this->fault_notifier_ior_ ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);
    FT::FaultNotifier_var notifier = FT::FaultNotifier::_narrow (
      notifier_obj.in() ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);
    this->register_fault_notifier_i (notifier.in() ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);
  }

  // Activate the RootPOA.
  PortableServer::POAManager_var poa_mgr =
    this->poa_->the_POAManager (ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  poa_mgr->activate (ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Register our IOR in the IORTable with the key-string
  // "ReplicationManager".
  CORBA::Object_var ior_table_obj =
    this->orb_->resolve_initial_references (
      TAO_OBJID_IORTABLE ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  IORTable::Table_var ior_table =
    IORTable::Table::_narrow (ior_table_obj.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  if (CORBA::is_nil (ior_table.in ()))
  {
    ACE_ERROR_RETURN ((LM_ERROR,
      ACE_TEXT ("%T %n (%P|%t) Unable to resolve the IORTable.\n")),
      -1);
  }
  else
  {
    CORBA::String_var rm_ior_str = this->orb_->object_to_string (
      this->replication_manager_ref_.in () ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);
    ior_table->bind ("ReplicationManager", rm_ior_str.in ()
      ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);
  }

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
  return this->quit_;
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

  this->quit_ = 1;

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

  // Cache new Fault Notifier object reference.
  this->fault_notifier_ = FT::FaultNotifier::_duplicate (fault_notifier);

  // Re-initialize our consumer.
  // Swallow any exception.
  int result = 0;
  ACE_TRY_NEW_ENV
  {
    result = this->fault_consumer_.fini (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;

    // Create a fault analyzer.
    TAO::FT_FaultAnalyzer * analyzer = 0;
    ACE_NEW_NORETURN (
      analyzer,
      TAO::FT_ReplicationManagerFaultAnalyzer (this));
    if (analyzer == 0)
    {
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT (
          "TAO::FT_ReplicationManager::register_fault_notifier_i: "
          "Error creating FaultAnalyzer.\n"
          )
      ));
      result = -1;
    }
    if (result == 0)
    {
      result = this->fault_consumer_.init (
        this->poa_.in(),
        this->fault_notifier_.in(),
        analyzer
        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
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
    ACE_ERROR ((LM_ERROR,
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
  return this->factory_registry_.reference();
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

  // this->orb_->shutdown (0 ACE_ENV_SINGLE_ARG_PARAMETER);
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


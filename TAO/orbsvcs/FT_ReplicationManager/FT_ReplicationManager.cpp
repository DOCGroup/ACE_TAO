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
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include "tao/Messaging/Messaging.h"
#include "tao/IORTable/IORTable.h"
#include "tao/debug.h"
#include "tao/ORB_Constants.h"
#include "orbsvcs/PortableGroup/PG_Object_Group.h"
#include "orbsvcs/PortableGroup/PG_Property_Set.h"
#include "orbsvcs/PortableGroup/PG_Properties_Encoder.h"
#include "orbsvcs/PortableGroup/PG_Property_Utils.h"
#include "orbsvcs/PortableGroup/PG_conf.h"

#include "orbsvcs/FaultTolerance/FT_IOGR_Property.h"
#include "orbsvcs/FT_ReplicationManager/FT_ReplicationManagerFaultAnalyzer.h"

// Use this macro at the beginning of CORBA methods
// to aid in debugging.
#define METHOD_ENTRY(name)    \
  if (TAO_debug_level > 6)    \
  {                           \
    ACE_DEBUG (( LM_DEBUG,    \
    "Enter %s\n", #name       \
      ));                     \
  }

// Use this macro to return from CORBA methods
// to aid in debugging.  Note that you can specify
// the return value after the macro, for example:
// METHOD_RETURN(Plugh::plover) xyzzy; is equivalent
// to return xyzzy;
// METHOD_RETURN(Plugh::troll); is equivalent to
// return;
// WARNING: THIS GENERATES TWO STATEMENTS!!! THE FOLLOWING
// will not do what you want it to:
//  if (cave_is_closing) METHOD_RETURN(Plugh::pirate) aarrggh;
// Moral:  Always use braces.
#define METHOD_RETURN(name)   \
  if (TAO_debug_level > 6)    \
  {                           \
    ACE_DEBUG (( LM_DEBUG,    \
      "Leave %s\n", #name     \
      ));                     \
  }                           \
  return /* value goes here */

#define TODO
//#define TODO int todo; // warn on todos

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::FT_ReplicationManager::FT_ReplicationManager ()
  : orb_ (CORBA::ORB::_nil ())
  , poa_ (PortableServer::POA::_nil ())
  , ior_output_file_ (0)
  , ns_name_ ("")
  , naming_context_ (CosNaming::NamingContext::_nil ())
  , replication_manager_ref_ (FT::ReplicationManager::_nil ())
  , fault_notifier_ (FT::FaultNotifier::_nil ())
  , fault_notifier_ior_string_ (0)
  , fault_consumer_ ()
  , factory_registry_ ("ReplicationManager::FactoryRegistry")
  , quit_ (0)
{
  // init must be called before using this object.
}

TAO::FT_ReplicationManager::~FT_ReplicationManager (void)
{
  // cleanup happens in fini
}

//public
int TAO::FT_ReplicationManager::parse_args (int argc, ACE_TCHAR * argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("n:o:f:"));
  int c;

  while ( (c = get_opts ()) != -1)
  {
    switch (c)
    {
      case 'o':
        this->ior_output_file_ = get_opts.opt_arg ();
        break;

      case 'n':
        this->ns_name_ = ACE_TEXT_ALWAYS_CHAR(get_opts.opt_arg ());
        break;

      case 'f':
        this->fault_notifier_ior_string_ = get_opts.opt_arg ();
        break;

      case '?':
        // fall thru
      default:
        ACE_ERROR_RETURN ( (LM_ERROR,
                           ACE_TEXT ("%T %n (%P|%t) - usage:  %s")
                           ACE_TEXT (" -o <iorfile (for testing)>")
                           ACE_TEXT (" -f <fault notifier IOR (for testing)>")
                           ACE_TEXT (" -n <name-to-bind-in-NameService (for testing)>")
                           ACE_TEXT ("\n"),
                           argv [0]),
                          -1);
      break;
    }
  }
  // Indicates successful parsing of the command line
  return 0;
}

//public
const char * TAO::FT_ReplicationManager::identity () const
{
  return this->identity_.c_str ();
}

//public
int TAO::FT_ReplicationManager::init (CORBA::ORB_ptr orb)
{
  int result = 0;

  if (TAO_debug_level > 1)
  {
    ACE_DEBUG ( (LM_DEBUG,
      ACE_TEXT (
        "%T %n (%P|%t) - Enter TAO::FT_ReplicationManager::init.\n")
    ));
  }


  this->orb_ = CORBA::ORB::_duplicate (orb);

  // Get the RootPOA.
  CORBA::Object_var poa_obj = this->orb_->resolve_initial_references (
    TAO_OBJID_ROOTPOA);
  this->poa_ = PortableServer::POA::_narrow (
    poa_obj.in ());


  // initialize the FactoryRegistry
  this->factory_registry_.init (this->orb_.in (), this->poa_.in ());

  PortableGroup::FactoryRegistry_var factory_registry = this->factory_registry_.reference ();

  // @@: do we want to use the same poa to create object groups?
  this->group_factory_.init (
    this->orb_.in (),
    this->poa_.in (),
    factory_registry.in ());

  // Activate ourself in the POA.
  PortableServer::ObjectId_var oid = this->poa_->activate_object (
    this);

  CORBA::Object_var this_obj = this->poa_->id_to_reference (
    oid.in ());
  this->replication_manager_ref_ = FT::ReplicationManager::_narrow (
    this_obj.in ());

  // If we were given an initial IOR string for a Fault Notifier on the
  // command line, convert it to an IOR, then register the fault
  // notifier.
  if (this->fault_notifier_ior_string_ != 0)
  {
    CORBA::Object_var notifier_obj = this->orb_->string_to_object (
      this->fault_notifier_ior_string_);
    FT::FaultNotifier_var notifier = FT::FaultNotifier::_narrow (
      notifier_obj.in ());
    if (! CORBA::is_nil (notifier.in ()))
    {
      this->register_fault_notifier_i (notifier.in ());
    }
    else
    {
      ACE_ERROR_RETURN ( (LM_ERROR,
        ACE_TEXT (
          "%T %n (%P|%t) - Could not resolve notifier IOR.\n")),
          -1);
    }
  }

  // Activate the RootPOA.
  PortableServer::POAManager_var poa_mgr =
    this->poa_->the_POAManager ();
  poa_mgr->activate ();

  // Register our IOR in the IORTable with the key-string
  // "ReplicationManager".
  CORBA::Object_var ior_table_obj =
    this->orb_->resolve_initial_references (
      TAO_OBJID_IORTABLE);

  IORTable::Table_var ior_table =
    IORTable::Table::_narrow (ior_table_obj.in ());
  if (CORBA::is_nil (ior_table.in ()))
  {
    ACE_ERROR_RETURN ( (LM_ERROR,
      ACE_TEXT ("%T %n (%P|%t) - Unable to resolve the IORTable.\n")),
      -1);
  }
  else
  {
    CORBA::String_var rm_ior_str = this->orb_->object_to_string (
      this->replication_manager_ref_.in ());
    ior_table->bind ("ReplicationManager", rm_ior_str.in ());
  }

  // Publish our IOR, either to a file or the Naming Service.
  if (this->ior_output_file_ != 0)
  {
    this->identity_ = "file:";
    this->identity_ += ACE_TEXT_ALWAYS_CHAR(this->ior_output_file_);
    result = this->write_ior ();
  }

  if (result == 0 && this->ns_name_.length () != 0)
  {
    this->identity_ = "name:";
    this->identity_ += this->ns_name_;

    CORBA::Object_var naming_obj = this->orb_->resolve_initial_references (
        TAO_OBJID_NAMESERVICE);

    this->naming_context_ =
      CosNaming::NamingContext::_narrow (
        naming_obj.in ());

    if (CORBA::is_nil (this->naming_context_.in ()))
    {
      ACE_ERROR_RETURN ( (LM_ERROR,
        ACE_TEXT ("%T %n (%P|%t) - Unable to find the Naming Service.\n")),
        -1);
    }

    this->this_name_.length (1);
    this->this_name_[0].id = CORBA::string_dup (this->ns_name_.c_str ());

    this->naming_context_->rebind (
      this->this_name_,
      this->replication_manager_ref_.in ());
  }

  if (TAO_debug_level > 1)
  {
    if (result == 0)
    {
      ACE_DEBUG ( (LM_DEBUG,
        ACE_TEXT (
          "%T %n (%P|%t) - Leave TAO::FT_ReplicationManager::init.\n")
      ));
    }
    else
    {
      ACE_DEBUG ( (LM_DEBUG,
        ACE_TEXT (
          "%T %n (%P|%t) - FT_ReplicationManager::init failed.\n")
      ));
    }
  }

  ////////////////////////////////
  // Initialize default properties
  PortableGroup::Value value;
  value <<= TAO_PG_MEMBERSHIP_STYLE;
  this->properties_support_.set_default_property (PortableGroup::PG_MEMBERSHIP_STYLE, value);

  value <<= TAO_PG_INITIAL_NUMBER_MEMBERS;
  this->properties_support_.set_default_property (PortableGroup::PG_INITIAL_NUMBER_MEMBERS, value);

  value <<= TAO_PG_MINIMUM_NUMBER_MEMBERS;
  this->properties_support_.set_default_property (PortableGroup::PG_MINIMUM_NUMBER_MEMBERS, value);

  value <<= FT::SEMI_ACTIVE;
  this->properties_support_.set_default_property (FT::FT_REPLICATION_STYLE, value);

  value <<= FT::CONS_APP_CTRL;
  this->properties_support_.set_default_property (  FT::FT_CONSISTENCY_STYLE, value);

  value <<= FT::PULL;
  this->properties_support_.set_default_property (FT::FT_FAULT_MONITORING_STYLE, value);

  value <<= FT::MEMB;
  this->properties_support_.set_default_property (FT::FT_FAULT_MONITORING_GRANULARITY, value);

#if 0
  FaultMonitoringIntervalAndTimeoutValue times;
  value <<= times;
  this->properties_support_.set_default_property (FT::FT_FAULT_MONITORING_INTERVAL_AND_TIMEOUT, value);
#endif

#if 0
  value << interval;
  this->properties_support_.set_default_property (FT::FT_CHECKPOINT_INTERVAL, value);
#endif


  return result;
}

//public
int TAO::FT_ReplicationManager::idle (int & result)
{
  ACE_UNUSED_ARG (result);
  return this->quit_;
}


//public
int TAO::FT_ReplicationManager::fini (void)
{
  int result = 0;

  result = this->fault_consumer_.fini ();

  if (this->ior_output_file_ != 0)
  {
    ACE_OS::unlink (this->ior_output_file_);
    this->ior_output_file_ = 0;
  }
  if (this->ns_name_.length () != 0)
  {
    this->naming_context_->unbind (this->this_name_);
    this->ns_name_.clear ();
  }

  return result;
}

//CORBA
void
TAO::FT_ReplicationManager::register_fault_notifier (
  FT::FaultNotifier_ptr fault_notifier)
{
  this->register_fault_notifier_i (fault_notifier);
}

//private
void
TAO::FT_ReplicationManager::register_fault_notifier_i (
  FT::FaultNotifier_ptr fault_notifier)
{
  if (CORBA::is_nil (fault_notifier))
  {
    ACE_ERROR ( (LM_ERROR,
      ACE_TEXT (
        "%T %n (%P|%t) - Bad Fault Notifier object reference provided.\n")
    ));
    throw CORBA::BAD_PARAM (
      CORBA::SystemException::_tao_minor_code (
        TAO::VMCID,
        EINVAL),
      CORBA::COMPLETED_NO);
  }

  // Cache new Fault Notifier object reference.
  this->fault_notifier_ = FT::FaultNotifier::_duplicate (fault_notifier);

  // Re-initialize our consumer.
  // Swallow any exception.
  int result = 0;
  try
  {
    //@@ should we check to see if a notifier is already registered, rather than
    // simply "unregistering"?
    result = this->fault_consumer_.fini ();

    // Note if the fini failed, we ignore it.  It may not have been registered
    // in the first place.

    // Create a fault analyzer.
    TAO::FT_FaultAnalyzer * analyzer = 0;
    ACE_NEW_NORETURN (
      analyzer,
      TAO::FT_ReplicationManagerFaultAnalyzer (this));
    if (analyzer == 0)
    {
      ACE_ERROR ( (LM_ERROR,
        ACE_TEXT (
          "%T %n (%P|%t) - Error creating FaultAnalyzer.\n"
          )
      ));
      result = -1;
    }
    if (result == 0)
    {
      result = this->fault_consumer_.init (
        this->poa_.in (),
        this->fault_notifier_.in (),
        analyzer);
    }
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception (
      ACE_TEXT ("TAO::FT_ReplicationManager::register_fault_notifier_i: ")
      ACE_TEXT ("Error reinitializing FT_FaultConsumer.\n"));
    result = -1;
  }

  if (result != 0)
  {
    ACE_ERROR ( (LM_ERROR,
      ACE_TEXT (
        "%T %n (%P|%t) - Could not re-initialize FT_FaultConsumer.\n")
    ));

    throw CORBA::INTERNAL (
      CORBA::SystemException::_tao_minor_code (
        TAO::VMCID,
        EINVAL),
      CORBA::COMPLETED_NO);
  }
}


// Returns the reference of the Fault Notifier.
//CORBA
FT::FaultNotifier_ptr
TAO::FT_ReplicationManager::get_fault_notifier ()
{
  if (CORBA::is_nil (this->fault_notifier_.in ()))
  {
    throw FT::InterfaceNotFound ();
  }
  return FT::FaultNotifier::_duplicate (this->fault_notifier_.in ());
}


// TAO-specific find factory registry
//CORBA
::PortableGroup::FactoryRegistry_ptr
TAO::FT_ReplicationManager::get_factory_registry (
  const PortableGroup::Criteria & selection_criteria)
{
  ACE_UNUSED_ARG (selection_criteria);
  return this->factory_registry_.reference ();
}

// TAO-specific shutdown operation.
//public
void TAO::FT_ReplicationManager::shutdown ()
{
  this->quit_ = 1;
}

// Get the type_id associated with an object group.
//CORBA
char * TAO::FT_ReplicationManager::type_id (
  PortableGroup::ObjectGroup_ptr object_group)
{
  char * result = 0;
  TAO::PG_Object_Group * group = 0;
  if (this->group_factory_.find_group (object_group, group))
  {
    result = group->get_type_id ();
  }
  else
  {
    throw PortableGroup::ObjectGroupNotFound ();
  }
  return result;
}

//////////////////////////////////////////////////////
// PortableGroup::PropertyManager methods

//CORBA
void
TAO::FT_ReplicationManager::set_default_properties (
  const PortableGroup::Properties & props)
{

  this->properties_support_.set_default_properties (props);
  //@@ validate properties?
}

//CORBA
PortableGroup::Properties *
TAO::FT_ReplicationManager::get_default_properties ()
{
  return this->properties_support_.get_default_properties ();
}

//CORBA
void
TAO::FT_ReplicationManager::remove_default_properties (
    const PortableGroup::Properties & props)
{
  this->properties_support_.remove_default_properties (props);
}

//CORBA
void
TAO::FT_ReplicationManager::set_type_properties (
    const char *type_id,
    const PortableGroup::Properties & overrides)
{
  this->properties_support_.set_type_properties (
    type_id,
    overrides);
}

//CORBA
PortableGroup::Properties *
TAO::FT_ReplicationManager::get_type_properties (
    const char *type_id)
{
  return this->properties_support_.get_type_properties (type_id);
}

//CORBA
void
TAO::FT_ReplicationManager::remove_type_properties (
    const char *type_id,
    const PortableGroup::Properties & props)
{
  this->properties_support_.remove_type_properties (
    type_id,
    props);
}

//CORBA
void
TAO::FT_ReplicationManager::set_properties_dynamically (
    PortableGroup::ObjectGroup_ptr object_group,
    const PortableGroup::Properties & overrides)
{

  TAO::PG_Object_Group * group = 0;
  if (this->group_factory_.find_group (object_group, group))
  {
    group->set_properties_dynamically (overrides);
  }
  else
  {
    throw PortableGroup::ObjectGroupNotFound ();
  }
}

//CORBA
PortableGroup::Properties *
TAO::FT_ReplicationManager::get_properties (
    PortableGroup::ObjectGroup_ptr object_group)
{
  PortableGroup::Properties_var result;
  ACE_NEW_THROW_EX (result, PortableGroup::Properties(), CORBA::NO_MEMORY ());

  TAO::PG_Object_Group * group = 0;
  if (this->group_factory_.find_group (object_group, group))
  {
    group->get_properties (result);
  }
  else
  {
    throw PortableGroup::ObjectGroupNotFound ();
  }
  return result._retn();
}


//////////////////////////////////////////////////////
// FT::FTObjectGroupManager methods

/// Sets the primary member of a group.
//CORBA
PortableGroup::ObjectGroup_ptr
TAO::FT_ReplicationManager::set_primary_member (
  PortableGroup::ObjectGroup_ptr object_group,
  const PortableGroup::Location & the_location)
{
  METHOD_ENTRY (TAO::FT_ReplicationManager::set_primary_member);
  PortableGroup::ObjectGroup_var result = PortableGroup::ObjectGroup::_nil();
  TAO::PG_Object_Group * group = 0;
  if (this->group_factory_.find_group (object_group, group))
  {

    PortableGroup::TagGroupTaggedComponent tag_component;
    TAO_FT_IOGR_Property prop (tag_component);

    int sts = group->set_primary_member (&prop, the_location);
    if (sts)
    {
      result = group->reference ();
    }
    else
    {
      throw FT::PrimaryNotSet ();
    }
  }
  else
  {
    throw PortableGroup::ObjectGroupNotFound ();
  }
  METHOD_RETURN (TAO::FT_ReplicationManager::set_primary_member) result._retn ();
}

//CORBA
PortableGroup::ObjectGroup_ptr
TAO::FT_ReplicationManager::create_member (
    PortableGroup::ObjectGroup_ptr object_group,
    const PortableGroup::Location & the_location,
    const char * type_id,
    const PortableGroup::Criteria & the_criteria)
{
  PortableGroup::ObjectGroup_var result = PortableGroup::ObjectGroup::_nil();
  TAO::PG_Object_Group * group = 0;
  if (this->group_factory_.find_group (object_group, group))
  {
    group->create_member (the_location, type_id, the_criteria);
    result = group->reference ();
  }
  else
  {
    if (TAO_debug_level > 0)
    {
      ACE_ERROR ( (LM_ERROR,
        ACE_TEXT ("%T %n (%P|%t) - FT_ReplicationManager::create_member: unknown group\n")
        ));
    }
    throw PortableGroup::ObjectGroupNotFound ();
  }
  return result._retn();
}


//CORBA
PortableGroup::ObjectGroup_ptr
TAO::FT_ReplicationManager::add_member (
    PortableGroup::ObjectGroup_ptr object_group,
    const PortableGroup::Location & the_location,
    CORBA::Object_ptr member)
{
  METHOD_ENTRY (TAO::FT_ReplicationManager::add_member);
  PortableGroup::ObjectGroup_var result = PortableGroup::ObjectGroup::_nil ();

  // Find the object group corresponding to this IOGR
  TAO::PG_Object_Group * group = 0;
  if (this->group_factory_.find_group (object_group, group))
  {
    group->add_member (
      the_location,
      member);

    result = group->reference ();

  }
  else
  {
    if (TAO_debug_level > 0)
    {
      ACE_ERROR ( (LM_ERROR,
        ACE_TEXT ("%T %n (%P|%t) - FT_ReplicationManager::add_member to unknown group\n")
        ));
    }
    throw PortableGroup::ObjectGroupNotFound ();
  }
  METHOD_RETURN (TAO::FT_ReplicationManager::add_member) result._retn ();
}

//CORBA
PortableGroup::ObjectGroup_ptr
TAO::FT_ReplicationManager::remove_member (
    PortableGroup::ObjectGroup_ptr object_group,
    const PortableGroup::Location & the_location)
{
  PortableGroup::ObjectGroup_var result = PortableGroup::ObjectGroup::_nil ();

  // Find the object group corresponding to this IOGR
  TAO::PG_Object_Group * group = 0;
  if (this->group_factory_.find_group (object_group, group))
  {
    group->remove_member (the_location);

    group->minimum_populate ();
      //@@ how about the case where the member was removed successfully,
      // but for one reason or another we were unable to bring the group
      // back up to minimum_number_of_replicas?

    result = group->reference ();
  }
  else
  {
    throw PortableGroup::ObjectGroupNotFound ();
  }
  return result._retn ();
}

//CORBA
PortableGroup::Locations *
TAO::FT_ReplicationManager::locations_of_members (
    PortableGroup::ObjectGroup_ptr object_group)
{
  PortableGroup::Locations * result = 0;

  // Find the object group corresponding to this IOGR
  TAO::PG_Object_Group * group = 0;
  if (this->group_factory_.find_group (object_group, group))
  {
    result = group->locations_of_members ();
  }
  else
  {
    if (TAO_debug_level > 0)
    {
      ACE_ERROR ( (LM_ERROR,
        ACE_TEXT ("%T %n (%P|%t) - FT_ReplicationManager::locations_of_members: unknown group\n")
        ));
    }
    throw PortableGroup::ObjectGroupNotFound ();
  }
  return result;
}

//CORBA
PortableGroup::ObjectGroups *
TAO::FT_ReplicationManager::groups_at_location (
    const PortableGroup::Location & the_location)
{
  return this->group_factory_.groups_at_location (the_location);
}

//CORBA
PortableGroup::ObjectGroupId
TAO::FT_ReplicationManager::get_object_group_id (
    PortableGroup::ObjectGroup_ptr object_group)
{
  PortableGroup::ObjectGroupId result = 0;
  TAO::PG_Object_Group * group = 0;
  if (this->group_factory_.find_group (object_group, group))
  {
    group->get_object_group_id ();
    result = group->get_object_group_id ();
  }
  else
  {
    if (TAO_debug_level > 0)
    {
      ACE_ERROR ( (LM_ERROR,
        ACE_TEXT ("%T %n (%P|%t) - FT_ReplicationManager::get_object_group_id: unknown group\n")
        ));
    }
    throw PortableGroup::ObjectGroupNotFound ();
  }
  return result;
}

//CORBA
PortableGroup::ObjectGroup_ptr
TAO::FT_ReplicationManager::get_object_group_ref (
    PortableGroup::ObjectGroup_ptr object_group)
{
  PortableGroup::ObjectGroup_var result = PortableGroup::ObjectGroup::_nil ();

  // Find the object group corresponding to this IOGR
  TAO::PG_Object_Group * group = 0;
  if (this->group_factory_.find_group (object_group, group))
  {
    result = group->reference ();
  }
  else
  {
    if (TAO_debug_level > 0)
    {
      ACE_ERROR ( (LM_ERROR,
        ACE_TEXT ("%T %n (%P|%t) - FT_ReplicationManager::get_object_group_ref: unknown group\n")
        ));
    }
    throw PortableGroup::ObjectGroupNotFound ();
  }
  return result._retn();
}

//CORBA, TAO specific
PortableGroup::ObjectGroup_ptr
TAO::FT_ReplicationManager::get_object_group_ref_from_id (
    PortableGroup::ObjectGroupId group_id)
{
  PortableGroup::ObjectGroup_var result = PortableGroup::ObjectGroup::_nil ();

  // Find the object group corresponding to this IOGR
  TAO::PG_Object_Group * group = 0;
  if (this->group_factory_.find_group (group_id, group))
  {
    result = group->reference ();
  }
  else
  {
    if (TAO_debug_level > 0)
    {
      ACE_ERROR ( (LM_ERROR,
        ACE_TEXT ("%T %n (%P|%t) - FT_ReplicationManager::get_object_group_ref_from_id: unknown group\n")
        ));
    }
    throw PortableGroup::ObjectGroupNotFound ();
  }
  return result._retn();
}

//CORBA
CORBA::Object_ptr
TAO::FT_ReplicationManager::get_member_ref (
    PortableGroup::ObjectGroup_ptr object_group,
    const PortableGroup::Location & the_location)
{
  CORBA::Object_var result = CORBA::Object::_nil();

  // Find the object group corresponding to this IOGR
  TAO::PG_Object_Group * group = 0;
  if (this->group_factory_.find_group (object_group, group))
  {
    result = group->get_member_reference (the_location);
  }
  else
  {
    if (TAO_debug_level > 0)
    {
      ACE_ERROR ( (LM_ERROR,
        ACE_TEXT ("%T %n (%P|%t) - FT_ReplicationManager::get_member_ref: unknown group\n")
        ));
    }
    throw PortableGroup::ObjectGroupNotFound ();
  }
  return result._retn();
}


//////////////////////////////////////////////////////
// PortableGroup::GenericFactory methods

//CORBA
CORBA::Object_ptr
TAO::FT_ReplicationManager::create_object (
  const char * type_id,
  const PortableGroup::Criteria & the_criteria,
  PortableGroup::GenericFactory::FactoryCreationId_out factory_creation_id)
{
  METHOD_ENTRY (TAO::FT_ReplicationManager::create_object)

  ////////////////////////////////
  // find the properties for this
  // type of object group
  TAO::PG_Property_Set * typeid_properties
    = this->properties_support_.find_typeid_properties (
      type_id);

  TAO::PG_Object_Group * group
    = this->group_factory_.create_group (
      type_id,
      the_criteria,
      typeid_properties,
      false);

  group->initial_populate ();
    //@@ on error we should remove the group from the Group_Factory
    // doing this "right" will require a var-type pointer to the object group
    // that knows about the factory, too.

 // Allocate a new FactoryCreationId for use as an "out" parameter.
  PortableGroup::GenericFactory::FactoryCreationId_ptr factory_id_ptr = 0;
  ACE_NEW_THROW_EX (factory_id_ptr,
                    PortableGroup::GenericFactory::FactoryCreationId,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  PortableGroup::GenericFactory::FactoryCreationId_var factory_id = factory_id_ptr;
  PortableGroup::ObjectGroupId group_id = group->get_object_group_id ();
  factory_id <<= group_id;
  factory_creation_id = factory_id._retn();

  METHOD_RETURN (TAO::FT_ReplicationManager::create_object) group->reference ();
}

//CORBA
void
TAO::FT_ReplicationManager::delete_object (
  const PortableGroup::GenericFactory::FactoryCreationId & factory_creation_id)
{

  PortableGroup::ObjectGroupId group_id = 0;
  if (factory_creation_id >>= group_id)
  {
    this->group_factory_.delete_group (
      group_id);
  }
  else
  {
    throw PortableGroup::ObjectNotFound ();
  }
}

//private
int TAO::FT_ReplicationManager::write_ior ()
{
  int result = -1;
  FILE* out = ACE_OS::fopen (this->ior_output_file_, "w");
  if (out)
  {
    CORBA::String_var ior_str = this->orb_->object_to_string (
      this->replication_manager_ref_.in ());
    ACE_OS::fprintf (out, "%s", ior_str.in ());
    ACE_OS::fclose (out);
    result = 0;
  }
  else
  {
    ACE_ERROR ( (LM_ERROR,
      ACE_TEXT ("%T %n (%P|%t) - Open failed for %s\n"), this->ior_output_file_
    ));
  }
  return result;
}

TAO_END_VERSIONED_NAMESPACE_DECL

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
  , iorm_(TAO_IOP::TAO_IOR_Manipulation::_nil())
  , ior_output_file_(0)
  , ns_name_(0)
  , replication_manager_ref_(FT::ReplicationManager::_nil())
  , naming_context_(CosNaming::NamingContext::_nil())
  , object_group_manager_()
  , property_manager_ (this->object_group_manager_)
  , generic_factory_ (this->object_group_manager_, this->property_manager_)
  , fault_notifier_(TAO::FT_FaultNotifier::_nil())
  , fault_consumer_()
  , test_iogr_(FT::ObjectGroup::_nil())
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
  ACE_Get_Opt get_opts (argc, argv, "n:o:");
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
  PortableServer::POA_var poa =
    PortableServer::POA::_narrow (poa_obj.in() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Initialize the Object Group Manager.
  this->object_group_manager_.init (this->orb_in(), poa.in());

  // Get an object reference for the ORBs IORManipulation object.
  CORBA::Object_var iorm_obj = this->orb_->resolve_initial_references (
    TAO_OBJID_IORMANIPULATION ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  this->iorm_ = TAO_IOP::TAO_IOR_Manipulation::_narrow (
    iorm_obj.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Activate ourselves in the POA.
  PortableServer::ObjectId_var oid = poa->activate_object (
    this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  CORBA::Object_var this_obj = poa->id_to_reference (
    oid.in() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  this->replication_manager_ref_ = FT::ReplicationManager::_narrow (
    this_obj.in() ACE_ENV_ARG_PARAMETER);
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

    CORBA::Object_var naming_obj =
      this->orb_->resolve_initial_references ("NameService" ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    if (CORBA::is_nil(naming_obj.in ())){
      ACE_ERROR_RETURN ((LM_ERROR,
                        "%T %n (%P|%t) Unable to find the Naming Service\n"),
                        1);
    }

    this->naming_context_ =
      CosNaming::NamingContext::_narrow (naming_obj.in () ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    this->this_name_.length (1);
    this->this_name_[0].id = CORBA::string_dup (this->ns_name_);

    //@@ Do NOT use _this() here.  Need to use the POA with which we
    // were activated to get the IOR.  Another reason not to use the
    // TAO_ORB_Manager; it does not give us much flexibility w.r.t.
    // POA usage.  -- Steve Totten
    this->naming_context_->rebind (this->this_name_, _this()
                            ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
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
  if (this->ior_output_file_ != 0)
  {
    ACE_OS::unlink (this->ior_output_file_);
    this->ior_output_file_ = 0;
  }
  if (this->ns_name_ != 0)
  {
    this->naming_context_->unbind (this->this_name_
                            ACE_ENV_ARG_PARAMETER);
    this->ns_name_ = 0;
  }
  return 0;
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
  return FT::FaultNotifier::_duplicate (this->fault_notifier_.in());
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
      this->test_iogr_group_id_ = (CORBA::ULongLong) 10;

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
        this->orb_->string_to_object ("corbaloc:iiop:acme.cs.wustl.edu:6060/xyz"
                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      CORBA::Object_var name2 =
        this->orb_->string_to_object ("corbaloc::iiop:tango.cs.wustl.edu:7070/xyz"
                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Create IOR list for use with merge_iors.
      TAO_IOP::TAO_IOR_Manipulation::IORList iors (3);
      iors.length (3);
      iors [0] = this->object_group_manager_.create_object_group(
                         this->test_iogr_group_id_,
                         domain_id,
                         "my-dummy-type-id",
                         props_in);
      iors [1] = name1;
      iors [2] = name2;

      this->test_iogr_ = this->iorm_->merge_iors (iors ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // we only need this so we can call IORManipulation's set_primary
      FT::TagFTGroupTaggedComponent ft_tag_component;
      TAO_FT_IOGR_Property ft_prop (ft_tag_component);

      // set primary
      CORBA::Boolean retval =
        this->iorm_->set_primary (&ft_prop, name2.in (), this->test_iogr_.in () ACE_ENV_ARG_PARAMETER);
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

  //@@ Object_var (this->test_iogr_) must give up ownership to return IOGR
  // as an Object_ptr.  -- Steve Totten
  return this->test_iogr_._retn();
}


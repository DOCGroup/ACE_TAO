/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    FT_FaultDetectorFactory_i.cpp
 *
 *  $Id$
 *
 *  This file is part of Fault Tolerant CORBA.
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================
#include "orbsvcs/Log_Macros.h"
#include "FT_FaultDetectorFactory_i.h"
#include "Fault_Detector_i.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Auto_Ptr.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/PortableGroup/PG_Property_Set.h"

// Use this macro at the beginning of CORBA methods
// to aid in debugging.
#define METHOD_ENTRY(name)    \
    ORBSVCS_DEBUG (( LM_DEBUG,    \
    "Enter %s\n", #name       \
      ))

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
    ORBSVCS_DEBUG (( LM_DEBUG,    \
      "Leave %s\n", #name     \
      ));                     \
    return /* value goes here */


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

//////////////////////////////////////////////////////
// FT_FaultDetectorFactory_i  Construction/destruction

TAO::FT_FaultDetectorFactory_i::FT_FaultDetectorFactory_i ()
  : orb_ (0)
  , poa_ (0)
  , objectId_ (0)
  , ior_output_file_ (0)
  , ns_name_ (0)
  , naming_context_ (0)
  , this_name_ (1)
  , quit_on_idle_ (0)
  , domain_ (CORBA::string_dup("default_domain"))
  , location_ (1)
  , notifier_ (0)
  , rm_register_ (1)
  , replication_manager_ (0)
  , registered_ (0)
  , factory_registry_ (0)
  , identity_ ("")
  , empty_slots_ (0)
  , quit_requested_ (0)

{
  this->location_.length(1);
  this->location_[0].id = CORBA::string_dup("default_location");
}

TAO::FT_FaultDetectorFactory_i::~FT_FaultDetectorFactory_i ()
{
  //scope the guard
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->internals_);

    // be sure all detectors are gone
    // before this object disappears
    shutdown_i ();
  }
  fini ();
  this->threadManager_.close ();
}

////////////////////////////////////////////
// FT_FaultDetectorFactory_i private methods

void TAO::FT_FaultDetectorFactory_i::shutdown_i(void)
{
  // assume mutex is locked
  for (size_t nDetector = 0; nDetector < this->detectors_.size(); ++nDetector)
  {
    Fault_Detector_i * detector = this->detectors_[nDetector];
    if (detector != 0)
    {
      detector->request_quit();
    }
  }
}

int TAO::FT_FaultDetectorFactory_i::write_ior(void)
{
  int result = -1;
  FILE* out = ACE_OS::fopen (this->ior_output_file_, "w");
  if (out)
  {
    ACE_OS::fprintf (out, "%s", this->ior_.in ());
    ACE_OS::fclose (out);
    result = 0;
  }
  else
  {
    ORBSVCS_ERROR ((LM_ERROR,
      "Open failed for %s\n", ior_output_file_
    ));
  }
  return result;
}

//////////////////////////////////////////////////////
// FT_FaultDetectorFactory_i public, non-CORBA methods

int TAO::FT_FaultDetectorFactory_i::parse_args (int argc, ACE_TCHAR * argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("d:l:o:qr"));
  int c;

  while ((c = get_opts ()) != -1)
  {
    switch (c)
    {
      case 'd':
      {
        this->domain_ = CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR(get_opts.opt_arg ()));
        break;
      }
      case 'l':
      {
        this->location_.length(1);
        this->location_[0].id = CORBA::string_dup(ACE_TEXT_ALWAYS_CHAR(get_opts.opt_arg ()));
        break;
      }
      case 'o':
      {
        this->ior_output_file_ = get_opts.opt_arg ();
        break;
      }
      case 'r':
      {
        this->rm_register_ = ! this->rm_register_;
        break;
      }
      case 'q':
      {
        this->quit_on_idle_ = 1;
        break;
      }

      case '?':
        // fall thru
      default:
        ORBSVCS_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " -d <FT Domain>"
                           " -l <FT Location>"
                           " -o <iorfile>"
                           " -r <disable registering with replication manager>"
                           " -q{uit on idle}"
                           "\n",
                           argv [0]),
                          -1);
      break;
    }
  }
  // Indicates successful parsing of the command line
  return 0;
}

const char * TAO::FT_FaultDetectorFactory_i::identity () const
{
  return this->identity_.c_str();
}

int TAO::FT_FaultDetectorFactory_i::idle (int & result)
{
  ACE_UNUSED_ARG (result);
  int quit = this->quit_requested_;
  if (quit == 0 && this->detectors_.size() == this->empty_slots_)
  {
    // don't quitOnIdle until something has happened
    if (this->quit_on_idle_ && this->empty_slots_ != 0)
    {
      ORBSVCS_ERROR (( LM_INFO,
        "FaultDetectorFactory exits due to quit on idle option.\n"
        ));
      quit = 1;
    }
  }

  return quit;
}


int TAO::FT_FaultDetectorFactory_i::init (CORBA::ORB_ptr orb)
{
  int result = 0;
  this->orb_ = CORBA::ORB::_duplicate (orb);

  // Use the ROOT POA for now
  CORBA::Object_var poa_object =
    this->orb_->resolve_initial_references (TAO_OBJID_ROOTPOA);

  if (CORBA::is_nil (poa_object.in ()))
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT (" (%P|%t) Unable to initialize the POA.\n")),
                      -1);

  // Get the POA object.
  this->poa_ =
    PortableServer::POA::_narrow (poa_object.in ());


  if (CORBA::is_nil(this->poa_.in ()))
  {
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT (" (%P|%t) Unable to narrow the POA.\n")),
                      -1);
  }

  PortableServer::POAManager_var poa_manager =
    this->poa_->the_POAManager ();

  poa_manager->activate ();

  // Register with the POA.

  this->objectId_ = this->poa_->activate_object (this);

  // find my IOR

  CORBA::Object_var this_obj =
    this->poa_->id_to_reference (objectId_.in ());

  this->ior_ = this->orb_->object_to_string (this_obj.in ());

  this->identity_ = "FaultDetectorFactory";

  ///////////////////////////////
  // Register with ReplicationManager
  if (this->rm_register_)
  {
    try
    {
      CORBA::Object_var rm_obj = orb->resolve_initial_references("ReplicationManager");
      this->replication_manager_ = ::FT::ReplicationManager::_narrow(rm_obj.in());
      if (!CORBA::is_nil (replication_manager_.in ()))
      {
        // capture the default notifier
        this->notifier_ =  this->replication_manager_->get_fault_notifier ();

        // register with ReplicationManager::RegistrationFactory
        PortableGroup::Criteria criteria(0);
        this->factory_registry_ = this->replication_manager_->get_factory_registry (criteria);

        if (! CORBA::is_nil(factory_registry_.in ()))
        {
          PortableGroup::FactoryInfo info;
          info.the_factory = ::PortableGroup::GenericFactory::_narrow(this_obj.in ());
          info.the_location = this->location_;
          info.the_criteria.length(1);
          info.the_criteria[0].nam.length(1);
          info.the_criteria[0].nam[0].id = CORBA::string_dup(PortableGroup::role_criterion);
          info.the_criteria[0].val <<= CORBA::string_dup(FT::FAULT_DETECTOR_ROLE_NAME);

          ORBSVCS_DEBUG ((LM_DEBUG,
            "FaultDetector registering with ReplicationManager.\n"
            ));
          this->factory_registry_->register_factory(
            FT::FAULT_DETECTOR_ROLE_NAME,
            FT::FAULT_DETECTOR_ROLE_NAME,
            info);

          ORBSVCS_DEBUG ((LM_DEBUG,
            "FaultDetector Registration complete.\n"
            ));
          this->registered_ = 1;
        }
        else
        {
          ORBSVCS_ERROR ((LM_ERROR,"FaultNotifier: ReplicationManager doesn't have RegistrationFactory.\n" ));
        }
      }
      else
      {
        ORBSVCS_ERROR ((LM_ERROR,"FaultNotifier: Can't resolve ReplicationManager, It will not be registered.\n" ));
      }
    }
    catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "ReplicaFactory: Exception resolving ReplicationManager.  Factory will not be registered.\n");
    }
  }
  else
  {
    ORBSVCS_DEBUG ((LM_DEBUG,
      "FaultNotifier: ReplicationManager registration disabled.\n"
      ));
  }

  if (this->ior_output_file_ != 0)
  {
    this->identity_ = "file:";
    this->identity_ += ACE_TEXT_ALWAYS_CHAR(this->ior_output_file_);
    write_ior();
  }

  if (this->ns_name_ != 0)
  {
    this->identity_ = "name:";
    this->identity_ += this->ns_name_;

    CORBA::Object_var naming_obj =
      this->orb_->resolve_initial_references ("NameService");

    if (CORBA::is_nil(naming_obj.in ())){
      ORBSVCS_ERROR_RETURN ((LM_ERROR,
                         "%T %n (%P|%t) Unable to find the Naming Service\n"),
                        1);
    }

    this->naming_context_ =
      ::CosNaming::NamingContext::_narrow (naming_obj.in ());

    this->this_name_.length (1);
    this->this_name_[0].id = CORBA::string_dup (this->ns_name_);

    this->naming_context_->rebind (this->this_name_, this_obj.in());
  }

  return result;
}

int TAO::FT_FaultDetectorFactory_i::fini (void)
{
  if (this->ior_output_file_ != 0)
  {
    ACE_OS::unlink (this->ior_output_file_);
    this->ior_output_file_ = 0;
  }
  if (this->ns_name_ != 0)
  {
    this->naming_context_->unbind (this_name_);
    this->ns_name_ = 0;
  }

  if (this->registered_)
  {
    this->factory_registry_->unregister_factory(
      FT::FAULT_DETECTOR_ROLE_NAME,
      this->location_);
    this->registered_ = 0;
  }
  return 0;
}


CORBA::ULong TAO::FT_FaultDetectorFactory_i::allocate_id()
{
  CORBA::ULong id = this->detectors_.size();
  if (this->empty_slots_ != 0)
  {
    for(CORBA::ULong pos = 0; pos < id; ++pos)
    {
      if (this->detectors_[pos] == 0)
      {
        id = pos;
      }
    }
  }
  else
  {
    this->detectors_.push_back(0);
    this->empty_slots_ += 1;
  }
  return id;
}

void TAO::FT_FaultDetectorFactory_i::remove_detector(CORBA::ULong id, TAO::Fault_Detector_i * detector)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->internals_);

  if (id < this->detectors_.size())
  {
    if(this->detectors_[id] == detector)
    {
      delete this->detectors_[id];
      this->detectors_[id] = 0;
      this->empty_slots_ += 1;
      if (this->empty_slots_ == this->detectors_.size())
      {
        ORBSVCS_ERROR (( LM_INFO,
          "FaultDetectorFactory is idle.\n"
          ));
      }
    }
    else
    {
      ORBSVCS_ERROR (( LM_ERROR,
        "Remove detector %d mismatch.\n",
        static_cast<int> (id)
        ));
    }
  }
  else
  {
    ORBSVCS_ERROR (( LM_ERROR,
      "Attempt to remove invalid detector %d. Limit %d.\n",
      static_cast<int> (id),
      static_cast<int> (this->detectors_.size())
      ));
  }
}

//////////////////////////////////////////
// FT_FaultDetectorFactory_i CORBA methods

void TAO::FT_FaultDetectorFactory_i::change_properties (
    const PortableGroup::Properties & property_set)
{
  METHOD_ENTRY(TAO::FT_FaultDetectorFactory_i::change_properties);

    // TimeT is expressed in 10E-7 seconds (== 100 nSec == 0.1 uSec)
  static const long timeT_per_uSec = 10L;
  static const long uSec_per_sec = 1000000L;

  ::TAO::PG_Property_Set decoder(property_set);

  TimeBase::TimeT value = 0;
  if( TAO::find (decoder, FT::FT_FAULT_MONITORING_INTERVAL, value) )
  {
    // note: these should be unsigned long, but
    // ACE_Time_Value wants longs.
    long uSec = static_cast<long> ((value / timeT_per_uSec) % uSec_per_sec);
    long sec = static_cast<long> ((value / timeT_per_uSec) / uSec_per_sec);
    ACE_Time_Value atv(sec, uSec);
    TAO::Fault_Detector_i::set_time_for_all_detectors(atv);
  }
  else
  {
    ORBSVCS_ERROR ((LM_ERROR,
      "Throwing Invalid Property: %s\n",
      FT::FT_FAULT_MONITORING_INTERVAL
      ));
    ::PortableGroup::InvalidProperty ex;
    ex.nam.length(1);
    ex.nam[0].id = CORBA::string_dup(FT::FT_FAULT_MONITORING_INTERVAL);
    throw ::PortableGroup::InvalidProperty (ex);
  }
  METHOD_RETURN(TAO::FT_FaultDetectorFactory_i::change_properties);
}

void TAO::FT_FaultDetectorFactory_i::shutdown (void)
{
  METHOD_ENTRY(TAO::FT_FaultDetectorFactory_i::shutdown);
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->internals_);
  shutdown_i ();
  this->quit_requested_ = 1;
  METHOD_RETURN(TAO::FT_FaultDetectorFactory_i::shutdown);
}

CORBA::Object_ptr TAO::FT_FaultDetectorFactory_i::create_object (
    const char * type_id,
    const PortableGroup::Criteria & the_criteria,
    PortableGroup::GenericFactory::FactoryCreationId_out factory_creation_id)
{
  METHOD_ENTRY(TAO::FT_FaultDetectorFactory_i::create_object);

  ACE_UNUSED_ARG (type_id); //@@ use it
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->internals_, CORBA::Object::_nil ());

  ::TAO::PG_Property_Set decoder (the_criteria);

  // boolean, becomes true if a required parameter is missing
  int missingParameter = 0;
  const char * missingParameterName = 0;

  FT::FaultNotifier_ptr notifier;
  if (! ::TAO::find (decoder, ::FT::FT_NOTIFIER, notifier) )
  {
    if (! CORBA::is_nil (this->notifier_.in ()))
    {
      notifier = FT::FaultNotifier::_duplicate (this->notifier_.in ());
    }
    else
    {
      ORBSVCS_ERROR ((LM_ERROR,
        "FaultDetectorFactory::create_object: Missing parameter %s\n",
        ::FT::FT_NOTIFIER
        ));
      missingParameter = 1;
      missingParameterName = ::FT::FT_NOTIFIER;
    }
  }

  FT::PullMonitorable_ptr monitorable;
  if (! ::TAO::find (decoder, ::FT::FT_MONITORABLE, monitorable) )
  {
    ORBSVCS_ERROR ((LM_ERROR,
      "FaultDetectorFactory::create_object: Missing parameter %s\n",
      ::FT::FT_MONITORABLE
      ));
    missingParameter = 1;
    missingParameterName = ::FT::FT_MONITORABLE;
  }

  FT::FTDomainId domain_id = 0;
  // note the cast in the next line makes ANY >>= work.
  const char * domain_id_string = 0;
  if (::TAO::find (decoder, ::FT::FT_DOMAIN_ID, domain_id_string) )
  {
    // NOTE the assumption that we can assign a char * to a domain id
    domain_id = const_cast<char *> (domain_id_string);
  }
  else
  {
    domain_id = this->domain_;

//    ORBSVCS_ERROR ((LM_ERROR,
//      "FaultDetectorFactory::create_object: Missing parameter %s\n",
//      ::FT::FT_DOMAIN_ID
//      ));
//    missingParameter = 1;
//    missingParameterName = ::FT::FT_DOMAIN_ID;
  }

  PortableGroup::Location * object_location = 0;
  if (! ::TAO::find (decoder, ::FT::FT_LOCATION, object_location) )
  {
      object_location = & this->location_;

//    ORBSVCS_ERROR ((LM_ERROR,
//      "FaultDetectorFactory::create_object: Missing parameter %s\n",
//      ::FT::FT_LOCATION
//      ));
//    missingParameter = 1;
//    missingParameterName = ::FT::FT_LOCATION;
  }

  PortableGroup::TypeId object_type = 0;
  const char * object_type_string;
  if (::TAO::find (decoder, ::FT::FT_TYPE_ID, object_type_string))
  {
    object_type = const_cast<char *> (object_type_string);
  }
  else
  {
    object_type = const_cast<char *> ("unknown");
    // Not required: missingParameter = 1;
    ORBSVCS_DEBUG ((LM_DEBUG, "Object type not given.\n"));
  }

  FT::ObjectGroupId group_id = 0;
  if (! ::TAO::find (decoder, ::FT::FT_GROUP_ID, group_id) )
  {
    // Not required: missingParameter = 1;
  }

  if (missingParameter)
  {
    ORBSVCS_ERROR ((LM_ERROR,
      "Throwing 'InvalidCriteria' due to missing %s\n",
      missingParameterName
      ));
    throw PortableGroup::InvalidCriteria();
  }

  CORBA::ULong detectorId = allocate_id();

  // NOTE: ACE_NEW is incompatable with auto_ptr
  // so create a bare pointer first.
  TAO::Fault_Detector_i * pFD = 0;

  ACE_NEW_NORETURN(pFD, TAO::Fault_Detector_i(
    *this,
    detectorId,
    notifier,
    monitorable,
    domain_id,
    *object_location,
    object_type,
    group_id));
  if (pFD == 0)
  {
    ORBSVCS_ERROR ((LM_ERROR,
      "New FaultDetector_i returned NULL.  Throwing ObjectNotCreated.\n"
      ));
    throw PortableGroup::ObjectNotCreated();
  }
  auto_ptr<TAO::Fault_Detector_i> detector(pFD);

  ACE_NEW_NORETURN ( factory_creation_id,
    PortableGroup::GenericFactory::FactoryCreationId);
  if (factory_creation_id.ptr() == 0)
  {
    ORBSVCS_ERROR ((LM_ERROR,
      "New factory_creation_id returned NULL.  Throwing ObjectNotCreated.\n"
      ));

    throw PortableGroup::ObjectNotCreated();
  }
  (*factory_creation_id) <<= detectorId;

  (*detector).start(this->threadManager_);

  this->detectors_[detectorId] = detector.release();
  this->empty_slots_ -= 1;

  // since FaultDetector is not a CORBA object (it does not implement
  // an interface.) we always return NIL;
  METHOD_RETURN(TAO::FT_FaultDetectorFactory_i::create_object)
    CORBA::Object::_nil();
}

void TAO::FT_FaultDetectorFactory_i::delete_object (
    const PortableGroup::GenericFactory::FactoryCreationId & factory_creation_id)
{
  METHOD_ENTRY(TAO::FT_FaultDetectorFactory_i::delete_object);

  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->internals_);

  CORBA::ULong detectorId;
  factory_creation_id >>= detectorId;
  if (detectorId < this->detectors_.size())
  {
    if(this->detectors_[detectorId] != 0)
    {
      this->detectors_[detectorId]->request_quit();
    }
    else
    {
      throw ::PortableGroup::ObjectNotFound();
    }
  }
  else
  {
    throw ::PortableGroup::ObjectNotFound();
  }
  METHOD_RETURN(TAO::FT_FaultDetectorFactory_i::delete_object);
}

CORBA::Boolean TAO::FT_FaultDetectorFactory_i::is_alive (void)
{
  METHOD_RETURN(TAO::FT_FaultDetectorFactory_i::is_alive)
    1;
}

TAO_END_VERSIONED_NAMESPACE_DECL

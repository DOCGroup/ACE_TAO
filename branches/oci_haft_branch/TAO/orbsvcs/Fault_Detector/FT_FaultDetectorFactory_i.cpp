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
#include "FT_FaultDetectorFactory_i.h"
#include "Fault_Detector_i.h"
#include "ace/Get_Opt.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/PortableGroup/PG_Properties_Decoder.h"

// Use this macro at the beginning of CORBA methods
// to aid in debugging.
#define METHOD_ENTRY(name)    \
    ACE_DEBUG (( LM_DEBUG,    \
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
    ACE_DEBUG (( LM_DEBUG,    \
      "Leave %s\n", #name     \
      ));                     \
    return /* value goes here */


//////////////////////////////////////////////////////
// FT_FaultDetectorFactory_i  Construction/destruction

TAO::FT_FaultDetectorFactory_i::FT_FaultDetectorFactory_i ()
  : ior_output_file_ (0)
  , ns_name_ (0)
  , quit_on_idle_ (0)
  , empty_slots_ (0)
  , quit_requested_ (0)
  , domain_ (CORBA::string_dup("default_domain"))
  , location_ (1)
  , rm_register_ (1)
  , replication_manager_ (0)
  , factory_registry_ (0)
  , registered_ (0)
{
  this->location_.length(1);
  this->location_[0].id = CORBA::string_dup("default_location");
}

TAO::FT_FaultDetectorFactory_i::~FT_FaultDetectorFactory_i ()
{
  //scope the guard
  {
    InternalGuard guard (this->internals_);

    // be sure all detectors are gone
    // before this object disappears
    shutdown_i ();
  }
  ACE_DECLARE_NEW_ENV;
  fini (ACE_ENV_SINGLE_ARG_PARAMETER);
  this->threadManager_.close ();
}

////////////////////////////////////////////
// FT_FaultDetectorFactory_i private methods

void TAO::FT_FaultDetectorFactory_i::shutdown_i()
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

int TAO::FT_FaultDetectorFactory_i::write_ior()
{
  int result = -1;
  FILE* out = ACE_OS::fopen (this->ior_output_file_, "w");
  if (out)
  {
    ACE_OS::fprintf (out, "%s", ACE_static_cast(const char *, this->ior_));
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

//////////////////////////////////////////////////////
// FT_FaultDetectorFactory_i public, non-CORBA methods

int TAO::FT_FaultDetectorFactory_i::parse_args (int argc, char * argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "d:l:o:q");
  int c;

  while ((c = get_opts ()) != -1)
  {
    switch (c)
    {
      case 'd':
        this->domain_ = CORBA::string_dup (get_opts.opt_arg ());
        break;
      case 'l':
        this->location_.length(1);
        this->location_[0].id = CORBA::string_dup(get_opts.opt_arg ());
        break;
      case 'o':
        this->ior_output_file_ = get_opts.opt_arg ();
        break;
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
        ACE_ERROR_RETURN ((LM_ERROR,
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
  // Indicates sucessful parsing of the command line
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
      ACE_ERROR (( LM_INFO,
        "FaultDetectorFactory exits due to quit on idle option.\n"
        ));
      quit = 1;
    }
  }

  return quit;
}


int TAO::FT_FaultDetectorFactory_i::init (CORBA::ORB_var & orb ACE_ENV_ARG_DECL)
{
  int result = 0;
  this->orb_ = orb;

  // Use the ROOT POA for now
  CORBA::Object_var poa_object =
    this->orb_->resolve_initial_references (TAO_OBJID_ROOTPOA
                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (CORBA::is_nil (poa_object.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT (" (%P|%t) Unable to initialize the POA.\n")),
                      -1);

  // Get the POA object.
  this->poa_ =
    PortableServer::POA::_narrow (poa_object.in ()
                                  ACE_ENV_ARG_PARAMETER);

  ACE_CHECK_RETURN (-1);

  if (CORBA::is_nil(this->poa_))
  {
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT (" (%P|%t) Unable to narrow the POA.\n")),
                      -1);
  }

  PortableServer::POAManager_var poa_manager =
    this->poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_TRY_CHECK;

  poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_TRY_CHECK;

  // Register with the POA.

  this->objectId_ = this->poa_->activate_object (this ACE_ENV_ARG_PARAMETER);
  ACE_TRY_CHECK;

  // find my IOR

  CORBA::Object_var this_obj =
    this->poa_->id_to_reference (objectId_.in ()
                                 ACE_ENV_ARG_PARAMETER);
  ACE_TRY_CHECK;

  this->ior_ = this->orb_->object_to_string (this_obj.in ()
                                  ACE_ENV_ARG_PARAMETER);
  ACE_TRY_CHECK;

  this->identity_ = "FaultDetectorFactory";

  ///////////////////////////////
  // Register with ReplicationManager
  if (this->rm_register_)
  {
    ACE_TRY_NEW_ENV
    {
      CORBA::Object_var rm_obj = orb->resolve_initial_references("ReplicationManager" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      this->replication_manager_ = ::FT::ReplicationManager::_narrow(rm_obj.in() ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (!CORBA::is_nil (replication_manager_))
      {
        // capture the default notifier
        this->notifier_ =  this->replication_manager_->get_fault_notifier (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;

        // register with ReplicationManager::RegistrationFactory
        PortableGroup::Criteria criteria(0);
        this->factory_registry_ = this->replication_manager_->get_factory_registry (criteria ACE_ENV_ARG_PARAMETER)
        ACE_TRY_CHECK;

        if (! CORBA::is_nil(factory_registry_))
        {
          PortableGroup::FactoryInfo info;
          info.the_factory = ::PortableGroup::GenericFactory::_narrow(this_obj);
          info.the_location = this->location_;
          info.the_criteria.length(1);
          info.the_criteria[0].nam.length(1);
          info.the_criteria[0].nam[0].id = CORBA::string_dup(PortableGroup::role_criterion);
          info.the_criteria[0].val <<= CORBA::string_dup(FT::FAULT_DETECTOR_ROLE_NAME);

          ACE_DEBUG ((LM_DEBUG,
            "FaultDetector registering with ReplicationManager.\n"
            ));
          this->factory_registry_->register_factory(
            FT::FAULT_DETECTOR_ROLE_NAME,
            FT::FAULT_DETECTOR_ROLE_NAME,
            info
            ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          ACE_DEBUG ((LM_DEBUG,
            "FaultDetector Registration complete.\n"
            ));
          this->registered_ = 1;
        }
        else
        {
          ACE_ERROR ((LM_ERROR,"FaultNotifier: ReplicationManager doesn't have RegistrationFactory.\n" ));
        }
      }
      else
      {
        ACE_ERROR ((LM_ERROR,"FaultNotifier: Can't resolve ReplicationManager, It will not be registered.\n" ));
      }
    }
    ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
        "ReplicaFactory: Exception resolving ReplicationManager, and no -f option was given.  Factory will not be registered.\n" );
    }
    ACE_ENDTRY;
  }
  else
  {
    ACE_DEBUG ((LM_DEBUG,
      "FaultNotifier: ReplicationManager registration disabled.\n"
      ));
  }

  if (this->ior_output_file_ != 0)
  {
    this->identity_ = "file:";
    this->identity_ += this->ior_output_file_;
    write_ior();
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
      ::CosNaming::NamingContext::_narrow (naming_obj.in () ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    this->this_name_.length (1);
    this->this_name_[0].id = CORBA::string_dup (this->ns_name_);

    this->naming_context_->rebind (this->this_name_, this_obj.in()  //CORBA::Object::_duplicate(this_obj)
                            ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
  }

  return result;
}

int TAO::FT_FaultDetectorFactory_i::fini (ACE_ENV_SINGLE_ARG_DECL)
{
  if (this->ior_output_file_ != 0)
  {
    ACE_OS::unlink (this->ior_output_file_);
    this->ior_output_file_ = 0;
  }
  if (this->ns_name_ != 0)
  {
    this->naming_context_->unbind (this_name_
                            ACE_ENV_ARG_PARAMETER);
    this->ns_name_ = 0;
  }

  if (this->registered_)
  {
    this->factory_registry_->unregister_factory(
      FT::FAULT_DETECTOR_ROLE_NAME,
      this->location_
      ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
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
  InternalGuard guard (this->internals_);
  if (id < this->detectors_.size())
  {
    if(this->detectors_[id] == detector)
    {
      delete this->detectors_[id];
      this->detectors_[id] = 0;
      this->empty_slots_ += 1;
      if (this->empty_slots_ == this->detectors_.size())
      {
        ACE_ERROR (( LM_INFO,
          "FaultDetectorFactory is idle.\n"
          ));
      }
    }
    else
    {
      ACE_ERROR (( LM_ERROR,
        "Remove detector %d mismatch.\n",
        ACE_static_cast(int, id)
        ));
    }
  }
  else
  {
    ACE_ERROR (( LM_ERROR,
      "Attempt to remove invalid detector %d. Limit %d.\n",
      ACE_static_cast(int, id),
      ACE_static_cast(int, this->detectors_.size())
      ));
  }
}

//////////////////////////////////////////
// FT_FaultDetectorFactory_i CORBA methods

void TAO::FT_FaultDetectorFactory_i::change_properties (
    const FT::Properties & property_set
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , PortableGroup::InvalidProperty
  ))
{
  METHOD_ENTRY(TAO::FT_FaultDetectorFactory_i::change_properties);

    // TimeT is expressed in 10E-7 seconds (== 100 nSec == 0.1 uSec)
  static const long timeT_per_uSec = 10L;
  static const long uSec_per_sec = 1000000L;

  ::TAO_PG::Properties_Decoder decoder(property_set);

  TimeBase::TimeT value = 0;
  if( TAO_PG::find (decoder, FT::FT_FAULT_MONITORING_INTERVAL, value) )
  {
    // note: these should be unsigned long, but
    // ACE_Time_Value wants longs.
    long uSec = ACE_static_cast (long, (value / timeT_per_uSec) % uSec_per_sec);
    long sec = ACE_static_cast (long, (value / timeT_per_uSec) / uSec_per_sec);
    ACE_Time_Value atv(sec, uSec);
    TAO::Fault_Detector_i::set_time_for_all_detectors(atv);
  }
  else
  {
    ACE_ERROR ((LM_ERROR,
      "Throwing Invalid Property: %s\n",
      FT::FT_FAULT_MONITORING_INTERVAL
      ));
    ::PortableGroup::InvalidProperty ex;
    ex.nam.length(1);
    ex.nam[0].id = CORBA::string_dup(FT::FT_FAULT_MONITORING_INTERVAL);
    ACE_THROW (ex);
  }
  METHOD_RETURN(TAO::FT_FaultDetectorFactory_i::change_properties);
}

void TAO::FT_FaultDetectorFactory_i::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  METHOD_ENTRY(TAO::FT_FaultDetectorFactory_i::shutdown);
  InternalGuard guard (this->internals_);
  shutdown_i ();
  this->quit_requested_ = 1;
  METHOD_RETURN(TAO::FT_FaultDetectorFactory_i::shutdown);
}

CORBA::Object_ptr TAO::FT_FaultDetectorFactory_i::create_object (
    const char * type_id,
    const FT::Criteria & the_criteria,
    FT::GenericFactory::FactoryCreationId_out factory_creation_id
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , PortableGroup::NoFactory
    , PortableGroup::ObjectNotCreated
    , PortableGroup::InvalidCriteria
    , PortableGroup::InvalidProperty
    , PortableGroup::CannotMeetCriteria
  ))
{
  METHOD_ENTRY(TAO::FT_FaultDetectorFactory_i::create_object);
  InternalGuard guard (this->internals_);

  ::TAO_PG::Properties_Decoder decoder (the_criteria);

  // boolean, becomes true if a required parameter is missing
  int missingParameter = 0;
  const char * missingParameterName = 0;

  FT::FaultNotifier_ptr notifier;
  if (! ::TAO_PG::find (decoder, ::FT::FT_NOTIFIER, notifier) )
  {
    if (! CORBA::is_nil (this->notifier_))
    {
      notifier = FT::FaultNotifier::_duplicate (this->notifier_);
    }
    else
    {
      ACE_ERROR ((LM_ERROR,
        "FaultDetectorFactory::create_object: Missing parameter %s\n",
        ::FT::FT_NOTIFIER
        ));
      missingParameter = 1;
      missingParameterName = ::FT::FT_NOTIFIER;
    }
  }

  FT::PullMonitorable_ptr monitorable;
  if (! ::TAO_PG::find (decoder, ::FT::FT_MONITORABLE, monitorable) )
  {
    ACE_ERROR ((LM_ERROR,
      "FaultDetectorFactory::create_object: Missing parameter %s\n",
      ::FT::FT_MONITORABLE
      ));
    missingParameter = 1;
    missingParameterName = ::FT::FT_MONITORABLE;
  }

  FT::FTDomainId domain_id = 0;
  if (! ::TAO_PG::find (decoder, ::FT::FT_DOMAIN_ID, domain_id) )
  {
    domain_id = this->domain_;

//    ACE_ERROR ((LM_ERROR,
//      "FaultDetectorFactory::create_object: Missing parameter %s\n",
//      ::FT::FT_DOMAIN_ID
//      ));
//    missingParameter = 1;
//    missingParameterName = ::FT::FT_DOMAIN_ID;
  }

  FT::Location * object_location = 0;
  if (! ::TAO_PG::find (decoder, ::FT::FT_LOCATION, object_location) )
  {
      object_location = & this->location_;

//    ACE_ERROR ((LM_ERROR,
//      "FaultDetectorFactory::create_object: Missing parameter %s\n",
//      ::FT::FT_LOCATION
//      ));
//    missingParameter = 1;
//    missingParameterName = ::FT::FT_LOCATION;
  }

  FT::TypeId object_type = 0;
  if (! ::TAO_PG::find (decoder, ::FT::FT_TYPE_ID, object_type) )
  {
    object_type = "unknown";
    // Not required: missingParameter = 1;
    ACE_DEBUG ((LM_DEBUG, "Object type not given.\n"));
  }
  else
  {
    ACE_DEBUG ((LM_DEBUG, "Object type: %s\n", object_type));
  }

  FT::ObjectGroupId group_id = 0;
  if (! ::TAO_PG::find (decoder, ::FT::FT_GROUP_ID, group_id) )
  {
    // Not required: missingParameter = 1;
  }

  if (missingParameter)
  {
    ACE_ERROR ((LM_ERROR,
      "Throwing 'InvalidCriteria' due to missing %s\n",
      missingParameterName
      ));
    ACE_THROW ( PortableGroup::InvalidCriteria() );
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
    ACE_ERROR ((LM_ERROR,
      "New FaultDetector_i returned NULL.  Throwing ObjectNotCreated.\n"
      ));
    ACE_THROW ( PortableGroup::ObjectNotCreated() );
  }
  auto_ptr<TAO::Fault_Detector_i> detector(pFD);

  ACE_NEW_NORETURN ( factory_creation_id,
    PortableGroup::GenericFactory::FactoryCreationId);
  if (factory_creation_id.ptr() == 0)
  {
    ACE_ERROR ((LM_ERROR,
      "New factory_creation_id returned NULL.  Throwing ObjectNotCreated.\n"
      ));

    ACE_THROW ( PortableGroup::ObjectNotCreated() );
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
    const FT::GenericFactory::FactoryCreationId & factory_creation_id
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , PortableGroup::ObjectNotFound
  ))
{
  METHOD_ENTRY(TAO::FT_FaultDetectorFactory_i::delete_object);

  InternalGuard guard (this->internals_);

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
      ACE_THROW(::PortableGroup::ObjectNotFound());
    }
  }
  else
  {
    ACE_THROW(::PortableGroup::ObjectNotFound());
  }
  METHOD_RETURN(TAO::FT_FaultDetectorFactory_i::delete_object);
}

CORBA::Boolean TAO::FT_FaultDetectorFactory_i::is_alive (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  METHOD_RETURN(TAO::FT_FaultDetectorFactory_i::is_alive)
    1;
}

///////////////////////////////////
// Template instantiation for
// competence-challenged compilers.

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
  template ACE_Vector<TAO::Fault_Detector_i *>;
  template ACE_Guard<ACE_SYNCH_MUTEX>;
  template auto_ptr<TAO::Fault_Detector_i>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
# pragma instantiate ACE_Vector<TAO::Fault_Detector_i *>
# pragma instantiate ACE_Guard<ACE_SYNCH_MUTEX>
# pragma instantiate auto_ptr<TAO::Fault_Detector_i>;
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

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
#include "ace/pre.h"
#include "FT_FaultDetectorFactory_i.h"
#include "Fault_Detector_i.h"
#include "ace/Get_Opt.h"
#include "orbsvcs/CosNamingC.h"
#include "tao/PortableServer/ORB_Manager.h"
#include "PG_Property_Set_Helper.h"

FT_FaultDetectorFactory_i::FT_FaultDetectorFactory_i ()
  : ior_output_file_(0)
  , nsName_(0)
  , quitOnIdle_(0)
{
}


FT_FaultDetectorFactory_i::~FT_FaultDetectorFactory_i ()
{
  //scope the guard
  {
    InternalGuard guard (internals_);

    // be sure all detectors are gone
    // before this object disappears
    shutdown_i ();
  }
  threadManager_.close ();
}

void FT_FaultDetectorFactory_i::shutdown_i()
{
  // assume mutex is locked
  for (size_t nDetector = 0; nDetector < detectors_.size(); ++nDetector)
  {
    Fault_Detector_i * detector = detectors_[nDetector];
    if (detector != 0)
    {
      detector->requestQuit();
    }
  }
}

int FT_FaultDetectorFactory_i::parse_args (int argc, char * argv[])
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
      case 'q':
      {
        quitOnIdle_ = 1;
        break;
      }

      case '?':
        // fall thru
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " -o <iorfile>"
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

const char * FT_FaultDetectorFactory_i::identity () const
{
  return identity_.c_str();
}


int FT_FaultDetectorFactory_i::self_register (TAO_ORB_Manager & orbManager)
{
  int result = 0;
  orb_ = orbManager.orb();

  // Register with the ORB.
  ior_ = orbManager.activate (this
      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

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
    nsName_ = "FT_FaultDetectorFactory";
  }

  if(nsName_ != 0)
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

    CosNaming::NamingContext_var naming_context =
      CosNaming::NamingContext::_narrow (naming_obj.in () ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    CosNaming::Name this_name (1);
    this_name.length (1);
    this_name[0].id = CORBA::string_dup (nsName_);

    naming_context->rebind (this_name, _this()
                            ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
  }

  return result;
}


void FT_FaultDetectorFactory_i::change_properties (
    const FT::Properties & property_set
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
    // TimeT is expressed in 10E-7 seconds (== 100 nSec == 0.1 uSec)
  static const long timeT_per_uSec = 10L;
  static const long uSec_per_sec = 1000000L;


  const PortableGroup::Properties * PGp = reinterpret_cast<const PortableGroup::Properties *>(&property_set);
  Portable_Group::Property_Set::Decoder decoder(*PGp);

  TimeBase::TimeT value = 0;
  if( Portable_Group::Property_Set::find (decoder, FT::FT_FAULT_MONITORING_INTERVAL, value) )
  {
    // note: these should be unsigned long, but
    // ACE_Time_Value wants longs.
    long uSec = static_cast<long>((value / timeT_per_uSec) % uSec_per_sec);
    long sec = static_cast<long>((value / timeT_per_uSec) / uSec_per_sec);
    ACE_Time_Value atv(sec, uSec);
    Fault_Detector_i::setTimeValue(atv);
  }
  else
  {
    ACE_THROW (::FT::InvalidProperty ());
  }
}

void FT_FaultDetectorFactory_i::shutdown (  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  delete this;
}

CORBA::Object_ptr FT_FaultDetectorFactory_i::create_object (
    const char * type_id,
    const FT::Criteria & the_criteria,
    FT::GenericFactory::FactoryCreationId_out factory_creation_id
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , FT::NoFactory
    , FT::ObjectNotCreated
    , FT::InvalidCriteria
    , FT::InvalidProperty
    , FT::CannotMeetCriteria
  ))
{
  InternalGuard guard (internals_);

  const PortableGroup::Properties * pgc
    = reinterpret_cast<const PortableGroup::Properties *> (&the_criteria);
  ::Portable_Group::Property_Set::Decoder decoder (*pgc);//the_criteria);

  // boolean, becomes true if a required parameter is missing
  int missingParameter = 0;


  CORBA::ULong detectorId = detectors_.size();

  FT::FaultNotifier_var notifier;
  if (! ::Portable_Group::Property_Set::find (
    decoder,
    ::FT::FT_NOTIFIER,
    notifier) )
  {
    missingParameter = 1;
  }

  FT::PullMonitorable_var monitorable;
  if (! ::Portable_Group::Property_Set::find (
    decoder,
    ::FT::FT_MONITORABLE,
    monitorable) )
  {
    missingParameter = 1;
  }

  FT::FTDomainId domain_id = 0;
  if (! ::Portable_Group::Property_Set::find (
    decoder,
    ::FT::FT_DOMAIN_ID,
    domain_id) )
  {
    missingParameter = 1;
  }

  FT::Location_var object_location;
  if (! ::Portable_Group::Property_Set::find (
    decoder,
    ::FT::FT_LOCATION,
    object_location) )
  {
    missingParameter = 1;
  }

  FT::TypeId object_type = 0;
  if (! ::Portable_Group::Property_Set::find (
    decoder,
    ::FT::FT_TYPE_ID,
    object_type) )
  {
    // Not required: missingParameter = 1;
  }

  FT::ObjectGroupId group_id = 0;
  if (! ::Portable_Group::Property_Set::find (
    decoder,
    ::FT::FT_GROUP_ID,
    group_id) )
  {
    // Not required: missingParameter = 1;
  }

  if (missingParameter)
  {
    ACE_THROW ( FT::InvalidCriteria() );
  }

  Fault_Detector_i * detector;
  ACE_NEW_NORETURN(detector, Fault_Detector_i(
    *this,
    detectorId,
    notifier,
    monitorable,
    domain_id,
    object_location,
    object_type,
    group_id));
  if (detector == 0)
  {
    ACE_THROW ( FT::ObjectNotCreated() );
  }

  detectors_.push_back(detector);

  (*factory_creation_id) <<= detectorId;

  // since FaultDetector is not a CORBA object (it does not implement
  // an interface.) we always return NIL;
  return CORBA::Object::_nil();
}

void FT_FaultDetectorFactory_i::delete_object (
    const FT::GenericFactory::FactoryCreationId & factory_creation_id
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , FT::ObjectNotFound
  ))
{
  InternalGuard guard (internals_);

  CORBA::ULong detectorId;
  factory_creation_id >>= detectorId;
  if (detectorId < detectors_.size())
  {
    if(detectors_[detectorId] != 0)
    {
      detectors_[detectorId]->requestQuit();
    }
    else
    {
      ACE_THROW(::FT::ObjectNotFound());
    }
  }
  else
  {
    ACE_THROW(::FT::ObjectNotFound());
  }
}

int FT_FaultDetectorFactory_i::write_IOR()
{
  int result = -1;
  FILE* out = ACE_OS::fopen (nsName_, "w");
  if (out)
  {
    ACE_OS::fprintf (out, "%s", static_cast<const char *>(ior_));
    ACE_OS::fclose (out);
    result = 0;
  }
  else
  {
    ACE_ERROR ((LM_ERROR,
      "Open failed for %s\n", nsName_
    ));
  }
  return result;
}


void FT_FaultDetectorFactory_i::removeDetector(CORBA::ULong id, Fault_Detector_i * detector)
{
  InternalGuard guard (internals_);
  if (id < detectors_.size())
  {
    if(detectors_[id] == detector)
    {
      delete detectors_[id];
      detectors_[id] = 0;
    }
  }
}

CORBA::Boolean FT_FaultDetectorFactory_i::is_alive ()
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 1;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
  template ACE_Vector<Fault_Detector_i *>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
# pragma instantiate ACE_Vector<Fault_Detector_i *>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */



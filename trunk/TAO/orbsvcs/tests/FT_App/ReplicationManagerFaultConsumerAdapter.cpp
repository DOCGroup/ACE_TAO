// -*- C++ -*-
//
// $Id$

#include "ReplicationManagerFaultConsumerAdapter.h"
#include "ace/Get_Opt.h"
#include "orbsvcs/PortableGroup/PG_Properties_Encoder.h"
#include "orbsvcs/FT_ReplicationManager/FT_DefaultFaultAnalyzer.h"
// FUZZ: disable check_for_streams_include
#include "ace/streams.h"
#include "ace/OS_NS_stdio.h"

ReplicationManagerFaultConsumerAdapter::ReplicationManagerFaultConsumerAdapter()
  : orb_(CORBA::ORB::_nil())
  , quit_(0)
  , readyFile_(0)
  , detector_ior_(0)
  , factory_(FT::FaultDetectorFactory::_nil())
  , notifier_ior_(0)
  , notifier_(FT::FaultNotifier::_nil())
  , p_fault_consumer_(0)
  , consumer_servant_(0)
{
}


ReplicationManagerFaultConsumerAdapter::~ReplicationManagerFaultConsumerAdapter()
{
}

size_t ReplicationManagerFaultConsumerAdapter::notifications () const
{
  // Delegate to the FT_FaultConsumer.
  return this->p_fault_consumer_->notifications ();
}


int ReplicationManagerFaultConsumerAdapter::parse_args (int argc, ACE_TCHAR * argv[])
{
  int optionError = 0;
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:r:d:n:"));
  int c;
  while ((c = get_opts ()) != -1)
  {
    switch (c)
    {
      case 'r':
      {
        this->replica_iors_.push_back (ACE_TEXT_ALWAYS_CHAR(get_opts.opt_arg ()));
        break;
      }
      case 'd':
      {
        this->detector_ior_ = get_opts.opt_arg ();
        break;
      }
      case 'n':
      {
        this->notifier_ior_ = get_opts.opt_arg ();
        break;
      }
      case 'o':
      {
        this->readyFile_ = get_opts.opt_arg ();
        break;
      }

      default:
        // fall thru
      case '?':
      {
        break;
      }
    }
  }

  if(! optionError)
  {
    if (0 == this->replica_iors_.size())
    {
      ACE_ERROR ((LM_ERROR,
        "-r option is required.\n"
        ));
      optionError = -1;
    }
    if (0 == this->detector_ior_)
    {
      ACE_ERROR ((LM_ERROR,
        "-d option is required.\n"
        ));
      optionError = -1;
    }
  }

  if(optionError)
  {
    ACE_ERROR ((LM_ERROR,
      "usage:  %s"
      " -r <replica.ior[ -r replica.ior]>"
      " -d <detector.ior>"
      " -o <this.ior>"
      " -n <nameService name>"
      "\n",
      argv [0]
      ));
  }
  return optionError;
}

/**
 * Register this object.
 */
int ReplicationManagerFaultConsumerAdapter::init (
  CORBA::ORB_ptr orb)
{
  ACE_DEBUG ((
    LM_DEBUG,
    ACE_TEXT ("Entered ReplicationManagerFaultConsumerAdapter::init.\n")
  ));

  int result = 0;
  this->orb_ = CORBA::ORB::_duplicate (orb);

  //////////////////////////////////////////
  // resolve reference to detector factory
  ACE_DEBUG ((
    LM_DEBUG,
    ACE_TEXT ("ReplicationManagerFaultConsumerAdapter::init: ")
    ACE_TEXT ("Getting ready to read iorDetectorFile.\n")
  ));

  CORBA::Object_var detector_obj = this->orb_->string_to_object (
    this->detector_ior_);
  this->factory_ = ::FT::FaultDetectorFactory::_narrow (
    detector_obj.in());
  if (CORBA::is_nil (this->factory_.in()))
  {
    ACE_ERROR_RETURN ((
      LM_ERROR,
      ACE_TEXT ("ReplicationManagerFaultConsumerAdapter::init: ")
      ACE_TEXT ("FaultDetectorFactory IOR is nil: %s\n"),
      this->detector_ior_),
      -1);
  }

  //////////////////////////////////////////
  // resolve references to notifier
  ACE_DEBUG ((
    LM_DEBUG,
    ACE_TEXT ("ReplicationManagerFaultConsumerAdapter::init: ")
    ACE_TEXT ("Getting ready to read Notifier IOR file.\n")
  ));

  CORBA::Object_var notifier_ior = this->orb_->string_to_object (
    this->notifier_ior_);
  this->notifier_ = ::FT::FaultNotifier::_narrow (
    notifier_ior.in());
  if (CORBA::is_nil (this->notifier_.in()))
  {
    ACE_ERROR_RETURN ((
      LM_ERROR,
      ACE_TEXT ("ReplicationManagerFaultConsumerAdapter::init: ")
      ACE_TEXT ("FaultNotifier IOR is nil: %s\n"),
      this->notifier_ior_),
      -1);
  }

  // Create the real FaultConsumer.
  //
  // Note: We have to hang onto the servant class pointer so we can
  // invoke member functions on it, but we also give ownership of it
  // to a PortableServer::ServantBase_var.
  ACE_DEBUG ((
    LM_DEBUG,
    ACE_TEXT ("ReplicationManagerFaultConsumerAdapter::init: ")
    ACE_TEXT ("Getting ready to create the real FaultConsumer.\n")
  ));

  ACE_NEW_RETURN (this->p_fault_consumer_, TAO::FT_FaultConsumer (), -1);
  if (this->p_fault_consumer_ != 0)
  {
    this->consumer_servant_ = this->p_fault_consumer_;
  }

  //////////////////////////
  // Get ready to initialize the consumer.  We need to provide it
  // with the following:
  // - The POA in which it is to be activated.
  // - FT::FaultNotifier IOR.
  // - FT::ReplicationManager IOR (fake it for now).

  // Get the RootPOA from the ORB.
  CORBA::Object_var poa_obj = this->orb_->resolve_initial_references (
    "RootPOA");
  PortableServer::POA_var poa = PortableServer::POA::_narrow (
    poa_obj.in());

  // Create a fault analyzer.
  TAO::FT_FaultAnalyzer * analyzer = 0;
  ACE_NEW_RETURN (analyzer, TAO::FT_DefaultFaultAnalyzer (), -1);

  // Initialize the FaultConsumer.
  // It will activate itself in the POA we pass it and connect to the
  // Fault Notifier we pass it.
  ACE_DEBUG ((
    LM_DEBUG,
    ACE_TEXT ("ReplicationManagerFaultConsumerAdapter::init: ")
    ACE_TEXT ("Getting ready to initialize the real FaultConsumer.\n")
  ));

  result = this->p_fault_consumer_->init (
    poa.in(),
    this->notifier_.in(),
    analyzer);
  if (result != 0)
  {
    ACE_ERROR_RETURN ((
      LM_ERROR,
      ACE_TEXT ("ReplicationManagerFaultConsumerAdapter::init: ")
      ACE_TEXT ("Unable to initialize the real FaultConsumer.\n")),
      result);
    }

  this->identity_ = "ReplicationManagerFaultConsumerAdapter";

  // Activate the RootPOA.
  PortableServer::POAManager_var poa_manager =
    poa->the_POAManager ();
  poa_manager->activate ();

  /////////////////////////
  // Set up fault detectors
  if (result == 0)
  {
    ////////////////////////////////////
    // resolve references to replicas
    // create a fault detector for each replica
    size_t replicaCount = this->replica_iors_.size();
    ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("Number of replicas being monitored: (%u)\n"),
      static_cast<unsigned int> (replicaCount)
    ));
    for (size_t nRep = 0; result == 0 && nRep < replicaCount; ++nRep)
    {
      const char * iorName = this->replica_iors_[nRep];
      CORBA::Object_var replica_obj = this->orb_->string_to_object (
        iorName);
      FT::PullMonitorable_var replica = FT::PullMonitorable::_narrow (
        replica_obj.in());
      if (CORBA::is_nil(replica.in()))
      {
        ACE_ERROR_RETURN ((
          LM_ERROR,
          ACE_TEXT ("ReplicationManagerFaultConsumerAdapter::init: ")
          ACE_TEXT ("Can't resolve Replica IOR: %s\n"),
          iorName),
          -1);
      }
      else
      {
        this->replicas_.push_back(replica);

        CORBA::String_var type_id = CORBA::string_dup("FaultDetector");

        TAO_PG::Properties_Encoder encoder;

        PortableGroup::Value value;
        value <<= notifier_.in ();
        encoder.add(::FT::FT_NOTIFIER, value);

        value <<= replica.in ();
        encoder.add(::FT::FT_MONITORABLE, value);

        FT::FTDomainId domain_id = 0;
        value <<= domain_id;
        encoder.add(::FT::FT_DOMAIN_ID, value);

        PortableGroup::Location object_location;
        object_location.length(2);
        object_location[0].id = CORBA::string_dup("test");
        object_location[1].id = CORBA::string_dup("Location_A");
        value <<= object_location;
        encoder.add(::FT::FT_LOCATION, value);

        PortableGroup::TypeId_var object_type = CORBA::string_dup (
          "IDL:org.omg/CosNaming/NamingContextExt:1.0");
        value <<= object_type.in ();
        encoder.add(::FT::FT_TYPE_ID, value);

        PortableGroup::ObjectGroupId group_id =
          static_cast<PortableGroup::ObjectGroupId> (6191982);
        value <<= group_id;
        encoder.add(::FT::FT_GROUP_ID, value);

        // allocate and populate the criteria
        PortableGroup::Criteria_var criteria;
        ACE_NEW_NORETURN (criteria,
          PortableGroup::Criteria);
        if (criteria.ptr() == 0)
        {
          ACE_ERROR_RETURN ((
            LM_ERROR,
            ACE_TEXT ("ReplicationManagerFaultConsumerAdapter::init: ")
            ACE_TEXT ("Error cannot allocate criteria.\n")),
            -1);
        }
        else
        {
          encoder.encode(criteria);
          PortableGroup::GenericFactory::FactoryCreationId_var factory_creation_id;

          this->factory_->create_object (
            type_id.in(),
            criteria.in(),
            factory_creation_id);
        }
      }
    }

    // Signal that we are ready to go.
    if (result == 0 && this->readyFile_ != 0)
    {
      FILE* ready = ACE_OS::fopen (this->readyFile_, "w");
      if (ready)
      {
        ACE_OS::fprintf (ready, "ready\n");
        ACE_OS::fclose (ready);
      }
    }
  }

  return result;
}

/**
 * Return a string to identify this object for logging/console message purposes.
 */
const char * ReplicationManagerFaultConsumerAdapter::identity () const
{
  return this->identity_.c_str();
}

/**
 * Clean house for process shut down.
 */
int ReplicationManagerFaultConsumerAdapter::fini (void)
{
  // Delegate to the FT_FaultConsumer.
  return this->p_fault_consumer_->fini ();
}


int ReplicationManagerFaultConsumerAdapter::idle(int & result)
{
  ACE_UNUSED_ARG(result);
  int quit = 0;

  if (this->replicas_.size() == this->p_fault_consumer_->notifications())
  {
    quit = 1;
  }
  return quit;
}

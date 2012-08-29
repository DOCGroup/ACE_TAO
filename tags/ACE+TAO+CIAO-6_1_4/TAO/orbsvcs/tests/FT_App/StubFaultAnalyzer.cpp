// -*- C++ -*-
//
// $Id$

#include "StubFaultAnalyzer.h"
#include "ace/Get_Opt.h"
#include "orbsvcs/PortableGroup/PG_Properties_Encoder.h"
// FUZZ: disable check_for_streams_include
#include "ace/streams.h"
#include "ace/OS_NS_stdio.h"

StubFaultAnalyzer::StubFaultAnalyzer ()
  : readyFile_(0)
  , detector_ior_(0)
  , notifier_ior_(0)
{
}


StubFaultAnalyzer::~StubFaultAnalyzer ()
{
}


int StubFaultAnalyzer::parse_args (int argc, ACE_TCHAR * argv[])
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
        this->replicaIORs.push_back (ACE_TEXT_ALWAYS_CHAR(get_opts.opt_arg ()));
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
    if (0 == replicaIORs.size())
    {
      ACE_ERROR ((LM_ERROR,
        "at least one -r option is required.\n"
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
    if (0 == this->notifier_ior_)
    {
      ACE_ERROR ((LM_ERROR,
        "-n option is required.\n"
        ));
      optionError = -1;
    }
  }

  if(optionError)
  {
    ACE_ERROR ((LM_ERROR,
      "usage:  %s"
      " -r <replica.ior[,replica.ior]>"
      " -d <detector.ior>"
      " -o <ready.file>"      // note: not an IOR file. just an "I'm alive" indicator."
      " -n <notifier.ior>"
      "\n",
      argv [0]
      ));
  }
  return optionError;
}

/**
 * Register this object as necessary
 */
int StubFaultAnalyzer::init (CORBA::ORB_ptr orb)
{
  int result = 0;
  this->orb_ = CORBA::ORB::_duplicate (orb);
  //////////////////////////////////////////
  // resolve reference to detector factory
  CORBA::Object_var detector_obj = this->orb_->string_to_object(this->detector_ior_);
  this->factory_ = ::FT::FaultDetectorFactory::_narrow(detector_obj.in ());
  if (CORBA::is_nil(this->factory_.in ()))
  {
    ACE_OS::fprintf (stderr, "Can't resolve Detector Factory IOR %s\n",
                     ACE_TEXT_ALWAYS_CHAR (this->detector_ior_));
    result = -1;
  }

  //////////////////////////////////////////
  // resolve references to notifier
  CORBA::Object_var not_obj = this->orb_->string_to_object(this->notifier_ior_);
  this->notifier_ = ::FT::FaultNotifier::_narrow(not_obj.in ());
  if (CORBA::is_nil(this->notifier_.in ()))
  {
    ACE_OS::fprintf (stderr, "Can't resolve Notifier IOR %s\n",
                     ACE_TEXT_ALWAYS_CHAR (this->notifier_ior_));
    result = -1;
  }


  //////////////////////////////////
  // register fault consumers
  if (result == 0)
  {
    result = this->faultConsumer_.init (orb, this->notifier_);
  }

  if (result == 0)
  {
    result = this->batchConsumer_.init (orb, this->notifier_);
  }

  /////////////////////////
  // Set up fault detectors
  if (result == 0)
  {
    ////////////////////////////////////
    // resolve references to replicas
    // create a fault detector for each replica
    size_t replicaCount = this->replicaIORs.size();
    for(size_t nRep = 0; result == 0 && nRep < replicaCount; ++nRep)
    {
      const char * iorName = this->replicaIORs[nRep];
      CORBA::Object_var rep_obj = this->orb_->string_to_object(iorName);
      FT::PullMonitorable_var replica = FT::PullMonitorable::_narrow(rep_obj.in ());
      if (CORBA::is_nil(replica.in ()))
      {
        ACE_OS::fprintf (stderr, "Can't resolve Replica IOR %s\n", iorName);
        result = -1;
      }
      else
      {
        this->replicas_.push_back(replica);

        CORBA::String_var type_id = CORBA::string_dup("FaultDetector");

        TAO_PG::Properties_Encoder encoder;

        PortableGroup::Value value;
        value <<= this->notifier_.in ();
        encoder.add(::FT::FT_NOTIFIER, value);

        value <<= replica.in ();
        encoder.add(::FT::FT_MONITORABLE, value);

        FT::FTDomainId domain_id = 0;
        value <<= domain_id;
        encoder.add(::FT::FT_DOMAIN_ID, value);

        PortableGroup::Location object_location;
        object_location.length(1);
        object_location[0].id = CORBA::string_dup("Test location");
        value <<= object_location;
        encoder.add(::FT::FT_LOCATION, value);

        PortableGroup::TypeId object_type = CORBA::string_dup("dummy_type");
        value <<= object_type;
        encoder.add(::FT::FT_TYPE_ID, value);

        FT::ObjectGroupId group_id = 0;
        value <<= group_id;
        encoder.add(::FT::FT_GROUP_ID, value);

        // allocate and populate the criteria
        PortableGroup::Criteria_var criteria;
        ACE_NEW_NORETURN (criteria,
          PortableGroup::Criteria);
        if (criteria.ptr() == 0)
        {
          ACE_ERROR((LM_ERROR,
            "Error cannot allocate criteria.\n"
            ));
            result = -1;
        }
        else
        {
          encoder.encode(criteria);
          PortableGroup::GenericFactory::FactoryCreationId_var factory_creation_id;

          ACE_DEBUG((LM_DEBUG,
            "Call create_object with type: %s\n", type_id.in()
            ));
          this->factory_->create_object (
            type_id.in(),
            criteria.in(),
            factory_creation_id);
        }
      }
    }

    if (result == 0 && this->readyFile_ != 0)
    {
      FILE *ready = ACE_OS::fopen (this->readyFile_, "w");
      if ( ready )
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
const char * StubFaultAnalyzer::identity () const
{
  return this->identity_.c_str();
}

/**
 * Clean house for process shut down.
 */
int StubFaultAnalyzer::fini (void)
{
  this->faultConsumer_.fini();
  this->batchConsumer_.fini();
  return 0;
}


int StubFaultAnalyzer::idle(int & result)
{
  ACE_UNUSED_ARG(result);
  int quit = 0;

  if (this->replicas_.size() == this->faultConsumer_.notifications())
  {
    quit = 1;
  }
  return quit;
}

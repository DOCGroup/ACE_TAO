// -*- C++ -*-
//
// $Id$

#include "StubFaultAnalyzer.h"
#include <ace/Get_Opt.h>
#include <tao/PortableServer/ORB_Manager.h>
#include <orbsvcs/PortableGroup/PG_Properties_Encoder.h>
#include <iostream>
#include <fstream>

StubFaultAnalyzer::StubFaultAnalyzer ()
  : readyFile_(0)
  , detector_ior_(0)
  , notifier_ior_(0)
{
}


StubFaultAnalyzer::~StubFaultAnalyzer ()
{
}


int StubFaultAnalyzer::parse_args (int argc, char * argv[])
{
  int optionError = 0;
  ACE_Get_Opt get_opts (argc, argv, "o:r:d:n:");
  int c;
  while ((c = get_opts ()) != -1)
  {
    switch (c)
    {
      case 'r':
      {
        this->replicaIORs.push_back (get_opts.opt_arg ());
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
int StubFaultAnalyzer::init (CORBA::ORB_var & orb ACE_ENV_ARG_DECL)
{
  int result = 0;
  this->orb_ = orb;
  //////////////////////////////////////////
  // resolve reference to detector factory
  CORBA::Object_var detector_obj = this->orb_->string_to_object(this->detector_ior_);
  this->factory_ = ::FT::FaultDetectorFactory::_narrow(detector_obj);
  if (CORBA::is_nil(this->factory_))
  {
    std::cerr << "Can't resolve Detector Factory IOR " << this->detector_ior_ << std::endl;
    result = -1;
  }

  //////////////////////////////////////////
  // resolve references to notifier
  CORBA::Object_var not_obj = this->orb_->string_to_object(this->notifier_ior_);
  this->notifier_ = ::FT::FaultNotifier::_narrow(not_obj);
  if (CORBA::is_nil(this->notifier_))
  {
    std::cerr << "Can't resolve Notifier IOR " << this->notifier_ior_ << std::endl;
    result = -1;
  }


  //////////////////////////////////
  // register fault consumers
  if (result == 0)
  {
    result = this->faultConsumer_.init(orb, this->notifier_);
  }

  if (result == 0)
  {
    result = this->batchConsumer_.init(orb, this->notifier_);
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
      FT::PullMonitorable_var replica = FT::PullMonitorable::_narrow(rep_obj);
      if (CORBA::is_nil(replica))
      {
        std::cerr << "Can't resolve Replica IOR " << iorName << std::endl;
        result = -1;
      }
      else
      {
        this->replicas_.push_back(replica);

        CORBA::String_var type_id = CORBA::string_dup("FaultDetector");

        TAO_PG::Properties_Encoder encoder;

        PortableGroup::Value value;
        value <<= this->notifier_;
        encoder.add(::FT::FT_NOTIFIER, value);

        value <<= replica;
        encoder.add(::FT::FT_MONITORABLE, value);

        FT::FTDomainId domain_id = 0;
        value <<= domain_id;
        encoder.add(::FT::FT_DOMAIN_ID, value);

        FT::Location object_location;
        object_location.length(1);
        object_location[0].id = CORBA::string_dup("Test location");
        value <<= object_location;
        encoder.add(::FT::FT_LOCATION, value);

        FT::TypeId object_type = 0;
        value <<= object_type;
        encoder.add(::FT::FT_TYPE_ID, value);

        FT::ObjectGroupId group_id = 0;
        value <<= group_id;
        encoder.add(::FT::FT_GROUP_ID, value);

        // allocate and populate the criteria
        FT::Criteria_var criteria;
        ACE_NEW_NORETURN (criteria,
          FT::Criteria);
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
          FT::GenericFactory::FactoryCreationId_var factory_creation_id;

          this->factory_->create_object (
            type_id.in(),
            criteria.in(),
            factory_creation_id
            ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      }
    }

    if (result == 0 && this->readyFile_ != 0)
    {
      std::ofstream ready(this->readyFile_, ios::out);
      ready << "ready" << std::endl;
      ready.close();
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
int StubFaultAnalyzer::fini (ACE_ENV_SINGLE_ARG_DECL)
{
  this->faultConsumer_.fini(ACE_ENV_SINGLE_ARG_PARAMETER);
  this->batchConsumer_.fini(ACE_ENV_SINGLE_ARG_PARAMETER);
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


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
  template instantiate ACE_Vector < const char * >;
  template ACE_Vector < FT::PullMonitorable_var > ;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
# pragma instantiate ACE_Vector < const char * >
# pragma instantiate ACE_Vector < FT::PullMonitorable_var >
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


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
  : replicaIorBuffer_(0)
  , readyFile_(0)
  , iorDetectorFile_(0)
  , iorNotifierFile_(0)

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
        if (replicaIorBuffer_ == 0)
        {
          const char * repNames = get_opts.opt_arg ();
          size_t repNameLen = ACE_OS::strlen(repNames);

          // make a working copy of the string
          ACE_NEW_NORETURN(replicaIorBuffer_,
            char[repNameLen + 1]);
          if ( replicaIorBuffer_ != 0)
          {
            ACE_OS::memcpy(replicaIorBuffer_, repNames, repNameLen+1);

            // tokenize the string on ','
            // into iorReplicaFiles_
            char * pos = replicaIorBuffer_;
            while (pos != 0)
            {
              iorReplicaFiles_.push_back(pos);
              // find a comma delimiter, and
              // chop the string there.
              pos = ACE_OS::strchr (pos, ',');
              if (pos != 0)
              {
                *pos = '\0';
                pos += 1;
              }
            }
          }
          else
          {
            ACE_ERROR ((LM_ERROR,
              "Command line option error: -r can't allocate buffer.\n"
              ));
            optionError = -1;
          }
        }
        else
        {
          ACE_ERROR ((LM_ERROR,
            "Command line option error: -r specified more than once.\n"
            ));
          optionError = -1;
        }
        break;
      }
      case 'd':
      {
        iorDetectorFile_ = get_opts.opt_arg ();
        break;
      }
      case 'n':
      {
        iorNotifierFile_ = get_opts.opt_arg ();
        break;
      }
      case 'o':
      {
        readyFile_ = get_opts.opt_arg ();
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
    if (0 == replicaIorBuffer_)
    {
      ACE_ERROR ((LM_ERROR,
        "-r option is required.\n"
        ));
      optionError = -1;
    }
    if (0 == iorDetectorFile_)
    {
      ACE_ERROR ((LM_ERROR,
        "-d option is required.\n"
        ));
      optionError = -1;
    }
    if (0 == iorNotifierFile_)
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
int StubFaultAnalyzer::init (TAO_ORB_Manager & orbManager  ACE_ENV_ARG_DECL)
{
  int result = 0;
  orb_ = orbManager.orb();

  //////////////////////////////////////////
  // resolve reference to detector factory
  CORBA::String_var factoryIOR;
  if (readIORFile(iorDetectorFile_, factoryIOR))
  {
    CORBA::Object_var obj = orb_->string_to_object(factoryIOR);
    factory_ = ::FT::FaultDetectorFactory::_narrow(obj);
    if (CORBA::is_nil(factory_))
    {
      std::cerr << "Can't resolve Detector Factory IOR " << iorDetectorFile_ << std::endl;
      result = -1;
    }
  }
  else
  {
    std::cerr << "Can't read " << iorDetectorFile_ << std::endl;
    result = -1;
  }

  //////////////////////////////////////////
  // resolve references to notifier
  CORBA::String_var notifierIOR;
  if (readIORFile(iorNotifierFile_, notifierIOR))
  {
    CORBA::Object_var obj = orb_->string_to_object(notifierIOR);
    notifier_ = ::FT::FaultNotifier::_narrow(obj);
    if (CORBA::is_nil(notifier_))
    {
      std::cerr << "Can't resolve Notifier IOR " << iorNotifierFile_ << std::endl;
      result = -1;
    }
  }
  else
  {
    std::cerr << "Can't read " << iorNotifierFile_ << std::endl;
    result = -1;
  }


  //////////////////////////////////
  // register fault consumers
  if (result == 0)
  {
    result = faultConsumer_.init(orbManager, notifier_);
  }

  if (result == 0)
  {
    result = batchConsumer_.init(orbManager, notifier_);
  }

  /////////////////////////
  // Set up fault detectors
  if (result == 0)
  {
    ////////////////////////////////////
    // resolve references to replicas
    // create a fault detector for each replica
    size_t replicaCount = iorReplicaFiles_.size();
    for(size_t nRep = 0; result == 0 && nRep < replicaCount; ++nRep)
    {
      const char * iorName = iorReplicaFiles_[nRep];
      CORBA::String_var ior;
      if (readIORFile(iorName, ior))
      {
        CORBA::Object_var obj = orb_->string_to_object(ior);
        FT::PullMonitorable_var replica = FT::PullMonitorable::_narrow(obj);
        if (CORBA::is_nil(replica))
        {
          std::cerr << "Can't resolve Replica IOR " << iorName << std::endl;
          result = -1;
        }
        else
        {
          replicas_.push_back(replica);

          CORBA::String_var type_id = CORBA::string_dup("FaultDetector");

          TAO_PG::Properties_Encoder encoder;

          PortableGroup::Value value;
          value <<= notifier_;
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

            factory_->create_object (
              type_id.in(),
              criteria.in(),
              factory_creation_id
              ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;
          }
        }
      }
      else
      {
        std::cerr << "Can't read " << iorName << std::endl;
        result = -1;
      }
    }

    if (result == 0 && readyFile_ != 0)
    {
      std::ofstream ready(readyFile_, ios::out);
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
  return identity_.c_str();
}

/**
 * Clean house for process shut down.
 */
int StubFaultAnalyzer::fini (ACE_ENV_SINGLE_ARG_DECL)
{
  faultConsumer_.fini(ACE_ENV_SINGLE_ARG_PARAMETER);
  batchConsumer_.fini(ACE_ENV_SINGLE_ARG_PARAMETER);
  return 0;
}


int StubFaultAnalyzer::idle(int & result)
{
  ACE_UNUSED_ARG(result);
  int quit = 0;

  if (replicas_.size() == faultConsumer_.notifications())
  {
    quit = 1;
  }
  return quit;
}


int StubFaultAnalyzer::readIORFile(const char * fileName, CORBA::String_var & ior)
{
  int result = 0;
  FILE *in = ACE_OS::fopen (fileName, "r");
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


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
  template instantiate ACE_Vector < const char * >;
  template ACE_Vector < FT::PullMonitorable_var > ;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
# pragma instantiate ACE_Vector < const char * >
# pragma instantiate ACE_Vector < FT::PullMonitorable_var >
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


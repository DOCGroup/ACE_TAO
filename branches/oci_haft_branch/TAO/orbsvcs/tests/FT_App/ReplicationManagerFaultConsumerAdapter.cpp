// -*- C++ -*-
//
// $Id$

#include "ReplicationManagerFaultConsumerAdapter.h"

#include <ace/Get_Opt.h>
#include <tao/PortableServer/ORB_Manager.h>
#include <orbsvcs/orbsvcs/PortableGroup/PG_Properties_Encoder.h>
#include <iostream>
#include <fstream>

ReplicationManagerFaultConsumerAdapter::ReplicationManagerFaultConsumerAdapter()
  : orb_(CORBA::ORB::_nil())
  , quit_(0)
  , readyFile_(0)
  , iorDetectorFile_(0)
  , factory_(FT::FaultDetectorFactory::_nil())
  , replicaIorBuffer_(0)
  , iorNotifierFile_(0)
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


int ReplicationManagerFaultConsumerAdapter::parse_args (int argc, char * argv[])
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
        if (this->replicaIorBuffer_ == 0)
        {
          const char * repNames = get_opts.opt_arg ();
          size_t repNameLen = ACE_OS::strlen(repNames);

          // make a working copy of the string
          ACE_NEW_NORETURN(this->replicaIorBuffer_,
            char[repNameLen + 1]);
          if ( this->replicaIorBuffer_ != 0)
          {
            ACE_OS::memcpy(this->replicaIorBuffer_, repNames, repNameLen+1);

            // tokenize the string on ','
            // into iorReplicaFiles_
            char * pos = this->replicaIorBuffer_;
            while (pos != 0)
            {
              this->iorReplicaFiles_.push_back(pos);
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
        this->iorDetectorFile_ = get_opts.opt_arg ();
        break;
      }
      case 'n':
      {
        this->iorNotifierFile_ = get_opts.opt_arg ();
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
    if (0 == this->replicaIorBuffer_)
    {
      ACE_ERROR ((LM_ERROR,
        "-r option is required.\n"
        ));
      optionError = -1;
    }
    if (0 == this->iorDetectorFile_)
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
      " -r <replica.ior[,replica.ior]>"
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
  CORBA::ORB_ptr orb
  ACE_ENV_ARG_DECL)
{
  ACE_DEBUG ((
    LM_DEBUG,
    ACE_TEXT ("Entered ReplicationManagerFaultConsumerAdapter::init.\n")
  ));

  int result = 0;
  this->orb_ = CORBA::ORB::_duplicate (orb);

  //////////////////////////////////////////
  // resolve reference to detector factory
  CORBA::String_var factoryIOR;
  ACE_DEBUG ((
    LM_DEBUG,
    ACE_TEXT ("ReplicationManagerFaultConsumerAdapter::init: ")
    ACE_TEXT ("Getting ready to read iorDetectorFile.\n")
  ));

  if (this->readIORFile(this->iorDetectorFile_, factoryIOR))
  {
    CORBA::Object_var obj = this->orb_->string_to_object (
      factoryIOR.in() ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);
    this->factory_ = ::FT::FaultDetectorFactory::_narrow (
      obj.in() ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);
    if (CORBA::is_nil (this->factory_.in()))
    {
      ACE_ERROR_RETURN ((
        LM_ERROR,
        ACE_TEXT ("ReplicationManagerFaultConsumerAdapter::init: ")
        ACE_TEXT ("FaultDetectorFactory IOR is nil: %s\n"),
        this->iorDetectorFile_),
        -1);
    }
  }
  else
  {
    ACE_ERROR_RETURN ((
      LM_ERROR,
      ACE_TEXT ("ReplicationManagerFaultConsumerAdapter::init: ")
      ACE_TEXT ("Can't read %s\n"),
      this->iorDetectorFile_),
      -1);
  }

  //////////////////////////////////////////
  // resolve references to notifier
  ACE_DEBUG ((
    LM_DEBUG,
    ACE_TEXT ("ReplicationManagerFaultConsumerAdapter::init: ")
    ACE_TEXT ("Getting ready to read Notifier IOR file.\n")
  ));

  CORBA::String_var notifierIOR;
  if (this->readIORFile(this->iorNotifierFile_, notifierIOR))
  {
    CORBA::Object_var obj = this->orb_->string_to_object (
      notifierIOR.in() ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);
    this->notifier_ = ::FT::FaultNotifier::_narrow (
      obj.in() ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);
    if (CORBA::is_nil (this->notifier_.in()))
    {
      ACE_ERROR_RETURN ((
        LM_ERROR,
        ACE_TEXT ("ReplicationManagerFaultConsumerAdapter::init: ")
        ACE_TEXT ("FaultNotifier IOR is nil: %s\n"),
        this->iorNotifierFile_),
        -1);
    }
  }
  else
  {
    ACE_ERROR_RETURN ((
      LM_ERROR,
      ACE_TEXT ("ReplicationManagerFaultConsumerAdapter::init: ")
      ACE_TEXT ("Can't read %s\n"),
      this->iorNotifierFile_),
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
    "RootPOA" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  PortableServer::POA_var poa = PortableServer::POA::_narrow (
    poa_obj.in() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Fake out the ReplicationManager IOR.
  CORBA::Object_var obj = this->orb_->string_to_object (
    "corbaloc::localhost:1900/ReplicationManager"
    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  FT::ReplicationManager_var repl_mgr =
    FT::ReplicationManager::_unchecked_narrow (
      obj.in() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  if (CORBA::is_nil (repl_mgr.in())) {
    ACE_ERROR_RETURN ((
      LM_ERROR,
      ACE_TEXT ("ReplicationManagerFaultConsumerAdapter::init: ")
      ACE_TEXT ("Unable to narrow the fake ReplicationManager IOR.\n")),
      -1);
  }

  // Initialize the FaultConsumer.
  // It will activate itself in the POA we pass it and connect to the
  // Fault Notifier we pass it.  It uses the Replication Manager we
  // pass it for dealing with fault reports.
  ACE_DEBUG ((
    LM_DEBUG,
    ACE_TEXT ("ReplicationManagerFaultConsumerAdapter::init: ")
    ACE_TEXT ("Getting ready to initialize the real FaultConsumer.\n")
  ));

  result = this->p_fault_consumer_->init (
    poa.in(),
    this->notifier_.in(),
    repl_mgr.in()
    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
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
    poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  /////////////////////////
  // Set up fault detectors
  if (result == 0)
  {
    ////////////////////////////////////
    // resolve references to replicas
    // create a fault detector for each replica
    size_t replicaCount = this->iorReplicaFiles_.size();
    ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("Number of replicas being monitored: (%u)\n"),
      ACE_static_cast (unsigned int, replicaCount)
    ));
    for (size_t nRep = 0; result == 0 && nRep < replicaCount; ++nRep)
    {
      const char * iorName = this->iorReplicaFiles_[nRep];
      CORBA::String_var ior;
      if (this->readIORFile(iorName, ior))
      {
        CORBA::Object_var obj = this->orb_->string_to_object (
          ior.in() ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (-1);
        FT::PullMonitorable_var replica = FT::PullMonitorable::_narrow (
          obj.in() ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (-1);
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
          value <<= notifier_;
          encoder.add(::FT::FT_NOTIFIER, value);

          value <<= replica;
          encoder.add(::FT::FT_MONITORABLE, value);

          FT::FTDomainId domain_id = 0;
          value <<= domain_id;
          encoder.add(::FT::FT_DOMAIN_ID, value);

          FT::Location object_location;
          object_location.length(2);
          object_location[0].id = CORBA::string_dup("test");
          object_location[1].id = CORBA::string_dup("Location_A");
          value <<= object_location;
          encoder.add(::FT::FT_LOCATION, value);

          FT::TypeId_var object_type = CORBA::string_dup (
            "IDL:org.omg/CosNaming/NamingContextExt:1.0");
          value <<= object_type;
          encoder.add(::FT::FT_TYPE_ID, value);

          FT::ObjectGroupId group_id =
            ACE_static_cast (FT::ObjectGroupId, 6191982);
          value <<= group_id;
          encoder.add(::FT::FT_GROUP_ID, value);

          // allocate and populate the criteria
          FT::Criteria_var criteria;
          ACE_NEW_NORETURN (criteria,
            FT::Criteria);
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
            FT::GenericFactory::FactoryCreationId_var factory_creation_id;

            this->factory_->create_object (
              type_id.in(),
              criteria.in(),
              factory_creation_id
              ACE_ENV_ARG_PARAMETER);
            ACE_CHECK_RETURN (-1);
          }
        }
      }
      else
      {
        ACE_ERROR_RETURN ((
          LM_ERROR,
          ACE_TEXT ("ReplicationManagerFaultConsumerAdapter::init: ")
          ACE_TEXT ("Can't read %s\n"),
          iorName),
          -1);
      }
    }

    // Signal that we are ready to go.
    if (result == 0 && this->readyFile_ != 0)
    {
      std::ofstream ready (this->readyFile_, ios::out);
      ready << "ready" << std::endl;
      ready.close();
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
int ReplicationManagerFaultConsumerAdapter::fini (ACE_ENV_SINGLE_ARG_DECL)
{
  // Delegate to the FT_FaultConsumer.
  return this->p_fault_consumer_->fini (ACE_ENV_SINGLE_ARG_PARAMETER);
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

int ReplicationManagerFaultConsumerAdapter::readIORFile(
  const char * fileName,
  CORBA::String_var & ior)
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


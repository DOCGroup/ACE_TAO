// -*- C++ -*-
//
// $Id$

#include "ace/pre.h"
#include "StubFaultNotifier.h"
#include "ace/Get_Opt.h"
#include "tao/PortableServer/ORB_Manager.h"
#include "orbsvcs/orbsvcs/PortableGroup/PG_Properties_Encoder.h"

StubFaultNotifier::StubFaultNotifier ()
  : iorOutputFile_(0)
  , iorDetectorFile_(0)
  , replicaIorBuffer_(0)
  , nsName_(0)
{
}


StubFaultNotifier::~StubFaultNotifier ()
{
  delete [] replicaIorBuffer_;
  replicaIorBuffer_ = 0;
}


int StubFaultNotifier::parse_args (int argc, char * argv[])
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
        nsName_ = get_opts.opt_arg ();
        break;
      }
      case 'o':
      {
        iorOutputFile_ = get_opts.opt_arg ();
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
 * Publish this objects IOR.
 */
int StubFaultNotifier::self_register (TAO_ORB_Manager & orbManager  ACE_ENV_ARG_DECL)
{
  int result = 0;
  orb_ = orbManager.orb();

  // Register with the ORB.
  ior_ = orbManager.activate (this
      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  //////////////////////////////////////////
  // resolve references to detector factory
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
  if (result == 0)
  {
    ////////////////////////////////////
    // resolve references to replicas
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
          value <<= _this();
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

    if (iorOutputFile_ != 0)
    {
      identity_ = "file:";
      identity_ += iorOutputFile_;
      result = writeIORFile();
    }
    else
    {
      // if no IOR file specified,
      // then always try to register with name service
      nsName_ = "FT_FaultNotifier";
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
  }
  return result;
}

int StubFaultNotifier::readIORFile(const char * fileName, CORBA::String_var & ior)
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

int StubFaultNotifier::writeIORFile()
{
  int result = -1;
  FILE* out = ACE_OS::fopen (iorOutputFile_, "w");
  if (out)
  {
    ACE_OS::fprintf (out, "%s", static_cast<const char *>(ior_));
    ACE_OS::fclose (out);
    result = 0;
  }
  return result;
}

/**
 * Return a string to identify this object for logging/console message purposes.
 */
const char * StubFaultNotifier::identity () const
{
  return identity_.c_str();
}

/**
 * Clean house for process shut down.
 */
void StubFaultNotifier::shutdown_i ()
{
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}

void StubFaultNotifier::push_structured_fault (
    const CosNotification::StructuredEvent & event
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_ERROR ((LM_ERROR,
    "FaultNotifier: Received Fault notification:\n"
    "FaultNotifier:   Header EventType domain: %s\n"
    "FaultNotifier:   Header EventType type: %s\n"
    "FaultNotifier:   Header EventName: %s\n",
    ACE_static_cast (const char *, event.header.fixed_header.event_type.domain_name),
    ACE_static_cast (const char *, event.header.fixed_header.event_type.type_name),
    ACE_static_cast (const char *, event.header.fixed_header.event_name)
    ));
  const CosNotification::FilterableEventBody & filterable = event.filterable_data;

  size_t propertyCount = filterable.length ();
  for (size_t nProp = 0; nProp < propertyCount; ++nProp)
  {
    const CosNotification::Property & property = filterable[nProp];
    ACE_ERROR ((LM_ERROR,
      "FaultNotifier:   Property Name: %s\n",
      ACE_static_cast (const char *, property.name)
      ));
    int todo_finish_decode;
  }
}


void StubFaultNotifier::push_sequence_fault (
    const CosNotification::EventBatch & events
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT());
}

::CosNotifyFilter::Filter_ptr StubFaultNotifier::create_subscription_filter (
    const char * constraint_grammar
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , CosNotifyFilter::InvalidGrammar
  ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT());
}


FT::FaultNotifier::ConsumerId StubFaultNotifier::connect_structured_fault_consumer (
    CosNotifyComm::StructuredPushConsumer_ptr push_consumer,
    CosNotifyFilter::Filter_ptr filter
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT());
}


FT::FaultNotifier::ConsumerId StubFaultNotifier::connect_sequence_fault_consumer (
    CosNotifyComm::SequencePushConsumer_ptr push_consumer,
    CosNotifyFilter::Filter_ptr filter
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT());
}

void StubFaultNotifier::disconnect_consumer (
    FT::FaultNotifier::ConsumerId connection
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , CosEventComm::Disconnected
  ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT());
}

CORBA::Boolean StubFaultNotifier::is_alive ()
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 1;
}

int StubFaultNotifier::idle(int & result)
{
  ACE_UNUSED_ARG(result);
  int quit = 0;
  ACE_TRY_NEW_ENV
  {
    if(factory_.ptr() != 0 && !CORBA::is_nil(factory_))
    {
      if (!factory_->is_alive( ACE_ENV_SINGLE_ARG_PARAMETER))
      {
        quit = 1;
      }
    }
  }
  ACE_CATCHANY
  {
    quit = 1;
  }
  ACE_ENDTRY;
  return quit;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
  template instantiate ACE_Vector < const char * >;
  template ACE_Vector < FT::PullMonitorable_var > ;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
# pragma instantiate ACE_Vector < const char * >
# pragma instantiate ACE_Vector < FT::PullMonitorable_var >
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#include "ace/post.h"

// -*- C++ -*-
//
// $Id$

#include "StubFaultNotifier.h"
#include <ace/Get_Opt.h>
#include <tao/PortableServer/ORB_Manager.h>
#include <orbsvcs/orbsvcs/PortableGroup/PG_Properties_Encoder.h>
#include <iostream>

StubFaultNotifier::StubFaultNotifier ()
  : ior_output_file_(0)
  , detector_ior_(0)
  , ns_name_(0)
{
}


StubFaultNotifier::~StubFaultNotifier ()
{
}


::PortableServer::POA_ptr StubFaultNotifier::_default_POA (ACE_ENV_SINGLE_ARG_DECL)
{
  return ::PortableServer::POA::_duplicate(this->poa_ ACE_ENV_ARG_PARAMETER);
}

PortableServer::ObjectId StubFaultNotifier::objectId()const
{
  return this->object_id_.in();
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
        this->iorReplicaFiles_.push_back( get_opts.opt_arg ());
        break;
      }
      case 'd':
      {
        this->detector_ior_ = get_opts.opt_arg ();
        break;
      }
      case 'n':
      {
        this->ns_name_ = get_opts.opt_arg ();
        break;
      }
      case 'o':
      {
        this->ior_output_file_ = get_opts.opt_arg ();
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
    if (iorReplicaFiles_.size() == 0)
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
      " -r <replica.ior [-r <replica2.ior]...>"
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
 * Prepare to exit.
 */
int StubFaultNotifier::fini ()
{
  if(this->ns_name_ != 0)
  {
    ACE_TRY_NEW_ENV
    {
      CORBA::Object_var naming_obj =
        this->orb_->resolve_initial_references ("NameService" ACE_ENV_ARG_PARAMETER);
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
      this_name[0].id = CORBA::string_dup (this->ns_name_);

      naming_context->unbind (this_name
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
    ACE_CATCHANY
    {
    }
    ACE_ENDTRY;
  }
  return 0;
}


/**
 * Publish this objects IOR.
 */
int StubFaultNotifier::init (CORBA::ORB_var & orb ACE_ENV_ARG_DECL)
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
  ACE_CHECK_RETURN (-1);

  poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Register with the POA.

  this->object_id_ = this->poa_->activate_object (this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // find my identity as a corba object
  CORBA::Object_var this_obj =
    this->poa_->id_to_reference (object_id_.in ()
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  //////////////////////////////////////////
  // resolve references to detector factory

  CORBA::Object_var obj = this->orb_->string_to_object(detector_ior_);
  this->factory_ = ::FT::FaultDetectorFactory::_narrow(obj);
  if (CORBA::is_nil(this->factory_))
  {
    std::cerr << "Can't resolve Detector Factory IOR " << this->detector_ior_ << std::endl;
    result = -1;
  }
  if (result == 0)
  {
    ////////////////////////////////////
    // resolve references to replicas
    size_t replicaCount = this->iorReplicaFiles_.size();
    for(size_t nRep = 0; result == 0 && nRep < replicaCount; ++nRep)
    {
      const char * iorName = this->iorReplicaFiles_[nRep];
      CORBA::Object_var obj = this->orb_->string_to_object(iorName);
      FT::PullMonitorable_var replica = FT::PullMonitorable::_narrow(obj);
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
        //////////////////
        // FaultDetectorFactory gets picky about FaultNotifier's object type.
        // coddle it.
        ::FT::FaultNotifier_var notifier = ::FT::FaultNotifier::_narrow(this_obj);
        value <<= notifier;
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
          ACE_CHECK_RETURN (-1);
        }
      }
    }

    if (this->ior_output_file_ != 0)
    {
      this->identity_ = "file:";
      this->identity_ += this->ior_output_file_;
      result = write_ior_file();
    }
    else
    {
      // if no IOR file specified,
      // then always try to register with name service
      this->ns_name_ = "FT_FaultNotifier";
    }

    if(this->ns_name_ != 0)
    {
      this->identity_ = "name:";
      this->identity_ += this->ns_name_;

      CORBA::Object_var naming_obj =
        this->orb_->resolve_initial_references ("NameService" ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      if (CORBA::is_nil(naming_obj.in ())){
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%T %n (%P|%t) Unable to find the Naming Service\n"),
                          1);
      }

      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in () ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      CosNaming::Name this_name (1);
      this_name.length (1);
      this_name[0].id = CORBA::string_dup (this->ns_name_);

      naming_context->rebind (this_name, this_obj.in()
                              ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
    }
  }
  return result;
}

int StubFaultNotifier::write_ior_file()
{
  int result = -1;
  FILE* out = ACE_OS::fopen (this->ior_output_file_, "w");
  if (out)
  {
    ACE_OS::fprintf (out, "%s", ACE_static_cast(const char *, this->ior_));
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
  return this->identity_.c_str();
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
    ACE_ENV_ARG_DECL
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
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT());
}

::CosNotifyFilter::Filter_ptr StubFaultNotifier::create_subscription_filter (
    const char * constraint_grammar
    ACE_ENV_ARG_DECL
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
    ACE_ENV_ARG_DECL
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
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT());
}

void StubFaultNotifier::disconnect_consumer (
    FT::FaultNotifier::ConsumerId connection
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , CosEventComm::Disconnected
  ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT());
}

CORBA::Boolean StubFaultNotifier::is_alive (ACE_ENV_SINGLE_ARG_DECL)
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
    if(this->factory_.ptr() != 0 && !CORBA::is_nil(this->factory_))
    {
      if (!this->factory_->is_alive( ACE_ENV_SINGLE_ARG_PARAMETER))
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

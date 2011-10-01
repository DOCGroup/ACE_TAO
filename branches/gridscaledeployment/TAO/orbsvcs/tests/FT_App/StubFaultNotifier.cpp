// -*- C++ -*-
//
// $Id$

#include "StubFaultNotifier.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "orbsvcs/PortableGroup/PG_Properties_Encoder.h"
// FUZZ: disable check_for_streams_include
#include "ace/streams.h"

StubFaultNotifier::StubFaultNotifier ()
  : ior_output_file_(0)
  , detector_ior_(0)
  , ns_name_("")
{
}


StubFaultNotifier::~StubFaultNotifier ()
{
}


::PortableServer::POA_ptr StubFaultNotifier::_default_POA (void)
{
  return ::PortableServer::POA::_duplicate(this->poa_.in ());
}

PortableServer::ObjectId StubFaultNotifier::objectId()const
{
  return this->object_id_.in();
}

int StubFaultNotifier::parse_args (int argc, ACE_TCHAR * argv[])
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
        this->iorReplicaFiles_.push_back( ACE_TEXT_ALWAYS_CHAR(get_opts.opt_arg ()));
        break;
      }
      case 'd':
      {
        this->detector_ior_ = get_opts.opt_arg ();
        break;
      }
      case 'n':
      {
        this->ns_name_ = ACE_TEXT_ALWAYS_CHAR(get_opts.opt_arg ());
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
int StubFaultNotifier::fini (void)
{
  if(this->ns_name_.length () != 0)
  {
    CORBA::Object_var naming_obj =
      this->orb_->resolve_initial_references ("NameService");

    if (CORBA::is_nil(naming_obj.in ())){
      ACE_ERROR_RETURN ((LM_ERROR,
                       "%T %n (%P|%t) Unable to find the Naming Service\n"),
                       1);
    }

    CosNaming::NamingContext_var naming_context =
      CosNaming::NamingContext::_narrow (naming_obj.in ());

    CosNaming::Name this_name (1);
    this_name.length (1);
    this_name[0].id = CORBA::string_dup (this->ns_name_.c_str ());

    naming_context->unbind (this_name);
  }
  return 0;
}


/**
 * Publish this objects IOR.
 */
int StubFaultNotifier::init (CORBA::ORB_ptr orb)
{
  int result = 0;
  this->orb_ = CORBA::ORB::_duplicate (orb);

  // Use the ROOT POA for now
  CORBA::Object_var poa_object =
    this->orb_->resolve_initial_references (TAO_OBJID_ROOTPOA);

  if (CORBA::is_nil (poa_object.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT (" (%P|%t) Unable to initialize the POA.\n")),
                      -1);

  // Get the POA object.
  this->poa_ =
    PortableServer::POA::_narrow (poa_object.in ());


  if (CORBA::is_nil(this->poa_.in ()))
  {
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT (" (%P|%t) Unable to narrow the POA.\n")),
                      -1);
  }

  PortableServer::POAManager_var poa_manager =
    this->poa_->the_POAManager ();

  poa_manager->activate ();

  // Register with the POA.

  this->object_id_ = this->poa_->activate_object (this);

  // find my identity as a corba object
  CORBA::Object_var this_obj =
    this->poa_->id_to_reference (object_id_.in ());

  //////////////////////////////////////////
  // resolve references to detector factory

  CORBA::Object_var obj = this->orb_->string_to_object(detector_ior_);
  this->factory_ = ::FT::FaultDetectorFactory::_narrow(obj.in ());
  if (CORBA::is_nil(this->factory_.in ()))
  {
    ACE_OS::fprintf (stderr, "Can't resolve Detector Factory IOR %s\n",
                     ACE_TEXT_ALWAYS_CHAR (this->detector_ior_));
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
      FT::PullMonitorable_var replica = FT::PullMonitorable::_narrow(obj.in ());
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
        //////////////////
        // FaultDetectorFactory gets picky about FaultNotifier's object type.
        // coddle it.
        ::FT::FaultNotifier_var notifier = ::FT::FaultNotifier::_narrow(this_obj.in ());
        value <<= notifier.in ();
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

        PortableGroup::TypeId object_type = 0;
        value <<= object_type;
        encoder.add(::FT::FT_TYPE_ID, value);

        PortableGroup::ObjectGroupId group_id = 0;
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

          this->factory_->create_object (
            type_id.in(),
            criteria.in(),
            factory_creation_id);
        }
      }
    }

    if (this->ior_output_file_ != 0)
    {
      this->identity_ = ACE_TEXT("file:");
      this->identity_ += this->ior_output_file_;
      result = write_ior_file();
    }
    else
    {
      // if no IOR file specified,
      // then always try to register with name service
      this->ns_name_ = "FT_FaultNotifier";
    }

    if(this->ns_name_.length () != 0)
    {
      this->identity_ = ACE_TEXT("name:");
      this->identity_ += ACE_TEXT_CHAR_TO_TCHAR(this->ns_name_.c_str ());

      CORBA::Object_var naming_obj =
        this->orb_->resolve_initial_references ("NameService");

      if (CORBA::is_nil(naming_obj.in ())){
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%T %n (%P|%t) Unable to find the Naming Service\n"),
                          1);
      }

      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in ());

      CosNaming::Name this_name (1);
      this_name.length (1);
      this_name[0].id = CORBA::string_dup (this->ns_name_.c_str ());

      naming_context->rebind (this_name, this_obj.in());
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
    ACE_OS::fprintf (out, "%s", this->ior_.in ());
    ACE_OS::fclose (out);
    result = 0;
  }
  return result;
}

/**
 * Return a string to identify this object for logging/console message purposes.
 */
const ACE_TCHAR * StubFaultNotifier::identity () const
{
  return this->identity_.c_str();
}

/**
 * Clean house for process shut down.
 */
void StubFaultNotifier::shutdown_i (void)
{
  this->orb_->shutdown (0);
}

void StubFaultNotifier::push_structured_fault (
    const CosNotification::StructuredEvent & event
  )
{
  ACE_ERROR ((LM_ERROR,
    "FaultNotifier: Received Fault notification:\n"
    "FaultNotifier:   Header EventType domain: %s\n"
    "FaultNotifier:   Header EventType type: %s\n"
    "FaultNotifier:   Header EventName: %s\n",
    static_cast<const char *> (event.header.fixed_header.event_type.domain_name),
    static_cast<const char *> (event.header.fixed_header.event_type.type_name),
    static_cast<const char *> (event.header.fixed_header.event_name)
    ));
  const CosNotification::FilterableEventBody & filterable = event.filterable_data;

  size_t propertyCount = filterable.length ();
  for (size_t nProp = 0; nProp < propertyCount; ++nProp)
  {
    const CosNotification::Property & property = filterable[nProp];
    ACE_ERROR ((LM_ERROR,
      "FaultNotifier:   Property Name: %s\n",
      static_cast<const char *> (property.name)
      ));
    //@@ we could stand to decode more--just for completeness
  }
}


void StubFaultNotifier::push_sequence_fault (
    const CosNotification::EventBatch & events
  )
{
  ACE_UNUSED_ARG (events);
  throw CORBA::NO_IMPLEMENT();
}

::CosNotifyFilter::Filter_ptr StubFaultNotifier::create_subscription_filter (
    const char * constraint_grammar
  )
{
  ACE_UNUSED_ARG (constraint_grammar);
  throw CORBA::NO_IMPLEMENT();
  return 0;
}


FT::FaultNotifier::ConsumerId StubFaultNotifier::connect_structured_fault_consumer (
    CosNotifyComm::StructuredPushConsumer_ptr push_consumer,
    CosNotifyFilter::Filter_ptr filter
  )
{
  ACE_UNUSED_ARG(push_consumer);
  ACE_UNUSED_ARG(filter);

  throw CORBA::NO_IMPLEMENT();
  return 0;
}


FT::FaultNotifier::ConsumerId StubFaultNotifier::connect_sequence_fault_consumer (
    CosNotifyComm::SequencePushConsumer_ptr push_consumer,
    CosNotifyFilter::Filter_ptr filter
  )
{
  ACE_UNUSED_ARG(push_consumer);
  ACE_UNUSED_ARG(filter);

  throw CORBA::NO_IMPLEMENT();
  return 0;
}

void StubFaultNotifier::disconnect_consumer (
    FT::FaultNotifier::ConsumerId connection
  )
{
  ACE_UNUSED_ARG(connection);

  throw CORBA::NO_IMPLEMENT();
}

CORBA::Boolean StubFaultNotifier::is_alive (void)
{
  return 1;
}

int StubFaultNotifier::idle(int & result)
{
  ACE_UNUSED_ARG(result);
  int quit = 0;
  try
  {
    if(!CORBA::is_nil(this->factory_.in ()))
    {
      int ok = this->factory_->is_alive();
      if (!ok)
      {
        quit = 1;
      }
    }
  }
  catch (const CORBA::Exception&)
  {
    quit = 1;
  }
  return quit;
}

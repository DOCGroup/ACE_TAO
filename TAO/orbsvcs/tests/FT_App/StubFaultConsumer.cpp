// -*- C++ -*-
//
// $Id$

#include "StubFaultConsumer.h"
#include "ace/Get_Opt.h"
#include "orbsvcs/PortableGroup/PG_Properties_Encoder.h"

StubFaultConsumer::StubFaultConsumer ()
  : quit_(0)
  , notifications_(0)
{
}


StubFaultConsumer::~StubFaultConsumer ()
{
}

::PortableServer::POA_ptr StubFaultConsumer::_default_POA (void)
{
  return ::PortableServer::POA::_duplicate(this->poa_.in ());
}

PortableServer::ObjectId StubFaultConsumer::objectId()const
{
  return this->object_id_.in();
}

size_t StubFaultConsumer::notifications () const
{
  return this->notifications_;
}


int StubFaultConsumer::parse_args (int argc, char * argv[])
{
  int optionError = 0;
#ifndef NO_ARGS_FOR_NOW
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);
#else // NO_ARGS_FOR_NOW
  ACE_Get_Opt get_opts (argc, argv, "");
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
        this->nsName_ = get_opts.opt_arg ();
        break;
      }
      case 'o':
      {
        this->iorOutputFile_ = get_opts.opt_arg ();
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
#endif  /* NO_ARGS_FOR_NOW */
  return optionError;
}

/**
 * Register this object.
 */
int StubFaultConsumer::init (CORBA::ORB_ptr orb,
    ::FT::FaultNotifier_var & notifier)
{
  int result = 0;
  this->orb_ = CORBA::ORB::_duplicate (orb);
  this->notifier_ = notifier;
  this->identity_ = "StubFaultConsumer";


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

  // find my identity as an object

  CORBA::Object_var this_obj =
    this->poa_->id_to_reference (object_id_.in ());

  CosNotifyFilter::Filter_var filter = CosNotifyFilter::Filter::_nil();

  this->consumer_id_ = notifier->connect_structured_fault_consumer(
    CosNotifyComm::StructuredPushConsumer::_narrow(this_obj.in ()),
    filter.in ());

  return result;
}

/**
 * Return a string to identify this object for logging/console message purposes.
 */
const char * StubFaultConsumer::identity () const
{
  return this->identity_.c_str();
}

/**
 * Clean house for process shut down.
 */
int StubFaultConsumer::fini (void)
{
  this->notifier_->disconnect_consumer(this->consumer_id_);
  return 0;
}


int StubFaultConsumer::idle(int & result)
{
  ACE_UNUSED_ARG(result);
  return this->quit_;
}

////////////////
// CORBA methods
void StubFaultConsumer::push_structured_event(
  const CosNotification::StructuredEvent &notification
  )
{
  ////////////////////////////////////////
  // keep track of how many we've received
  this->notifications_ += 1;
  ACE_ERROR ((LM_ERROR,
    "FaultConsumer: Received Fault notification(%d):\n"
    "FaultConsumer:   Header EventType domain: %s\n"
    "FaultConsumer:   Header EventType type: %s\n"
    "FaultConsumer:   Header EventName: %s\n",
    static_cast<unsigned int> (this->notifications_),
    static_cast<const char *> (notification.header.fixed_header.event_type.domain_name),
    static_cast<const char *> (notification.header.fixed_header.event_type.type_name),
    static_cast<const char *> (notification.header.fixed_header.event_name)
    ));
  const CosNotification::FilterableEventBody & filterable = notification.filterable_data;

  size_t propertyCount = filterable.length ();
  for (size_t nProp = 0; nProp < propertyCount; ++nProp)
  {
    const CosNotification::Property & property = filterable[nProp];

    const char * property_name = static_cast<const char *> (property.name);
    const char * value = "<unknown>";
    if (property.value >>= value )
    {
      // ok!
    }
    ACE_ERROR ((LM_ERROR,
      "FaultConsumer:   Property Name: %s=%s\n",
      property_name,
      value
      ));


//    int todo_finish_decode;
  }

}

void StubFaultConsumer::offer_change (
    const CosNotification::EventTypeSeq & added,
    const CosNotification::EventTypeSeq & removed
  )
{
  ACE_UNUSED_ARG (added);
  ACE_UNUSED_ARG (removed);
  ACE_ERROR ((LM_ERROR,
    "StubFaultConsumer: offer_change call ignored.\n"
  ));
}

void StubFaultConsumer::disconnect_structured_push_consumer(void)
{
  ACE_ERROR ((LM_ERROR,
    "StubFaultConsumer:disconnect_structured_push_consumer interpreted as quit request.\n"
  ));
  this->quit_ = 1;
}

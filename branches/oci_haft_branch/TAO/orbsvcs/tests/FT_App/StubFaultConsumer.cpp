// -*- C++ -*-
//
// $Id$

#include "StubFaultConsumer.h"
#include <ace/Get_Opt.h>
#include <tao/PortableServer/ORB_Manager.h>
#include <orbsvcs/orbsvcs/PortableGroup/PG_Properties_Encoder.h>

StubFaultConsumer::StubFaultConsumer ()
  : quit_(0)
  , notifications_(0)
{
}


StubFaultConsumer::~StubFaultConsumer ()
{
}

size_t StubFaultConsumer::notifications () const
{
  return notifications_;
}


int StubFaultConsumer::parse_args (int argc, char * argv[])
{
  int optionError = 0;
#ifdef NO_ARGS_FOR_NOW
  ACE_Get_Opt get_opts (argc, argv, "");
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
#endif
  return optionError;
}

/**
 * Register this object.
 */
int StubFaultConsumer::init (TAO_ORB_Manager & orbManager,
    ::FT::FaultNotifier_var & notifier
    ACE_ENV_ARG_DECL)
{
  int result = 0;
  orb_ = orbManager.orb();

  notifier_ = notifier;

  // Register with the ORB.
  ior_ = orbManager.activate (this
      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  identity_ = "StubFaultConsumer";

  CosNotifyFilter::Filter_var filter = CosNotifyFilter::Filter::_nil();

  consumerId_ = notifier->connect_structured_fault_consumer(
    _this(),
    filter);

  return result;
}

/**
 * Return a string to identify this object for logging/console message purposes.
 */
const char * StubFaultConsumer::identity () const
{
  return identity_.c_str();
}

/**
 * Clean house for process shut down.
 */
int StubFaultConsumer::fini (ACE_ENV_SINGLE_ARG_DECL)
{
  notifier_->disconnect_consumer(consumerId_ ACE_ENV_ARG_PARAMETER);
  return 0;
}


int StubFaultConsumer::idle(int & result)
{
  ACE_UNUSED_ARG(result);
  return quit_;
}

////////////////
// CORBA methods
void StubFaultConsumer::push_structured_event(
  const CosNotification::StructuredEvent &notification
  ACE_ENV_ARG_DECL
  )
  throw (CORBA::SystemException, CosEventComm::Disconnected)
{
  ////////////////////////////////////////
  // keep track of how many we've received
  notifications_ += 1;
  ACE_ERROR ((LM_ERROR,
    "FaultConsumer: Received Fault notification(%d):\n"
    "FaultConsumer:   Header EventType domain: %s\n"
    "FaultConsumer:   Header EventType type: %s\n"
    "FaultConsumer:   Header EventName: %s\n",
    ACE_static_cast (unsigned int, notifications_),
    ACE_static_cast (const char *, notification.header.fixed_header.event_type.domain_name),
    ACE_static_cast (const char *, notification.header.fixed_header.event_type.type_name),
    ACE_static_cast (const char *, notification.header.fixed_header.event_name)
    ));
  const CosNotification::FilterableEventBody & filterable = notification.filterable_data;

  size_t propertyCount = filterable.length ();
  for (size_t nProp = 0; nProp < propertyCount; ++nProp)
  {
    const CosNotification::Property & property = filterable[nProp];
    ACE_ERROR ((LM_ERROR,
      "FaultConsumer:   Property Name: %s\n",
      ACE_static_cast (const char *, property.name)
      ));
//    int todo_finish_decode;
  }

}

void StubFaultConsumer::offer_change (
    const CosNotification::EventTypeSeq & added,
    const CosNotification::EventTypeSeq & removed
    ACE_ENV_ARG_DECL
  )
  throw (CORBA::SystemException, CosNotifyComm::InvalidEventType)
{
  ACE_ERROR ((LM_ERROR,
    "StubFaultConsumer: offer_change call ignored.\n"
  ));
}

void StubFaultConsumer::disconnect_structured_push_consumer(ACE_ENV_SINGLE_ARG_DECL)
  throw (CORBA::SystemException)
{
  ACE_ERROR ((LM_ERROR,
    "StubFaultConsumer:disconnect_structured_push_consumer interpreted as quit request.\n"
  ));
  quit_ = 1;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
//  template instantiate ACE_Vector < const char * >;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
//# pragma instantiate ACE_Vector < const char * >
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


// -*- C++ -*-
//=============================================================================
/**
 *  @file    FT_FaultNotifier_i.cpp
 *
 *  $Id$
 *
 *  This file is part of Fault Tolerant CORBA.
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================

#include "FT_Notifier_i.h"

#include "ace/Get_Opt.h"
#include "tao/PortableServer/ORB_Manager.h"
// Use this macro at the beginning of CORBA methods
// to aid in debugging.
#define METHOD_ENTRY(name)    \
    ACE_DEBUG (( LM_DEBUG,    \
    "Enter %s\n", #name       \
      ))

// Use this macro to return from CORBA methods
// to aid in debugging.  Note that you can specify
// the return value after the macro, for example:
// METHOD_RETURN(Plugh::plover) xyzzy; is equivalent
// to return xyzzy;
// METHOD_RETURN(Plugh::troll); is equivalent to
// return;
// WARNING: THIS GENERATES TWO STATEMENTS!!! THE FOLLOWING
// will not do what you want it to:
//  if (cave_is_closing) METHOD_RETURN(Plugh::pirate) aarrggh;
// Moral:  Always use braces.
#define METHOD_RETURN(name)   \
    ACE_DEBUG (( LM_DEBUG,    \
      "Leave %s\n", #name     \
      ));                     \
    return /* value goes here */


// Implementation skeleton constructor
FT_FaultNotifier_i::FT_FaultNotifier_i ()
  : ior_output_file_(0)
  , nsName_(0)
{
}

// Implementation skeleton destructor
FT_FaultNotifier_i::~FT_FaultNotifier_i ()
{
  shutdown_i ();
  self_unregister ();
}


////////////////////////////////////////////
// FT_FaultNotifier_i private methods

void FT_FaultNotifier_i::shutdown_i()
{
}

// TODO: find this a common home
int FT_FaultNotifier_i::write_IOR()
{
  int result = -1;
  FILE* out = ACE_OS::fopen (ior_output_file_, "w");
  if (out)
  {
    ACE_OS::fprintf (out, "%s", static_cast<const char *>(ior_));
    ACE_OS::fclose (out);
    result = 0;
  }
  else
  {
    ACE_ERROR ((LM_ERROR,
      "Open failed for %s\n", ior_output_file_
    ));
  }
  return result;
}

//////////////////////////////////////////////////////
// FT_FaultNotifier_i public, non-CORBA methods

int FT_FaultNotifier_i::parse_args (int argc, char * argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:");
  int c;

  while ((c = get_opts ()) != -1)
  {
    switch (c)
    {
      case 'o':
        ior_output_file_ = get_opts.opt_arg ();
        break;

      case '?':
        // fall thru
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " -o <iorfile>"
                           " -q{uit on idle}"
                           "\n",
                           argv [0]),
                          -1);
      break;
    }
  }
  // Indicates sucessful parsing of the command line
  return 0;
}

const char * FT_FaultNotifier_i::identity () const
{
  return identity_.c_str();
}

int FT_FaultNotifier_i::self_unregister (ACE_ENV_SINGLE_ARG_DECL)
{
  if (ior_output_file_ != 0)
  {
    ACE_OS::unlink (ior_output_file_);
    ior_output_file_ = 0;
  }
  if (nsName_ != 0)
  {
    naming_context_->unbind (this_name_
                            ACE_ENV_ARG_PARAMETER);
    nsName_ = 0;
  }
  return 0;
}

int FT_FaultNotifier_i::self_register (TAO_ORB_Manager & orbManager
  ACE_ENV_ARG_DECL)
{
  int result = 0;
  orb_ = orbManager.orb();

  // Register with the ORB.
  ior_ = orbManager.activate (this
      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (ior_output_file_ != 0)
  {
    identity_ = "file:";
    identity_ += ior_output_file_;
    result = write_IOR();
  }
  else
  {
    // if no IOR file specified,
    // then always try to register with name service
    nsName_ = "FT_FaultDetectorFactory";
  }

  if (nsName_ != 0)
  {
    identity_ = "name:";
    identity_ += nsName_;

    CORBA::Object_var naming_obj =
      orb_->resolve_initial_references ("NameService" ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);

    if (CORBA::is_nil(naming_obj.in ())){
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%T %n (%P|%t) Unable to find the Naming Service\n"),
                        1);
    }

    naming_context_ =
      CosNaming::NamingContext::_narrow (naming_obj.in () ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);
    int todo_check_narrow;

    this_name_.length (1);
    this_name_[0].id = CORBA::string_dup (nsName_);

    naming_context_->rebind (this_name_, _this()
                            ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);

    ////////////////////////////////////////////////
    // Register with coresident Notification Channel

    PortableServer::POA_var poa = orbManager.root_poa();

    CosNotifyChannelAdmin::EventChannelFactory_var notify_factory =
      TAO_Notify_EventChannelFactory_i::create (poa.in ()
      ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);
    CosNotification::QoSProperties initial_qos;
    CosNotification::AdminProperties initial_admin;

    notify_channel_ =
      notify_factory->create_channel (initial_qos,
      initial_admin,
      channelId_
      ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);

    ///////////////////////////
    // Producer registration

    supplierAdmin_ = notify_channel_->default_supplier_admin (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);

    ::CosNotifyChannelAdmin::ProxyID proxyId = 0;

    //////////////////////
    // structured producer
    ::CosNotifyChannelAdmin::ProxyConsumer_var consumer
      = supplierAdmin_->obtain_notification_push_consumer (
        ::CosNotifyChannelAdmin::STRUCTURED_EVENT,
        proxyId
        ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);

    structuredProxyPushConsumer_
      = ::CosNotifyChannelAdmin::StructuredProxyPushConsumer::_narrow(consumer
        ACE_ENV_ARG_PARAMETER);
    int todo_check_narrow1;

    // todo: implement this if we want to receive disconnect notice
    // todo:  if not, is this call necessary?
    CosNotifyComm::StructuredPushSupplier_var stubPushSupplier =
      CosNotifyComm::StructuredPushSupplier::_nil();

    structuredProxyPushConsumer_->connect_structured_push_supplier (stubPushSupplier
      ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);

    ////////////////////
    // Sequence producer
    consumer
      = supplierAdmin_->obtain_notification_push_consumer (
        ::CosNotifyChannelAdmin::SEQUENCE_EVENT,
        proxyId
        ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);

    ::CosNotifyChannelAdmin::SequenceProxyPushConsumer_var sequenceProxyPushConsumer_
      = ::CosNotifyChannelAdmin::SequenceProxyPushConsumer::_narrow(consumer
        ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);
    int todo_check_narrow3;

    // todo: implement this if we want to receive disconnect notice
    // todo:  if not, is this call necessary?
    CosNotifyComm::SequencePushSupplier_var stubSeqPushSupplier =
      CosNotifyComm::SequencePushSupplier::_nil();

    sequenceProxyPushConsumer_->connect_sequence_push_supplier (stubSeqPushSupplier
      ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);

    ///////////////////////////
    // Consumer registration

    consumerAdmin_ = notify_channel_->default_consumer_admin (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);

    // everything else happens when subscriber shows up

  }

  return result;
}

void FT_FaultNotifier_i::push_structured_fault (
    const CosNotification::StructuredEvent & event
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  structuredProxyPushConsumer_->push_structured_event (event
    ACE_ENV_ARG_PARAMETER);
// int todo;
}

void FT_FaultNotifier_i::push_sequence_fault (
    const CosNotification::EventBatch & events
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  sequenceProxyPushConsumer_->push_structured_events (events
    ACE_ENV_ARG_PARAMETER);
  //int todo;
}

::CosNotifyFilter::Filter_ptr FT_FaultNotifier_i::create_subscription_filter (
    const char * constraint_grammar
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , CosNotifyFilter::InvalidGrammar
  ))
{
  int todo;
  return ::CosNotifyFilter::Filter::_nil();
}

FT::FaultNotifier::ConsumerId FT_FaultNotifier_i::connect_structured_fault_consumer (
    CosNotifyComm::StructuredPushConsumer_ptr push_consumer,
    CosNotifyFilter::Filter_ptr filter
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  ::CosNotifyChannelAdmin::ProxyID proxyId = 0;

  //////////////////////
  // get a proxy supplier
  ::CosNotifyChannelAdmin::ProxySupplier_var supplier
    = consumerAdmin_->obtain_notification_push_supplier (
      ::CosNotifyChannelAdmin::STRUCTURED_EVENT,
      proxyId
      ACE_ENV_ARG_PARAMETER);

  ::CosNotifyChannelAdmin::StructuredProxyPushSupplier_var proxySupplier
    = ::CosNotifyChannelAdmin::StructuredProxyPushSupplier::_narrow(supplier
      ACE_ENV_ARG_PARAMETER);

  proxySupplier->connect_structured_push_consumer ( push_consumer
    ACE_ENV_ARG_PARAMETER);

  int todo; // something about that d**n filter
  return proxyId;
}

FT::FaultNotifier::ConsumerId FT_FaultNotifier_i::connect_sequence_fault_consumer (
    CosNotifyComm::SequencePushConsumer_ptr push_consumer,
    CosNotifyFilter::Filter_ptr filter
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  ::CosNotifyChannelAdmin::ProxyID proxyId = 0;

  //////////////////////
  // get a proxy supplier
  ::CosNotifyChannelAdmin::ProxySupplier_var supplier
    = consumerAdmin_->obtain_notification_push_supplier (
      ::CosNotifyChannelAdmin::SEQUENCE_EVENT,
      proxyId
      ACE_ENV_ARG_PARAMETER);

  ::CosNotifyChannelAdmin::SequenceProxyPushSupplier_var proxySupplier
    = ::CosNotifyChannelAdmin::SequenceProxyPushSupplier::_narrow(supplier
      ACE_ENV_ARG_PARAMETER);

  proxySupplier->connect_sequence_push_consumer ( push_consumer
    ACE_ENV_ARG_PARAMETER);

  int todo; // something about that d**n filter
  return proxyId;
}

void FT_FaultNotifier_i::disconnect_consumer (
    FT::FaultNotifier::ConsumerId connection
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , CosEventComm::Disconnected
  ))
{
  int todo;
}

CORBA::Boolean FT_FaultNotifier_i::is_alive ()
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  return 1;
}

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
      ));

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
  , consumerConnects_(0)
  , consumerDisconnects_(0)
  , quitOnIdle_(0)
{
}

// Implementation skeleton destructor
FT_FaultNotifier_i::~FT_FaultNotifier_i ()
{
  self_unregister (ACE_ENV_SINGLE_ARG_PARAMETER);
}


int FT_FaultNotifier_i::idle(int &result ACE_ENV_ARG_DECL)
{
  ACE_UNUSED_ARG(result);
  int quit = 0;
  if (quitOnIdle_)
  {
    if ( consumerConnects_ != 0 
      && consumerConnects_ == consumerDisconnects_)
    {
      quit = 1;
      ACE_ERROR((LM_ERROR,
        "FaultNotifier quit on idle: connects %d, disconnects %d\n",
        ACE_static_cast (unsigned int, consumerConnects_),
        ACE_static_cast (unsigned int, consumerDisconnects_)
        ));
    }
  }
  return quit;
}

////////////////////////////////////////////
// FT_FaultNotifier_i private methods


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
      "%T %n (%P|%t) Open failed for %s\n", ior_output_file_
    ));
  }
  return result;
}

//////////////////////////////////////////////////////
// FT_FaultNotifier_i public, non-CORBA methods

int FT_FaultNotifier_i::parse_args (int argc, char * argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:vq");
  int c;

  while ((c = get_opts ()) != -1)
  {
    switch (c)
    {
      case 'o':
      {
        ior_output_file_ = get_opts.opt_arg ();
        break;
      }
      case 'q':
      {
        quitOnIdle_ = 1;
        break;
      }
      case '?':
        // fall thru
      default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " -o <iorfile>"
                           "\n",
                           argv [0]),
                          -1);
        break;
      }
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
  if (nsName_ != 0 && naming_context_.in() != 0)
  {
    naming_context_->unbind (this_name_
                            ACE_ENV_ARG_PARAMETER);
    nsName_ = 0;
  }

  int TODO_unregister_with_ReplicationManager;

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


  ////////////////////////////////////////////////
  // Register with coresident Notification Channel

  result = orbManager.activate_poa_manager(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  if (result == 0)
  {
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

    filter_factory_ = notify_channel_->default_filter_factory (ACE_ENV_SINGLE_ARG_PARAMETER);
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
    ACE_CHECK_RETURN (-1);
    if (CORBA::is_nil (structuredProxyPushConsumer_))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
         "%T %n (%P|%t) Should not occur: Unable to narrow Structured Proxy Push Consumer\n"),
        1);
    }

    // todo: implement a push supplier if we want to receive disconnect notice
    // todo:  if not, is this call necessary?
    CosNotifyComm::StructuredPushSupplier_var stubPushSupplier =
      CosNotifyComm::StructuredPushSupplier::_nil();

    structuredProxyPushConsumer_->connect_structured_push_supplier (stubPushSupplier.in()
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
    if (CORBA::is_nil (sequenceProxyPushConsumer_))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
         "%T %n (%P|%t) Should not occur: Unable to narrow Sequence Proxy Push Consumer\n"),
        1);
    }

    // todo: implement this if we want to receive disconnect notice
    // todo:  if not, is this call necessary?
    CosNotifyComm::SequencePushSupplier_var stubSeqPushSupplier =
      CosNotifyComm::SequencePushSupplier::_nil();

    sequenceProxyPushConsumer_->connect_sequence_push_supplier (stubSeqPushSupplier.in()
      ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);

    ///////////////////////////
    // Consumer registration

    // find the channel administrator for consumers
    consumerAdmin_ = notify_channel_->default_consumer_admin (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);
    if (CORBA::is_nil (consumerAdmin_))
    {
      ACE_ERROR ((LM_ERROR,
        "%T %n (%P|%t) NIL consumer admin\n"
        ));
      result = -1;
    }
    // everything else happens when subscriber shows up
  }

  ///////////////////////////////
  // Set up and ready for action
  // publish our IOR

  if(result == 0)
  {
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
      nsName_ = "FT_FaultNotifier";
    }
  }

  if (result == 0)
  {
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
      if (CORBA::is_nil(naming_context_))
      {
        ACE_ERROR_RETURN ((LM_ERROR,
           "%T %n (%P|%t) Should not occur: Can't narrow initial reference to naming context.\n"),
          1);
      }
      this_name_.length (1);
      this_name_[0].id = CORBA::string_dup (nsName_);

      naming_context_->rebind (this_name_, _this()
                              ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
    }
  }

  int TODO_register_with_ReplicationManager;

  return result;
}

///////////////////
// CORBA METHODS

void FT_FaultNotifier_i::push_structured_fault (
    const CosNotification::StructuredEvent & event
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  METHOD_ENTRY(FT_FaultNotifier_i::push_structured_fault);

  structuredProxyPushConsumer_->push_structured_event (event
    ACE_ENV_ARG_PARAMETER);

  METHOD_RETURN(FT_FaultNotifier_i::push_structured_fault);
}

void FT_FaultNotifier_i::push_sequence_fault (
    const CosNotification::EventBatch & events
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  METHOD_ENTRY(FT_FaultNotifier_i::push_sequence_fault);

  sequenceProxyPushConsumer_->push_structured_events (events
    ACE_ENV_ARG_PARAMETER);

  METHOD_RETURN(FT_FaultNotifier_i::push_sequence_fault);
}

::CosNotifyFilter::Filter_ptr FT_FaultNotifier_i::create_subscription_filter (
    const char * constraint_grammar
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException, CosNotifyFilter::InvalidGrammar))
{
  METHOD_ENTRY(FT_FaultNotifier_i::create_subscription_filter);

  CosNotifyFilter::Filter_var filter = filter_factory_->create_filter ("ETCL");
  METHOD_RETURN(FT_FaultNotifier_i::create_subscription_filter)
    filter._retn ();
}

FT::FaultNotifier::ConsumerId FT_FaultNotifier_i::connect_structured_fault_consumer (
    CosNotifyComm::StructuredPushConsumer_ptr push_consumer,
    CosNotifyFilter::Filter_ptr filter
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  METHOD_ENTRY(FT_FaultNotifier_i::connect_structured_fault_consumer);

  /////////////////////////
  // find a ProxyInfo entry
  // use the first nil entry or a new entry if no nils found

  size_t infoPos = 0;
  int looking = 1;
  for ( size_t pos = 0; looking && pos < proxyInfos_.size (); ++pos)
  {
    ProxyInfo & pi = proxyInfos_[pos];
    if (CORBA::is_nil(pi.proxyVar_))
    {
      infoPos = pos;
      looking = 0;
    }
  }
  if (looking)
  {
    infoPos = proxyInfos_.size();
    proxyInfos_.push_back(ProxyInfo());
  }

  ///////////////////////////////////////
  // Assign an ID, populate the ProxyInfo
  FT::FaultNotifier::ConsumerId result = infoPos;
  ProxyInfo & info = proxyInfos_[infoPos];
  info.proxyVar_
      = consumerAdmin_->obtain_notification_push_supplier (
      ::CosNotifyChannelAdmin::STRUCTURED_EVENT,
      info.proxyId_
      ACE_ENV_ARG_PARAMETER);

  consumerConnects_ += 1;

  ::CosNotifyChannelAdmin::StructuredProxyPushSupplier_var proxySupplier
    = ::CosNotifyChannelAdmin::StructuredProxyPushSupplier::_narrow(info.proxyVar_
        ACE_ENV_ARG_PARAMETER);

  if ( CORBA::is_nil (proxySupplier))
  {
    // this is a shoould-not-occur situation.  The consumer admin returned
    // the wrong kind of object.
    ACE_ERROR(( LM_ERROR,
       "%T %n (%P|%t) Unexpected result: Wrong type of object returned from obtain_notification_push_supplier\n"
       ));
  }
  else
  {
    proxySupplier->connect_structured_push_consumer ( push_consumer
      ACE_ENV_ARG_PARAMETER);

    if (! CORBA::is_nil (filter))
    {
      proxySupplier->add_filter(filter);
    }
  }

  METHOD_RETURN(FT_FaultNotifier_i::connect_structured_fault_consumer) result;
}

FT::FaultNotifier::ConsumerId FT_FaultNotifier_i::connect_sequence_fault_consumer (
    CosNotifyComm::SequencePushConsumer_ptr push_consumer,
    CosNotifyFilter::Filter_ptr filter
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  METHOD_ENTRY(FT_FaultNotifier_i::connect_sequence_fault_consumer);
  /////////////////////////
  // find a ProxyInfo entry
  // use the first nil entry or a new entry if no nils found

  size_t infoPos = 0;
  int looking = 1;
  for ( size_t pos = 0; looking && pos < proxyInfos_.size (); ++pos)
  {
    ProxyInfo & pi = proxyInfos_[pos];
    if (CORBA::is_nil(pi.proxyVar_))
    {
      infoPos = pos;
      looking = 0;
    }
  }
  if (looking)
  {
    infoPos = proxyInfos_.size();
    proxyInfos_.push_back(ProxyInfo());
  }

  ///////////////////////////////////////
  // Assign an ID, populate the ProxyInfo
  FT::FaultNotifier::ConsumerId result = infoPos;
  ProxyInfo & info = proxyInfos_[infoPos];
  info.proxyVar_
    = consumerAdmin_->obtain_notification_push_supplier (
      ::CosNotifyChannelAdmin::SEQUENCE_EVENT,
      info.proxyId_
      ACE_ENV_ARG_PARAMETER);

  consumerConnects_ += 1;

  ::CosNotifyChannelAdmin::SequenceProxyPushSupplier_var proxySupplier
    = ::CosNotifyChannelAdmin::SequenceProxyPushSupplier::_narrow(info.proxyVar_
      ACE_ENV_ARG_PARAMETER);
  if ( CORBA::is_nil (proxySupplier))
  {
    // this is a shoould-not-occur situation.  The consumer admin returned
    // the wrong kind of object.
    ACE_ERROR(( LM_ERROR,
       "%T %n (%P|%t) Unexpected result: Wrong type of object returned from obtain_notification_push_supplier\n"
       ));
  }
  else
  {
    proxySupplier->connect_sequence_push_consumer ( push_consumer
      ACE_ENV_ARG_PARAMETER);

    if (! CORBA::is_nil (filter))
    {
      proxySupplier->add_filter(filter);
    }
  }
  METHOD_RETURN(FT_FaultNotifier_i::connect_sequence_fault_consumer) result;
}

void FT_FaultNotifier_i::disconnect_consumer (
    FT::FaultNotifier::ConsumerId connection
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException, CosEventComm::Disconnected))
{
  METHOD_ENTRY(FT_FaultNotifier_i::disconnect_consumer);

  size_t index = ACE_static_cast ( size_t, connection);
  if (index < proxyInfos_.size())
  {
    ProxyInfo & info = proxyInfos_[index];
    if (CORBA::is_nil(info.proxyVar_) )
    {
      ACE_THROW(CosEventComm::Disconnected());
    }
    else
    {
      ::CosNotifyChannelAdmin::StructuredProxyPushSupplier_var proxySupplier
        = ::CosNotifyChannelAdmin::StructuredProxyPushSupplier::_narrow(info.proxyVar_
            ACE_ENV_ARG_PARAMETER);
      if (! CORBA::is_nil (proxySupplier))
      {
        proxySupplier->disconnect_structured_push_supplier ();
        info.proxyVar_ = ::CosNotifyChannelAdmin::ProxySupplier::_nil();
      }
      else
      {
        ::CosNotifyChannelAdmin::SequenceProxyPushSupplier_var proxySupplier
          = ::CosNotifyChannelAdmin::SequenceProxyPushSupplier::_narrow(info.proxyVar_
              ACE_ENV_ARG_PARAMETER);
        if (! CORBA::is_nil (proxySupplier))
        {
          proxySupplier->disconnect_sequence_push_supplier ();
          info.proxyVar_ = ::CosNotifyChannelAdmin::ProxySupplier::_nil();
        }
        else
        {
          ACE_ERROR((LM_ERROR,
            "Unexpected proxy supplier type\n"
            ));
          ACE_THROW(CosEventComm::Disconnected());
        }
      }
    }
  }
  else
  {
    ACE_THROW(CosEventComm::Disconnected());
  }

  consumerDisconnects_ += 1;

  METHOD_RETURN(FT_FaultNotifier_i::disconnect_consumer);
}

CORBA::Boolean FT_FaultNotifier_i::is_alive (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  METHOD_RETURN(FT_FaultNotifier_i::is_alive) 1;
}

//////////////
// ProxyInfo

FT_FaultNotifier_i::ProxyInfo::ProxyInfo ()
  : proxyId_ (0)
  , proxyVar_ (::CosNotifyChannelAdmin::ProxySupplier::_nil())
{
}

FT_FaultNotifier_i::ProxyInfo::ProxyInfo (const ProxyInfo & rhs)
  : proxyId_ (rhs.proxyId_)
  , proxyVar_ (rhs.proxyVar_)
{
}

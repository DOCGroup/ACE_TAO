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
TAO::FT_FaultNotifier_i::FT_FaultNotifier_i ()
  : ior_output_file_(0)
  , ns_name_(0)
  , consumer_connects_(0)
  , consumer_disconnects_(0)
  , quit_on_idle_(0)
  , quitting_(0)
  , gone_(0)
  , rm_register_(1)
  , registered_(0)
  , replication_manager_(0)
{
}

// Implementation skeleton destructor
TAO::FT_FaultNotifier_i::~FT_FaultNotifier_i ()
{
  fini (ACE_ENV_SINGLE_ARG_PARAMETER);
}


int TAO::FT_FaultNotifier_i::idle(int &result ACE_ENV_ARG_DECL)
{
  static unsigned long linger = 0;
  ACE_UNUSED_ARG(result);
  if (gone_)
  {
    if ( linger == 0)
    {
      ACE_ERROR ((LM_ERROR,
        "FaultNotifier (%P|%t) Begin linger.\n"
      ));
    }
    if(++linger > 5)//10)
    {
      ACE_ERROR ((LM_ERROR,
        "FaultNotifier (%P|%t) idle returnning gone\n"
      ));
    }
    else
    {
      return 0;
    }
  }
  return this->gone_;
}

////////////////////////////////////////////
// FT_FaultNotifier_i private methods


// TODO: find this a common home
int TAO::FT_FaultNotifier_i::write_ior()
{
  int result = -1;
  FILE* out = ACE_OS::fopen (this->ior_output_file_, "w");
  if (out)
  {
    ACE_OS::fprintf (out, "%s", ACE_static_cast(const char *, this->ior_));
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

int TAO::FT_FaultNotifier_i::parse_args (int argc, char * argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:rq");
  int c;

  while ((c = get_opts ()) != -1)
  {
    switch (c)
    {
      case 'o':
      {
        this->ior_output_file_ = get_opts.opt_arg ();
        break;
      }
      case 'r':
      {
        this->rm_register_ = ! this->rm_register_;
        break;
      }
      case 'q':
      {
        this->quit_on_idle_ = 1;
        break;
      }
      case '?':
        // fall thru
      default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " -o <iorfile>"
                           " -r disable registration with ReplicationManager"
                           " -q(uit on idle)"
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

const char * TAO::FT_FaultNotifier_i::identity () const
{
  return this->identity_.c_str();
}

PortableServer::POA_ptr TAO::FT_FaultNotifier_i::_default_POA (ACE_ENV_SINGLE_ARG_DECL)
{
  return poa_.in();
}


void TAO::FT_FaultNotifier_i::_remove_ref (ACE_ENV_SINGLE_ARG_DECL)
{
  notify_channel_->destroy(ACE_ENV_SINGLE_ARG_DECL);
  ACE_CHECK;

  ACE_ERROR ((LM_ERROR,
    "FaultNotifier (%P|%t) _remove_ref setting gone\n"
  ));
  this->gone_ = 1;
}

int TAO::FT_FaultNotifier_i::fini (ACE_ENV_SINGLE_ARG_DECL)
{
  if (this->ior_output_file_ != 0)
  {
    ACE_OS::unlink (this->ior_output_file_);
    this->ior_output_file_ = 0;
  }
  if (this->ns_name_ != 0 && this->naming_context_.in() != 0)
  {
    this->naming_context_->unbind (this_name_
                            ACE_ENV_ARG_PARAMETER);
    this->ns_name_ = 0;
  }

  if (this->registered_)
  {
    ACE_TRY
    {
      this->replication_manager_->register_fault_notifier(::FT::FaultNotifier::_nil ());
      ACE_TRY_CHECK;
      ACE_DEBUG ((LM_DEBUG,
        "FaultNotifier unregistered from ReplicationManager.\n"
        ));
    }
    ACE_CATCHANY
    {
      ACE_DEBUG ((LM_DEBUG,
        "FaultNotifier Can't unregister from ReplicationManager.\n"
        ));
      // complain, but otherwise ignore this error
      // RM may be down.
    }
    ACE_ENDTRY;

    this->registered_ = 0;
  }
  return 0;
}

int TAO::FT_FaultNotifier_i::init (CORBA::ORB_var & orb ACE_ENV_ARG_DECL )
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
  ACE_CHECK_RETURN(-1);

  poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN(-1);

  // Register with the POA.

  this->object_id_ = this->poa_->activate_object (this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(-1);

  // find my IOR

  CORBA::Object_var this_obj =
    this->poa_->id_to_reference (object_id_.in ()
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(-1);

  this->ior_ = this->orb_->object_to_string (this_obj.in ()
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(-1);


  ////////////////////////////////////////////////
  // Register with coresident Notification Channel
  CosNotifyChannelAdmin::EventChannelFactory_var notify_factory =
    TAO_Notify_EventChannelFactory_i::create (poa_.in ()
    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(-1);
  CosNotification::QoSProperties initial_qos;
  CosNotification::AdminProperties initial_admin;
  this->notify_channel_ =
    notify_factory->create_channel (initial_qos,
    initial_admin,
    channel_id_
    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(-1);

  this->filter_factory_ = this->notify_channel_->default_filter_factory (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN(-1);

  ///////////////////////////
  // Producer registration

  this->supplier_admin_ = this->notify_channel_->default_supplier_admin (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN(-1);

  ::CosNotifyChannelAdmin::ProxyID proxyId = 0;

  //////////////////////
  // structured producer
  ::CosNotifyChannelAdmin::ProxyConsumer_var consumer
    = this->supplier_admin_->obtain_notification_push_consumer (
      ::CosNotifyChannelAdmin::STRUCTURED_EVENT,
      proxyId
      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(-1);

  structured_proxy_push_consumer_
    = ::CosNotifyChannelAdmin::StructuredProxyPushConsumer::_narrow(consumer
      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(-1);
  if (CORBA::is_nil (this->structured_proxy_push_consumer_))
  {
    ACE_ERROR_RETURN ((LM_ERROR,
       "%T %n (%P|%t) Should not occur: Unable to narrow Structured Proxy Push Consumer\n"),
      1);
  }

  // todo: implement a push supplier if we want to receive disconnect notice
  CosNotifyComm::StructuredPushSupplier_var stubPushSupplier =
    CosNotifyComm::StructuredPushSupplier::_nil();

  this->structured_proxy_push_consumer_->connect_structured_push_supplier (stubPushSupplier.in()
     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(-1);

  ////////////////////
  // Sequence producer
  consumer
    = this->supplier_admin_->obtain_notification_push_consumer (
      ::CosNotifyChannelAdmin::SEQUENCE_EVENT,
      proxyId
      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  this->sequence_proxy_push_consumer_
    = ::CosNotifyChannelAdmin::SequenceProxyPushConsumer::_narrow(consumer
      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  if (CORBA::is_nil (this->sequence_proxy_push_consumer_))
  {
    ACE_ERROR_RETURN ((LM_ERROR,
       "%T %n (%P|%t) Should not occur: Unable to narrow Sequence Proxy Push Consumer\n"),
      1);
  }

  // todo: implement this if we want to receive disconnect notice
  CosNotifyComm::SequencePushSupplier_var stubSeqPushSupplier =
    CosNotifyComm::SequencePushSupplier::_nil();

  this->sequence_proxy_push_consumer_->connect_sequence_push_supplier (stubSeqPushSupplier.in()
    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(-1);
  ///////////////////////////
  // Consumer registration

  // find the channel administrator for consumers
  this->consumer_admin_ = this->notify_channel_->default_consumer_admin (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN(-1);
  if (CORBA::is_nil (this->consumer_admin_))
  {
    ACE_ERROR ((LM_ERROR,
      "%T %n (%P|%t) NIL consumer admin\n"
      ));
    result = -1;
  }
  // everything else happens when subscriber shows up

  ///////////////////////////////
  // Register with ReplicationManager
  if (this->rm_register_)
  {
    ACE_TRY_NEW_ENV
    {
      CORBA::Object_var rm_obj = orb->resolve_initial_references("ReplicationManager" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      this->replication_manager_ = ::FT::ReplicationManager::_narrow(rm_obj.in() ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (!CORBA::is_nil (replication_manager_))
      {
        // @@: should we check to see if there's already one registered?
        FT::FaultNotifier_var notifier = FT::FaultNotifier::_narrow (this_obj);
        if (! CORBA::is_nil (notifier))
        {
          this->replication_manager_->register_fault_notifier(notifier.in ());
          ACE_DEBUG ((LM_DEBUG,
            "FaultNotifier registered with ReplicationManager.\n"
            ));
          this->registered_ = 1;
        }
        else
        {
          ACE_ERROR ((LM_ERROR,
            "Error: Registration failed.  This is not a FaultNotifier (should not occur.)\n"
            ));
        }
      }
      else
      {
        ACE_ERROR ((LM_ERROR,"FaultNotifier: Can't resolve ReplicationManager, It will not be registered.\n" ));
      }
    }
    ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
        "ReplicaFactory: Exception resolving ReplicationManager, and no -f option was given.  Factory will not be registered.\n" );
    }
    ACE_ENDTRY;
  }
  else
  {
    ACE_DEBUG ((LM_DEBUG,
      "FaultNotifier: ReplicationManager registration disabled.\n"
      ));
  }
  ///////////////////////////////
  // Set up and ready for action
  // publish our IOR

  if(result == 0)
  {
    if (this->ior_output_file_ != 0)
    {
      this->identity_ = "file:";
      this->identity_ += this->ior_output_file_;
      result = write_ior();
    }
  }

  if (result == 0)
  {
    if (this->ns_name_ != 0)
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

      this->naming_context_ =
        CosNaming::NamingContext::_narrow (naming_obj.in () ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
      if (CORBA::is_nil(this->naming_context_))
      {
        ACE_ERROR_RETURN ((LM_ERROR,
           "%T %n (%P|%t) Should not occur: Can't narrow initial reference to naming context.\n"),
          1);
      }
      this->this_name_.length (1);
      this->this_name_[0].id = CORBA::string_dup (this->ns_name_);

      this->naming_context_->rebind (this->this_name_, this_obj.in()  //CORBA::Object::_duplicate(this_obj)
                              ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
    }
  }

  return result;
}

///////////////////
// CORBA METHODS

void TAO::FT_FaultNotifier_i::push_structured_fault (
    const CosNotification::StructuredEvent & event
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  METHOD_ENTRY(TAO::FT_FaultNotifier_i::push_structured_fault);

  this->structured_proxy_push_consumer_->push_structured_event (event
    ACE_ENV_ARG_PARAMETER);

  METHOD_RETURN(TAO::FT_FaultNotifier_i::push_structured_fault);
}

void TAO::FT_FaultNotifier_i::push_sequence_fault (
    const CosNotification::EventBatch & events
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  METHOD_ENTRY(TAO::FT_FaultNotifier_i::push_sequence_fault);

  this->sequence_proxy_push_consumer_->push_structured_events (events
    ACE_ENV_ARG_PARAMETER);

  METHOD_RETURN(TAO::FT_FaultNotifier_i::push_sequence_fault);
}

::CosNotifyFilter::Filter_ptr TAO::FT_FaultNotifier_i::create_subscription_filter (
    const char * constraint_grammar
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException, CosNotifyFilter::InvalidGrammar))
{
  METHOD_ENTRY(TAO::FT_FaultNotifier_i::create_subscription_filter);

  CosNotifyFilter::Filter_var filter = this->filter_factory_->create_filter ("ETCL");
  METHOD_RETURN(TAO::FT_FaultNotifier_i::create_subscription_filter)
    filter._retn ();
}

FT::FaultNotifier::ConsumerId TAO::FT_FaultNotifier_i::connect_structured_fault_consumer (
    CosNotifyComm::StructuredPushConsumer_ptr push_consumer,
    CosNotifyFilter::Filter_ptr filter
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  METHOD_ENTRY(TAO::FT_FaultNotifier_i::connect_structured_fault_consumer);

  /////////////////////////
  // find a ProxyInfo entry
  // use the first nil entry or a new entry if no nils found

  size_t infoPos = 0;
  int looking = 1;
  for ( size_t pos = 0; looking && pos < this->proxy_infos_.size (); ++pos)
  {
    ProxyInfo & pi = this->proxy_infos_[pos];
    if (CORBA::is_nil(pi.proxyVar_))
    {
      infoPos = pos;
      looking = 0;
    }
  }
  if (looking)
  {
    infoPos = this->proxy_infos_.size();
    this->proxy_infos_.push_back(ProxyInfo());
  }

  ///////////////////////////////////////
  // Assign an ID, populate the ProxyInfo
  FT::FaultNotifier::ConsumerId result = infoPos;
  ProxyInfo & info = this->proxy_infos_[infoPos];
  info.proxyVar_
      = this->consumer_admin_->obtain_notification_push_supplier (
      ::CosNotifyChannelAdmin::STRUCTURED_EVENT,
      info.proxyId_
      ACE_ENV_ARG_PARAMETER);

  this->consumer_connects_ += 1;

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

  METHOD_RETURN(TAO::FT_FaultNotifier_i::connect_structured_fault_consumer) result;
}

FT::FaultNotifier::ConsumerId TAO::FT_FaultNotifier_i::connect_sequence_fault_consumer (
    CosNotifyComm::SequencePushConsumer_ptr push_consumer,
    CosNotifyFilter::Filter_ptr filter
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  METHOD_ENTRY(TAO::FT_FaultNotifier_i::connect_sequence_fault_consumer);
  /////////////////////////
  // find a ProxyInfo entry
  // use the first nil entry or a new entry if no nils found

  size_t infoPos = 0;
  int looking = 1;
  for ( size_t pos = 0; looking && pos < this->proxy_infos_.size (); ++pos)
  {
    ProxyInfo & pi = this->proxy_infos_[pos];
    if (CORBA::is_nil(pi.proxyVar_))
    {
      infoPos = pos;
      looking = 0;
    }
  }
  if (looking)
  {
    infoPos = this->proxy_infos_.size();
    this->proxy_infos_.push_back(ProxyInfo());
  }

  ///////////////////////////////////////
  // Assign an ID, populate the ProxyInfo
  FT::FaultNotifier::ConsumerId result = infoPos;
  ProxyInfo & info = this->proxy_infos_[infoPos];
  info.proxyVar_
    = this->consumer_admin_->obtain_notification_push_supplier (
      ::CosNotifyChannelAdmin::SEQUENCE_EVENT,
      info.proxyId_
      ACE_ENV_ARG_PARAMETER);

  this->consumer_connects_ += 1;

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
  METHOD_RETURN(TAO::FT_FaultNotifier_i::connect_sequence_fault_consumer) result;
}

void TAO::FT_FaultNotifier_i::disconnect_consumer (
    FT::FaultNotifier::ConsumerId connection
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException, CosEventComm::Disconnected))
{
  METHOD_ENTRY(TAO::FT_FaultNotifier_i::disconnect_consumer);

  size_t index = ACE_static_cast ( size_t, connection);
  if (index < this->proxy_infos_.size())
  {
    ProxyInfo & info = this->proxy_infos_[index];
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
            "%T %n (%P|%t) Unexpected proxy supplier type\n"
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

  this->consumer_disconnects_ += 1;
  if (this->quit_on_idle_)
  {
    if (! this->quitting_
      && this->consumer_connects_ == this->consumer_disconnects_)
    {
      ACE_ERROR((LM_ERROR,
        "FaultNotifier (%P|%t) quit on idle: connects %d, disconnects %d\n",
        ACE_static_cast (unsigned int, this->consumer_connects_),
        ACE_static_cast (unsigned int, this->consumer_disconnects_)
        ));
      this->poa_->deactivate_object (this->object_id_.in ()
                   ACE_ENV_ARG_PARAMETER);
      this->quitting_ = 1;
    }
  }

  METHOD_RETURN(TAO::FT_FaultNotifier_i::disconnect_consumer);
}

CORBA::Boolean TAO::FT_FaultNotifier_i::is_alive (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  METHOD_RETURN(TAO::FT_FaultNotifier_i::is_alive) 1;
}

//////////////
// ProxyInfo

TAO::FT_FaultNotifier_i::ProxyInfo::ProxyInfo ()
  : proxyId_ (0)
  , proxyVar_ (::CosNotifyChannelAdmin::ProxySupplier::_nil())
{
}

TAO::FT_FaultNotifier_i::ProxyInfo::ProxyInfo (const ProxyInfo & rhs)
  : proxyId_ (rhs.proxyId_)
  , proxyVar_ (rhs.proxyVar_)
{
}
#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
  template ACE_Guard<ACE_SYNCH_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
# pragma instantiate ACE_Guard<ACE_SYNCH_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

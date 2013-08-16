// -*- C++ -*-
//=============================================================================
/**
 *  @file    FT_Notifier_i.cpp
 *
 *  $Id$
 *
 *  This file is part of Fault Tolerant CORBA.
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================

#include "orbsvcs/Log_Macros.h"
#include "FT_Notifier_i.h"

#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include "tao/debug.h"

// Use this macro at the beginning of CORBA methods
// to aid in debugging.
#define METHOD_ENTRY(name)    \
  if (TAO_debug_level > 6)    \
  {                           \
    ORBSVCS_DEBUG (( LM_DEBUG,    \
    "Enter %C\n", #name       \
      ));                     \
  }

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
  if (TAO_debug_level > 6)    \
  {                           \
    ORBSVCS_DEBUG (( LM_DEBUG,    \
      "Leave %C\n", #name     \
      ));                     \
  }                           \
  return /* value goes here */


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Implementation skeleton constructor
TAO::FT_FaultNotifier_i::FT_FaultNotifier_i ()
  : orb_ (0)
  , poa_ (0)
  , object_id_ (0)
  , ior_output_file_(0)
  , ns_name_(0)
  , naming_context_ (0)
  , this_name_ (1)
  , rm_register_ (1)
  , replication_manager_ (0)
  , registered_ (0)
  , identity_ ("")
  , proxy_infos_ (0)
  , consumer_connects_(0)
  , consumer_disconnects_(0)
  , notify_channel_ (0)
  , filter_factory_ (0)
  , supplier_admin_ (0)
  , consumer_admin_ (0)
  , structured_proxy_push_consumer_ (0)
  , sequence_proxy_push_consumer_ (0)
  , quit_on_idle_(0)
  , quitting_(0)
  , gone_(0)
{
}

// Implementation skeleton destructor
TAO::FT_FaultNotifier_i::~FT_FaultNotifier_i ()
{
  try
  {
    fini ();
  }
  catch (...)
  {
  }
}


int TAO::FT_FaultNotifier_i::idle(int &result)
{
  static unsigned long linger = 0;
  ACE_UNUSED_ARG(result);
  if (gone_)
  {
    if ( linger == 0)
    {
      ORBSVCS_ERROR ((LM_ERROR,
        "FaultNotifier (%P|%t) Begin linger.\n"
      ));
    }
    if(++linger > 5)//10)
    {
      ORBSVCS_ERROR ((LM_ERROR,
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
    ACE_OS::fprintf (out, "%s", this->ior_.in ());
    ACE_OS::fclose (out);
    result = 0;
  }
  else
  {
    ORBSVCS_ERROR ((LM_ERROR,
      "%T %n (%P|%t) Open failed for %s\n", ior_output_file_
    ));
  }
  return result;
}

//////////////////////////////////////////////////////
// FT_FaultNotifier_i public, non-CORBA methods

int TAO::FT_FaultNotifier_i::parse_args (int argc, ACE_TCHAR * argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:rq"));
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
        ORBSVCS_ERROR_RETURN ((LM_ERROR,
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
  // Indicates successful parsing of the command line
  return 0;
}

const char * TAO::FT_FaultNotifier_i::identity () const
{
  return this->identity_.c_str();
}

PortableServer::POA_ptr TAO::FT_FaultNotifier_i::_default_POA (void)
{
  return this->poa_.in();
}


void TAO::FT_FaultNotifier_i::_remove_ref (void)
{
  notify_channel_->destroy();

  ORBSVCS_ERROR ((LM_ERROR,
    "FaultNotifier (%P|%t) _remove_ref setting gone\n"
  ));
  this->gone_ = 1;
}

int TAO::FT_FaultNotifier_i::fini (void)
{
  if (this->ior_output_file_ != 0)
  {
    ACE_OS::unlink (this->ior_output_file_);
    this->ior_output_file_ = 0;
  }
  if (this->ns_name_ != 0 && this->naming_context_.in() != 0)
  {
    this->naming_context_->unbind (this_name_);
    this->ns_name_ = 0;
  }

  if (this->registered_)
  {
    try
    {
      this->replication_manager_->register_fault_notifier(::FT::FaultNotifier::_nil ());
      ORBSVCS_DEBUG ((LM_DEBUG,
        "FaultNotifier unregistered from ReplicationManager.\n"
        ));
    }
    catch (const CORBA::Exception&)
    {
      ORBSVCS_DEBUG ((LM_DEBUG,
        "FaultNotifier Can't unregister from ReplicationManager.\n"
        ));
      // complain, but otherwise ignore this error
      // RM may be down.
    }

    this->registered_ = 0;
  }
  return 0;
}

int TAO::FT_FaultNotifier_i::init (CORBA::ORB_ptr orb )
{
  int result = 0;
  this->orb_ = CORBA::ORB::_duplicate (orb);

  // Use the ROOT POA for now
  CORBA::Object_var poa_object =
    this->orb_->resolve_initial_references (TAO_OBJID_ROOTPOA);

  if (CORBA::is_nil (poa_object.in ()))
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT (" (%P|%t) Unable to initialize the POA.\n")),
                      -1);

  // Get the POA object.
  this->poa_ =
    PortableServer::POA::_narrow (poa_object.in ());


  if (CORBA::is_nil(this->poa_.in ()))
  {
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT (" (%P|%t) Unable to narrow the POA.\n")),
                      -1);
  }

  PortableServer::POAManager_var poa_manager =
    this->poa_->the_POAManager ();

  poa_manager->activate ();

  // Register with the POA.

  this->object_id_ = this->poa_->activate_object (this);

  // find my IOR

  CORBA::Object_var this_obj =
    this->poa_->id_to_reference (object_id_.in ());

  this->ior_ = this->orb_->object_to_string (this_obj.in ());


  ////////////////////////////////////////////////
  // Register with coresident Notification Channel
  CosNotifyChannelAdmin::EventChannelFactory_var notify_factory =
    TAO_Notify_EventChannelFactory_i::create (poa_.in ());
  CosNotification::QoSProperties initial_qos;
  CosNotification::AdminProperties initial_admin;
  this->notify_channel_ =
    notify_factory->create_channel (initial_qos,
    initial_admin,
    channel_id_);

  this->filter_factory_ = this->notify_channel_->default_filter_factory ();

  ///////////////////////////
  // Producer registration

  this->supplier_admin_ = this->notify_channel_->default_supplier_admin ();

  ::CosNotifyChannelAdmin::ProxyID proxyId = 0;

  //////////////////////
  // structured producer
  ::CosNotifyChannelAdmin::ProxyConsumer_var consumer
    = this->supplier_admin_->obtain_notification_push_consumer (
      ::CosNotifyChannelAdmin::STRUCTURED_EVENT,
      proxyId);

  structured_proxy_push_consumer_
    = ::CosNotifyChannelAdmin::StructuredProxyPushConsumer::_narrow(consumer.in ());
  if (CORBA::is_nil (this->structured_proxy_push_consumer_.in ()))
  {
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
       "%T %n (%P|%t) Should not occur: Unable to narrow Structured Proxy Push Consumer\n"),
      1);
  }

  // todo: implement a push supplier if we want to receive disconnect notice
  CosNotifyComm::StructuredPushSupplier_var stubPushSupplier =
    CosNotifyComm::StructuredPushSupplier::_nil();

  this->structured_proxy_push_consumer_->connect_structured_push_supplier (stubPushSupplier.in());

  ////////////////////
  // Sequence producer
  consumer
    = this->supplier_admin_->obtain_notification_push_consumer (
      ::CosNotifyChannelAdmin::SEQUENCE_EVENT,
      proxyId);

  this->sequence_proxy_push_consumer_
    = ::CosNotifyChannelAdmin::SequenceProxyPushConsumer::_narrow(consumer.in ());
  if (CORBA::is_nil (this->sequence_proxy_push_consumer_.in ()))
  {
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
       "%T %n (%P|%t) Should not occur: Unable to narrow Sequence Proxy Push Consumer\n"),
      1);
  }

  // todo: implement this if we want to receive disconnect notice
  CosNotifyComm::SequencePushSupplier_var stubSeqPushSupplier =
    CosNotifyComm::SequencePushSupplier::_nil();

  this->sequence_proxy_push_consumer_->connect_sequence_push_supplier (stubSeqPushSupplier.in());
  ///////////////////////////
  // Consumer registration

  // find the channel administrator for consumers
  this->consumer_admin_ = this->notify_channel_->default_consumer_admin ();
  if (CORBA::is_nil (this->consumer_admin_.in ()))
  {
    ORBSVCS_ERROR ((LM_ERROR,
      "%T %n (%P|%t) NIL consumer admin\n"
      ));
    result = -1;
  }
  // everything else happens when subscriber shows up

  ///////////////////////////////
  // Register with ReplicationManager
  if (this->rm_register_)
  {
    try
    {
      CORBA::Object_var rm_obj = orb->resolve_initial_references("ReplicationManager");
      this->replication_manager_ = ::FT::ReplicationManager::_narrow(rm_obj.in());
      if (!CORBA::is_nil (replication_manager_.in ()))
      {
        // @@: should we check to see if there's already one registered?
        FT::FaultNotifier_var notifier = FT::FaultNotifier::_narrow (this_obj.in ());
        if (! CORBA::is_nil (notifier.in ()))
        {
          this->replication_manager_->register_fault_notifier(notifier.in ());
          ORBSVCS_DEBUG ((LM_DEBUG,
            "FaultNotifier registered with ReplicationManager.\n"
            ));
          this->registered_ = 1;
        }
        else
        {
          ORBSVCS_ERROR ((LM_ERROR,
            "Error: Registration failed.  This is not a FaultNotifier (should not occur.)\n"
            ));
        }
      }
      else
      {
        ORBSVCS_ERROR ((LM_ERROR,"FaultNotifier: Can't resolve ReplicationManager, It will not be registered.\n" ));
      }
    }
    catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "FaultNotifier: Exception resolving ReplicationManager.  Notifier will not be registered.\n");
    }
  }
  else
  {
    ORBSVCS_DEBUG ((LM_DEBUG,
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
      this->identity_ += ACE_TEXT_ALWAYS_CHAR(this->ior_output_file_);
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
        this->orb_->resolve_initial_references ("NameService");

      if (CORBA::is_nil(naming_obj.in ())){
        ORBSVCS_ERROR_RETURN ((LM_ERROR,
                           "%T %n (%P|%t) Unable to find the Naming Service\n"),
                          1);
      }

      this->naming_context_ =
        CosNaming::NamingContext::_narrow (naming_obj.in ());
      if (CORBA::is_nil(this->naming_context_.in ()))
      {
        ORBSVCS_ERROR_RETURN ((LM_ERROR,
           "%T %n (%P|%t) Should not occur: Can't narrow initial reference to naming context.\n"),
          1);
      }
      this->this_name_.length (1);
      this->this_name_[0].id = CORBA::string_dup (this->ns_name_);

      this->naming_context_->rebind (this->this_name_, this_obj.in()  //CORBA::Object::_duplicate(this_obj)
                              );
    }
  }

  return result;
}

///////////////////
// CORBA METHODS

void TAO::FT_FaultNotifier_i::push_structured_fault (
    const CosNotification::StructuredEvent & event
  )
{
  METHOD_ENTRY(TAO::FT_FaultNotifier_i::push_structured_fault);

  this->structured_proxy_push_consumer_->push_structured_event (event);

  METHOD_RETURN(TAO::FT_FaultNotifier_i::push_structured_fault);
}

void TAO::FT_FaultNotifier_i::push_sequence_fault (
    const CosNotification::EventBatch & events
  )
{
  METHOD_ENTRY(TAO::FT_FaultNotifier_i::push_sequence_fault);

  this->sequence_proxy_push_consumer_->push_structured_events (events);

  METHOD_RETURN(TAO::FT_FaultNotifier_i::push_sequence_fault);
}

::CosNotifyFilter::Filter_ptr TAO::FT_FaultNotifier_i::create_subscription_filter (
    const char * constraint_grammar
  )
{
  METHOD_ENTRY(TAO::FT_FaultNotifier_i::create_subscription_filter);
  ACE_UNUSED_ARG (constraint_grammar); //@@todo

  CosNotifyFilter::Filter_var filter = this->filter_factory_->create_filter ("ETCL");
  METHOD_RETURN(TAO::FT_FaultNotifier_i::create_subscription_filter)
    filter._retn ();
}

FT::FaultNotifier::ConsumerId TAO::FT_FaultNotifier_i::connect_structured_fault_consumer (
    CosNotifyComm::StructuredPushConsumer_ptr push_consumer,
    CosNotifyFilter::Filter_ptr filter
  )
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
    if (CORBA::is_nil(pi.proxyVar_.in ()))
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
      info.proxyId_);

  this->consumer_connects_ += 1;

  ::CosNotifyChannelAdmin::StructuredProxyPushSupplier_var proxySupplier
    = ::CosNotifyChannelAdmin::StructuredProxyPushSupplier::_narrow(info.proxyVar_.in ());

  if ( CORBA::is_nil (proxySupplier.in ()))
  {
    // this is a shoould-not-occur situation.  The consumer admin returned
    // the wrong kind of object.
    ORBSVCS_ERROR(( LM_ERROR,
       "%T %n (%P|%t) Unexpected result: Wrong type of object returned from obtain_notification_push_supplier\n"
       ));
  }
  else
  {
    proxySupplier->connect_structured_push_consumer ( push_consumer);

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
  )
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
    if (CORBA::is_nil(pi.proxyVar_.in ()))
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
      info.proxyId_);

  this->consumer_connects_ += 1;

  ::CosNotifyChannelAdmin::SequenceProxyPushSupplier_var proxySupplier
    = ::CosNotifyChannelAdmin::SequenceProxyPushSupplier::_narrow(info.proxyVar_.in ());
  if ( CORBA::is_nil (proxySupplier.in ()))
  {
    // this is a shoould-not-occur situation.  The consumer admin returned
    // the wrong kind of object.
    ORBSVCS_ERROR(( LM_ERROR,
       "%T %n (%P|%t) Unexpected result: Wrong type of object returned from obtain_notification_push_supplier\n"
       ));
  }
  else
  {
    proxySupplier->connect_sequence_push_consumer ( push_consumer);

    if (! CORBA::is_nil (filter))
    {
      proxySupplier->add_filter(filter);
    }
  }
  METHOD_RETURN(TAO::FT_FaultNotifier_i::connect_sequence_fault_consumer) result;
}

void TAO::FT_FaultNotifier_i::disconnect_consumer (
    FT::FaultNotifier::ConsumerId connection
  )
{
  METHOD_ENTRY(TAO::FT_FaultNotifier_i::disconnect_consumer);

  size_t index = static_cast<size_t> (connection);
  if (index < this->proxy_infos_.size())
  {
    ProxyInfo & info = this->proxy_infos_[index];
    if (CORBA::is_nil(info.proxyVar_.in ()) )
    {
      throw CosEventComm::Disconnected();
    }
    else
    {
      ::CosNotifyChannelAdmin::StructuredProxyPushSupplier_var proxySupplier
        = ::CosNotifyChannelAdmin::StructuredProxyPushSupplier::_narrow(info.proxyVar_.in ());
      if (! CORBA::is_nil (proxySupplier.in ()))
      {
        proxySupplier->disconnect_structured_push_supplier ();
        info.proxyVar_ = ::CosNotifyChannelAdmin::ProxySupplier::_nil();
      }
      else
      {
        ::CosNotifyChannelAdmin::SequenceProxyPushSupplier_var proxySupplier
          = ::CosNotifyChannelAdmin::SequenceProxyPushSupplier::_narrow(info.proxyVar_.in ());
        if (! CORBA::is_nil (proxySupplier.in ()))
        {
          proxySupplier->disconnect_sequence_push_supplier ();
          info.proxyVar_ = ::CosNotifyChannelAdmin::ProxySupplier::_nil();
        }
        else
        {
          ORBSVCS_ERROR((LM_ERROR,
            "%T %n (%P|%t) Unexpected proxy supplier type\n"
            ));
          throw CosEventComm::Disconnected();
        }
      }
    }
  }
  else
  {
    throw CosEventComm::Disconnected();
  }

  this->consumer_disconnects_ += 1;
  if (this->quit_on_idle_)
  {
    if (! this->quitting_
      && this->consumer_connects_ == this->consumer_disconnects_)
    {
      ORBSVCS_ERROR((LM_ERROR,
        "FaultNotifier (%P|%t) quit on idle: connects %d, disconnects %d\n",
        static_cast<unsigned int> (this->consumer_connects_),
        static_cast<unsigned int> (this->consumer_disconnects_)
        ));
      this->poa_->deactivate_object (this->object_id_.in ());
      this->quitting_ = 1;
    }
  }

  METHOD_RETURN(TAO::FT_FaultNotifier_i::disconnect_consumer);
}

CORBA::Boolean TAO::FT_FaultNotifier_i::is_alive (void)
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

TAO_END_VERSIONED_NAMESPACE_DECL

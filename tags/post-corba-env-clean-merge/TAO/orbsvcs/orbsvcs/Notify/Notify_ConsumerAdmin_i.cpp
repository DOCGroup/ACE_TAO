// $Id$

// @@ Pradeep: you should always include the header file first, that
// way it is easier to detect inconsistencies or missing #includes in
// the .h file, so the first include in this case should be
// "Notify_ConsumerAdmin_i.h"

#include "orbsvcs/ESF/ESF_Proxy_Collection.h"
#include "Notify_ConsumerAdmin_i.h"
#include "Notify_ProxyPushSupplier_i.h"
#include "Notify_StructuredProxyPushSupplier_i.h"
#include "Notify_SequenceProxyPushSupplier_i.h"
#include "Notify_EventChannel_i.h"
#include "Notify_Event_Manager.h"
#include "Notify_Channel_Objects_Factory.h"
#include "Notify_POA_Factory.h"
#include "Notify_Event_Manager_Objects_Factory.h"
#include "Notify_Collection_Factory.h"
#include "Notify_Factory.h"
#include "Notify_Listener_Filter_Eval_Command.h"
#include "Notify_Worker_Task.h"
#include "Notify_Event_Dispatch_Command.h"
#include "Notify_Event_Processor.h"
#include "Notify_Worker_Task.h"

#include "tao/debug.h"

ACE_RCSID(Notify, Notify_ConsumerAdmin_i, "$Id$")

// Implementation skeleton constructor
TAO_Notify_ConsumerAdmin_i::TAO_Notify_ConsumerAdmin_i (TAO_Notify_EventChannel_i* event_channel)
  :lock_ (0),
   refcount_ (1),
   destory_child_POAs_ (0),
   event_channel_ (event_channel),
   // @@ Pradeep: don't use statics, it is better to pass these as
   // arguments.
   channel_objects_factory_ (TAO_Notify_Factory::get_channel_objects_factory ()),
   poa_factory_ (TAO_Notify_Factory::get_poa_factory ()),
   collection_factory_ (TAO_Notify_Factory::get_collection_factory ()),
   event_manager_ (event_channel->get_event_manager ()),
   event_listener_list_ (0),
   dispatching_task_ (0),
   filter_eval_task_ (0)
{
  // @@ Pradeep: don't forget the this-> stuff for local variables.
  this->event_manager_objects_factory_ = this->event_manager_->resource_factory ();
  this->event_channel_->_add_ref (); // we don't want our parent to go away!
}

// Implementation skeleton destructor
TAO_Notify_ConsumerAdmin_i::~TAO_Notify_ConsumerAdmin_i (void)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,"in CA %d dtor\n", this->my_id_));

   delete this->lock_;

   delete this->event_listener_list_;

   this->event_channel_->consumer_admin_destroyed (this->my_id_);
   event_channel_->_remove_ref ();

   delete this->dispatching_task_;
   delete this->filter_eval_task_;
}

CORBA::ULong
TAO_Notify_ConsumerAdmin_i::_incr_refcnt (void)
{
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);
  return this->refcount_++;
}

CORBA::ULong
TAO_Notify_ConsumerAdmin_i::_decr_refcnt (void)
{
  {
    ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);
    this->refcount_--;
    if (this->refcount_ != 0)
      return this->refcount_;
  }

  delete this;
  return 0;
}

void
TAO_Notify_ConsumerAdmin_i::_add_ref (TAO_ENV_SINGLE_ARG_DECL_NOT_USED/*TAO_ENV_SINGLE_ARG_PARAMETER*/)
{
  this->_incr_refcnt ();
}

void
TAO_Notify_ConsumerAdmin_i::_remove_ref (TAO_ENV_SINGLE_ARG_DECL_NOT_USED/*TAO_ENV_SINGLE_ARG_PARAMETER*/)
{
  this->_decr_refcnt ();
}

void
TAO_Notify_ConsumerAdmin_i::dispatch_event (TAO_Notify_Event &event TAO_ENV_ARG_DECL)
{
  // Dispatch the event to all the registered listeners.
  TAO_Notify_Dispatch_Command_Worker worker (&event, this->event_manager_->event_processor ());
  // Propogate the filter command.

  this->event_listener_list_->for_each (&worker TAO_ENV_ARG_PARAMETER);
}

CORBA::Boolean
TAO_Notify_ConsumerAdmin_i::evaluate_filter (TAO_Notify_Event &event, CORBA::Boolean /*eval_parent*/ TAO_ENV_ARG_DECL)
{
  CORBA::Boolean bval =
    this->get_filter_admin ().match (event TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if ((bval == 1 && this->filter_operator_ == CosNotifyChannelAdmin::AND_OP) ||
      (bval == 0 && this->filter_operator_ == CosNotifyChannelAdmin::OR_OP))
  {
    // If the filter operator requires that each listener attached to this admin be evaluated,
    // we feed the listeners to the "listener filter evaluation" task.

    TAO_Notify_Filter_Command_Worker worker (&event, this->event_manager_->event_processor (), 0);
    // note the last param. we ask that the parent filter *not* be evaluated again
    // because we've done it here.

    this->event_listener_list_->for_each (&worker TAO_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);
    // Propogate the filter command.

    return 0;
  }
  else if (bval == 1 && this->filter_operator_ == CosNotifyChannelAdmin::OR_OP)
    {
      // no need to check any of the children. happy dispatching!
      return 1;

      // OR we could instead do:
      //
      // this->dispatch_event (event TAO_ENV_ARG_PARAMETER);
      // return 0;
      //
      // This will short circuit a few steps, because the event processor is going to
      // turn around and create a dispatching command object to call dispatch_event.
      // My $.02 worth optimization fetish!
      // but we'll give in favour of consistency ;)
    }

  return 0; // in all [other] cases, return false.
}

void
TAO_Notify_ConsumerAdmin_i::proxy_pushsupplier_destroyed (CosNotifyChannelAdmin::ProxyID proxyID)
{
  this->proxy_pushsupplier_ids_.put (proxyID); // This id is no longer in use.
}

void
TAO_Notify_ConsumerAdmin_i::init (CosNotifyChannelAdmin::AdminID my_id,
                                  CosNotifyChannelAdmin::InterFilterGroupOperator filter_operator,
                                  PortableServer::POA_ptr my_POA
                                  TAO_ENV_ARG_DECL)
{
  my_POA_ = PortableServer::POA::_duplicate (my_POA);

  this->lock_ =
    this->channel_objects_factory_->create_consumer_admin_lock (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->proxy_pushsupplier_POA_ = this->poa_factory_->
    create_proxy_pushsupplier_POA (this->my_POA_.in (), my_id
                                   TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->my_id_ = my_id;
  this->filter_operator_ = filter_operator;

  this->event_listener_list_ =
    this->collection_factory_->create_event_listener_list (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Create the task to forward filtering/dispatching commands to:
  // @@ think about how get rid of these 2 allocations per consumer admin.
  // add a "get_singleton_reactive_task" to the event manager factory interface.
  this->dispatching_task_ =
    new TAO_Notify_Worker_Task ();
  /*this->event_manager_objects_factory_->create_dispatching_task (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  */
  this->filter_eval_task_ =
    new TAO_Notify_Worker_Task ();
  // this->event_manager_objects_factory_->create_listener_eval_task (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Get hold of the admin properties.
  TAO_Notify_AdminProperties* const admin_properties =
    this->event_manager_->admin_properties ();

  // Init the tasks
  this->dispatching_task_->init_task (admin_properties);
  this->filter_eval_task_->init_task (admin_properties);

  // Initially we set up things so that all listeners are subscribed for
  // all the events so that things "work" even if we don't twiddle with
  // the subscriptions. The side effect is that whenever we start setting
  // up subscriptions, we must remove the special event type otherwise we
  // will get the same event twice!
  // check with the resource manager if this option is enabled.
  //if (this->resource_manager_->default_subscription_enabled () == 1)
  /*    {
      TAO_Notify_EventType& special_type =
        TAO_Notify_EventType::special_event_type ();

      this->subscription_list_.insert (special_type);
    }
  */
}

void
TAO_Notify_ConsumerAdmin_i::destroy_i (TAO_ENV_SINGLE_ARG_DECL)
{
  CosNotification::EventTypeSeq removed;

  CosNotification::EventTypeSeq added (0);
  added.length (0);

  // unsubscribe it to our current subscriptions.
  removed.length (this->subscription_list_.size ());

  this->subscription_list_.populate (removed);

  this->event_manager_->subscribe_for_events (this,
                                              added, removed TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // deactivate ourselves
  this->poa_factory_->deactivate_object (this, this->my_POA_.in ()
                                         TAO_ENV_ARG_PARAMETER);

  // shutdown proxy's.
  TAO_Notify_Shutdown_Worker shutdown_worker;

  this->event_listener_list_->for_each (&shutdown_worker TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->event_listener_list_->shutdown (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->poa_factory_->destroy_POA (this->proxy_pushsupplier_POA_.in ()
                                   TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->dispatching_task_->shutdown (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->filter_eval_task_->shutdown (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_Notify_ConsumerAdmin_i::destroy (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  // unregister from EC
  this->event_channel_->unregister_listener (this TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->destroy_i (TAO_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Notify_ConsumerAdmin_i::shutdown (TAO_ENV_SINGLE_ARG_DECL)
{
  this->destroy_i (TAO_ENV_SINGLE_ARG_PARAMETER);
}

TAO_Notify_Worker_Task*
TAO_Notify_ConsumerAdmin_i::event_dispatch_task (void)
{
  return this->dispatching_task_;
}

TAO_Notify_Worker_Task*
TAO_Notify_ConsumerAdmin_i::filter_eval_task (void)
{
  return this->filter_eval_task_;
}

void
TAO_Notify_ConsumerAdmin_i::subscription_change (const CosNotification::EventTypeSeq & added, const CosNotification::EventTypeSeq & removed TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyComm::InvalidEventType
                   ))
{
  this->event_manager_->subscribe_for_events (this, //*this->event_listener_list_,
                                              added, removed TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
  {
    ACE_GUARD_THROW_EX (ACE_Lock, ace_mon, *this->lock_,
                        CORBA::INTERNAL ());
    ACE_CHECK;

    // simply update our subscription list.
    this->subscription_list_.insert_seq (added);
    this->subscription_list_.remove_seq (removed);
  }
}

CosNotifyChannelAdmin::ConsumerAdmin_ptr
TAO_Notify_ConsumerAdmin_i::get_ref (TAO_ENV_SINGLE_ARG_DECL)
{
  CORBA::Object_var obj = this->poa_factory_->
    servant_to_reference (this->my_POA_.in (), this TAO_ENV_ARG_PARAMETER);

  ACE_CHECK_RETURN (CosNotifyChannelAdmin::ConsumerAdmin::_nil ());

  return CosNotifyChannelAdmin::ConsumerAdmin::_narrow (obj.in ()
                                                        TAO_ENV_ARG_PARAMETER);
}

TAO_Notify_Event_Manager*
TAO_Notify_ConsumerAdmin_i::get_event_manager (void)
{
  return this->event_manager_;
}

TAO_Notify_FilterAdmin_i&
TAO_Notify_ConsumerAdmin_i::get_filter_admin (void)
{
  return this->filter_admin_;
}

void
TAO_Notify_ConsumerAdmin_i::register_listener (TAO_Notify_EventListener *listener TAO_ENV_ARG_DECL)
{
  // register it.
  this->event_listener_list_->connected (listener TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_Notify_ConsumerAdmin_i::unregister_listener (TAO_Notify_EventListener *listener TAO_ENV_ARG_DECL)
{
  this->event_listener_list_->disconnected (listener TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_Notify_ConsumerAdmin_i::deactivate_proxy_pushsupplier (PortableServer::Servant servant TAO_ENV_ARG_DECL)
{
  this->poa_factory_->
    deactivate_object (servant, this->proxy_pushsupplier_POA_.in ()
                       TAO_ENV_ARG_PARAMETER);
}

CosNotifyChannelAdmin::AdminID
TAO_Notify_ConsumerAdmin_i::MyID (TAO_ENV_SINGLE_ARG_DECL_NOT_USED/*TAO_ENV_SINGLE_ARG_PARAMETER*/)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return my_id_;
}

CosNotifyChannelAdmin::EventChannel_ptr
TAO_Notify_ConsumerAdmin_i::MyChannel (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return event_channel_->get_ref (TAO_ENV_SINGLE_ARG_PARAMETER);
}

CosNotifyChannelAdmin::InterFilterGroupOperator
TAO_Notify_ConsumerAdmin_i::MyOperator (TAO_ENV_SINGLE_ARG_DECL_NOT_USED/*TAO_ENV_SINGLE_ARG_PARAMETER*/)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return filter_operator_;
}

CosNotifyChannelAdmin::ProxyIDSeq*
TAO_Notify_ConsumerAdmin_i::push_suppliers (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->proxy_pushsupplier_ids_.get_sequence (TAO_ENV_SINGLE_ARG_PARAMETER);
}

CosNotifyChannelAdmin::ProxySupplier_ptr
TAO_Notify_ConsumerAdmin_i::get_proxy_supplier (CosNotifyChannelAdmin::ProxyID proxy_id TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyChannelAdmin::ProxyNotFound
                   ))
{
  CORBA::Object_var obj =
    this->poa_factory_->id_to_reference (proxy_id,
                                         proxy_pushsupplier_POA_.in ()
                                         TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::ProxySupplier::_nil ());

  return CosNotifyChannelAdmin::ProxySupplier::_narrow (obj.in ());
}

CORBA::Object_ptr
TAO_Notify_ConsumerAdmin_i::obtain_struct_proxy_pushsupplier_i (CosNotifyChannelAdmin::ProxyID proxy_id TAO_ENV_ARG_DECL)
{
  TAO_Notify_StructuredProxyPushSupplier_i* struct_proxy_pushsupplier =
    this->channel_objects_factory_->create_struct_proxy_pushsupplier (this
                                                                      TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  PortableServer::ServantBase_var proxy_pushsupplier_var (struct_proxy_pushsupplier);

  struct_proxy_pushsupplier->init (proxy_id TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  return this->poa_factory_->
    activate_object_with_id (proxy_id,
                             this->proxy_pushsupplier_POA_.in (),
                             struct_proxy_pushsupplier
                             TAO_ENV_ARG_PARAMETER);
}

CORBA::Object_ptr
TAO_Notify_ConsumerAdmin_i::obtain_sequence_proxy_pushsupplier_i (CosNotifyChannelAdmin::ProxyID proxy_id TAO_ENV_ARG_DECL)
{
  TAO_Notify_SequenceProxyPushSupplier_i* seq_proxy_pushsupplier =
    this->channel_objects_factory_->create_seq_proxy_pushsupplier (this
                                                                   TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  PortableServer::ServantBase_var proxy_pushsupplier_var (seq_proxy_pushsupplier);

  seq_proxy_pushsupplier->init (proxy_id TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  return this->poa_factory_->
    activate_object_with_id (proxy_id,
                             this->proxy_pushsupplier_POA_.in (),
                             seq_proxy_pushsupplier
                             TAO_ENV_ARG_PARAMETER);
}

CORBA::Object_ptr
TAO_Notify_ConsumerAdmin_i::obtain_proxy_pushsupplier_i (CosNotifyChannelAdmin::ProxyID proxy_id TAO_ENV_ARG_DECL)
{
  TAO_Notify_ProxyPushSupplier_i* proxy_pushsupplier =
    this->channel_objects_factory_->create_proxy_pushsupplier (this TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  PortableServer::ServantBase_var proxy_pushsupplier_var (proxy_pushsupplier);

  proxy_pushsupplier->init (proxy_id TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  return this->poa_factory_->
    activate_object_with_id (proxy_id,
                             this->proxy_pushsupplier_POA_.in (),
                             proxy_pushsupplier
                             TAO_ENV_ARG_PARAMETER);
}

CosNotifyChannelAdmin::ProxySupplier_ptr
TAO_Notify_ConsumerAdmin_i::obtain_notification_push_supplier (CosNotifyChannelAdmin::ClientType ctype, CosNotifyChannelAdmin::ProxyID_out proxy_id TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyChannelAdmin::AdminLimitExceeded
                   ))
{
  CORBA::Object_var obj;

  {
    ACE_GUARD_THROW_EX (ACE_Lock, ace_mon, *this->lock_,
                        CORBA::INTERNAL ());
    ACE_CHECK_RETURN (CosNotifyChannelAdmin::ProxySupplier::_nil ());

    proxy_id = this->proxy_pushsupplier_ids_.get ();
  }

  switch (ctype)
    {
    case CosNotifyChannelAdmin::ANY_EVENT:
      {
        obj = this->obtain_proxy_pushsupplier_i (proxy_id TAO_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (CosNotifyChannelAdmin::ProxySupplier::_nil ());
      }
      break;
    case CosNotifyChannelAdmin::STRUCTURED_EVENT:
      {
        obj = this->obtain_struct_proxy_pushsupplier_i (proxy_id TAO_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (CosNotifyChannelAdmin::ProxySupplier::_nil ());
      }
      break;

    case CosNotifyChannelAdmin::SEQUENCE_EVENT:
      {
        obj = this->obtain_sequence_proxy_pushsupplier_i (proxy_id
                                                          TAO_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (CosNotifyChannelAdmin::ProxySupplier::_nil ());
      }
      break;

    default:
      ACE_THROW_RETURN (CORBA::BAD_PARAM (),
                        CosNotifyChannelAdmin::ProxySupplier::_nil ());
    }

  this->proxy_pushsupplier_ids_.next (); // commit this id to the pool
  return CosNotifyChannelAdmin::ProxySupplier::_narrow (obj.in ()
                                                        TAO_ENV_ARG_PARAMETER);
}

CosNotification::QoSProperties*
TAO_Notify_ConsumerAdmin_i::get_qos (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->qos_admin_.get_qos (TAO_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Notify_ConsumerAdmin_i::set_qos (const CosNotification::QoSProperties & qos TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotification::UnsupportedQoS
                   ))
{
  this->qos_admin_.set_qos (qos TAO_ENV_ARG_PARAMETER);
}

void
TAO_Notify_ConsumerAdmin_i::validate_qos (const CosNotification::QoSProperties & required_qos, CosNotification::NamedPropertyRangeSeq_out available_qos TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotification::UnsupportedQoS
                   ))
{
  this->qos_admin_.validate_qos (required_qos, available_qos TAO_ENV_ARG_PARAMETER);
}

CosNotifyFilter::FilterID
TAO_Notify_ConsumerAdmin_i::add_filter (CosNotifyFilter::Filter_ptr new_filter TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->filter_admin_.add_filter (new_filter TAO_ENV_ARG_PARAMETER);
}

void
TAO_Notify_ConsumerAdmin_i::remove_filter (CosNotifyFilter::FilterID filter TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyFilter::FilterNotFound
                   ))
{
  this->filter_admin_.remove_filter (filter TAO_ENV_ARG_PARAMETER);
}

CosNotifyFilter::Filter_ptr
TAO_Notify_ConsumerAdmin_i::get_filter (CosNotifyFilter::FilterID filter TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyFilter::FilterNotFound
                   ))
{
  return this->filter_admin_.get_filter (filter TAO_ENV_ARG_PARAMETER);
}

CosNotifyFilter::FilterIDSeq*
TAO_Notify_ConsumerAdmin_i::get_all_filters (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->filter_admin_.get_all_filters (TAO_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Notify_ConsumerAdmin_i::remove_all_filters (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  this->filter_admin_.remove_all_filters (TAO_ENV_SINGLE_ARG_PARAMETER);
}

CosEventChannelAdmin::ProxyPushSupplier_ptr
TAO_Notify_ConsumerAdmin_i::obtain_push_supplier (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  TAO_Notify_CosEC_ProxyPushSupplier_i* cosec_proxy;

  ACE_NEW_THROW_EX (cosec_proxy,
                    TAO_Notify_CosEC_ProxyPushSupplier_i (this),
                    CORBA::NO_MEMORY ());

  PortableServer::ServantBase_var proxy_var (cosec_proxy);

  cosec_proxy->init (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CosEventChannelAdmin::ProxyPushSupplier::_nil ());

  PortableServer::POA_var def_poa =
    this->event_channel_->get_default_POA ();

  CORBA::Object_var obj = this->poa_factory_->
    activate_object (def_poa.in (), cosec_proxy TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CosEventChannelAdmin::ProxyPushSupplier::_nil ());

  return CosEventChannelAdmin::ProxyPushSupplier::_narrow (obj.in ());
}

CosNotifyChannelAdmin::ProxySupplier_ptr
TAO_Notify_ConsumerAdmin_i::obtain_notification_pull_supplier (CosNotifyChannelAdmin::ClientType /*ctype*/, CosNotifyChannelAdmin::ProxyID_out /*proxy_id*/ TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::AdminLimitExceeded
    ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    CosNotifyChannelAdmin::ProxySupplier::_nil ());
}

CosEventChannelAdmin::ProxyPullSupplier_ptr
TAO_Notify_ConsumerAdmin_i::obtain_pull_supplier (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    CosEventChannelAdmin::ProxyPullSupplier::_nil ());
}


CosNotifyFilter::MappingFilter_ptr
TAO_Notify_ConsumerAdmin_i::priority_filter (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    CosNotifyFilter::MappingFilter::_nil ());
}

void
TAO_Notify_ConsumerAdmin_i::priority_filter (CosNotifyFilter::MappingFilter_ptr /*priority_filter*/ TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

CosNotifyFilter::MappingFilter_ptr
TAO_Notify_ConsumerAdmin_i::lifetime_filter (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    CosNotifyFilter::MappingFilter::_nil ());
}

void
TAO_Notify_ConsumerAdmin_i::lifetime_filter (CosNotifyFilter::MappingFilter_ptr /*lifetime_filter*/ TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

CosNotifyChannelAdmin::ProxyIDSeq*
TAO_Notify_ConsumerAdmin_i::pull_suppliers (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

/****************************************************************************************************/

TAO_Notify_Filter_Command_Worker::TAO_Notify_Filter_Command_Worker (TAO_Notify_Event* event, TAO_Notify_Event_Processor* event_processor, CORBA::Boolean eval_parent)
  : event_ (event),
    event_processor_ (event_processor),
    eval_parent_ (eval_parent)
{
}

void
TAO_Notify_Filter_Command_Worker::work (TAO_Notify_EventListener* event_listener TAO_ENV_ARG_DECL)
{
  this->event_processor_->evaluate_listener_filter (this->event_, event_listener,
                                                    this->eval_parent_ TAO_ENV_ARG_PARAMETER);
}

/****************************************************************************************************/

TAO_Notify_Dispatch_Command_Worker::TAO_Notify_Dispatch_Command_Worker (TAO_Notify_Event* event, TAO_Notify_Event_Processor* event_processor)
  : event_ (event),
    event_processor_ (event_processor)
{
}

TAO_Notify_Dispatch_Command_Worker::~TAO_Notify_Dispatch_Command_Worker ()
{
}

void
TAO_Notify_Dispatch_Command_Worker::work (TAO_Notify_EventListener* event_listener TAO_ENV_ARG_DECL)
{
  this->event_processor_->dispatch_event (this->event_, event_listener TAO_ENV_ARG_PARAMETER);
}

/****************************************************************************************************/

// $Id$

#include "Notify_ConsumerAdmin_i.h"
#include "Notify_ProxyPushSupplier_i.h"
#include "Notify_StructuredProxyPushSupplier_i.h"
#include "Notify_SequenceProxyPushSupplier_i.h"
#include "Notify_EventChannel_i.h"
#include "Notify_Resource_Manager.h"
#include "Notify_Event_Manager.h"

// Implementation skeleton constructor
TAO_Notify_ConsumerAdmin_i::TAO_Notify_ConsumerAdmin_i (TAO_Notify_EventChannel_i* myChannel, TAO_Notify_Resource_Manager* resource_manager)
  :my_channel_ (myChannel),
   resource_manager_ (resource_manager),
   event_manager_ (myChannel->get_event_manager ()),
   is_destroyed_ (0)
{
  // No-Op.
}

// Implementation skeleton destructor
TAO_Notify_ConsumerAdmin_i::~TAO_Notify_ConsumerAdmin_i (void)
{
  if (this->is_destroyed_ == 0)
    this->cleanup_i ();
}

void
TAO_Notify_ConsumerAdmin_i::init (CosNotifyChannelAdmin::AdminID myID,
                                  CosNotifyChannelAdmin::InterFilterGroupOperator myOperator,
                                  PortableServer::POA_ptr my_POA,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  my_POA_ = PortableServer::POA::_duplicate (my_POA);

  this->proxy_pushsupplier_POA_ = this->resource_manager_->
    create_proxy_pushsupplier_POA (this->my_POA_.in (),
                                   ACE_TRY_ENV);
  ACE_CHECK;

  this->myID_ = myID;
  this->myOperator_ = myOperator;

  // Initially we set up things so that all listeners are subscribed for
  // all the events so that things "work" even if we don't twiddle with
  // the subscriptions. The side effect is that whenever we start setting
  // up subscriptions, we must remove the special event type otherwise we
  // will get the same event twice!
  // check with the resource manager if this option is enabled.
  if (this->resource_manager_->default_subscription_enabled () == 1)
    {
      TAO_Notify_EventType& special_type =
        TAO_Notify_EventType::special_event_type ();

      this->subscription_list_.insert (special_type);
    }
}

void
TAO_Notify_ConsumerAdmin_i::destroy (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  this->is_destroyed_ = 1;

  // deactivate ourselves
  this->resource_manager_->deactivate_object (this, this->my_POA_.in (),
                                              ACE_TRY_ENV);
  this->cleanup_i (ACE_TRY_ENV);
}

void
TAO_Notify_ConsumerAdmin_i::subscription_change (const CosNotification::EventTypeSeq & added, const CosNotification::EventTypeSeq & removed, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyComm::InvalidEventType
                   ))
{
  this->event_manager_->subscribe_for_events (this->event_listener_list_,
                                              &this->subscription_list_,
                                              added, removed, ACE_TRY_ENV);
}

CosNotifyChannelAdmin::ConsumerAdmin_ptr
TAO_Notify_ConsumerAdmin_i::get_ref (CORBA::Environment &ACE_TRY_ENV)
{
  return CosNotifyChannelAdmin::ConsumerAdmin
    ::_narrow (this->resource_manager_->
               servant_to_reference (this->my_POA_.in (), this, ACE_TRY_ENV));
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
TAO_Notify_ConsumerAdmin_i::register_listener (TAO_Notify_Event_Listener *listener, CORBA::Environment &ACE_TRY_ENV)
{
  // register it.
  this->event_listener_list_.insert (listener);

  // subscribe it to our current subscriptions.
  CosNotification::EventTypeSeq added (this->subscription_list_.size ());
  CosNotification::EventTypeSeq removed (0);

  added.length (this->subscription_list_.size ());
  removed.length (0);

  // copy value to the added list.
  EVENTTYPE_LIST::ITERATOR iter (this->subscription_list_);
  TAO_Notify_EventType* event_type;

  CORBA::ULong i = 0;
  for (iter.first (); iter.next (event_type); iter.advance (), ++i)
    added[i] = event_type->get_native ();

  this->event_manager_->subscribe_for_events (listener,
                                              0,
                                              added, removed, ACE_TRY_ENV);
}

void
TAO_Notify_ConsumerAdmin_i::unregister_listener (TAO_Notify_Event_Listener *listener, CORBA::Environment &ACE_TRY_ENV)
{
  this->event_listener_list_.remove (listener);

  // unsubscribe it to our current subscriptions.
  CosNotification::EventTypeSeq added (0);
  CosNotification::EventTypeSeq removed (this->subscription_list_.size ());

  removed.length (this->subscription_list_.size ());
  added.length (0);

  // copy value to the added list.
  EVENTTYPE_LIST::ITERATOR iter (this->subscription_list_);
  TAO_Notify_EventType* event_type;

  CORBA::ULong i = 0;
  for (iter.first (); iter.next (event_type); iter.advance (), ++i)
    removed[i] = event_type->get_native ();

  this->event_manager_->subscribe_for_events (listener,
                                              0,
                                              added, removed, ACE_TRY_ENV);
}

void
TAO_Notify_ConsumerAdmin_i::cleanup_i (CORBA::Environment &ACE_TRY_ENV)
{
  // Cleanup all resources..
  this->resource_manager_->destroy_POA (this->proxy_pushsupplier_POA_.in (),
                                        ACE_TRY_ENV);
  this->proxy_pushsupplier_POA_ = PortableServer::POA::_nil ();
  this->my_POA_ = PortableServer::POA::_nil ();
}

void
TAO_Notify_ConsumerAdmin_i::deactivate_proxy_pushsupplier (PortableServer::Servant servant, CORBA::Environment &ACE_TRY_ENV)
{
  this->resource_manager_->
    deactivate_object (servant, this->proxy_pushsupplier_POA_.in (),
                       ACE_TRY_ENV);
}

CosNotifyChannelAdmin::AdminID
TAO_Notify_ConsumerAdmin_i::MyID (CORBA::Environment &/*ACE_TRY_ENV*/)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return myID_;
}

CosNotifyChannelAdmin::EventChannel_ptr
TAO_Notify_ConsumerAdmin_i::MyChannel (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return my_channel_->get_ref (ACE_TRY_ENV);
}

CosNotifyChannelAdmin::InterFilterGroupOperator
TAO_Notify_ConsumerAdmin_i::MyOperator (CORBA::Environment &/*ACE_TRY_ENV*/)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return myOperator_;
}

CosNotifyFilter::MappingFilter_ptr
TAO_Notify_ConsumerAdmin_i::priority_filter (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    CosNotifyFilter::MappingFilter::_nil ());
}

void
TAO_Notify_ConsumerAdmin_i::priority_filter (CosNotifyFilter::MappingFilter_ptr /*priority_filter*/, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

CosNotifyFilter::MappingFilter_ptr
TAO_Notify_ConsumerAdmin_i::lifetime_filter (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    CosNotifyFilter::MappingFilter::_nil ());
}

void
TAO_Notify_ConsumerAdmin_i::lifetime_filter (CosNotifyFilter::MappingFilter_ptr /*lifetime_filter*/, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

CosNotifyChannelAdmin::ProxyIDSeq*
TAO_Notify_ConsumerAdmin_i::pull_suppliers (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

CosNotifyChannelAdmin::ProxyIDSeq*
TAO_Notify_ConsumerAdmin_i::push_suppliers (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->proxy_pushsupplier_ids_.get_sequence (ACE_TRY_ENV);
}

CosNotifyChannelAdmin::ProxySupplier_ptr
TAO_Notify_ConsumerAdmin_i::get_proxy_supplier (CosNotifyChannelAdmin::ProxyID proxy_id, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyChannelAdmin::ProxyNotFound
                   ))
{
  CORBA::Object_var obj =
    this->resource_manager_->id_to_reference (proxy_id,
                                              proxy_pushsupplier_POA_.in (),
                                              ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::ProxySupplier::_nil ());

  return CosNotifyChannelAdmin::ProxySupplier::_narrow (obj.in ());
}

CosNotifyChannelAdmin::ProxySupplier_ptr
TAO_Notify_ConsumerAdmin_i::obtain_notification_pull_supplier (CosNotifyChannelAdmin::ClientType /*ctype*/, CosNotifyChannelAdmin::ProxyID_out /*proxy_id*/, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::AdminLimitExceeded
    ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    CosNotifyChannelAdmin::ProxySupplier::_nil ());
}

CORBA::Object_ptr
TAO_Notify_ConsumerAdmin_i::obtain_struct_proxy_pushsupplier_i (CosNotifyChannelAdmin::ProxyID proxy_id, CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Notify_StructuredProxyPushSupplier_i* struct_proxy_pushsupplier =
    this->resource_manager_->create_struct_proxy_pushsupplier (this,
                                                               ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  PortableServer::ServantBase_var proxy_pushsupplier_var (struct_proxy_pushsupplier);

  struct_proxy_pushsupplier->init (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  return this->resource_manager_->
    activate_object_with_id (proxy_id,
                             this->proxy_pushsupplier_POA_.in (),
                             struct_proxy_pushsupplier,
                             ACE_TRY_ENV);
}

CORBA::Object_ptr
TAO_Notify_ConsumerAdmin_i::obtain_sequence_proxy_pushsupplier_i (CosNotifyChannelAdmin::ProxyID proxy_id, CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Notify_SequenceProxyPushSupplier_i* seq_proxy_pushsupplier =
    this->resource_manager_->create_seq_proxy_pushsupplier (this,
                                                            ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  PortableServer::ServantBase_var proxy_pushsupplier_var (seq_proxy_pushsupplier);

  seq_proxy_pushsupplier->init (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  return this->resource_manager_->
    activate_object_with_id (proxy_id,
                             this->proxy_pushsupplier_POA_.in (),
                             seq_proxy_pushsupplier,
                             ACE_TRY_ENV);
}

CORBA::Object_ptr
TAO_Notify_ConsumerAdmin_i::obtain_proxy_pushsupplier_i (CosNotifyChannelAdmin::ProxyID proxy_id, CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Notify_ProxyPushSupplier_i* proxy_pushsupplier =
    this->resource_manager_->create_proxy_pushsupplier (this, ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  PortableServer::ServantBase_var proxy_pushsupplier_var (proxy_pushsupplier);

  proxy_pushsupplier->init (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  return this->resource_manager_->
    activate_object_with_id (proxy_id,
                             this->proxy_pushsupplier_POA_.in (),
                             proxy_pushsupplier,
                             ACE_TRY_ENV);
}

CosNotifyChannelAdmin::ProxySupplier_ptr
TAO_Notify_ConsumerAdmin_i::obtain_notification_push_supplier (CosNotifyChannelAdmin::ClientType ctype, CosNotifyChannelAdmin::ProxyID_out proxy_id, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyChannelAdmin::AdminLimitExceeded
                   ))
{
  CORBA::Object_var obj;

  proxy_id = this->proxy_pushsupplier_ids_.get ();

  switch (ctype)
    {
    case CosNotifyChannelAdmin::ANY_EVENT:
      {
        obj = this->obtain_proxy_pushsupplier_i (proxy_id, ACE_TRY_ENV);
        ACE_CHECK_RETURN (CosNotifyChannelAdmin::ProxySupplier::_nil ());
      }
      break;
    case CosNotifyChannelAdmin::STRUCTURED_EVENT:
      {
        obj = this->obtain_struct_proxy_pushsupplier_i (proxy_id, ACE_TRY_ENV);
        ACE_CHECK_RETURN (CosNotifyChannelAdmin::ProxySupplier::_nil ());
      }
      break;

    case CosNotifyChannelAdmin::SEQUENCE_EVENT:
      {
        obj = this->obtain_sequence_proxy_pushsupplier_i (proxy_id,
                                                          ACE_TRY_ENV);
        ACE_CHECK_RETURN (CosNotifyChannelAdmin::ProxySupplier::_nil ());
      }
      break;

    default:
      ACE_THROW_RETURN (CORBA::BAD_PARAM (),
                        CosNotifyChannelAdmin::ProxySupplier::_nil ());
    }

  this->proxy_pushsupplier_ids_.next ();

  return CosNotifyChannelAdmin::ProxySupplier::_narrow (obj.in (),
                                                        ACE_TRY_ENV);
}

CosNotification::QoSProperties*
TAO_Notify_ConsumerAdmin_i::get_qos (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->qos_admin_.get_qos (ACE_TRY_ENV);
}

void
TAO_Notify_ConsumerAdmin_i::set_qos (const CosNotification::QoSProperties & qos, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotification::UnsupportedQoS
                   ))
{
  this->qos_admin_.set_qos (qos, ACE_TRY_ENV);
}

void
TAO_Notify_ConsumerAdmin_i::validate_qos (const CosNotification::QoSProperties & required_qos, CosNotification::NamedPropertyRangeSeq_out available_qos, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotification::UnsupportedQoS
                   ))
{
  this->qos_admin_.validate_qos (required_qos, available_qos, ACE_TRY_ENV);
}

CosNotifyFilter::FilterID
TAO_Notify_ConsumerAdmin_i::add_filter (CosNotifyFilter::Filter_ptr new_filter, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->filter_admin_.add_filter (new_filter, ACE_TRY_ENV);
}

void
TAO_Notify_ConsumerAdmin_i::remove_filter (CosNotifyFilter::FilterID filter, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyFilter::FilterNotFound
                   ))
{
  this->filter_admin_.remove_filter (filter, ACE_TRY_ENV);
}

CosNotifyFilter::Filter_ptr
TAO_Notify_ConsumerAdmin_i::get_filter (CosNotifyFilter::FilterID filter, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyFilter::FilterNotFound
                   ))
{
  return this->filter_admin_.get_filter (filter, ACE_TRY_ENV);
}

CosNotifyFilter::FilterIDSeq*
TAO_Notify_ConsumerAdmin_i::get_all_filters (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->filter_admin_.get_all_filters (ACE_TRY_ENV);
}

void
TAO_Notify_ConsumerAdmin_i::remove_all_filters (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  this->filter_admin_.remove_all_filters (ACE_TRY_ENV);
}

CosEventChannelAdmin::ProxyPushSupplier_ptr
TAO_Notify_ConsumerAdmin_i::obtain_push_supplier (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  TAO_Notify_CosEC_ProxyPushSupplier_i* cosec_proxy;

  ACE_NEW_THROW_EX (cosec_proxy,
                    TAO_Notify_CosEC_ProxyPushSupplier_i (this, this->resource_manager_),
                    CORBA::NO_MEMORY ());

  PortableServer::ServantBase_var proxy_var (cosec_proxy);

  cosec_proxy->init (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosEventChannelAdmin::ProxyPushSupplier::_nil ());

  PortableServer::POA_var def_poa =
    this->resource_manager_->get_default_POA ();

  CORBA::Object_var obj = this->resource_manager_->
    activate_object (def_poa.in (), cosec_proxy, ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosEventChannelAdmin::ProxyPushSupplier::_nil ());

  return CosEventChannelAdmin::ProxyPushSupplier::_narrow (obj.in ());
}

CosEventChannelAdmin::ProxyPullSupplier_ptr
TAO_Notify_ConsumerAdmin_i::obtain_pull_supplier (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    CosEventChannelAdmin::ProxyPullSupplier::_nil ());
}

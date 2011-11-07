// $Id$

#include "orbsvcs/Notify/SupplierAdmin.h"
#include "orbsvcs/Notify/Builder.h"
#include "orbsvcs/Notify/ProxyConsumer.h"
#include "orbsvcs/Notify/EventChannel.h"
#include "orbsvcs/Notify/Subscription_Change_Worker.h"
#include "orbsvcs/Notify/Find_Worker_T.h"
#include "orbsvcs/Notify/Seq_Worker_T.h"
#include "orbsvcs/Notify/Properties.h"

#include "tao/debug.h"

#include "orbsvcs/ESF/ESF_Proxy_Collection.h"

//#define DEBUG_LEVEL 9
#ifndef DEBUG_LEVEL
# define DEBUG_LEVEL TAO_debug_level
#endif //DEBUG_LEVEL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

typedef TAO_Notify_Find_Worker_T<TAO_Notify_Proxy
                                 , CosNotifyChannelAdmin::ProxyConsumer
                                 , CosNotifyChannelAdmin::ProxyConsumer_ptr
                                 , CosNotifyChannelAdmin::ProxyNotFound>
TAO_Notify_ProxyConsumer_Find_Worker;

typedef TAO_Notify_Seq_Worker_T<TAO_Notify_Proxy> TAO_Notify_Proxy_Seq_Worker;

TAO_Notify_SupplierAdmin::TAO_Notify_SupplierAdmin (void)
  : TAO_Notify_Admin ()
{
}

const char *
TAO_Notify_SupplierAdmin::get_admin_type_name () const
{
  return "supplier_admin";
}

TAO_Notify_SupplierAdmin::~TAO_Notify_SupplierAdmin ()
{
}

void
TAO_Notify_SupplierAdmin::init (TAO_Notify_EventChannel *ec)
{
  TAO_Notify_Admin::init (ec);

  const CosNotification::QoSProperties &default_sa_qos =
    TAO_Notify_PROPERTIES::instance ()->default_supplier_admin_qos_properties ();

  this->set_qos (default_sa_qos);
}

void
TAO_Notify_SupplierAdmin::_add_ref (void)
{
  this->_incr_refcnt ();
}

void
TAO_Notify_SupplierAdmin::_remove_ref (void)
{
  this->_decr_refcnt ();
}

void
TAO_Notify_SupplierAdmin::release (void)
{
  delete this;
  //@@ inform factory
}

void
TAO_Notify_SupplierAdmin::destroy (void)
{
  this->shutdown ();
  this->ec_->remove (this);
  this->proxy_container ().destroy ();
}

TAO_Notify::Topology_Object*
TAO_Notify_SupplierAdmin::load_child (const ACE_CString &type,
  CORBA::Long id, const TAO_Notify::NVPList& attrs)
{
  TAO_Notify::Topology_Object* result = this;
  if (type == "proxy_push_consumer")
  {
    if (DEBUG_LEVEL) ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Admin reload proxy %d\n")
      , static_cast<int> (id)
      ));
    result = this->load_proxy(id, CosNotifyChannelAdmin::ANY_EVENT, attrs);
  }
  else if (type == "structured_proxy_push_consumer")
  {
    if (DEBUG_LEVEL) ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Admin reload proxy %d\n")
      , static_cast<int> (id)
      ));
    result = this->load_proxy(id, CosNotifyChannelAdmin::STRUCTURED_EVENT, attrs);
  }
  else if (type == "sequence_proxy_push_consumer")
  {
    if (DEBUG_LEVEL) ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Admin reload proxy %d\n")
      , static_cast<int> (id)
      ));
    result = this->load_proxy(id, CosNotifyChannelAdmin::SEQUENCE_EVENT, attrs);
  }
  else
  {
    result = TAO_Notify_Admin::load_child (type, id, attrs);
  }
  return result;
}

TAO_Notify::Topology_Object*
TAO_Notify_SupplierAdmin::load_proxy (
  CORBA::Long id,
  CosNotifyChannelAdmin::ClientType ctype,
  const TAO_Notify::NVPList& attrs)
{
  TAO_Notify_Builder* bld = TAO_Notify_PROPERTIES::instance()->builder();
  TAO_Notify_ProxyConsumer * proxy =
    bld->build_proxy (this
               , ctype
               , id);
  ACE_ASSERT(proxy != 0);
  proxy->load_attrs (attrs);
  return proxy;
}


void
TAO_Notify_SupplierAdmin::set_qos (const CosNotification::QoSProperties & qos)
{
  this->TAO_Notify_Object::set_qos (qos);
}

CosNotification::QoSProperties*
TAO_Notify_SupplierAdmin::get_qos (void)
{
  return this->TAO_Notify_Object::get_qos ();
}

CosNotifyChannelAdmin::ProxyConsumer_ptr
TAO_Notify_SupplierAdmin::obtain_notification_push_consumer (CosNotifyChannelAdmin::ClientType ctype
                                                         , CosNotifyChannelAdmin::ProxyID_out proxy_id)

{
  CosNotification::QoSProperties initial_qos;

  CosNotifyChannelAdmin::ProxyConsumer_var proxy =
    TAO_Notify_PROPERTIES::instance()->builder()->build_proxy (this
                                                                , ctype
                                                                , proxy_id
                                                                , initial_qos);
  this->self_change ();
  return proxy._retn ();
}

CosNotifyChannelAdmin::ProxyConsumer_ptr
TAO_Notify_SupplierAdmin::obtain_notification_push_consumer_with_qos (CosNotifyChannelAdmin::ClientType ctype,
                                                                  CosNotifyChannelAdmin::ProxyID_out proxy_id,
                                                                  const CosNotification::QoSProperties & initial_qos)
{
  CosNotifyChannelAdmin::ProxyConsumer_var proxy =
    TAO_Notify_PROPERTIES::instance()->builder()->build_proxy (this
                                                                , ctype
                                                                , proxy_id,
                                                                initial_qos);
  this->self_change ();
  return proxy._retn ();
}


CosEventChannelAdmin::ProxyPushConsumer_ptr
TAO_Notify_SupplierAdmin::obtain_push_consumer (void)
{
  CosEventChannelAdmin::ProxyPushConsumer_var proxy =
    TAO_Notify_PROPERTIES::instance()->builder()->build_proxy (this);
  this->self_change ();
  return proxy._retn ();
}

CosNotifyChannelAdmin::AdminID
TAO_Notify_SupplierAdmin::MyID (void)
{
  return this->id ();
}

CosNotifyChannelAdmin::EventChannel_ptr
TAO_Notify_SupplierAdmin::MyChannel (void)
{
  return this->ec_->_this ();
}

::CosNotifyChannelAdmin::InterFilterGroupOperator
TAO_Notify_SupplierAdmin::MyOperator (void)
{
  return this->filter_operator_;
}

CosNotifyChannelAdmin::ProxyIDSeq*
TAO_Notify_SupplierAdmin::push_consumers (void)
{
  TAO_Notify_Proxy_Seq_Worker seq_worker;

  return seq_worker.create (this->proxy_container());
}

CosNotifyChannelAdmin::ProxyConsumer_ptr
TAO_Notify_SupplierAdmin::get_proxy_consumer (CosNotifyChannelAdmin::ProxyID proxy_id)
{
  TAO_Notify_ProxyConsumer_Find_Worker find_worker;

  return find_worker.resolve (proxy_id, this->proxy_container());
}

void
TAO_Notify_SupplierAdmin::offer_change (const CosNotification::EventTypeSeq & added,
                                        const CosNotification::EventTypeSeq & removed)

{
  TAO_Notify_EventTypeSeq seq_added (added);
  TAO_Notify_EventTypeSeq seq_removed (removed);

  {
    ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                        CORBA::INTERNAL ());

    this->subscribed_types_.add_and_remove (seq_added, seq_removed);

    TAO_Notify_Subscription_Change_Worker worker (added, removed);

    this->proxy_container().collection ()->for_each (&worker);
  }
}

CosNotifyFilter::FilterID
TAO_Notify_SupplierAdmin::add_filter (CosNotifyFilter::Filter_ptr new_filter)
{
  CosNotifyFilter::FilterID fid =
    this->filter_admin_.add_filter (new_filter);
  this->self_change ();
  return fid;
}

void
TAO_Notify_SupplierAdmin::remove_filter (CosNotifyFilter::FilterID filter)
{
  this->filter_admin_.remove_filter (filter);
}

::CosNotifyFilter::Filter_ptr
TAO_Notify_SupplierAdmin::get_filter (CosNotifyFilter::FilterID filter)
{
  return this->filter_admin_.get_filter (filter);
}

::CosNotifyFilter::FilterIDSeq*
TAO_Notify_SupplierAdmin::get_all_filters (void)
{
  return this->filter_admin_.get_all_filters ();
}

void
TAO_Notify_SupplierAdmin::remove_all_filters (void)
{
  this->filter_admin_.remove_all_filters ();
}

TAO_Notify_ProxyConsumer *
TAO_Notify_SupplierAdmin::find_proxy_consumer (
    TAO_Notify::IdVec & id_path,
    size_t position)
{
  TAO_Notify_ProxyConsumer * result = 0;
  size_t path_size = id_path.size ();
  if (position < path_size)
  {
    TAO_Notify_ProxyConsumer_Find_Worker find_worker;
    TAO_Notify_Proxy * proxy = find_worker.find (id_path[position], this->proxy_container());
    result = dynamic_cast <TAO_Notify_ProxyConsumer *> (proxy);
  }
  return result;
}

/************** UNIMPLEMENTED METHODS ***************/

CosEventChannelAdmin::ProxyPullConsumer_ptr
TAO_Notify_SupplierAdmin::obtain_pull_consumer (void)
{
  throw CORBA::NO_IMPLEMENT ();
}

CosNotifyChannelAdmin::ProxyIDSeq*
TAO_Notify_SupplierAdmin::pull_consumers (void)
{
  throw CORBA::NO_IMPLEMENT ();
}


void
TAO_Notify_SupplierAdmin::validate_qos (const CosNotification::QoSProperties & /*required_qos*/,
                                    CosNotification::NamedPropertyRangeSeq_out /*available_qos*/)
{
  throw CORBA::NO_IMPLEMENT ();
}

CosNotifyChannelAdmin::ProxyConsumer_ptr
TAO_Notify_SupplierAdmin::obtain_notification_pull_consumer (CosNotifyChannelAdmin::ClientType /*ctype*/,
                                                         CosNotifyChannelAdmin::ProxyID_out /*proxy_id*/)
{
  throw CORBA::NO_IMPLEMENT ();
}

TAO_END_VERSIONED_NAMESPACE_DECL

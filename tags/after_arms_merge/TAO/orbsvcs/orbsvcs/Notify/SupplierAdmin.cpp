// $Id$

#include "orbsvcs/Notify/SupplierAdmin.h"

ACE_RCSID (RT_Notify,
           TAO_Notify_SupplierAdmin,
           "$Id$")

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
TAO_Notify_SupplierAdmin::init (TAO_Notify_EventChannel *ec ACE_ENV_ARG_DECL)
{
  TAO_Notify_Admin::init (ec ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  const CosNotification::QoSProperties &default_sa_qos =
    TAO_Notify_PROPERTIES::instance ()->default_supplier_admin_qos_properties ();

  this->set_qos (default_sa_qos ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_SupplierAdmin::_add_ref (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  this->_incr_refcnt ();
}

void
TAO_Notify_SupplierAdmin::_remove_ref (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
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
TAO_Notify_SupplierAdmin::destroy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  int result = this->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  if ( result == 1)
    return;

  this->ec_->remove (this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

TAO_Notify::Topology_Object*
TAO_Notify_SupplierAdmin::load_child (const ACE_CString &type,
  CORBA::Long id, const TAO_Notify::NVPList& attrs ACE_ENV_ARG_DECL)
{
  TAO_Notify::Topology_Object* result = this;
  if (type == "proxy_push_consumer")
  {
    if (DEBUG_LEVEL) ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Admin reload proxy %d\n")
      , static_cast<int> (id)
      ));
    result = this->load_proxy(id, CosNotifyChannelAdmin::ANY_EVENT, attrs ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);
  }
  else if (type == "structured_proxy_push_consumer")
  {
    if (DEBUG_LEVEL) ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Admin reload proxy %d\n")
      , static_cast<int> (id)
      ));
    result = this->load_proxy(id, CosNotifyChannelAdmin::STRUCTURED_EVENT, attrs ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);
  }
  else if (type == "sequence_proxy_push_consumer")
  {
    if (DEBUG_LEVEL) ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Admin reload proxy %d\n")
      , static_cast<int> (id)
      ));
    result = this->load_proxy(id, CosNotifyChannelAdmin::SEQUENCE_EVENT, attrs ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);
  }
  else
  {
    result = TAO_Notify_Admin::load_child (type, id, attrs ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);
  }
  return result;
}

TAO_Notify::Topology_Object*
TAO_Notify_SupplierAdmin::load_proxy (
  CORBA::Long id,
  CosNotifyChannelAdmin::ClientType ctype,
  const TAO_Notify::NVPList& attrs ACE_ENV_ARG_DECL)
{
  TAO_Notify_Builder* bld = TAO_Notify_PROPERTIES::instance()->builder();
  TAO_Notify_ProxyConsumer * proxy =
    bld->build_proxy (this
               , ctype
               , id
               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(0);
  ACE_ASSERT(proxy != 0);
  proxy->load_attrs (attrs);
  return proxy;
}


void
TAO_Notify_SupplierAdmin::set_qos (const CosNotification::QoSProperties & qos ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotification::UnsupportedQoS
                   ))
{
  this->TAO_Notify_Object::set_qos (qos ACE_ENV_ARG_PARAMETER);
}

CosNotification::QoSProperties*
TAO_Notify_SupplierAdmin::get_qos (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->TAO_Notify_Object::get_qos (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CosNotifyChannelAdmin::ProxyConsumer_ptr
TAO_Notify_SupplierAdmin::obtain_notification_push_consumer (CosNotifyChannelAdmin::ClientType ctype
                                                         , CosNotifyChannelAdmin::ProxyID_out proxy_id
                                                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotifyChannelAdmin::AdminLimitExceeded
                   ))

{
  CosNotification::QoSProperties initial_qos;

  CosNotifyChannelAdmin::ProxyConsumer_var proxy =
    TAO_Notify_PROPERTIES::instance()->builder()->build_proxy (this
                                                                , ctype
                                                                , proxy_id
                                                                , initial_qos
                                                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (proxy._retn ());
  this->self_change (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (proxy._retn ());
  return proxy._retn ();
}

CosNotifyChannelAdmin::ProxyConsumer_ptr
TAO_Notify_SupplierAdmin::obtain_notification_push_consumer_with_qos (CosNotifyChannelAdmin::ClientType ctype,
                                                                  CosNotifyChannelAdmin::ProxyID_out proxy_id,
                                                                  const CosNotification::QoSProperties & initial_qos
                                                                  ACE_ENV_ARG_DECL
                                                                  )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotifyChannelAdmin::AdminLimitExceeded
                   , CosNotification::UnsupportedQoS
                   ))
{
  CosNotifyChannelAdmin::ProxyConsumer_var proxy =
    TAO_Notify_PROPERTIES::instance()->builder()->build_proxy (this
                                                                , ctype
                                                                , proxy_id,
                                                                initial_qos
                                                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (proxy._retn ());
  this->self_change (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (proxy._retn ());
  return proxy._retn ();
}


CosEventChannelAdmin::ProxyPushConsumer_ptr
TAO_Notify_SupplierAdmin::obtain_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  CosEventChannelAdmin::ProxyPushConsumer_var proxy =
    TAO_Notify_PROPERTIES::instance()->builder()->build_proxy (this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (proxy._retn ());
  this->self_change (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (proxy._retn ());
  return proxy._retn ();
}

CosNotifyChannelAdmin::AdminID
TAO_Notify_SupplierAdmin::MyID (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->id ();
}

CosNotifyChannelAdmin::EventChannel_ptr
TAO_Notify_SupplierAdmin::MyChannel (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->ec_->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
}

::CosNotifyChannelAdmin::InterFilterGroupOperator
TAO_Notify_SupplierAdmin::MyOperator (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->filter_operator_;
}

CosNotifyChannelAdmin::ProxyIDSeq*
TAO_Notify_SupplierAdmin::push_consumers (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  TAO_Notify_Proxy_Seq_Worker seq_worker;

  return seq_worker.create (this->proxy_container() ACE_ENV_ARG_PARAMETER);
}

CosNotifyChannelAdmin::ProxyConsumer_ptr
TAO_Notify_SupplierAdmin::get_proxy_consumer (CosNotifyChannelAdmin::ProxyID proxy_id ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotifyChannelAdmin::ProxyNotFound
                   ))
{
  TAO_Notify_ProxyConsumer_Find_Worker find_worker;

  return find_worker.resolve (proxy_id, this->proxy_container() ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_SupplierAdmin::offer_change (const CosNotification::EventTypeSeq & added,
                                    const CosNotification::EventTypeSeq & removed
                                    ACE_ENV_ARG_DECL
                                    )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotifyComm::InvalidEventType
                   ))

{
  TAO_Notify_EventTypeSeq seq_added (added);
  TAO_Notify_EventTypeSeq seq_removed (removed);

  {
    ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                        CORBA::INTERNAL ());
    ACE_CHECK;

    this->subscribed_types_.add_and_remove (seq_added, seq_removed);

    TAO_Notify_Subscription_Change_Worker worker (added, removed);

    this->proxy_container().collection ()->for_each (&worker ACE_ENV_ARG_PARAMETER);
  }
}

CosNotifyFilter::FilterID
TAO_Notify_SupplierAdmin::add_filter (CosNotifyFilter::Filter_ptr new_filter ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  CosNotifyFilter::FilterID fid =
    this->filter_admin_.add_filter (new_filter ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (fid);
  this->self_change (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (fid);
  return fid;
}

void
TAO_Notify_SupplierAdmin::remove_filter (CosNotifyFilter::FilterID filter ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotifyFilter::FilterNotFound
                   ))
{
  this->filter_admin_.remove_filter (filter ACE_ENV_ARG_PARAMETER);
}

::CosNotifyFilter::Filter_ptr
TAO_Notify_SupplierAdmin::get_filter (CosNotifyFilter::FilterID filter ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotifyFilter::FilterNotFound
                   ))
{
  return this->filter_admin_.get_filter (filter ACE_ENV_ARG_PARAMETER);
}

::CosNotifyFilter::FilterIDSeq*
TAO_Notify_SupplierAdmin::get_all_filters (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->filter_admin_.get_all_filters (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Notify_SupplierAdmin::remove_all_filters (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  this->filter_admin_.remove_all_filters (ACE_ENV_SINGLE_ARG_PARAMETER);
}

/************** UNIMPLEMENTED METHODS ***************/

CosEventChannelAdmin::ProxyPullConsumer_ptr
TAO_Notify_SupplierAdmin::obtain_pull_consumer (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), CosEventChannelAdmin::ProxyPullConsumer::_nil ());
}

CosNotifyChannelAdmin::ProxyIDSeq*
TAO_Notify_SupplierAdmin::pull_consumers (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}


void
TAO_Notify_SupplierAdmin::validate_qos (const CosNotification::QoSProperties & /*required_qos*/,
                                    CosNotification::NamedPropertyRangeSeq_out /*available_qos*/
                                    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotification::UnsupportedQoS
                   ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

CosNotifyChannelAdmin::ProxyConsumer_ptr
TAO_Notify_SupplierAdmin::obtain_notification_pull_consumer (CosNotifyChannelAdmin::ClientType /*ctype*/,
                                                         CosNotifyChannelAdmin::ProxyID_out /*proxy_id*/
                                                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotifyChannelAdmin::AdminLimitExceeded
                   ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), CosNotifyChannelAdmin::ProxyConsumer::_nil ());
}

TAO_Notify_ProxyConsumer *
TAO_Notify_SupplierAdmin::find_proxy_consumer (
    TAO_Notify::IdVec & id_path,
    size_t position
    ACE_ENV_ARG_DECL)
{
  TAO_Notify_ProxyConsumer * result = 0;
  size_t path_size = id_path.size ();
  if (position < path_size)
  {
    TAO_Notify_ProxyConsumer_Find_Worker find_worker;
    TAO_Notify_Proxy * proxy = find_worker.find (id_path[position], this->proxy_container() ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);
    result = dynamic_cast <TAO_Notify_ProxyConsumer *> (proxy);
  }
  return result;
}

TAO_END_VERSIONED_NAMESPACE_DECL

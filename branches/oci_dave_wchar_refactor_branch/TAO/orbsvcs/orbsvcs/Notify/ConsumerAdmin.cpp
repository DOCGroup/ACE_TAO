// $Id$

#include "orbsvcs/Notify/ConsumerAdmin.h"

ACE_RCSID (RT_Notify,
           TAO_Notify_ConsumerAdmin,
           "$Id$")

#include "orbsvcs/Notify/Subscription_Change_Worker.h"
#include "orbsvcs/Notify/Proxy.h"
#include "orbsvcs/Notify/EventChannel.h"
#include "orbsvcs/Notify/Properties.h"
#include "orbsvcs/Notify/Factory.h"
#include "orbsvcs/Notify/Builder.h"
#include "orbsvcs/Notify/Find_Worker_T.h"
#include "orbsvcs/Notify/Seq_Worker_T.h"
#include "orbsvcs/Notify/ProxySupplier.h"

#include "tao/debug.h"

#include "orbsvcs/ESF/ESF_Proxy_Collection.h"

#include "ace/Auto_Ptr.h"

//#define DEBUG_LEVEL 9
#ifndef DEBUG_LEVEL
# define DEBUG_LEVEL TAO_debug_level
#endif //DEBUG_LEVEL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

typedef TAO_Notify_Find_Worker_T<TAO_Notify_Proxy
                             , CosNotifyChannelAdmin::ProxySupplier
                             , CosNotifyChannelAdmin::ProxySupplier_ptr
                             , CosNotifyChannelAdmin::ProxyNotFound>
TAO_Notify_ProxySupplier_Find_Worker;

typedef TAO_Notify_Seq_Worker_T<TAO_Notify_Proxy> TAO_Notify_Proxy_Seq_Worker;

TAO_Notify_ConsumerAdmin::TAO_Notify_ConsumerAdmin (void)
  : TAO_Notify_Admin ()
{
}

const char *
TAO_Notify_ConsumerAdmin::get_admin_type_name () const
{
  return "consumer_admin";
}



TAO_Notify_ConsumerAdmin::~TAO_Notify_ConsumerAdmin ()
{
}

void
TAO_Notify_ConsumerAdmin::init (TAO_Notify_EventChannel *ec ACE_ENV_ARG_DECL)
{
  TAO_Notify_Admin::init (ec ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  const CosNotification::QoSProperties &default_ca_qos =
    TAO_Notify_PROPERTIES::instance ()->default_consumer_admin_qos_properties ();

  this->set_qos (default_ca_qos ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_Notify_ConsumerAdmin::_add_ref (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  this->_incr_refcnt ();
}

void
TAO_Notify_ConsumerAdmin::_remove_ref (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  this->_decr_refcnt ();
}

void
TAO_Notify_ConsumerAdmin::release (void)
{
  delete this;
  //@@ inform factory
}

void
TAO_Notify_ConsumerAdmin::destroy (ACE_ENV_SINGLE_ARG_DECL)
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
TAO_Notify_ConsumerAdmin::load_child (const ACE_CString &type,
  CORBA::Long id, const TAO_Notify::NVPList& attrs ACE_ENV_ARG_DECL)
{
  TAO_Notify::Topology_Object* result = this;
  if (type == "proxy_push_supplier")
  {
    if (DEBUG_LEVEL) ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Admin reload proxy %d\n")
      , static_cast<int> (id)
      ));
    result = this->load_proxy(id, CosNotifyChannelAdmin::ANY_EVENT, attrs ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);
  }
  else if (type == "structured_proxy_push_supplier")
  {
    if (DEBUG_LEVEL) ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Admin reload proxy %d\n")
      , static_cast<int> (id)
      ));
    result = this->load_proxy(id, CosNotifyChannelAdmin::STRUCTURED_EVENT, attrs ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);
  }
  else if (type == "sequence_proxy_push_supplier")
  {
    if (DEBUG_LEVEL) ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Admin reload proxy %d\n")
      , static_cast<int> (id)
      ));
    result = this->load_proxy(id, CosNotifyChannelAdmin::SEQUENCE_EVENT, attrs ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);
  }
#if 0
  else if (type == "ec_proxy_push_supplier")
  {
    if (DEBUG_LEVEL) ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Admin reload proxy %d\n")
      , static_cast<int> (id)
      ));
    result = this->load_proxy(id, attrs ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);
  }
#endif
  else
  {
    result = TAO_Notify_Admin::load_child (type, id, attrs ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);
  }
  return result;
}

TAO_Notify::Topology_Object*
TAO_Notify_ConsumerAdmin::load_proxy (
  CORBA::Long id,
  CosNotifyChannelAdmin::ClientType ctype,
  const TAO_Notify::NVPList& attrs ACE_ENV_ARG_DECL)
{
  TAO_Notify_Builder* bld = TAO_Notify_PROPERTIES::instance()->builder();
  TAO_Notify_ProxySupplier * proxy =
    bld->build_proxy (this
               , ctype
               , id
               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(0);
  ACE_ASSERT(proxy != 0);
  proxy->load_attrs (attrs);
  return proxy;
}

CosNotifyChannelAdmin::ProxySupplier_ptr
TAO_Notify_ConsumerAdmin::obtain_notification_push_supplier (CosNotifyChannelAdmin::ClientType ctype,
                                                         CosNotifyChannelAdmin::ProxyID_out proxy_id
                                                         ACE_ENV_ARG_DECL
                                                         )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotifyChannelAdmin::AdminLimitExceeded
                   ))
{
  CosNotification::QoSProperties initial_qos;

  CosNotifyChannelAdmin::ProxySupplier_var proxy =
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

CosNotifyChannelAdmin::ProxySupplier_ptr
TAO_Notify_ConsumerAdmin::obtain_notification_push_supplier_with_qos (CosNotifyChannelAdmin::ClientType ctype,
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
  CosNotifyChannelAdmin::ProxySupplier_var proxy =
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

CosEventChannelAdmin::ProxyPushSupplier_ptr
TAO_Notify_ConsumerAdmin::obtain_push_supplier (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  CosEventChannelAdmin::ProxyPushSupplier_var proxy =
    TAO_Notify_PROPERTIES::instance()->builder()->build_proxy (this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (proxy._retn ());
  this->self_change (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (proxy._retn ());
  return proxy._retn ();
}

CosNotifyChannelAdmin::AdminID
TAO_Notify_ConsumerAdmin::MyID (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->id ();
}

CosNotifyChannelAdmin::EventChannel_ptr
TAO_Notify_ConsumerAdmin::MyChannel (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->ec_->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
}

::CosNotifyChannelAdmin::InterFilterGroupOperator
TAO_Notify_ConsumerAdmin::MyOperator (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->filter_operator_;
}

CosNotifyChannelAdmin::ProxyIDSeq*
TAO_Notify_ConsumerAdmin::push_suppliers (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  TAO_Notify_Proxy_Seq_Worker seq_worker;

  return seq_worker.create (this->proxy_container() ACE_ENV_ARG_PARAMETER);
}

CosNotifyChannelAdmin::ProxySupplier_ptr
TAO_Notify_ConsumerAdmin::get_proxy_supplier (CosNotifyChannelAdmin::ProxyID proxy_id ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotifyChannelAdmin::ProxyNotFound
                   ))
{
  TAO_Notify_ProxySupplier_Find_Worker find_worker;

  return find_worker.resolve (proxy_id, this->proxy_container() ACE_ENV_ARG_PARAMETER);
}

void TAO_Notify_ConsumerAdmin::set_qos (const CosNotification::QoSProperties & qos ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotification::UnsupportedQoS
                   ))
{
  this->TAO_Notify_Object::set_qos (qos ACE_ENV_ARG_PARAMETER);
}


CosNotification::QoSProperties*
TAO_Notify_ConsumerAdmin::get_qos (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->TAO_Notify_Object::get_qos (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Notify_ConsumerAdmin::subscription_change (const CosNotification::EventTypeSeq & added,
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

    this->proxy_container().collection()->for_each (&worker ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  }
  this->self_change (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

CosNotifyFilter::FilterID
TAO_Notify_ConsumerAdmin::add_filter (CosNotifyFilter::Filter_ptr new_filter ACE_ENV_ARG_DECL)
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
TAO_Notify_ConsumerAdmin::remove_filter (CosNotifyFilter::FilterID filter ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotifyFilter::FilterNotFound
                   ))
{
  this->filter_admin_.remove_filter (filter ACE_ENV_ARG_PARAMETER);
}

::CosNotifyFilter::Filter_ptr
TAO_Notify_ConsumerAdmin::get_filter (CosNotifyFilter::FilterID filter ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotifyFilter::FilterNotFound
                   ))
{
  return this->filter_admin_.get_filter (filter ACE_ENV_ARG_PARAMETER);
}

::CosNotifyFilter::FilterIDSeq*
TAO_Notify_ConsumerAdmin::get_all_filters (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->filter_admin_.get_all_filters (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Notify_ConsumerAdmin::remove_all_filters (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  this->filter_admin_.remove_all_filters (ACE_ENV_SINGLE_ARG_PARAMETER);
}

/************ UNIMPLMENTED METHODS *************************/

CosNotifyFilter::MappingFilter_ptr
TAO_Notify_ConsumerAdmin::priority_filter (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), CosNotifyFilter::MappingFilter::_nil ());
}

void
TAO_Notify_ConsumerAdmin::priority_filter (CosNotifyFilter::MappingFilter_ptr /*priority_filter*/ ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))

{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

CosNotifyFilter::MappingFilter_ptr
TAO_Notify_ConsumerAdmin::lifetime_filter (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))

{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), CosNotifyFilter::MappingFilter::_nil ());
}

void
TAO_Notify_ConsumerAdmin::lifetime_filter (CosNotifyFilter::MappingFilter_ptr /*lifetime_filter*/ ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

::CosNotifyChannelAdmin::ProxyIDSeq*
TAO_Notify_ConsumerAdmin::pull_suppliers (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

CosNotifyChannelAdmin::ProxySupplier_ptr
TAO_Notify_ConsumerAdmin::obtain_notification_pull_supplier (CosNotifyChannelAdmin::ClientType /*ctype*/,
                                                         CosNotifyChannelAdmin::ProxyID_out /*proxy_id*/
                                                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotifyChannelAdmin::AdminLimitExceeded
                   ))

{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), CosNotifyChannelAdmin::ProxySupplier::_nil ());
}

void
TAO_Notify_ConsumerAdmin::validate_qos (const CosNotification::QoSProperties & /*required_qos*/,
                                    CosNotification::NamedPropertyRangeSeq_out /*available_qos*/
                                    ACE_ENV_ARG_DECL
                                    )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotification::UnsupportedQoS
                   ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

CosEventChannelAdmin::ProxyPullSupplier_ptr
TAO_Notify_ConsumerAdmin::obtain_pull_supplier (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), CosEventChannelAdmin::ProxyPullSupplier::_nil ());
}

TAO_Notify_ProxySupplier *
TAO_Notify_ConsumerAdmin::find_proxy_supplier (
    TAO_Notify::IdVec & id_path,
    size_t position
    ACE_ENV_ARG_DECL)
{
  TAO_Notify_ProxySupplier * result = 0;
  size_t path_size = id_path.size ();
  if (position < path_size)
  {
    TAO_Notify_ProxySupplier_Find_Worker find_worker;
    TAO_Notify_Proxy * proxy = find_worker.find (id_path[position], this->proxy_container() ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);
    result = dynamic_cast <TAO_Notify_ProxySupplier *> (proxy);
  }
  return result;
}

TAO_END_VERSIONED_NAMESPACE_DECL

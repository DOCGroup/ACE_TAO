// $Id$

#include "ConsumerAdmin.h"

#if ! defined (__ACE_INLINE__)
#include "ConsumerAdmin.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_ConsumerAdmin, "$Id$")

#include "ace/Auto_Ptr.h"
#include "orbsvcs/ESF/ESF_Proxy_Collection.h"
#include "Subscription_Change_Worker.h"
#include "Proxy.h"
#include "EventChannel.h"
#include "Properties.h"
#include "Factory.h"
#include "Builder.h"
#include "Find_Worker_T.h"
#include "Seq_Worker_T.h"

typedef TAO_NS_Find_Worker_T<TAO_NS_Proxy
                             , CosNotifyChannelAdmin::ProxySupplier
                             , CosNotifyChannelAdmin::ProxySupplier_ptr
                             , CosNotifyChannelAdmin::ProxyNotFound>
TAO_NS_ProxySupplier_Find_Worker;

typedef TAO_NS_Seq_Worker_T<TAO_NS_Proxy> TAO_NS_Proxy_Seq_Worker;

TAO_NS_ConsumerAdmin::TAO_NS_ConsumerAdmin (void)
{
}

TAO_NS_ConsumerAdmin::~TAO_NS_ConsumerAdmin ()
{
}

void
TAO_NS_ConsumerAdmin::init (TAO_NS_EventChannel *ec ACE_ENV_ARG_DECL)
{
  TAO_NS_Admin::init (ec ACE_ENV_ARG_PARAMETER);

  const CosNotification::QoSProperties &default_ca_qos =
    TAO_NS_PROPERTIES::instance ()->default_consumer_admin_qos_properties ();

  this->set_qos (default_ca_qos ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_NS_ConsumerAdmin::_add_ref (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  this->_incr_refcnt ();
}

void
TAO_NS_ConsumerAdmin::_remove_ref (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  this->_decr_refcnt ();
}

void
TAO_NS_ConsumerAdmin::release (void)
{
  delete this;
  //@@ inform factory
}

void
TAO_NS_ConsumerAdmin::destroy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  if (this->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER) == 1)
    return;

  ACE_CHECK;

  this->ec_->remove (this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

CosNotifyChannelAdmin::ProxySupplier_ptr
TAO_NS_ConsumerAdmin::obtain_notification_push_supplier (CosNotifyChannelAdmin::ClientType ctype,
                                                         CosNotifyChannelAdmin::ProxyID_out proxy_id
                                                         ACE_ENV_ARG_DECL
                                                         )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotifyChannelAdmin::AdminLimitExceeded
                   ))
{
  CosNotification::QoSProperties initial_qos;

  return TAO_NS_PROPERTIES::instance()->builder()->build_proxy (this
                                                                , ctype
                                                                , proxy_id
                                                                                                                                                                , initial_qos
                                                                ACE_ENV_ARG_PARAMETER);
}

CosNotifyChannelAdmin::ProxySupplier_ptr
TAO_NS_ConsumerAdmin::obtain_notification_push_supplier_with_qos (CosNotifyChannelAdmin::ClientType ctype,
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
  return TAO_NS_PROPERTIES::instance()->builder()->build_proxy (this
                                                                , ctype
                                                                , proxy_id
                                                                , initial_qos
                                                                ACE_ENV_ARG_PARAMETER);
}

CosEventChannelAdmin::ProxyPushSupplier_ptr
TAO_NS_ConsumerAdmin::obtain_push_supplier (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return TAO_NS_PROPERTIES::instance()->builder()->build_proxy (this ACE_ENV_ARG_PARAMETER);
}

CosNotifyChannelAdmin::AdminID
TAO_NS_ConsumerAdmin::MyID (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->id ();
}

CosNotifyChannelAdmin::EventChannel_ptr
TAO_NS_ConsumerAdmin::MyChannel (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->ec_->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
}

::CosNotifyChannelAdmin::InterFilterGroupOperator
TAO_NS_ConsumerAdmin::MyOperator (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->filter_operator_;
}

CosNotifyChannelAdmin::ProxyIDSeq*
TAO_NS_ConsumerAdmin::push_suppliers (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  TAO_NS_Proxy_Seq_Worker seq_worker;

  return seq_worker.create (*this->proxy_container_ ACE_ENV_ARG_PARAMETER);
}

CosNotifyChannelAdmin::ProxySupplier_ptr
TAO_NS_ConsumerAdmin::get_proxy_supplier (CosNotifyChannelAdmin::ProxyID proxy_id ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotifyChannelAdmin::ProxyNotFound
                   ))
{
  TAO_NS_ProxySupplier_Find_Worker find_worker;

  return find_worker.resolve (proxy_id, *this->proxy_container_ ACE_ENV_ARG_PARAMETER);
}

void TAO_NS_ConsumerAdmin::set_qos (const CosNotification::QoSProperties & qos ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotification::UnsupportedQoS
                   ))
{
  this->TAO_NS_Object::set_qos (qos ACE_ENV_ARG_PARAMETER);
}


CosNotification::QoSProperties*
TAO_NS_ConsumerAdmin::get_qos (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->TAO_NS_Object::get_qos (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_NS_ConsumerAdmin::subscription_change (const CosNotification::EventTypeSeq & added,
                                           const CosNotification::EventTypeSeq & removed
                                           ACE_ENV_ARG_DECL
                                           )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotifyComm::InvalidEventType
                   ))
{
  TAO_NS_EventTypeSeq seq_added (added);
  TAO_NS_EventTypeSeq seq_removed (removed);

  {
    ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                        CORBA::INTERNAL ());
    ACE_CHECK;

    this->subscribed_types_.init (seq_added, seq_removed);

    TAO_NS_Subscription_Change_Worker worker (added, removed);

    this->proxy_container_->collection()->for_each (&worker ACE_ENV_ARG_PARAMETER);
  }
}

CosNotifyFilter::FilterID
TAO_NS_ConsumerAdmin::add_filter (CosNotifyFilter::Filter_ptr new_filter ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->filter_admin_.add_filter (new_filter ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_ConsumerAdmin::remove_filter (CosNotifyFilter::FilterID filter ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotifyFilter::FilterNotFound
                   ))
{
  this->filter_admin_.remove_filter (filter ACE_ENV_ARG_PARAMETER);
}

::CosNotifyFilter::Filter_ptr
TAO_NS_ConsumerAdmin::get_filter (CosNotifyFilter::FilterID filter ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotifyFilter::FilterNotFound
                   ))
{
  return this->filter_admin_.get_filter (filter ACE_ENV_ARG_PARAMETER);
}

::CosNotifyFilter::FilterIDSeq*
TAO_NS_ConsumerAdmin::get_all_filters (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->filter_admin_.get_all_filters (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_NS_ConsumerAdmin::remove_all_filters (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  this->filter_admin_.remove_all_filters (ACE_ENV_SINGLE_ARG_PARAMETER);
}

/************ UNIMPLMENTED METHODS *************************/

CosNotifyFilter::MappingFilter_ptr
TAO_NS_ConsumerAdmin::priority_filter (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), CosNotifyFilter::MappingFilter::_nil ());
}

void
TAO_NS_ConsumerAdmin::priority_filter (CosNotifyFilter::MappingFilter_ptr /*priority_filter*/ ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))

{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

CosNotifyFilter::MappingFilter_ptr
TAO_NS_ConsumerAdmin::lifetime_filter (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))

{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), CosNotifyFilter::MappingFilter::_nil ());
}

void
TAO_NS_ConsumerAdmin::lifetime_filter (CosNotifyFilter::MappingFilter_ptr /*lifetime_filter*/ ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

::CosNotifyChannelAdmin::ProxyIDSeq*
TAO_NS_ConsumerAdmin::pull_suppliers (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

CosNotifyChannelAdmin::ProxySupplier_ptr
TAO_NS_ConsumerAdmin::obtain_notification_pull_supplier (CosNotifyChannelAdmin::ClientType /*ctype*/,
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
TAO_NS_ConsumerAdmin::validate_qos (const CosNotification::QoSProperties & /*required_qos*/,
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
TAO_NS_ConsumerAdmin::obtain_pull_supplier (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), CosEventChannelAdmin::ProxyPullSupplier::_nil ());
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_NS_Find_Worker_T<TAO_NS_Proxy
                             , CosNotifyChannelAdmin::ProxySupplier
                             , CosNotifyChannelAdmin::ProxySupplier_ptr
                             , CosNotifyChannelAdmin::ProxyNotFound>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_NS_Find_Worker_T<TAO_NS_Proxy
                             , CosNotifyChannelAdmin::ProxySupplier
                             , CosNotifyChannelAdmin::ProxySupplier_ptr
                             , CosNotifyChannelAdmin::ProxyNotFound>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

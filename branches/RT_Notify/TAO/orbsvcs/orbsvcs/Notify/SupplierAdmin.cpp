// $Id$

#include "SupplierAdmin.h"

#if ! defined (__ACE_INLINE__)
#include "SupplierAdmin.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_SupplierAdmin, "$Id$")

#include "orbsvcs/ESF/ESF_Proxy_Collection.h"
#include "Builder.h"
#include "Proxy.h"
#include "EventChannel.h"
#include "Subscription_Change_Worker.h"
#include "Find_Worker_T.h"
#include "Seq_Worker_T.h"
#include "Properties.h"

typedef TAO_NS_Find_Worker_T<TAO_NS_Proxy
                             , CosNotifyChannelAdmin::ProxyConsumer
                             , CosNotifyChannelAdmin::ProxyConsumer_ptr
                             , CosNotifyChannelAdmin::ProxyNotFound>
TAO_NS_ProxyConsumer_Find_Worker;

typedef TAO_NS_Seq_Worker_T<TAO_NS_Proxy> TAO_NS_Proxy_Seq_Worker;

TAO_NS_SupplierAdmin::TAO_NS_SupplierAdmin (void)
{
}

TAO_NS_SupplierAdmin::~TAO_NS_SupplierAdmin ()
{
}

void
TAO_NS_SupplierAdmin::init (TAO_NS_EventChannel *ec ACE_ENV_ARG_DECL)
{
  TAO_NS_Admin::init (ec ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  const CosNotification::QoSProperties &default_sa_qos =
    TAO_NS_PROPERTIES::instance ()->default_supplier_admin_qos_properties ();

  this->set_qos (default_sa_qos ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_SupplierAdmin::_add_ref (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  this->_incr_refcnt ();
}

void
TAO_NS_SupplierAdmin::_remove_ref (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  this->_decr_refcnt ();
}

void
TAO_NS_SupplierAdmin::release (void)
{
  delete this;
  //@@ inform factory
}

void
TAO_NS_SupplierAdmin::destroy (ACE_ENV_SINGLE_ARG_DECL)
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

void
TAO_NS_SupplierAdmin::set_qos (const CosNotification::QoSProperties & qos ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotification::UnsupportedQoS
                   ))
{
  this->TAO_NS_Object::set_qos (qos ACE_ENV_ARG_PARAMETER);
}

CosNotification::QoSProperties*
TAO_NS_SupplierAdmin::get_qos (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->TAO_NS_Object::get_qos (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CosNotifyChannelAdmin::ProxyConsumer_ptr
TAO_NS_SupplierAdmin::obtain_notification_push_consumer (CosNotifyChannelAdmin::ClientType ctype
                                                         , CosNotifyChannelAdmin::ProxyID_out proxy_id
                                                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotifyChannelAdmin::AdminLimitExceeded
                   ))

{
  return TAO_NS_PROPERTIES::instance()->builder()->build_proxy (this
                                                                , ctype
                                                                , proxy_id
                                                                ACE_ENV_ARG_PARAMETER);
}

CosEventChannelAdmin::ProxyPushConsumer_ptr
TAO_NS_SupplierAdmin::obtain_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return TAO_NS_PROPERTIES::instance()->builder()->build_proxy (this ACE_ENV_ARG_PARAMETER);
}

CosNotifyChannelAdmin::AdminID
TAO_NS_SupplierAdmin::MyID (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->id ();
}

CosNotifyChannelAdmin::EventChannel_ptr
TAO_NS_SupplierAdmin::MyChannel (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->ec_->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
}

::CosNotifyChannelAdmin::InterFilterGroupOperator
TAO_NS_SupplierAdmin::MyOperator (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->filter_operator_;
}

CosNotifyChannelAdmin::ProxyIDSeq*
TAO_NS_SupplierAdmin::push_consumers (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  TAO_NS_Proxy_Seq_Worker seq_worker;

  return seq_worker.create (*this->proxy_container_ ACE_ENV_ARG_PARAMETER);
}

CosNotifyChannelAdmin::ProxyConsumer_ptr
TAO_NS_SupplierAdmin::get_proxy_consumer (CosNotifyChannelAdmin::ProxyID proxy_id ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotifyChannelAdmin::ProxyNotFound
                   ))
{
  TAO_NS_ProxyConsumer_Find_Worker find_worker;

  return find_worker.resolve (proxy_id, *this->proxy_container_ ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_SupplierAdmin::offer_change (const CosNotification::EventTypeSeq & added,
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

    this->proxy_container_->collection ()->for_each (&worker ACE_ENV_ARG_PARAMETER);
  }
}

CosNotifyFilter::FilterID
TAO_NS_SupplierAdmin::add_filter (CosNotifyFilter::Filter_ptr new_filter ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->filter_admin_.add_filter (new_filter ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_SupplierAdmin::remove_filter (CosNotifyFilter::FilterID filter ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotifyFilter::FilterNotFound
                   ))
{
  this->filter_admin_.remove_filter (filter ACE_ENV_ARG_PARAMETER);
}

::CosNotifyFilter::Filter_ptr
TAO_NS_SupplierAdmin::get_filter (CosNotifyFilter::FilterID filter ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotifyFilter::FilterNotFound
                   ))
{
  return this->filter_admin_.get_filter (filter ACE_ENV_ARG_PARAMETER);
}

::CosNotifyFilter::FilterIDSeq*
TAO_NS_SupplierAdmin::get_all_filters (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->filter_admin_.get_all_filters (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_NS_SupplierAdmin::remove_all_filters (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  this->filter_admin_.remove_all_filters (ACE_ENV_SINGLE_ARG_PARAMETER);
}

/************** UNIMPLEMENTED METHODS ***************/

CosEventChannelAdmin::ProxyPullConsumer_ptr
TAO_NS_SupplierAdmin::obtain_pull_consumer (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), CosEventChannelAdmin::ProxyPullConsumer::_nil ());
}

CosNotifyChannelAdmin::ProxyIDSeq*
TAO_NS_SupplierAdmin::pull_consumers (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}


void
TAO_NS_SupplierAdmin::validate_qos (const CosNotification::QoSProperties & /*required_qos*/,
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
TAO_NS_SupplierAdmin::obtain_notification_pull_consumer (CosNotifyChannelAdmin::ClientType /*ctype*/,
                                                         CosNotifyChannelAdmin::ProxyID_out /*proxy_id*/
                                                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotifyChannelAdmin::AdminLimitExceeded
                   ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), CosNotifyChannelAdmin::ProxyConsumer::_nil ());
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_NS_Find_Worker_T<TAO_NS_Proxy
                             , CosNotifyChannelAdmin::ProxyConsumer
                             , CosNotifyChannelAdmin::ProxyConsumer_ptr
                             , CosNotifyChannelAdmin::ProxyNotFound>;
template class TAO_NS_Seq_Worker_T<TAO_NS_Proxy>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_NS_Find_Worker_T<TAO_NS_Proxy
                             , CosNotifyChannelAdmin::ProxyConsumer
                             , CosNotifyChannelAdmin::ProxyConsumer_ptr
                             , CosNotifyChannelAdmin::ProxyNotFound>
#pragma instantiate TAO_NS_Seq_Worker_T<TAO_NS_Proxy>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

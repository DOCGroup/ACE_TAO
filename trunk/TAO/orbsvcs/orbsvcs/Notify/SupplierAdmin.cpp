// $Id$

#include "SupplierAdmin.h"

#if ! defined (__ACE_INLINE__)
#include "SupplierAdmin.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_SupplierAdmin, "$Id$")

#include "orbsvcs/ESF/ESF_Proxy_Collection.h"
#include "Builder.h"
#include "Proxy.h"
#include "Admin.h"
#include "EventChannel.h"
#include "EventChannelFactory.h"
#include "Notify_Service.h"
#include "Subscription_Change_Worker.h"

TAO_NS_SupplierAdmin::TAO_NS_SupplierAdmin (void)
:TAO_NS_Admin (TAO_NS_Admin::SUPPLIER_ADMIN)
{
}

TAO_NS_SupplierAdmin::~TAO_NS_SupplierAdmin ()
{
}

PortableServer::Servant
TAO_NS_SupplierAdmin::servant (void)
{
  return this;
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
  this->inherited::destroy (this ACE_ENV_ARG_PARAMETER);
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
TAO_NS_SupplierAdmin::obtain_notification_push_consumer (CosNotifyChannelAdmin::ClientType ctype, CosNotifyChannelAdmin::ProxyID_out proxy_id  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotifyChannelAdmin::AdminLimitExceeded
                   ))

{
  return TAO_NS_PROPERTIES::instance()->builder ()->build_notification_push_consumer (this,
                                                                                      ctype,
                                                                                      proxy_id ACE_ENV_ARG_PARAMETER);
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
  CORBA::Object_var object = this->parent_->ref (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::EventChannel::_nil ());

  CosNotifyChannelAdmin::EventChannel_var ec_ret;

  ec_ret = CosNotifyChannelAdmin::EventChannel::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::EventChannel::_nil ());

  return ec_ret._retn ();
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
  CosNotifyChannelAdmin::ProxyIDSeq* seq;

  ACE_NEW_THROW_EX (seq,
                    CosNotifyChannelAdmin::ProxyIDSeq (),
                    CORBA::NO_MEMORY ());

  seq->length (this->proxy_id_list_.size ());

  TAO_NS_Object_Id_Seq::CONST_ITERATOR iter (this->proxy_id_list_);

  TAO_NS_Object_Id* object_id;

  CORBA::ULong i = 0;
  for (iter.first (); iter.next (object_id); iter.advance (), ++i)
    seq[i] = *object_id;

  return seq;
}

CosNotifyChannelAdmin::ProxyConsumer_ptr
TAO_NS_SupplierAdmin::get_proxy_consumer (CosNotifyChannelAdmin::ProxyID proxy_id ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotifyChannelAdmin::ProxyNotFound
                   ))
{
  TAO_NS_Object* ns_object = this->find (proxy_id ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::ProxyConsumer::_nil ());

  if (ns_object != 0)
    {
      CORBA::Object_var object = ns_object->ref (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (CosNotifyChannelAdmin::ProxyConsumer::_nil ());

      CosNotifyChannelAdmin::ProxyConsumer_var pc_ret;

      pc_ret = CosNotifyChannelAdmin::ProxyConsumer::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (CosNotifyChannelAdmin::ProxyConsumer::_nil ());

      return pc_ret._retn ();
    }
  else
    ACE_THROW_RETURN (CosNotifyChannelAdmin::ProxyNotFound (), CosNotifyChannelAdmin::ProxyConsumer::_nil ());
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
  }

  TAO_NS_Subscription_Change_Worker worker (added, removed);

  this->collection_->for_each (&worker ACE_ENV_ARG_PARAMETER);
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
  this->filter_admin_.get_all_filters (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CosEventChannelAdmin::ProxyPushConsumer_ptr
TAO_NS_SupplierAdmin::obtain_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return TAO_NS_PROPERTIES::instance()->builder ()->build_push_consumer (this ACE_ENV_ARG_PARAMETER);
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

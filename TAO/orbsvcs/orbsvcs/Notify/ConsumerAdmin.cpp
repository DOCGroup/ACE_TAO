// $Id$

#include "ConsumerAdmin.h"

#if ! defined (__ACE_INLINE__)
#include "ConsumerAdmin.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_ConsumerAdmin, "$Id$")

#include "orbsvcs/ESF/ESF_Proxy_Collection.h"
#include "Builder.h"
#include "Subscription_Change_Worker.h"
#include "Proxy.h"
#include "Admin.h"
#include "EventChannel.h"
#include "EventChannelFactory.h"
#include "Notify_Service.h"

TAO_NS_ConsumerAdmin::TAO_NS_ConsumerAdmin (void)
:TAO_NS_Admin (TAO_NS_Admin::CONSUMER_ADMIN)
{
}

TAO_NS_ConsumerAdmin::~TAO_NS_ConsumerAdmin ()
{
}

PortableServer::Servant
TAO_NS_ConsumerAdmin::servant (void)
{
  return this;
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
  this->inherited::destroy (this ACE_ENV_ARG_PARAMETER);
}

::CosNotifyChannelAdmin::ProxySupplier_ptr
TAO_NS_ConsumerAdmin::obtain_notification_push_supplier (CosNotifyChannelAdmin::ClientType ctype,
                                                         CosNotifyChannelAdmin::ProxyID_out proxy_id
                                                         ACE_ENV_ARG_DECL
                                                         )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotifyChannelAdmin::AdminLimitExceeded
                   ))
{
  return TAO_NS_PROPERTIES::instance()->builder ()->build_notification_push_supplier (this,
                                                                                      ctype,
                                                                                      proxy_id ACE_ENV_ARG_PARAMETER);
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
  CORBA::Object_var object = this->parent_->ref (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::EventChannel::_nil ());

  return CosNotifyChannelAdmin::EventChannel::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
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

CosNotifyChannelAdmin::ProxySupplier_ptr
TAO_NS_ConsumerAdmin::get_proxy_supplier (CosNotifyChannelAdmin::ProxyID proxy_id ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotifyChannelAdmin::ProxyNotFound
                   ))
{
  TAO_NS_Object* ns_object = this->find (proxy_id ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::ProxySupplier::_nil ());

  if (ns_object != 0)
    {
      CORBA::Object_var object = ns_object->ref (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (CosNotifyChannelAdmin::ProxySupplier::_nil ());

      return CosNotifyChannelAdmin::ProxySupplier::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
    }
  else
    ACE_THROW_RETURN (CosNotifyChannelAdmin::ProxyNotFound (), CosNotifyChannelAdmin::ProxySupplier::_nil ());
}

CosEventChannelAdmin::ProxyPushSupplier_ptr
TAO_NS_ConsumerAdmin::obtain_push_supplier (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return TAO_NS_PROPERTIES::instance()->builder ()->build_push_supplier (this ACE_ENV_ARG_PARAMETER);
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
  }

  TAO_NS_Subscription_Change_Worker worker (added, removed);

  this->collection_->for_each (&worker ACE_ENV_ARG_PARAMETER);
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
  this->filter_admin_.get_all_filters (ACE_ENV_SINGLE_ARG_PARAMETER);
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

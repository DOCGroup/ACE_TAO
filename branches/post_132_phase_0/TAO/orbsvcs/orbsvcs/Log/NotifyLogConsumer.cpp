#include "orbsvcs/Log/NotifyLog_i.h"
#include "orbsvcs/Log/LogMgr_i.h"
#include "orbsvcs/Log/LogNotification.h"

ACE_RCSID (Log,
           NotifyLogConsumer,
           "$Id$")



TAO_Notify_LogConsumer::TAO_Notify_LogConsumer (TAO_NotifyLog_i *log)
: log_ (log)
{
  // No-Op.
}

TAO_Notify_LogConsumer::~TAO_Notify_LogConsumer (void)
{
  // No-Op.
}

void
TAO_Notify_LogConsumer::connect (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin ACE_ENV_ARG_DECL)
{
  // Activate the consumer with the default_POA_
  CosNotifyComm::StructuredPushConsumer_var objref =
    this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CosNotifyChannelAdmin::ProxySupplier_var proxysupplier =
    consumer_admin->obtain_notification_push_supplier (CosNotifyChannelAdmin::STRUCTURED_EVENT, proxy_supplier_id_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (proxysupplier.in ()));

  // narrow
  this->proxy_supplier_ =
    CosNotifyChannelAdmin::StructuredProxyPushSupplier::
    _narrow (proxysupplier.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (proxy_supplier_.in ()));

  proxy_supplier_->connect_structured_push_consumer (objref.in ()
                                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

}

void
TAO_Notify_LogConsumer::disconnect (ACE_ENV_SINGLE_ARG_DECL)
{
  this->proxy_supplier_->
    disconnect_structured_push_supplier(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_Notify_LogConsumer::offer_change
   (const CosNotification::EventTypeSeq & /*added*/,
    const CosNotification::EventTypeSeq & /*removed*/
    ACE_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosNotifyComm::InvalidEventType
      ))
{
  // No-Op.
}

void
TAO_Notify_LogConsumer::push_structured_event
   (const CosNotification::StructuredEvent & notification
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosEventComm::Disconnected
                   ))
{
  CORBA::Long val;

  notification.remainder_of_body >>= val;

  CORBA::Any any;

  any <<= val;

  // create a record list...
  DsLogAdmin::RecordList recList (1);
  recList.length (1);

  recList [0].info = any;

  this->log_->write_recordlist (recList ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_Notify_LogConsumer::disconnect_structured_push_consumer
   (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  // No-Op.
}


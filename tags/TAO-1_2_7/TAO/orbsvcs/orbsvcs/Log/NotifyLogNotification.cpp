#include "orbsvcs/Log/LogNotification.h"
#include "orbsvcs/Log/NotifyLogNotification.h"
#include "orbsvcs/Time_Utilities.h"
#include "tao/debug.h"

ACE_RCSID (Log,
           NotifyLogNotification,
           "$Id$")


TAO_NotifyLogNotification::TAO_NotifyLogNotification (CosNotifyChannelAdmin::EventChannel_ptr ec)
: TAO_LogNotification (), event_channel_ (CosNotifyChannelAdmin::EventChannel::_duplicate (ec))
{
  ACE_DECLARE_NEW_CORBA_ENV;
  
  CosNotifyComm::PushSupplier_var objref =
    this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  ACE_ASSERT (!CORBA::is_nil (objref.in ()));

  CosNotifyChannelAdmin::AdminID adminID;
  CosNotifyChannelAdmin::SupplierAdmin_var supplier_admin = 
    this->event_channel_->new_for_suppliers (CosNotifyChannelAdmin::OR_OP, adminID);

  ACE_ASSERT (!CORBA::is_nil (supplier_admin.in ()));

  CosNotifyChannelAdmin::ProxyConsumer_var proxyconsumer =
    supplier_admin->obtain_notification_push_consumer (CosNotifyChannelAdmin::ANY_EVENT, proxy_consumer_id_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (proxyconsumer.in ()));

  // narrow
  this->proxy_consumer_ =
    CosNotifyChannelAdmin::ProxyPushConsumer::_narrow (proxyconsumer.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (this->proxy_consumer_.in ()));

  proxy_consumer_->connect_any_push_supplier (objref.in ()
                                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

TAO_NotifyLogNotification::~TAO_NotifyLogNotification (void)
{
  // No-Op.
}

void
TAO_NotifyLogNotification::send_notification (const CORBA::Any& any)
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{ 
  this->proxy_consumer_->push (any);
}

void
TAO_NotifyLogNotification::subscription_change
   (const CosNotification::EventTypeSeq & /*added*/,
    const CosNotification::EventTypeSeq & /*removed */
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyComm::InvalidEventType
                   ))
{
  //No-Op.
}

void
TAO_NotifyLogNotification::disconnect_push_supplier
   (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_ASSERT (!CORBA::is_nil (this->proxy_consumer_.in ()));

  this->proxy_consumer_->disconnect_push_consumer(ACE_ENV_SINGLE_ARG_PARAMETER);
}

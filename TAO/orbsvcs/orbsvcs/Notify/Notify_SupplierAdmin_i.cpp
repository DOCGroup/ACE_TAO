/* -*- C++ -*- $Id$ */

#include "ace/Auto_Ptr.h"
#include "orbsvcs/orbsvcs/Notify/Notify_SupplierAdmin_i.h"
#include "orbsvcs/orbsvcs/Notify/Notify_ProxyPushConsumer_i.h"
#include "Notify_EventChannel_i.h"

// Implementation skeleton constructor
TAO_Notify_SupplierAdmin_i::TAO_Notify_SupplierAdmin_i (TAO_Notify_EventChannel_i& myChannel)
  :myChannel_ (myChannel)
{
}

// Implementation skeleton destructor
TAO_Notify_SupplierAdmin_i::~TAO_Notify_SupplierAdmin_i (void)
{
}

TAO_Notify_Dispatcher&
TAO_Notify_SupplierAdmin_i::get_dispatcher (void)
{
  return *dispatcher_;
}

void
TAO_Notify_SupplierAdmin_i::init (CORBA::Environment &ACE_TRY_ENV)
{

  dispatcher_ = auto_ptr<TAO_Notify_Dispatcher>(TAO_Notify_Dispatcher::create (ACE_TRY_ENV));
  ACE_CHECK;

  dispatcher_->add_dispatcher (myChannel_.get_dispatcher ());
}

CosNotifyChannelAdmin::SupplierAdmin_ptr
TAO_Notify_SupplierAdmin_i::get_ref (CORBA::Environment &ACE_TRY_ENV)
{
  return _this (ACE_TRY_ENV);
}

CosNotifyChannelAdmin::AdminID TAO_Notify_SupplierAdmin_i::MyID (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
  {
    //Add your implementation here
    return 0;
  }

CosNotifyChannelAdmin::EventChannel_ptr TAO_Notify_SupplierAdmin_i::MyChannel (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
  {
    //Add your implementation here
    return 0;
  }

CosNotifyChannelAdmin::InterFilterGroupOperator TAO_Notify_SupplierAdmin_i::MyOperator (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
  {
    //Add your implementation here
    return myOperator_;
  }

CosNotifyChannelAdmin::ProxyIDSeq * TAO_Notify_SupplierAdmin_i::pull_consumers (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
  {
    //Add your implementation here
    return 0;
  }

CosNotifyChannelAdmin::ProxyIDSeq * TAO_Notify_SupplierAdmin_i::push_consumers (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
  {
    //Add your implementation here
    return 0;
  }

CosNotifyChannelAdmin::ProxyConsumer_ptr TAO_Notify_SupplierAdmin_i::get_proxy_consumer (
    CosNotifyChannelAdmin::ProxyID proxy_id,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::ProxyNotFound
  ))
  {
    //Add your implementation here
    return 0;
  }

CosNotifyChannelAdmin::ProxyConsumer_ptr TAO_Notify_SupplierAdmin_i::obtain_notification_pull_consumer (
    CosNotifyChannelAdmin::ClientType ctype,
    CosNotifyChannelAdmin::ProxyID_out proxy_id,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::AdminLimitExceeded
  ))

  {
    //Add your implementation here
    return 0;
  }

CosNotifyChannelAdmin::ProxyConsumer_ptr
TAO_Notify_SupplierAdmin_i::obtain_notification_push_consumer (
    CosNotifyChannelAdmin::ClientType ctype,
    CosNotifyChannelAdmin::ProxyID_out proxy_id,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::AdminLimitExceeded
  ))
  {
    //Add your implementation here
    CosNotifyChannelAdmin::ProxyConsumer_var proxyconsumer_ret;
    CORBA::Object_var obj;

    switch (ctype)
      {
      case CosNotifyChannelAdmin::ANY_EVENT:
        {
          TAO_Notify_ProxyPushConsumer_i* proxypushconsumer;
          ACE_NEW_THROW_EX (proxypushconsumer,
                            TAO_Notify_ProxyPushConsumer_i (*this),
                            CORBA::NO_MEMORY ());
          auto_ptr <TAO_Notify_ProxyPushConsumer_i> auto_proxypushcons;

          obj = proxypushconsumer->get_ref (ACE_TRY_ENV);
          ACE_CHECK_RETURN (CosNotifyChannelAdmin::ProxyConsumer::_nil ());

          proxyconsumer_ret =
            CosNotifyChannelAdmin::ProxyConsumer::_narrow (obj.in (),
                                                           ACE_TRY_ENV);
          ACE_CHECK_RETURN (CosNotifyChannelAdmin::ProxyConsumer::_nil ());
          obj._retn (); // giveup ownership.

          proxy_id = proxy_consumer_ids.get ();

          if (proxypushconsumer_map_.bind (proxy_id,
                                           proxypushconsumer) == -1)
            ACE_THROW_RETURN (CORBA::INTERNAL (),
                              CosNotifyChannelAdmin::ProxyConsumer::_nil ());
          auto_proxypushcons.release ();
        }
        break;
      case CosNotifyChannelAdmin::STRUCTURED_EVENT:
        break;
      case CosNotifyChannelAdmin::SEQUENCE_EVENT:
        break;
      default:
        break;
      }

    return proxyconsumer_ret._retn ();
  }

void TAO_Notify_SupplierAdmin_i::destroy (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
  {
    //Add your implementation here
  }

CosEventChannelAdmin::ProxyPushConsumer_ptr
TAO_Notify_SupplierAdmin_i::obtain_push_consumer (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  // Add your implementation here
  return 0;
}

CosEventChannelAdmin::ProxyPullConsumer_ptr
TAO_Notify_SupplierAdmin_i::obtain_pull_consumer (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  //Add your implementation here
  return 0;
}

/* -*- C++ -*- $Id$ */

#include "orbsvcs/orbsvcs/Notify/Notify_ConsumerAdmin_i.h"
#include "orbsvcs/orbsvcs/Notify/Notify_ProxyPushSupplier_i.h"
#include "ace/Auto_Ptr.h"

// Implementation skeleton constructor
TAO_Notify_ConsumerAdmin_i::TAO_Notify_ConsumerAdmin_i (TAO_Notify_EventChannel_i& myChannel)
  :myChannel_ (myChannel)
{
}

// Implementation skeleton destructor
TAO_Notify_ConsumerAdmin_i::~TAO_Notify_ConsumerAdmin_i (void)
{
}

void
TAO_Notify_ConsumerAdmin_i::init (CORBA::Environment &ACE_TRY_ENV)
{
  dispatcher_ = auto_ptr<TAO_Notify_Dispatcher>(TAO_Notify_Dispatcher::create (ACE_TRY_ENV));
  ACE_CHECK;
}

TAO_Notify_Dispatcher&
TAO_Notify_ConsumerAdmin_i::get_dispatcher (void)
{
  return *dispatcher_;
}

CosNotifyChannelAdmin::ConsumerAdmin_ptr
TAO_Notify_ConsumerAdmin_i::get_ref (CORBA::Environment &ACE_TRY_ENV)
{
  CosNotifyChannelAdmin::ConsumerAdmin_var consumeradmin =
    _this (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::ConsumerAdmin::_nil ());

  return consumeradmin._retn ();
}

CosNotifyChannelAdmin::AdminID TAO_Notify_ConsumerAdmin_i::MyID (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))

  {
    //Add your implementation here
    return 0;
  }

CosNotifyChannelAdmin::EventChannel_ptr TAO_Notify_ConsumerAdmin_i::MyChannel (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))

  {
    //Add your implementation here
    return 0;
  }

CosNotifyChannelAdmin::InterFilterGroupOperator TAO_Notify_ConsumerAdmin_i::MyOperator (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))

  {
    //Add your implementation here
    return myOperator_;
  }

CosNotifyFilter::MappingFilter_ptr TAO_Notify_ConsumerAdmin_i::priority_filter (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))

  {
    //Add your implementation here
    return 0;
  }

void TAO_Notify_ConsumerAdmin_i::priority_filter (
    CosNotifyFilter::MappingFilter_ptr priority_filter,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))

  {
    //Add your implementation here
  }

CosNotifyFilter::MappingFilter_ptr TAO_Notify_ConsumerAdmin_i::lifetime_filter (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))

  {
    //Add your implementation here
    return 0;
  }

void TAO_Notify_ConsumerAdmin_i::lifetime_filter (
    CosNotifyFilter::MappingFilter_ptr lifetime_filter,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))

  {
    //Add your implementation here
  }

CosNotifyChannelAdmin::ProxyIDSeq * TAO_Notify_ConsumerAdmin_i::pull_suppliers (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))

  {
    //Add your implementation here
    return 0;
  }

CosNotifyChannelAdmin::ProxyIDSeq* TAO_Notify_ConsumerAdmin_i::push_suppliers (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))

  {
    //Add your implementation here
    return 0;
  }

CosNotifyChannelAdmin::ProxySupplier_ptr TAO_Notify_ConsumerAdmin_i::get_proxy_supplier (
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

CosNotifyChannelAdmin::ProxySupplier_ptr TAO_Notify_ConsumerAdmin_i::obtain_notification_pull_supplier (
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

CosNotifyChannelAdmin::ProxySupplier_ptr
TAO_Notify_ConsumerAdmin_i::obtain_notification_push_supplier (
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
    CosNotifyChannelAdmin::ProxySupplier_var proxysupplier_ret;
    CORBA::Object_var obj;

    switch (ctype)
      {
      case CosNotifyChannelAdmin::ANY_EVENT:
        {
          TAO_Notify_ProxyPushSupplier_i* proxypushsupplier;
          ACE_NEW_THROW_EX (proxypushsupplier,
                            TAO_Notify_ProxyPushSupplier_i (*this),
                            CORBA::NO_MEMORY ());
          auto_ptr <TAO_Notify_ProxyPushSupplier_i>
            auto_proxypushsupp (proxypushsupplier);

          proxypushsupplier->init (ACE_TRY_ENV);
          ACE_CHECK_RETURN (CosNotifyChannelAdmin::ProxySupplier::_nil ());

          dispatcher_->add_dispatcher (*proxypushsupplier);
          // @@ check ret val.

          obj = proxypushsupplier->get_ref (ACE_TRY_ENV);
          ACE_CHECK_RETURN (CosNotifyChannelAdmin::ProxySupplier::_nil ());

          proxysupplier_ret =
            CosNotifyChannelAdmin::ProxySupplier::_narrow (obj.in (),
                                                           ACE_TRY_ENV);
          ACE_CHECK_RETURN (CosNotifyChannelAdmin::ProxySupplier::_nil ());
          obj._retn (); // giveup ownership.

          proxy_id = proxy_supplier_ids.get ();

          if (proxypushsupplier_map_.bind (proxy_id,
                                           proxypushsupplier) == -1)
            ACE_THROW_RETURN (CORBA::INTERNAL (),
                              CosNotifyChannelAdmin::ProxySupplier::_nil ());
          auto_proxypushsupp.release ();
        }
        break;
      case CosNotifyChannelAdmin::STRUCTURED_EVENT:
        break;
      case CosNotifyChannelAdmin::SEQUENCE_EVENT:
        break;
      default:
        break;
      }

    return proxysupplier_ret._retn ();
  }

void TAO_Notify_ConsumerAdmin_i::destroy (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))

  {
    //Add your implementation here
  }

CosEventChannelAdmin::ProxyPushSupplier_ptr
TAO_Notify_ConsumerAdmin_i::obtain_push_supplier (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  return 0;
}

CosEventChannelAdmin::ProxyPullSupplier_ptr
TAO_Notify_ConsumerAdmin_i::obtain_pull_supplier (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  return 0;
}

/* -*- C++ -*- $Id$ */

#include "ace/Auto_Ptr.h"
#include "Notify_SupplierAdmin_i.h"
#include "Notify_ProxyPushConsumer_i.h"
#include "Notify_StructuredProxyPushConsumer_i.h"
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
  // ???? Pradeep: auto_ptr can't be used this way.
  // dispatcher_ =
  //  auto_ptr<TAO_Notify_Dispatcher>(TAO_Notify_Dispatcher::
  //                                  create (ACE_TRY_ENV));
  dispatcher_ = TAO_Notify_Dispatcher::create (ACE_TRY_ENV);
  ACE_CHECK;

  dispatcher_->set_FilterAdmin (this);
  dispatcher_->add_dispatcher (myChannel_.get_dispatcher ());
}

CosNotifyChannelAdmin::SupplierAdmin_ptr
TAO_Notify_SupplierAdmin_i::get_ref (CORBA::Environment &ACE_TRY_ENV)
{
  return _this (ACE_TRY_ENV);
}

CosNotifyChannelAdmin::AdminID TAO_Notify_SupplierAdmin_i::MyID (
    CORBA::Environment & /* ACE_TRY_ENV */
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
  {
    //Add your implementation here
    return 0;
  }

CosNotifyChannelAdmin::EventChannel_ptr
TAO_Notify_SupplierAdmin_i::MyChannel (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
  {
    CosNotifyChannelAdmin::EventChannel_var ec =
      myChannel_.get_ref (ACE_TRY_ENV);
    ACE_CHECK_RETURN (0);

    return ec._retn ();
  }

CosNotifyChannelAdmin::InterFilterGroupOperator TAO_Notify_SupplierAdmin_i::MyOperator (
    CORBA::Environment & /* ACE_TRY_ENV */
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
  {
    //Add your implementation here
    return myOperator_;
  }

CosNotifyChannelAdmin::ProxyIDSeq * TAO_Notify_SupplierAdmin_i::pull_consumers (
    CORBA::Environment & /* ACE_TRY_ENV */
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
  {
    //Add your implementation here
    return 0;
  }

CosNotifyChannelAdmin::ProxyIDSeq * TAO_Notify_SupplierAdmin_i::push_consumers (
    CORBA::Environment & /* ACE_TRY_ENV */
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
  {
    //Add your implementation here
    return 0;
  }

CosNotifyChannelAdmin::ProxyConsumer_ptr TAO_Notify_SupplierAdmin_i::get_proxy_consumer (
    CosNotifyChannelAdmin::ProxyID /* proxy_id */,
    CORBA::Environment & /* ACE_TRY_ENV */
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
    CosNotifyChannelAdmin::ClientType /* ctype */,
    CosNotifyChannelAdmin::ProxyID_out /* proxy_id */,
    CORBA::Environment & /* ACE_TRY_ENV */
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
  TAO_Notify_ProxyConsumer_i* proxyconsumer;

  switch (ctype)
    {
    case CosNotifyChannelAdmin::ANY_EVENT:
      {
        ACE_NEW_THROW_EX (proxyconsumer,
                          TAO_Notify_ProxyPushConsumer_i (*this),
                          CORBA::NO_MEMORY ());
      }
      break;
    case CosNotifyChannelAdmin::STRUCTURED_EVENT:
      {
        ACE_NEW_THROW_EX (proxyconsumer,
                          TAO_Notify_StructuredProxyPushConsumer_i (*this),
                          CORBA::NO_MEMORY ());
      }
      break;

    case CosNotifyChannelAdmin::SEQUENCE_EVENT:
    default:
      ACE_THROW_RETURN (CORBA::BAD_PARAM (),
                        CosNotifyChannelAdmin::ProxyConsumer::_nil ());
    }

    CosNotifyChannelAdmin::ProxyConsumer_var proxyconsumer_ret;
    CORBA::Object_var obj;

    auto_ptr <TAO_Notify_ProxyConsumer_i> auto_proxycons (proxyconsumer);

    obj = proxyconsumer->get_ref (ACE_TRY_ENV);
    ACE_CHECK_RETURN (CosNotifyChannelAdmin::ProxyConsumer::_nil ());

    proxyconsumer_ret =
      CosNotifyChannelAdmin::ProxyConsumer::_narrow (obj.in (),
                                                     ACE_TRY_ENV);
    ACE_CHECK_RETURN (CosNotifyChannelAdmin::ProxyConsumer::_nil ());
    obj._retn (); // giveup ownership.

    proxy_id = proxy_consumer_ids.get ();

    if (proxyconsumer_map_.bind (proxy_id,
                                 proxyconsumer) == -1)
      ACE_THROW_RETURN (CORBA::INTERNAL (),
                        CosNotifyChannelAdmin::ProxyConsumer::_nil ());
    auto_proxycons.release ();

    return proxyconsumer_ret._retn ();
}

void TAO_Notify_SupplierAdmin_i::destroy (
    CORBA::Environment & /* ACE_TRY_ENV */
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
  {
    //Add your implementation here
  }

CosEventChannelAdmin::ProxyPushConsumer_ptr
TAO_Notify_SupplierAdmin_i::obtain_push_consumer (
                                                  CORBA::Environment & /*ACE_TRY_ENV*/
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
    CORBA::Environment & /*ACE_TRY_ENV*/
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  //Add your implementation here
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Hash_Map_Entry<CosNotifyChannelAdmin::ProxyID, TAO_Notify_ProxyConsumer_i*>;
template class ACE_Hash_Map_Manager<CosNotifyChannelAdmin::ProxyID, TAO_Notify_ProxyConsumer_i*, ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Manager_Ex<CosNotifyChannelAdmin::ProxyID, TAO_Notify_ProxyConsumer_i*, ACE_Hash<CosNotifyChannelAdmin::ProxyID>,ACE_Equal_To<CosNotifyChannelAdmin::ProxyID>,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator<CosNotifyChannelAdmin::ProxyID, TAO_Notify_ProxyConsumer_i*, ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Ex<CosNotifyChannelAdmin::ProxyID, TAO_Notify_ProxyConsumer_i*, ACE_Hash<CosNotifyChannelAdmin::ProxyID>,ACE_Equal_To<CosNotifyChannelAdmin::ProxyID>,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Base_Ex<CosNotifyChannelAdmin::ProxyID, TAO_Notify_ProxyConsumer_i*, ACE_Hash<CosNotifyChannelAdmin::ProxyID>,ACE_Equal_To<CosNotifyChannelAdmin::ProxyID>,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator<CosNotifyChannelAdmin::ProxyID, TAO_Notify_ProxyConsumer_i*, ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<CosNotifyChannelAdmin::ProxyID, TAO_Notify_ProxyConsumer_i*, ACE_Hash<CosNotifyChannelAdmin::ProxyID>,ACE_Equal_To<CosNotifyChannelAdmin::ProxyID>,ACE_SYNCH_MUTEX>;

//template class ID_Pool<CosNotifyChannelAdmin::ProxyID>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash_Map_Entry<CosNotifyChannelAdmin::ProxyID, TAO_Notify_ProxyConsumer_i*, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Manager<CosNotifyChannelAdmin::ProxyID, TAO_Notify_ProxyConsumer_i*, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Manager_Ex<CosNotifyChannelAdmin::ProxyID, TAO_Notify_ProxyConsumer_i*, ACE_Hash<CosNotifyChannelAdmin::ProxyID>,ACE_Equal_To<CosNotifyChannelAdmin::ProxyID>,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator<CosNotifyChannelAdmin::ProxyID, TAO_Notify_ProxyConsumer_i*, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<CosNotifyChannelAdmin::ProxyID, TAO_Notify_ProxyConsumer_i*, ACE_Hash<CosNotifyChannelAdmin::ProxyID>,ACE_Equal_To<CosNotifyChannelAdmin::ProxyID>,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<CosNotifyChannelAdmin::ProxyID, TAO_Notify_ProxyConsumer_i*, ACE_Hash<CosNotifyChannelAdmin::ProxyID>,ACE_Equal_To<CosNotifyChannelAdmin::ProxyID>,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<CosNotifyChannelAdmin::ProxyID, TAO_Notify_ProxyConsumer_i*, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<CosNotifyChannelAdmin::ProxyID, TAO_Notify_ProxyConsumer_i*, ACE_Hash<CosNotifyChannelAdmin::ProxyID>,ACE_Equal_To<CosNotifyChannelAdmin::ProxyID>,ACE_SYNCH_MUTEX>

//#pragma instantiate ID_Pool<CosNotifyChannelAdmin::ProxyID>
#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

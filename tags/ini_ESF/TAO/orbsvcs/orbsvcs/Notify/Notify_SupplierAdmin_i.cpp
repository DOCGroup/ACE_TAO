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
  delete dispatcher_;
}

TAO_Notify_Dispatcher&
TAO_Notify_SupplierAdmin_i::get_dispatcher (void)
{
  return *dispatcher_;
}

void
TAO_Notify_SupplierAdmin_i::init (CosNotifyChannelAdmin::AdminID myID,
                                  CosNotifyChannelAdmin::InterFilterGroupOperator myOperator,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  myID_ = myID;
  myOperator_ = myOperator;

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

CosNotifyChannelAdmin::AdminID
TAO_Notify_SupplierAdmin_i::MyID (
                                  CORBA::Environment & /* ACE_TRY_ENV */
                                  )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return myID_;
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

CosNotifyChannelAdmin::InterFilterGroupOperator
TAO_Notify_SupplierAdmin_i::MyOperator (
                                        CORBA::Environment& /* ACE_TRY_ENV */
                                        )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return myOperator_;
}

CosNotifyChannelAdmin::ProxyIDSeq*
TAO_Notify_SupplierAdmin_i::pull_consumers (
    CORBA::Environment& ACE_TRY_ENV
    )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    0);
}

CosNotifyChannelAdmin::ProxyIDSeq*
TAO_Notify_SupplierAdmin_i::push_consumers (CORBA::Environment& ACE_TRY_ENV
                                            )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  CosNotifyChannelAdmin::ProxyIDSeq* list;

  // Figure out length of the list.
  CORBA::ULong len = proxyconsumer_map_.current_size ();

  // Allocate the list of <len> length.
  ACE_NEW_THROW_EX (list,
                    CosNotifyChannelAdmin::ProxyIDSeq (len),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  list->length (len);

  // Create an iterator
  PROXYCONSUMER_MAP::ITERATOR iter (proxyconsumer_map_);

  // Iterate over and populate the list.
  PROXYCONSUMER_MAP::ENTRY *hash_entry;

  for (CORBA::ULong i = 0; i < len; i++)
    {
      iter.next (hash_entry);
      iter.advance ();

      (*list)[i] =
        hash_entry->ext_id_;
    }

  return list;
}

CosNotifyChannelAdmin::ProxyConsumer_ptr
TAO_Notify_SupplierAdmin_i::get_proxy_consumer (
    CosNotifyChannelAdmin::ProxyID proxy_id,
    CORBA::Environment& ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::ProxyNotFound
  ))
{
  TAO_Notify_ProxyConsumer_i* proxy;

  if (proxyconsumer_map_.find (proxy_id,
                               proxy) == -1)
    ACE_THROW_RETURN (CosNotifyChannelAdmin::ProxyNotFound (),
                      CosNotifyChannelAdmin::ProxyConsumer::_nil ());

  return proxy->get_ref (ACE_TRY_ENV);
}

CosNotifyChannelAdmin::ProxyConsumer_ptr
TAO_Notify_SupplierAdmin_i::obtain_notification_pull_consumer (
    CosNotifyChannelAdmin::ClientType /* ctype */,
    CosNotifyChannelAdmin::ProxyID_out /* proxy_id */,
    CORBA::Environment& ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::AdminLimitExceeded
  ))

{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    0);
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
      {
        proxy_consumer_ids.put (proxy_id);
        ACE_THROW_RETURN (CORBA::INTERNAL (),
                          CosNotifyChannelAdmin::ProxyConsumer::_nil ());
      }

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
                                                  CORBA::Environment& ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  TAO_Notify_ProxyConsumer_i* proxyconsumer;

  ACE_NEW_THROW_EX (proxyconsumer,
                    TAO_Notify_ProxyPushConsumer_i (*this),
                    CORBA::NO_MEMORY ());

  CosEventChannelAdmin::ProxyPushConsumer_var proxyconsumer_ret;
  CORBA::Object_var obj;

  auto_ptr <TAO_Notify_ProxyConsumer_i> auto_proxycons (proxyconsumer);

  obj = proxyconsumer->get_ref (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosEventChannelAdmin::ProxyPushConsumer::_nil ());

  proxyconsumer_ret =
    CosEventChannelAdmin::ProxyPushConsumer::_narrow (obj.in (),
                                                  ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosEventChannelAdmin::ProxyPushConsumer::_nil ());
  obj._retn (); // giveup ownership.

  auto_proxycons.release ();

  return proxyconsumer_ret._retn ();
}

CosEventChannelAdmin::ProxyPullConsumer_ptr
TAO_Notify_SupplierAdmin_i::obtain_pull_consumer (
    CORBA::Environment& ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    0);
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

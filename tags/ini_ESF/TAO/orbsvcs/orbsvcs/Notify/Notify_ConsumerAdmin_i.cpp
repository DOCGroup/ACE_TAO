/* -*- C++ -*- $Id$ */

#include "ace/Auto_Ptr.h"
#include "Notify_ConsumerAdmin_i.h"
#include "Notify_ProxyPushSupplier_i.h"
#include "Notify_StructuredProxyPushSupplier_i.h"
#include "Notify_EventChannel_i.h"

// Implementation skeleton constructor
TAO_Notify_ConsumerAdmin_i::TAO_Notify_ConsumerAdmin_i (TAO_Notify_EventChannel_i& myChannel)
  :myChannel_ (myChannel)
{
}

// Implementation skeleton destructor
TAO_Notify_ConsumerAdmin_i::~TAO_Notify_ConsumerAdmin_i (void)
{
  delete dispatcher_;
}

void
TAO_Notify_ConsumerAdmin_i::init (CosNotifyChannelAdmin::AdminID myID,
                                  CosNotifyChannelAdmin::InterFilterGroupOperator myOperator,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  myID_ = myID;
  myOperator_ = myOperator;

  dispatcher_ = TAO_Notify_Dispatcher::create (ACE_TRY_ENV);
  ACE_CHECK;
  dispatcher_->set_FilterAdmin (this);
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

CosNotifyChannelAdmin::AdminID
TAO_Notify_ConsumerAdmin_i::MyID (
    CORBA::Environment & /* ACE_TRY_ENV */
    )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return myID_;
}

CosNotifyChannelAdmin::EventChannel_ptr
TAO_Notify_ConsumerAdmin_i::MyChannel (
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
TAO_Notify_ConsumerAdmin_i::MyOperator (
    CORBA::Environment & /* ACE_TRY_ENV */
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  return myOperator_;
}

CosNotifyFilter::MappingFilter_ptr
TAO_Notify_ConsumerAdmin_i::priority_filter (
                                             CORBA::Environment & /* ACE_TRY_ENV */
                                             )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    ))
{
  //Add your implementation here
  return 0;
}

void TAO_Notify_ConsumerAdmin_i::priority_filter (
    CosNotifyFilter::MappingFilter_ptr /* priority_filter */,
    CORBA::Environment & /* ACE_TRY_ENV */
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  //Add your implementation here
}

CosNotifyFilter::MappingFilter_ptr
TAO_Notify_ConsumerAdmin_i::lifetime_filter (
    CORBA::Environment & /* ACE_TRY_ENV */
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  //Add your implementation here
  return 0;
}

void TAO_Notify_ConsumerAdmin_i::lifetime_filter (
    CosNotifyFilter::MappingFilter_ptr /* lifetime_filter */,
    CORBA::Environment & /* ACE_TRY_ENV */
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  //Add your implementation here
}

CosNotifyChannelAdmin::ProxyIDSeq*
TAO_Notify_ConsumerAdmin_i::pull_suppliers (CORBA::Environment& ACE_TRY_ENV
                                            )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    0);
}

CosNotifyChannelAdmin::ProxyIDSeq*
TAO_Notify_ConsumerAdmin_i::push_suppliers (CORBA::Environment& ACE_TRY_ENV
                                            )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  CosNotifyChannelAdmin::ProxyIDSeq* list;

  // Figure out length of the list.
  CORBA::ULong len = proxysupplier_map_.current_size ();

  // Allocate the list of <len> length.
  ACE_NEW_THROW_EX (list,
                    CosNotifyChannelAdmin::ProxyIDSeq (len),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  list->length (len);

  // Create an iterator
  PROXYSUPPLIER_MAP::ITERATOR iter (proxysupplier_map_);

  // Iterate over and populate the list.
  PROXYSUPPLIER_MAP::ENTRY *hash_entry;

  for (CORBA::ULong i = 0; i < len; i++)
    {
      iter.next (hash_entry);
      iter.advance ();

      (*list)[i] =
        hash_entry->ext_id_;
    }

  return list;
}

CosNotifyChannelAdmin::ProxySupplier_ptr
TAO_Notify_ConsumerAdmin_i::get_proxy_supplier (
    CosNotifyChannelAdmin::ProxyID proxy_id,
    CORBA::Environment& ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::ProxyNotFound
  ))
{
  TAO_Notify_ProxySupplier_i* proxy;

  if (proxysupplier_map_.find (proxy_id,
                               proxy) == -1)
    ACE_THROW_RETURN (CosNotifyChannelAdmin::ProxyNotFound (),
                      CosNotifyChannelAdmin::ProxySupplier::_nil ());

  return proxy->get_ref (ACE_TRY_ENV);
}

CosNotifyChannelAdmin::ProxySupplier_ptr
TAO_Notify_ConsumerAdmin_i::obtain_notification_pull_supplier (
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
  TAO_Notify_ProxySupplier_i* proxysupplier;

  switch (ctype)
    {
    case CosNotifyChannelAdmin::ANY_EVENT:
      {
        ACE_NEW_THROW_EX (proxysupplier,
                          TAO_Notify_ProxyPushSupplier_i (*this),
                          CORBA::NO_MEMORY ());

      }
      break;
    case CosNotifyChannelAdmin::STRUCTURED_EVENT:
      {
        ACE_NEW_THROW_EX (proxysupplier,
                          TAO_Notify_StructuredProxyPushSupplier_i (*this),
                          CORBA::NO_MEMORY ());
      }
      break;

    case CosNotifyChannelAdmin::SEQUENCE_EVENT:
    default:
      ACE_THROW_RETURN (CORBA::BAD_PARAM (),
                        CosNotifyChannelAdmin::ProxySupplier::_nil ());
    }

  CosNotifyChannelAdmin::ProxySupplier_var proxysupplier_ret;

  auto_ptr <TAO_Notify_ProxySupplier_i> auto_proxysupp (proxysupplier);

  proxysupplier->init (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::ProxySupplier::_nil ());

  dispatcher_->add_dispatcher (*proxysupplier);
  // @@ check ret val.

  CORBA::Object_var obj =
    proxysupplier->get_ref (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::ProxySupplier::_nil ());

  proxysupplier_ret =
    CosNotifyChannelAdmin::ProxySupplier::_narrow (obj.in (),
                                                   ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::ProxySupplier::_nil ());
  obj._retn (); // giveup ownership.

  proxy_id = proxy_supplier_ids.get ();

  if (proxysupplier_map_.bind (proxy_id,
                               proxysupplier) == -1)
    ACE_THROW_RETURN (CORBA::INTERNAL (),
                      CosNotifyChannelAdmin::ProxySupplier::_nil ());
  auto_proxysupp.release ();  // all o.k., let go!

  return proxysupplier_ret._retn ();
}

void TAO_Notify_ConsumerAdmin_i::destroy (
    CORBA::Environment & /* ACE_TRY_ENV */
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  //Add your implementation here
}

CosEventChannelAdmin::ProxyPushSupplier_ptr
TAO_Notify_ConsumerAdmin_i::obtain_push_supplier (
    CORBA::Environment& ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  TAO_Notify_ProxySupplier_i* proxysupplier;

  ACE_NEW_THROW_EX (proxysupplier,
                    TAO_Notify_ProxyPushSupplier_i (*this),
                    CORBA::NO_MEMORY ());

  CosEventChannelAdmin::ProxyPushSupplier_var proxysupplier_ret;
  CORBA::Object_var obj;

  auto_ptr <TAO_Notify_ProxySupplier_i> auto_proxysupp (proxysupplier);

  obj = proxysupplier->get_ref (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosEventChannelAdmin::ProxyPushSupplier::_nil ());

  proxysupplier_ret =
    CosEventChannelAdmin::ProxyPushSupplier::_narrow (obj.in (),
                                                      ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosEventChannelAdmin::ProxyPushSupplier::_nil ());
  obj._retn (); // giveup ownership.

  auto_proxysupp.release ();

  return proxysupplier_ret._retn ();
}

CosEventChannelAdmin::ProxyPullSupplier_ptr
TAO_Notify_ConsumerAdmin_i::obtain_pull_supplier (
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
template class ACE_Hash_Map_Entry <CosNotifyChannelAdmin::ProxyID,TAO_Notify_ProxySupplier_i*>;
template class ACE_Hash_Map_Manager<CosNotifyChannelAdmin::ProxyID,TAO_Notify_ProxySupplier_i*,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Manager_Ex<CosNotifyChannelAdmin::ProxyID,TAO_Notify_ProxySupplier_i*,ACE_Hash<CosNotifyChannelAdmin::ProxyID>, ACE_Equal_To<CosNotifyChannelAdmin::ProxyID>, ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator<CosNotifyChannelAdmin::ProxyID,TAO_Notify_ProxySupplier_i*,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Ex<CosNotifyChannelAdmin::ProxyID,TAO_Notify_ProxySupplier_i*,ACE_Hash<CosNotifyChannelAdmin::ProxyID>, ACE_Equal_To<CosNotifyChannelAdmin::ProxyID>, ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Base_Ex<CosNotifyChannelAdmin::ProxyID,TAO_Notify_ProxySupplier_i*,ACE_Hash<CosNotifyChannelAdmin::ProxyID>, ACE_Equal_To<CosNotifyChannelAdmin::ProxyID>, ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator<CosNotifyChannelAdmin::ProxyID,TAO_Notify_ProxySupplier_i*,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<CosNotifyChannelAdmin::ProxyID,TAO_Notify_ProxySupplier_i*,ACE_Hash<CosNotifyChannelAdmin::ProxyID>, ACE_Equal_To<CosNotifyChannelAdmin::ProxyID>, ACE_SYNCH_MUTEX>;

//template class ID_Pool<CosNotifyChannelAdmin::ProxyID>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash_Map_Entry<CosNotifyChannelAdmin::ProxyID,TAO_Notify_ProxySupplier_i*,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Manager<CosNotifyChannelAdmin::ProxyID,TAO_Notify_ProxySupplier_i*,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Manager_Ex<CosNotifyChannelAdmin::ProxyID,TAO_Notify_ProxySupplier_i*,ACE_Hash<CosNotifyChannelAdmin::ProxyID>, ACE_Equal_To<CosNotifyChannelAdmin::ProxyID>, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator<CosNotifyChannelAdmin::ProxyID,TAO_Notify_ProxySupplier_i*,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<CosNotifyChannelAdmin::ProxyID,TAO_Notify_ProxySupplier_i*,ACE_Hash<CosNotifyChannelAdmin::ProxyID>, ACE_Equal_To<CosNotifyChannelAdmin::ProxyID>, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<CosNotifyChannelAdmin::ProxyID,TAO_Notify_ProxySupplier_i*,ACE_Hash<CosNotifyChannelAdmin::ProxyID>, ACE_Equal_To<CosNotifyChannelAdmin::ProxyID>, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<CosNotifyChannelAdmin::ProxyID,TAO_Notify_ProxySupplier_i*,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<CosNotifyChannelAdmin::ProxyID,TAO_Notify_ProxySupplier_i*,ACE_Hash<CosNotifyChannelAdmin::ProxyID>, ACE_Equal_To<CosNotifyChannelAdmin::ProxyID>, ACE_SYNCH_MUTEX>

//#pragma instantiate ID_Pool<CosNotifyChannelAdmin::ProxyID>
#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

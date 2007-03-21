// $Id$

#ifndef TAO_Notify_PROXYCONSUMER_T_CPP
#define TAO_Notify_PROXYCONSUMER_T_CPP

#include "orbsvcs/Notify/ProxyConsumer_T.h"
#include "orbsvcs/Notify/SupplierAdmin.h"

#include "orbsvcs/Notify/Event_Manager.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <class SERVANT_TYPE>
TAO_Notify_ProxyConsumer_T<SERVANT_TYPE>::TAO_Notify_ProxyConsumer_T (void)
{
}

template <class SERVANT_TYPE>
TAO_Notify_ProxyConsumer_T<SERVANT_TYPE>::~TAO_Notify_ProxyConsumer_T ()
{
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxyConsumer_T<SERVANT_TYPE>::admin_types_changed (const CosNotification::EventTypeSeq & added,
                                                           const CosNotification::EventTypeSeq & removed)
{
  this->offer_change (added, removed);
}

template <class SERVANT_TYPE> CosNotifyChannelAdmin::SupplierAdmin_ptr
TAO_Notify_ProxyConsumer_T<SERVANT_TYPE>::MyAdmin (void)
{
  CosNotifyChannelAdmin::SupplierAdmin_var ret;

  CORBA::Object_var object = this->supplier_admin().ref ();

  ret = CosNotifyChannelAdmin::SupplierAdmin::_narrow (object.in ());

  return ret._retn ();
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxyConsumer_T<SERVANT_TYPE>::offer_change (const CosNotification::EventTypeSeq & added, const CosNotification::EventTypeSeq & removed)
{
  TAO_Notify_EventTypeSeq seq_added (added);
  TAO_Notify_EventTypeSeq seq_removed (removed);

  {
    ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                        CORBA::INTERNAL ());

    this->subscribed_types_.add_and_remove (seq_added, seq_removed);
  }

  this->event_manager().offer_change (this,
                                      seq_added,
                                      seq_removed);
}

template <class SERVANT_TYPE> CosNotification::EventTypeSeq*
TAO_Notify_ProxyConsumer_T<SERVANT_TYPE>::obtain_subscription_types (
    CosNotifyChannelAdmin::ObtainInfoMode mode)
{
  return
    this->obtain_types (mode, this->event_manager().subscription_types ());
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_Notify_PROXYCONSUMER_T_CPP */

// $Id$

#ifndef TAO_Notify_PROXYCONSUMER_T_CPP
#define TAO_Notify_PROXYCONSUMER_T_CPP

#include "ProxyConsumer_T.h"
#include "SupplierAdmin.h"

#if ! defined (__ACE_INLINE__)
#include "ProxyConsumer_T.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_Notify_ProxyConsumer_T, "$Id$")

#include "Event_Manager.h"

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
                                                           const CosNotification::EventTypeSeq & removed
                                                           ACE_ENV_ARG_DECL)
{
  this->offer_change (added, removed ACE_ENV_ARG_PARAMETER);
}

template <class SERVANT_TYPE> CosNotifyChannelAdmin::SupplierAdmin_ptr
TAO_Notify_ProxyConsumer_T<SERVANT_TYPE>::MyAdmin (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  CosNotifyChannelAdmin::SupplierAdmin_var ret;

  CORBA::Object_var object = this->supplier_admin_->ref (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (ret._retn ());

  ret = CosNotifyChannelAdmin::SupplierAdmin::_narrow (object.in () ACE_ENV_ARG_PARAMETER);

  return ret._retn ();
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxyConsumer_T<SERVANT_TYPE>::offer_change (const CosNotification::EventTypeSeq & added, const CosNotification::EventTypeSeq & removed ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyComm::InvalidEventType
                   ))
{
  TAO_Notify_EventTypeSeq seq_added (added);
  TAO_Notify_EventTypeSeq seq_removed (removed);

  {
    ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                        CORBA::INTERNAL ());
    ACE_CHECK;

    this->subscribed_types_.init (seq_added, seq_removed);
  }

  this->event_manager_->offer_change (this, seq_added, seq_removed ACE_ENV_ARG_PARAMETER);
}

template <class SERVANT_TYPE> CosNotification::EventTypeSeq*
TAO_Notify_ProxyConsumer_T<SERVANT_TYPE>::obtain_subscription_types (CosNotifyChannelAdmin::ObtainInfoMode mode ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->obtain_types (mode, this->event_manager_->subscription_types () ACE_ENV_ARG_PARAMETER);
}

#endif /* TAO_Notify_PROXYCONSUMER_T_CPP */

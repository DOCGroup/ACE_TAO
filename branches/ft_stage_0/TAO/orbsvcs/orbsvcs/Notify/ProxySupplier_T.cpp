// $Id$

#ifndef TAO_NS_PROXYSUPPLIER_T_C
#define TAO_NS_PROXYSUPPLIER_T_C

#include "ProxySupplier_T.h"

#if ! defined (__ACE_INLINE__)
#include "ProxySupplier_T.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_NS_ProxySupplier_T, "$Id$")

#include "Consumer.h"
#include "Structured/StructuredEvent.h"
#include "Any/AnyEvent.h"

#include "Method_Request_Dispatch.h"
#include "Worker_Task.h"
#include "Event_Manager.h"
#include "ConsumerAdmin.h"

template <class SERVANT_TYPE>
TAO_NS_ProxySupplier_T<SERVANT_TYPE>::TAO_NS_ProxySupplier_T (void)
  :is_suspended_ (0)
{
}

template <class SERVANT_TYPE>
TAO_NS_ProxySupplier_T<SERVANT_TYPE>::~TAO_NS_ProxySupplier_T ()
{
}

template <class SERVANT_TYPE> void
TAO_NS_ProxySupplier_T<SERVANT_TYPE>::admin_types_changed (const CosNotification::EventTypeSeq & added,
                                                           const CosNotification::EventTypeSeq & removed
                                                           ACE_ENV_ARG_DECL)
{
  this->subscription_change (added, removed ACE_ENV_ARG_PARAMETER);
}

template <class SERVANT_TYPE> void
TAO_NS_ProxySupplier_T<SERVANT_TYPE>::forward_structured (const CosNotification::StructuredEvent& notification ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  TAO_NS_StructuredEvent_No_Copy event (notification);

  TAO_NS_Method_Request_Dispatch_No_Copy request (&event, this, 1);

  this->worker_task ()->execute (request ACE_ENV_ARG_PARAMETER);
}

template <class SERVANT_TYPE> void
TAO_NS_ProxySupplier_T<SERVANT_TYPE>::forward_structured_no_filtering (const CosNotification::StructuredEvent& notification ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  TAO_NS_StructuredEvent_No_Copy event(notification);

  TAO_NS_Method_Request_Dispatch_No_Copy request (&event, this, 0);

  this->worker_task ()->execute (request ACE_ENV_ARG_PARAMETER);
}

template <class SERVANT_TYPE> void
TAO_NS_ProxySupplier_T<SERVANT_TYPE>::forward_any (const CORBA::Any & any ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  TAO_NS_AnyEvent_No_Copy event (any);

  TAO_NS_Method_Request_Dispatch_No_Copy request (&event, this, 1);

  this->worker_task ()->execute (request ACE_ENV_ARG_PARAMETER);
}

template <class SERVANT_TYPE> void
TAO_NS_ProxySupplier_T<SERVANT_TYPE>::forward_any_no_filtering (const CORBA::Any& any ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ))
{
  TAO_NS_AnyEvent_No_Copy event(any);

  TAO_NS_Method_Request_Dispatch_No_Copy request (&event, this, 0);

  this->worker_task ()->execute (request ACE_ENV_ARG_PARAMETER);
}

template <class SERVANT_TYPE> CosNotification::EventTypeSeq*
TAO_NS_ProxySupplier_T<SERVANT_TYPE>::obtain_offered_types (CosNotifyChannelAdmin::ObtainInfoMode mode ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->obtain_types (mode, this->event_manager_->offered_types () ACE_ENV_ARG_PARAMETER);
}

template <class SERVANT_TYPE> void
TAO_NS_ProxySupplier_T<SERVANT_TYPE>::subscription_change (const CosNotification::EventTypeSeq & added,
                                                           const CosNotification::EventTypeSeq & removed
                                                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CosNotifyComm::InvalidEventType))
{
  TAO_NS_EventTypeSeq seq_added (added);
  TAO_NS_EventTypeSeq seq_removed (removed);

  {
    ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                        CORBA::INTERNAL ());
    ACE_CHECK;

    this->subscribed_types_.init (seq_added, seq_removed);
  }

  this->event_manager_->subscription_change (this, seq_added, seq_removed ACE_ENV_ARG_PARAMETER);
}

template <class SERVANT_TYPE> void
TAO_NS_ProxySupplier_T<SERVANT_TYPE>::suspend_connection (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyChannelAdmin::ConnectionAlreadyInactive,
                   CosNotifyChannelAdmin::NotConnected
                   ))
{
  {
    ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_, CORBA::INTERNAL ());

    if (this->is_connected () == 0)
      ACE_THROW (CosNotifyChannelAdmin::NotConnected ());

    if (this->consumer_->is_suspended () == 1)
      ACE_THROW (CosNotifyChannelAdmin::ConnectionAlreadyInactive ());
  }

  this->consumer_->suspend (ACE_ENV_SINGLE_ARG_PARAMETER);
}

template <class SERVANT_TYPE> void
TAO_NS_ProxySupplier_T<SERVANT_TYPE>::resume_connection (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyChannelAdmin::ConnectionAlreadyActive,
                   CosNotifyChannelAdmin::NotConnected
                   ))
{
  {
    ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_, CORBA::INTERNAL ());

    if (this->is_connected () == 0)
      ACE_THROW (CosNotifyChannelAdmin::NotConnected ());

    if (this->consumer_->is_suspended () == 0)
      ACE_THROW (CosNotifyChannelAdmin::ConnectionAlreadyActive ());
  }

  this->consumer_->resume (ACE_ENV_SINGLE_ARG_PARAMETER);
}

template <class SERVANT_TYPE> CosNotifyChannelAdmin::ConsumerAdmin_ptr
TAO_NS_ProxySupplier_T<SERVANT_TYPE>::MyAdmin (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  CosNotifyChannelAdmin::ConsumerAdmin_var ret;

  CORBA::Object_var object = this->consumer_admin_->ref (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (ret._retn ());

  ret = CosNotifyChannelAdmin::ConsumerAdmin::_narrow (object.in () ACE_ENV_ARG_PARAMETER);

  return ret._retn ();
}

/***************************** UNIMPLEMENTED METHODS***************************************/

template <class SERVANT_TYPE> CosNotifyFilter::MappingFilter_ptr
TAO_NS_ProxySupplier_T<SERVANT_TYPE>::priority_filter (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    CosNotifyFilter::MappingFilter::_nil ());
}

template <class SERVANT_TYPE> void
TAO_NS_ProxySupplier_T<SERVANT_TYPE>::priority_filter (CosNotifyFilter::MappingFilter_ptr /*priority_filter*/ ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

template <class SERVANT_TYPE> CosNotifyFilter::MappingFilter_ptr
TAO_NS_ProxySupplier_T<SERVANT_TYPE>::lifetime_filter (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    CosNotifyFilter::MappingFilter::_nil ());
}

template <class SERVANT_TYPE> void
TAO_NS_ProxySupplier_T<SERVANT_TYPE>::lifetime_filter (CosNotifyFilter::MappingFilter_ptr /*lifetime_filter*/ ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

#endif /* #define TAO_NS_PROXYSUPPLIER_T_C */

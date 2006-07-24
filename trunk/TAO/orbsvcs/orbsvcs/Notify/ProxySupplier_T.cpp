// $Id$

#ifndef TAO_Notify_PROXYSUPPLIER_T_CPP
#define TAO_Notify_PROXYSUPPLIER_T_CPP

#include "orbsvcs/Notify/ProxySupplier_T.h"

#include "orbsvcs/Notify/Consumer.h"
#include "orbsvcs/Notify/Structured/StructuredEvent.h"
#include "orbsvcs/Notify/Any/AnyEvent.h"

#include "orbsvcs/Notify/Method_Request_Dispatch.h"
#include "orbsvcs/Notify/Worker_Task.h"
#include "orbsvcs/Notify/Event_Manager.h"
#include "orbsvcs/Notify/ConsumerAdmin.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <class SERVANT_TYPE>
TAO_Notify_ProxySupplier_T<SERVANT_TYPE>::TAO_Notify_ProxySupplier_T (void)
  : is_suspended_ (0)
{
}

template <class SERVANT_TYPE>
TAO_Notify_ProxySupplier_T<SERVANT_TYPE>::~TAO_Notify_ProxySupplier_T ()
{
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxySupplier_T<SERVANT_TYPE>::admin_types_changed (const CosNotification::EventTypeSeq & added,
                                                           const CosNotification::EventTypeSeq & removed
                                                           ACE_ENV_ARG_DECL)
{
  this->subscription_change (added, removed ACE_ENV_ARG_PARAMETER);
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxySupplier_T<SERVANT_TYPE>::forward_structured (const CosNotification::StructuredEvent& notification ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  TAO_Notify_StructuredEvent_No_Copy event (notification);

  TAO_Notify_Method_Request_Dispatch_No_Copy request (&event, this, 1);

  this->execute_task (request ACE_ENV_ARG_PARAMETER);
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxySupplier_T<SERVANT_TYPE>::forward_structured_no_filtering (const CosNotification::StructuredEvent& notification ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  TAO_Notify_StructuredEvent_No_Copy event(notification);

  TAO_Notify_Method_Request_Dispatch_No_Copy request (&event, this, 0);

  this->execute_task (request ACE_ENV_ARG_PARAMETER);
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxySupplier_T<SERVANT_TYPE>::forward_any (const CORBA::Any & any ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  TAO_Notify_AnyEvent_No_Copy event (any);

  TAO_Notify_Method_Request_Dispatch_No_Copy request (&event, this, 1);

  this->execute_task (request ACE_ENV_ARG_PARAMETER);
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxySupplier_T<SERVANT_TYPE>::forward_any_no_filtering (const CORBA::Any& any ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ))
{
  TAO_Notify_AnyEvent_No_Copy event(any);

  TAO_Notify_Method_Request_Dispatch_No_Copy request (&event, this, 0);

  this->execute_task (request ACE_ENV_ARG_PARAMETER);
}

template <class SERVANT_TYPE> CosNotification::EventTypeSeq*
TAO_Notify_ProxySupplier_T<SERVANT_TYPE>::obtain_offered_types (CosNotifyChannelAdmin::ObtainInfoMode mode ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->obtain_types (mode, this->event_manager().offered_types () ACE_ENV_ARG_PARAMETER);
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxySupplier_T<SERVANT_TYPE>::subscription_change (const CosNotification::EventTypeSeq & added,
                                                           const CosNotification::EventTypeSeq & removed
                                                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CosNotifyComm::InvalidEventType))
{
  TAO_Notify_EventTypeSeq seq_added (added);
  TAO_Notify_EventTypeSeq seq_removed (removed);

  {
    ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                        CORBA::INTERNAL ());
    ACE_CHECK;

    this->subscribed_types_.add_and_remove (seq_added, seq_removed);
  }

  this->event_manager().subscription_change (this, seq_added, seq_removed ACE_ENV_ARG_PARAMETER);
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxySupplier_T<SERVANT_TYPE>::suspend_connection (ACE_ENV_SINGLE_ARG_DECL)
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

    if (this->consumer()->is_suspended () == 1)
      ACE_THROW (CosNotifyChannelAdmin::ConnectionAlreadyInactive ());
  }

  this->consumer()->suspend (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  this->self_change (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxySupplier_T<SERVANT_TYPE>::resume_connection (ACE_ENV_SINGLE_ARG_DECL)
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

    if (this->consumer()->is_suspended () == 0)
      ACE_THROW (CosNotifyChannelAdmin::ConnectionAlreadyActive ());
  }

  this->consumer()->resume (ACE_ENV_SINGLE_ARG_PARAMETER);
}

template <class SERVANT_TYPE> CosNotifyChannelAdmin::ConsumerAdmin_ptr
TAO_Notify_ProxySupplier_T<SERVANT_TYPE>::MyAdmin (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  CosNotifyChannelAdmin::ConsumerAdmin_var ret;

  CORBA::Object_var object = this->consumer_admin().ref (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (ret._retn ());

  ret = CosNotifyChannelAdmin::ConsumerAdmin::_narrow (object.in () ACE_ENV_ARG_PARAMETER);

  return ret._retn ();
}

/***************************** UNIMPLEMENTED METHODS***************************************/

template <class SERVANT_TYPE> CosNotifyFilter::MappingFilter_ptr
TAO_Notify_ProxySupplier_T<SERVANT_TYPE>::priority_filter (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    CosNotifyFilter::MappingFilter::_nil ());

  return CosNotifyFilter::MappingFilter::_nil ();
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxySupplier_T<SERVANT_TYPE>::priority_filter (CosNotifyFilter::MappingFilter_ptr /*priority_filter*/ ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

template <class SERVANT_TYPE> CosNotifyFilter::MappingFilter_ptr
TAO_Notify_ProxySupplier_T<SERVANT_TYPE>::lifetime_filter (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    CosNotifyFilter::MappingFilter::_nil ());

  return CosNotifyFilter::MappingFilter::_nil ();
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxySupplier_T<SERVANT_TYPE>::lifetime_filter (CosNotifyFilter::MappingFilter_ptr /*lifetime_filter*/ ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* #define TAO_Notify_PROXYSUPPLIER_T_CPP */

// $Id$
#ifndef TAO_NOTIFY_PROXYCONSUMER_T_C
#define TAO_NOTIFY_PROXYCONSUMER_T_C

#include "Notify_ProxyConsumer_T.h"

#include "Notify_Event_Manager.h"
#include "Notify_SupplierAdmin_i.h"
#include "ace/Auto_Ptr.h"

template <class SERVANT_TYPE>
TAO_Notify_ProxyConsumer<SERVANT_TYPE>::TAO_Notify_ProxyConsumer (TAO_Notify_SupplierAdmin_i* supplieradmin, TAO_Notify_Resource_Manager* resource_manager)
  :TAO_Notify_Proxy<SERVANT_TYPE> (resource_manager),
  myadmin_ (supplieradmin)
{
  event_manager_ = supplieradmin->get_event_manager ();
}

// Implementation skeleton destructor
template <class SERVANT_TYPE>
TAO_Notify_ProxyConsumer<SERVANT_TYPE>::~TAO_Notify_ProxyConsumer (void)
{
  if (!is_destroyed_)
    this->cleanup_i ();
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxyConsumer<SERVANT_TYPE>::init (CORBA::Environment& /*ACE_TRY_ENV*/)
{
  // No-Op.
}

template <class SERVANT_TYPE> CORBA::Boolean
TAO_Notify_ProxyConsumer<SERVANT_TYPE>::check_filters_i (const TAO_Notify_Event& event, CORBA::Environment& ACE_TRY_ENV)
{
  // check if it passes the parent filter.
  CORBA::Boolean bval =
    this->myadmin_->get_filter_admin ().match (event,
                                               ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  if (bval == 0) // If the filter did not match, don't send the event.
    return bval;

  // Do we need to check our filter too.
  if (myadmin_->MyOperator (ACE_TRY_ENV) == CosNotifyChannelAdmin::AND_OP)
    {
      bval = this->filter_admin_.match (event,
                                        ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
    }

  return bval;
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxyConsumer<SERVANT_TYPE>::on_connected (CORBA::Environment &ACE_TRY_ENV)
{
  this->event_manager_->register_for_subscription_updates (this, ACE_TRY_ENV);
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxyConsumer<SERVANT_TYPE>::cleanup_i (CORBA::Environment& ACE_TRY_ENV)
{
  // cleanup here
  this->is_destroyed_ = 1;

  this->event_manager_->unregister_from_subscription_updates (this,
                                                              ACE_TRY_ENV);
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxyConsumer<SERVANT_TYPE>::offer_change (const CosNotification::EventTypeSeq & added, const CosNotification::EventTypeSeq & removed, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyComm::InvalidEventType
                   ))
{
  this->event_manager_->update_publication_list (added, removed, ACE_TRY_ENV);
}

template <class SERVANT_TYPE> CosNotification::EventTypeSeq*
TAO_Notify_ProxyConsumer<SERVANT_TYPE>::obtain_subscription_types (CosNotifyChannelAdmin::ObtainInfoMode mode, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  CosNotification::EventTypeSeq* event_type_seq = 0;

  if (mode == CosNotifyChannelAdmin::ALL_NOW_UPDATES_OFF ||
      mode == CosNotifyChannelAdmin::ALL_NOW_UPDATES_ON)
    {
      event_type_seq = this->event_manager_->obtain_subscription_types ();
    }

  if (mode == CosNotifyChannelAdmin::NONE_NOW_UPDATES_ON ||
      mode == CosNotifyChannelAdmin::ALL_NOW_UPDATES_ON)
    {
      // if updates are currently off, switch them on.
      if (this->updates_on_ == 0)
        this->event_manager_->register_for_subscription_updates (this,
                                                                 ACE_TRY_ENV);
    }
  else
    {
      // if updates are currently on, switch them off.
      if (this->updates_on_ == 1)
        this->event_manager_->unregister_from_subscription_updates (this,
                                                                    ACE_TRY_ENV);
    }

  return event_type_seq;
}

template <class SERVANT_TYPE> CosNotifyChannelAdmin::SupplierAdmin_ptr
TAO_Notify_ProxyConsumer<SERVANT_TYPE>::MyAdmin (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->myadmin_->get_ref (ACE_TRY_ENV);
}

#endif /* TAO_NOTIFY_PROXYCONSUMER_T_C */

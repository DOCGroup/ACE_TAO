// $Id$

#ifndef TAO_NOTIFY_PROXYSUPPLIER_T_C
#define TAO_NOTIFY_PROXYSUPPLIER_T_C

#include "Notify_ProxySupplier_T.h"
#include "Notify_Event_Manager.h"
#include "Notify_ConsumerAdmin_i.h"

template <class SERVANT_TYPE>
TAO_Notify_ProxySupplier<SERVANT_TYPE>::TAO_Notify_ProxySupplier (TAO_Notify_ConsumerAdmin_i* consumeradmin, TAO_Notify_Resource_Manager* resource_manager)
  :TAO_Notify_Proxy<SERVANT_TYPE> (resource_manager),
  myadmin_ (consumeradmin),
  is_suspended_ (0)
{
  event_manager_ = consumeradmin->get_event_manager ();
}

// Implementation skeleton destructor
template <class SERVANT_TYPE>
TAO_Notify_ProxySupplier<SERVANT_TYPE>::~TAO_Notify_ProxySupplier (void)
{
  if (!is_destroyed_)
    this->cleanup_i ();

  this->myadmin_->proxy_pushsupplier_destroyed (this->myID_);
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxySupplier<SERVANT_TYPE>::cleanup_i (CORBA::Environment& ACE_TRY_ENV)
{
  this->is_destroyed_ = 1;
  this->event_manager_->unregister_from_publication_updates (this,
                                                             ACE_TRY_ENV);

  // unsubscribe it to our current subscriptions.
  CosNotification::EventTypeSeq added (0);
  CosNotification::EventTypeSeq removed (this->subscription_list_.size ());

  this->subscription_list_.populate (removed);
  added.length (0);

  this->event_manager_->subscribe_for_events (this,
                                              0,
                                              added, removed, ACE_TRY_ENV);
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxySupplier<SERVANT_TYPE>::dispatch_event (TAO_Notify_Event &event, CORBA::Environment &ACE_TRY_ENV)
{
  if (this->is_connected_ == 0)
    {
      ACE_DEBUG ((LM_DEBUG,"%t, %p",
                  "dispatch_event to disconnected proxy supplier from EC\n"));
      return;
    }

  // check if it passes the parent filter.
  CORBA::Boolean bval =
    this->myadmin_->get_filter_admin ().match (event,
                                               ACE_TRY_ENV);
  ACE_CHECK;

  if (bval == 0) // If the filter did not match, don't send the event.
    return;

  // Do we need to check our filter too.
  if (myadmin_->MyOperator (ACE_TRY_ENV) == CosNotifyChannelAdmin::AND_OP)
    {
      bval = this->filter_admin_.match (event,
                                        ACE_TRY_ENV);
      ACE_CHECK;

      if (bval == 0) // If the filter did not match, don't send the event.
        return;
    }

  if (this->is_suspended_ == 1)
    {
      this->event_list_.enqueue_tail (event.clone ());
    }
  else
    this->dispatch_event_i (event, ACE_TRY_ENV);
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxySupplier<SERVANT_TYPE>::subscription_change (const CosNotification::EventTypeSeq & added, const CosNotification::EventTypeSeq & removed, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyComm::InvalidEventType
                   ))
{
  if (this->is_connected_ == 1)
    {
      this->event_manager_->subscribe_for_events (this,
                                                  &this->subscription_list_,
                                                  added, removed, ACE_TRY_ENV);
    }
  else // simply update our subscription list.
    {
      this->subscription_list_.insert_seq (added);
      this->subscription_list_.remove_seq (removed);
    }
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxySupplier<SERVANT_TYPE>::on_connected (CORBA::Environment &ACE_TRY_ENV)
{
  // register with CA
  this->myadmin_->register_listener (this, ACE_TRY_ENV);
  ACE_CHECK;

  // subscribe it to our current subscriptions.
  CosNotification::EventTypeSeq added (this->subscription_list_.size ());
  CosNotification::EventTypeSeq removed (0);

  this->subscription_list_.populate (added);
  removed.length (0);

  this->event_manager_->subscribe_for_events (this,
                                              0,
                                              added, removed, ACE_TRY_ENV);
  ACE_CHECK;

  this->event_manager_->register_for_publication_updates (this, ACE_TRY_ENV);
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxySupplier<SERVANT_TYPE>::suspend_connection (CORBA::Environment &/*ACE_TRY_ENV*/)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyChannelAdmin::ConnectionAlreadyInactive,
                   CosNotifyChannelAdmin::NotConnected
                   ))
{
  this->is_suspended_ = 1;
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxySupplier<SERVANT_TYPE>::resume_connection (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyChannelAdmin::ConnectionAlreadyActive,
                   CosNotifyChannelAdmin::NotConnected
                   ))
{
  TAO_Notify_Event* event;

  while (this->event_list_.dequeue_head (event) == 0)
    {
      this->dispatch_event_i (*event, ACE_TRY_ENV);
      delete event;
    }

  this->is_suspended_ = 0;
}

template <class SERVANT_TYPE> CosNotifyChannelAdmin::ConsumerAdmin_ptr
TAO_Notify_ProxySupplier<SERVANT_TYPE>::MyAdmin (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->myadmin_->get_ref (ACE_TRY_ENV);
}

template <class SERVANT_TYPE> CosNotifyFilter::MappingFilter_ptr
TAO_Notify_ProxySupplier<SERVANT_TYPE>::priority_filter (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    CosNotifyFilter::MappingFilter::_nil ());
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxySupplier<SERVANT_TYPE>::priority_filter (CosNotifyFilter::MappingFilter_ptr /*priority_filter*/, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

template <class SERVANT_TYPE> CosNotifyFilter::MappingFilter_ptr
TAO_Notify_ProxySupplier<SERVANT_TYPE>::lifetime_filter (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    CosNotifyFilter::MappingFilter::_nil ());
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxySupplier<SERVANT_TYPE>::lifetime_filter (CosNotifyFilter::MappingFilter_ptr /*lifetime_filter*/, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

template <class SERVANT_TYPE> CosNotification::EventTypeSeq*
TAO_Notify_ProxySupplier<SERVANT_TYPE>::obtain_offered_types (CosNotifyChannelAdmin::ObtainInfoMode mode, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  CosNotification::EventTypeSeq* event_type_seq = 0;

  if (mode == CosNotifyChannelAdmin::ALL_NOW_UPDATES_OFF ||
      mode == CosNotifyChannelAdmin::ALL_NOW_UPDATES_ON)
    {
      event_type_seq = this->event_manager_->obtain_offered_types ();
    }

  if (mode == CosNotifyChannelAdmin::NONE_NOW_UPDATES_ON ||
      mode == CosNotifyChannelAdmin::ALL_NOW_UPDATES_ON)
    {
      // if updates are currently off, switch them on.
      if (this->updates_on_ == 0)
        this->event_manager_->register_for_publication_updates (this,
                                                                ACE_TRY_ENV);
    }
  else
    {
      // if updates are currently on, switch them off.
      if (this->updates_on_ == 1)
        this->event_manager_->unregister_from_publication_updates (this,
                                                                   ACE_TRY_ENV);
    }
  return event_type_seq;
}

#endif /* TAO_NOTIFY_PROXYSUPPLIER_T_C */

// $Id$

#ifndef TAO_NOTIFY_PROXYSUPPLIER_T_C
#define TAO_NOTIFY_PROXYSUPPLIER_T_C

#include "Notify_ProxySupplier_T.h"
#include "Notify_Event_Manager.h"
#include "Notify_ConsumerAdmin_i.h"

template <class SERVANT_TYPE>
TAO_Notify_ProxySupplier<SERVANT_TYPE>::TAO_Notify_ProxySupplier (TAO_Notify_ConsumerAdmin_i* consumeradmin, TAO_Notify_Resource_Manager* resource_manager)
  :mytype_ (CosNotifyChannelAdmin::PUSH_ANY),
   myadmin_ (consumeradmin),
   event_manager_ (consumeradmin->get_event_manager ()),
   resource_manager_ (resource_manager),
   is_connected_ (0),
   is_suspended_ (0),
   is_destroyed_ (0)
{
  // No-Op.
}

// Implementation skeleton destructor
template <class SERVANT_TYPE>
TAO_Notify_ProxySupplier<SERVANT_TYPE>::~TAO_Notify_ProxySupplier (void)
{
  if (!is_destroyed_)
    this->cleanup_i ();
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxySupplier<SERVANT_TYPE>::init (CORBA::Environment& /*ACE_TRY_ENV*/)
{
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxySupplier<SERVANT_TYPE>::cleanup_i (CORBA::Environment& ACE_TRY_ENV)
{
  this->is_destroyed_ = 1;
  this->event_manager_->unsubscribe_from_events (this, ACE_TRY_ENV);
  this->myadmin_->unsubscribe_from_events (this);
  this->event_manager_->unregister_from_publication_updates (this,
                                                             ACE_TRY_ENV);
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxySupplier<SERVANT_TYPE>::subscription_change (const CosNotification::EventTypeSeq & added, const CosNotification::EventTypeSeq & removed, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyComm::InvalidEventType
                   ))
{
  EVENTTYPE_LIST added_ret;
  EVENTTYPE_LIST removed_ret;

  this->update_subscription_list_i (added, removed, added_ret, removed_ret,
                                    ACE_TRY_ENV);
  ACE_CHECK;

  if (this->is_connected_ == 1)
    this->event_manager_->
      update_subscription_for_events (this,
                                      added_ret, removed_ret,
                                      ACE_TRY_ENV);
  ACE_CHECK;
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxySupplier<SERVANT_TYPE>::update_subscription_list_i (const CosNotification::EventTypeSeq & added, const CosNotification::EventTypeSeq & removed, EVENTTYPE_LIST& added_ret, EVENTTYPE_LIST& removed_ret, CORBA::Environment &/*ACE_TRY_ENV*/)
{
  TAO_Notify_EventType event_type;

  CORBA::ULong len = removed.length ();
  CORBA::ULong i = 0;
  for (; i < len; ++i)
    {
      event_type = removed[i];

      if (this->subscription_list_.remove (event_type) == 0)
        {
          removed_ret.insert (event_type);
        }
    }

  const EVENTTYPE_LIST& ca_subscription_list =
    this->myadmin_->get_subscription_list ();

  len = added.length ();
  for (i = 0; i < len; ++i)
    {
      event_type = added[i];

      // insert into our list only if we don't already get the event via
      // the CA.
      if (ca_subscription_list.find (event_type) == -1 &&
          this->subscription_list_.insert (event_type) == 0)
        {
          added_ret.insert (event_type);
        }
    }
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxySupplier<SERVANT_TYPE>::subscribe_for_events_i (CORBA::Environment &ACE_TRY_ENV)
{
  // Subscribe to the CA for all the events that it receives.
  this->myadmin_->subscribe_for_events (this);

  if (!this->subscription_list_.is_empty ())
    {
      this->event_manager_->subscribe_for_events (this,
                                                  this->subscription_list_,
                                                  ACE_TRY_ENV);
    }
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxySupplier<SERVANT_TYPE>::suspend_connection (CORBA::Environment &/*ACE_TRY_ENV*/)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyChannelAdmin::ConnectionAlreadyInactive,
                   CosNotifyChannelAdmin::NotConnected
                   ))
{
  // TODO:
  //Add your implementation here
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxySupplier<SERVANT_TYPE>::resume_connection (CORBA::Environment &/*ACE_TRY_ENV*/)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyChannelAdmin::ConnectionAlreadyActive,
                   CosNotifyChannelAdmin::NotConnected
                   ))
{
  // TODO:
  //Add your implementation here
}

template <class SERVANT_TYPE> CosNotifyChannelAdmin::ProxyType
TAO_Notify_ProxySupplier<SERVANT_TYPE>::MyType (CORBA::Environment &/*ACE_TRY_ENV*/)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return mytype_;
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
TAO_Notify_ProxySupplier<SERVANT_TYPE>::obtain_offered_types (CosNotifyChannelAdmin::ObtainInfoMode /*mode*/, CORBA::Environment &/*ACE_TRY_ENV*/)
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  // TODO:
  return 0;
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxySupplier<SERVANT_TYPE>::validate_event_qos (const CosNotification::QoSProperties & /*required_qos*/, CosNotification::NamedPropertyRangeSeq_out /*available_qos*/, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotification::UnsupportedQoS
                   ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

template <class SERVANT_TYPE> CosNotification::QoSProperties*
TAO_Notify_ProxySupplier<SERVANT_TYPE>::get_qos (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->qos_admin_.get_qos (ACE_TRY_ENV);
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxySupplier<SERVANT_TYPE>::set_qos (const CosNotification::QoSProperties & qos, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotification::UnsupportedQoS
                   ))
{
  this->qos_admin_.set_qos (qos, ACE_TRY_ENV);
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxySupplier<SERVANT_TYPE>::validate_qos (
    const CosNotification::QoSProperties & required_qos,
    CosNotification::NamedPropertyRangeSeq_out available_qos,
    CORBA::Environment &ACE_TRY_ENV
    )
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotification::UnsupportedQoS
                   ))
{
  this->qos_admin_.validate_qos (required_qos, available_qos, ACE_TRY_ENV);
}

template <class SERVANT_TYPE> CosNotifyFilter::FilterID
TAO_Notify_ProxySupplier<SERVANT_TYPE>::add_filter (CosNotifyFilter::Filter_ptr new_filter, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->filter_admin_.add_filter (new_filter, ACE_TRY_ENV);
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxySupplier<SERVANT_TYPE>::remove_filter (
    CosNotifyFilter::FilterID filter,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyFilter::FilterNotFound
                   ))
{
  this->filter_admin_.remove_filter (filter, ACE_TRY_ENV);
}

template <class SERVANT_TYPE> CosNotifyFilter::Filter_ptr
TAO_Notify_ProxySupplier<SERVANT_TYPE>::get_filter (CosNotifyFilter::FilterID filter, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyFilter::FilterNotFound
                   ))
{
  return this->filter_admin_.get_filter (filter, ACE_TRY_ENV);
}

template <class SERVANT_TYPE> CosNotifyFilter::FilterIDSeq*
TAO_Notify_ProxySupplier<SERVANT_TYPE>::get_all_filters (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->filter_admin_.get_all_filters (ACE_TRY_ENV);
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxySupplier<SERVANT_TYPE>::remove_all_filters (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  this->filter_admin_.remove_all_filters (ACE_TRY_ENV);
}

#endif /* TAO_NOTIFY_PROXYSUPPLIER_T_C */

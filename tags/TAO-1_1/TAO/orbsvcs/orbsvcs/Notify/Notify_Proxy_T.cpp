// $Id$

#ifndef TAO_NOTIFY_PROXY_T_C
#define TAO_NOTIFY_PROXY_T_C

#include "Notify_Proxy_T.h"
#include "Notify_Event_Manager.h"

template <class SERVANT_TYPE>
TAO_Notify_Proxy<SERVANT_TYPE>::TAO_Notify_Proxy (TAO_Notify_Resource_Manager* resource_manager)
  : resource_manager_ (resource_manager),
    is_connected_ (0),
    is_destroyed_ (0),
    updates_on_ (1)
{
  // No-Op.
}

// Implementation skeleton destructor
template <class SERVANT_TYPE>
TAO_Notify_Proxy<SERVANT_TYPE>::~TAO_Notify_Proxy (void)
{
  if (!is_destroyed_)
    this->cleanup_i ();
}

template <class SERVANT_TYPE> void
TAO_Notify_Proxy<SERVANT_TYPE>::init (CORBA::Environment& /*ACE_TRY_ENV*/)
{
}

template <class SERVANT_TYPE> void
TAO_Notify_Proxy<SERVANT_TYPE>::cleanup_i (CORBA::Environment& /*ACE_TRY_ENV*/)
{
  this->is_destroyed_ = 1;
}

template <class SERVANT_TYPE> void
TAO_Notify_Proxy<SERVANT_TYPE>::dispatch_update (EVENTTYPE_LIST& added_list, EVENTTYPE_LIST& removed_list, CORBA::Environment &ACE_TRY_ENV)
{
  CosNotification::EventTypeSeq added (added_list.size ());
  CosNotification::EventTypeSeq removed (removed_list.size ());

  added_list.populate (added);
  removed_list.populate (removed);

  this->dispatch_update_i (added, removed, ACE_TRY_ENV);
}

template <class SERVANT_TYPE> CosNotifyChannelAdmin::ProxyType
TAO_Notify_Proxy<SERVANT_TYPE>::MyType (CORBA::Environment &/*ACE_TRY_ENV*/)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return mytype_;
}

template <class SERVANT_TYPE> void
TAO_Notify_Proxy<SERVANT_TYPE>::validate_event_qos (const CosNotification::QoSProperties & /*required_qos*/, CosNotification::NamedPropertyRangeSeq_out /*available_qos*/, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotification::UnsupportedQoS
                   ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

template <class SERVANT_TYPE> CosNotification::QoSProperties*
TAO_Notify_Proxy<SERVANT_TYPE>::get_qos (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->qos_admin_.get_qos (ACE_TRY_ENV);
}

template <class SERVANT_TYPE> void
TAO_Notify_Proxy<SERVANT_TYPE>::set_qos (const CosNotification::QoSProperties & qos, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotification::UnsupportedQoS
                   ))
{
  this->qos_admin_.set_qos (qos, ACE_TRY_ENV);
}

template <class SERVANT_TYPE> void
TAO_Notify_Proxy<SERVANT_TYPE>::validate_qos (
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
TAO_Notify_Proxy<SERVANT_TYPE>::add_filter (CosNotifyFilter::Filter_ptr new_filter, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->filter_admin_.add_filter (new_filter, ACE_TRY_ENV);
}

template <class SERVANT_TYPE> void
TAO_Notify_Proxy<SERVANT_TYPE>::remove_filter (
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
TAO_Notify_Proxy<SERVANT_TYPE>::get_filter (CosNotifyFilter::FilterID filter, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyFilter::FilterNotFound
                   ))
{
  return this->filter_admin_.get_filter (filter, ACE_TRY_ENV);
}

template <class SERVANT_TYPE> CosNotifyFilter::FilterIDSeq*
TAO_Notify_Proxy<SERVANT_TYPE>::get_all_filters (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->filter_admin_.get_all_filters (ACE_TRY_ENV);
}

template <class SERVANT_TYPE> void
TAO_Notify_Proxy<SERVANT_TYPE>::remove_all_filters (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  this->filter_admin_.remove_all_filters (ACE_TRY_ENV);
}

#endif /* TAO_NOTIFY_PROXY_T_C */

// $Id$

#ifndef TAO_NOTIFY_PROXY_T_C
#define TAO_NOTIFY_PROXY_T_C

#include "Notify_Proxy_T.h"
#include "Notify_Event_Manager.h"

#include "tao/debug.h"

ACE_RCSID(Notify, Notify_Proxy_T, "$Id$")

template <class SERVANT_TYPE>
TAO_Notify_Proxy<SERVANT_TYPE>::TAO_Notify_Proxy (void)
  :lock_ (0),
   refcount_ (1),
   is_connected_ (0),
   updates_on_ (1)
{
  // No-Op.
}

// Implementation skeleton destructor
template <class SERVANT_TYPE>
TAO_Notify_Proxy<SERVANT_TYPE>::~TAO_Notify_Proxy (void)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "in ~TAO_Notify_Proxy\n"));
}

template <class SERVANT_TYPE> CORBA::ULong
TAO_Notify_Proxy<SERVANT_TYPE>::_incr_refcnt (void)
{
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);
  return this->refcount_++;
}

template <class SERVANT_TYPE> CORBA::ULong
TAO_Notify_Proxy<SERVANT_TYPE>::_decr_refcnt (void)
{
  {
    ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);
    this->refcount_--;
    if (this->refcount_ != 0)
      return this->refcount_;
  }

  delete this;
  return 0;
}

template <class SERVANT_TYPE> void
TAO_Notify_Proxy<SERVANT_TYPE>::_add_ref (TAO_ENV_SINGLE_ARG_DECL_NOT_USED/*TAO_ENV_SINGLE_ARG_PARAMETER*/)
{
  this->_incr_refcnt ();
}

template <class SERVANT_TYPE> void
TAO_Notify_Proxy<SERVANT_TYPE>::_remove_ref (TAO_ENV_SINGLE_ARG_DECL_NOT_USED/*TAO_ENV_SINGLE_ARG_PARAMETER*/)
{
  this->_decr_refcnt ();
}

template <class SERVANT_TYPE> void
TAO_Notify_Proxy<SERVANT_TYPE>::dispatch_update (TAO_Notify_EventType_List& added_list, TAO_Notify_EventType_List& removed_list TAO_ENV_ARG_DECL)
{
  CosNotification::EventTypeSeq added (added_list.size ());
  CosNotification::EventTypeSeq removed (removed_list.size ());

  added_list.populate (added);
  removed_list.populate (removed);

  this->dispatch_update_i (added, removed TAO_ENV_ARG_PARAMETER);
}

template <class SERVANT_TYPE> CosNotifyChannelAdmin::ProxyType
TAO_Notify_Proxy<SERVANT_TYPE>::MyType (TAO_ENV_SINGLE_ARG_DECL_NOT_USED/*TAO_ENV_SINGLE_ARG_PARAMETER*/)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return proxy_type_;
}

template <class SERVANT_TYPE> void
TAO_Notify_Proxy<SERVANT_TYPE>::validate_event_qos (const CosNotification::QoSProperties & /*required_qos*/, CosNotification::NamedPropertyRangeSeq_out /*available_qos*/ TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotification::UnsupportedQoS
                   ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

template <class SERVANT_TYPE> CosNotification::QoSProperties*
TAO_Notify_Proxy<SERVANT_TYPE>::get_qos (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->qos_admin_.get_qos (TAO_ENV_SINGLE_ARG_PARAMETER);
}

template <class SERVANT_TYPE> void
TAO_Notify_Proxy<SERVANT_TYPE>::set_qos (const CosNotification::QoSProperties & qos TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotification::UnsupportedQoS
                   ))
{
  this->qos_admin_.set_qos (qos TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->setup_qos_policies (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

template <class SERVANT_TYPE> void
TAO_Notify_Proxy<SERVANT_TYPE>::validate_qos (
    const CosNotification::QoSProperties & required_qos,
    CosNotification::NamedPropertyRangeSeq_out available_qos
    TAO_ENV_ARG_DECL
    )
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotification::UnsupportedQoS
                   ))
{
  this->qos_admin_.validate_qos (required_qos, available_qos TAO_ENV_ARG_PARAMETER);
}

template <class SERVANT_TYPE> CosNotifyFilter::FilterID
TAO_Notify_Proxy<SERVANT_TYPE>::add_filter (CosNotifyFilter::Filter_ptr new_filter TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->filter_admin_.add_filter (new_filter TAO_ENV_ARG_PARAMETER);
}

template <class SERVANT_TYPE> void
TAO_Notify_Proxy<SERVANT_TYPE>::remove_filter (
    CosNotifyFilter::FilterID filter
    TAO_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyFilter::FilterNotFound
                   ))
{
  this->filter_admin_.remove_filter (filter TAO_ENV_ARG_PARAMETER);
}

template <class SERVANT_TYPE> CosNotifyFilter::Filter_ptr
TAO_Notify_Proxy<SERVANT_TYPE>::get_filter (CosNotifyFilter::FilterID filter TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyFilter::FilterNotFound
                   ))
{
  return this->filter_admin_.get_filter (filter TAO_ENV_ARG_PARAMETER);
}

template <class SERVANT_TYPE> CosNotifyFilter::FilterIDSeq*
TAO_Notify_Proxy<SERVANT_TYPE>::get_all_filters (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->filter_admin_.get_all_filters (TAO_ENV_SINGLE_ARG_PARAMETER);
}

template <class SERVANT_TYPE> void
TAO_Notify_Proxy<SERVANT_TYPE>::remove_all_filters (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  this->filter_admin_.remove_all_filters (TAO_ENV_SINGLE_ARG_PARAMETER);
}

template <class SERVANT_TYPE> const TAO_Notify_QoSAdmin_i&
TAO_Notify_Proxy<SERVANT_TYPE>::qos_admin (void) const
{
  return this->qos_admin_;
}

template <class SERVANT_TYPE> void
TAO_Notify_Proxy<SERVANT_TYPE>::setup_qos_policies (
                                  TAO_ENV_SINGLE_ARG_DECL)
{
  this->filter_admin_.remove_all_filters (TAO_ENV_SINGLE_ARG_PARAMETER);
}

#endif /* TAO_NOTIFY_PROXY_T_C */

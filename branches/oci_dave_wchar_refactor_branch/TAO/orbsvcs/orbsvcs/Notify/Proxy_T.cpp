// $Id$

#include "orbsvcs/Notify/Proxy_T.h"

#ifndef TAO_Notify_PROXY_T_CPP
#define TAO_Notify_PROXY_T_CPP

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <class SERVANT_TYPE>
TAO_Notify_Proxy_T<SERVANT_TYPE>::TAO_Notify_Proxy_T (void)
{
  // Any object that is used by the POA gets a reference count of 1 at construction
  this->_incr_refcnt();
}

template <class SERVANT_TYPE>
TAO_Notify_Proxy_T<SERVANT_TYPE>::~TAO_Notify_Proxy_T ()
{
}

template <class SERVANT_TYPE> PortableServer::Servant
TAO_Notify_Proxy_T<SERVANT_TYPE>::servant (void)
{
  return this;
}

template <class SERVANT_TYPE> void
TAO_Notify_Proxy_T<SERVANT_TYPE>::_add_ref (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  this->_incr_refcnt ();
}

template <class SERVANT_TYPE> void
TAO_Notify_Proxy_T<SERVANT_TYPE>::_remove_ref (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  this->_decr_refcnt ();
}

template <class SERVANT_TYPE> void
TAO_Notify_Proxy_T<SERVANT_TYPE>::validate_event_qos (const CosNotification::QoSProperties & /*required_qos*/, CosNotification::NamedPropertyRangeSeq_out /*available_qos*/ ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotification::UnsupportedQoS
                   ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

template <class SERVANT_TYPE> CosNotification::QoSProperties*
TAO_Notify_Proxy_T<SERVANT_TYPE>::get_qos (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

  return this->TAO_Notify_Object::get_qos (ACE_ENV_SINGLE_ARG_PARAMETER);
}

template <class SERVANT_TYPE> void
TAO_Notify_Proxy_T<SERVANT_TYPE>::set_qos (const CosNotification::QoSProperties & qos ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotification::UnsupportedQoS
                   ))
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());

  this->TAO_Notify_Object::set_qos (qos ACE_ENV_ARG_PARAMETER);
}

template <class SERVANT_TYPE> void
TAO_Notify_Proxy_T<SERVANT_TYPE>::validate_qos (
                                            const CosNotification::QoSProperties & /*required_qos*/,
                                            CosNotification::NamedPropertyRangeSeq_out /*available_qos*/
    ACE_ENV_ARG_DECL
    )
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotification::UnsupportedQoS
                   ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

template <class SERVANT_TYPE> CosNotifyFilter::FilterID
TAO_Notify_Proxy_T<SERVANT_TYPE>::add_filter (CosNotifyFilter::Filter_ptr new_filter ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

  CosNotifyFilter::FilterID fid =
    this->filter_admin_.add_filter (new_filter ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(0);
  this->self_change (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN(fid);
  return fid;
}

template <class SERVANT_TYPE> void
TAO_Notify_Proxy_T<SERVANT_TYPE>::remove_filter (
    CosNotifyFilter::FilterID filter
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyFilter::FilterNotFound
                   ))
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());

  this->filter_admin_.remove_filter (filter ACE_ENV_ARG_PARAMETER);
}

template <class SERVANT_TYPE> CosNotifyFilter::Filter_ptr
TAO_Notify_Proxy_T<SERVANT_TYPE>::get_filter (CosNotifyFilter::FilterID filter ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyFilter::FilterNotFound
                   ))
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK_RETURN (CosNotifyFilter::Filter::_nil ());

  return this->filter_admin_.get_filter (filter ACE_ENV_ARG_PARAMETER);
}

template <class SERVANT_TYPE> CosNotifyFilter::FilterIDSeq*
TAO_Notify_Proxy_T<SERVANT_TYPE>::get_all_filters (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

  return this->filter_admin_.get_all_filters (ACE_ENV_SINGLE_ARG_PARAMETER);
}

template <class SERVANT_TYPE> void
TAO_Notify_Proxy_T<SERVANT_TYPE>::remove_all_filters (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());

  this->filter_admin_.remove_all_filters (ACE_ENV_SINGLE_ARG_PARAMETER);
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_Notify_PROXY_T_CPP */

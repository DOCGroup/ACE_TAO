// $Id$

#ifndef TAO_ESF_PROXY_ADMIN_CPP
#define TAO_ESF_PROXY_ADMIN_CPP

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/ESF/ESF_Proxy_Admin.i"
#endif /* __ACE_INLINE__ */

#include "orbsvcs/ESF/ESF_Shutdown_Proxy.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<class EVENT_CHANNEL, class PROXY, class INTERFACE>
TAO_ESF_Proxy_Admin<EVENT_CHANNEL,PROXY,INTERFACE>::
    TAO_ESF_Proxy_Admin (EVENT_CHANNEL *ec)
  : event_channel_ (ec)
{
  this->event_channel_->create_proxy_collection (this->collection_);
}

template<class EVENT_CHANNEL, class PROXY, class INTERFACE>
TAO_ESF_Proxy_Admin<EVENT_CHANNEL,PROXY,INTERFACE>::
    ~TAO_ESF_Proxy_Admin (void)
{
  this->event_channel_->destroy_proxy_collection (this->collection_);
}

template<class EVENT_CHANNEL, class PROXY, class INTERFACE> INTERFACE*
TAO_ESF_Proxy_Admin<EVENT_CHANNEL,PROXY,INTERFACE>::
    obtain (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PROXY* proxy;
  this->event_channel_->create_proxy (proxy);

  TAO_ESF_RefCountedRef<PROXY> holder (proxy);

  ACE_TYPENAME PROXY::_ptr_type r;
  proxy->activate (r ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (INTERFACE::_nil ());

  ACE_TYPENAME PROXY::_var_type result = r;

  this->collection_->connected (proxy ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (INTERFACE::_nil ());

  return result._retn ();
}

template<class EVENT_CHANNEL, class PROXY, class INTERFACE> void
TAO_ESF_Proxy_Admin<EVENT_CHANNEL,PROXY,INTERFACE>::
    shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_ESF_Shutdown_Proxy<PROXY> worker;

  this->collection_->for_each (&worker ACE_ENV_ARG_PARAMETER);
  ACE_CHECK; // Cannot happen, just following the discipline.

  this->collection_->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
}

template<class EVENT_CHANNEL, class PROXY, class INTERFACE> void
TAO_ESF_Proxy_Admin<EVENT_CHANNEL,PROXY,INTERFACE>::
    connected (PROXY *
               ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

template<class EVENT_CHANNEL, class PROXY, class INTERFACE> void
TAO_ESF_Proxy_Admin<EVENT_CHANNEL,PROXY,INTERFACE>::
    reconnected (PROXY *proxy
                 ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->collection_->reconnected (proxy ACE_ENV_ARG_PARAMETER);
}

template<class EVENT_CHANNEL, class PROXY, class INTERFACE> void
TAO_ESF_Proxy_Admin<EVENT_CHANNEL,PROXY,INTERFACE>::
    disconnected (PROXY *proxy
                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  proxy->deactivate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK; // Cannot happen, just following the discipline.

  ACE_TRY
    {
      this->collection_->disconnected (proxy ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // @@ In the future the collections may raise exceptions to
      // report errors (such as out of memory or things like that).
      // We must decide how is that info going to be used, and how
      // would we propagate the exception to the application.
      // For example: the CosEC has no exceptions for "out of
      // resources" or something similar, and i've never seen a spec
      // that has an exception for "could not acquire a mutex".
    }
  ACE_ENDTRY;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_ESF_PROXY_ADMIN_CPP */

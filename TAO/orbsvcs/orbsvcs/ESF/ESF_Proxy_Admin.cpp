// $Id$

#ifndef TAO_ESF_PROXY_ADMIN_CPP
#define TAO_ESF_PROXY_ADMIN_CPP

#include "ESF_Proxy_Admin.h"

#if ! defined (__ACE_INLINE__)
#include "ESF_Proxy_Admin.i"
#endif /* __ACE_INLINE__ */

#include "ESF_Shutdown_Proxy.h"

ACE_RCSID(ESF, ESF_Proxy_Admin, "$Id$")

template<class EC, class P>
TAO_ESF_Proxy_Admin<EC,P>::TAO_ESF_Proxy_Admin (EC *ec)
  :  event_channel_ (ec)
{
  this->event_channel_->create_proxy_collection (this->collection_);
}

template<class EC, class P>
TAO_ESF_Proxy_Admin<EC,P>::~TAO_ESF_Proxy_Admin (void)
{
  this->event_channel_->destroy_proxy_collection (this->collection_);
}

template<class EC, class PROXY> ACE_TYPENAME PROXY::_ptr_type
TAO_ESF_Proxy_Admin<EC,PROXY>::obtain (CORBA::Environment &)
  ACE_THROW_SPEC (())
{
  PROXY* proxy;
  this->event_channel_->create_proxy (proxy);

  PortableServer::ServantBase_var holder = proxy;

  ACE_TYPENAME PROXY::_var_type result =
    proxy->activate (ACE_TRY_ENV);
  ACE_CHECK_RETURN (PROXY::Interface::_nil ());

  this->collection_->connected (proxy, ACE_TRY_ENV);
  ACE_CHECK_RETURN (PROXY::Interface::_nil ());

  return result._retn ();
}

template<class EC, class P> void
TAO_ESF_Proxy_Admin<EC,P>::shutdown (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC (())
{
  TAO_ESF_Shutdown_Proxy<P> worker;

  this->collection_->for_each (&worker, ACE_TRY_ENV);
  ACE_CHECK; // Cannot happen, just following the discipline.

  this->collection_->shutdown (ACE_TRY_ENV);
}

template<class EC, class P> void
TAO_ESF_Proxy_Admin<EC,P>::connected (P *,
                                      CORBA::Environment &)
  ACE_THROW_SPEC (())
{
}

template<class EC, class P> void
TAO_ESF_Proxy_Admin<EC,P>::reconnected (P *proxy,
                                        CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC (())
{
  this->collection_->reconnected (proxy, ACE_TRY_ENV);
}

template<class EC, class P> void
TAO_ESF_Proxy_Admin<EC,P>::disconnected (P *proxy,
                                         CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC (())
{
  ACE_TRY
    {
      this->collection_->disconnected (proxy, ACE_TRY_ENV);
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

  proxy->deactivate (ACE_TRY_ENV);
  ACE_CHECK; // Cannot happen, just following the discipline.
}

#endif /* TAO_ESF_PROXY_ADMIN_CPP */

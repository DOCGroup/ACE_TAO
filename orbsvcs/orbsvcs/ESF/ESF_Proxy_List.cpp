// $Id$

#ifndef TAO_ESF_PROXY_LIST_CPP
#define TAO_ESF_PROXY_LIST_CPP

#include "orbsvcs/ESF/ESF_Proxy_List.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/ESF/ESF_Proxy_List.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<class PROXY>
TAO_ESF_Proxy_List<PROXY>::TAO_ESF_Proxy_List (void)
{
}

template<class PROXY> void
TAO_ESF_Proxy_List<PROXY>::connected (PROXY *proxy)
{
  int r = this->impl_.insert (proxy);
  if (r == 0)
    return;

  if (r == 1)
    {
      // @@ Already there, throw some user exception..
      proxy->_decr_refcnt ();
    }
  if (r == -1)
    {
      // @@ Cannot insert, running out of memory? throw some other
      // user exception
      proxy->_decr_refcnt ();
    }
}

template<class PROXY> void
TAO_ESF_Proxy_List<PROXY>::reconnected (PROXY *proxy)
{
  int r = this->impl_.insert (proxy);
  if (r == 0)
    return;

  if (r == 1)
    {
      // Reference count is incremented by the callers to [re]connected.
      // @@ Find out if the protocol could be simplified, and decoupling
      //    increased.
      proxy->_decr_refcnt ();
      return;
    }

  if (r == -1)
    {
      // @@ Cannot insert, running out of memory? throw some other
      // user exception
      proxy->_decr_refcnt ();
    }
}

template<class PROXY> void
TAO_ESF_Proxy_List<PROXY>::disconnected (PROXY *proxy)
{
  int r = this->impl_.remove (proxy);
  if (r != 0)
    {
      // @@ Cannot remove, throw some other
      // user exception
      return;
    }
  proxy->_decr_refcnt ();
}

template<class PROXY> void
TAO_ESF_Proxy_List<PROXY>::shutdown (void)
{
  Iterator end = this->impl_.end ();
  for (Iterator i = this->impl_.begin (); i != end; ++i)
    {
      // Decrement reference count
      (*i)->_decr_refcnt ();
    }
  this->impl_.reset ();
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_ESF_PROXY_LIST_CPP */

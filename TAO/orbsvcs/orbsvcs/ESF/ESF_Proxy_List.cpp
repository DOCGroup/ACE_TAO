#ifndef TAO_ESF_PROXY_LIST_CPP
#define TAO_ESF_PROXY_LIST_CPP

#include "orbsvcs/ESF/ESF_Proxy_List.h"

#include <algorithm>
#include <new>

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/ESF/ESF_Proxy_List.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<class PROXY>
TAO_ESF_Proxy_List<PROXY>::TAO_ESF_Proxy_List ()
{
}

template<class PROXY> void
TAO_ESF_Proxy_List<PROXY>::connected (PROXY *proxy)
{
  if (std::find (this->impl_.begin (), this->impl_.end (), proxy) != this->impl_.end ())
    {
      // @@ Already there, throw some user exception..
      proxy->_decr_refcnt ();
      return;
    }
  try
    {
      this->impl_.push_back (proxy);
    }
  catch (std::bad_alloc const &)
    {
      proxy->_decr_refcnt ();
    }
}

template<class PROXY> void
TAO_ESF_Proxy_List<PROXY>::reconnected (PROXY *proxy)
{
  if (std::find (this->impl_.begin (), this->impl_.end (), proxy) != this->impl_.end ())
    {
      // Reference count is incremented by the callers to [re]connected.
      // @@ Find out if the protocol could be simplified, and decoupling
      //    increased.
      proxy->_decr_refcnt ();
      return;
    }

  try
    {
      this->impl_.push_back (proxy);
    }
  catch (std::bad_alloc const &)
    {
      proxy->_decr_refcnt ();
    }
}

template<class PROXY> void
TAO_ESF_Proxy_List<PROXY>::disconnected (PROXY *proxy)
{
  typename Implementation::iterator i =
    std::find (this->impl_.begin (), this->impl_.end (), proxy);
  if (i == this->impl_.end ())
    {
      // @@ Cannot remove, throw some other
      // user exception
      return;
    }
  this->impl_.erase (i);
  proxy->_decr_refcnt ();
}

template<class PROXY> void
TAO_ESF_Proxy_List<PROXY>::shutdown ()
{
  for (Iterator i = this->impl_.begin (); i != this->impl_.end (); ++i)
    {
      // Decrement reference count
      (*i)->_decr_refcnt ();
    }
  this->impl_.clear ();
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_ESF_PROXY_LIST_CPP */

#ifndef TAO_ESF_PROXY_RB_TREE_CPP
#define TAO_ESF_PROXY_RB_TREE_CPP

#include "orbsvcs/ESF/ESF_Proxy_RB_Tree.h"

#include <new>

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/ESF/ESF_Proxy_RB_Tree.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<class PROXY>
TAO_ESF_Proxy_RB_Tree<PROXY>::TAO_ESF_Proxy_RB_Tree ()
{
}

template<class PROXY> void
TAO_ESF_Proxy_RB_Tree<PROXY>::connected (PROXY *proxy)
{
  try
    {
      if (!this->impl_.insert (proxy).second)
        {
          // @@ Already there, throw some user exception..
          proxy->_decr_refcnt ();
        }
    }
  catch (std::bad_alloc const &)
    {
      proxy->_decr_refcnt ();
    }
}

template<class PROXY> void
TAO_ESF_Proxy_RB_Tree<PROXY>::reconnected (PROXY *proxy)
{
  try
    {
      if (!this->impl_.insert (proxy).second)
        {
          // Reference count is incremented by the callers to [re]connected.
          // @@ Find out if the protocol could be simplified, and decoupling
          //    increased.
          proxy->_decr_refcnt ();
        }
    }
  catch (std::bad_alloc const &)
    {
      proxy->_decr_refcnt ();
    }
}

template<class PROXY> void
TAO_ESF_Proxy_RB_Tree<PROXY>::disconnected (PROXY *proxy)
{
  typename Implementation::iterator i = this->impl_.find (proxy);
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
TAO_ESF_Proxy_RB_Tree<PROXY>::shutdown ()
{
  Iterator end (this->impl_.end ());
  for (Iterator i (this->impl_.begin ()); i != end; ++i)
    {
      // Decrement reference count
      (*i)->_decr_refcnt ();
    }
  this->impl_.clear ();
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_ESF_PROXY_RB_TREE_CPP */

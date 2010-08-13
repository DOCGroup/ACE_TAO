// $Id$

#ifndef TAO_ESF_PROXY_RB_TREE_CPP
#define TAO_ESF_PROXY_RB_TREE_CPP

#include "orbsvcs/ESF/ESF_Proxy_RB_Tree.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/ESF/ESF_Proxy_RB_Tree.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<class PROXY>
TAO_ESF_Proxy_RB_Tree<PROXY>::TAO_ESF_Proxy_RB_Tree (void)
{
}

template<class PROXY> void
TAO_ESF_Proxy_RB_Tree<PROXY>::connected (PROXY *proxy)
{
  int r = this->impl_.bind (proxy, 1);
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
TAO_ESF_Proxy_RB_Tree<PROXY>::reconnected (PROXY *proxy)
{
  int r = this->impl_.rebind (proxy, 1);
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
      proxy->_decr_refcnt ();
    }
}

template<class PROXY> void
TAO_ESF_Proxy_RB_Tree<PROXY>::disconnected (PROXY *proxy)
{
  int r = this->impl_.unbind (proxy);
  if (r != 0)
    {
      // @@ Cannot remove, throw some other
      // user exception
      return;
    }
  proxy->_decr_refcnt ();
}

template<class PROXY> void
TAO_ESF_Proxy_RB_Tree<PROXY>::shutdown (void)
{
  Iterator end = this->impl_.end ();
  for (Iterator i = this->impl_.begin (); i != end; ++i)
    {
      // Decrement reference count
      (*i)->_decr_refcnt ();
    }
  this->impl_.clear ();
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_ESF_PROXY_RB_TREE_CPP */

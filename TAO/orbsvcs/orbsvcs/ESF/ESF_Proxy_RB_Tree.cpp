// $Id$

#ifndef TAO_ESF_PROXY_RB_TREE_CPP
#define TAO_ESF_PROXY_RB_TREE_CPP

#include "ESF_Proxy_RB_Tree.h"

#if ! defined (__ACE_INLINE__)
#include "ESF_Proxy_RB_Tree.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ESF, ESF_Proxy_RB_Tree, "$Id$")

template<class PROXY>
TAO_ESF_Proxy_RB_Tree<PROXY>::
      TAO_ESF_Proxy_RB_Tree (void)
{
}

template<class PROXY> void
TAO_ESF_Proxy_RB_Tree<PROXY>::connected (PROXY *proxy
                                         TAO_ENV_ARG_DECL_NOT_USED)
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
TAO_ESF_Proxy_RB_Tree<PROXY>::reconnected (PROXY *proxy
                                           TAO_ENV_ARG_DECL_NOT_USED)
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
TAO_ESF_Proxy_RB_Tree<PROXY>::disconnected (PROXY *proxy
                                            TAO_ENV_ARG_DECL_NOT_USED)
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
TAO_ESF_Proxy_RB_Tree<PROXY>::shutdown (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  Iterator end = this->impl_.end ();
  for (Iterator i = this->impl_.begin (); i != end; ++i)
    {
      // Decrement reference count
      (*i)->_decr_refcnt ();
    }
  this->impl_.clear ();
}

#endif /* TAO_ESF_PROXY_RB_TREE_CPP */

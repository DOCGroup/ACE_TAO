// $Id$

#ifndef TAO_PORTABLESERVER_SERVANT_VAR_CPP
#define TAO_PORTABLESERVER_SERVANT_VAR_CPP

#include "tao/PortableServer/Servant_var.h"

#if !defined (__ACE_INLINE__)
# include "tao/PortableServer/Servant_var.inl"
#endif /* __ACE_INLINE__ */

#include "tao/Exception.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <class T>
T *
PortableServer::Servant_var<T>::_duplicate (T * p)
{
  try
    {
      if (p != 0)
        {
          p->_add_ref ();
        }
    }
  catch (...)
    {
      throw;
    }

  return p;
}

template <class T>
PortableServer::Servant_var<T>::~Servant_var (void)  /* throw () */
{
  // Unfortunately, there is no throw spec on _remove_ref, so we
  // can't assume that it will not throw.  If it does, then we are in
  // trouble.  In any event, we can't let the exception escape our
  // destructor.
  try
    {
      if (this->ptr_ != 0)
        {
          this->ptr_->_remove_ref ();
        }
    }
  catch (...)
    {
      // Forget the exception..
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /* TAO_PORTABLESERVER_SERVANT_VAR_CPP */

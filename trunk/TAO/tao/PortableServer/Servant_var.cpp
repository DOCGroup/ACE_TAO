// $Id$

#ifndef TAO_PORTABLESERVER_SERVANT_VAR_CPP
#define TAO_PORTABLESERVER_SERVANT_VAR_CPP

#include "tao/PortableServer/Servant_var.h"

#if !defined (__ACE_INLINE__)
# include "tao/PortableServer/Servant_var.inl"
#endif /* __ACE_INLINE__ */

#include "tao/Exception.h"
#include "tao/Environment.h"
#include "ace/CORBA_macros.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <class T>
T *
PortableServer::Servant_var<T>::_duplicate (T * p)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      if (p != 0)
        {
          p->_add_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHALL
    {
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);

  return p;
}

template <class T>
PortableServer::Servant_var<T>::~Servant_var (void)  /* throw () */
{
  // Unfortunately, there is no throw spec on _remove_ref, so we
  // can't assume that it will not throw.  If it does, then we are in
  // trouble.  In any event, we can't let the exception escape our
  // destructor.
  ACE_TRY_NEW_ENV
    {
      if (this->ptr_ != 0)
        {
          this->ptr_->_remove_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHALL
    {
      // Forget the exception..
    }
  ACE_ENDTRY;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /* TAO_PORTABLESERVER_SERVANT_VAR_CPP */

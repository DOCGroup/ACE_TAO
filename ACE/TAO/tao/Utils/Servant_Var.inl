// -*- C++ -*-
//
// $Id$

#include "tao/Exception.h"
#include "ace/Swap.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <class T>
ACE_INLINE T *
TAO::Utils::Servant_Var<T>::_duplicate (T * p)
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
ACE_INLINE void
TAO::Utils::Servant_Var<T>::swap (Servant_Var<T> & rhs) ACE_THROW_SPEC(())
{
  ACE_Swap<T*>::swap (this->ptr_, rhs.ptr_);
}

template <class T>
ACE_INLINE TAO::Utils::Servant_Var<T>::Servant_Var (T * p)
  : ptr_ (p)
{
}

// If _add_ref throws, this object will not be completely constructed
// so the destructor will not be called.
template <class T>
ACE_INLINE TAO::Utils::Servant_Var<T>::Servant_Var (Servant_Var<T> const & rhs)
  : ptr_ (Servant_Var<T>::_duplicate(rhs.ptr_))
{
}

template <class T>
ACE_INLINE TAO::Utils::Servant_Var<T> &
TAO::Utils::Servant_Var<T>::operator= (Servant_Var<T> const & rhs)
{
  TAO::Utils::Servant_Var<T> tmp (rhs);
  this->swap (tmp);
  return *this;
}

template <class T>
ACE_INLINE typename TAO::Utils::Servant_Var<T> &
TAO::Utils::Servant_Var<T>::operator= (T * p)
{
  if (this->ptr_ != p)
    {
      // This constructor doesn't increase the reference count so we
      // we must check for self-assignment.  Otherwise the reference
      // count would be prematurely decremented upon exiting this
      // scope.
      TAO::Utils::Servant_Var<T> tmp (p);
      this->swap (tmp);
    }

  return *this;
}

template <class T> ACE_INLINE
TAO::Utils::Servant_Var<T>::~Servant_Var (void)
  ACE_THROW_SPEC (())
{
  // Unfortunately, there is no throw spec on _remove_ref, so we
  // can't assume that it will not throw.  If it does, then we are in
  // trouble.  In any event, we can't let the exception escape our
  // destructor.
  ACE_TRY_NEW_ENV
    {
      if (ptr_ != 0)
        {
          ptr_->_remove_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHALL
    {
      // Forget the exception..
    }
  ACE_ENDTRY;
}

#if !defined(ACE_LACKS_MEMBER_TEMPLATES)
template <class T> template <class Y>
ACE_INLINE TAO::Utils::Servant_Var<T>::Servant_Var (Y * p)
  : ptr_ (p)
{
}

template <class T> template <class Y>
ACE_INLINE TAO::Utils::Servant_Var<T>::Servant_Var (Servant_Var<Y> const & rhs)
  : ptr_ (Servant_Var<T>::_duplicate (rhs.in ()))
{
}

template <class T> template <class Y>
ACE_INLINE typename TAO::Utils::Servant_Var<T> &
TAO::Utils::Servant_Var<T>::
operator=(Servant_Var<Y> const & rhs)
{
  TAO::Utils::Servant_Var<T> tmp (rhs);
  this->swap (tmp);
  return *this;
}

template <class T> template <class Y>
ACE_INLINE typename TAO::Utils::Servant_Var<T> &
TAO::Utils::Servant_Var<T>::operator= (Y * p)
{
  if (this->ptr_ != p)
    {
      // This constructor doesn't increase the reference count so we
      // we must check for self-assignment.  Otherwise the reference
      // count would be prematurely decremented upon exiting this
      // scope.
      TAO::Utils::Servant_Var<T> tmp (p);
      this->swap (tmp);
    }

  return *this;
}
#endif /* ACE_LACKS_MEMBER_TEMPLATES */

template <class T>
ACE_INLINE T const *
TAO::Utils::Servant_Var<T>::operator->() const
{
  return ptr_;
}

template <class T>
ACE_INLINE T *
TAO::Utils::Servant_Var<T>::operator->()
{
  return ptr_;
}

template <class T>
ACE_INLINE T const & TAO::Utils::Servant_Var<T>::operator*() const
{
  return *ptr_;
}

template <class T>
ACE_INLINE T & TAO::Utils::Servant_Var<T>::operator*()
{
  return *ptr_;
}

template <class T>
ACE_INLINE TAO::Utils::Servant_Var<T>::operator void const * () const
{
  return ptr_;
}

template <class T>
ACE_INLINE T *
TAO::Utils::Servant_Var<T>::in (void) const
{
  return ptr_;
}

template <class T>
ACE_INLINE T *&
TAO::Utils::Servant_Var<T>::inout (void)
{
  return ptr_;
}

template <class T>
ACE_INLINE T *&
TAO::Utils::Servant_Var<T>::out (void)
{
  TAO::Utils::Servant_Var<T> tmp;
  this->swap (tmp);
  return ptr_;
}

template <class T>
ACE_INLINE T *
TAO::Utils::Servant_Var<T>::_retn (void)
{
  T * rval = ptr_;
  ptr_ = 0;
  return rval;
}

#ifndef ACE_LACKS_MEMBER_TEMPLATES
template <class X, class Y>
ACE_INLINE bool
operator== (typename TAO::Utils::Servant_Var<X> const & x,
            typename TAO::Utils::Servant_Var<Y> const & y)
{
  return x.in () == y.in ();
}

template <class X, class Y>
ACE_INLINE bool
operator!= (typename TAO::Utils::Servant_Var<X> const & x,
            typename TAO::Utils::Servant_Var<Y> const & y)
{
  return x.in () != y.in ();
}
#endif /* ! ACE_LACKS_MEMBER_TEMPLATES */

TAO_END_VERSIONED_NAMESPACE_DECL

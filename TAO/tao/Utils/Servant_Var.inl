// $Id$

#include "ace/Swap.h"

template <class T>
ACE_INLINE T * TAO::Utils::Servant_Var<T>::
_duplicate(T * p)
{
  if (p != 0)
  {
    p->_add_ref();
  }
  return p;
}

template <class T>
ACE_INLINE void
TAO::Utils::Servant_Var<T>::swap(Servant_Var<T> & rhs) ACE_THROW_SPEC(())
{
  ACE_Swap<T*>::swap(this->ptr_, rhs.ptr_);
}

template <class T>
ACE_INLINE TAO::Utils::Servant_Var<T>::
Servant_Var(T * p)
  : ptr_(p)
{
}

// If _add_ref throws, this object will not be completely constructed
// so the destructor will not be called.
template <class T>
ACE_INLINE TAO::Utils::Servant_Var<T>::
Servant_Var(Servant_Var<T> const & rhs)
  : ptr_(Servant_Var<T>::_duplicate(rhs.ptr_))
{
}

template <class T>
ACE_INLINE TAO::Utils::Servant_Var<T> & TAO::Utils::Servant_Var<T>::
operator=(Servant_Var<T> const & rhs)
{
  TAO::Utils::Servant_Var<T> tmp(rhs);
  this->swap(tmp);
  return *this;
}

template <class T>
ACE_INLINE ACE_TYPENAME TAO::Utils::Servant_Var<T> & TAO::Utils::Servant_Var<T>::
operator=(T * p)
{
  TAO::Utils::Servant_Var<T> tmp(p);
  this->swap(tmp);
  return *this;
}

template <class T> ACE_INLINE
TAO::Utils::Servant_Var<T>::~Servant_Var ()
  ACE_THROW_SPEC (())
{
  ACE_DECLARE_NEW_CORBA_ENV;
  // Unfortunately, there is no throw spec on _remove_ref, so we
  // can't assume that it will not throw.  If it does, then we are in
  // trouble.  In any event, we can't let the exception escape our
  // destructor.
  if (ptr_ != 0)
  {
    ACE_TRY
      {
        ptr_->_remove_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }
    ACE_CATCHALL
      {
      }
    ACE_ENDTRY;
  }
}

#if !defined(ACE_LACKS_MEMBER_TEMPLATES)
template <class T> template <class Y>
ACE_INLINE TAO::Utils::Servant_Var<T>::
Servant_Var(Y * p)
  : ptr_(p)
{
}

template <class T> template <class Y>
ACE_INLINE TAO::Utils::Servant_Var<T>::
Servant_Var(Servant_Var<Y> const & rhs)
  : ptr_(Servant_Var<T>::_duplicate(rhs.in()))
{
}

template <class T> template <class Y>
ACE_INLINE ACE_TYPENAME TAO::Utils::Servant_Var<T> & TAO::Utils::Servant_Var<T>::
operator=(Servant_Var<Y> const & rhs)
{
  TAO::Utils::Servant_Var<T> tmp(rhs);
  this->swap(tmp);
  return *this;
}

template <class T> template <class Y>
ACE_INLINE ACE_TYPENAME TAO::Utils::Servant_Var<T> & TAO::Utils::Servant_Var<T>::
operator=(Y * p)
{
  TAO::Utils::Servant_Var<T> tmp(p);
  this->swap(tmp);
  return *this;
}
#endif /* ACE_LACKS_MEMBER_TEMPLATES */

template <class T>
ACE_INLINE T const * TAO::Utils::Servant_Var<T>::
operator->() const
{
  return ptr_;
}

template <class T>
ACE_INLINE T * TAO::Utils::Servant_Var<T>::
operator->()
{
  return ptr_;
}

template <class T>
ACE_INLINE T const & TAO::Utils::Servant_Var<T>::
operator*() const
{
  return *ptr_;
}

template <class T>
ACE_INLINE T & TAO::Utils::Servant_Var<T>::
operator*()
{
  return *ptr_;
}

template <class T>
ACE_INLINE TAO::Utils::Servant_Var<T>::
operator void const * () const
{
  return ptr_;
}

template <class T>
ACE_INLINE T * TAO::Utils::Servant_Var<T>::
in() const
{
  return ptr_;
}

template <class T>
ACE_INLINE T *& TAO::Utils::Servant_Var<T>::
inout()
{
  return ptr_;
}

template <class T>
ACE_INLINE T *& TAO::Utils::Servant_Var<T>::
out()
{
  TAO::Utils::Servant_Var<T> tmp;
  this->swap(tmp);
  return ptr_;
}

template <class T>
ACE_INLINE T * TAO::Utils::Servant_Var<T>::
_retn()
{
  T * rval = ptr_;
  ptr_ = 0;
  return rval;
}

template <class X, class Y>
ACE_INLINE bool
operator==(ACE_TYPENAME TAO::Utils::Servant_Var<X> const & x,
           ACE_TYPENAME TAO::Utils::Servant_Var<Y> const & y)
{
  return x.in() == y.in();
}

// -*- C++ -*-
// $Id$
template <class X, class Y>
ACE_INLINE bool
operator!=(ACE_TYPENAME TAO::Utils::Servant_Var<X> const & x,
           ACE_TYPENAME TAO::Utils::Servant_Var<Y> const & y)
{
  return x.in() != y.in();
}

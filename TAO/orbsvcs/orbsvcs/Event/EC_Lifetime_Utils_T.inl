// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <class T>
ACE_INLINE
TAO_EC_Auto_Command<T>::TAO_EC_Auto_Command (void)
  : command_ ()
  , allow_command_ (0)
{
}

template <class T>
ACE_INLINE
TAO_EC_Auto_Command<T>::TAO_EC_Auto_Command (const T & command)
  : command_ (command)
  , allow_command_ (1)
{
}

template <class T>
ACE_INLINE
TAO_EC_Auto_Command<T>::~TAO_EC_Auto_Command (void)
{
  this->execute ();
}

template <class T>
ACE_INLINE void
TAO_EC_Auto_Command<T>::set_command (const T & command)
{
  this->command_ = command;
  this->allow_command_ = 1;
}

template <class T>
ACE_INLINE void
TAO_EC_Auto_Command<T>::set_command (TAO_EC_Auto_Command<T> & auto_command)
{
  if (this == &auto_command)
    return;

  this->command_ = auto_command.command_;
  this->allow_command_ = auto_command.allow_command_;
  auto_command.allow_command_ = 0;
}

template <class T>
ACE_INLINE void
TAO_EC_Auto_Command<T>::execute (void)
{
  if (this->allow_command_)
    {
      this->allow_command_ = 0;

      try
        {
          this->command_.execute ();
        }
      catch (const CORBA::Exception&)
        {
          // ignore.
        }
    }
}

template <class T>
ACE_INLINE void
TAO_EC_Auto_Command<T>::allow_command (void)
{
  this->allow_command_ = 1;
}

template <class T>
ACE_INLINE void
TAO_EC_Auto_Command<T>::disallow_command (void)
{
  this->allow_command_ = 0;
}


//***************************************************************************

template <class T>
ACE_INLINE
TAO_EC_Shutdown_Command<T>::TAO_EC_Shutdown_Command (void)
  : target_ ()
{
}

template <class T>
ACE_INLINE
TAO_EC_Shutdown_Command<T>::TAO_EC_Shutdown_Command (T target)
  : target_ (target)
{
}

template <class T>
ACE_INLINE void
TAO_EC_Shutdown_Command<T>::execute (void)
{
  if (this->target_.in ())
    {
      this->target_->shutdown ();
    }
}

//***************************************************************************


TAO_END_VERSIONED_NAMESPACE_DECL

// Life would be much easier if _add_ref() and _remove_ref()
// had throw specs of "throw ()"

#include <algorithm>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <class T>
ACE_INLINE TAO_EC_Servant_Var<T>::
TAO_EC_Servant_Var(T * p)
  : ptr_(p)
{
}

// If _add_ref throws, this object will not be completely constructed
// so the destructor will not be called.
template <class T>
ACE_INLINE TAO_EC_Servant_Var<T>::
TAO_EC_Servant_Var(TAO_EC_Servant_Var<T> const & rhs)
  : ptr_(rhs.ptr_)
{
  if (ptr_)
    {
      try
        {
          ptr_->_add_ref ();
        }
      catch (...)
        {
          throw;
        }
    }
}

template <class T>
ACE_INLINE TAO_EC_Servant_Var<T> & TAO_EC_Servant_Var<T>::
operator=(TAO_EC_Servant_Var<T> const & rhs)
{
  TAO_EC_Servant_Var<T> tmp(rhs);

  // std::swap(tmp.ptr_, ptr_);
  T * swap_temp = tmp.ptr_;
  tmp.ptr_ = ptr_;
  ptr_ = swap_temp;

  return *this;
}

template <class T>
ACE_INLINE TAO_EC_Servant_Var<T> & TAO_EC_Servant_Var<T>::
operator=(T * p)
{
  TAO_EC_Servant_Var<T> tmp(p);

  // std::swap(tmp.ptr_, ptr_);
  T * swap_temp = tmp.ptr_;
  tmp.ptr_ = ptr_;
  ptr_ = swap_temp;

  return *this;
}

template <class T>
ACE_INLINE TAO_EC_Servant_Var<T>::
~TAO_EC_Servant_Var()
{
  // Unfortunately, there is no throw spec on _remove_ref, so we
  // can't assume that it will not throw.  If it does, then we are in
  // trouble.  In any event, we can't let the exception escape our
  // destructor.
  if (ptr_ != 0)
  {
    try
    {
      ptr_->_remove_ref ();
    }
    catch (...)
    {
    }
  }
}

template <class T> template <class Y>
ACE_INLINE TAO_EC_Servant_Var<T>::
TAO_EC_Servant_Var(Y * p)
  : ptr_(p)
{
}

template <class T> template <class Y>
ACE_INLINE TAO_EC_Servant_Var<T>::
TAO_EC_Servant_Var(TAO_EC_Servant_Var<Y> const & rhs)
  : ptr_(rhs.in())
{
  if (ptr_)
  {
    ptr_->_add_ref();
  }
}

template <class T> template <class Y>
ACE_INLINE TAO_EC_Servant_Var<T> & TAO_EC_Servant_Var<T>::
operator=(TAO_EC_Servant_Var<Y> const & rhs)
{
  TAO_EC_Servant_Var<T> tmp(rhs);

  // std::swap(tmp.ptr_, ptr_);
  T * swap_temp = tmp.ptr_;
  tmp.ptr_ = ptr_;
  ptr_ = swap_temp;

  return *this;
}

template <class T> template <class Y>
ACE_INLINE TAO_EC_Servant_Var<T> & TAO_EC_Servant_Var<T>::
operator=(Y * p)
{
  TAO_EC_Servant_Var<T> tmp(p);

  // std::swap(tmp.ptr_, ptr_);
  T * swap_temp = tmp.ptr_;
  tmp.ptr_ = ptr_;
  ptr_ = swap_temp;

  return *this;
}

template <class T>
ACE_INLINE T const * TAO_EC_Servant_Var<T>::
operator->() const
{
  return ptr_;
}

template <class T>
ACE_INLINE T * TAO_EC_Servant_Var<T>::
operator->()
{
  return ptr_;
}

template <class T>
ACE_INLINE T const & TAO_EC_Servant_Var<T>::
operator*() const
{
  return *ptr_;
}

template <class T>
ACE_INLINE T & TAO_EC_Servant_Var<T>::
operator*()
{
  return *ptr_;
}

template <class T>
ACE_INLINE TAO_EC_Servant_Var<T>::
operator void const * () const
{
  return ptr_;
}

template <class T>
ACE_INLINE T * TAO_EC_Servant_Var<T>::
in() const
{
  return ptr_;
}

template <class T>
ACE_INLINE T *& TAO_EC_Servant_Var<T>::
inout()
{
  return ptr_;
}

template <class T>
ACE_INLINE T *& TAO_EC_Servant_Var<T>::
out()
{
  TAO_EC_Servant_Var<T> tmp;

  // std::swap(tmp.ptr_, ptr_);
  T * swap_temp = tmp.ptr_;
  tmp.ptr_ = ptr_;
  ptr_ = swap_temp;

  return ptr_;
}

template <class T>
ACE_INLINE T * TAO_EC_Servant_Var<T>::
_retn()
{
  T * rval = ptr_;
  ptr_ = 0;
  return rval;
}

template <class X, class Y>
ACE_INLINE bool
operator==(TAO_EC_Servant_Var<X> const & x,
           TAO_EC_Servant_Var<Y> const & y)
{
  return x.in() == y.in();
}

template <class X, class Y>
ACE_INLINE bool
operator!=(TAO_EC_Servant_Var<X> const & x,
           TAO_EC_Servant_Var<Y> const & y)
{
  return x.in() != y.in();
}

TAO_END_VERSIONED_NAMESPACE_DECL

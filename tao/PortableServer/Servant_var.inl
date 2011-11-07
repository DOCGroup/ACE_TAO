// -*- C++ -*-
//
// $Id$

#include <algorithm>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <class T>
ACE_INLINE void
PortableServer::Servant_var<T>::swap (Servant_var<T> & rhs) /* throw () */
{
  std::swap (this->ptr_, rhs.ptr_);
}

template <class T>
ACE_INLINE
PortableServer::Servant_var<T>::Servant_var (T * p)
  : ptr_ (p)
{
}

// If _add_ref throws, this object will not be completely constructed
// so the destructor will not be called.
template <class T>
ACE_INLINE
PortableServer::Servant_var<T>::Servant_var (Servant_var<T> const & rhs)
  : ptr_ (Servant_var<T>::_duplicate(rhs.ptr_))
{
}

template <class T>
ACE_INLINE PortableServer::Servant_var<T> &
PortableServer::Servant_var<T>::operator= (Servant_var<T> const & rhs)
{
  PortableServer::Servant_var<T> tmp (rhs);
  this->swap (tmp);
  return *this;
}

template <class T>
ACE_INLINE typename PortableServer::Servant_var<T> &
PortableServer::Servant_var<T>::operator= (T * p)
{
  if (this->ptr_ != p)
    {
      // This constructor doesn't increase the reference count, nor is
      // it a copy constructor, so we must check for self-assignment.
      // Otherwise the reference count would be prematurely
      // decremented upon exiting this scope.
      PortableServer::Servant_var<T> tmp (p);
      this->swap (tmp);
    }

  return *this;
}

template <class T>
ACE_INLINE T *
PortableServer::Servant_var<T>::operator->() const
{
  return this->ptr_;
}

template <class T>
ACE_INLINE T const &
PortableServer::Servant_var<T>::operator*() const
{
  return *this->ptr_;
}

template <class T>
ACE_INLINE T &
PortableServer::Servant_var<T>::operator*()
{
  return *this->ptr_;
}

template <class T>
ACE_INLINE PortableServer::Servant_var<T>::operator void const * () const
{
  return this->ptr_;
}

template <class T>
ACE_INLINE T *
PortableServer::Servant_var<T>::in (void) const
{
  return this->ptr_;
}

template <class T>
ACE_INLINE T *&
PortableServer::Servant_var<T>::inout (void)
{
  return this->ptr_;
}

template <class T>
ACE_INLINE T *&
PortableServer::Servant_var<T>::out (void)
{
  PortableServer::Servant_var<T> tmp;
  this->swap (tmp);
  return this->ptr_;
}

template <class T>
ACE_INLINE T *
PortableServer::Servant_var<T>::_retn (void)
{
  T * const rval = ptr_;
  this->ptr_ = 0;
  return rval;
}

template <class X, class Y>
ACE_INLINE bool
operator== (typename PortableServer::Servant_var<X> const & x,
            typename PortableServer::Servant_var<Y> const & y)
{
  return x.in () == y.in ();
}

template <class X, class Y>
ACE_INLINE bool
operator!= (typename PortableServer::Servant_var<X> const & x,
            typename PortableServer::Servant_var<Y> const & y)
{
  return x.in () != y.in ();
}

TAO_END_VERSIONED_NAMESPACE_DECL

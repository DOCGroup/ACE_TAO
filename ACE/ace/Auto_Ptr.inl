// -*- C++ -*-
#include "ace/Global_Macros.h"

#if !defined (ACE_HAS_CPP17)

#if defined (ACE_HAS_ALLOC_HOOKS)
# include "ace/Malloc_Base.h"
#endif /* ACE_HAS_ALLOC_HOOKS */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

template<class X> ACE_INLINE void
ACE_Auto_Basic_Ptr<X>::dump () const
{
#if defined (ACE_HAS_DUMP)
  ACE_TRACE ("ACE_Auto_Basic_Ptr<X>::dump");
#endif /* ACE_HAS_DUMP */
}

template<class X> ACE_INLINE void
ACE_Auto_Basic_Array_Ptr<X>::dump () const
{
#if defined (ACE_HAS_DUMP)
  ACE_TRACE ("ACE_Auto_Basic_Array_Ptr<X>::dump");
#endif /* ACE_HAS_DUMP */
}

template<class X> ACE_INLINE
ACE_Auto_Basic_Ptr<X>::ACE_Auto_Basic_Ptr (ACE_Auto_Basic_Ptr<X> &rhs)
  : p_ (rhs.release ())
{
  ACE_TRACE ("ACE_Auto_Basic_Ptr<X>::ACE_Auto_Basic_Ptr");
}

template<class X> ACE_INLINE X *
ACE_Auto_Basic_Ptr<X>::get () const
{
  ACE_TRACE ("ACE_Auto_Basic_Ptr<X>::get");
  return this->p_;
}

template<class X> ACE_INLINE X *
ACE_Auto_Basic_Ptr<X>::release ()
{
  ACE_TRACE ("ACE_Auto_Basic_Ptr<X>::release");
  X *old = this->p_;
  this->p_ = nullptr;
  return old;
}

template<class X> ACE_INLINE void
ACE_Auto_Basic_Ptr<X>::reset (X *p)
{
  ACE_TRACE ("ACE_Auto_Basic_Ptr<X>::reset");
  if (this->get () != p)
    delete this->get ();
  this->p_ = p;
}

template<class X> ACE_INLINE ACE_Auto_Basic_Ptr<X> &
ACE_Auto_Basic_Ptr<X>::operator= (ACE_Auto_Basic_Ptr<X> &rhs)
{
  ACE_TRACE ("ACE_Auto_Basic_Ptr<X>::operator=");
  if (this != &rhs)
    {
      this->reset (rhs.release ());
    }
  return *this;
}

template<class X> ACE_INLINE
ACE_Auto_Basic_Ptr<X>::~ACE_Auto_Basic_Ptr ()
{
  ACE_TRACE ("ACE_Auto_Basic_Ptr<X>::~ACE_Auto_Basic_Ptr");
  delete this->get ();
}

template<class X> ACE_INLINE X &
ACE_Auto_Basic_Ptr<X>::operator *() const
{
  ACE_TRACE ("ACE_Auto_Basic_Ptr<X>::operator *()");
  return *this->get ();
}

#if defined (ACE_LACKS_AUTO_PTR)
template<class X> ACE_INLINE X *
auto_ptr<X>::operator-> () const
{
  ACE_TRACE ("auto_ptr<X>::operator->");
  return this->get ();
}
#endif /* ACE_LACKS_AUTO_PTR */

template<class X> ACE_INLINE X *
ACE_Auto_Ptr<X>::operator-> () const
{
  ACE_TRACE ("ACE_Auto_Ptr<X>::operator->");
  return this->get ();
}

template<class X> ACE_INLINE X *
ACE_Auto_Basic_Array_Ptr<X>::get () const
{
  ACE_TRACE ("ACE_Auto_Basic_Array_Ptr<X>::get");
  return this->p_;
}

template<class X> ACE_INLINE X *
ACE_Auto_Basic_Array_Ptr<X>::release ()
{
  ACE_TRACE ("ACE_Auto_Basic_Array_Ptr<X>::release");
  X *old = this->p_;
  this->p_ = 0;
  return old;
}

template<class X> ACE_INLINE void
ACE_Auto_Basic_Array_Ptr<X>::reset (X *p)
{
  ACE_TRACE ("ACE_Auto_Basic_Array_Ptr<X>::reset");
  if (this->get () != p)
#if defined (ACE_HAS_ALLOC_HOOKS)
    ACE_Allocator::instance()->free(this->get ());
#else
    delete [] this->get ();
#endif /* ACE_HAS_ALLOC_HOOKS */

  this->p_ = p;
}

template<class X> ACE_INLINE
ACE_Auto_Basic_Array_Ptr<X>::ACE_Auto_Basic_Array_Ptr (ACE_Auto_Basic_Array_Ptr<X> &rhs)
  : p_ (rhs.release ())
{
  ACE_TRACE ("ACE_Auto_Basic_Array_Ptr<X>::ACE_Auto_Basic_Array_Ptr");
}

template<class X> ACE_INLINE ACE_Auto_Basic_Array_Ptr<X> &
ACE_Auto_Basic_Array_Ptr<X>::operator= (ACE_Auto_Basic_Array_Ptr<X> &rhs)
{
  ACE_TRACE ("ACE_Auto_Basic_Array_Ptr<X>::operator=");
  if (this != &rhs)
    {
      this->reset (rhs.release ());
    }
  return *this;
}

template<class X> ACE_INLINE
ACE_Auto_Basic_Array_Ptr<X>::~ACE_Auto_Basic_Array_Ptr ()
{
  ACE_TRACE ("ACE_Auto_Basic_Array_Ptr<X>::~ACE_Auto_Basic_Array_Ptr");
#if defined (ACE_HAS_ALLOC_HOOKS)
  ACE_Allocator::instance()->free(this->get ());
#else
  delete [] this->get ();
#endif /* ACE_HAS_ALLOC_HOOKS */
}

template<class X> ACE_INLINE X &
ACE_Auto_Basic_Array_Ptr<X>::operator *() const
{
  return *this->get ();
}

template<class X> ACE_INLINE X &
ACE_Auto_Basic_Array_Ptr<X>::operator[](int i) const
{
  X *array = this->get ();
  return array[i];
}

template<class X> ACE_INLINE X *
ACE_Auto_Array_Ptr<X>::operator->() const
{
  return this->get ();
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_CPP17 */

/* -*- C++ -*- */
// $Id$

// Auto_Ptr.i

#include "Synch_T.h"

template<class X> ACE_INLINE
ACE_Auto_Basic_Ptr<X>::ACE_Auto_Basic_Ptr (ACE_Auto_Basic_Ptr<X> &rhs)
  : p_ (rhs.release ())
{
  ACE_TRACE ("ACE_Auto_Basic_Ptr<X>::ACE_Auto_Basic_Ptr");
}

template<class X> ACE_INLINE X *
ACE_Auto_Basic_Ptr<X>::get (void) const
{
  ACE_TRACE ("ACE_Auto_Basic_Ptr<X>::get");
  return this->p_;
}

template<class X> ACE_INLINE X *
ACE_Auto_Basic_Ptr<X>::release (void)
{
  ACE_TRACE ("ACE_Auto_Basic_Ptr<X>::release");
  X *old = this->p_;
  this->p_ = 0;
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
ACE_Auto_Basic_Ptr<X>::~ACE_Auto_Basic_Ptr (void)
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

#if defined (ACE_LACKS_AUTO_PTR) || \
    !defined (ACE_HAS_STANDARD_CPP_LIBRARY) || \
             (ACE_HAS_STANDARD_CPP_LIBRARY == 0)

template<class X> ACE_INLINE X *
auto_ptr<X>::operator-> () const
{
  ACE_TRACE ("auto_ptr<X>::operator->");
  return this->get ();
}

#endif /* ACE_HAS_STANDARD_CPP_LIBRARY */

template<class X> ACE_INLINE X *
ACE_Auto_Basic_Array_Ptr<X>::get (void) const
{
  ACE_TRACE ("ACE_Auto_Basic_Array_Ptr<X>::get");
  return this->p_;
}

template<class X> ACE_INLINE X *
ACE_Auto_Basic_Array_Ptr<X>::release (void)
{
  ACE_TRACE ("ACE_Auto_Basic_Array_Ptr<X>::release");
  X *old = this->p_;
  this->p_ = 0;
  return old;
}

template<class X> ACE_INLINE void
ACE_Auto_Basic_Array_Ptr<X>::reset (X *p)
{
  ACE_TRACE ("ACE_Auto_Basic_Ptr<X>::reset");
  if (this->get () != p)
    delete [] this->get ();
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
ACE_Auto_Basic_Array_Ptr<X>::~ACE_Auto_Basic_Array_Ptr (void)
{
  ACE_TRACE ("ACE_Auto_Basic_Array_Ptr<X>::~ACE_Auto_Basic_Array_Ptr");
  delete [] this->get ();
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

template <class X, class ACE_LOCK> ACE_INLINE ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK> *
ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK>::create (X *p)
{
  // Yes set ref count to zero.
  return new ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK> (p);
}

template <class X, class ACE_LOCK> ACE_INLINE ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK> *
ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK>::attach (ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK>*& rep)
{
  ACE_ASSERT (rep != 0);

  ACE_GUARD_RETURN (ACE_LOCK, guard, rep->lock_, rep);

  ++rep->ref_count_;

  return rep;
}

template <class X, class ACE_LOCK> ACE_INLINE void
ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK>::detach (ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK>*& rep)
{
  ACE_ASSERT (rep != 0);

  ACE_GUARD (ACE_LOCK, guard, rep->lock_);

  if (rep->ref_count_-- == 0)
    // We do not need the lock when deleting the representation.
    // There should be no side effects from deleting rep and we don
    // not want to release a deleted mutex.
    delete rep;
}

template <class X, class ACE_LOCK> ACE_INLINE void
ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK>::assign (ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK>*& rep, ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK>* new_rep)
{
  ACE_ASSERT (rep != 0);
  ACE_ASSERT (new_rep != 0);

  ACE_GUARD (ACE_LOCK, guard, rep->lock_);

  ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK> *old = rep;
  rep = new_rep;

  // detached old last for exception safety
  if(old->ref_count_-- == 0)
    // We do not need the lock when deleting the representation.
    // There should be no side effects from deleting rep and we don
    // not want to release a deleted mutex.
    delete old;
}

template <class X, class ACE_LOCK> ACE_INLINE
ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK>::ACE_Refcounted_Auto_Ptr_Rep (X *p)
  : ptr_ (p),
    ref_count_ (0)
{
}

template <class X, class ACE_LOCK> ACE_INLINE
ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK>::~ACE_Refcounted_Auto_Ptr_Rep (void)
{
}

template<class X, class ACE_LOCK> ACE_INLINE X *
ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK>::release (void)
{
  ACE_GUARD_RETURN (ACE_LOCK, guard, this->lock_, 0);

  return this->ptr_.release ();
}

template<class X, class ACE_LOCK> ACE_INLINE void
ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK>::reset (X *p)
{
  ACE_GUARD (ACE_LOCK, guard, this->lock_);

  this->ptr_.reset (p);
}

template <class X, class ACE_LOCK> X *
ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK>::get (void)
{
  ACE_GUARD_RETURN (ACE_LOCK, guard, this->lock_, 0);

  return this->ptr_.get ();
}

template <class X, class ACE_LOCK>
ACE_Refcounted_Auto_Ptr<X, ACE_LOCK>::ACE_Refcounted_Auto_Ptr (X *p)
  : rep_ (AUTO_REFCOUNTED_PTR_REP::create (p))
{
}

template <class X, class ACE_LOCK>
ACE_Refcounted_Auto_Ptr<X, ACE_LOCK>::ACE_Refcounted_Auto_Ptr (const ACE_Refcounted_Auto_Ptr<X, ACE_LOCK> &r)
  : rep_ (AUTO_REFCOUNTED_PTR_REP::attach (((ACE_Refcounted_Auto_Ptr<X, ACE_LOCK> &) r).rep_))
{
}

template <class X, class ACE_LOCK>
ACE_Refcounted_Auto_Ptr<X, ACE_LOCK>::~ACE_Refcounted_Auto_Ptr (void)
{
  AUTO_REFCOUNTED_PTR_REP::detach (rep_);
}

template <class X, class ACE_LOCK> int
ACE_Refcounted_Auto_Ptr<X, ACE_LOCK>::operator== (const ACE_Refcounted_Auto_Ptr<X, ACE_LOCK> &r) const
{
  return r.rep_ == this->rep_;
}

template <class X, class ACE_LOCK> int
ACE_Refcounted_Auto_Ptr<X, ACE_LOCK>::operator!= (const ACE_Refcounted_Auto_Ptr<X, ACE_LOCK> &r) const
{
  return r.rep_ != this->rep_;
}

template <class X, class ACE_LOCK> X *
ACE_Refcounted_Auto_Ptr<X, ACE_LOCK>::operator-> (void) const
{
    return this->rep_->get();
}

template<class X, class ACE_LOCK> ACE_INLINE X &
ACE_Refcounted_Auto_Ptr<X, ACE_LOCK>::operator *() const
{
  return *this->rep_->get ();
}

template <class X, class ACE_LOCK> X*
ACE_Refcounted_Auto_Ptr<X, ACE_LOCK>::get (void)
{
  // We return the ACE_Future_rep.
  return this->rep_->get ();
}

template<class X, class ACE_LOCK> ACE_INLINE X *
ACE_Refcounted_Auto_Ptr<X, ACE_LOCK>::release (void)
{
  return this->rep_->release ();
}

template<class X, class ACE_LOCK> ACE_INLINE void
ACE_Refcounted_Auto_Ptr<X, ACE_LOCK>::reset (X *p)
{
  this->rep_->reset (p);
}

template <class X, class ACE_LOCK> void
ACE_Refcounted_Auto_Ptr<X, ACE_LOCK>::operator = (const ACE_Refcounted_Auto_Ptr<X, ACE_LOCK> &rhs)
{
  // assignment:
  //
  //  bind <this> to the same <ACE_Refcounted_Auto_Ptr_Rep> as <r>.

  // This will work if &r == this, by first increasing the ref count
  ACE_Refcounted_Auto_Ptr<X, ACE_LOCK> &r = (ACE_Refcounted_Auto_Ptr<X, ACE_LOCK> &) rhs;
  AUTO_REFCOUNTED_PTR_REP::assign (this->rep_,
                                   AUTO_REFCOUNTED_PTR_REP::attach (r.rep_));
}


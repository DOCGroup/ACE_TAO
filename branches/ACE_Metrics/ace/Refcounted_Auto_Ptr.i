/* -*- C++ -*- */
// $Id$

// Refcounted_Auto_Ptr.i

#include "Synch_T.h"
#include "ace/Log_Msg.h"

template <class X, class ACE_LOCK> inline int
ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK>::count (void) const
{
  ACE_GUARD_RETURN (ACE_LOCK, guard, ACE_const_cast (ACE_LOCK &, this->lock_), 0);
  return this->ref_count_;
}

template <class X, class ACE_LOCK> inline int
ACE_Refcounted_Auto_Ptr<X, ACE_LOCK>::count (void) const
{
  return this->rep_->count ();
}

template <class X, class ACE_LOCK> inline int
ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK>::null (void) const
{
  ACE_GUARD_RETURN (ACE_LOCK, guard,
                    ACE_const_cast (ACE_LOCK&, this->lock_), 0);

  return this->ptr_.get() == 0;
}

template <class X, class ACE_LOCK> inline int
ACE_Refcounted_Auto_Ptr<X, ACE_LOCK>::null (void) const
{
  return this->rep_->null ();
}

template <class X, class ACE_LOCK> inline ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK> *
ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK>::internal_create (X *p)
{
  ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK> *temp = 0;
  ACE_NEW_RETURN (temp,
                  (ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK>) (p),
                  0);
  return temp;
}

template <class X, class ACE_LOCK> inline ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK> *
ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK>::create (X *p)
{
  // Yes set ref count to zero.
  ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK> *temp = internal_create (p);
#if defined (ACE_NEW_THROWS_EXCEPTIONS)
  if (temp == 0)
    ACE_throw_bad_alloc;
#else
  ACE_ASSERT (temp != 0);
#endif /* ACE_NEW_THROWS_EXCEPTIONS */
   return temp;
}

template <class X, class ACE_LOCK> inline ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK> *
ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK>::attach (ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK>*& rep)
{
  ACE_ASSERT (rep != 0);

  ACE_GUARD_RETURN (ACE_LOCK, guard, rep->lock_, rep);

  ++rep->ref_count_;

  return rep;
}

template <class X, class ACE_LOCK> inline void
ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK>::detach (ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK>*& rep)
{
  ACE_ASSERT (rep != 0);
  ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK> *rep_del = 0;

  {
    ACE_GUARD (ACE_LOCK, guard, rep->lock_);

    if (rep->ref_count_-- == 0)
      // Since rep contains the lock held by the ACE_Guard, the guard
      // needs to be released before freeing the memory holding the
      // lock. So save the pointer to free, then release, then free.
      rep_del = rep;
  }  // Release the lock
  if (0 != rep_del)
    delete rep;
}

template <class X, class ACE_LOCK> inline void
ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK>::assign (ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK>*& rep,
                                                  ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK>* new_rep)
{
  ACE_ASSERT (rep != 0);
  ACE_ASSERT (new_rep != 0);

  ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK> *old = 0;
  {
    // detached old last for exception safety
    ACE_GUARD (ACE_LOCK, guard, rep->lock_);
    old = rep;
    rep = new_rep;
 
    if (old->ref_count_-- > 0)
      return;
 
  } // The lock is released before deleting old rep object below.
 
  delete old;
}

template <class X, class ACE_LOCK> inline
ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK>::ACE_Refcounted_Auto_Ptr_Rep (X *p)
  : ptr_ (p),
    ref_count_ (0)
{
}

template <class X, class ACE_LOCK> inline
ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK>::~ACE_Refcounted_Auto_Ptr_Rep (void)
{
}

template<class X, class ACE_LOCK> inline X *
ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK>::release (void)
{
  ACE_GUARD_RETURN (ACE_LOCK, guard, this->lock_, 0);

  return this->ptr_.release ();
}

template<class X, class ACE_LOCK> inline void
ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK>::reset (X *p)
{
  ACE_GUARD (ACE_LOCK, guard, this->lock_);

  this->ptr_.reset (p);
}

template <class X, class ACE_LOCK> inline X *
ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK>::get (void) const
{
  ACE_GUARD_RETURN (ACE_LOCK, guard, ACE_const_cast (ACE_LOCK &, this->lock_), 0);

  return this->ptr_.get ();
}

template <class X, class ACE_LOCK> inline
ACE_Refcounted_Auto_Ptr<X, ACE_LOCK>::ACE_Refcounted_Auto_Ptr (X *p)
  : rep_ (AUTO_REFCOUNTED_PTR_REP::create (p))
{
}

template <class X, class ACE_LOCK> inline
ACE_Refcounted_Auto_Ptr<X, ACE_LOCK>::ACE_Refcounted_Auto_Ptr (const ACE_Refcounted_Auto_Ptr<X, ACE_LOCK> &r)
  : rep_ (AUTO_REFCOUNTED_PTR_REP::attach (((ACE_Refcounted_Auto_Ptr<X, ACE_LOCK> &) r).rep_))
{
}

template <class X, class ACE_LOCK> inline
ACE_Refcounted_Auto_Ptr<X, ACE_LOCK>::~ACE_Refcounted_Auto_Ptr (void)
{
  AUTO_REFCOUNTED_PTR_REP::detach (rep_);
}

template <class X, class ACE_LOCK> inline int
ACE_Refcounted_Auto_Ptr<X, ACE_LOCK>::operator== (const ACE_Refcounted_Auto_Ptr<X, ACE_LOCK> &r) const
{
  return r.rep_ == this->rep_;
}

template <class X, class ACE_LOCK> inline int
ACE_Refcounted_Auto_Ptr<X, ACE_LOCK>::operator!= (const ACE_Refcounted_Auto_Ptr<X, ACE_LOCK> &r) const
{
  return r.rep_ != this->rep_;
}

template <class X, class ACE_LOCK> inline X *
ACE_Refcounted_Auto_Ptr<X, ACE_LOCK>::operator-> (void) const
{
    return this->rep_->get();
}

template<class X, class ACE_LOCK> inline X &
ACE_Refcounted_Auto_Ptr<X, ACE_LOCK>::operator *() const
{
  return *this->rep_->get ();
}

template <class X, class ACE_LOCK> inline X*
ACE_Refcounted_Auto_Ptr<X, ACE_LOCK>::get (void) const
{
  // We return the ACE_Future_rep.
  return this->rep_->get ();
}

template<class X, class ACE_LOCK> inline X *
ACE_Refcounted_Auto_Ptr<X, ACE_LOCK>::release (void)
{
  return this->rep_->release ();
}

template<class X, class ACE_LOCK> inline void
ACE_Refcounted_Auto_Ptr<X, ACE_LOCK>::reset (X *p)
{
  this->rep_->reset (p);
}

template <class X, class ACE_LOCK> inline void
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


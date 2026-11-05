// -*- C++ -*-
#include "ace/Guard_T.h"
#include "ace/Log_Category.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

template <class X, class ACE_LOCK> inline long
ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK>::count () const
{
  return this->ref_count_.value();
}

template <class X, class ACE_LOCK> inline long
ACE_Refcounted_Auto_Ptr<X, ACE_LOCK>::count () const
{
  return this->rep_ == nullptr ? 0 : this->rep_->count ();
}

template <class X, class ACE_LOCK> inline bool
ACE_Refcounted_Auto_Ptr<X, ACE_LOCK>::null () const
{
  return this->rep_ == nullptr || this->rep_->get () == nullptr;
}

template <class X, class ACE_LOCK> inline ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK> *
ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK>::internal_create (X *p)
{
  ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK> *temp = nullptr;
  ACE_NEW_RETURN (temp,
                  (ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK>) (p),
                  nullptr);
  return temp;
}

template <class X, class ACE_LOCK> inline ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK> *
ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK>::create (X *p)
{
  // Yes set ref count to zero.
  ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK> *temp = internal_create (p);
  if (!temp)
    throw std::bad_alloc ();
   return temp;
}

template <class X, class ACE_LOCK> inline ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK> *
ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK>::attach (
  ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK> *rep)
{
  if (rep == nullptr)
    return nullptr;

  ++rep->ref_count_;

  return rep;
}

template <class X, class ACE_LOCK> inline void
ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK>::detach (
  ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK> *&rep)
{
  if (rep == nullptr)
    return;

  if (rep->ref_count_-- == 0)
    delete rep;
}

template <class X, class ACE_LOCK> inline
ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK>::ACE_Refcounted_Auto_Ptr_Rep (X *p)
  : ptr_ (p),
    ref_count_ (0)
{
}

template <class X, class ACE_LOCK> inline
ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK>::~ACE_Refcounted_Auto_Ptr_Rep ()
{
}

template <class X, class ACE_LOCK> inline X *
ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK>::get () const
{
  return this->ptr_.get ();
}

template <class X, class ACE_LOCK> inline
ACE_Refcounted_Auto_Ptr<X, ACE_LOCK>::ACE_Refcounted_Auto_Ptr (X *p)
  : rep_ (AUTO_REFCOUNTED_PTR_REP::create (p))
{
}

template <class X, class ACE_LOCK> inline
ACE_Refcounted_Auto_Ptr<X, ACE_LOCK>::ACE_Refcounted_Auto_Ptr (
  ACE_Refcounted_Auto_Ptr<X, ACE_LOCK> const &r)
  : rep_ (AUTO_REFCOUNTED_PTR_REP::attach (r.rep_))
{
}

template <class X, class ACE_LOCK> inline bool
ACE_Refcounted_Auto_Ptr<X, ACE_LOCK>::operator== (
  ACE_Refcounted_Auto_Ptr<X, ACE_LOCK> const &r) const
{
  return r.rep_ == this->rep_;
}

template <class X, class ACE_LOCK> inline bool
ACE_Refcounted_Auto_Ptr<X, ACE_LOCK>::operator!= (
  ACE_Refcounted_Auto_Ptr<X, ACE_LOCK> const &r) const
{
  return r.rep_ != this->rep_;
}

template <class X, class ACE_LOCK> inline X *
ACE_Refcounted_Auto_Ptr<X, ACE_LOCK>::operator-> () const
{
  return this->get ();
}

template<class X, class ACE_LOCK> inline X &
ACE_Refcounted_Auto_Ptr<X, ACE_LOCK>::operator *() const
{
  return *this->get ();
}

template<class X, class ACE_LOCK> inline bool
ACE_Refcounted_Auto_Ptr<X, ACE_LOCK>::operator !() const
{
  return this->null ();
}

template<class X, class ACE_LOCK> inline
ACE_Refcounted_Auto_Ptr<X, ACE_LOCK>::operator bool () const
{
  return !this->null ();
}

template <class X, class ACE_LOCK> inline X *
ACE_Refcounted_Auto_Ptr<X, ACE_LOCK>::get () const
{
  return this->rep_ == nullptr ? nullptr : this->rep_->get ();
}

template<class X, class ACE_LOCK> inline X *
ACE_Refcounted_Auto_Ptr<X, ACE_LOCK>::release ()
{
  X *p = this->get ();
  AUTO_REFCOUNTED_PTR_REP::detach (this->rep_);
  this->rep_ = nullptr;
  return p;
}

template<class X, class ACE_LOCK> inline void
ACE_Refcounted_Auto_Ptr<X, ACE_LOCK>::reset (X *p)
{
  // Avoid deleting the underlying auto_ptr if assigning the same actual
  // pointer value.
  if (this->get () == p)
    return;

  AUTO_REFCOUNTED_PTR_REP *old_rep = this->rep_;
  if ((this->rep_ = AUTO_REFCOUNTED_PTR_REP::create (p)) != nullptr)
    AUTO_REFCOUNTED_PTR_REP::detach (old_rep);
  else
    this->rep_ = old_rep;
  return;
}

template <class X, class ACE_LOCK> inline void
ACE_Refcounted_Auto_Ptr<X, ACE_LOCK>::operator = (
  ACE_Refcounted_Auto_Ptr<X, ACE_LOCK> const &rhs)
{
  //  bind <this> to the same <ACE_Refcounted_Auto_Ptr_Rep> as <r>.
  AUTO_REFCOUNTED_PTR_REP *old_rep = this->rep_;
  if (rhs.rep_ != nullptr)
    {
      this->rep_ = AUTO_REFCOUNTED_PTR_REP::attach (rhs.rep_);
      if (this->rep_ != nullptr)
        AUTO_REFCOUNTED_PTR_REP::detach (old_rep);
    }
  else    // Assign a null rep to this
    {
      AUTO_REFCOUNTED_PTR_REP::detach (old_rep);
      this->rep_ = nullptr;
    }
}

ACE_END_VERSIONED_NAMESPACE_DECL

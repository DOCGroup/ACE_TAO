/* -*- C++ -*- */
// $Id$

// Auto_Ptr.i

template<class X> ACE_INLINE
ACE_Auto_Basic_Ptr<X>::ACE_Auto_Basic_Ptr (X *p)
  : p_ (p)
{
  ACE_TRACE ("ACE_Auto_Basic_Ptr<X>::ACE_Auto_Basic_Ptr");
}

template<class X> ACE_INLINE
ACE_Auto_Basic_Ptr<X>::ACE_Auto_Basic_Ptr (const ACE_Auto_Basic_Ptr<X> &ap)
  : p_ (ap.release ()) 
{
  ACE_TRACE ("ACE_Auto_Basic_Ptr<X>::ACE_Auto_Basic_Ptr");
}

template<class X> ACE_INLINE ACE_Auto_Basic_Ptr<X> &
ACE_Auto_Basic_Ptr<X>::operator= (const ACE_Auto_Basic_Ptr<X> &rhs)
{
  ACE_TRACE ("ACE_Auto_Basic_Ptr<X>::operator=");
  if (this != &rhs) 
    {
      delete this->p_;
      this->p_ = rhs.release ();
    }
  return *this;
}

template<class X> ACE_INLINE
ACE_Auto_Basic_Ptr<X>::~ACE_Auto_Basic_Ptr (void) 
{ 
  ACE_TRACE ("ACE_Auto_Basic_Ptr<X>::~ACE_Auto_Basic_Ptr");
  delete p_; 
}

template<class X> ACE_INLINE X &
ACE_Auto_Basic_Ptr<X>::operator *() const 
{
  ACE_TRACE ("ACE_Auto_Basic_Ptr<X>::operator *()");
  return *p_;
}

template<class X> ACE_INLINE X *
ACE_Auto_Basic_Ptr<X>::get (void) const 
{
  ACE_TRACE ("ACE_Auto_Basic_Ptr<X>::get");
  return p_;
}

template<class X> ACE_INLINE X *
ACE_Auto_Basic_Ptr<X>::release (void) const
{
  ACE_TRACE ("ACE_Auto_Basic_Ptr<X>::release");
  X *old = this->p_;
  ACE_Auto_Basic_Ptr<X> *fake_this = (ACE_Auto_Basic_Ptr<X> *) this;
  fake_this->p_ = 0;
  return old;
}

#if !defined (ACE_HAS_STANDARD_CPP_LIBRARY) || \
             (ACE_HAS_STANDARD_CPP_LIBRARY == 0)

template<class X> ACE_INLINE
auto_ptr<X>::auto_ptr (X *p)
  : ACE_Auto_Basic_Ptr<X> (p)
{
  ACE_TRACE ("auto_ptr<X>::auto_ptr");
}

template<class X> ACE_INLINE X *
auto_ptr<X>::operator-> () const 
{
  ACE_TRACE ("auto_ptr<X>::operator->");
  return p_;
}

#endif /* ACE_HAS_STANDARD_CPP_LIBRARY */

template<class X> ACE_INLINE
ACE_Auto_Basic_Array_Ptr<X>::ACE_Auto_Basic_Array_Ptr (X *p) 
  : p_ (p) 
{
  ACE_TRACE ("ACE_Auto_Basic_Array_Ptr<X>::ACE_Auto_Basic_Array_Ptr");
}

template<class X> ACE_INLINE
ACE_Auto_Basic_Array_Ptr<X>::ACE_Auto_Basic_Array_Ptr (const ACE_Auto_Basic_Array_Ptr<X> &ap) 
  : p_ (ap.release ()) 
{
  ACE_TRACE ("ACE_Auto_Basic_Array_Ptr<X>::ACE_Auto_Basic_Array_Ptr");
}

template<class X> ACE_INLINE ACE_Auto_Basic_Array_Ptr<X> &
ACE_Auto_Basic_Array_Ptr<X>::operator= (const ACE_Auto_Basic_Array_Ptr<X> &rhs)
{
  ACE_TRACE ("ACE_Auto_Basic_Array_Ptr<X>::operator=");
  if (this != &rhs) 
    {
      delete [] this->p_;
      this->p_ = rhs.release ();
    }
  return *this;
}

template<class X> ACE_INLINE
ACE_Auto_Basic_Array_Ptr<X>::~ACE_Auto_Basic_Array_Ptr (void) 
{
  ACE_TRACE ("ACE_Auto_Basic_Array_Ptr<X>::~ACE_Auto_Basic_Array_Ptr");
  delete [] p_;
}

template<class X> ACE_INLINE X &
ACE_Auto_Basic_Array_Ptr<X>::operator *() const
{
  return *p_;
}

template<class X> ACE_INLINE X &
ACE_Auto_Basic_Array_Ptr<X>::operator[](int i) const
{
  return p_[i];
}

template<class X> ACE_INLINE X *
ACE_Auto_Basic_Array_Ptr<X>::get (void) const 
{
  ACE_TRACE ("ACE_Auto_Basic_Array_Ptr<X>::get");
  return p_;
}

template<class X> ACE_INLINE X *
ACE_Auto_Basic_Array_Ptr<X>::release (void) const 
{
  ACE_TRACE ("ACE_Auto_Basic_Array_Ptr<X>::release");
  X *old = this->p_;
  ACE_Auto_Basic_Array_Ptr<X> *fake_this = (ACE_Auto_Basic_Array_Ptr<X> *) this;
  fake_this->p_ = 0;
  return old;
}

template<class X> ACE_INLINE
ACE_Auto_Array_Ptr<X>::ACE_Auto_Array_Ptr (X *p) 
  : ACE_Auto_Basic_Array_Ptr<X> (p) 
{
  ACE_TRACE ("ACE_Auto_Basic_Array_Ptr<X>::ACE_Auto_Basic_Array_Ptr");
}

template<class X> ACE_INLINE X *
ACE_Auto_Array_Ptr<X>::operator->() const
{
  return p_;
}


/* -*- C++ -*- */
// $Id$

// Auto_Ptr.i

#if !defined (ACE_HAS_STANDARD_CPP_LIBRARY)

template<class X> ACE_INLINE void
ACE_Auto_Basic_Ptr<X>::operator= (ACE_Auto_Basic_Ptr<X> &rhs)
{
  ACE_TRACE ("ACE_Auto_Basic_Ptr<X>::operator=");
  if (this != &rhs) 
    {
      ACE_Auto_Basic_Ptr<X>::remove (p_);
      p_ = rhs.release ();
    }
}

template<class X> ACE_INLINE
ACE_Auto_Basic_Ptr<X>::ACE_Auto_Basic_Ptr (X *p)
  : p_ (p)
{
  ACE_TRACE ("ACE_Auto_Basic_Ptr<X>::ACE_Auto_Basic_Ptr");
}

template<class X> ACE_INLINE
ACE_Auto_Basic_Ptr<X>::ACE_Auto_Basic_Ptr (ACE_Auto_Basic_Ptr<X> &ap)
  : p_ (ap.release ()) 
{
  ACE_TRACE ("ACE_Auto_Basic_Ptr<X>::ACE_Auto_Basic_Ptr");
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
  return *p_;
}

template<class X> ACE_INLINE X *
ACE_Auto_Basic_Ptr<X>::get (void) const 
{
  ACE_TRACE ("ACE_Auto_Basic_Ptr<X>::get");
  return p_;
}

template<class X> ACE_INLINE X *
ACE_Auto_Basic_Ptr<X>::release (void) 
{
  ACE_TRACE ("ACE_Auto_Basic_Ptr<X>::release");
  return this->reset (0);
}

template<class X> ACE_INLINE X *
ACE_Auto_Basic_Ptr<X>::reset (X *p) 
{
  ACE_TRACE ("ACE_Auto_Basic_Ptr<X>::reset");
  X *tp = p_; 
  p_ = p; 
  return tp;
}

template<class X> ACE_INLINE
auto_ptr<X>::auto_ptr (X *p)
  : ACE_Auto_Basic_Ptr<X> (p)
{
  ACE_TRACE ("ACE_Auto_Basic_Ptr<X>::ACE_Auto_Basic_Ptr");
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
ACE_Auto_Basic_Array_Ptr<X>::ACE_Auto_Basic_Array_Ptr (ACE_Auto_Basic_Array_Ptr<X> &ap) 
  : p_ (ap.release ()) 
{
  ACE_TRACE ("ACE_Auto_Basic_Array_Ptr<X>::ACE_Auto_Basic_Array_Ptr");
}

template<class X> ACE_INLINE
ACE_Auto_Basic_Array_Ptr<X>::~ACE_Auto_Basic_Array_Ptr (void) 
{
  ACE_TRACE ("ACE_Auto_Basic_Array_Ptr<X>::~ACE_Auto_Basic_Array_Ptr");
  delete [] p_;
}

template<class X> ACE_INLINE X &
ACE_Auto_Basic_Array_Ptr<X>::operator*() 
{
  return *p_;
}

template<class X> ACE_INLINE X &
ACE_Auto_Basic_Array_Ptr<X>::operator[](int i) 
{
  return p_[i];
}

template<class X> ACE_INLINE X 
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
ACE_Auto_Basic_Array_Ptr<X>::release (void) 
{
  ACE_TRACE ("ACE_Auto_Basic_Array_Ptr<X>::release");
  return this->reset (0);
}

template<class X> ACE_INLINE X *
ACE_Auto_Basic_Array_Ptr<X>::reset (X *p) 
{
  ACE_TRACE ("ACE_Auto_Basic_Array_Ptr<X>::reset");
  X *tp = p_; 
  p_ = p; 
  return tp;
}

template<class X> ACE_INLINE void 
ACE_Auto_Basic_Array_Ptr<X>::operator= (ACE_Auto_Basic_Array_Ptr<X> &rhs)
{
  ACE_TRACE ("ACE_Auto_Basic_Array_Ptr<X>::operator=");
  if (this != &rhs) 
    {
      ACE_Auto_Basic_Array_Ptr<X>::remove (p_);
      p_ = rhs.release ();
    }
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


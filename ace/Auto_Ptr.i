/* -*- C++ -*- */
// $Id$

// Auto_Ptr.i

template<class X> ACE_INLINE void
auto_basic_ptr<X>::operator= (auto_basic_ptr<X> &rhs)
{
  ACE_TRACE ("auto_basic_ptr<X>::operator=");
  if (this != &rhs) 
    {
      auto_basic_ptr<X>::remove (p_);
      p_ = rhs.release ();
    }
}

template<class X> ACE_INLINE
auto_basic_ptr<X>::auto_basic_ptr (X *p)
  : p_ (p)
{
  ACE_TRACE ("auto_basic_ptr<X>::auto_basic_ptr");
}

template<class X> ACE_INLINE
auto_basic_ptr<X>::auto_basic_ptr (auto_basic_ptr<X> &ap)
  : p_ (ap.release ()) 
{
  ACE_TRACE ("auto_basic_ptr<X>::auto_basic_ptr");
}

template<class X> ACE_INLINE
auto_basic_ptr<X>::~auto_basic_ptr (void) 
{ 
  ACE_TRACE ("auto_basic_ptr<X>::~auto_basic_ptr");
  delete p_; 
}

template<class X> ACE_INLINE X &
auto_basic_ptr<X>::operator *() const 
{
  return *p_;
}

template<class X> ACE_INLINE X *
auto_basic_ptr<X>::get (void) const 
{
  ACE_TRACE ("auto_basic_ptr<X>::get");
  return p_;
}

template<class X> ACE_INLINE X *
auto_basic_ptr<X>::release (void) 
{
  ACE_TRACE ("auto_basic_ptr<X>::release");
  return this->reset (0);
}

template<class X> ACE_INLINE X *
auto_basic_ptr<X>::reset (X *p) 
{
  ACE_TRACE ("auto_basic_ptr<X>::reset");
  X *tp = p_; 
  p_ = p; 
  return tp;
}

template<class X> ACE_INLINE
auto_ptr<X>::auto_ptr (X *p)
  : auto_basic_ptr<X> (p)
{
  ACE_TRACE ("auto_basic_ptr<X>::auto_basic_ptr");
}

template<class X> ACE_INLINE X *
auto_ptr<X>::operator-> () const 
{
  ACE_TRACE ("auto_ptr<X>::operator->");
  return p_;
}

template<class X> ACE_INLINE
auto_basic_array_ptr<X>::auto_basic_array_ptr (X *p) 
  : p_ (p) 
{
  ACE_TRACE ("auto_basic_array_ptr<X>::auto_basic_array_ptr");
}

template<class X> ACE_INLINE
auto_basic_array_ptr<X>::auto_basic_array_ptr (auto_basic_array_ptr<X> &ap) 
  : p_ (ap.release ()) 
{
  ACE_TRACE ("auto_basic_array_ptr<X>::auto_basic_array_ptr");
}

template<class X> ACE_INLINE
auto_basic_array_ptr<X>::~auto_basic_array_ptr (void) 
{
  ACE_TRACE ("auto_basic_array_ptr<X>::~auto_basic_array_ptr");
  delete [] p_;
}

template<class X> ACE_INLINE X &
auto_basic_array_ptr<X>::operator*() 
{
  return *p_;
}

template<class X> ACE_INLINE X &
auto_basic_array_ptr<X>::operator[](int i) 
{
  return p_[i];
}

template<class X> ACE_INLINE X 
auto_basic_array_ptr<X>::operator[](int i) const 
{
  return p_[i];
}

template<class X> ACE_INLINE X *
auto_basic_array_ptr<X>::get (void) const 
{
  ACE_TRACE ("auto_basic_array_ptr<X>::get");
  return p_;
}

template<class X> ACE_INLINE X *
auto_basic_array_ptr<X>::release (void) 
{
  ACE_TRACE ("auto_basic_array_ptr<X>::release");
  return this->reset (0);
}

template<class X> ACE_INLINE X *
auto_basic_array_ptr<X>::reset (X *p) 
{
  ACE_TRACE ("auto_basic_array_ptr<X>::reset");
  X *tp = p_; 
  p_ = p; 
  return tp;
}

template<class X> ACE_INLINE void 
auto_basic_array_ptr<X>::operator= (auto_basic_array_ptr<X> &rhs)
{
  ACE_TRACE ("auto_basic_array_ptr<X>::operator=");
  if (this != &rhs) 
    {
      auto_basic_array_ptr<X>::remove (p_);
      p_ = rhs.release ();
    }
}

template<class X> ACE_INLINE
auto_array_ptr<X>::auto_array_ptr (X *p) 
  : auto_basic_array_ptr<X> (p) 
{
  ACE_TRACE ("auto_basic_array_ptr<X>::auto_basic_array_ptr");
}

template<class X> ACE_INLINE X *
auto_array_ptr<X>::operator->() 
{
  return p_;
}


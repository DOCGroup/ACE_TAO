// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <typename T>
ACE_INLINE
TAO_Intrusive_Ref_Count_Handle<T>::TAO_Intrusive_Ref_Count_Handle (void)
  : ptr_(0)
{
}


template <typename T>
ACE_INLINE
TAO_Intrusive_Ref_Count_Handle<T>::TAO_Intrusive_Ref_Count_Handle (
  T*   p,
  bool take_ownership)
  : ptr_(p)
{
  if (!take_ownership)
    {
      this->claim ();
    }
}


template <typename T>
ACE_INLINE
TAO_Intrusive_Ref_Count_Handle<T>::TAO_Intrusive_Ref_Count_Handle (
  const TAO_Intrusive_Ref_Count_Handle<T>& b)
  : ptr_(b.ptr_)
{
  this->claim();
}


template <typename T>
ACE_INLINE
TAO_Intrusive_Ref_Count_Handle<T>::~TAO_Intrusive_Ref_Count_Handle()
{
  this->drop();
}


template <typename T>
ACE_INLINE
TAO_Intrusive_Ref_Count_Handle<T>&
TAO_Intrusive_Ref_Count_Handle<T>::operator=(T* p)
{
  TAO_Intrusive_Ref_Count_Handle<T> tmp (p);
  return this->operator= (tmp);
}


template <typename T>
ACE_INLINE
TAO_Intrusive_Ref_Count_Handle<T>&
TAO_Intrusive_Ref_Count_Handle<T>::operator=
                                 (const TAO_Intrusive_Ref_Count_Handle<T>& b)
{
  // Strongly exception-safe assignment through the usual copy and
  // swap technique.

  TAO_Intrusive_Ref_Count_Handle<T> tmp (b);

  T * old_ptr = this->ptr_;
  this->ptr_ = tmp.ptr_;
  tmp.ptr_ = old_ptr;

  return *this;
}


template <typename T>
ACE_INLINE
T*
TAO_Intrusive_Ref_Count_Handle<T>::operator->() const
{
  return this->ptr_;
}


template <typename T>
ACE_INLINE
bool
TAO_Intrusive_Ref_Count_Handle<T>::is_nil() const
{
  return this->ptr_ == 0;
}


template <typename T>
ACE_INLINE
T*
TAO_Intrusive_Ref_Count_Handle<T>::in() const
{
  return this->ptr_;
}


template <typename T>
ACE_INLINE
T*&
TAO_Intrusive_Ref_Count_Handle<T>::inout()
{
  return this->ptr_;
}


template <typename T>
ACE_INLINE
T*&
TAO_Intrusive_Ref_Count_Handle<T>::out()
{
  this->drop();
  return this->ptr_;
}


template <typename T>
ACE_INLINE
T*
TAO_Intrusive_Ref_Count_Handle<T>::_retn()
{
  T* retval = this->ptr_;
  this->ptr_ = 0;
  return retval;
}


template <typename T>
ACE_INLINE
bool
TAO_Intrusive_Ref_Count_Handle<T>::operator== (const TAO_Intrusive_Ref_Count_Handle& h) const
{
  return this->ptr_ == h.in();
}


template <typename T>
ACE_INLINE
void
TAO_Intrusive_Ref_Count_Handle<T>::claim()
{
  if (this->ptr_ != 0)
    {
      this->ptr_->_add_ref();
    }
}


template <typename T>
ACE_INLINE
void
TAO_Intrusive_Ref_Count_Handle<T>::drop()
{
  if (this->ptr_ != 0)
    {
      this->ptr_->_remove_ref();
      this->ptr_ = 0;
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL

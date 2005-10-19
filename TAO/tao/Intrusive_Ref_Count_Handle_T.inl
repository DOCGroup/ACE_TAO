// $Id$


template <typename T>
ACE_INLINE
TAO_Intrusive_Ref_Count_Handle<T>::TAO_Intrusive_Ref_Count_Handle()
  : ptr_(0)
{
}


template <typename T>
ACE_INLINE
TAO_Intrusive_Ref_Count_Handle<T>::TAO_Intrusive_Ref_Count_Handle
                                              (T*   p,
                                               bool take_ownership)
  : ptr_(p)
{
  if (!take_ownership)
    {
      this->claim();
    }
}


template <typename T>
ACE_INLINE
TAO_Intrusive_Ref_Count_Handle<T>::TAO_Intrusive_Ref_Count_Handle
                                (const TAO_Intrusive_Ref_Count_Handle<T>& b)
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
  if (this->ptr_ != p)
    {
      this->drop();
      this->ptr_ = p;
    }

  return *this;
}


template <typename T>
ACE_INLINE
TAO_Intrusive_Ref_Count_Handle<T>&
TAO_Intrusive_Ref_Count_Handle<T>::operator=
                                 (const TAO_Intrusive_Ref_Count_Handle<T>& b)
{
  if (this->ptr_ != b.ptr_)
    {
      this->drop();
      this->ptr_ = b.ptr_;
      this->claim();
    }

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

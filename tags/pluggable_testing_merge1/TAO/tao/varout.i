//
// $Id$
//

template<class T> ACE_INLINE
TAO_Object_Field_T<T>::TAO_Object_Field_T (void)
  :  ptr_ (0)
{
}

template<class T> ACE_INLINE
TAO_Object_Field_T<T>::TAO_Object_Field_T (T* object)
  :  ptr_ (object)
{
}

template<class T> ACE_INLINE 
TAO_Object_Field_T<T>::TAO_Object_Field_T (const TAO_Object_Field_T<T>& rhs)
  :  ptr_ (T::_duplicate (rhs.ptr_))
{
}

template<class T> ACE_INLINE
TAO_Object_Field_T<T>::~TAO_Object_Field_T (void)
{
  CORBA::release (this->ptr_);
}

template<class T> ACE_INLINE TAO_Object_Field_T<T> &
TAO_Object_Field_T<T>::operator= (T* object)
{
  CORBA::release (this->ptr_);
  this->ptr_ = object;
  return *this;
}

template<class T> ACE_INLINE TAO_Object_Field_T<T> &
TAO_Object_Field_T<T>::operator= (const TAO_Object_Field_T<T> &rhs)
{
  if (this != &rhs)
    {
      CORBA::release (this->ptr_);
      this->ptr_ = T::_duplicate (rhs.ptr_);
    }
  return *this;
}

template<class T> ACE_INLINE T*
TAO_Object_Field_T<T>::ptr (void) const
{
  return this->ptr_;
}

template<class T> ACE_INLINE
TAO_Object_Field_T<T>::operator T* const &() const
{
  return this->ptr_;
}

template<class T> ACE_INLINE
TAO_Object_Field_T<T>::operator T* &()
{
  return this->ptr_;
}

template<class T> ACE_INLINE T*
TAO_Object_Field_T<T>::operator-> (void) const
{
  return this->ptr_;
}

template<class T> ACE_INLINE T*
TAO_Object_Field_T<T>::in (void) const
{
  return this->ptr_;
}

template<class T> ACE_INLINE T* &
TAO_Object_Field_T<T>::inout (void)
{
  return this->ptr_;
}

template<class T> ACE_INLINE T* &
TAO_Object_Field_T<T>::out (void)
{
  CORBA::release (this->ptr_);
  this->ptr_ = 0;
  return this->ptr_;
}

template<class T> ACE_INLINE T*
TAO_Object_Field_T<T>::_retn (void)
{
  // yield ownership of managed obj reference
  T* val = this->ptr_;
  this->ptr_ = 0;
  return val;
}


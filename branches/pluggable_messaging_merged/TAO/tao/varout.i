//
// $Id$
//

template<class T, class T_var> ACE_INLINE
TAO_Object_Field_T<T,T_var>::TAO_Object_Field_T (void)
  :  ptr_ (0)
{
}

template<class T, class T_var> ACE_INLINE
TAO_Object_Field_T<T,T_var>::TAO_Object_Field_T (T* object)
  :  ptr_ (object)
{
}

template<class T, class T_var> ACE_INLINE T*
TAO_Object_Field_T<T,T_var>::ptr (void) const
{
  return this->ptr_;
}

template<class T, class T_var> ACE_INLINE
TAO_Object_Field_T<T,T_var>::operator T* const &() const
{
  return this->ptr_;
}

template<class T, class T_var> ACE_INLINE
TAO_Object_Field_T<T,T_var>::operator T* &()
{
  return this->ptr_;
}

template<class T, class T_var> ACE_INLINE T*
TAO_Object_Field_T<T,T_var>::operator-> (void) const
{
  return this->ptr_;
}

template<class T, class T_var> ACE_INLINE T*
TAO_Object_Field_T<T,T_var>::in (void) const
{
  return this->ptr_;
}

template<class T, class T_var> ACE_INLINE T* &
TAO_Object_Field_T<T,T_var>::inout (void)
{
  return this->ptr_;
}

template<class T, class T_var> ACE_INLINE T*
TAO_Object_Field_T<T,T_var>::_retn (void)
{
  // yield ownership of managed obj reference
  T* val = this->ptr_;
  this->ptr_ = 0;
  return val;
}

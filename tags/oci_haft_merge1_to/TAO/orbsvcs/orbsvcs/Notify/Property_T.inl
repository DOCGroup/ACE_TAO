// $Id$

template <class TYPE> ACE_INLINE const TYPE&
TAO_Notify_PropertyBase_T<TYPE>::value (void) const
{
  return this->value_;
}

template <class TYPE> ACE_INLINE CORBA::Boolean
TAO_Notify_PropertyBase_T<TYPE>::is_valid (void) const
{
  return this->valid_;
}

template <class TYPE> ACE_INLINE int
TAO_Notify_PropertyBase_T<TYPE>::operator== (const TYPE &rhs) const
{
  return (this->value_ == rhs);
}

template <class TYPE> ACE_INLINE int
TAO_Notify_PropertyBase_T<TYPE>::operator!= (const TYPE &rhs) const
{
  return (this->value_ != rhs);
}

template <class TYPE> ACE_INLINE TAO_Notify_PropertyBase_T<TYPE>&
TAO_Notify_PropertyBase_T<TYPE>::operator=(const TYPE& value)
{
  this->value_ = value;

  return *this;
}

template <class TYPE> ACE_INLINE void
TAO_Notify_PropertyBase_T<TYPE>:: invalidate (void)
{
  this->valid_ = 0;
}

/******************************************************************************/

template <class TYPE> ACE_INLINE TAO_Notify_Property_T<TYPE>&
TAO_Notify_Property_T<TYPE>::operator=(const TYPE& value)
{
  this->TAO_Notify_PropertyBase_T<TYPE>::operator= (value);
  return *this;
}

/******************************************************************************/

template <class TYPE> ACE_INLINE const TYPE&
TAO_Notify_StructProperty_T<TYPE>::value (void) const
{
  return this->value_;
}

template <class TYPE> ACE_INLINE CORBA::Boolean
TAO_Notify_StructProperty_T<TYPE>::is_valid (void) const
{
  return this->valid_;
}

// $Id$

template <class TYPE> ACE_INLINE const TYPE&
TAO_NS_PropertyBase_T<TYPE>::value (void) const
{
  return this->value_;
}

template <class TYPE> ACE_INLINE CORBA::Boolean
TAO_NS_PropertyBase_T<TYPE>::is_valid (void) const
{
  return this->valid_;
}

template <class TYPE> ACE_INLINE int
TAO_NS_PropertyBase_T<TYPE>::operator== (const TYPE &rhs) const
{
  return (this->value_ == rhs);
}

template <class TYPE> ACE_INLINE int
TAO_NS_PropertyBase_T<TYPE>::operator!= (const TYPE &rhs) const
{
  return (this->value_ != rhs);
}

template <class TYPE> ACE_INLINE TAO_NS_PropertyBase_T<TYPE>&
TAO_NS_PropertyBase_T<TYPE>::operator= (const TAO_NS_PropertyBase_T<TYPE>& rhs)
{
  if (this == &rhs)
    return *this;

  if (rhs.is_valid ())
  {
    this->name_ = rhs.name_;
    this->value_ = rhs.value_;
    this->valid_ = rhs.valid_;
  }

  return *this;
}

template <class TYPE> ACE_INLINE TAO_NS_PropertyBase_T<TYPE>&
TAO_NS_PropertyBase_T<TYPE>::operator=(const TYPE& value)
{
  this->value_ = value;

  return *this;
}

template <class TYPE> ACE_INLINE void
TAO_NS_PropertyBase_T<TYPE>:: invalidate (void)
{
  this->valid_ = 0;
}

/******************************************************************************/

template <class TYPE> ACE_INLINE TAO_NS_Property_T<TYPE>&
TAO_NS_Property_T<TYPE>::operator=(const TYPE& value)
{
  this->TAO_NS_PropertyBase_T<TYPE>::operator= (value);
  return *this;
}

/******************************************************************************/

template <class TYPE> ACE_INLINE const TYPE&
TAO_NS_StructProperty_T<TYPE>::value (void) const
{
  return this->value_;
}

template <class TYPE> ACE_INLINE CORBA::Boolean
TAO_NS_StructProperty_T<TYPE>::is_valid (void) const
{
  return this->valid_;
}

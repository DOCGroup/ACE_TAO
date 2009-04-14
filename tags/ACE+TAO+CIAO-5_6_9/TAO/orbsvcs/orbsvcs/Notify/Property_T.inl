// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <class TYPE> ACE_INLINE const TYPE&
TAO_Notify_PropertyBase_T<TYPE>::value (void) const
{
  return this->value_;
}

template <class TYPE> ACE_INLINE
const char *
TAO_Notify_PropertyBase_T<TYPE>::name (void) const
{
  return this->name_;
}

template <class TYPE> ACE_INLINE CORBA::Boolean
TAO_Notify_PropertyBase_T<TYPE>::is_valid (void) const
{
  return this->valid_;
}

template <class TYPE> ACE_INLINE bool
TAO_Notify_PropertyBase_T<TYPE>::operator== (const TYPE &rhs) const
{
  return (this->value_ == rhs);
}

template <class TYPE> ACE_INLINE bool
TAO_Notify_PropertyBase_T<TYPE>::operator!= (const TYPE &rhs) const
{
  return (this->value_ != rhs);
}

template <class TYPE> ACE_INLINE void
TAO_Notify_PropertyBase_T<TYPE>:: invalidate (void)
{
  this->valid_ = 0;
}

template <class TYPE> ACE_INLINE
void
TAO_Notify_PropertyBase_T<TYPE>:: assign (const TYPE& value)
{
  this->value_ = value;
  this->valid_ = 1;
}



/******************************************************************************/

template <class TYPE> ACE_INLINE
const char *
TAO_Notify_StructProperty_T<TYPE>::name (void) const
{
  return this->name_;
}

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

TAO_END_VERSIONED_NAMESPACE_DECL

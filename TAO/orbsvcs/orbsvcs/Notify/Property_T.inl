TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <class TYPE> ACE_INLINE TYPE const&
TAO_Notify_PropertyBase_T<TYPE>::value () const
{
  return this->value_;
}

template <class TYPE> ACE_INLINE
char const *
TAO_Notify_PropertyBase_T<TYPE>::name () const
{
  return this->name_;
}

template <class TYPE> ACE_INLINE CORBA::Boolean
TAO_Notify_PropertyBase_T<TYPE>::is_valid () const
{
  return this->valid_;
}

template <class TYPE> ACE_INLINE bool
TAO_Notify_PropertyBase_T<TYPE>::operator== (TYPE const &rhs) const
{
  return (this->value_ == rhs);
}

template <class TYPE> ACE_INLINE bool
TAO_Notify_PropertyBase_T<TYPE>::operator!= (TYPE const &rhs) const
{
  return (this->value_ != rhs);
}

template <class TYPE> ACE_INLINE void
TAO_Notify_PropertyBase_T<TYPE>:: invalidate ()
{
  this->valid_ = false;
}

template <class TYPE> ACE_INLINE
void
TAO_Notify_PropertyBase_T<TYPE>:: assign (TYPE const& value)
{
  this->value_ = value;
  this->valid_ = true;
}

/******************************************************************************/

template <class TYPE> ACE_INLINE
char const *
TAO_Notify_StructProperty_T<TYPE>::name () const
{
  return this->name_;
}

template <class TYPE> ACE_INLINE TYPE const&
TAO_Notify_StructProperty_T<TYPE>::value () const
{
  return this->value_;
}

template <class TYPE> ACE_INLINE CORBA::Boolean
TAO_Notify_StructProperty_T<TYPE>::is_valid () const
{
  return this->valid_;
}

TAO_END_VERSIONED_NAMESPACE_DECL

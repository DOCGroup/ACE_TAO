// $Id$

template<typename T> 
ACE_INLINE 
CORBA::Boolean
TAO::Any_Dual_Impl_T<T>::marshal_value (TAO_OutputCDR &cdr)
{
  return (cdr << *this->value_);
}

template<typename T> 
ACE_INLINE 
const void *
TAO::Any_Dual_Impl_T<T>::value (void) const
{
  return this->value_;
}


// $Id$

template<typename T, typename from_T, typename to_T> 
ACE_INLINE 
CORBA::Boolean
TAO::Any_Special_Basic_Impl_T<T, from_T, to_T>::marshal_value (
    TAO_OutputCDR &cdr
  )
{
  return (cdr << from_T (this->value_));
}

template<typename T, typename from_T, typename to_T> 
ACE_INLINE 
CORBA::Boolean
TAO::Any_Special_Basic_Impl_T<T, from_T, to_T>::demarshal_value (
    TAO_InputCDR &cdr
  )
{
  return (cdr >> to_T (this->value_));
}

template<typename T, typename from_T, typename to_T> 
ACE_INLINE 
const void *
TAO::Any_Special_Basic_Impl_T<T, from_T, to_T>::value (void) const
{
  return &this->value_;
}


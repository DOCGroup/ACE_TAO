// $Id$

template<typename T_slice, typename T_forany> 
ACE_INLINE 
CORBA::Boolean
TAO::Any_Array_Impl_T<T_slice, T_forany>::marshal_value (TAO_OutputCDR &cdr)
{
  return (cdr << T_forany (this->value_));
}

template<typename T_slice, typename T_forany> 
ACE_INLINE 
CORBA::Boolean
TAO::Any_Array_Impl_T<T_slice, T_forany>::demarshal_value (TAO_InputCDR &cdr)
{
  T_forany tmp (this->value_);
  return (cdr >> tmp);
}

template<typename T_slice, typename T_forany> 
ACE_INLINE 
const void *
TAO::Any_Array_Impl_T<T_slice, T_forany>::value (void) const
{
  return this->value_;
}


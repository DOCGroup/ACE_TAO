// This may look like C, but it's really -*- C++ -*-
// $Id$

// These three are overridden in generated code where appropriate.

template<typename T> 
ACE_INLINE 
CORBA::Boolean
TAO::Any_Impl_T<T>::to_object (CORBA::Object_ptr &) const
{
  return 0;
}

template<typename T> 
ACE_INLINE 
CORBA::Boolean
TAO::Any_Impl_T<T>::to_value (CORBA::ValueBase *&) const
{
  return 0;
}

template<typename T> 
ACE_INLINE 
CORBA::Boolean
TAO::Any_Impl_T<T>::to_abstract_base (CORBA::AbstractBase_ptr &) const
{
  return 0;
}

template<typename T> 
ACE_INLINE 
CORBA::Boolean
TAO::Any_Impl_T<T>::marshal_value (TAO_OutputCDR &cdr)
{
  return (cdr << this->value_);
}

template<typename T> 
ACE_INLINE 
CORBA::Boolean
TAO::Any_Impl_T<T>::demarshal_value (TAO_InputCDR &cdr)
{
  return (cdr >> this->value_);
}

template<typename T> 
ACE_INLINE 
const void *
TAO::Any_Impl_T<T>::value (void) const
{
  return this->value_;
}


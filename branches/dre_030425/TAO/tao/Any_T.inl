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

// ***********************************************************************

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

// ***********************************************************************

template<typename T, typename from_T, typename to_T> 
ACE_INLINE 
CORBA::Boolean
TAO::Any_Special_Impl_T<T, from_T, to_T>::marshal_value (
    TAO_OutputCDR &cdr
  )
{
  return (cdr << from_T (this->value_, this->bound_));
}

template<typename T, typename from_T, typename to_T> 
ACE_INLINE 
CORBA::Boolean
TAO::Any_Special_Impl_T<T, from_T, to_T>::demarshal_value (
    TAO_InputCDR &cdr
  )
{
  return (cdr >> to_T (this->value_, this->bound_));
}

template<typename T, typename from_T, typename to_T> 
ACE_INLINE 
const void *
TAO::Any_Special_Impl_T<T, from_T, to_T>::value (void) const
{
  return this->value_;
}

// ***********************************************************************

template<typename T> 
ACE_INLINE 
CORBA::Boolean
TAO::Any_Basic_Impl_T<T>::marshal_value (TAO_OutputCDR &cdr)
{
  return (cdr << this->value_);
}

template<typename T> 
ACE_INLINE 
CORBA::Boolean
TAO::Any_Basic_Impl_T<T>::demarshal_value (TAO_InputCDR &cdr)
{
  return (cdr >> this->value_);
}

template<typename T> 
ACE_INLINE 
const void *
TAO::Any_Basic_Impl_T<T>::value (void) const
{
  return &this->value_;
}

// ***********************************************************************

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

// ***********************************************************************

template<typename T> 
ACE_INLINE 
CORBA::Boolean
TAO::Any_Dual_Impl_T<T>::marshal_value (TAO_OutputCDR &cdr)
{
  return (cdr << *this->value_);
}

template<typename T> 
ACE_INLINE 
CORBA::Boolean
TAO::Any_Dual_Impl_T<T>::demarshal_value (TAO_InputCDR &cdr)
{
  return (cdr >> *this->value_);
}

template<typename T> 
ACE_INLINE 
const void *
TAO::Any_Dual_Impl_T<T>::value (void) const
{
  return this->value_;
}


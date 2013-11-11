// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// These three are overridden in generated code where appropriate.

template<typename T>
ACE_INLINE
CORBA::Boolean
TAO::Any_Impl_T<T>::demarshal_value (TAO_InputCDR &cdr)
{
  return (cdr >> this->value_);
}

TAO_END_VERSIONED_NAMESPACE_DECL

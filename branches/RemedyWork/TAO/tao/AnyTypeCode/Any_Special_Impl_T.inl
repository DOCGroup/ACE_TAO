// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<typename T, typename from_T, typename to_T>
ACE_INLINE
CORBA::Boolean
TAO::Any_Special_Impl_T<T, from_T, to_T>::demarshal_value (
    TAO_InputCDR &cdr
  )
{
  return (cdr >> to_T (this->value_, this->bound_));
}

TAO_END_VERSIONED_NAMESPACE_DECL

// $Id$

#ifndef TAO_FIXED_SIZE_ARGUMENT_T_C
#define TAO_FIXED_SIZE_ARGUMENT_T_C

#include "tao/Fixed_Size_Argument_T.h"

#if !defined (__ACE_INLINE__)
#include "tao/Fixed_Size_Argument_T.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao,
           Fixed_Size_Argument_T,
           "$Id$")

template<typename S,
         typename Insert_Policy>
CORBA::Boolean
TAO::In_Fixed_Size_Argument_T<S,Insert_Policy>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << *this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S,
         typename Insert_Policy>
void
TAO::In_Fixed_Size_Argument_T<S,Insert_Policy>::interceptor_value (CORBA::Any *any) const
{
  this->Insert_Policy::any_insert (any, *this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ===========================================================

template<typename S,
         typename Insert_Policy>
CORBA::Boolean
TAO::Inout_Fixed_Size_Argument_T<S,Insert_Policy>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << *this->x_;
}

template<typename S,
         typename Insert_Policy>
CORBA::Boolean
TAO::Inout_Fixed_Size_Argument_T<S,Insert_Policy>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> *this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S,
         typename Insert_Policy>
void
TAO::Inout_Fixed_Size_Argument_T<S,Insert_Policy>::interceptor_value (CORBA::Any *any) const
{
  this->Insert_Policy::any_insert (any, *this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ==============================================================

template<typename S,
         typename Insert_Policy>
CORBA::Boolean
TAO::Out_Fixed_Size_Argument_T<S,Insert_Policy>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S,
         typename Insert_Policy>
void
TAO::Out_Fixed_Size_Argument_T<S,Insert_Policy>::interceptor_value (CORBA::Any *any) const
{
  this->Insert_Policy::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ============================================================

template<typename S,
         typename Insert_Policy>
CORBA::Boolean
TAO::Ret_Fixed_Size_Argument_T<S,Insert_Policy>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S,
         typename Insert_Policy>
void
TAO::Ret_Fixed_Size_Argument_T<S,Insert_Policy>::
interceptor_value (CORBA::Any *any) const
{
  this->Insert_Policy::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

#endif /* TAO_FIXED_SIZE_ARGUMENT_T_C */

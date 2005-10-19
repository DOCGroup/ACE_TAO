// $Id$

#ifndef TAO_OBJECT_SARGUMENT_T_CPP
#define TAO_OBJECT_SARGUMENT_T_CPP

#include "tao/PortableServer/Object_SArgument_T.h"

#if !defined (__ACE_INLINE__)
#include "tao/PortableServer/Object_SArgument_T.inl"
#endif /* __ACE_INLINE__ */


template<typename S_ptr,
         typename S_var,
         typename Insert_Policy>
CORBA::Boolean
TAO::In_Object_SArgument_T<S_ptr,S_var,Insert_Policy>::demarshal (TAO_InputCDR &cdr)
{
  return cdr >> this->x_.out ();
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S_ptr,
         typename S_var,
         typename Insert_Policy>
void
TAO::In_Object_SArgument_T<S_ptr,S_var,Insert_Policy>::interceptor_value (
  CORBA::Any *any) const
{
  this->Insert_Policy::any_insert (any, this->x_.in ());
}

#endif /* TAO_HAS_INTERCEPTORS */

// ===========================================================

template<typename S_ptr,
         typename S_var,
         typename Insert_Policy>
CORBA::Boolean
TAO::Inout_Object_SArgument_T<S_ptr,S_var,Insert_Policy>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << this->x_.in ();
}

template<typename S_ptr,
         typename S_var,
         typename Insert_Policy>
CORBA::Boolean
TAO::Inout_Object_SArgument_T<S_ptr,S_var,Insert_Policy>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_.out ();
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S_ptr,
         typename S_var,
         typename Insert_Policy>
void
TAO::Inout_Object_SArgument_T<S_ptr,S_var,Insert_Policy>::interceptor_value (
  CORBA::Any *any) const
{
  this->Insert_Policy::any_insert (any, this->x_.in ());
}

#endif /* TAO_HAS_INTERCEPTORS */

// ==============================================================

template<typename S_ptr,
         typename S_var,
         typename S_out,
         typename Insert_Policy>
CORBA::Boolean
TAO::Out_Object_SArgument_T<S_ptr,S_var,S_out,Insert_Policy>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << this->x_.in ();
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S_ptr,
         typename S_var,
         typename S_out,
         typename Insert_Policy>
void
TAO::Out_Object_SArgument_T<S_ptr,S_var,S_out,Insert_Policy>::interceptor_value (
  CORBA::Any *any) const
{
  this->Insert_Policy::any_insert (any, this->x_.in ());
}

#endif /* TAO_HAS_INTERCEPTORS */

// ============================================================

template<typename S_ptr,
         typename S_var,
         typename Insert_Policy>
CORBA::Boolean
TAO::Ret_Object_SArgument_T<S_ptr,S_var,Insert_Policy>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << this->x_.in ();
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S_ptr,
         typename S_var,
         typename Insert_Policy>
void
TAO::Ret_Object_SArgument_T<S_ptr,S_var,Insert_Policy>::interceptor_value (
  CORBA::Any *any) const
{
  this->Insert_Policy::any_insert (any, this->x_.in ());
}

#endif /* TAO_HAS_INTERCEPTORS */

#endif /* TAO_OBJECT_SARGUMENT_T_CPP */

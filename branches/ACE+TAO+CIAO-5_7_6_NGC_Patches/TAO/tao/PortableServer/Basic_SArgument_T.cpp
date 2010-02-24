// $Id$

#ifndef TAO_BASIC_SARGUMENT_T_CPP
#define TAO_BASIC_SARGUMENT_T_CPP

#include "tao/PortableServer/Basic_SArgument_T.h"

#if !defined (__ACE_INLINE__)
#include "tao/PortableServer/Basic_SArgument_T.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<typename S,
         class Insert_Policy>
CORBA::Boolean
TAO::In_Basic_SArgument_T<S,Insert_Policy>::demarshal (TAO_InputCDR &cdr)
{
  return cdr >> this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S,
         class Insert_Policy>
void
TAO::In_Basic_SArgument_T<S,Insert_Policy>::interceptor_value (CORBA::Any *any) const
{
  Insert_Policy::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ===========================================================

template<typename S,
         class Insert_Policy>
CORBA::Boolean
TAO::Inout_Basic_SArgument_T<S,Insert_Policy>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << this->x_;
}

template<typename S,
         class Insert_Policy>
CORBA::Boolean
TAO::Inout_Basic_SArgument_T<S,Insert_Policy>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S,
         class Insert_Policy>
void
TAO::Inout_Basic_SArgument_T<S,Insert_Policy>::interceptor_value (CORBA::Any *any) const
{
  Insert_Policy::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ==============================================================

template<typename S,
         class Insert_Policy>
CORBA::Boolean
TAO::Out_Basic_SArgument_T<S,Insert_Policy>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S,
         class Insert_Policy>
void
TAO::Out_Basic_SArgument_T<S,Insert_Policy>::interceptor_value (CORBA::Any *any) const
{
  Insert_Policy::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ============================================================

template<typename S,
         class Insert_Policy>
CORBA::Boolean
TAO::Ret_Basic_SArgument_T<S,Insert_Policy>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S,
         class Insert_Policy>
void
TAO::Ret_Basic_SArgument_T<S,Insert_Policy>::interceptor_value (CORBA::Any *any) const
{
  Insert_Policy::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_BASIC_SARGUMENT_T_CPP */

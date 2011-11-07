// $Id$

#ifndef TAO_FIXED_SIZE_SARGUMENT_T_CPP
#define TAO_FIXED_SIZE_SARGUMENT_T_CPP

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#include "tao/PortableServer/Fixed_Size_SArgument_T.h"
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#if !defined (__ACE_INLINE__)
#include "tao/PortableServer/Fixed_Size_SArgument_T.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<typename S,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::In_Fixed_Size_SArgument_T<S,Insert_Policy>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S,
         template <typename> class Insert_Policy>
void
TAO::In_Fixed_Size_SArgument_T<S,Insert_Policy>::interceptor_value (
  CORBA::Any *any) const
{
  Insert_Policy<S>::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ===========================================================

template<typename S,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::Inout_Fixed_Size_SArgument_T<S,Insert_Policy>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << this->x_;
}

template<typename S,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::Inout_Fixed_Size_SArgument_T<S,Insert_Policy>::demarshal (
  TAO_InputCDR & cdr)
{
  return cdr >> this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S,
         template <typename> class Insert_Policy>
void
TAO::Inout_Fixed_Size_SArgument_T<S,Insert_Policy>::interceptor_value (
  CORBA::Any *any) const
{
  Insert_Policy<S>::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ==============================================================

template<typename S,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::Out_Fixed_Size_SArgument_T<S,Insert_Policy>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S,
         template <typename> class Insert_Policy>
void
TAO::Out_Fixed_Size_SArgument_T<S,Insert_Policy>::interceptor_value (
  CORBA::Any *any) const
{
  Insert_Policy<S>::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ============================================================

template<typename S,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::Ret_Fixed_Size_SArgument_T<S,Insert_Policy>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S,
         template <typename> class Insert_Policy>
void
TAO::Ret_Fixed_Size_SArgument_T<S,Insert_Policy>::interceptor_value (
  CORBA::Any *any) const
{
  Insert_Policy<S>::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_FIXED_SIZE_SARGUMENT_T_CPP */

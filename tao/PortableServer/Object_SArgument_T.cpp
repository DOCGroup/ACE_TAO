// $Id$

#ifndef TAO_OBJECT_SARGUMENT_T_CPP
#define TAO_OBJECT_SARGUMENT_T_CPP

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#include "tao/PortableServer/Object_SArgument_T.h"
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#if !defined (__ACE_INLINE__)
#include "tao/PortableServer/Object_SArgument_T.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<typename S_ptr,
         typename S_var,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::In_Object_SArgument_T<S_ptr,S_var,Insert_Policy>::demarshal (TAO_InputCDR &cdr)
{
  return cdr >> this->x_.out ();
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S_ptr,
         typename S_var,
         template <typename> class Insert_Policy>
void
TAO::In_Object_SArgument_T<S_ptr,S_var,Insert_Policy>::interceptor_value (
  CORBA::Any *any) const
{
  Insert_Policy<S_ptr>::any_insert (any, this->x_.in ());
}

#endif /* TAO_HAS_INTERCEPTORS */

// ===========================================================

template<typename S_ptr,
         typename S_var,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::Inout_Object_SArgument_T<S_ptr,S_var,Insert_Policy>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << this->x_.in ();
}

template<typename S_ptr,
         typename S_var,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::Inout_Object_SArgument_T<S_ptr,S_var,Insert_Policy>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_.out ();
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S_ptr,
         typename S_var,
         template <typename> class Insert_Policy>
void
TAO::Inout_Object_SArgument_T<S_ptr,S_var,Insert_Policy>::interceptor_value (
  CORBA::Any *any) const
{
  Insert_Policy<S_ptr>::any_insert (any, this->x_.in ());
}

#endif /* TAO_HAS_INTERCEPTORS */

// ==============================================================

template<typename S_ptr,
         typename S_var,
         typename S_out,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::Out_Object_SArgument_T<S_ptr,S_var,S_out,Insert_Policy>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << this->x_.in ();
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S_ptr,
         typename S_var,
         typename S_out,
         template <typename> class Insert_Policy>
void
TAO::Out_Object_SArgument_T<S_ptr,S_var,S_out,Insert_Policy>::interceptor_value (
  CORBA::Any *any) const
{
  Insert_Policy<S_ptr>::any_insert (any, this->x_.in ());
}

#endif /* TAO_HAS_INTERCEPTORS */

// ============================================================

template<typename S_ptr,
         typename S_var,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::Ret_Object_SArgument_T<S_ptr,S_var,Insert_Policy>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << this->x_.in ();
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S_ptr,
         typename S_var,
         template <typename> class Insert_Policy>
void
TAO::Ret_Object_SArgument_T<S_ptr,S_var,Insert_Policy>::interceptor_value (
  CORBA::Any *any) const
{
  Insert_Policy<S_ptr>::any_insert (any, this->x_.in ());
}

#endif /* TAO_HAS_INTERCEPTORS */

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_OBJECT_SARGUMENT_T_CPP */

// $Id$

#ifndef TAO_VAR_ARRAY_SARGUMENT_T_CPP
#define TAO_VAR_ARRAY_SARGUMENT_T_CPP

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#include "tao/PortableServer/Var_Array_SArgument_T.h"
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#if !defined (__ACE_INLINE__)
#include "tao/PortableServer/Var_Array_SArgument_T.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<typename S_forany,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::In_Var_Array_SArgument_T<S_forany,
                              Insert_Policy>::demarshal (TAO_InputCDR & cdr)
{
  S_forany tmp (this->x_);
  return cdr >> tmp;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S_forany,
         template <typename> class Insert_Policy>
void
TAO::In_Var_Array_SArgument_T<S_forany,
                              Insert_Policy>::interceptor_value (
    CORBA::Any *any) const
{
  Insert_Policy<S_forany>::any_insert (any, S_forany (this->x_));
}

#endif /* TAO_HAS_INTERCEPTORS */

// ===========================================================

template<typename S_forany,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::Inout_Var_Array_SArgument_T<S_forany,
                                 Insert_Policy>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << S_forany (this->x_);
}

template<typename S_forany,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::Inout_Var_Array_SArgument_T<S_forany,
                                 Insert_Policy>::demarshal (TAO_InputCDR & cdr)
{
  S_forany tmp (this->x_);
  return cdr >> tmp;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S_forany,
         template <typename> class Insert_Policy>
void
TAO::Inout_Var_Array_SArgument_T<S_forany,
                                 Insert_Policy>::interceptor_value (
  CORBA::Any *any) const
{
  typedef typename S_forany::_slice_type slice_type;
  Insert_Policy<S_forany>::any_insert (
      any,
      S_forany (const_cast <slice_type *>(this->x_))
    );
}

#endif /* TAO_HAS_INTERCEPTORS */

// ==============================================================

template<typename S_var,
         typename S_forany,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::Out_Var_Array_SArgument_T<S_var,
                               S_forany,
                               Insert_Policy>::marshal (
    TAO_OutputCDR & cdr
  )
{
  S_forany tmp (this->x_.ptr ());
  return cdr << tmp;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S_var,
         typename S_forany,
         template <typename> class Insert_Policy>
void
TAO::Out_Var_Array_SArgument_T<S_var,
                               S_forany,
                               Insert_Policy>::interceptor_value (
  CORBA::Any *any) const
{
  Insert_Policy<S_forany>::any_insert (any, S_forany (this->x_.ptr ()));
}

#endif /* TAO_HAS_INTERCEPTORS */

// ============================================================

template<typename S_var,
         typename S_forany,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::Ret_Var_Array_SArgument_T<S_var,
                               S_forany,
                               Insert_Policy>::marshal (
    TAO_OutputCDR & cdr
  )
{
  return cdr << S_forany (this->x_.ptr ());
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S_var,
         typename S_forany,
         template <typename> class Insert_Policy>
void
TAO::Ret_Var_Array_SArgument_T<S_var,
                               S_forany,
                               Insert_Policy>::interceptor_value (
  CORBA::Any *any) const
{
  Insert_Policy<S_forany>::any_insert (any, S_forany (this->x_.ptr ()));
}

#endif /* TAO_HAS_INTERCEPTORS */

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_VAR_ARRAY_SARGUMENT_T_CPP */

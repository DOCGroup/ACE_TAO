// $Id$

#ifndef TAO_UB_STRING_ARGUMENT_T_CPP
#define TAO_UB_STRING_ARGUMENT_T_CPP

#include "tao/UB_String_Argument_T.h"
#include "tao/CDR.h"
#include "ace/OS_NS_string.h"

#if !defined (__ACE_INLINE__)
#include "tao/UB_String_Argument_T.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<typename S_var,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::In_UB_String_Argument_T<S_var,Insert_Policy>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S_var,
         template <typename> class Insert_Policy>
void
TAO::In_UB_String_Argument_T<S_var,Insert_Policy>::interceptor_value (
    CORBA::Any *any) const
{
  Insert_Policy<S_var>::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

template<typename S_var,
         template <typename> class Insert_Policy>
TAO::In_UB_String_Clonable_Argument_T<S_var,Insert_Policy>::~In_UB_String_Clonable_Argument_T (void)
{
  if (this->is_clone_)
    {
      typename S_var::s_traits::char_type * tmp =
           const_cast<typename S_var::s_traits::char_type *> (this->x_);
      delete [] tmp;
    }
}

template<typename S_var,
         template <typename> class Insert_Policy>
TAO::Argument*
TAO::In_UB_String_Clonable_Argument_T<S_var,Insert_Policy>::clone ()
{
  size_t const len = ACE_OS::strlen (this->x_);
  typename S_var::s_traits::char_type * clone_x =
           new typename S_var::s_traits::char_type [len + 1];
  ACE_OS::strncpy (clone_x, this->x_, len);
  clone_x[len] = '\0';
  In_UB_String_Clonable_Argument_T<S_var,Insert_Policy>* clone_arg =
       new In_UB_String_Clonable_Argument_T<S_var,Insert_Policy> (clone_x);
  clone_arg->is_clone_ = true;
  return clone_arg;
}

// ===========================================================

template<typename S_var,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::Inout_UB_String_Argument_T<S_var,Insert_Policy>::marshal (
    TAO_OutputCDR &cdr)
{
  return cdr << this->x_;
}

template<typename S_var,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::Inout_UB_String_Argument_T<S_var,Insert_Policy>::demarshal (
    TAO_InputCDR & cdr)
{
  S_var::s_traits::release (this->x_);
  return cdr >> this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S_var,
         template <typename> class Insert_Policy>
void
TAO::Inout_UB_String_Argument_T<S_var,Insert_Policy>::interceptor_value (
    CORBA::Any *any) const
{
  Insert_Policy<typename S_var::s_traits::char_type const *>::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ==============================================================

template<typename S_var,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::Out_UB_String_Argument_T<S_var,Insert_Policy>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S_var,
         template <typename> class Insert_Policy>
void
TAO::Out_UB_String_Argument_T<S_var,Insert_Policy>::interceptor_value (
    CORBA::Any *any) const
{
  Insert_Policy<typename S_var::s_traits::char_type const *>::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ============================================================

template<typename S_var,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::Ret_UB_String_Argument_T<S_var,Insert_Policy>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_.out ();
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S_var,
         template <typename> class Insert_Policy>
void
TAO::Ret_UB_String_Argument_T<S_var,Insert_Policy>::interceptor_value (
  CORBA::Any *any) const
{
  Insert_Policy<typename S_var::s_traits::char_type const *>::any_insert (any, this->x_.in ());
}

#endif /* TAO_HAS_INTERCEPTORS */

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_UB_STRING_ARGUMENT_T_CPP */

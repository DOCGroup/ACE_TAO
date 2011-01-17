// $Id$

#ifndef TAO_FIXED_ARRAY_ARGUMENT_T_CPP
#define TAO_FIXED_ARRAY_ARGUMENT_T_CPP

#include "tao/Fixed_Array_Argument_T.h"

#if !defined (__ACE_INLINE__)
#include "tao/Fixed_Array_Argument_T.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<typename S_forany,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::In_Fixed_Array_Argument_T<S_forany,Insert_Policy>::marshal (
    TAO_OutputCDR & cdr
  )
{
  return cdr << this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S_forany,
         template <typename> class Insert_Policy>
void
TAO::In_Fixed_Array_Argument_T<S_forany,Insert_Policy>::
  interceptor_value (CORBA::Any *any) const
{
  Insert_Policy<S_forany>::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

template<typename S_forany,
         template <typename> class Insert_Policy>
TAO::In_Fixed_Array_Clonable_Argument_T<S_forany,Insert_Policy>::~In_Fixed_Array_Clonable_Argument_T (void)
{
  if (this->is_clone_)
    {
      typedef TAO::details::array_traits<S_forany> ARRAY_TRAITS;
      typename ARRAY_TRAITS::slice_type * tmp =
               const_cast<typename ARRAY_TRAITS::slice_type *>(this->x_.in ());
      ARRAY_TRAITS::free (tmp);
    }
}

template<typename S_forany,
         template <typename> class Insert_Policy>
TAO::Argument*
TAO::In_Fixed_Array_Clonable_Argument_T<S_forany,Insert_Policy>::clone (void)
{
  typedef TAO::details::array_traits<S_forany> ARRAY_TRAITS;
  typename ARRAY_TRAITS::slice_type * tmp_ptr = 0;
  ACE_ALLOCATOR_RETURN (tmp_ptr,
                        ARRAY_TRAITS::alloc (),
                        0);
  ARRAY_TRAITS::copy (tmp_ptr, this->x_.in ());

  In_Fixed_Array_Clonable_Argument_T<S_forany,Insert_Policy>* clone_arg
    = new In_Fixed_Array_Clonable_Argument_T<S_forany,Insert_Policy> (tmp_ptr);
  clone_arg->is_clone_ = true;
  return clone_arg;
}

// ===========================================================

template<typename S_forany,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::Inout_Fixed_Array_Argument_T<S_forany,Insert_Policy>::marshal (
    TAO_OutputCDR & cdr)
{
  return cdr << this->x_;
}

template<typename S_forany,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::Inout_Fixed_Array_Argument_T<S_forany,Insert_Policy>::demarshal (
    TAO_InputCDR & cdr)
{
  return cdr >> this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S_forany,
         template <typename> class Insert_Policy>
void
TAO::Inout_Fixed_Array_Argument_T<S_forany,Insert_Policy>::
  interceptor_value (CORBA::Any *any) const
{
  Insert_Policy<S_forany>::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ==============================================================

template<typename S_forany,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::Out_Fixed_Array_Argument_T<S_forany,Insert_Policy>::demarshal (
    TAO_InputCDR & cdr)
{
  return cdr >> this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S_forany,
         template <typename> class Insert_Policy>
void
TAO::Out_Fixed_Array_Argument_T<S_forany,Insert_Policy>::
  interceptor_value (CORBA::Any *any) const
{
  Insert_Policy<S_forany>::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ============================================================

template<typename S_var,
         typename S_forany,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::Ret_Fixed_Array_Argument_T<S_var,S_forany,Insert_Policy>::
  demarshal (TAO_InputCDR & cdr)
{
  typedef TAO::Array_Traits<S_forany> ARRAY_TRAITS;
  typename S_forany::_slice_type * tmp_ptr = 0;
  ACE_ALLOCATOR_RETURN (tmp_ptr,
                        ARRAY_TRAITS::alloc (),
                        false);
  this->x_ = tmp_ptr;
  S_forany tmp (this->x_.ptr ());
  return cdr >> tmp;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S_var,
         typename S_forany,
         template <typename> class Insert_Policy>
void
TAO::Ret_Fixed_Array_Argument_T<S_var,S_forany,Insert_Policy>::
interceptor_value (CORBA::Any *any) const
{
  Insert_Policy<S_forany>::any_insert (any, S_forany (this->x_.ptr ()));
}

#endif /* TAO_HAS_INTERCEPTORS */

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_FIXED_ARRAY_ARGUMENT_T_CPP */

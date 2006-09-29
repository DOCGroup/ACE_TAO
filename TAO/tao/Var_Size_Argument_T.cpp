// $Id$

#ifndef TAO_VAR_SIZE_ARGUMENT_T_CPP
#define TAO_VAR_SIZE_ARGUMENT_T_CPP

#include "tao/Var_Size_Argument_T.h"

#if !defined (__ACE_INLINE__)
#include "tao/Var_Size_Argument_T.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<typename S,
         class Insert_Policy>
CORBA::Boolean
TAO::In_Var_Size_Argument_T<S,Insert_Policy>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << *this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S,
         class Insert_Policy>
void
TAO::In_Var_Size_Argument_T<S,Insert_Policy>::interceptor_value (
    CORBA::Any *any
  ) const
{
  Insert_Policy::any_insert (any, *this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

template<typename S,
         class Insert_Policy>
TAO::In_Var_Size_Clonable_Argument_T<S,Insert_Policy>::~In_Var_Size_Clonable_Argument_T (void)
{
  if (this->is_clone_)
    {
      S* tmp = const_cast<S*> (this->x_);
      delete tmp;
    }
}

template<typename S,
         class Insert_Policy>
TAO::Argument*
TAO::In_Var_Size_Clonable_Argument_T<S,Insert_Policy>::clone (void)
{
  S* clone_x = new S (*(this->x_));
  In_Var_Size_Clonable_Argument_T<S,Insert_Policy>* clone_arg
    = new In_Var_Size_Clonable_Argument_T<S,Insert_Policy> (*clone_x);
  clone_arg->is_clone_ = true;
  return clone_arg;
}

// ===========================================================

template<typename S,
         class Insert_Policy>
CORBA::Boolean
TAO::Inout_Var_Size_Argument_T<S,Insert_Policy>::marshal (
    TAO_OutputCDR &cdr
  )
{
  return cdr << *this->x_;
}

template<typename S,
         class Insert_Policy>
CORBA::Boolean
TAO::Inout_Var_Size_Argument_T<S,Insert_Policy>::demarshal (
    TAO_InputCDR & cdr
  )
{
  return cdr >> *this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S,
         class Insert_Policy>
void
TAO::Inout_Var_Size_Argument_T<S,Insert_Policy>::interceptor_value (
    CORBA::Any *any
  ) const
{
  Insert_Policy::any_insert (any, *this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ==============================================================

template<typename S,
         class Insert_Policy>
CORBA::Boolean
TAO::Out_Var_Size_Argument_T<S,Insert_Policy>::demarshal (
    TAO_InputCDR & cdr
  )
{
#if defined (ACE_HAS_NEW_NOTHROW)
  this->x_ = new (ACE_nothrow) S;
#else
  this->x_ = new S;
#endif /* ACE_HAS_NEW_NOTHROW */
  return cdr >> *this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S,
         class Insert_Policy>
void
TAO::Out_Var_Size_Argument_T<S,Insert_Policy>::interceptor_value (
    CORBA::Any *any
  ) const
{
  Insert_Policy::any_insert (any, *this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ============================================================

template<typename S,
         class Insert_Policy>
CORBA::Boolean
TAO::Ret_Var_Size_Argument_T<S,Insert_Policy>::demarshal (
    TAO_InputCDR & cdr
  )
{
  S * tmp = 0;
  ACE_NEW_RETURN (tmp,
                  S (),
                  0);
  this->x_ = tmp;
  return cdr >> this->x_.inout ();
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S,
         class Insert_Policy>
void
TAO::Ret_Var_Size_Argument_T<S,Insert_Policy>::interceptor_value (
    CORBA::Any *any
  ) const
{
  Insert_Policy::any_insert (any, this->x_.in ());
}

#endif /* TAO_HAS_INTERCEPTORS */

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_VAR_SIZE_ARGUMENT_T_CPP */

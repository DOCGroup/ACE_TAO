// $Id$

#ifndef TAO_BASIC_ARGUMENT_T_CPP
#define TAO_BASIC_ARGUMENT_T_CPP

#include "tao/Basic_Argument_T.h"

#if !defined (__ACE_INLINE__)
#include "tao/Basic_Argument_T.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<typename S,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::In_Basic_Argument_T<S, Insert_Policy>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S,
         template <typename> class Insert_Policy>
void
TAO::In_Basic_Argument_T<S, Insert_Policy>::interceptor_value (CORBA::Any *any) const
{
  Insert_Policy<S>::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

template<typename S,
         template <typename> class Insert_Policy>
TAO::In_Basic_Clonable_Argument_T<S,Insert_Policy>::~In_Basic_Clonable_Argument_T ()
{
  if (this->is_clone_)
    {
      S* tmp = const_cast<S*> (&this->x_);
      delete tmp;
    }
}

template<typename S,
         template <typename> class Insert_Policy>
TAO::Argument*
TAO::In_Basic_Clonable_Argument_T<S,Insert_Policy>::clone ()
{
  S* clone_x = new S (this->x_);
  In_Basic_Clonable_Argument_T<S,Insert_Policy>* clone_arg =
          new In_Basic_Clonable_Argument_T<S,Insert_Policy> (*clone_x);
  clone_arg->is_clone_ = true;
  return clone_arg;
}

// ===========================================================

template<typename S,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::Inout_Basic_Argument_T<S, Insert_Policy>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << this->x_;
}

template<typename S,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::Inout_Basic_Argument_T<S, Insert_Policy>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S,
         template <typename> class Insert_Policy>
void
TAO::Inout_Basic_Argument_T<S, Insert_Policy>::interceptor_value (CORBA::Any *any) const
{
  Insert_Policy<S>::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ==============================================================

template<typename S,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::Out_Basic_Argument_T<S, Insert_Policy>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S,
         template <typename> class Insert_Policy>
void
TAO::Out_Basic_Argument_T<S, Insert_Policy>::interceptor_value (CORBA::Any *any) const
{
  Insert_Policy<S>::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ============================================================

template<typename S,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::Ret_Basic_Argument_T<S, Insert_Policy>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S,
         template <typename> class Insert_Policy>
void
TAO::Ret_Basic_Argument_T<S, Insert_Policy>::interceptor_value (CORBA::Any *any) const
{
  Insert_Policy<S>::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_BASIC_ARGUMENT_T_CPP */

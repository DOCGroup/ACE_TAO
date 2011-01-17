// $Id$

#ifndef TAO_OBJECT_ARGUMENT_T_CPP
#define TAO_OBJECT_ARGUMENT_T_CPP

#include "tao/Object_Argument_T.h"

#if !defined (__ACE_INLINE__)
#include "tao/Object_Argument_T.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<typename S_ptr,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::In_Object_Argument_T<S_ptr,Insert_Policy>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S_ptr,
         template <typename> class Insert_Policy>
void
TAO::In_Object_Argument_T<S_ptr,Insert_Policy>::interceptor_value (CORBA::Any *any) const
{
  Insert_Policy<S_ptr>::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

template<typename S_ptr>
void
TAO::In_Object_Argument_Cloner_T<S_ptr>::duplicate (S_ptr objref)
{
  if (objref)
    {
      objref->_add_ref ();
    }
}

template<typename S_ptr>
void
TAO::In_Object_Argument_Cloner_T<S_ptr>::release (S_ptr objref)
{
  if (objref)
    {
      objref->_remove_ref ();
    }
}

template<typename S_ptr,
         template <typename> class Insert_Policy>
TAO::In_Object_Clonable_Argument_T<S_ptr,Insert_Policy>::~In_Object_Clonable_Argument_T (void)
{
  if (this->is_clone_)
    {
      In_Object_Argument_Cloner_T<S_ptr>::release (this->x_);
    }
}

template<typename S_ptr,
         template <typename> class Insert_Policy>
TAO::Argument*
TAO::In_Object_Clonable_Argument_T<S_ptr,Insert_Policy>::clone (void)
{
  In_Object_Argument_Cloner_T<S_ptr>::duplicate (this->x_);

  In_Object_Clonable_Argument_T<S_ptr,Insert_Policy>* clone_arg
    = new In_Object_Clonable_Argument_T<S_ptr,Insert_Policy> (this->x_);
  clone_arg->is_clone_ = true;
  return clone_arg;
}

// ===========================================================

template<typename S_ptr,
         typename S_traits,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::Inout_Object_Argument_T<S_ptr,S_traits,Insert_Policy>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << this->x_;
}

template<typename S_ptr,
         typename S_traits,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::Inout_Object_Argument_T<S_ptr,S_traits,Insert_Policy>::demarshal (TAO_InputCDR & cdr)
{
  S_traits::release (this->x_);
  return cdr >> this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S_ptr,
         typename S_traits,
         template <typename> class Insert_Policy>
void
TAO::Inout_Object_Argument_T<S_ptr,S_traits,Insert_Policy>::interceptor_value (
    CORBA::Any *any) const
{
  Insert_Policy<S_ptr>::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ==============================================================

template<typename S_ptr,
         typename S_out,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::Out_Object_Argument_T<S_ptr,S_out,Insert_Policy>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S_ptr,
         typename S_out,
         template <typename> class Insert_Policy>
void
TAO::Out_Object_Argument_T<S_ptr,S_out,Insert_Policy>::interceptor_value (
    CORBA::Any *any) const
{
  Insert_Policy<S_ptr>::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ============================================================

template<typename S_ptr,
         typename S_var,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::Ret_Object_Argument_T<S_ptr,S_var,Insert_Policy>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_.out ();
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S_ptr,
         typename S_var,
         template <typename> class Insert_Policy>
void
TAO::Ret_Object_Argument_T<S_ptr,S_var,Insert_Policy>::interceptor_value (
    CORBA::Any *any) const
{
  Insert_Policy<S_ptr>::any_insert (any, this->x_.in ());
}

#endif /* TAO_HAS_INTERCEPTORS */

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_OBJECT_ARGUMENT_T_CPP */

// $Id$

#ifndef TAO_BD_STRING_ARGUMENT_T_CPP
#define TAO_BD_STRING_ARGUMENT_T_CPP

#include "tao/BD_String_Argument_T.h"
#include "tao/SystemException.h"

#if !defined (__ACE_INLINE__)
#include "tao/BD_String_Argument_T.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<typename S_var,
         size_t BOUND,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::In_BD_String_Argument_T<S_var,BOUND,Insert_Policy>::marshal (
    TAO_OutputCDR & cdr)
{
  if (this->x_ != 0 && ACE_OS::strlen (this->x_) > BOUND)
    {
      throw ::CORBA::BAD_PARAM ();
    }
  return cdr << this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S_var,
         size_t BOUND,
         template <typename> class Insert_Policy>
void
TAO::In_BD_String_Argument_T<S_var,BOUND,Insert_Policy>::interceptor_value (
  CORBA::Any *any) const
{
  typedef typename S_var::s_traits::from_type from_type;
  Insert_Policy<from_type>::any_insert (
      any,
      from_type (this->x_, BOUND));
}

#endif /* TAO_HAS_INTERCEPTORS */

template<typename S_var,
         size_t BOUND,
         template <typename> class Insert_Policy>
TAO::In_BD_String_Clonable_Argument_T<S_var,BOUND,Insert_Policy>::~In_BD_String_Clonable_Argument_T (void)
{
  if (this->is_clone_)
    {
      typename S_var::s_traits::char_type * tmp =
            const_cast<typename S_var::s_traits::char_type *> (this->x_);
      delete [] tmp;
    }
}

template<typename S_var,
         size_t BOUND,
         template <typename> class Insert_Policy>
TAO::Argument*
TAO::In_BD_String_Clonable_Argument_T<S_var,BOUND,Insert_Policy>::clone (void)
{
  typename S_var::s_traits::char_type * clone_x =
             new typename S_var::s_traits::char_type [BOUND];
  ACE_OS::strncpy (clone_x, this->x_, BOUND);
  In_BD_String_Clonable_Argument_T<S_var,BOUND,Insert_Policy>* clone_arg =
    new In_BD_String_Clonable_Argument_T<S_var,BOUND,Insert_Policy> (clone_x);
  clone_arg->is_clone_ = true;
  return clone_arg;
}

// ===========================================================

template<typename S_var,
         size_t BOUND,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::Inout_BD_String_Argument_T<S_var,BOUND,Insert_Policy>::marshal (
    TAO_OutputCDR & cdr)
{
  if (this->x_ != 0 && ACE_OS::strlen (this->x_) > BOUND)
    {
      throw ::CORBA::BAD_PARAM ();
    }
  return cdr << this->x_;
}

template<typename S_var,
         size_t BOUND,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::Inout_BD_String_Argument_T<S_var,BOUND,Insert_Policy>::demarshal (
    TAO_InputCDR & cdr)
{
  S_var tmp = this->x_;
  CORBA::Boolean const demarshalled = cdr >> this->x_;
  if (this->x_ != 0 && ACE_OS::strlen (this->x_) > BOUND)
    {
      S_var for_deletion = this->x_;
      this->x_ = tmp._retn ();
      throw ::CORBA::BAD_PARAM ();
    }
  return demarshalled;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S_var,
         size_t BOUND,
         template <typename> class Insert_Policy>
void
TAO::Inout_BD_String_Argument_T<S_var,BOUND,Insert_Policy>::interceptor_value (
  CORBA::Any *any) const
{
  typedef typename S_var::s_traits::from_type from_type;
  Insert_Policy<from_type>::any_insert (
      any,
      from_type (this->x_, BOUND));
}

#endif /* TAO_HAS_INTERCEPTORS */

// ==============================================================

template<typename S_var,
         size_t BOUND,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::Out_BD_String_Argument_T<S_var,BOUND,Insert_Policy>::demarshal (
    TAO_InputCDR & cdr)
{
  CORBA::Boolean const demarshalled = cdr >> this->x_;
  if (this->x_ != 0 && ACE_OS::strlen (this->x_) > BOUND)
    {
      throw ::CORBA::BAD_PARAM ();
    }
  return demarshalled;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S_var,
         size_t BOUND,
         template <typename> class Insert_Policy>
void
TAO::Out_BD_String_Argument_T<S_var,BOUND,Insert_Policy>::interceptor_value (
  CORBA::Any *any) const
{
  typedef typename S_var::s_traits::from_type from_type;
  Insert_Policy<from_type>::any_insert (
      any,
      from_type (this->x_, BOUND)
    );
}

#endif /* TAO_HAS_INTERCEPTORS */

// ============================================================

template<typename S_var,
         size_t BOUND,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::Ret_BD_String_Argument_T<S_var,BOUND,Insert_Policy>::demarshal (
    TAO_InputCDR & cdr)
{
  CORBA::Boolean const demarshalled = cdr >> this->x_.out ();
  if (this->x_.in () != 0 && ACE_OS::strlen (this->x_.in ()) > BOUND)
    {
      throw ::CORBA::BAD_PARAM ();
    }
  return demarshalled;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S_var,
         size_t BOUND,
         template <typename> class Insert_Policy>
void
TAO::Ret_BD_String_Argument_T<S_var,BOUND,Insert_Policy>::interceptor_value (
  CORBA::Any *any) const
{
  typedef typename S_var::s_traits::from_type from_type;
  Insert_Policy<from_type>::any_insert (
      any,
      from_type (this->x_.in (), BOUND));
}

#endif /* TAO_HAS_INTERCEPTORS */

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_BD_STRING_ARGUMENT_T_CPP */

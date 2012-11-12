// $Id$

#ifndef TAO_BD_STRING_SARGUMENT_T_CPP
#define TAO_BD_STRING_SARGUMENT_T_CPP

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#include "tao/PortableServer/BD_String_SArgument_T.h"
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "tao/SystemException.h"

#if !defined (__ACE_INLINE__)
#include "tao/PortableServer/BD_String_SArgument_T.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<typename S_var,
         size_t BOUND,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::In_BD_String_SArgument_T<S_var,BOUND,Insert_Policy>::demarshal (
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
TAO::In_BD_String_SArgument_T<S_var,BOUND,Insert_Policy>::interceptor_value (
  CORBA::Any *any) const
{
  typedef typename S_var::s_traits::from_type from_type;
  Insert_Policy<from_type>::any_insert (
      any,
      from_type (this->x_.in (), BOUND));
}

#endif /* TAO_HAS_INTERCEPTORS */

// ===========================================================

template<typename S_var,
         size_t BOUND,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::Inout_BD_String_SArgument_T<S_var,BOUND,Insert_Policy>::marshal (
    TAO_OutputCDR & cdr)
{
  if (this->x_.in () != 0 && ACE_OS::strlen (this->x_.in ()) > BOUND)
    {
      throw ::CORBA::BAD_PARAM ();
    }
  return cdr << this->x_.in ();
}

template<typename S_var,
         size_t BOUND,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::Inout_BD_String_SArgument_T<S_var,BOUND,Insert_Policy>::demarshal (
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
TAO::Inout_BD_String_SArgument_T<S_var,BOUND,Insert_Policy>::
interceptor_value (CORBA::Any *any) const
{
  typedef typename S_var::s_traits::from_type from_type;
  Insert_Policy<from_type>::any_insert (
      any,
      from_type (this->x_.in (), BOUND));
}

#endif /* TAO_HAS_INTERCEPTORS */

// ==============================================================

template<typename S_var,
         size_t BOUND,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::Out_BD_String_SArgument_T<S_var,BOUND,Insert_Policy>::marshal (
    TAO_OutputCDR &cdr)
{
  if (this->x_.in () != 0 && ACE_OS::strlen (this->x_.in ()) > BOUND)
    {
      throw ::CORBA::BAD_PARAM ();
    }
  return cdr << this->x_.in ();
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S_var,
         size_t BOUND,
         template <typename> class Insert_Policy>
void
TAO::Out_BD_String_SArgument_T<S_var,BOUND,Insert_Policy>::
interceptor_value (CORBA::Any *any) const
{
  typedef typename S_var::s_traits::from_type from_type;
  Insert_Policy<from_type>::any_insert (
      any,
      from_type (this->x_.in (), BOUND));
}

#endif /* TAO_HAS_INTERCEPTORS */

// ============================================================

template<typename S_var,
         size_t BOUND,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::Ret_BD_String_SArgument_T<S_var,BOUND,Insert_Policy>::marshal (
    TAO_OutputCDR & cdr
  )
{
  if (this->x_.in () != 0 && ACE_OS::strlen (this->x_.in ()) > BOUND)
    {
      throw ::CORBA::BAD_PARAM ();
    }
  return cdr << this->x_.in ();
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S_var,
         size_t BOUND,
         template <typename> class Insert_Policy>
void
TAO::Ret_BD_String_SArgument_T<S_var,BOUND,Insert_Policy>::
interceptor_value (CORBA::Any *any) const
{
  typedef typename S_var::s_traits::from_type from_type;
  Insert_Policy<from_type>::any_insert (
      any,
      from_type (this->x_.in (), BOUND)
    );
}

#endif /* TAO_HAS_INTERCEPTORS */

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_BD_STRING_SARGUMENT_T_CPP */

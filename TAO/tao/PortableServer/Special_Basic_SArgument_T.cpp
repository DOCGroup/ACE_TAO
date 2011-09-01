// $Id$

#ifndef TAO_SPECIAL_BASIC_SARGUMENT_T_CPP
#define TAO_SPECIAL_BASIC_SARGUMENT_T_CPP

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#include "tao/PortableServer/Special_Basic_SArgument_T.h"
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#if !defined (__ACE_INLINE__)
#include "tao/PortableServer/Special_Basic_SArgument_T.inl"
#endif /* __ACE_INLINE__ */


// ============================================================

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<typename S,
         typename to_S,
         typename from_S,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::In_Special_Basic_SArgument_T<S,to_S,from_S,Insert_Policy>::demarshal (
    TAO_InputCDR & cdr
  )
{
  return cdr >> to_S (this->x_);
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S,
         typename to_S,
         typename from_S,
         template <typename> class Insert_Policy>
void
TAO::In_Special_Basic_SArgument_T<S,to_S,from_S,Insert_Policy>::
interceptor_value (CORBA::Any *any) const
{
  Insert_Policy<from_S>::any_insert (any, from_S (this->x_));
}

#endif /* TAO_HAS_INTERCEPTORS */

// ===========================================================

template<typename S,
         typename to_S,
         typename from_S,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::Inout_Special_Basic_SArgument_T<S,to_S,from_S,Insert_Policy>::marshal (
    TAO_OutputCDR & cdr
  )
{
  return cdr << from_S (this->x_);
}

template<typename S,
         typename to_S,
         typename from_S,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::Inout_Special_Basic_SArgument_T<S,to_S,from_S,Insert_Policy>::demarshal (
    TAO_InputCDR & cdr
  )
{
  return cdr >> to_S (this->x_);
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S,
         typename to_S,
         typename from_S,
         template <typename> class Insert_Policy>
void
TAO::Inout_Special_Basic_SArgument_T<S,to_S,from_S,Insert_Policy>::
interceptor_value (CORBA::Any *any) const
{
  Insert_Policy<from_S>::any_insert (any, from_S (this->x_));
}

#endif /* TAO_HAS_INTERCEPTORS */

// ==============================================================

template<typename S,
         typename to_S,
         typename from_S,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::Out_Special_Basic_SArgument_T<S,to_S,from_S,Insert_Policy>::marshal (
    TAO_OutputCDR & cdr
  )
{
  return cdr << from_S (this->x_);
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S,
         typename to_S,
         typename from_S,
         template <typename> class Insert_Policy>
void
TAO::Out_Special_Basic_SArgument_T<S,to_S,from_S,Insert_Policy>::
interceptor_value (CORBA::Any *any) const
{
  Insert_Policy<from_S>::any_insert (any, from_S (this->x_));
}

#endif /* TAO_HAS_INTERCEPTORS */

// ==============================================================

template<typename S,
         typename to_S,
         typename from_S,
         template <typename> class Insert_Policy>
CORBA::Boolean
TAO::Ret_Special_Basic_SArgument_T<S,to_S,from_S,Insert_Policy>::marshal (
    TAO_OutputCDR & cdr
  )
{
  return cdr << from_S (this->x_);
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S,
         typename to_S,
         typename from_S,
         template <typename> class Insert_Policy>
void
TAO::Ret_Special_Basic_SArgument_T<S,to_S,from_S,Insert_Policy>::
interceptor_value (CORBA::Any *any) const
{
  Insert_Policy<from_S>::any_insert (any, from_S (this->x_));
}

#endif /* TAO_HAS_INTERCEPTORS */

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_SPECIAL_BASIC_SARGUMENT_T_CPP */

// $Id$

#ifndef TAO_TYPECODE_DEFAULT_CASE_CPP
#define TAO_TYPECODE_DEFAULT_CASE_CPP

#include "TypeCode_Default_Case.h"

#include "tao/CDR.h"
#include "tao/Any.h"

#ifndef __ACE_INLINE__
# include "tao/TypeCode_Default_Case.inl"
#endif /* __ACE_INLINE__ */


namespace TAO
{
  namespace TypeCode
  {

    // Default case's label is a zero octet.
    static TAO_OutputCDR::from_octet const zero_octet (0);

  }  // Namespace TypeCode
} // Namespace TAO

// ------------------------------------------------------------

template <typename STRING_TYPE>
bool
TAO::TypeCode::Default_Case<STRING_TYPE>::marshal_label (
  TAO_OutputCDR & cdr) const
{
  // Default case's label is a zero octet.
  return (cdr << TAO::TypeCode::zero_octet);
}

template <typename DISCRIMINATOR_TYPE, typename STRING_TYPE>
bool
TAO::TypeCode::Default_Case<DISCRIMINATOR_TYPE,
                            STRING_TYPE>::equal_label (
  CORBA::ULong index
  CORBA::TypeCode_ptr tc
  ACE_ENV_ARG_DECL) const
{
  CORBA::Any_var const any = tc->member_label (index
                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (false);


  // Label for default case is always zero octet.
  static CORBA::Octet const this_label = 0;

  CORBA::Octet tc_label;
  if (any.in () >>= CORBA::Any::to_octet (tc_label)
      && this_label == tc_label)
    {
      return true;
    }

  return false;
}

template <typename STRING_TYPE>
CORBA::Any *
TAO::TypeCode::Default_Case<STRING_TYPE>::label (
  ACE_ENV_SINGLE_ARG_DECL) const
{
  CORBA::Any * value;

  ACE_NEW_THROW_EX (value,
                    CORBA::Any,
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  CORBA::Any_var safe_value (value);

  *value <<= TAO::TypeCode::zero_octet;

  return safe_value._retn ();
}



#endif  /* TAO_TYPECODE_DEFAULT_CASE_CPP */

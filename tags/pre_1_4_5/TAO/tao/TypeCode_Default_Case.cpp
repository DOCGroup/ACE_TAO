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

template <typename StringType, typename TypeCodeType>
bool
TAO::TypeCode::Default_Case<StringType, TypeCodeType>::marshal_label (
  TAO_OutputCDR & cdr) const
{
  // Default case's label is a zero octet.

  /**
   * @todo This isn't correct.  We should be marshaling any legal
   *       label value for the given union.  It is only marshaled for
   *       syntactic correctness, and is ignored otherwise.
   */

  return (cdr << TAO::TypeCode::zero_octet);
}

template <typename StringType, typename TypeCodeType>
bool
TAO::TypeCode::Default_Case<StringType, TypeCodeType>::equal_label (CORBA::ULong index,
                                                       CORBA::TypeCode_ptr tc
                                                       ACE_ENV_ARG_DECL) const
{
  CORBA::Any_var const any = tc->member_label (index
                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (false);


  // Label for default case is always zero octet.
  static CORBA::Octet const this_label = 0;

  CORBA::Octet tc_label;
  if ((any.in () >>= CORBA::Any::to_octet (tc_label))
      && this_label == tc_label)
    {
      return true;
    }

  return false;
}

template <typename StringType, typename TypeCodeType>
CORBA::Any *
TAO::TypeCode::Default_Case<StringType, TypeCodeType>::label (
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

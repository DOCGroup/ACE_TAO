// $Id$

#ifndef TAO_TYPECODE_NON_DEFAULT_CASE_CPP
#define TAO_TYPECODE_NON_DEFAULT_CASE_CPP

#include "TypeCode_Non_Default_Case.h"

#include "tao/CDR.h"
#include "tao/Any.h"

#ifndef __ACE_INLINE__
# include "tao/TypeCode_Non_Default_Case.inl"
#endif /* __ACE_INLINE__ */

namespace TAO
{
  namespace TypeCode
  {
    template <typename T>
    struct Case_Traits
    {
      inline static T any_from (T v)
      {
        return v;
      }
    };

    // Specializations for types that require wrapper for Any
    // insertion.  Note that we only define specializations for types
    // that can be used in an IDL union discriminator.

    template <>
    struct Case_Traits<CORBA::Boolean>
    {
      inline static CORBA::Any::from_boolean any_from (CORBA::Boolean v)
      {
        return CORBA::Any::from_boolean (v);
      }
    };

    template <>
    struct Case_Traits<CORBA::Char>
    {
      inline static CORBA::Any::from_char any_from (CORBA::Char v)
      {
        return CORBA::Any::from_char (v);
      }
    };

    template <>
    struct Case_Traits<CORBA::WChar>
    {
      inline static CORBA::Any::from_wchar any_from (CORBA::WChar v)
      {
        return CORBA::Any::from_wchar (v);
      }
    };

  } // End TypeCode namespace
}  // End TAO namespace

// ----------------------------------------------------------------

template <typename DISCRIMINATOR_TYPE, typename STRING_TYPE>
bool
TAO::TypeCode::Non_Default_Case<DISCRIMINATOR_TYPE, STRING_TYPE>::marshal_label (
  TAO_OutputCDR & cdr) const
{
  return (cdr << this->label_);
}

template <typename DISCRIMINATOR_TYPE, typename STRING_TYPE>
CORBA::Any *
TAO::TypeCode::Non_Default_Case<DISCRIMINATOR_TYPE, STRING_TYPE>::label (
  ACE_ENV_SINGLE_ARG_DECL) const
{
  CORBA::Any * value;

  ACE_NEW_THROW_EX (value,
                    CORBA::Any,
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  CORBA::Any_var safe_value (value);

  *value <<=
    TAO::TypeCode::Case_Traits<DISCRIMINATOR_TYPE>::any_from (this->label_);

  return safe_value._retn ();
}



#endif  /* TAO_TYPECODE_NON_DEFAULT_CASE_CPP */

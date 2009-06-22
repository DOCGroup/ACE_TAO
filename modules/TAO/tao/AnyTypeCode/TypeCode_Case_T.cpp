// $Id$

#ifndef TAO_TYPECODE_CASE_T_CPP
#define TAO_TYPECODE_CASE_T_CPP

#include "tao/AnyTypeCode/TypeCode_Case_T.h"

#include "tao/CDR.h"
#include "tao/SystemException.h"
#include "tao/AnyTypeCode/Any.h"
#include "ace/CORBA_macros.h"

#ifndef __ACE_INLINE__
# include "tao/AnyTypeCode/TypeCode_Case_T.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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

      inline static T & any_to (T & v)
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

      inline static CORBA::Any::to_boolean any_to (CORBA::Boolean & v)
      {
        return CORBA::Any::to_boolean (v);
      }
    };

    template <>
    struct Case_Traits<CORBA::Char>
    {
      inline static CORBA::Any::from_char any_from (CORBA::Char v)
      {
        return CORBA::Any::from_char (v);
      }

      inline static CORBA::Any::to_char any_to (CORBA::Char & v)
      {
        return CORBA::Any::to_char (v);
      }
    };

  } // End TypeCode namespace
}  // End TAO namespace

// ----------------------------------------------------------------


template <typename DiscriminatorType,
          typename StringType,
          typename TypeCodeType>
TAO::TypeCode::Case<StringType, TypeCodeType> *
TAO::TypeCode::Case_T<DiscriminatorType,
                      StringType,
                      TypeCodeType>::clone (void) const
{
  Case<StringType, TypeCodeType> * p = 0;

  typedef Case_T<DiscriminatorType,
                 StringType,
                 TypeCodeType> case_type;

  // The compiler generated memberwise copy constructor is sufficient.
  ACE_NEW_RETURN (p,
                  case_type (*this),
                  p);

  return p;
}

template <typename DiscriminatorType,
          typename StringType,
          typename TypeCodeType>
bool
TAO::TypeCode::Case_T<DiscriminatorType,
                      StringType,
                      TypeCodeType>::marshal_label (TAO_OutputCDR & cdr) const
{
  return
    (cdr <<
     TAO::TypeCode::Case_Traits<DiscriminatorType>::any_from (this->label_));
}

template <typename DiscriminatorType,
          typename StringType,
          typename TypeCodeType>
bool
TAO::TypeCode::Case_T<DiscriminatorType,
                      StringType,
                      TypeCodeType>::equal_label (CORBA::ULong index,
                                                  CORBA::TypeCode_ptr tc
                                                  ) const
{
  CORBA::Any_var const any = tc->member_label (index);

  // The equality operator == below is guaranteed to be defined for
  // the discriminator type since an IDL union discriminator type must
  // be any of the following: (1) an integer, (2) a character, (3) a
  // boolean, or (4) an enumeration.

  DiscriminatorType tc_label;
  if ((any.in ()
       >>= TAO::TypeCode::Case_Traits<DiscriminatorType>::any_to (tc_label))
      && this->label_ == tc_label)
    {
      return true;
    }

  return false;
}

template <typename DiscriminatorType,
          typename StringType,
          typename TypeCodeType>
CORBA::Any *
TAO::TypeCode::Case_T<DiscriminatorType,
                      StringType,
                      TypeCodeType>::label (void) const
{
  CORBA::Any * value = 0;

  ACE_NEW_THROW_EX (value,
                    CORBA::Any,
                    CORBA::NO_MEMORY ());

  CORBA::Any_var safe_value (value);

  *value <<=
    TAO::TypeCode::Case_Traits<DiscriminatorType>::any_from (this->label_);

  return safe_value._retn ();
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /* TAO_TYPECODE_CASE_T_CPP */

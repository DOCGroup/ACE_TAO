// -*- C++ -*-

//=============================================================================
/**
 *  @file    BasicTypeTraits.h
 *
 *  Specializations of template traits in BasicTypeTraits_T.h.
 *
 *  @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef BASIC_TYPE_TRAITS_H
#define BASIC_TYPE_TRAITS_H

#include "tao/AnyTypeCode/BasicTypeTraits_T.h"
#include "tao/AnyTypeCode/Any.h"

#include "tao/Typecode_typesC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  class TypeCode;
  using TypeCode_ptr = TypeCode *;

  class BooleanSeq;
  class CharSeq;
  class OctetSeq;
  class WCharSeq;
  class ShortSeq;
  class UShortSeq;
  class LongSeq;
  class ULongSeq;
  class LongLongSeq;
  class ULongLongSeq;
  class FloatSeq;
  class DoubleSeq;
  class LongDoubleSeq;
  class AnySeq;
}

namespace TAO
{
  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::Boolean>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    using insert_type = CORBA::Any::from_boolean;
    using extract_type = CORBA::Any::to_boolean;
    using return_type = CORBA::Boolean;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::Octet>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    using insert_type = CORBA::Any::from_octet;
    using extract_type = CORBA::Any::to_octet;
    using return_type = CORBA::Octet;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::Char>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    using insert_type = CORBA::Any::from_char;
    using extract_type = CORBA::Any::to_char;
    using return_type = CORBA::Char;

    static return_type convert (extract_type& et);
  };

#if (defined (ACE_HAS_WCHAR) || defined (ACE_HAS_XPG4_MULTIBYTE_CHAR)) && !defined (ACE_LACKS_NATIVE_WCHAR_T)
  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::WChar>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    using insert_type = CORBA::Any::from_wchar;
    using extract_type = CORBA::Any::to_wchar;
    using return_type = CORBA::WChar;

    static return_type convert (extract_type& et);
  };
#endif

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::Short>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    using insert_type = CORBA::Short;
    using extract_type = CORBA::Short;
    using return_type = CORBA::Short;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::UShort>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    using insert_type = CORBA::UShort;
    using extract_type = CORBA::UShort;
    using return_type = CORBA::UShort;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::Long>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    using insert_type = CORBA::Long;
    using extract_type = CORBA::Long;
    using return_type = CORBA::Long;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::ULong>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    using insert_type = CORBA::ULong;
    using extract_type = CORBA::ULong;
    using return_type = CORBA::ULong;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::LongLong>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    using insert_type = CORBA::LongLong;
    using extract_type = CORBA::LongLong;
    using return_type = CORBA::LongLong;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::ULongLong>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    using insert_type = CORBA::ULongLong;
    using extract_type = CORBA::ULongLong;
    using return_type = CORBA::ULongLong;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::Float>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    using insert_type = CORBA::Float;
    using extract_type = CORBA::Float;
    using return_type = CORBA::Float;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::Double>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    using insert_type = CORBA::Double;
    using extract_type = CORBA::Double;
    using return_type = CORBA::Double;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::LongDouble>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    using insert_type = CORBA::LongDouble;
    using extract_type = CORBA::LongDouble;
    using return_type = CORBA::LongDouble;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::Char *>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    using insert_type = CORBA::Char *;
    using extract_type = CORBA::Char const *;
    using return_type = CORBA::Char const *;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::WChar *>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    using insert_type = CORBA::WChar *;
    using extract_type = CORBA::WChar const *;
    using return_type = CORBA::WChar const *;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::Any>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    using insert_type = CORBA::Any;
    using extract_type = CORBA::Any const *;
    using return_type = CORBA::Any const *;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::Object_ptr>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    using insert_type = CORBA::Object_ptr;
    using extract_type = CORBA::Object_ptr;
    using return_type = CORBA::Object_ptr;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::TypeCode_ptr>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    using insert_type = CORBA::TypeCode_ptr;
    using extract_type = CORBA::TypeCode_ptr const;
    using return_type = CORBA::TypeCode_ptr;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::BooleanSeq>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    using insert_type = CORBA::BooleanSeq;
    using extract_type = CORBA::BooleanSeq const *;
    using return_type = CORBA::BooleanSeq const *;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::OctetSeq>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value ;

    using insert_type = CORBA::OctetSeq;
    using extract_type = CORBA::OctetSeq const *;
    using return_type = CORBA::OctetSeq const *;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::CharSeq>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    using insert_type = CORBA::CharSeq;
    using extract_type = CORBA::CharSeq const *;
    using return_type = CORBA::CharSeq const *;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::WCharSeq>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    using insert_type = CORBA::WCharSeq;
    using extract_type = CORBA::WCharSeq const *;
    using return_type = CORBA::WCharSeq const *;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::ShortSeq>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    using insert_type = CORBA::ShortSeq;
    using extract_type = CORBA::ShortSeq const *;
    using return_type = CORBA::ShortSeq const *;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::UShortSeq>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    using insert_type = CORBA::UShortSeq;
    using extract_type = CORBA::UShortSeq const *;
    using return_type = CORBA::UShortSeq const *;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::LongSeq>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    using insert_type = CORBA::LongSeq;
    using extract_type = CORBA::LongSeq const *;
    using return_type = CORBA::LongSeq const *;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::ULongSeq>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    using insert_type = CORBA::ULongSeq;
    using extract_type = CORBA::ULongSeq const *;
    using return_type = CORBA::ULongSeq const *;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::LongLongSeq>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    using insert_type = CORBA::LongLongSeq;
    using extract_type = CORBA::LongLongSeq const *;
    using return_type = CORBA::LongLongSeq const *;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::ULongLongSeq>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    using insert_type = CORBA::ULongLongSeq;
    using extract_type = CORBA::ULongLongSeq const *;
    using return_type = CORBA::ULongLongSeq const *;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::FloatSeq>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    using insert_type = CORBA::FloatSeq;
    using extract_type = CORBA::FloatSeq const *;
    using return_type = CORBA::FloatSeq const *;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::DoubleSeq>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    using insert_type = CORBA::DoubleSeq;
    using extract_type = CORBA::DoubleSeq const *;
    using return_type = CORBA::DoubleSeq const *;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::LongDoubleSeq>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    using insert_type = CORBA::LongDoubleSeq;
    using extract_type = CORBA::LongDoubleSeq const *;
    using return_type = CORBA::LongDoubleSeq const *;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::AnySeq>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    using insert_type = CORBA::AnySeq;
    using extract_type = CORBA::AnySeq const *;
    using return_type = CORBA::AnySeq const *;

    static return_type convert (extract_type& et);
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* BASIC_TYPE_TRAITS_H */

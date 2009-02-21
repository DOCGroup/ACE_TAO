// -*- C++ -*-

//=============================================================================
/**
 *  @file    BasicTypeTraits.h
 *
 *  $Id$
 *
 *  Specializations of template traits in BasicTypeTraits_T.h.
 *
 *  @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

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
  typedef TypeCode * TypeCode_ptr;

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

    typedef CORBA::Any::from_boolean insert_type;
    typedef CORBA::Any::to_boolean extract_type;
    typedef CORBA::Boolean return_type;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::Octet>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    typedef CORBA::Any::from_octet insert_type;
    typedef CORBA::Any::to_octet extract_type;
    typedef CORBA::Octet return_type;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::Char>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    typedef CORBA::Any::from_char insert_type;
    typedef CORBA::Any::to_char extract_type;
    typedef CORBA::Char return_type;

    static return_type convert (extract_type& et);
  };

#if (defined (ACE_HAS_WCHAR) || defined (ACE_HAS_XPG4_MULTIBYTE_CHAR)) && !defined (ACE_LACKS_NATIVE_WCHAR_T)
  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::WChar>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    typedef CORBA::Any::from_wchar insert_type;
    typedef CORBA::Any::to_wchar extract_type;
    typedef CORBA::WChar return_type;

    static return_type convert (extract_type& et);
  };
#endif

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::Short>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    typedef CORBA::Short insert_type;
    typedef CORBA::Short extract_type;
    typedef CORBA::Short return_type;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::UShort>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    typedef CORBA::UShort insert_type;
    typedef CORBA::UShort extract_type;
    typedef CORBA::UShort return_type;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::Long>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    typedef CORBA::Long insert_type;
    typedef CORBA::Long extract_type;
    typedef CORBA::Long return_type;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::ULong>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    typedef CORBA::ULong insert_type;
    typedef CORBA::ULong extract_type;
    typedef CORBA::ULong return_type;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::LongLong>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    typedef CORBA::LongLong insert_type;
    typedef CORBA::LongLong extract_type;
    typedef CORBA::LongLong return_type;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::ULongLong>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    typedef CORBA::ULongLong insert_type;
    typedef CORBA::ULongLong extract_type;
    typedef CORBA::ULongLong return_type;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::Float>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    typedef CORBA::Float insert_type;
    typedef CORBA::Float extract_type;
    typedef CORBA::Float return_type;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::Double>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    typedef CORBA::Double insert_type;
    typedef CORBA::Double extract_type;
    typedef CORBA::Double return_type;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::LongDouble>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    typedef CORBA::LongDouble insert_type;
    typedef CORBA::LongDouble extract_type;
    typedef CORBA::LongDouble return_type;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::Char *>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    typedef CORBA::Char * insert_type;
    typedef CORBA::Char * extract_type;
    typedef CORBA::Char * return_type;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::WChar *>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    typedef CORBA::WChar * insert_type;
    typedef CORBA::WChar * extract_type;
    typedef CORBA::WChar * return_type;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::Any>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    typedef CORBA::Any insert_type;
    typedef CORBA::Any * extract_type;
    typedef CORBA::Any * return_type;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::Object_ptr>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    typedef CORBA::Object_ptr insert_type;
    typedef CORBA::Object_ptr extract_type;
    typedef CORBA::Object_ptr return_type;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::TypeCode_ptr>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    typedef CORBA::TypeCode_ptr insert_type;
    typedef CORBA::TypeCode_ptr extract_type;
    typedef CORBA::TypeCode_ptr return_type;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::BooleanSeq>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    typedef CORBA::BooleanSeq insert_type;
    typedef CORBA::BooleanSeq * extract_type;
    typedef CORBA::BooleanSeq * return_type;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::OctetSeq>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value ;

    typedef CORBA::OctetSeq insert_type;
    typedef CORBA::OctetSeq * extract_type;
    typedef CORBA::OctetSeq * return_type;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::CharSeq>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    typedef CORBA::CharSeq insert_type;
    typedef CORBA::CharSeq * extract_type;
    typedef CORBA::CharSeq * return_type;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::WCharSeq>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    typedef CORBA::WCharSeq insert_type;
    typedef CORBA::WCharSeq * extract_type;
    typedef CORBA::WCharSeq * return_type;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::ShortSeq>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    typedef CORBA::ShortSeq insert_type;
    typedef CORBA::ShortSeq * extract_type;
    typedef CORBA::ShortSeq * return_type;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::UShortSeq>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    typedef CORBA::UShortSeq insert_type;
    typedef CORBA::UShortSeq * extract_type;
    typedef CORBA::UShortSeq * return_type;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::LongSeq>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    typedef CORBA::LongSeq insert_type;
    typedef CORBA::LongSeq * extract_type;
    typedef CORBA::LongSeq * return_type;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::ULongSeq>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    typedef CORBA::ULongSeq insert_type;
    typedef CORBA::ULongSeq * extract_type;
    typedef CORBA::ULongSeq * return_type;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::LongLongSeq>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    typedef CORBA::LongLongSeq insert_type;
    typedef CORBA::LongLongSeq * extract_type;
    typedef CORBA::LongLongSeq * return_type;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::ULongLongSeq>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    typedef CORBA::ULongLongSeq insert_type;
    typedef CORBA::ULongLongSeq * extract_type;
    typedef CORBA::ULongLongSeq * return_type;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::FloatSeq>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    typedef CORBA::FloatSeq insert_type;
    typedef CORBA::FloatSeq * extract_type;
    typedef CORBA::FloatSeq * return_type;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::DoubleSeq>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    typedef CORBA::DoubleSeq insert_type;
    typedef CORBA::DoubleSeq * extract_type;
    typedef CORBA::DoubleSeq * return_type;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::LongDoubleSeq>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    typedef CORBA::LongDoubleSeq insert_type;
    typedef CORBA::LongDoubleSeq * extract_type;
    typedef CORBA::LongDoubleSeq * return_type;

    static return_type convert (extract_type& et);
  };

  template<>
  struct TAO_AnyTypeCode_Export BasicTypeTraits<CORBA::AnySeq>
  {
    static CORBA::TypeCode_ptr const tc_value;
    static CORBA::TCKind const tckind_value;

    typedef CORBA::AnySeq insert_type;
    typedef CORBA::AnySeq * extract_type;
    typedef CORBA::AnySeq * return_type;

    static return_type convert (extract_type& et);
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL


// -*- C++ -*-

//=============================================================================
/**
 *  @file    BasicTypeTraits.cpp
 *
 *  $Id$
 *
 *  Assign values to specialization members in BasicTypeTraits.h.
 *
 *  @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#include "tao/AnyTypeCode/BasicTypeTraits.h"
#include "tao/AnyTypeCode/BooleanSeqA.h"
#include "tao/AnyTypeCode/CharSeqA.h"
#include "tao/AnyTypeCode/OctetSeqA.h"
#include "tao/AnyTypeCode/WCharSeqA.h"
#include "tao/AnyTypeCode/ShortSeqA.h"
#include "tao/AnyTypeCode/UShortSeqA.h"
#include "tao/AnyTypeCode/LongSeqA.h"
#include "tao/AnyTypeCode/ULongSeqA.h"
#include "tao/AnyTypeCode/LongLongSeqA.h"
#include "tao/AnyTypeCode/ULongLongSeqA.h"
#include "tao/AnyTypeCode/FloatSeqA.h"
#include "tao/AnyTypeCode/DoubleSeqA.h"
#include "tao/AnyTypeCode/LongDoubleSeqA.h"
#include "tao/AnyTypeCode/AnySeqA.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  CORBA::TypeCode_ptr const
  BasicTypeTraits<CORBA::Boolean>::tc_value = CORBA::_tc_boolean;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::Boolean>::tckind_value = CORBA::tk_boolean;

  //================================================================

  CORBA::TypeCode_ptr const
  BasicTypeTraits<CORBA::Octet>::tc_value = CORBA::_tc_octet;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::Octet>::tckind_value = CORBA::tk_octet;

  //================================================================

  CORBA::TypeCode_ptr const
  BasicTypeTraits<CORBA::Char>::tc_value = CORBA::_tc_char;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::Char>::tckind_value = CORBA::tk_char;

  //================================================================

#if (defined (ACE_HAS_WCHAR) || defined (ACE_HAS_XPG4_MULTIBYTE_CHAR)) && !defined (ACE_LACKS_NATIVE_WCHAR_T)

  CORBA::TypeCode_ptr const
  BasicTypeTraits<CORBA::WChar>::tc_value = CORBA::_tc_wchar;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::WChar>::tckind_value = CORBA::tk_wchar;

#endif

  //================================================================

  CORBA::TypeCode_ptr const
  BasicTypeTraits<CORBA::Short>::tc_value = CORBA::_tc_short;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::Short>::tckind_value = CORBA::tk_short;

  //================================================================

  CORBA::TypeCode_ptr const
  BasicTypeTraits<CORBA::UShort>::tc_value = CORBA::_tc_ushort;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::UShort>::tckind_value = CORBA::tk_ushort;

  //================================================================

  CORBA::TypeCode_ptr const
  BasicTypeTraits<CORBA::Long>::tc_value = CORBA::_tc_long;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::Long>::tckind_value = CORBA::tk_long;

  //================================================================

  CORBA::TypeCode_ptr const
  BasicTypeTraits<CORBA::ULong>::tc_value = CORBA::_tc_ulong;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::ULong>::tckind_value = CORBA::tk_ulong;

  //================================================================

  CORBA::TypeCode_ptr const
  BasicTypeTraits<CORBA::LongLong>::tc_value = CORBA::_tc_longlong;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::LongLong>::tckind_value = CORBA::tk_longlong;

  //================================================================

  CORBA::TypeCode_ptr const
  BasicTypeTraits<CORBA::ULongLong>::tc_value = CORBA::_tc_ulonglong;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::ULongLong>::tckind_value = CORBA::tk_ulonglong;

  //================================================================

  CORBA::TypeCode_ptr const
  BasicTypeTraits<CORBA::Float>::tc_value = CORBA::_tc_float;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::Float>::tckind_value = CORBA::tk_float;

  //================================================================

  CORBA::TypeCode_ptr const
  BasicTypeTraits<CORBA::Double>::tc_value = CORBA::_tc_double;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::Double>::tckind_value = CORBA::tk_double;

  //================================================================

  CORBA::TypeCode_ptr const
  BasicTypeTraits<CORBA::LongDouble>::tc_value = CORBA::_tc_longdouble;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::LongDouble>::tckind_value = CORBA::tk_longdouble;

  //================================================================

  CORBA::TypeCode_ptr const
  BasicTypeTraits<CORBA::Char *>::tc_value = CORBA::_tc_string;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::Char *>::tckind_value = CORBA::tk_string;

  //================================================================

  CORBA::TypeCode_ptr const
  BasicTypeTraits<CORBA::WChar *>::tc_value = CORBA::_tc_wstring;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::WChar *>::tckind_value = CORBA::tk_wstring;

  //================================================================

  CORBA::TypeCode_ptr const
  BasicTypeTraits<CORBA::Any>::tc_value = CORBA::_tc_any;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::Any>::tckind_value = CORBA::tk_any;

  //================================================================

  CORBA::TypeCode_ptr const
  BasicTypeTraits<CORBA::Object_ptr>::tc_value = CORBA::_tc_Object;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::Object_ptr>::tckind_value = CORBA::tk_objref;

  //================================================================

  CORBA::TypeCode_ptr const
  BasicTypeTraits<CORBA::TypeCode_ptr>::tc_value = CORBA::_tc_TypeCode;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::TypeCode_ptr>::tckind_value = CORBA::tk_TypeCode;

  //================================================================

  CORBA::TypeCode_ptr const
  BasicTypeTraits<CORBA::BooleanSeq>::tc_value = CORBA::_tc_BooleanSeq;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::BooleanSeq>::tckind_value = CORBA::tk_sequence;

  //================================================================

  CORBA::TypeCode_ptr const
  BasicTypeTraits<CORBA::OctetSeq>::tc_value = CORBA::_tc_OctetSeq;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::OctetSeq>::tckind_value = CORBA::tk_sequence;

  //================================================================

  CORBA::TypeCode_ptr const
  BasicTypeTraits<CORBA::CharSeq>::tc_value = CORBA::_tc_CharSeq;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::CharSeq>::tckind_value = CORBA::tk_sequence;

  //================================================================

  CORBA::TypeCode_ptr const
  BasicTypeTraits<CORBA::WCharSeq>::tc_value = CORBA::_tc_WCharSeq;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::WCharSeq>::tckind_value = CORBA::tk_sequence;

  //================================================================

  CORBA::TypeCode_ptr const
  BasicTypeTraits<CORBA::ShortSeq>::tc_value = CORBA::_tc_ShortSeq;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::ShortSeq>::tckind_value = CORBA::tk_sequence;

  //================================================================

  CORBA::TypeCode_ptr const
  BasicTypeTraits<CORBA::UShortSeq>::tc_value = CORBA::_tc_UShortSeq;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::UShortSeq>::tckind_value = CORBA::tk_sequence;

  //================================================================

  CORBA::TypeCode_ptr const
  BasicTypeTraits<CORBA::LongSeq>::tc_value = CORBA::_tc_LongSeq;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::LongSeq>::tckind_value = CORBA::tk_sequence;

  //================================================================

  CORBA::TypeCode_ptr const
  BasicTypeTraits<CORBA::ULongSeq>::tc_value = CORBA::_tc_ULongSeq;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::ULongSeq>::tckind_value = CORBA::tk_sequence;

  //================================================================

  CORBA::TypeCode_ptr const
  BasicTypeTraits<CORBA::LongLongSeq>::tc_value = CORBA::_tc_LongLongSeq;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::LongLongSeq>::tckind_value = CORBA::tk_sequence;

  //================================================================

  CORBA::TypeCode_ptr const
  BasicTypeTraits<CORBA::ULongLongSeq>::tc_value = CORBA::_tc_ULongLongSeq;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::ULongLongSeq>::tckind_value = CORBA::tk_sequence;

  //================================================================

  CORBA::TypeCode_ptr const
  BasicTypeTraits<CORBA::FloatSeq>::tc_value = CORBA::_tc_FloatSeq;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::FloatSeq>::tckind_value = CORBA::tk_sequence;

  //================================================================

  CORBA::TypeCode_ptr const
  BasicTypeTraits<CORBA::DoubleSeq>::tc_value = CORBA::_tc_DoubleSeq;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::DoubleSeq>::tckind_value = CORBA::tk_sequence;

  //================================================================

  CORBA::TypeCode_ptr const
  BasicTypeTraits<CORBA::LongDoubleSeq>::tc_value = CORBA::_tc_LongDoubleSeq;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::LongDoubleSeq>::tckind_value = CORBA::tk_sequence;

  //================================================================

  CORBA::TypeCode_ptr const
  BasicTypeTraits<CORBA::AnySeq>::tc_value = CORBA::_tc_AnySeq;

  CORBA::TCKind const
  BasicTypeTraits<CORBA::AnySeq>::tckind_value = CORBA::tk_sequence;
}

TAO_END_VERSIONED_NAMESPACE_DECL


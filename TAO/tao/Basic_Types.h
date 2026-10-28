// -*- C++ -*-

//=============================================================================
/**
 *  @file    Basic_Types.h
 *
 *  @authors Jeff Parsons
 */
//=============================================================================


#ifndef TAO_BASIC_TYPES_H
#define TAO_BASIC_TYPES_H

#include /**/ "ace/pre.h"
#include "ace/CDR_Base.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  template <typename charT> class String_var;
  template <typename charT> class String_out;
}

namespace CORBA
{
  /**
   * @name CORBA Primitive Types
   *
   * Declarations of all CORBA primitive types.
   */
  //@{
  using Boolean = ACE_CDR::Boolean;
  using Boolean_out = Boolean &;

  using Octet = ACE_CDR::Octet;
  using Octet_out = Octet &;

  using Short = ACE_CDR::Short;
  using Short_out = Short &;

  using UShort = ACE_CDR::UShort;
  using UShort_out = UShort &;

  using Long = ACE_CDR::Long;
  using Long_out = Long &;

  using ULong = ACE_CDR::ULong;
  using ULong_out = ULong &;

  using LongLong = ACE_CDR::LongLong;
  using LongLong_out = LongLong &;

  using ULongLong = ACE_CDR::ULongLong;
  using ULongLong_out = ULongLong &;

  using Float = ACE_CDR::Float;
  using Float_out = Float &;

  using Double = ACE_CDR::Double;
  using Double_out = Double &;

  using LongDouble = ACE_CDR::LongDouble;
  using LongDouble_out = LongDouble &;

  using Char = ACE_CDR::Char;
  using Char_out = Char &;

  using WChar = ACE_CDR::WChar;
  using WChar_out = WChar &;

  using Int16 = ACE_CDR::Int16;
  using Int16_out = Int16 &;

  using UInt16 = ACE_CDR::UInt16;
  using UInt16_out = UInt16 &;

  using Int32 = ACE_CDR::Int32;
  using Int32_out = Int32 &;

  using UInt32 = ACE_CDR::UInt32;
  using UInt32_out = UInt32 &;

  using Int64 = ACE_CDR::Int64;
  using Int64_out = Int64 &;

  using UInt64 = ACE_CDR::UInt64;
  using UInt64_out = UInt64 &;
  //@}

  using String_var = TAO::String_var<CORBA::Char>;
  using String_out = TAO::String_out<CORBA::Char>;
  using WString_var = TAO::String_var<CORBA::WChar>;
  using WString_out = TAO::String_out<CORBA::WChar>;

  class TypeCode;
  using TypeCode_ptr = TypeCode *;

  class Any;
}

class TAO_InputCDR;
class TAO_OutputCDR;

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_BASIC_TYPES_H */

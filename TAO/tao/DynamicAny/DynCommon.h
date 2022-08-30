// -*- C++ -*-

//=============================================================================
/**
 *  @file    DynCommon.h
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_DYNCOMMON_H
#define TAO_DYNCOMMON_H
#include /**/ "ace/pre.h"

#include "tao/DynamicAny/DynamicAny.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_DynCommon
 *
 * Contains most of the functionality common to all the
 * Dynamic Any implementation classes.
 */
class TAO_DynamicAny_Export TAO_DynCommon
  : public virtual DynamicAny::DynAny
{
public:
  /// Constructor.
  TAO_DynCommon (CORBA::Boolean allow_truncation);

  /// Destructor.
  ~TAO_DynCommon () override;

  // = Some functions common to all Dynamic Any types.

  CORBA::TypeCode_ptr type () override;

  void assign (DynamicAny::DynAny_ptr dyn_any) override;

  void insert_boolean (CORBA::Boolean value) override;

  void insert_octet (CORBA::Octet value) override;

  void insert_char (CORBA::Char value) override;
  void insert_short (CORBA::Short value) override;
  void insert_ushort (CORBA::UShort value) override;

  void insert_long (CORBA::Long value) override;

  void insert_ulong (CORBA::ULong value) override;

  void insert_float (CORBA::Float value) override;

  void insert_double (CORBA::Double value) override;

  void insert_string (const char * value) override;

  void insert_reference (CORBA::Object_ptr value) override;

  void insert_typecode (CORBA::TypeCode_ptr value) override;

  void insert_longlong (CORBA::LongLong value) override;

  void insert_ulonglong (CORBA::ULongLong value) override;

  void insert_longdouble (CORBA::LongDouble value) override;

  void insert_wchar (CORBA::WChar value) override;

  void insert_wstring (const CORBA::WChar * value) override;

  void insert_any (const CORBA::Any & value) override;

  void insert_dyn_any (DynamicAny::DynAny_ptr value) override;

  void insert_val (CORBA::ValueBase * value) override;

  CORBA::Boolean get_boolean () override;

  CORBA::Octet get_octet () override;

  CORBA::Char get_char () override;

  CORBA::Short get_short () override;

  CORBA::UShort get_ushort () override;

  CORBA::Long get_long () override;

  CORBA::ULong get_ulong () override;

  CORBA::Float get_float () override;

  CORBA::Double get_double () override;

  char * get_string () override;

  CORBA::Object_ptr get_reference () override;

  CORBA::TypeCode_ptr get_typecode () override;

  CORBA::LongLong get_longlong () override;

  CORBA::ULongLong get_ulonglong () override;

  CORBA::LongDouble get_longdouble () override;

  CORBA::WChar get_wchar () override;

  CORBA::WChar * get_wstring () override;

  CORBA::Any * get_any () override;

  DynamicAny::DynAny_ptr get_dyn_any () override;

  CORBA::ValueBase * get_val () override;

  CORBA::Boolean seek (CORBA::Long index) override;

  void rewind () override;

  CORBA::Boolean next () override;

  DynamicAny::DynAny_ptr copy () override;

  CORBA::ULong component_count () override;

  void insert_abstract (CORBA::AbstractBase_ptr value) override;

  CORBA::AbstractBase_ptr get_abstract () override;

  void insert_boolean_seq (const CORBA::BooleanSeq & value) override;

  void insert_octet_seq (const CORBA::OctetSeq & value) override;

  void insert_char_seq (const CORBA::CharSeq & value) override;

  void insert_short_seq (const CORBA::ShortSeq & value) override;

  void insert_ushort_seq (const CORBA::UShortSeq & value) override;

  void insert_long_seq (const CORBA::LongSeq & value) override;

  void insert_ulong_seq (const CORBA::ULongSeq & value) override;

  void insert_float_seq (const CORBA::FloatSeq & value) override;

  void insert_double_seq (const CORBA::DoubleSeq & value) override;

  void insert_longlong_seq (const CORBA::LongLongSeq & value) override;

  void insert_ulonglong_seq (const CORBA::ULongLongSeq & value) override;

  void insert_longdouble_seq (const CORBA::LongDoubleSeq & value) override;

  void insert_wchar_seq (const CORBA::WCharSeq & value) override;

  CORBA::BooleanSeq * get_boolean_seq () override;

  CORBA::OctetSeq * get_octet_seq () override;

  CORBA::CharSeq * get_char_seq () override;

  CORBA::ShortSeq * get_short_seq () override;

  CORBA::UShortSeq * get_ushort_seq () override;

  CORBA::LongSeq * get_long_seq () override;

  CORBA::ULongSeq * get_ulong_seq () override;

  CORBA::FloatSeq * get_float_seq () override;

  CORBA::DoubleSeq * get_double_seq () override;

  CORBA::LongLongSeq * get_longlong_seq () override;

  CORBA::ULongLongSeq * get_ulonglong_seq () override;

  CORBA::LongDoubleSeq * get_longdouble_seq () override;

  CORBA::WCharSeq * get_wchar_seq () override;

  /// This sets one of two flags in this base class. CORBA 2.4.2
  /// requires that destroy() do nothing if it is called on
  /// the result of current_component(), the only non-deep-copying
  /// method in the Dynamic Any API. If the boolean arg below is 0,
  /// the call is from inside current_component(), and the flag
  /// ref_to_component_ is set. If the boolean arg is 1, the call
  /// is from inside destroy(), and the container_is_destroying_
  /// flag is set, overriding the first flag in the component's
  /// destroy() method.
  void set_flag (DynamicAny::DynAny_ptr component, CORBA::Boolean destroying);

  // Utility functions.

  DynamicAny::DynAny_ptr check_component (CORBA::Boolean isValueType = false);

  void check_type (CORBA::TypeCode_ptr tc);

  static bool is_basic_type_seq (CORBA::TypeCode_ptr tc);

  // Accessors

  CORBA::Boolean has_components () const;
  CORBA::Boolean destroyed () const;
  CORBA::Any &the_any ();

  // Mutators

  void container_is_destroying (CORBA::Boolean val);
  void ref_to_component (CORBA::Boolean val);

protected:
  /// Were we created by current_component()?
  CORBA::Boolean ref_to_component_;

  /// Flag to let destroy() call by container override the flag above.
  CORBA::Boolean container_is_destroying_;

  /// Do we contain other Dynamic Anys?
  CORBA::Boolean has_components_;

  /// Has destroy() been called on us yet?
  CORBA::Boolean destroyed_;

  /// Slot of the current component (if any).
  CORBA::Long current_position_;

  /// Number of components, as defined by CORBA 2.4.2.
  CORBA::ULong component_count_;

  /// Holder for our type code.
  CORBA::TypeCode_var type_;

  /// Gets a value only for basic types, but used in insert_*
  /// and get_*, defined in this base class.
  CORBA::Any any_;

  /// Are we allowed to truncate any valuetypes in our hyarchy?
  CORBA::Boolean allow_truncation_;

private:
  // Utility functions used by insert_* and get_*.
  CORBA::TypeCode_ptr check_type_and_unalias (CORBA::TypeCode_ptr tc);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_DYNCOMMON_H */
